#ifndef _OF_EXEC_H
#define _OF_EXEC_H

#include <netinet/in.h>

#define BRIDGE_NAME "cro1"

#define LABEL_MASK 0xFFFFC000 

typedef struct flow_data
{
  in_addr_t ip;
  in_addr_t label;
  in_addr_t gateway_ip;
  unsigned short int port; 

} flow_data;


void of_add_flow ( flow_data *fd );

void of_del_flow ( flow_data *fd );

static inline in_addr_t ip_to_label ( in_addr_t ip )
{
	return  ip & LABEL_MASK;
}

#endif /* _OF_EXEC_H */
