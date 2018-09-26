#include "header.h"

int send_MY(SOCKET s, const char *data, const int size, const int id, const int data_type, int flags) {
	int ret;
	int info[3];
	char *buf;

	info[0] = size;
	info[1] = id;
	info[2] = data_type;

	buf = (char *)malloc(size + sizeof(info));

	memset(buf, NULL, sizeof(buf));
	memcpy(buf, info, sizeof(info));
	memcpy(buf + 12, data, size);

	ret = send(s, buf, sizeof(info) + size, flags);
	free(buf);

	return ret;
}
