#ifndef _UTILS_H
#define _UTILS_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include "jsmn.h"
#include "uthash.h"

#define JSON_TOKENS 256

typedef struct mapping {
    char ip[16];             /* key (string is WITHIN the structure) */
    char mac[18];
    UT_hash_handle hh;         /* makes this structure hashable */
} mapping;

extern mapping* maps;
extern char IN_FILE[256];

int get_if_number(char* if_name);
int get_port_number(char* if_name);
void get_loopback_address(char* loopback);
char * json_fetch(char *file);
jsmntok_t * json_tokenise(char *js);
bool json_token_streq(char *js, jsmntok_t *t, char *s);
char * json_token_tostr(char *js, jsmntok_t *t);

void load_map();

#endif /* _UTILS_H */
