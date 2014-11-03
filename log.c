#include <time.h>
#include "log.h"
#include "of_exec.h"
#include "global.h"

const char* log_prefix = "logs_";
const char* log_postfix = ".txt";
int log_level = 1;
const char* fpm_path = "/fpm-of/";
FILE* log_fp = 0;

void log_to_file(int print_level, const char* pattern, ...){

	if(print_level <= log_level){

		char file1[100];
		char file2[100];

		const char* p;
		va_list argp;
		int support1;
		const char* support2;
		char fmtbuf[256];


		if( ! log_fp ){
			sprintf( file1, "%s%s%s%s%d%s", prefix_path, br_name, fpm_path, log_prefix, 0, log_postfix);		
		    log_fp = fopen( file1, "a" );
		}

		if( log_fp ){
		    if( ftell( log_fp ) > MAX_LOG_SIZE ){
		        fclose( log_fp );
		        log_fp = 0;
			
				int i;
		        for( i = (MAX_LOG_FILE - 1); i >= 0; i-- ){
		            sprintf( file1, "%s%s%s%s%d%s", prefix_path, br_name, fpm_path, log_prefix, i, log_postfix );
		            sprintf( file1, "%s%s%s%s%d%s", prefix_path, br_name, fpm_path, log_prefix, i+1, log_postfix );
		            rename( file1, file2 );
		        }

		        sprintf( file1, "%s%s%s%s%d%s", prefix_path, br_name, fpm_path, log_prefix, 0, log_postfix);
		        log_fp = fopen( file1, "a" );
		    }

			time_t ltime;
			ltime=time(NULL);
			memset(fmtbuf, 0, sizeof(fmtbuf));
			char * time = asctime(localtime(&ltime));
			time[strlen(time)-1] = '\0';
			snprintf(fmtbuf, sizeof(fmtbuf), "[%s] - ", time);
			support2 = fmtbuf;
			fputs(support2, log_fp);

			va_start(argp, pattern);

			for(p = pattern; *p != '\0'; p++){

				if(*p != '%'){
					fputc(*p, log_fp);
					continue;
				}

				switch(*++p){
					case 'c':
						support1 = va_arg(argp, int);
						fputc(support1, log_fp);
						break;

					case 'd':
						support1 = va_arg(argp, int);
						memset(fmtbuf, 0, sizeof(fmtbuf));
						snprintf (fmtbuf, sizeof(fmtbuf), "%d", support1);
						support2 = fmtbuf;
						fputs(support2, log_fp);
						break;

					case 's':
						support2 = va_arg(argp, const char*);
						fputs(support2, log_fp);
						break;

					case 'x':
						support1 = va_arg(argp, int);
						memset(fmtbuf, 0, sizeof(fmtbuf));
						snprintf (fmtbuf, sizeof(fmtbuf), "%x", support1);
						support2 = fmtbuf;
						fputs(support2, log_fp);
						break;

					case '%':
						fputc('%', log_fp);
						break;
				}
			}

			va_end(argp);
			fflush(log_fp);
		}
	}
}









