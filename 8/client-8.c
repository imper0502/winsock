#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024
#define CHANNELNUM 3 
int main(int argc, char** argv) {
  //�ŧi�P��l��=====================================
  WSADATA  wsadata;
  SOCKET  sd;  
  struct sockaddr_in serv, cli; 
  int n, serv_len = sizeof(serv), cli_len = sizeof(cli), i;
  boolean group[CHANNELNUM];
  char  str[MAXLINE]="",tmp[MAXLINE]="";

  // �I�s WSAStartup() ���U WinSock DLL ���ϥ�
  int nResult = WSAStartup(0x101, (LPWSADATA)&wsadata);
  if(nResult!=0) {
    printf("WSA Initialization failed: %d\n", nResult);
    WSACleanup();
    return 0;
  }

  // �}�� UDP socket
  LPHOSTENT hp = (sd = socket(AF_INET, SOCK_DGRAM, 0));
  if(hp == SOCKET_ERROR) {
    printf("get hp error, code: %d\n", WSAGetLastError());
  }

  // �]�w����
  cli.sin_family       = AF_INET;
  cli.sin_addr.s_addr  = 0;
  cli.sin_port         = htons(55549);
  // ��J�s����}
  struct ip_mreq multicastRequest[CHANNELNUM];
  
  multicastRequest[0].imr_multiaddr.s_addr = inet_addr("224.1.1.10");
  multicastRequest[1].imr_multiaddr.s_addr = inet_addr("224.1.1.20");
  multicastRequest[2].imr_multiaddr.s_addr = inet_addr("224.1.1.30");
  
	for(i=0; i<CHANNELNUM; i++) {
	  multicastRequest[i].imr_interface.s_addr = htonl(INADDR_ANY);
	}
  printf("���U�Ʀr1, 2, 3�i�H�i�J�����}�s��\n���U'c'�M���e��\n");
  // �[�J�s��
  hp = setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multicastRequest[0], sizeof(multicastRequest[0]));
  if(hp < 0) {
	  printf("setsockopt() failed.\n");
  }else {
    printf("�[�J�s��1: %s\n", inet_ntoa(multicastRequest[0].imr_multiaddr));
    group[0] = 1;
  }
  
  // Bind
  hp = bind(sd, (LPSOCKADDR) &cli, cli_len);
  if(hp < 0) {
	  printf("Bind error!\n");
  }
  sleep(1);
  // �u�@��========================================
  while(1){
  	// ��L���� 
		if (_kbhit()) {
			// ���o��J���r��
			char ch = _getch();
			switch(ch) {
      case '1':
        if(group[0]) {
        	if(group[1] == 1 ||group[2] == 1) {
        	  hp = setsockopt(sd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &multicastRequest[0], sizeof(multicastRequest[0]));
            if(hp < 0) {
	            printf("setsockopt() failed.\n");
            }else {
              printf("���}�s��1: %s\n", inet_ntoa(multicastRequest[0].imr_multiaddr));
              group[0] = 0;
            }
					}else {
            printf("�ܤ֭n�b�@�Ӹs�դ��A�{�b�b�s��1��\n");
          }
				}else {
          // �[�J�s��
          hp = setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multicastRequest[0], sizeof(multicastRequest[0]));
          if(hp < 0) {
	          printf("setsockopt() failed.\n");
          }else {
            printf("�[�J�s��1: %s\n", inet_ntoa(multicastRequest[0].imr_multiaddr));
            group[0] = 1;
          }
				}		
				break;
      case '2':
        if(group[1]) {
        	if(group[0] == 1 ||group[2] == 1) {
            hp = setsockopt(sd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &multicastRequest[1], sizeof(multicastRequest[1]));
            if(hp < 0) {
	            printf("setsockopt() failed.\n");
            }else {
              printf("���}�s��2: %s\n", inet_ntoa(multicastRequest[1].imr_multiaddr));
              group[1] = 0;
            }
          }else {
            printf("�ܤ֭n�b�@�Ӹs�դ��A�{�b�b�s��2��\n");
          }
				}else {
          hp = setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multicastRequest[1], sizeof(multicastRequest[1]));
          if(hp < 0) {
	          printf("setsockopt() failed.\n");
          }else {
            printf("�[�J�s��2: %s\n", inet_ntoa(multicastRequest[1].imr_multiaddr));
            group[1] = 1;
          }
				}	
				break;
			case '3':
        if(group[2]) {
        	if(group[0] == 1 || group[1] == 1) {
            hp = setsockopt(sd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &multicastRequest[2], sizeof(multicastRequest[2]));
            if(hp < 0) {
	            printf("setsockopt() failed.\n");
            }else {
              printf("���}�s��3: %s\n", inet_ntoa(multicastRequest[2].imr_multiaddr));
              group[2] = 0;
            }
          }else {
            printf("�ܤ֭n�b�@�Ӹs�դ��A�{�b�b�s��3��\n");
          }
				}else {
          hp = setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multicastRequest[2], sizeof(multicastRequest[2]));
          if(hp < 0) {
	          printf("setsockopt() failed.\n");
          }else {
            printf("�[�J�s��3: %s\n", inet_ntoa(multicastRequest[2].imr_multiaddr));
            group[2] = 1;
          }
				}	
				break;
			case 'c':
      // �M�z�ù�
				system("cls");
				if(group[0] == 1) printf("���b�s��1��\n");
        if(group[1] == 1) printf("���b�s��2��\n");
        if(group[2] == 1) printf("���b�s��3��\n");
				break;
			}
		}
    n = recvfrom(sd, str, MAXLINE, 0, (LPSOCKADDR)&serv, &serv_len);
    str[n]='\0';
    printf("Message form Server[%s:%d]: %s\n", inet_ntoa(serv.sin_addr), ntohs(serv.sin_port), str);
  }
  // ���� socket
  closesocket(sd);

  // ���� WinSock DLL ���ϥ�
  WSACleanup();
  return 0;
}
