#include "Admin.h"
#include "Thread.h"
#include "UI.h"

char message[256];
int sel_clt_cnt; // 선택된 클라언트의 '수'

UINT WINAPI SendCommand_Thread(void *arg) {
	int i;
	char select;
	int Selected_Client[CLIENT_SIZE]; // 선택된 클라이언트의 '인덱스'
	reset = TRUE;
	first = TRUE;
	
	while (SERVER_LIVE) {
		int flag;
		ShowAdminInfo(59, 2);

		// 메
		if (first == TRUE) {
			ShowMainScr(1, 1, Selected_Client);
			ShowCmdMenu(21, 7, -1);
		}

		// 명령 종류 선택
		select = ShowCmdBox(17, 5);

		switch (select) {
			
		case SHUTDOWN:  // 임시로
			flag = ShutDownBox(21,7);
			if (flag != FALSE) {
				ShutDown(send_time, sel_clt_cnt, Selected_Client, flag);
				first = TRUE;
				reset = TRUE;				
			}
			break;

		case SLEEP_MODE:
			flag = SleepModeBox(21, 7);
			if (flag != FALSE) {
				SleepMode(send_time, sel_clt_cnt, Selected_Client, flag);
				first = TRUE;
				reset = TRUE;
			}
			break;

		case SEND_MESSAGE:
			textcolor(WHITE, BLACK);
			MessageSend(sel_clt_cnt, Selected_Client);
			first = TRUE;
			reset = TRUE;
			break;

		case RUN_WEBBROWSER:
			flag = RunWebBrowserBox(21, 7);  
			if (flag != FALSE) {
				RunWebBrowser(sel_clt_cnt, Selected_Client);
				first = TRUE;
				reset = TRUE;
			}
			break;

		case BLOCK_INTERNET:
			flag = BlockInternetBox(21, 7);
			BlockInternet(sel_clt_cnt, Selected_Client, flag);
			first = TRUE;
			reset = TRUE;
			break;

		case FTP:
			FileTranse(sel_clt_cnt, Selected_Client);  
			first = TRUE;
			reset = TRUE;
			break;

		default:
			break;
		}
		ShowPresentTime();
		ShowServerInfo(61, 21);
	}
	return;
}