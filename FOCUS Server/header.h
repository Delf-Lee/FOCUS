#pragma warning(disable 4996)
#ifndef _HEADER_
#define _HEADER_

#pragma comment(lib,"ws2_32.lib")
#define _CRT_SECURE_NO_WARNINGS

// include
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <winsock.h>
#include <process.h> //스레드
#include <windows.h> //스레드
#include <signal.h> // 스트림 관련 file디스크립터


// define
#define TRUE 1
#define FALSE 0
#define FREQUENT 1

#define PORT 5454

//ID
#define SERVER 1
#define ADMINISTRATOR 2
#define CLIENT 3


//Data Type
#define ADMINISTRATOR_STATE 1
#define ADMINISTRATOR_LOGIN 2
#define CLIENT_STATE 3
#define ADMINISTRATOR_REQUEST_LIST 4
#define ADMINISTRATOR_ADDR 5

//Data Size
#define BUFSIZE 1024
#define CLIENT_SIZE 256
#define ADMINISTRATOR_SIZE 10
#define HOST_SIZE 266

//typedef
typedef time_t TIME;

typedef struct S_Temp {
	unsigned char IP_ADDR[30];	
	unsigned int IP_ADDR_INT;	//30
	SOCKET Socket;			//4
}Temp;

typedef struct S_State_Admin {
	unsigned char IP_ADDR[30];		//30
	unsigned char MAC_ADDR[30];		//30
	unsigned char LOGIN;
	unsigned int ADDR_S;
	unsigned int ADDR_E;
	SOCKET Socket;			//4	
	TIME Time;				//8
}State_A; // 81

typedef struct S_State_Client {
	unsigned char IP_ADDR[30];		//30
	unsigned char MAC_ADDR[30];	
	unsigned int IP_ADDR_INT;	//30
	SOCKET Socket;			//4	
	TIME Time;				//8
}State_C;

typedef struct S_Table {
	State_A **list;
	int howmany;
	int size;
}Table;



// Function Header
int Table_Init(Table *table);
int Table_Add(Table *table, State_A *admin);
int Table_Delete(Table *table, State_A *admin);



unsigned char Login(const char *Id, const char *Pwd);
unsigned int IP2INT(char *addr);

int send_MY(SOCKET s, const char *data, const int size, const int id, const int data_type, int flags);
int Show_User(State_A *list, int howmany);

SOCKET Get_Max_Sock(const FD_SET *Socket_Arr, int k, int size);


#endif