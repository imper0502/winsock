#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024
#define clientNum 1
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

  // 設定群播功能=====================================
  int multicastTTL = 1;
  hp = setsockopt(serv_sd, IPPROTO_IP, 1, &multicastTTL, sizeof(multicastTTL));
  if(hp == SOCKET_ERROR) {
    printf("setsockopt() failed.\n");
  }

  // 指定 socket 的 IP 位址和 port number
  // 因為要廣播，所以特別設定client IP。
  unsigned short port = 0;
  printf("Server will multicast.\n");

  cli_len[0] = sizeof(cli[0]);
  cli[0].sin_family = AF_INET;
  cli[0].sin_addr.s_addr = inet_addr("224.1.1.10");
  cli[0].sin_port = htons(55549);
  
  // printf("YOU CAN SET PORT. (1111~99999) >>");
  // fgets(str, MAXLINE, stdin);
  // str[strlen(str)-1]='\0'; 
  // if(strlen(str) == 0) {
  //   printf("Use default.\n");
  // }else {
  //   port = (u_short)atoi(str);
  //   for(i=1; i<clientNum; i++) {
  //     if(port==ntohs(cli[i].sin_port)) {
  //       printf("The port is used!\n");
  //       return 0;
  //     }
  //   }
  //   cli[0].sin_port = htons(port);
  // }

  
  // 工作區================================================
  while(1) {
    memset(tmp, i%10+'0', sizeof(tmp));
    sendto(serv_sd, tmp, strlen(tmp), 0, (LPSOCKADDR)&cli[0], cli_len[0]);
    sleep(1);
    i++;
  }

  //========================================================
  closesocket(serv_sd);

  //結束 WinSock DLL 的使用=================================
  WSACleanup();
  return 0;
}
