#ifndef _OF_EXEC_H
#define _OF_EXEC_H

#define BRIDGE_NAME "br-one"


void of_add_flow( const char *addr, const char *gateway, int ifindex );

// void of_del_flow( char *addr, char *gateway, int ifindex );

#endif /* _OF_EXEC_H */
