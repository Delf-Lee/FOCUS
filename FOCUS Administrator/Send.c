#include "Admin.h"
/*
 s			: 수신할 호스트의 소켓
 data		: 전송할 데이터
 id			: 전송자의 정보
 data_type  : 전솔할 데이터의 종류
*/

int Send_Packet(SOCKET s, const char *data, const int size, const int id, const int data_type, int flags) {
	int ret;
	int Header[3];
	char *buf;

	Header[0] = size;
	Header[1] = id;
	Header[2] = data_type;

	buf = (char *)malloc(size + sizeof(Header));
	memset(buf, NULL, sizeof(buf));
	memcpy(buf, Header, sizeof(Header));
	memcpy(buf + 12, data, size);

	ret = send(s, buf, sizeof(Header) + size, flags);

	free(buf);

	return ret;
}