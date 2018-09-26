#ifndef _THREAD_
#define _THREAD_

#include "Admin.h"
#include <windows.h>
#include <process.h>

// 명령종류에 따른 data type 정의
#define ACK				100 // ACK
#define SHUTDOWN		101 // 전원 종료
#define SHUTDOWN_T		201	// 전원 종료 (타이머)
#define SLEEP_MODE		102 // 절전 모드
#define SLEEP_MODE_T	202 // 절전 모드 (타이머)
#define SEND_MESSAGE	103 // 메세지 전송
#define RUN_WEBBROWSER	104 // 웹 브라우저 실행
#define BLOCK_INTERNET	105 // 인터넷 차단
#define FREE_INTBLOCK   205	// 인터넷 차단 해제
#define FTP				106 // 파일 전송

UINT WINAPI SendCommand_Thread(void *arg);	// 명령 보내기 스레드
UINT WINAPI ManageList(void *arg);			// 클라이언트 관리
UINT WINAPI TRANS_FILE(void *arg);			// FTP 만들기
UINT WINAPI ARP(void *arg);

char filename[1024]; // 클라리언트에게 전송할 파일명
char first; // UI 제어에 대한변수 
char reset; // UI 제어에 대한변수 

void ShutDown(char time[], int n, int select[], int flag);		// 전원 종료
void SleepMode(char time[], int n, int select[], int flag);		// 절전 모드
void MessageSend(int n, int select[]);							// 메세지 전송
void RunWebBrowser(int n, int select[]);						// 웹 브라우저 실행
void BlockInternet(int n, int select[], int flag);				// 인터넷 차단
void FileTranse(int n, int select[]);							// 파일 전송

#endif
