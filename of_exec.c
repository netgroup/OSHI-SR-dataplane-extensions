#include <stdlib.h>
#include <stdio.h>
#include "of_exec.h"



void of_add_flow( const char *addr, const char *gateway, int ifindex ){
	char cmd[255];
	sprintf(cmd,"sudo ovs-ofctl add-flow %s hard_timeout=0,priority=401,in_port=%d,actions=drop", BRIDGE_NAME, ifindex);
	printf("%s\n",cmd);
	system(cmd);
}
