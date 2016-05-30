#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024
#define clientNum 3
int main() {
  // 宣告與初始化========================================
  WSADATA wsadata;
  SOCKET  serv_sd;
  struct  sockaddr_in  serv, cli[clientNum];
  int     n, cli_len[clientNum], i, j, k;
  char    str[MAXLINE], tmp[2][15];

  // 呼叫 WSAStrartup() 註冊 WinSock DLL 的使用
  int nResult = WSAStartup(0x101, (LPWSADATA)&wsadata);
  if(nResult!=0) {
    printf("WSA Initialization failed: %d\n", nResult);
    WSACleanup();
    return 0;
  }

  // 開啟 UDP socket
  LPHOSTENT hp = (serv_sd = socket(AF_INET, SOCK_DGRAM, 0));
  if(hp == SOCKET_ERROR) {
    printf("get hp error, code: %d\n", WSAGetLastError());
  }
  
  // 查詢本機(Server)資訊=====================================
  printf("What is local host's name & IP?\n");
  hp = gethostname((LPSTR)str, MAXLINE);
  if(hp != 0) {
    printf("get hp error, code: %d\n\n", WSAGetLastError());
  }else {
    hp = gethostbyname(str);
    if(hp == NULL){
      printf("get hp error, code: %d\n", WSAGetLastError());
    }else {
      printf("host name: %s\n", hp->h_name);
      printf("host IP: %s\n", inet_ntoa(*(LPIN_ADDR)(hp->h_addr)));
    }
  }

  // 設定群播功能=====================================
  int multicastTTL = 1;
  hp = setsockopt(serv_sd, IPPROTO_IP, 1, &multicastTTL, sizeof(multicastTTL));
  if(hp == SOCKET_ERROR) {
    printf("setsockopt() failed.\n");
  }

  // 指定 socket 的 IP 位址和 port number
  printf("Server will multicast.\n");
  for(i=0; i<clientNum; i++) {
    cli_len[i] = sizeof(cli[i]);
    cli[i].sin_family = AF_INET;
    cli[i].sin_port = htons(55549);
  }
  cli[0].sin_addr.s_addr = inet_addr("224.1.1.10");
  cli[1].sin_addr.s_addr = inet_addr("224.1.1.20");
  cli[2].sin_addr.s_addr = inet_addr("224.1.1.30");

	// 工作區================================================
	strcpy(str, "\n群組1: 頻道介紹\n群組2: 數字\n群組3: 字母\n");
  while(1) {
    memset(tmp[0], i%10+'0', 15);
    memset(tmp[1], i%26+'A', 15);
    sendto(serv_sd, str, strlen(str), 0, (LPSOCKADDR)&cli[0], cli_len[0]);
		for(j=1; j<clientNum; j++) {
      sendto(serv_sd, tmp[j-1], 15, 0, (LPSOCKADDR)&cli[j], cli_len[j]);
    }
    sleep(2);
    i++;
  }

  //========================================================
  closesocket(serv_sd);

  //結束 WinSock DLL 的使用=================================
  WSACleanup();
  return 0;
}
