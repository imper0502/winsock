#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024

int main(int argc, char** argv) {
  //宣告與初始化=====================================
  WSADATA  wsadata;
  SOCKET  sd;  
  struct sockaddr_in serv, cli; 
  int n, serv_len = sizeof(serv), cli_len = sizeof(cli), i;
  boolean group[2];
  char  str[MAXLINE]="",tmp[MAXLINE]="";

  // 呼叫 WSAStartup() 註冊 WinSock DLL 的使用
  int nResult = WSAStartup(0x101, (LPWSADATA)&wsadata);
  if(nResult!=0) {
    printf("WSA Initialization failed: %d\n", nResult);
    WSACleanup();
    return 0;
  }

  // 開啟 UDP socket
  LPHOSTENT hp = (sd = socket(AF_INET, SOCK_DGRAM, 0));
  if(hp == SOCKET_ERROR) {
    printf("get hp error, code: %d\n", WSAGetLastError());
  }

  // 設定本機
  cli.sin_family       = AF_INET;
  cli.sin_addr.s_addr  = 0;
  cli.sin_port         = htons(55549);
  // 填入群播位址
  struct ip_mreq multicastRequest[MAXLINE];
  multicastRequest[0].imr_multiaddr.s_addr = inet_addr("224.1.1.10");
  multicastRequest[0].imr_interface.s_addr = htonl(INADDR_ANY);
  multicastRequest[1].imr_multiaddr.s_addr = inet_addr("224.1.1.20");
  multicastRequest[1].imr_interface.s_addr = htonl(INADDR_ANY);
  
  // 加入群組
  hp = setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multicastRequest[0], sizeof(multicastRequest[0]));
  if(hp < 0) {
	  printf("setsockopt() failed.\n");
  }else {
    printf("加入群組: %s\n", inet_ntoa(multicastRequest[0].imr_multiaddr));
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
  
  
  // 工作區========================================
  while(1){
  	// 使用者按下鍵盤
		if (_kbhit()) {
			// 取得輸入的字元
			char ch = _getch();
			switch(ch) {
      case 'a':
        if(group[0]) {
        	if(group[1] == 1) {
        	  hp = setsockopt(sd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &multicastRequest[0], sizeof(multicastRequest[0]));
            if(hp < 0) {
	            printf("setsockopt() failed.\n");
            }else {
              printf("離開群組A: %s\n", inet_ntoa(multicastRequest[0].imr_multiaddr));
              group[0] = 0;
            }
					}else {
            printf("至少要在一個群組中\n");
          }
				}else {
          // 加入群組
          hp = setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multicastRequest[0], sizeof(multicastRequest[0]));
          if(hp < 0) {
	          printf("setsockopt() failed.\n");
          }else {
            printf("加入群組A: %s\n", inet_ntoa(multicastRequest[0].imr_multiaddr));
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
              printf("離開群組B: %s\n", inet_ntoa(multicastRequest[1].imr_multiaddr));
              group[1] = 0;
            }
          }else {
            printf("至少要在一個群組中\n");
          }
				}else {
          hp = setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multicastRequest[1], sizeof(multicastRequest[1]));
          if(hp < 0) {
	          printf("setsockopt() failed.\n");
          }else {
            printf("加入群組B: %s\n", inet_ntoa(multicastRequest[1].imr_multiaddr));
            group[1] = 1;
          }
				}	
				break;
			case 'c':
      // 清理螢幕
				system("cls");
				break;
			}
		}
    n = recvfrom(sd, str, MAXLINE, 0, (LPSOCKADDR)&serv, &serv_len);
    str[n]='\0';
    printf("Message form Server[%s:%d]: %s\n", inet_ntoa(serv.sin_addr), ntohs(serv.sin_port), str);
  }
  // 關閉 socket
  closesocket(sd);

  // 結束 WinSock DLL 的使用
  WSACleanup();
  return 0;
}
