#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024
#define CHANNELNUM 3 
int main(int argc, char** argv) {
  //宣告與初始化=====================================
  WSADATA  wsadata;
  SOCKET  sd;  
  struct sockaddr_in serv, cli; 
  int n, serv_len = sizeof(serv), cli_len = sizeof(cli), i;
  boolean group[CHANNELNUM];
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
  struct ip_mreq multicastRequest[CHANNELNUM];
  
  multicastRequest[0].imr_multiaddr.s_addr = inet_addr("224.1.1.10");
  multicastRequest[1].imr_multiaddr.s_addr = inet_addr("224.1.1.20");
  multicastRequest[2].imr_multiaddr.s_addr = inet_addr("224.1.1.30");
  
	for(i=0; i<CHANNELNUM; i++) {
	  multicastRequest[i].imr_interface.s_addr = htonl(INADDR_ANY);
	}
  printf("按下數字1, 2, 3可以進入或離開群組\n按下'c'清除畫面\n");
  // 加入群組
  hp = setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multicastRequest[0], sizeof(multicastRequest[0]));
  if(hp < 0) {
	  printf("setsockopt() failed.\n");
  }else {
    printf("加入群組1: %s\n", inet_ntoa(multicastRequest[0].imr_multiaddr));
    group[0] = 1;
  }
  
  // Bind
  hp = bind(sd, (LPSOCKADDR) &cli, cli_len);
  if(hp < 0) {
	  printf("Bind error!\n");
  }
  sleep(1);
  // 工作區========================================
  while(1){
  	// 鍵盤控制 
		if (_kbhit()) {
			// 取得輸入的字元
			char ch = _getch();
			switch(ch) {
      case '1':
        if(group[0]) {
        	if(group[1] == 1 ||group[2] == 1) {
        	  hp = setsockopt(sd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &multicastRequest[0], sizeof(multicastRequest[0]));
            if(hp < 0) {
	            printf("setsockopt() failed.\n");
            }else {
              printf("離開群組1: %s\n", inet_ntoa(multicastRequest[0].imr_multiaddr));
              group[0] = 0;
            }
					}else {
            printf("至少要在一個群組中，現在在群組1中\n");
          }
				}else {
          // 加入群組
          hp = setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multicastRequest[0], sizeof(multicastRequest[0]));
          if(hp < 0) {
	          printf("setsockopt() failed.\n");
          }else {
            printf("加入群組1: %s\n", inet_ntoa(multicastRequest[0].imr_multiaddr));
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
              printf("離開群組2: %s\n", inet_ntoa(multicastRequest[1].imr_multiaddr));
              group[1] = 0;
            }
          }else {
            printf("至少要在一個群組中，現在在群組2中\n");
          }
				}else {
          hp = setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multicastRequest[1], sizeof(multicastRequest[1]));
          if(hp < 0) {
	          printf("setsockopt() failed.\n");
          }else {
            printf("加入群組2: %s\n", inet_ntoa(multicastRequest[1].imr_multiaddr));
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
              printf("離開群組3: %s\n", inet_ntoa(multicastRequest[2].imr_multiaddr));
              group[2] = 0;
            }
          }else {
            printf("至少要在一個群組中，現在在群組3中\n");
          }
				}else {
          hp = setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multicastRequest[2], sizeof(multicastRequest[2]));
          if(hp < 0) {
	          printf("setsockopt() failed.\n");
          }else {
            printf("加入群組3: %s\n", inet_ntoa(multicastRequest[2].imr_multiaddr));
            group[2] = 1;
          }
				}	
				break;
			case 'c':
      // 清理螢幕
				system("cls");
				if(group[0] == 1) printf("正在群組1中\n");
        if(group[1] == 1) printf("正在群組2中\n");
        if(group[2] == 1) printf("正在群組3中\n");
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
