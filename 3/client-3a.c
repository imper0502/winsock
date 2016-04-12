/* client-a*/

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
  serv.sin_addr.s_addr = inet_addr("127.0.0.1");
  serv.sin_port = htons(IPPORT_ECHO);
  
  if((sd = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR) {
    fprintf(stderr, "echo_cli: Can't open TCP socket.\n");
    exit(1);
  }
  /**改port功能 
  printf("INPUT PORT(default:4321) >> ");
  gets(str);
  if(strlen(str)!=0) {
    port=(u_short)atoi(str);
    if(port != (u_short)4321)serv.sin_port = htons(port);
  }
  
  /**工作區*****************************************/ 
  printf("Client: waiting for server...\n");
  if(connect(sd, (LPSOCKADDR)&serv, sizeof(serv))== SOCKET_ERROR) {
    fprintf(stderr, "echo_cli: Can't connect to echo server.\n");
    exit(1);
  }
  /*
  printf("INPUT >> ");
  gets(str);
  if(strlen(str)==0)strcpy(str, "How are you?");
  
  send(sd, str, strlen(str), 0);
  printf("client >>> %s\n", str);
  
  n=recv(sd, str, MAXLINE, 0);  //n = 字串長度 
  str[n] = '\0';
  printf("client-recv: %s\n", str);
  
  */
  printf("INPUT >> ");
  while( fgets(str, MAXLINE, stdin) != NULL) {
    if(strcmp(str, "quit\n")==0||strcmp(str, "exit\n")==0) break;
    if(send(sd, str, strlen(str), 0) == SOCKET_ERROR) {
      fprintf(stderr, "echo_cli: send() error!!!\n");
      break;
    }
    printf("client >>> %s", str);
    if(strcmp(str, "over\n\0")==0) {
      printf("INPUT >> ");
      continue;
    }
    if((n=recv(sd, str,MAXLINE,0)) == 0) {
      fprintf(stderr, "echo_cli: Connection closed.\n");
      break;
    }else if(n == SOCKET_ERROR) {
      fprintf(stderr, "echo_cli: recv() error!!!\n");
      break;
    }else {
      str[n] = '\0';
      printf("client <<< %s", str);
    }
  /**************************************************/
    printf("INPUT >> ");
  }
  closesocket(sd);
  WSACleanup();
}
