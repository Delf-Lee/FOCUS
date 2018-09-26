#include "Admin.h"
#include "UI.h"

char* inputIP();	// IP 주소 입력
char* InputID();	// ID 입력
char* InputPwd();	// 비밀번호

int LoginTry() {
	char input[16], SHAid[64], SHApwd[64];
	char *tmp_pwd; // 흐름 제어를 위한 변수

	// 로그인 화면
	ShowLogin(12, VER);

	// 입력
	while (SERVER_LIVE) {
		memset(input, NULL, 16);
		memcpy(input, InputID(), 16);
		memcpy(adminID, input, 16);
		SHA256_Encrpyt(input, 16, SHAid);		// id 암호화 (64byte의 문자열로 전환)
		memset(input, NULL, 16);
		tmp_pwd = InputPwd();
		if (tmp_pwd == NULL) {
			continue;
		}
		else {
			memcpy(input, tmp_pwd, 16);
			SHA256_Encrpyt(input, 16, SHApwd);	// 비밀번호 암호화 (64byte의 문자열로 전환)
			break;
		}
	}

	// 로그인 요청
	if (LoginReq(Main_Socket, SHAid, SHApwd)) {
		while (SERVER_LIVE) {
			if (Send_Packet(Main_Socket, (char *)&s, sizeof(s), ADMINISTRATOR, ADMINISTRATOR_STATE, 0) == -1) {
				closesocket(Main_Socket);
				SERVER_LIVE = FALSE;
			}
			else {
				// 로그인 성공
				LoginMsg(27, 12, TRUE);
				ShowLogin(12, VER);
				return TRUE;
			}
		}
	}
	else {
		// 로그인 실패
		LoginMsg(27, 12, FALSE);
		return FALSE;
	}
}

int LoginReq(SOCKET Socket, char *ID_SHA, char *PWD_SHA) {
	char SHAstr[128];
	char buf[512] = { 0, };	// 데이터를 받을 버퍼
	int *p = buf;			// 헤더 해석을 위한 포인터
	int size = 0;

	struct timeval val;
	val.tv_sec = 0;
	val.tv_usec = 100;

	// ID와 Password에 해당하는 문자열 합침
	memcpy(&SHAstr[0], ID_SHA, 64);
	memcpy(&SHAstr[64], PWD_SHA, 64);

	/* 로그인 정보를 송신 */
	if (Send_Packet(Socket, SHAstr, 128, ADMINISTRATOR, ADMINISTRATOR_LOGIN, 0) == -1) {
		// 1-A.  송신 실패
		return FALSE;
	}
	else {
		// 1-B. 송신 성공
		if (recv(Socket, buf, 12, 0) == -1) {
			// 2-A. 수신 실패
			return FALSE;
		}
		else {
			// 2-B. 수신 성공
			size = p[0];
			recv(Socket, buf, size, 0);
			if (strcmp(buf, "LOGINSUCCESS") == 0) {
				// 3-A. 정보 일치
				return TRUE;
			}
			else {
				// 3-B. 정보 불일치
				return FALSE;
			}
		}
	}
}

//ip 대역 요청
int IRrangeReq() {
	char recvdata[60] = { 0, };
	char* addr;
	char opt;
	int num;
	int *p = recvdata;

	while (SERVER_LIVE) {
		// ip 입력방법 선택
		opt = ShowRangeSetBox(15, 18);

		if (opt == SET_IP) {					// 직접 입력
			if ((addr = inputIP()) == NULL)
				continue;
			memcpy(recvdata, addr, 60);
		}
		else if (opt == 0) {					// 강의실 선택
			// 0.0.0.0 ~ 255.255.255.255
			strcpy(recvdata, "0.0.0.0");
			strcpy(recvdata + 30, "255.255.255.255");
		}
		break;
	}


	if (Send_Packet(Main_Socket, recvdata, sizeof(recvdata), ADMINISTRATOR, ADMINISTRATOR_REQUEST_LIST, 0) == -1) {
		// 요청 실패 창
		closesocket(Main_Socket);
		LOGIN = FALSE;
		bootThread = TRUE;
	}
	else {
		if (recv(Main_Socket, recvdata, 12, 0) == -1) {
			// 요청 실패 창
			closesocket(Main_Socket);
			LOGIN = FALSE;
			bootThread = TRUE;
		}
		else {
			memset(recvdata, NULL, sizeof(recvdata));
			recv(Main_Socket, recvdata, 4, 0);
			num = p[0];
			return num;
		}
	}
	system("cls");
	return;
}

// 
char* inputIP() {
	// 이전 커서 정보를 저장하기 위한 변수들
	char input_ip[2][30] = { {0,},{0,} };
	char recvdata[60] = { 0, };
	unsigned char ch;
	int area = 0; // ip 주소 구분 (0~1)
	int box[2] = { 0, }; // ip 주소의 한칸 (0~3)
	int cnt[2][4] = { 0, }; // ip 주소 한칸의 한 글자 (0~3)
	int cursor[2] = { 0, }; // input에서의 현재 입력 index

	VisibleCursor(TRUE);
	while (SERVER_LIVE) {
		gotoxy(21 + 5 * box[area] + cnt[area][box[area]] + area * 22, 23);
		if (kbhit() == TRUE) {
			ch = getch();
			if (ch == ESC) { // 취소
				return NULL;
			}
			else if (ch == ENTER) { // 모두 입력 완료 시, 리턴
				if (box[area] == 3 && cnt[area][box[area]] != 0) {
					if (area == 0) {
						area = 1;
						memcpy(recvdata, input_ip[0], 30);
					}
					else {
						memcpy(recvdata + 30, input_ip[1], 30);
						break;
					}
				}
				else {

				}
			}
			else if (ch == BACKSPACE) {
				if (cursor[area] >= 0) {
					if (input_ip[area][cursor[area] - 1] == '.') {
						input_ip[area][--cursor[area]] = NULL;
						box[area]--;
						gotoxy(21 + 5 * box[area] + cnt[area][box[area]] - 1 + area * 22, 23); printf(" ");
					}
					if (area == 1 && box[area] == 0 && cnt[area][box[area]] == 0) {
						area = 0;
					}
					gotoxy(21 + 5 * box[area] + cnt[area][box[area]] - 1 + area * 22, 23); printf(" ");
					if (cursor[area] > 0)
						cursor[area]--;

					input_ip[area][cursor[area]] = NULL;

					if (cnt[area][box[area]] > 0)
						cnt[area][box[area]]--;
				}
			}
			else if ((ch == TAB || ch == '.') && box[area] < 3) {
				if (cnt[area][box[area]] != 0) {
					input_ip[area][cursor[area]++] = '.';
					box[area]++;
					cnt[area][box[area]] = 0;
				}
			}
			else if ((ch >= '0' && ch <= '9') && box[area] != 4 && cnt[area][box[area]] != 3) {
				textcolor(BLACK, WHITE);
				input_ip[area][cursor[area]++] = ch;
				printf("%c", ch);
				cnt[area][box[area]]++;

				if (cnt[area][box[area]] == 3 && box[area] <= 3) {
					if (box[area] == 3) {
						if (area == 0) {
							memcpy(recvdata, input_ip[area], 30);
							area = 1;
						}
					}
					else {
						input_ip[area][cursor[area]++] = '.';
						box[area]++;
						cnt[area][box[area]] = 0;
					}
				}
			}
		}
		Sleep(50);
	}
	return recvdata;
}

char* InputID() {
	unsigned char ch;
	int cursor = 0;
	char input[16] = { '\0', };

	VisibleCursor(TRUE);
	while (SERVER_LIVE) {
		textcolor(WHITE, BLACK);
		gotoxy(30 + cursor, 20); printf("_");
		if (kbhit() == 1) {
			ch = getch();
			if (ch == ESC) {
				cursor = 0;
				memset(input, NULL, 16);
				gotoxy(30, 20); printf("                         ");
			}
			else if (ch == ENTER) {
				// 입력 존재 시, 리턴
				gotoxy(30 + cursor, 20); printf(" ");
				if (cursor != 0) {
					return input;
				}
			}
			else if (ch == TAB) {
				// 입력 존재 시, 리턴
				if (cursor != 0) {
					return input;
				}
			}
			else if (ch == BACKSPACE) {
				gotoxy(45, 20); printf("           ");
				if (cursor != 0) {
					gotoxy(29 + cursor, 20);
					printf("  ");
					input[cursor--] = NULL;
				}
			}
			else if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
				// 입력는 영문과 숫자로 제한
				gotoxy(45, 20); printf("           ");
				gotoxy(31 + cursor, 20); printf("_");
				gotoxy(30 + cursor, 20);

				if (cursor < 15) { // 글자 수 제한
					textcolor(WHITE, BLACK);
					input[cursor++] = ch;
					printf("%c", ch);
				}
				else {
					gotoxy(45, 20);
					textcolor(RED2, BLACK);
					printf("입력 초과");
				}
			}
			else {
				gotoxy(45, 20);
				textcolor(RED2, BLACK);
				printf("잘못된 입력");
			}
		}
		ShowServerInfo(61, 21);
		Sleep(50);
	}
}
char* InputPwd() {
	unsigned char ch;
	int cursor = 0;
	char input[16] = { '\0', };

	VisibleCursor(TRUE);
	while (SERVER_LIVE) {
		textcolor(WHITE, BLACK);
		gotoxy(30 + cursor, 23); printf("_");
		if (kbhit() == 1) {
			ch = getch();
			if (ch == ESC) {
				gotoxy(30 + cursor, 23); printf(" ");
				if (cursor != 0) {
					// 입력 존재 시, 삭제
					cursor = 0;
					memset(input, NULL, 16);
					gotoxy(30, 23); printf("                         ");

				}
				else {
					// 입력이 없으면 아이디 입력창으로 복귀
					gotoxy(30, 20); printf("                         ");
					return NULL;
				}
			}
			else if (ch == ENTER) {
				// 입력이 있으면 리턴
				gotoxy(30 + cursor, 23); printf(" ");
				if (cursor != 0) {
					return input;
				}
			}
			else if (ch == BACKSPACE) {
				gotoxy(45, 23); printf("           ");
				if (cursor != 0) {
					gotoxy(29 + cursor, 23);
					printf("  ");
					input[cursor--] = NULL;
				}

			}
			else if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >='A' && ch <='Z')){
				// 입력는 영문과 숫자로 제한
				gotoxy(45, 23); printf("           ");
				gotoxy(31 + cursor, 23); printf("_");
				gotoxy(30 + cursor, 23);
				
				if (cursor < 15) { // 글자 수 제한
					textcolor(WHITE, BLACK);
					input[cursor++] = ch;
					printf("*");
				}
				else {
					gotoxy(45, 23);
					textcolor(RED2, BLACK);
					printf("입력 초과");
				}
			}
			else {
				gotoxy(45, 22);
				textcolor(RED2, BLACK);
				printf("잘못된 입력");
			}
		}
		ShowServerInfo(61, 21);
		Sleep(50);
	}
}
