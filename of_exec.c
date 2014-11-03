#include <stdlib.h>
#include <stdio.h>
#include "log.h"
#include "of_exec.h"
#include "global.h"
#include "utils.h"

char br_name[11] = "";
char mac[256] = "";
const char* ovs_path = "/ovs/";

void of_add_flow( flow_data *fd ){
	char cmd[255];
	sprintf(cmd, "sudo ovs-ofctl -O OpenFlow13 add-flow %s \"table=%d,hard_timeout=0,priority=%d,eth_type=%d,mpls_label=%u,actions=output:%d\"", br_name, TABLE_SBP, MAX_PRIORITY, MPLS_UNICAST, ip_to_label_VLL(fd->ip), fd->port);
	system(cmd);
	log_to_file(0, "%s\n", cmd);
	sprintf(cmd, "sudo ovs-ofctl -O OpenFlow13 add-flow %s \"table=%d,hard_timeout=0,priority=%d,eth_type=%d,mpls_label=%u,actions=output:%d\"", br_name, TABLE_SBP, MAX_PRIORITY, MPLS_MULTICAST, ip_to_label_VLL(fd->ip), fd->port);
	system(cmd);
	log_to_file(0, "%s\n", cmd);

	struct in_addr gateway_addr;
	gateway_addr.s_addr = htonl(fd->gateway_ip);
	mapping* s = NULL;
	HASH_FIND_STR( maps, inet_ntoa(gateway_addr), s);
    if (s){
		sprintf(cmd, "sudo ovs-ofctl -O OpenFlow13 add-flow %s \"table=%d,hard_timeout=0,priority=%d,dl_dst=%s,eth_type=%d,mpls_label=%u,actions=set_field:%s->dl_dst,output:%d\"", br_name, TABLE_SBP, MAX_PRIORITY, mac, MPLS_UNICAST, ip_to_label_PW(fd->ip), s->mac, fd->port);
		system(cmd);
		log_to_file(0, "%s\n", cmd);
	} else{
		log_to_file(-1, "Error No Mapping Gateway %s To Mac\n", inet_ntoa(gateway_addr));
		exit(1);
	}
}

void of_del_flow( flow_data *fd ){
	char cmd[255];
	sprintf(cmd, "sudo ovs-ofctl -O OpenFlow13 del-flows %s \"table=%d,eth_type=%d,mpls_label=%u\"", br_name, TABLE_SBP, MPLS_UNICAST, ip_to_label_VLL
	(fd->ip));
	system(cmd);
	log_to_file(0, "%s\n", cmd);
	sprintf(cmd, "sudo ovs-ofctl -O OpenFlow13 del-flows %s \"table=%d,eth_type=%d,mpls_label=%u\"", br_name, TABLE_SBP, MPLS_MULTICAST,	
	ip_to_label_VLL(fd->ip));
	system(cmd);
	log_to_file(0, "%s\n", cmd);
	sprintf(cmd, "sudo ovs-ofctl -O OpenFlow13 del-flows %s \"table=%d,dl_dst=%s,eth_type=%d,mpls_label=%u\"", br_name, TABLE_SBP, mac, MPLS_UNICAST, 
	ip_to_label_PW(fd->ip));
	system(cmd);
	log_to_file(0, "%s\n", cmd);
}

void of_bootstrap_flow(char* loopback){
	char cmd[255];
	in_addr_t loopback_addr = ntohl(inet_addr(loopback));
	sprintf(cmd, "sudo ovs-ofctl -O OpenFlow13 add-flow %s \"table=%d,hard_timeout=0,priority=%d,eth_type=%d,mpls_label=%u,actions=pop_mpls:%d,resubmit(,%d)\"", br_name, TABLE_SBP, MAX_PRIORITY, MPLS_UNICAST, ip_to_label_VLL(loopback_addr), MPLS_UNICAST, TABLE_SBP);
	system(cmd);
	log_to_file(1,"%s\n", cmd);
	sprintf(cmd, "sudo ovs-ofctl -O OpenFlow13 add-flow %s \"table=%d,hard_timeout=0,priority=%d,eth_type=%d,mpls_label=%u,actions=pop_mpls:%d,resubmit(,%d)\"", br_name, TABLE_SBP, MAX_PRIORITY, MPLS_MULTICAST, ip_to_label_VLL(loopback_addr), MPLS_MULTICAST, TABLE_SBP);
	system(cmd);	
	log_to_file(1,"%s\n", cmd);
	sprintf(cmd, "sudo ovs-ofctl -O OpenFlow13 add-flow %s \"table=%d,hard_timeout=0,priority=%d,dl_dst=%s,eth_type=%d,mpls_label=%u,actions=pop_mpls:%d,resubmit(,%d)\"", br_name, TABLE_SBP, MAX_PRIORITY, mac, MPLS_UNICAST, ip_to_label_PW(loopback_addr), MPLS_UNICAST, TABLE_SBP);
	system(cmd);	
	log_to_file(1,"%s\n", cmd);
}
