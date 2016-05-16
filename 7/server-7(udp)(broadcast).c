#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024
#define clientNum 10
int main() {
  // 宣告與初始化========================================
  WSADATA wsadata;
  SOCKET  serv_sd;
  struct  sockaddr_in  serv, cli[clientNum];
  int     n, cli_len[clientNum], i, j, k;
  char    str[MAXLINE], tmp[15];

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
    printf("local host name: %s\n", str);
  }
  hp = gethostbyname(str);
  if(hp == NULL){
    printf("get hp error, code: %d\n", WSAGetLastError());
  }else {
    printf("host IP: %s\n", inet_ntoa(*(LPIN_ADDR)(hp->h_addr)));
  }

  // 設定廣播功能
  char broadcast = 'a';
  setsockopt(serv_sd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));

  // 指定 socket 的 IP 位址和 port number
  // 因為要廣播，所以特別設定client IP。
  unsigned short port = 0;
  printf("Server will broadcast.\n");
  for(i=1; i<clientNum; i++) {
    cli_len[i] = sizeof(cli[i]);
    cli[i].sin_family = AF_INET;
    cli[i].sin_addr.s_addr = inet_addr("255.255.255.255");// 此區網內所有電腦
    port = 0;
    for(j=0; j<4; j++) {
      port = (u_short)(i) + port*10;    
    }
    cli[i].sin_port = htons(port);
    printf("Channel_%i(port:%i) is OK!\n", i, ntohs(cli[i].sin_port));
  }
  cli_len[0] = sizeof(cli[0]);
  cli[0].sin_family = AF_INET;
  cli[0].sin_addr.s_addr = inet_addr("255.255.255.255");
  cli[0].sin_port = htons(5554);
  printf("YOU CAN SET PORT. (1111~9999) >>");
  fgets(str, MAXLINE, stdin);
  str[strlen(str)-1]='\0'; 
  if(strlen(str) == 0) {
    printf("Use default.\n");
  }else {
    port = (u_short)atoi(str);
    for(i=1; i<clientNum; i++) {
      if(port==ntohs(cli[i].sin_port)) {
        printf("The port is used!\n");
        return 0;
      }
    }
    cli[0].sin_port = htons(port);
  }
  
  // 工作區================================================
  while(1) {
    memset(tmp, i%10+'0', sizeof(tmp));
    for(j=0; j<clientNum; j++) {
      sendto(serv_sd, tmp, 2+j, 0, (LPSOCKADDR)&cli[j], cli_len[j]);
      sleep((j+10)/10);
    }
    i++;
  }

  //========================================================
  closesocket(serv_sd);

  //結束 WinSock DLL 的使用=================================
  WSACleanup();
  return 0;
}
