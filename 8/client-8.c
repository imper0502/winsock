#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024

int main(int argc, char** argv) {
  //�ŧi�P��l��=====================================
  WSADATA  wsadata;
  SOCKET  sd;  
  struct sockaddr_in serv, cli; 
  int n, serv_len = sizeof(serv), cli_len = sizeof(cli), i;
  boolean group[2];
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
  struct ip_mreq multicastRequest[MAXLINE];
  multicastRequest[0].imr_multiaddr.s_addr = inet_addr("224.1.1.10");
  multicastRequest[0].imr_interface.s_addr = htonl(INADDR_ANY);
  multicastRequest[1].imr_multiaddr.s_addr = inet_addr("224.1.1.20");
  multicastRequest[1].imr_interface.s_addr = htonl(INADDR_ANY);
  
  // �[�J�s��
  hp = setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multicastRequest[0], sizeof(multicastRequest[0]));
  if(hp < 0) {
	  printf("setsockopt() failed.\n");
  }else {
    printf("�[�J�s��: %s\n", inet_ntoa(multicastRequest[0].imr_multiaddr));
    group[0] = 1;
  }
  
  // Bind
  hp = bind(sd, (LPSOCKADDR) &cli, cli_len);
  if(hp < 0) {
	  printf("Bind error!\n");
  }
  n = recvfrom(sd, tmp, MAXLINE, 0, (LPSOCKADDR)&serv, &serv_len);
  tmp[n]='\0';
  printf("Server[%s:%d]\n", inet_ntoa(serv.sin_addr), ntohs(serv.sin_port));
  
  
  // �u�@��========================================
  while(1){
  	// �ϥΪ̫��U��L
		if (_kbhit()) {
			// ���o��J���r��
			char ch = _getch();
			switch(ch) {
      case 'a':
        if(group[0]) {
        	if(group[1] == 1) {
        	  hp = setsockopt(sd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &multicastRequest[0], sizeof(multicastRequest[0]));
            if(hp < 0) {
	            printf("setsockopt() failed.\n");
            }else {
              printf("���}�s��A: %s\n", inet_ntoa(multicastRequest[0].imr_multiaddr));
              group[0] = 0;
            }
					}else {
            printf("�ܤ֭n�b�@�Ӹs�դ�\n");
          }
				}else {
          // �[�J�s��
          hp = setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multicastRequest[0], sizeof(multicastRequest[0]));
          if(hp < 0) {
	          printf("setsockopt() failed.\n");
          }else {
            printf("�[�J�s��A: %s\n", inet_ntoa(multicastRequest[0].imr_multiaddr));
            group[0] = 1;
          }
				}		
				break;
      case 'b':
        if(group[1]) {
        	if(group[0] == 1) {
            hp = setsockopt(sd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &multicastRequest[1], sizeof(multicastRequest[1]));
            if(hp < 0) {
	            printf("setsockopt() failed.\n");
            }else {
              printf("���}�s��B: %s\n", inet_ntoa(multicastRequest[1].imr_multiaddr));
              group[1] = 0;
            }
          }else {
            printf("�ܤ֭n�b�@�Ӹs�դ�\n");
          }
				}else {
          hp = setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multicastRequest[1], sizeof(multicastRequest[1]));
          if(hp < 0) {
	          printf("setsockopt() failed.\n");
          }else {
            printf("�[�J�s��B: %s\n", inet_ntoa(multicastRequest[1].imr_multiaddr));
            group[1] = 1;
          }
				}	
				break;
			case 'c':
      // �M�z�ù�
				system("cls");
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
