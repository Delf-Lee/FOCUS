#include "header.h"

unsigned int IP2INT(char *addr) {
	char Addr[512] = { 0, };
	unsigned char ret[4] = { 0, };
	unsigned char tmp;
	unsigned char *p1;
	unsigned int RET;

	memcpy(Addr, addr, sizeof(char) * 30);

	p1 = strtok(Addr, ".");
	tmp = atoi(p1);
	ret[0] = tmp;

	p1 = strtok(NULL, ".");
	tmp = atoi(p1);
	ret[1] = tmp;

	p1 = strtok(NULL, ".");
	tmp = atoi(p1);
	ret[2] = tmp;

	p1 = strtok(NULL, ".");
	tmp = atoi(p1);
	ret[3] = tmp;

	memcpy(&RET, ret, 4);

	return RET;
}