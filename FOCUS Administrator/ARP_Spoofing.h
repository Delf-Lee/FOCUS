//#ifndef _ARP_
//#define _ARP_
//
//#include "pcap.h"
//
//typedef struct arp {
//	SOCKET Socket;
//	struct eth_h {
//		unsigned char Dest_H_Addr[6];
//		unsigned char Send_H_Addr[6];
//		unsigned short TYPE;
//	}ethh;
//	struct arp_h {
//		unsigned short Hardware_Type;
//		unsigned short Protocol_Type;
//		unsigned char Hardware_Addr_Length;
//		unsigned char Protocol_Length;
//		unsigned short Operation;
//		unsigned char Sender_H_Addr[6];
//		unsigned char Sender_IP_Addr[4];
//		unsigned char Target_H_Addr[6];
//		unsigned char Target_IP_Addr[4];
//	}arph;
//}arp_t;
//
//typedef struct arp_list {
//	arp_t *list;
//	int size;
//	int howmany;
//}ARP_List;
//
//#endif