/****
**
** client-a
**
****/

#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024

void main() {
  /*
  ** 宣告
  **/

  WSADATA wsadata;
  SOCKET sd;
  struct sockaddr_in serv;
  // u_short port;
  int n = 0;
  char str[MAXLINE], myIP[17];

  /*
  ** 呼叫 WSAStrartup() 註冊 WinSock DLL 的使用
  **/

  if(WSAStartup(0x101, (LPWSADATA)&wsadata) != 0) {
    fprintf(stderr, "\necho_cli: WSAStartup() fails!!!\n");
    exit(1);
  }

  /*
  ** 指定socket 的 IP & port
  **/

  serv.sin_family = AF_INET;
  serv.sin_addr.s_addr = inet_addr("127.0.0.1");
  serv.sin_port = htons(IPPORT_ECHO);
  /*
  printf("Echo server IP: ");
  fgets(myIP, MAXLINE, stdin);
  if(strlen(myIP) == 0) {
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");
  }else if((serv.sin_addr.s_addr = inet_addr(myIP)) == 0){
    printf("ERROR!\n");
  }

  printf("PORT >> ");
  fgets(str, MAXLINE, stdin);
  if(strlen(str) == 0) {
    serv.sin_port = htons(IPPORT_ECHO);
  }else {
    port = (u_short)atoi(str);
    serv.sin_port = htons(port);
  }
  */

  /*
  ** 開啟 TCP socket
  **/

  if((sd = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR) {
    fprintf(stderr, "\necho_cli: Can't open TCP socket.\n");
    exit(1);
  }

  /**********************
  ** 工作區
  ***********************/

  printf("Client: waiting for server...\n");
  if(connect(sd, (LPSOCKADDR)&serv, sizeof(serv))== SOCKET_ERROR) {
    fprintf(stderr, "\necho_cli: Can't connect to echo server.\n");
    exit(1);
  }

  while(1) {
    printf("INPUT >> ");
    while( fgets(str, MAXLINE, stdin) != NULL) {
      //=======================================
      // TO DO:
      if(strcmp(str, "over\n\0")==0) {
        break;
      }
      //=======================================
      if(send(sd, str, strlen(str), 0) == SOCKET_ERROR) {
        fprintf(stderr, "\necho_cli: send() error!!!\n");
        break;
      }
      printf("<<< client: %s", str);
      if((n = recv(sd, str,MAXLINE,0)) == 0) {
        fprintf(stderr, "\necho_cli: Connection closed.\n");
        break;
      }else if(n == SOCKET_ERROR) {
        fprintf(stderr, "\necho_cli: recv() error!!!\n");
        break;
      }else {
        str[n-1] = '\0';
        printf(">>> client: %s\n", str);
      }
      break;
    }
  }
  closesocket(sd);
  WSACleanup();
}