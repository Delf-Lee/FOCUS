#ifndef _UI_H_
#define _UI_H_
#pragma warning(disable:4996)

#include <conio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>

#define VER "4.0"

#define BLACK 0
#define BLUE1 1
#define GREEN1 2
#define CYAN1 3
#define RED1 4
#define MAGENTA1 5
#define YELLOW1 6
#define GRAY1 7
#define GRAY2 8
#define BLUE2 9
#define GREEN2 10
#define CYAN2 11
#define RED2 12
#define MAGENTA2 13
#define YELLOW2 14
#define WHITE 15

#define ESC		0x1b
#define SPACE	0x20
#define ENTER	0x0D
#define TAB		0x09
#define BACKSPACE 0x08

#define SPECIAL1 0xe0
#define SPECIAL2 0x00

#define UP		0x48
#define DOWN	0x50
#define LEFT	0x4b
#define RIGHT	0x4d

#define LINE 17

#define SET_ROOM 0
#define SET_IP 99


char send_time[5];
char send_web[128];
char login_error_window;

// 현재 시간
time_t time_now;
struct tm *timeinfo;

// UI 배치, 배색을 위한 함수
void gotoxy(int x, int y);
void VisibleCursor(int flag);
void textcolor(int fg_color, int bg_color);
void cls(int bg_color, int text_color);

// 로고 출력
void ShowFOCUS(int x, int y, char* ver);

// 로그인 UI
void ShowLogin(int y, char* ver);
int ShowRangeSetBox(int x, int y);

// 메인화면 UI - 클라이언트 리스트
int ShowMainScr(int x, int y, int select[]);
void PrintList(int x, int y, int n, int arrow, int page);
void WipeList(int x, int y);
void ShowCltInfo(int x, int y, int clt);

// 메인화면 UI - 명령 메뉴
int ShowCmdBox(int x, int y);
void CmdBox(int x, int y);
int ShowCmdMenu(int x, int y, int arrow);
void ClrCmdBox();

// 명령 메뉴 (상세)
int ShutDownBox(int x, int y);
int SleepModeBox(int x, int y);
char* InputTime(int x, int y, int flag);
int RunWebBrowserBox(int x, int y);
int BlockInternetBox(int x, int y);

// 화살표 및 스크롤
void ShowArrow(int x, int y, int arrow);
void WipeArrow(int x, int y, int arrow);
void ScrollCursor(int x, int y, int arrow);

// 기타 정보 출력
void ShowAdminInfo(int x, int y); // 관리자 정보
void ShowServerInfo(int x, int y); // 서버 정보
void ShowPresentTime(); // 현재 시간

// 확인 및 알림창
void AnyClientMsg(int x, int y);
int ConfirmMsg_Block(int x, int y, int flag);
int ConfirmMsg_Block(int x, int y, int flag);
void ConnectMsg(int x, int y, int flag);


#endif _UI_H_ 