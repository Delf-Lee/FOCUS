#include "Admin.h"
#include "Thread.h"
#include "UI.h"

int howmany_C;
// 화살표 커서
int clt_arrow = 0;
int cmd_arrow = 0;
int arrow = 0;
unsigned char ch; // 조작에 받을 키
char ui_flag;

int last_page;  // 리스트의 마지막 페이지
int page;		// 현재 보고있는 리스트 페이지
int all;		// 전체 선택 여부

char is_select[512] = { 0, };
// 로고
char FOCUS[5][39] = {
	{ 1,1,1,1,1,1,2,0,1,1,1,1,1,1,2,0,1,1,1,1,1,1,2,0,1,1,2,0,1,1,2,0,1,1,1,1,1,1,2 },
	{ 1,1,2,0,0,0,0,0,1,1,2,0,1,1,2,0,1,1,2,0,0,0,0,0,1,1,2,0,1,1,2,0,1,1,2,0,0,0,0 },
	{ 1,1,1,1,1,1,2,0,1,1,2,0,1,1,2,0,1,1,2,0,0,0,0,0,1,1,2,0,1,1,2,0,1,1,1,1,1,1,2 },
	{ 1,1,2,0,0,0,0,0,1,1,2,0,1,1,2,0,1,1,2,0,0,0,0,0,1,1,2,0,1,1,2,0,0,0,0,0,1,1,2 },
	{ 1,1,2,0,0,0,0,0,1,1,1,1,1,1,2,0,1,1,1,1,1,1,2,0,1,1,1,1,1,1,2,0,1,1,1,1,1,1,2 }
};

void gotoxy(int x, int y) { //내가 원하는 위치로 커서 이동
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void cls(int bg_color, int text_color) {
	char cmd[100];
	system("cls");
	sprintf(cmd, "COLOR %x%x", bg_color, text_color);
	system(cmd);
}
void textcolor(int fg_color, int bg_color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);

}
void VisibleCursor(int flag) { // 커서를 안보이게 한다
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = flag;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

// 로고 출력
void ShowFOCUS(int x, int y, char* ver) {
	int i, j;
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 39; j++) {
			gotoxy(x + j, y + i);
			if (FOCUS[i][j] == 0)
				textcolor(BLACK, BLACK);
			else if (FOCUS[i][j] == 1)
				textcolor(WHITE, CYAN2);
			else
				textcolor(GRAY2, CYAN1);
			printf(" ");
		}
	}
	textcolor(WHITE, BLACK);
	gotoxy(x + 41, y); printf("v %s", ver);
}

// 로그인 화면
void ShowLogin(int y, char* ver) {
	int i;
	cls(BLACK, WHITE);

	// 로고 출력
	ShowFOCUS(18, 0, ver);

	// 바닥 선
	textcolor(WHITE, BLACK);
	gotoxy(0, y);  printf("────────────────────────────────────────");

	textcolor(YELLOW2, BLACK);
	// 컴돌이 본체
	gotoxy(11, y - 6); printf("┌─────────┐");
	gotoxy(11, y - 5); printf("│┌───────┐│");
	gotoxy(11, y - 4); printf("││              ││");
	gotoxy(11, y - 3); printf("││              ││");
	gotoxy(11, y - 2); printf("││              ││");
	gotoxy(11, y - 1); printf("││              ││");
	gotoxy(11, y);	   printf("│└───────┘│");
	gotoxy(11, y + 1); printf("└────＼/ ───┘");
	gotoxy(12, y + 2); printf(".─┴─┘/＼└─┴─.");
	gotoxy(11, y + 3); printf("/                     ＼");
	gotoxy(10, y + 4); printf("/                       ＼");
	gotoxy(10, y + 5); printf("\"────────────\"");

	// 컴순이 본체
	gotoxy(45, y - 6);
	gotoxy(45, y - 6); printf("┌│＼=/│─────┐");
	gotoxy(45, y - 5); printf("│|_/=＼│────┐│");
	gotoxy(45, y - 4); printf("││              ││");
	gotoxy(45, y - 3); printf("││              ││");
	gotoxy(45, y - 2); printf("││              ││");
	gotoxy(45, y - 1); printf("││              ││");
	gotoxy(45, y);	   printf("│└───────┘│");
	gotoxy(45, y + 1); printf("└─────────┘");
	gotoxy(46, y + 2); printf(".─┴─┘   └─┴─.");
	gotoxy(45, y + 3); printf("/                     ＼");
	gotoxy(44, y + 4); printf("/                       ＼");
	gotoxy(44, y + 5); printf("\"────────────\"");


	textcolor(WHITE, BLACK);
	// 컴돌이 표정
	gotoxy(17, y - 3); printf("●      ●");
	gotoxy(17, y - 2); printf("    ­");
	gotoxy(17, y - 1); printf("  └─┘");
	gotoxy(13, y + 3); printf("*.*.*.*.*.*.*.*.*.*");
	gotoxy(12, y + 4); printf("*.*.*.*.*.*.*.*.*.*.*");

	// 컴순이 표정
	gotoxy(50, y - 3); printf("\"●      ●\"");
	gotoxy(51, y - 2); printf("    ­");
	gotoxy(51, y - 1); printf("  └─┘");
	gotoxy(47, y + 3); printf("*.*.*.*.*.*.*.*.*.*");
	gotoxy(46, y + 4); printf("*.*.*.*.*.*.*.*.*.*.*");

	// 정보 입력란
	gotoxy(24, y + 8); printf("ID :");
	gotoxy(28, y + 7); printf("┌─────────────┐");
	gotoxy(28, y + 8); printf("│                          │");
	gotoxy(28, y + 9); printf("└─────────────┘");
	gotoxy(18, y + 11); printf("PASSWORD :");
	gotoxy(28, y + 10); printf("┌─────────────┐");
	gotoxy(28, y + 11); printf("│                          │");
	gotoxy(28, y + 12); printf("└─────────────┘");

	gotoxy(0, 25);
}

// ip 대역 설정 ui 출력, 조작
int ShowRangeSetBox(int x, int y) {
	char is_select = FALSE;
	char select_way;
	int num = 4;
	char room[][7] = { " 전체 ", "101 호", "102 호", "103 호" }; // 강의실 저장 기능을 추가한다면 수정 필요
	int roomNo = 0;

	textcolor(WHITE, BLACK);
	// 텍스트 박스
	gotoxy(x, y);		printf("┏━━ IP 대역  ━━━━━━━━━━━━━━━━┓");
	gotoxy(x, y + 1);	printf("┃                                              ┃");
	gotoxy(x, y + 2);	printf("┃                                              ┃");
	gotoxy(x, y + 3);	printf("┃                                              ┃");
	gotoxy(x, y + 4);	printf("┃                                              ┃");
	gotoxy(x, y + 5);	printf("┃                                              ┃");
	gotoxy(x, y + 6);	printf("┃                                              ┃");
	gotoxy(x, y + 7);	printf("┗━━━━━━━━━━━━━━━━━━━━━━━┛");



	// ip 빈칸 
	textcolor(WHITE, BLACK);
	gotoxy(x + 9, y + 5);  printf(".    .    .     ~");
	gotoxy(x + 31, y + 5);  printf(".    .    .    ");
	textcolor(BLACK, WHITE);
	gotoxy(x + 5, y + 5); printf("    ");
	gotoxy(x + 10, y + 5); printf("    ");
	gotoxy(x + 15, y + 5); printf("    ");
	gotoxy(x + 20, y + 5); printf("    ");
	gotoxy(x + 27, y + 5); printf("    ");
	gotoxy(x + 32, y + 5); printf("    ");
	gotoxy(x + 37, y + 5); printf("    ");
	gotoxy(x + 42, y + 5); printf("    ");

	// 제어 부
	while (SERVER_LIVE) {

		if (kbhit() == TRUE) {
			ch = getch();
			if (ch == ESC) {
				if (is_select == TRUE) {
					is_select = FALSE;
				}
				else {
				}
			}
			else if (ch == SPACE || ch == ENTER) {
				if (arrow == 2) {
					return SET_IP;
				}
				else {
					if (is_select == TRUE) {
						return roomNo % num;
					}
					else {
						is_select = TRUE;
						WipeArrow(x + 2, y + 2, arrow);
					}
				}
			}
			else if (ch == SPECIAL1 || ch == SPECIAL2) {
				ch = getch();
				switch (ch) {
				case UP:
					if (is_select == FALSE) {
						WipeArrow(x + 2, y + 2, arrow);
						arrow = (arrow == 2 ? 0 : 2);
					}
					break;
				case DOWN:
					if (is_select == FALSE) {
						WipeArrow(x + 2, y + 2, arrow);
						arrow = (arrow == 0 ? 2 : 0);
					}
					break;
				case LEFT:
					if (is_select == TRUE) {
						roomNo++;
					}
					break;
				case RIGHT:

					if (is_select == TRUE) {
						roomNo++;
					}
					break;
				}
			}
		}
		if (is_select == FALSE) {
			ShowArrow(x + 2, y + 2, arrow);
		}
		// 선택 메뉴
		if (arrow == 0) {
			textcolor(WHITE, BLACK);
			gotoxy(x + 5, y + 2);  printf("강의실");
			textcolor(GRAY2, BLACK);
			gotoxy(x + 5, y + 4); printf("직접 입력");
		}
		else {
			textcolor(GRAY2, BLACK);
			gotoxy(x + 5, y + 2);  printf("강의실");
			textcolor(WHITE, BLACK);
			gotoxy(x + 5, y + 4); printf("직접 입력");
		}
		if (is_select == TRUE) {
			textcolor(RED2, BLACK);
			gotoxy(x + 25, y + 2); printf("◀");
			gotoxy(x + 37, y + 2); printf("▶");
			textcolor(WHITE, BLACK);
			gotoxy(x + 29, y + 2); printf("%s", room[roomNo % num]);
		}
		else {
			textcolor(GRAY2, BLACK);
			gotoxy(x + 25, y + 2); printf("◀");
			gotoxy(x + 37, y + 2); printf("▶");
			textcolor(GRAY2, BLACK);
			gotoxy(x + 29, y + 2); printf("%s", room[roomNo % num]);
		}
		Sleep(90);
	}
}

// 리스트 박스 출력 (메인 좌측)
int ShowMainScr(int x, int y, int select[]) {
	int i;
	ShowFOCUS(18, 0, VER);
	VisibleCursor(FALSE);

	if (reset == TRUE) {
		clt_arrow = 0;
		cmd_arrow = 0;
		sel_clt_cnt = 0;
		memset(select, NULL, 256);
		memset(is_select, FALSE, 512);
		page = 0;
		all = FALSE;
	}

	last_page = (howmany_C - 1) / LINE;

	// 클라이언트 리스트 박스
	textcolor(WHITE, BLACK);
	gotoxy(x, y + 2); printf("┏━━━━━━┓");
	for (i = 3; i < 23; i++) {
		gotoxy(x, y + i);	printf("┃            ┃");
	}
	gotoxy(x, y + 23); printf("┗━━━━━━┛");
	gotoxy(x + 12, y + 3); printf("▲");
	gotoxy(x + 12, y + 22); printf("▼");

	// 명령 창 (출력만)
	CmdBox(17, 5);
	ShowCmdMenu(21, 7, -1);


	// 조작 부
	while (SERVER_LIVE) {
		if (kbhit() == TRUE) {
			ch = getch();
			if (howmany_C == 0) {
				AnyClientMsg(27, 12);
				ShowCmdMenu(21, 7, -1);
			}
			else if (ch == ESC) {
				if (sel_clt_cnt != 0) {
					memset(is_select, 0, 512);
					sel_clt_cnt = 0;
					all = FALSE;
				}
				else {
					// "종료하시겠습니까?
				}
			}
			else if (ch == SPACE) {
				if (is_select[clt_arrow + (page * LINE)] == FALSE) {
					select[sel_clt_cnt++] = clt_arrow + (page * LINE);
					is_select[clt_arrow + (page * LINE)] = TRUE;
				}
				else {
					select[sel_clt_cnt--] = NULL; // 여기 어쩌지
					is_select[clt_arrow + (page * LINE)] = FALSE;
				}
				if (sel_clt_cnt != howmany_C) {
					all = FALSE;
				}
				// 클라이언트 선택
			}
			else if (ch == ENTER) {
				WipeArrow(x + 2, y + 4, clt_arrow);
				PrintList(x, y + 1, LINE, -1, page);
				if (sel_clt_cnt != 0) {
					return; // 명령 while 호출
				}
				else {
					select[sel_clt_cnt++] = clt_arrow + (page * LINE);
					is_select[clt_arrow + (page * LINE)] = TRUE;
					return;
				}
				// "선택한 클라이언트 없습니다"
			}
			else if (ch == 'a') {
				memset(is_select, 0, 512);
				sel_clt_cnt = 0;
				if (all == FALSE) {
					WipeArrow(x + 2, y + 4, clt_arrow);
					PrintList(x, y + 1, LINE, -1, page);
					sel_clt_cnt = howmany_C;
					for (i = 0; i < sel_clt_cnt; i++) {
						select[i] = i;
						is_select[i + (page * LINE)] = TRUE;
					}
					all = TRUE;
				}
				else {
					sel_clt_cnt = 0;
					all = FALSE;
				}

			}
			else if (ch == SPECIAL1 || ch == SPECIAL2) {
				ch = getch();
				switch (ch) {
				case UP:
					if (page == last_page && clt_arrow == 0) {
						clt_arrow = (howmany_C % LINE) - 1;
					}
					else if (clt_arrow > 0) {
						clt_arrow--;
					}
					else {
						clt_arrow = 16;
					}
					WipeArrow(x + 2, y + 4, clt_arrow);
					break;
				case DOWN:
					if (page == last_page && clt_arrow >= (howmany_C % LINE) - 1) {
						clt_arrow = 0;
					}
					else if (clt_arrow < 16) {
						clt_arrow++;
					}
					else {
						clt_arrow = 0;
					}
					WipeArrow(x + 2, y + 4, clt_arrow);
					break;

				case LEFT:
					if (page > 0) {
						page--;
						clt_arrow = 0;
					}
					break;
				case RIGHT:
					if (page < last_page) {
						clt_arrow = 0;
						page++;
					}
					break;
				}
			}
		}
		// 출력 부
		PrintList(x, y + 1, LINE, clt_arrow, page);	// 리스트 출력
		ShowServerInfo(61, 21);
		ShowPresentTime();
		if (howmany_C != 0) {
			ShowArrow(x + 2, y + 5, clt_arrow);
			ShowCltInfo(20, 19, clt_arrow + (page * LINE));
			ScrollCursor(x, y, clt_arrow + (page * LINE));
		}

		// 접속 인원 출력
		textcolor(WHITE, BLACK);
		gotoxy(x + 2, y); printf("접속 중");
		gotoxy(x + 12, y);
		if (howmany_C != 0) {
			gotoxy(x + 10, y);	printf("    ");
			gotoxy(x + 12, y);
			textcolor(GREEN2, BLACK); printf("%d", howmany_C);
		}
		else {
			gotoxy(x + 10, y);
			textcolor(RED2, BLACK); printf("없음");
		}


		Sleep(90);
	}
}

// 클라이언트 리스트 출력
void PrintList(int x, int y, int n, int arrow, int page) {
	int i;
	WipeList(x, y);
	for (i = 0; i < LINE; i++) {
		if (i + (page * LINE) >= howmany_C)
			break;

		if (sel_clt_cnt != howmany_C) {
			textcolor(GRAY2, BLACK);
		}
		else {
			textcolor(GREEN2, BLACK);
		}

		gotoxy(x + 2, y + 3);
		printf("   ALL");
		gotoxy(x + 8, y + 3);
		textcolor(GRAY2, BLACK);
		printf("(A)");

		if (is_select[i + (page * LINE)] == FALSE) {
			if (i % LINE == arrow) {
				gotoxy(x + 3, y + 4 + i % LINE);
				textcolor(GRAY2, BLACK);
				printf("   CLT");
				textcolor(WHITE, BLACK);
				printf("%.3d", (i + 1) + (page * LINE));
			}
			else {
				gotoxy(x + 3, y + 4 + i % LINE);
				textcolor(GRAY2, BLACK);
				printf("- CLT");
				textcolor(WHITE, BLACK);
				printf("%.3d ", (i + 1) + (page * LINE));
			}
		}
		else {
			gotoxy(x + 3, y + 4 + i % LINE);
			textcolor(BLACK, WHITE);
			if (i % LINE == arrow) {
				printf(" - CLT%.3d", (i + 1) + (page * LINE));
			}
			else {
				printf("- CLT%.3d", (i + 1) + (page * LINE));
			}
		}
	}
}

// 클라이언트 리스트 지우기
void WipeList(int x, int y) {
	int i;
	for (i = 0; i < LINE; i++) {
		gotoxy(x + 3, y + 4 + i % LINE);
		textcolor(BLACK, BLACK);
		printf("         ");
	}
}

// 클라이언트 정보 출력
void ShowCltInfo(int x, int y, int clt) {
	textcolor(WHITE, BLACK);
	gotoxy(x, y); printf("┌ ▶ 클라이언트 정보 ──────┐");
	gotoxy(x, y + 1); printf("│                                │");
	gotoxy(x, y + 2); printf("│                                │");
	gotoxy(x, y + 3); printf("│                                │");
	gotoxy(x, y + 4); printf("└────────────────┘");

	if (howmany_C == 0) {
		gotoxy(x + 2, y + 1); printf("                          ");
		gotoxy(x + 4, y + 2); printf("                          ");
		gotoxy(x + 2, y + 3); printf("                          ");
	}

	else if (clt == -1) {
		gotoxy(x + 2, y + 1); printf("                          ");
		gotoxy(x + 4, y + 2); printf("                          ");
		gotoxy(x + 4, y + 2); printf("%d명의 클라이언트", sel_clt_cnt);
		gotoxy(x + 2, y + 3); printf("                          ");
	}
	else {
		textcolor(YELLOW2, BLACK);
		gotoxy(x + 25, y); printf("%2d", sel_clt_cnt);
		textcolor(WHITE, BLACK);
		gotoxy(x + 27, y); printf("명 선택", sel_clt_cnt);
		gotoxy(x + 4, y + 1); printf("CLT%.3d", clt + 1);
		gotoxy(x + 4, y + 2); printf("IP         %s", Client_List[clt].IP_ADDR);
		gotoxy(x + 4, y + 3); printf("MAC        %s", Client_List[clt].MAC_ADDR);
	}
}

// 명령메뉴 테두리
void CmdBox(int x, int y) {
	int i;
	textcolor(WHITE, BLACK);
	gotoxy(x, y); printf("┏━━━━━━━━━━━━━━━━━━━┓");
	for (i = 0; i < 18; i++) {
		gotoxy(x, y + 1 + i); printf("┃                                      ┃");
	}
	gotoxy(x, y + 19); printf("┗━━━━━━━━━━━━━━━━━━━┛");
}

// 명령메뉴 텍스트 출력
int ShowCmdMenu(int x, int y, int arrow) {

	int i;
	char menu[6][32] = { "전원 종료", "절전 모드", "메시지 전송", "웹브라우저 실행", "인터넷 차단 실행/해제","파일 전송" };
	ClrCmdBox();
	for (i = 0; i < 6; i++) {
		gotoxy(x, y + 2 * i);
		if (i == arrow) {
			textcolor(BLACK, BLACK); printf(" ");
			textcolor(BLACK, YELLOW2); printf(" ");
			textcolor(WHITE, BLACK); printf(" %s", menu[i]);
		}
		else {
			textcolor(BLACK, GRAY2); printf(" ");
			textcolor(GRAY2, BLACK); printf(" %s", menu[i]);
		}
	}
}

// 명령메뉴 텍스트 지우기
void ClrCmdBox() {
	int i;
	for (i = 0; i < 18; i++) {
		gotoxy(20, 6 + i); printf("                                     ");
	}
}

// 명령메뉴 출력, 조작
int ShowCmdBox(int x, int y) {
	int i;
	// 창
	CmdBox(x, y);

	// 조작부
	while (SERVER_LIVE) {

		if (kbhit() == TRUE) {
			ch = getch();
			if (ch == ESC) {
				first = TRUE;
				reset = FALSE;
				break;
			}
			else if (ch == ENTER || ch == SPACE) {
				switch (cmd_arrow) {
				case 0:
					return SHUTDOWN;
					break;
				case 1:
					return SLEEP_MODE;
					break;
				case 2:
					return SEND_MESSAGE;
					break;
				case 3:
					return RUN_WEBBROWSER;
					break;
				case 4:
					return BLOCK_INTERNET;
					break;
				case 5:
					return FTP;
					break;
				}
			}
			else if (ch == SPECIAL1 || ch == SPECIAL2) {
				ch = getch();
				switch (ch) {
				case UP:
					if (cmd_arrow > 0) {
						cmd_arrow--;
					}
					else {
						cmd_arrow = 5;
					}
					break;
				case DOWN:
					if (cmd_arrow < 5) {
						cmd_arrow++;
					}
					else {
						cmd_arrow = 0;
					}
					break;
				case LEFT:
					break;
				case RIGHT:
					break;
				}
			}
		}
		// 출력
		ShowCmdMenu(x + 4, y + 2, cmd_arrow);
		ShowCltInfo(20, 19, -1);
		Sleep(90);
		ShowPresentTime();
		ShowServerInfo(61, 21);
		//debug(0, 0, cmd_arrow, 0);
	}
	ShowCmdMenu(x + 4, y + 2, -1); // 활성화 취소	
}

// 전원 종료 ui 출력, 조작
int ShutDownBox(int x, int y) {

	int i;
	int confirm;
	char* time_p;
	arrow = 0;

	ClrCmdBox();
	gotoxy(x, y);
	textcolor(BLACK, YELLOW2); printf(" ");
	textcolor(WHITE, BLACK); printf(" 전원 종료");
	gotoxy(x + 2, y + 2); printf("시간 설정");

	while (SERVER_LIVE) {

		if (kbhit() == TRUE) {
			ch = getch();
			if (ch == ESC) {
				first = FALSE;
				return FALSE;
			}
			else if (ch == ENTER || ch == SPACE) {
				memset(send_time, NULL, 5);
				if (arrow == 0) {
					int flag = 0;
					textcolor(WHITE, BLACK);
					gotoxy(x + 6, y + 4);  printf("__ 시 __ 분에 종료");

					time_p = InputTime(x, y, SHUTDOWN);
					if (time_p == NULL) {
						continue;
					}
					else {
						memcpy(send_time, time_p, 5);
						if (strcmp(time_p, "9999") == 0) {
							flag = 2;
						}
					}

					confirm = ConfirmMsg_Time(21, 11, flag);
					ClrCmdBox();
					gotoxy(x, y);
					textcolor(BLACK, YELLOW2); printf(" ");
					textcolor(WHITE, BLACK); printf(" 전원 종료");
					gotoxy(x + 2, y + 2); printf("시간 설정");

					if (confirm == TRUE) {
						return SHUTDOWN;
					}
					arrow = 0;
				}
				else {
					int flag = 1;
					textcolor(WHITE, BLACK);
					gotoxy(x + 6, y + 6);  printf("__ 시간 __ 분 후에 종료");

					time_p = InputTime(x, y, SHUTDOWN_T);
					if (time_p == NULL) {
						continue;
					}
					else {
						memcpy(send_time, time_p, 5);
						if (strcmp(time_p, "0000") == 0) {
							flag = 2;
						}
					}

					confirm = ConfirmMsg_Time(21, 11, flag);
					ClrCmdBox();
					gotoxy(x, y);
					textcolor(BLACK, YELLOW2); printf(" ");
					textcolor(WHITE, BLACK); printf(" 전원 종료");
					gotoxy(x + 2, y + 2); printf("시간 설정");

					if (confirm == TRUE) {
						return SHUTDOWN_T;
					}
					arrow = 2;
				}
			}
			else if (ch == SPECIAL1 || ch == SPECIAL2) {
				ch = getch();
				switch (ch) {
				case UP:
					WipeArrow(x + 3, y + 4, arrow);
					arrow = (arrow == 2 ? 0 : 2);
					break;
				case DOWN:
					WipeArrow(x + 3, y + 4, arrow);
					arrow = (arrow == 0 ? 2 : 0);
					break;
				}
			}
		}

		textcolor(GRAY2, BLACK);
		gotoxy(x + 6, y + 4);  printf("__ 시 __ 분에 종료");
		gotoxy(x + 6, y + 6);  printf("__ 시간 __ 분 후에 종료");

		ShowArrow(x + 3, y + 4, arrow);
		Sleep(90);
	}
}

// 절전 모드 ui 출력, 조작
int SleepModeBox(int x, int y) {

	int i;
	int confirm;
	char* time_p;
	arrow = 0;

	ClrCmdBox();
	gotoxy(x, y);
	textcolor(BLACK, YELLOW2); printf(" ");
	textcolor(WHITE, BLACK); printf(" 절전 모드");
	gotoxy(x + 2, y + 2); printf("시간 설정");

	while (SERVER_LIVE) {

		if (kbhit() == TRUE) {
			ch = getch();
			if (ch == ESC) {
				first = FALSE;
				return FALSE;
			}
			else if (ch == ENTER || ch == SPACE) {

				memset(send_time, NULL, 5);
				if (arrow == 0) {
					int flag = 0;
					textcolor(WHITE, BLACK);
					gotoxy(x + 6, y + 4);  printf("__ 시 __ 분에 절전모드");

					time_p = InputTime(x, y, SLEEP_MODE);
					if (time_p == NULL) {
						continue;
					}
					else {
						memcpy(send_time, time_p, 5);
						if (strcmp(time_p, "9999") == 0) {
							flag = 2;
						}
					}
					confirm = ConfirmMsg_Time(21, 11, flag);

					ClrCmdBox();
					gotoxy(x, y);
					textcolor(BLACK, YELLOW2); printf(" ");
					textcolor(WHITE, BLACK); printf(" 전원 종료");
					gotoxy(x + 2, y + 2); printf("시간 설정");

					if (confirm == TRUE) {
						return SLEEP_MODE;
					}
					arrow = 0;
				}
				else {
					int flag = 1;
					textcolor(WHITE, BLACK);
					gotoxy(x + 6, y + 6);  printf("__ 시간 __ 분 후에 절전모드");
					time_p = InputTime(x, y, SLEEP_MODE_T);
					if (time_p == NULL) {
						continue;
					}
					else {
						memcpy(send_time, time_p, 5);
						if (strcmp(time_p, "0000") == 0) {
							flag = 2;
						}
					}
					confirm = ConfirmMsg_Time(21, 11, flag);

					ClrCmdBox();
					gotoxy(x, y);
					textcolor(BLACK, YELLOW2); printf(" ");
					textcolor(WHITE, BLACK); printf(" 전원 종료");
					gotoxy(x + 2, y + 2); printf("시간 설정");

					if (confirm == TRUE) {
						return SLEEP_MODE_T;
					}
					arrow = 2;
				}
			}
			else if (ch == SPECIAL1 || ch == SPECIAL2) {
				ch = getch();
				switch (ch) {
				case UP:
					WipeArrow(x + 3, y + 4, arrow);
					arrow = (arrow == 2 ? 0 : 2);
					break;
				case DOWN:
					WipeArrow(x + 3, y + 4, arrow);
					arrow = (arrow == 0 ? 2 : 0);
					break;
				}
			}
		}

		textcolor(GRAY2, BLACK);
		gotoxy(x + 6, y + 4);  printf("__ 시 __ 분에 절전모드");
		gotoxy(x + 6, y + 6);  printf("__ 시간 __ 분 후에 절전모드");

		ShowArrow(x + 3, y + 4, arrow);
		Sleep(90);
	}
}

// '전원 종료 & 절전 모드'에서 시간 입력
char* InputTime(int x, int y, int flag) {
	char input[5] = { '\0', };
	int cursor = 0;
	int box = 0;
	int cnt[2] = { 0, };
	int confirm = 0;

	// 동작 시각 설정
	if (flag == SHUTDOWN || flag == SLEEP_MODE) {
		while (TRUE) {
			if (kbhit() == TRUE) {
				
				ch = getch();
				if (ch == ESC) {
					return NULL;
				}
				else if (ch == ENTER) {
					if (cursor == 0) {
						memset(input, '9', 4);
						return input;
					}
					else if (cursor != 4) {
					}
					else {
						confirm = atoi(input);
						if (confirm / 100 <= 23 && confirm % 100 < 60) {
							return input;
						}
						else {
							textcolor(RED2, BLACK);
							gotoxy(x + 25, y + 4); printf("입력 오류");
							textcolor(WHITE, BLACK);
							cursor = box = 0;
							cnt[0] = cnt[1] = 0;
							gotoxy(x + 6, y + 4); printf("__");
							gotoxy(x + 12, y + 4); printf("__");
							continue;
						}
					}
				}
				else if (ch == BACKSPACE) {
					gotoxy(x + 25, y + 4); printf("         ");
					if (cursor >= 0) {
						if (box == 1 && cnt[box] == 0) {
							box--;
						}
						if (cursor > 0) {
							gotoxy(x + 5 + cnt[box] + 6 * box, y + 4);  printf("_");
							cursor--;
						}
						input[cursor] = NULL;
						if (cnt[box] > 0) {
							cnt[box]--;
						}
					}
				}
				else if (ch == TAB) {
				}
				else if (ch == SPECIAL1 || ch == SPECIAL2) {
				}
				else if (ch >= '0' && ch <= '9') {
					gotoxy(x + 25, y + 4); printf("         ");
					gotoxy(x + 6 + cnt[box] + 6 * box, y + 4);
					if (cursor < 4) {
						input[cursor++] = ch;
						printf("%c", ch);
						cnt[box]++;

						if (cnt[box] == 2 && box == 0) {
							box++;
							cnt[box] = 0;
						}
					}
				}
				else {
				}
			}

			Sleep(50);
		}
	}

	// 타이머 시간 설정
	else if (flag == SHUTDOWN_T || flag == SLEEP_MODE_T) {
		while (TRUE) {
			if (kbhit() == TRUE) {
				gotoxy(x + 6 + cnt[box] + 8 * box, y + 6);
				ch = getch();
				if (ch == ESC) {
					return NULL;
				}
				else if (ch == ENTER) {
					if (cursor == 0) {
						memset(input, '0000', 4);
						return input;
					}
					else if (cursor != 4) {
					}
					else {
						return input;
					}
				}
				else if (ch == BACKSPACE) {
					if (cursor >= 0) {
						if (box == 1 && cnt[box] == 0) {
							box--;
						}
						if (cursor > 0) {
							gotoxy(x + 5 + cnt[box] + 8 * box, y + 6);  printf("_");
							cursor--;
						}
						input[cursor] = NULL;
						if (cnt[box] > 0) {
							cnt[box]--;
						}
					}
				}
				else if (ch == TAB) {
				}
				else if (ch == SPECIAL1 || ch == SPECIAL2) {
				}
				else if (ch >= '0' && ch <= '9') {
					if (cursor < 4) {
						input[cursor++] = ch;
						printf("%c", ch);
						cnt[box]++;

						if (cnt[box] == 2 && box == 0) {
							box++;
							cnt[box] = 0;
						}
					}
				}
				else {
				}
			}

			Sleep(50);
		}
	}
}

// 웹 브라우저 실행 ui 출력, 조작
int RunWebBrowserBox(int x, int y) {
	int i;
	int confirm;
	char tmpinput[128] = { '\0', };
	arrow = 0;

	ClrCmdBox();
	gotoxy(x, y);
	textcolor(BLACK, YELLOW2); printf(" ");
	textcolor(WHITE, BLACK); printf(" 웹 브라우저 실행");

	textcolor(WHITE, WHITE);
	gotoxy(x + 2, y + 4); printf("                             ");
	gotoxy(x + 2, y + 5); printf("                             ");
	gotoxy(x + 2, y + 6); printf("                             ");

	memset(send_web, NULL, 128);
	while (SERVER_LIVE) {

		if (kbhit() == TRUE) {
			ch = getch();
			if (ch == ESC) {
				first = FALSE;
				return FALSE;
			}
			else if (ch == ENTER || ch == SPACE) {
				if (arrow == 0) {
					textcolor(WHITE, BLACK);
					gotoxy(x + 4, y + 2);  printf("실행 할 웹 페이지 주소");
					textcolor(BLACK, WHITE);
					gotoxy(x + 3, y + 4);
					scanf("%s", tmpinput);
					strcpy(send_web, "http://");
					strcat(send_web, tmpinput);

					arrow = 1;
				}
				else {
					return TRUE;
				}
			}
			else if (ch == SPECIAL1 || ch == SPECIAL2) {
				ch = getch();
				switch (ch) {
				case UP:
					WipeArrow(x + 1, y + 2, arrow);
					arrow = arrow == 0 ? 1 : 0;
					break;
				case DOWN:
					WipeArrow(x + 1, y + 2, arrow);
					arrow = arrow == 0 ? 1 : 0;
					break;
				}
			}
		}

		if (arrow == 0) {
			textcolor(RED2, BLACK); gotoxy(x + 1, y + 2);  printf("▶");
			textcolor(GRAY2, BLACK); gotoxy(x + 4, y + 2);  printf("실행 할 웹 페이지 주소");

			gotoxy(x + 23, y + 8);  printf("  ");
			gotoxy(x + 26, y + 8);  printf("확인");
		}
		else {
			textcolor(GRAY2, BLACK); gotoxy(x + 1, y + 2);  printf("  ");
			textcolor(GRAY2, BLACK); gotoxy(x + 4, y + 2);  printf("실행 할 웹 페이지 주소");

			textcolor(RED2, BLACK); gotoxy(x + 23, y + 8);  printf("▶");
			textcolor(WHITE, BLACK); gotoxy(x + 26, y + 8);  printf("확인");
			gotoxy(x + 1, y + 3); printf("  ");
		}
		//debug(0, 0, arrow, 0);
		Sleep(90);
	}
}

// 인터넷 차단 ui 출력, 조작
int BlockInternetBox(int x, int y) {

	int i;
	int confirm;
	char* time_p;
	arrow = 0;

	ClrCmdBox();
	gotoxy(x, y);
	textcolor(BLACK, YELLOW2); printf(" ");
	textcolor(WHITE, BLACK); printf(" 인터넷 차단");

	while (SERVER_LIVE) {

		if (kbhit() == TRUE) {
			ch = getch();
			if (ch == ESC) {
				first = FALSE;
				return FALSE;
			}
			else if (ch == ENTER || ch == SPACE) {
				memset(send_time, NULL, 5);
				if (arrow == 0) {
					textcolor(WHITE, BLACK);
					gotoxy(x + 6, y + 4);  printf("실행");

					confirm = ConfirmMsg_Block(21, 11, 0);
					ClrCmdBox();
					gotoxy(x, y);
					textcolor(BLACK, YELLOW2); printf(" ");
					textcolor(WHITE, BLACK); printf(" 인터넷 차단");

					if (confirm == TRUE) {
						return TRUE;
					}
					else {
						return FALSE;
					}
					arrow = 0;
				}
				else {
					textcolor(WHITE, BLACK);
					gotoxy(x + 6, y + 6);  printf("해제");

					confirm = ConfirmMsg_Block(21, 11, 1);
					ClrCmdBox();
					gotoxy(x, y);
					textcolor(BLACK, YELLOW2); printf(" ");
					textcolor(WHITE, BLACK); printf(" 인터넷 차단");

					if (confirm == TRUE) {
						return FALSE;
					}
					else {
						return TRUE;
					}
					arrow = 2;
				}
			}
			else if (ch == SPECIAL1 || ch == SPECIAL2) {
				ch = getch();
				switch (ch) {
				case UP:
					WipeArrow(x + 3, y + 4, arrow);
					arrow = (arrow == 2 ? 0 : 2);
					break;
				case DOWN:
					WipeArrow(x + 3, y + 4, arrow);
					arrow = (arrow == 0 ? 2 : 0);
					break;
				}
			}
		}

		textcolor(GRAY2, BLACK);
		gotoxy(x + 6, y + 4);  printf("실행");
		gotoxy(x + 6, y + 6);  printf("해제");

		ShowArrow(x + 3, y + 4, arrow);
		Sleep(90);
	}
}

// 화살표 출력
void ShowArrow(int x, int y, int arrow) {
	gotoxy(x, y + arrow);
	textcolor(RED2, BLACK); printf("▶");
	VisibleCursor(FALSE);

}

// 화살표 지우기
void WipeArrow(int x, int y, int arrow) {
	gotoxy(x, y + arrow);
	textcolor(BLACK, BLACK); printf("   ");
}

// 관리자 정보 출력
void ShowAdminInfo(int x, int y) {
	int i;
	char info[4][7] = { "아이디", "이름", "강의실", "IP주소" };

	// 창
	textcolor(WHITE, BLACK);
	gotoxy(x, y); printf("┏┫관리자 정보 ┣┓");
	for (i = 0; i < 10; i++) {
		gotoxy(x, y + 1 + i); printf("┃                ┃");
	}
	gotoxy(x, y + 11);  printf("┗━━━━━━━━┛");

	// 내용

	textcolor(GREEN1, CYAN1);
	for (i = 0; i < 4; i++) {
		gotoxy(x + 2, y + 2 + 2 * i); printf(" ");
	}
	textcolor(WHITE, BLACK);
	for (i = 0; i < 4; i++) {
		gotoxy(x + 4, y + 2 + 2 * i); printf("%s", info[i]);
	}
	gotoxy(x + 4, y + 3); printf("%s", adminID);
	gotoxy(x + 4, y + 5);
	gotoxy(x + 4, y + 7);
	gotoxy(x + 3, y + 9); printf("%s", s.IP_ADDR);
}

// 서버 정보 출력
void ShowServerInfo(int x, int y) {

	textcolor(BLACK, RED1);
	gotoxy(x, y); printf(" ");
	textcolor(WHITE, BLACK);
	gotoxy(x + 2, y); printf("서버 IP");

	gotoxy(x + 2, y + 1); printf("%s", SERVER_IP);
	gotoxy(x, y + 3); printf("서버 연결상태");
	gotoxy(x + 14, y + 3);

	if (SERVER_LIVE) {
		textcolor(GREEN1, BLACK);
		printf("정상");
	}
	else {
		textcolor(RED2, BLACK);
		printf("끊김");
	}
}

// 현재 시간 출력
void ShowPresentTime() {

	char *weeks[] = { "일", "월", "화", "수", "목", "금", "토" };

	time(&time_now);
	timeinfo = localtime(&time_now);
	textcolor(WHITE, BLACK);
	gotoxy(61, 15);
	printf("%d. %d. %d (%s)", 1900 + timeinfo->tm_year, timeinfo->tm_mon + 1, timeinfo->tm_mday, weeks[timeinfo->tm_wday]);
	gotoxy(64, 17);
	textcolor(WHITE, BLACK);
	printf("[ %.2d:%.2d ]", timeinfo->tm_hour, timeinfo->tm_min);
	if (timeinfo->tm_sec % 2 == 0) {
		gotoxy(68, 17); printf(" ");
	}
	return 0;
}

// 로그인 성공 여부 알림창
void LoginMsg(int x, int y, int flag) {
	if (flag == FALSE)
		textcolor(RED1, BLACK);
	else
		textcolor(GREEN2, BLACK);

	gotoxy(x, y); printf("┏━━━━━━━━━━┓");
	gotoxy(x, y + 1); printf("┃                    ┃");
	gotoxy(x, y + 2); printf("┗━━━━━━━━━━┛");
	textcolor(WHITE, BLACK);

	if (flag == FALSE) {
		gotoxy(x + 6, y + 1); printf("로그인 실패");
	}
	else {
		gotoxy(x + 6, y + 1); printf("로그인 성공");
	}
	Sleep(1000);
}

// "전송 성공"
void SendCompleteMsg(int x, int y) {
	textcolor(GREEN2, BLACK);
	gotoxy(x, y); printf("▒━━━━━━━━━━▒");
	gotoxy(x, y + 1); printf("┃                    ┃");
	gotoxy(x, y + 2); printf("▒━━━━━━━━━━▒");
	textcolor(WHITE, BLACK);
	gotoxy(x + 7, y + 1); printf("전송 성공");

	Sleep(1500);
}

// "클라이언트 없음"
void AnyClientMsg(int x, int y) {
	// 접속 중인 클라이언트 없을 때, 조작이 있다면 출력
	textcolor(YELLOW1, BLACK);
	gotoxy(x, y); printf("┏━━━━━━━━━━┓");
	gotoxy(x, y + 1); printf("┃                    ┃");
	gotoxy(x, y + 2); printf("┗━━━━━━━━━━┛");
	textcolor(WHITE, BLACK);
	gotoxy(x + 4, y + 1); printf("클라이언트 없음");

	Sleep(500);
}

// 스크롤 바 이동 ui
void ScrollCursor(int x, int y, int arrow) {

	int i;
	int p;
	if (last_page == 0)
		p = 0;
	else 
		p = ((arrow + 1) * LINE) / howmany_C;
	for (i = 0; i < 18; i++) {
		gotoxy(x + 12, y + 4 + i);
		printf("  ");
	}
	gotoxy(x + 12, y + 4 + p);
	textcolor(GRAY2, BLACK);
	printf("▒");
}

// 확인창(인터넷 차단)
int ConfirmMsg_Block(int x, int y, int flag) {

	arrow = 0;
	char h[3] = { '\0', };
	char m[3] = { '\0', };

	memcpy(h, send_time, 2);
	memcpy(m, send_time + 2, 2);

	// 창
	gotoxy(x, y); printf("┌───────────────┐");
	gotoxy(x, y + 1); printf("│                              │");
	gotoxy(x, y + 2); printf("│                              │");
	gotoxy(x, y + 3); printf("│                              │");
	gotoxy(x, y + 4); printf("└───────────────┘");


	if (flag == 0) {
		gotoxy(x + 2, y + 1); printf("실행 하시겠습니까?");
	}
	else {
		gotoxy(x + 2, y + 1); printf("해제 하시겠습니까?");
	}

	while (SERVER_LIVE) {


		if (kbhit() == TRUE) {
			ch = getch();
			if (ch == ESC) {
				return FALSE;
			}
			else if (ch == ENTER) {
				if (arrow == 0)
					return TRUE;
				else
					return FALSE;
			}
			else if (ch == SPECIAL1 || ch == SPECIAL2) {
				ch = getch();
				switch (ch) {
				case LEFT:
					arrow = arrow == 1 ? 0 : 1;
					break;
				case RIGHT:
					arrow = arrow == 1 ? 0 : 1;
					break;
				}
			}
		}
		if (arrow == 0) {
			textcolor(GREEN2, BLACK);
			gotoxy(x + 8, y + 3); printf("확인");
			textcolor(GRAY2, BLACK);
			gotoxy(x + 21, y + 3); printf("취소");
		}
		else {
			textcolor(GRAY2, BLACK);
			gotoxy(x + 8, y + 3); printf("확인");
			textcolor(GREEN2, BLACK);
			gotoxy(x + 21, y + 3); printf("취소");
		}
		//debug(0, 0, arrow, 0);
		Sleep(90);
	}

}

// 확인 창(전원종료, 절전모드)
int ConfirmMsg_Time(int x, int y, int flag) {

	arrow = 0;
	char h[3] = { '\0', };
	char m[3] = { '\0', };

	//strcpy(send_time, "1234");

	memcpy(h, send_time, 2);
	memcpy(m, send_time + 2, 2);

	// 창
	gotoxy(x, y); printf("┌───────────────┐");
	gotoxy(x, y + 1); printf("│                              │");
	gotoxy(x, y + 2); printf("│                              │");
	gotoxy(x, y + 3); printf("│                              │");
	gotoxy(x, y + 4); printf("└───────────────┘");


	if (flag == 0) {
		gotoxy(x + 2, y + 1); printf("%s시 %s분에 종료합니다.", h, m);
	}
	else if (flag == 1) {
		gotoxy(x + 2, y + 1); printf("%s시간 %s분 후에 종료합니다.", h, m);
	}
	else {
		gotoxy(x + 2, y + 1); printf("지금 종료합니다.");
	}

	while (SERVER_LIVE) {


		if (kbhit() == TRUE) {
			ch = getch();
			if (ch == ESC) {
				return FALSE;
			}
			else if (ch == ENTER) {
				if (arrow == 0)
					return TRUE;
				else
					return FALSE;
			}
			else if (ch == SPECIAL1 || ch == SPECIAL2) {
				ch = getch();
				switch (ch) {
				case LEFT:
					arrow = arrow == 1 ? 0 : 1;
					break;
				case RIGHT:
					arrow = arrow == 1 ? 0 : 1;
					break;
				}
			}
		}
		if (arrow == 0) {
			textcolor(GREEN2, BLACK);
			gotoxy(x + 8, y + 3); printf("확인");
			textcolor(GRAY2, BLACK);
			gotoxy(x + 21, y + 3); printf("취소");
		}
		else {
			textcolor(GRAY2, BLACK);
			gotoxy(x + 8, y + 3); printf("확인");
			textcolor(GREEN2, BLACK);
			gotoxy(x + 21, y + 3); printf("취소");
		}
		//debug(0, 0, arrow, 0);
		Sleep(90);
	}

}

// 연결 상태 알림창
void ConnectMsg(int x, int y, int flag) {
	if (flag == FALSE)
		textcolor(RED1, BLACK);
	else
		textcolor(GREEN2, BLACK);

	gotoxy(x, y); printf("▒━━━━━━━━━━▒");
	gotoxy(x, y + 1); printf("┃                    ┃");
	gotoxy(x, y + 2); printf("▒━━━━━━━━━━▒");
	textcolor(WHITE, BLACK);

	if (flag == FALSE) {
		gotoxy(x + 5, y + 1); printf("서버 신호 없음");
	}
	else {
		gotoxy(x + 5, y + 1); printf("서버 연결 성공");
	}
	Sleep(1000);
}