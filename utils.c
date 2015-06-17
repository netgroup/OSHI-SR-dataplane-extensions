#include "utils.h"
#include "global.h"
#include "of_exec.h"
#include "log.h"

int get_if_number(char* if_name){
	char *p = if_name + (strlen(if_name))-1;
	char *end = if_name;
	char *endnumber;
	int find = 0;
	while(p!=end){
		if (isdigit(*p)){
			find = find + 1;
			p--;
		}
		else{
			if(find == 0) 
				p--;
			else{
				endnumber = (p + find);
				p = p + 1;
				int val = (int) strtol(p, &endnumber, 10);
				return val;
			}
		}

	}

	if (isdigit(*p)){
		endnumber = (p + find);
		int val = (int) strtol(p, &endnumber, 10);
		return val;
	}
	else{
		if(find != 0){
			endnumber = (p + find);
			p = p + 1;
			int val = (int) strtol(p, &endnumber, 10);
			return val;
		}	
	}
}

int get_port_number(char* if_name){
	FILE *fp;
	char* p;
	char buf[256];
	char cmd[256];
	memset(buf, 0, sizeof(buf));
	memset(cmd, 0, sizeof(cmd));
	char sock[256];
	if(strcmp(br_name, "br-dreamer") == 0)
		sprintf(sock, "%s", "");
	else
		sprintf(sock, "%s%s%s", prefix_path, br_name, ovs_path);
	sprintf(cmd, "sudo ovs-vsctl --db=unix:%s/db.sock find Interface name=%s | grep ofport | awk -F\':\' \'{print $2}\' | awk \'{ gsub (\" \", \"\", $0); print}\'", sock, if_name);
	fp = popen(cmd, "r");
	if (fp == NULL){
    	log_to_file(-1, "Error unable to run find interface program\n");
		exit(1);
	}
	fgets(buf, 256, fp);
	pclose(fp);
	p = buf;
	int val = (int) strtol(p, &(p), 10);
	return val;
}

void get_loopback_address(char * loopback){
	FILE *fp;
	char cmd[256];
	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd, "ip -4 addr show dev lo | grep -m 2 \"inet \" | awk \'{print $2}\' | cut -d \"/\" -f 1");
	sprintf(loopback, "%s", "127.0.0.1");
	while(strcmp(loopback, "127.0.0.1") == 0){
		fp = popen(cmd, "r");
		if (fp == NULL){
			log_to_file(-1, "Error unable to run ip addr program\n");
			exit(1);
		}
		while (fgets(loopback, 256, fp) != NULL);
		loopback[strlen(loopback)-1] = '\0';
		pclose(fp);
	}
	pclose(fp);
}

char * json_fetch(char *file){

	char ch;
	FILE *fp;
	fp = fopen(file,"r"); 
	if( fp == NULL ){
		log_to_file(-1, "Error while opening the file.\n");
		exit(1);
	}
	fseek(fp, 0L, SEEK_END);
	int sz = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	char* data = malloc((sizeof(char))*sz + 1);
	char* ret = data;
	while( ( ch = fgetc(fp) ) != EOF ){
		*data = ch;
		data = data + 1;
	}
	*data = '\0';
	fclose(fp);
	return ret;

}

jsmntok_t * json_tokenise(char *js){
    jsmn_parser parser;
    jsmn_init(&parser);

    unsigned int n = JSON_TOKENS;
    jsmntok_t *tokens = malloc(sizeof(jsmntok_t) * n);

    int ret = jsmn_parse(&parser, js, tokens, n);

    while (ret == JSMN_ERROR_NOMEM){
        n = n * 2 + 1;
        tokens = realloc(tokens, sizeof(jsmntok_t) * n);
        ret = jsmn_parse(&parser, js, tokens, n);
    }

    if (ret == JSMN_ERROR_INVAL){
		log_to_file(-1, "Error invalid JSON string\n");
		exit(EXIT_FAILURE);
	}
    if (ret == JSMN_ERROR_PART){
		log_to_file(-1, "Error truncated JSON string\n");
		exit(EXIT_FAILURE);
	}
    return tokens;
}

bool json_token_streq(char *js, jsmntok_t *t, char *s){
    return (strncmp(js + t->start, s, t->end - t->start) == 0
            && strlen(s) == (size_t) (t->end - t->start));
}

char * json_token_tostr(char *js, jsmntok_t *t){
    js[t->end] = '\0';
    return js + t->start;
}

char IN_FILE[256]="";
mapping* maps = NULL;

void load_map(){

	mapping *s, *tmp = NULL;

	sprintf(IN_FILE, "%s%s", prefix_path, "ip_to_mac.cfg");	

	char* data = json_fetch(IN_FILE);

	jsmntok_t *tokens = json_tokenise(data);

	typedef enum { START, KEY, STORE, STOP } parse_state;
    parse_state state = START;

    size_t object_tokens = 0;

	size_t i,j;
    for (i = 0, j = 1; j > 0; i++, j--){
        jsmntok_t *t = &tokens[i];

        if(!(t->start != -1 && t->end != -1)){
			log_to_file(-1, "Error uninitialized json tokens\n");
			exit(1);
		}

        if (t->type == JSMN_ARRAY || t->type == JSMN_OBJECT)
            j += t->size;

        switch (state){
            case START:
                if (t->type != JSMN_OBJECT){
					log_to_file(-1, "Error root element must be an object\n");
					exit(1);
				}

                state = KEY;
                object_tokens = t->size;

                if (object_tokens == 0)
                    state = STOP;

                if (object_tokens % 2 != 0){
					log_to_file(-1, "Error object must have even number of children\n");
					exit(1);
				}

                break;

            case KEY:
                object_tokens--;

                if (t->type != JSMN_STRING){
                    log_to_file(-1, "Error key must be a string\n");
					exit(1);
				}

                state = STORE;

                char *key = json_token_tostr(data, t);

                break;

            case STORE:
                if (t->type != JSMN_STRING){
                    log_to_file(-1, "Error value must be a string\n");
					exit(1);
				}

                char *str = json_token_tostr(data, t);
                s = (mapping*)malloc(sizeof(mapping));
        		strcpy(s->ip, key);
        		strcpy(s->mac, str);
        		HASH_ADD_STR( maps, ip, s );

                object_tokens--;
                state = KEY;

                if (object_tokens == 0)
                    state = STOP;

                break;

            case STOP:
                break;

            default:
                log_to_file(-1, "invalid JSON state\n");
				exit(1);
        
		}    	
	}
}
