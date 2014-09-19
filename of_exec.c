#include <stdlib.h>
#include <stdio.h>
#include "of_exec.h"



void of_add_flow( flow_data *fd ){
	char cmd[255];
	sprintf(cmd,"sudo ovs-ofctl add-flow %s hard_timeout=0,priority=401,in_port=2,actions=drop", BRIDGE_NAME);
	printf("%s\n",cmd);
	system(cmd);
}


void of_del_flow( flow_data *fd ){
	char cmd[255];
	sprintf(cmd,"sudo ovs-ofctl del-flow %s hard_timeout=0,priority=401,in_port=2,actions=drop", BRIDGE_NAME);
	printf("%s\n",cmd);
	system(cmd);
}