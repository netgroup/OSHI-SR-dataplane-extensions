fpm-of : of_exec.c fpm_of.c of_exec.h log.c log.h utils.c utils.h global.c global.h jsmn.c jsmn.h uthash.h
	gcc  -o $@.bin -g of_exec.c fpm_of.c of_exec.h log.c log.h utils.c utils.h global.c global.h jsmn.c jsmn.h uthash.h

clean :
	@-rm -f fpm-of
