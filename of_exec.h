#ifndef _OF_EXEC_H
#define _OF_EXEC_H

#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

extern const char* ovs_path;
extern char br_name[11];
extern char mac[256];

#define LABEL_MASK 0x0FFFF
#define LABEL_MASK_VLL 0X080000
#define LABEL_MASK_PW 0x090000 
#define MPLS_UNICAST 0x8847
#define MPLS_MULTICAST 0x8848
#define MAX_PRIORITY 32768
#define TABLE_SBP 1 

typedef struct flow_data{
	in_addr_t ip;
	in_addr_t gateway_ip;
	unsigned short int port;
} flow_data;


void of_add_flow ( flow_data *fd );

void of_del_flow ( flow_data *fd );

static inline in_addr_t ip_to_label_VLL ( in_addr_t ip ){
	return  (uint32_t)(((uint32_t)ip & LABEL_MASK) | LABEL_MASK_VLL);
}

static inline in_addr_t ip_to_label_PW ( in_addr_t ip ){
	return  (uint32_t)(((uint32_t)ip & LABEL_MASK) | LABEL_MASK_PW);
}

static inline void loopback_to_mac( const char* loopback, char* mac_out){
	char mac_temp[256];	
	sprintf(mac_temp, "0200%x", ntohl(inet_addr(loopback)));
	sprintf(mac_out, "%c%c:%c%c:%c%c:%c%c:%c%c:%c%c", mac_temp[0], mac_temp[1], mac_temp[2], mac_temp[3], mac_temp[4], mac_temp[5],
	mac_temp[6], mac_temp[7], mac_temp[8], mac_temp[9], mac_temp[10], mac_temp[11]);
	
}

void of_bootstrap_flow(char* loopback);



#endif /* _OF_EXEC_H */
