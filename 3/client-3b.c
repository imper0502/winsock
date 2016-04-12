/* client-b*/

#include <stdio.h> 
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024

void main() {
  /**宣告區******************************************/
  WSADATA wsadata;
  SOCKET sd;
  struct sockaddr_in serv;
  
  //u_short port = 4321;
  int n = 0;
  char str[MAXLINE];
  /**初始化區****************************************/ 
  if(WSAStartup(0x101, (LPWSADATA)&wsadata) != 0) {
    fprintf(stderr, "echo_cli: WSAStartup() fails!!!\n");
    exit(1);
  }
  
  //printf("Echo server IP: ");
  //fgets(str, MAXLINE, stdin);
  
  serv.sin_family = AF_INET;
  serv.sin_addr.s_addr = inet_addr("127.0.0.2");
  serv.sin_port = htons(IPPORT_ECHO);
  
  if((sd = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR) {
    fprintf(stderr, "echo_cli: Can't open TCP socket.\n");
    exit(1);
  }
  /**工作區*****************************************/ 
  printf("Client: waiting for server...\n");
  if(connect(sd, (LPSOCKADDR)&serv, sizeof(serv))== SOCKET_ERROR) {
    fprintf(stderr, "echo_cli: Can't connect to echo server.\n");
    exit(1);
  }
  while(1) {
    if((n=recv(sd, str,MAXLINE,0)) == 0) {
      fprintf(stderr, "echo_cli: Connection closed.\n");
      break;
    }else if(n == SOCKET_ERROR) {
      fprintf(stderr, "echo_cli: recv() error!!!\n");
      break;
    }else if(strcmp(str, "over\n\0")==0) {
      printf("INPUT >> ");
      break;
    }else {
      str[n] = '\0';
      printf("client <<< %s", str);
    }
  }
  closesocket(sd);
  WSACleanup();
}
