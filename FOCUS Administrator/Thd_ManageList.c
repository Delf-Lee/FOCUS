#include "Admin.h"
#include "Thread.h"
char message[BUFSIZE];

// 예외 상황 발생 시, 리스트에서 클라이언트 삭제
void DeleteClient(int i) {
	closesocket(Client_List[i].Socket);
	memset(&Client_List[i], NULL, sizeof(State));
	memcpy(&Client_List[i], &Client_List[i + 1], sizeof(State)*(howmany_C - 1 - i));
	howmany_C--;
}

UINT WINAPI ManageList(void *arg) {
	int i;
	SOCKET Client;
	TIME Now;
	unsigned int option_value = 1; // 소켓 옵션 설정

	// seclet()의 timeout을 조절
	struct timeval val;
	val.tv_sec = 0;
	val.tv_usec = 100;

	// 소켓 생성 (ADMIN ↔ CLIENT) , 오류 제어
	Server_Socket = socket(PF_INET, SOCK_STREAM, 0);
	if (Server_Socket == INVALID_SOCKET) {
		printf("#error here 1\n");
		Sleep(10000);
		exit(1);
	}

	// 소켓 옵션: 주소 재활용
	setsockopt(Server_Socket, SOL_SOCKET, SO_REUSEADDR, (char *)&option_value, sizeof(option_value));

	memset((char *)&sockaddr_in, 0, sizeof(sockaddr_in));
	sockaddr_in.sin_family = AF_INET; // ipv4를 사용
	sockaddr_in.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	sockaddr_in.sin_port = htons(5455); // 클라이언트와 연결될 포트 넘버

	// 소켓에 주소 부여, 오류제어
	if (bind(Server_Socket, (SOCKADDR *)&sockaddr_in, sizeof(sockaddr_in)) < 0) {
		printf("#error here 3\n");
		Sleep(10000);
		exit(1);
	}
	
	if (listen(Server_Socket, CLIENT_SIZE) == -1) {
		printf("#error here 4\n");
		Sleep(10000);
		exit(1);
	}

	while (SERVER_LIVE) {

		Now = time(NULL);
		FD_ZERO(&Socket_Arr);

		FD_SET(Server_Socket, &Socket_Arr);

		for (i = 0; i < howmany_C; i++)
			FD_SET(Client_List[i].Socket, &Socket_Arr);

		Max_Socket = Get_Max_Sock(&Socket_Arr, Server_Socket, ADMINISTRATOR_STATE_SIZE) + 1;

		if (select(Max_Socket, &Socket_Arr, (fd_set *)0, (fd_set *)0, &val) < 0) {
			gotoxy(1, 1);
			printf("select error #55\n");
			Sleep(1000000);
			exit(0);
		}

		// 디스크립터 변화 확인
		if (FD_ISSET(Server_Socket, &Socket_Arr)) { // fd로 전달된 파일 디스크립터 정보가 있으면 양수 반환
			sockaddr_size = sizeof(SOCKADDR);
			Client = accept(Server_Socket, (SOCKADDR*)&sockaddr_in, &sockaddr_size);

			if (Client == -1) {
				gotoxy(1, 1);
				printf("accept error\n");
				Sleep(100000); exit(0);
			}
			else {
				Client_List[howmany_C++].Socket = Client;
				Client_List[i].Time = time(NULL);
				continue;
			}
		}

		// 클라이언트 관리
		for (i = 0; i < howmany_C; i++) {
			// A. 응답 없는 클라이언트 관리
			if ((Now - Client_List[i].Time)  > FREQUENT) {
				DeleteClient(i);
				continue;
			}
			// B. 정기보고 수신
			if (FD_ISSET(Client_List[i].Socket, &Socket_Arr)) {
				memset(message, NULL, sizeof(message));
				
				// 헤더 수신 성공
				if (recv(Client_List[i].Socket, message, HEADER_SIZE, 0) > 0) {
					memset(message, NULL, sizeof(message));
					// 클라이언트 정보 수신, 리스트에 정보 저장
					recv(Client_List[i].Socket, message, CLIENT_SIZE, 0);
					strcpy(Client_List[i].IP_ADDR, message);
					strcpy(Client_List[i].MAC_ADDR, message + 30);
					Client_List[i].Time = time(NULL);
					// ACK 회신
					if (Send_Packet(Client_List[i].Socket, "hello", 6, ADMINISTRATOR, ACK, 0) == -1) {
						DeleteClient(i);
						continue;
					}
					else {
						Client_List[i].Time = time(NULL);
					}
				}
				else {
					DeleteClient(i);
				}
			} // B
		} // A
	}
	return 0;
}