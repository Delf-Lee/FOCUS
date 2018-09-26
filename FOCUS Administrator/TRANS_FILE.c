#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "Admin.h"
#include "Thread.h"

#pragma comment (lib, "ws2_32.lib")

#define FTP_BUFSIZE 500000
#define FTP_PORT 5456
#define CLIENT_SIZE 256

extern int flag_ftp;
extern int flag_ftp1;
extern long FILE_SIZE;

typedef struct Clnt {
	SOCKET socket;
	int flag;
	long seek;
}Clnt;

Clnt FTPCLNT[CLIENT_SIZE];

WSADATA FTP_wsaData;
SOCKET ListenSocket;

int maxfd1;
int Client_num = 0;

SOCKADDR_IN SockAdr_in;
int SockAdr_in_size = sizeof(SOCKADDR_IN);

char FTP_buf[FTP_BUFSIZE + 4];

int getmax(int k) {
	int max = k;
	int r;
	for (r = 0; r < Client_num; r++) {
		if (FTPCLNT[r].socket > max) max = FTPCLNT[r].socket;
	}
	return max;
}

//꼭! 프로그램 실행 파일이 저장된 위치에 있어야 한다.
UINT WINAPI TRANS_FILE(void *arg) {
	int i, j, k;
	int count = 0;
	int delay_count = 0;
	unsigned int length_recv;
	unsigned int *p;
	long end;
	unsigned int option_value = 1;
	unsigned int read_size;
	fd_set read_fds;
	SOCKET MAX;

	FILE *FilePointer;
	struct timeval val;

	val.tv_sec = 0;
	val.tv_usec = 100;

	Client_num = 0;
	memset(FTPCLNT, NULL, sizeof(Clnt)*CLIENT_SIZE);

	FilePointer = fopen(filename, "rb"); //전송할 파일을 읽기 위한 fp

	if (FilePointer == NULL) {
		printf("FTP: FILE OPEN ERROR\n");
		flag_ftp = TRUE;
		flag_ftp1 = FALSE;
		return;
	}

	fseek(FilePointer, 0, SEEK_END);
	end = ftell(FilePointer);

	if (WSAStartup(MAKEWORD(2, 2), &FTP_wsaData) != 0) {
		printf("FTP: WSASTARTUP ERROR\n");
		fclose(FilePointer);
		flag_ftp = TRUE;
		return;
	}

	ListenSocket = socket(PF_INET, SOCK_STREAM, 0);

	setsockopt(ListenSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&option_value, sizeof(option_value));

	memset(&SockAdr_in, 0, sizeof(SockAdr_in));
	SockAdr_in.sin_family = AF_INET;
	SockAdr_in.sin_addr.s_addr = htonl(INADDR_ANY);
	SockAdr_in.sin_port = htons(FTP_PORT);

	if (bind(ListenSocket, (SOCKADDR*)&SockAdr_in, sizeof(SockAdr_in)) == SOCKET_ERROR) {
		printf("FTP: BIND ERROR\n");
		Sleep(3000);
		WSACleanup();
		return 0;
	}

	if (listen(ListenSocket, CLIENT_SIZE) == SOCKET_ERROR) {
		printf("FTP: LISTEN ERROR\n");
		Sleep(3000);
		WSACleanup();
		return 0;
	}

	flag_ftp1 = FALSE; // 원래 쓰레드 해제

	MAX = ListenSocket + 1;

	while (1) {
		FD_ZERO(&read_fds);

		FD_SET(ListenSocket, &read_fds);

		for (i = 0; i < Client_num; i++) {
			FD_SET(FTPCLNT[i].socket, &read_fds);
		}

		MAX = getmax(ListenSocket) + 1;

		if (select(MAX, &read_fds, 0, 0, &val) < 0) {
			printf("select error \n");
		}

		if (FD_ISSET(ListenSocket, &read_fds)) {
			FTPCLNT[Client_num].socket = accept(ListenSocket, (SOCKADDR*)&SockAdr_in, &SockAdr_in_size);

			if (FTPCLNT[Client_num].socket == INVALID_SOCKET) {
				printf("FTP: ACCEPT ERROR\n");
				closesocket(FTPCLNT[Client_num].socket);
				closesocket(ListenSocket);
			}

			//accept가 정상이면
			FTPCLNT[Client_num].flag = TRUE;
			FTPCLNT[Client_num].seek = 0;
			Client_num++;
			continue;
		}

		for (j = 0; j < Client_num; j++) {
			if (FTPCLNT[j].flag == FALSE) {
				count++;
				// 종료 조건
				if (count == Client_num) {
					for (k = 0; k < Client_num; k++) {
						shutdown(FTPCLNT[k].socket, 1);
					}
					closesocket(ListenSocket);
					_fcloseall();
					WSACleanup();
					printf("FTP:FILE SEND COMPLETE\n");
					flag_ftp = TRUE;
					return 0;
				}
				continue;
			}
			// 종료 조건

			if (FTPCLNT[j].flag) {
				memset(FTP_buf, NULL, sizeof(FTP_buf));

				fseek(FilePointer, FTPCLNT[j].seek, SEEK_SET);
				read_size = fread(FTP_buf, sizeof(char), FTP_BUFSIZE, FilePointer);
				FTPCLNT[j].seek = ftell(FilePointer);

				if (send(FTPCLNT[j].socket, FTP_buf, read_size, 0) == SOCKET_ERROR) {
					printf("FTP: FILE SEND ERROR\n");
					closesocket(FTPCLNT[Client_num].socket);
					FTPCLNT[j].flag = FALSE;
					break;
				}

				if (feof(FilePointer)) {
					FTPCLNT[j].flag = FALSE;
				}
			}
			delay_count = 0;
		}

		if (delay_count > 10000) {
			for (k = 0; k < Client_num; k++) {
				closesocket(FTPCLNT[k].socket);
			}
			_fcloseall();
			flag_ftp = TRUE;
			closesocket(ListenSocket);
			WSACleanup();
			return 0;
		}
		delay_count++;
		count = 0;
	}

	flag_ftp = TRUE;
	closesocket(ListenSocket);
	WSACleanup();
	return 0;
}