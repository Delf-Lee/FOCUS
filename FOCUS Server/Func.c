#include "header.h"

SOCKET Get_Max_Sock(const FD_SET *Socket_Arr, int k, int size) {
	SOCKET max = k;
	int i;

	for (i = 0; i < size; i++) {
		if (Socket_Arr->fd_array[i] > max) {
			max = Socket_Arr->fd_array[i];
		}
	}
	return max;
}

int Show_User(State_A *list_A, State_C *list_C, int howmany_A, int howmany_C) {
	int i;
	char *tmp;

	system("cls");


	for (i = 0; i < howmany_A; i++) {
		tmp = &list_A[i];
		printf("ADMIN-%d\n %d	%s	%s\n", i + 1, list_A[i].Socket, list_A[i].IP_ADDR, list_A[i].MAC_ADDR);
	}

	printf("--------------------------------------------------------------------------------\n");
	for (i = 0; i < howmany_C; i++) {
		tmp = &list_A[i];
		printf("CLIENT-%d\n %d	%s	%s\n", i + 1, list_C[i].Socket, list_C[i].IP_ADDR, list_C[i].MAC_ADDR);
	}
	return 0;
}
