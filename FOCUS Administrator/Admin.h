#define _WINSOCK_DEPRECATED_NO_WARNINGS

#ifndef _HEADER_
#define _HEADER_

#pragma comment(lib,"ws2_32.lib") //lib 파일 불러옴
#pragma comment(lib, "iphlpapi.lib" )

#include <winsock2.h>
#include <IPHlpApi.h> 

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include <signal.h> // 스트림 관련 file 디스크립터
#include <time.h>

#define TRUE 1
#define FALSE 0

#define BUFSIZE 1024
#define CLIENT_SIZE 256
#define ADMINISTRATOR_SIZE 10
#define HOST_SIZE 266
#define SERVER_IP "테스트 프로그램"
#define PORT 5454

// ID
#define SERVER 1
#define ADMINISTRATOR 2
#define CLIENT 3

// Data Type
#define ADMINISTRATOR_STATE 1
#define ADMINISTRATOR_LOGIN 2
#define CLIENT_STATE 3
#define ADMINISTRATOR_REQUEST_LIST 4

// size
#define ADMINISTRATOR_STATE_SIZE 72
#define CLIENT_STATE_SIZE 72
#define HEADER_SIZE sizeof(int)*3

#define FREQUENT 60 // 최대 연결 지연 시간

WSADATA wsadata;

SOCKADDR_IN sockaddr_in;
int sockaddr_size;

SOCKET Main_Socket;
SOCKET Server_Socket;
SOCKET Max_Socket;

HANDLE  hThread;
DWORD   dwThreadID;

fd_set  Socket_Arr;

typedef time_t TIME;

typedef struct S_State {
	char IP_ADDR[30];      //30
	char MAC_ADDR[30];      //30
	SOCKET Socket;         //4
	TIME Time;
} State;

State s;
State Client_List[CLIENT_SIZE];

extern int howmany_C;   // 관리 중인 클라이언트 수
extern int sel_clt_cnt; // 선택된 클라언트의 '수'
extern int LOGIN; // 로그인 재시도
extern int SERVER_LIVE;
extern int bootThread;
char adminID[16];
int Send_Packet(SOCKET s, const char *data, const int size, const int id, const int data_type, int flags);
int LoginReq(SOCKET Socket, char *ID_SHA, char *PWD_SHA);
int LoginTry();
char *getMAC();

#endif

