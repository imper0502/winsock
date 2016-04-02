/* client*/

#include <stdio.h> 
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024

void main() {
  /**宣告區******************************************/
  WSADATA wsadata;
  SOCKET sd;
  struct sockaddr_in serv;
  
  u_short port = 4321;
  int n = 0;
  char str[MAXLINE];
  /**初始化區****************************************/ 
  WSAStartup(0x101, &wsadata);
  sd = socket(AF_INET, SOCK_STREAM, 0);
  
  serv.sin_family = AF_INET;
  serv.sin_port = htons(port);
  serv.sin_addr.s_addr = inet_addr("127.0.0.1");
  /**改port功能**/ 
  printf("INPUT PORT(default:4321) >> ");
  gets(str);
  if(strlen(str)!=0) {
    port=(u_short)atoi(str);
    if(port != (u_short)4321)serv.sin_port = htons(port);
  }
  
  /**工作區*****************************************/ 
  printf("Client: waiting for server...\n");
  while(connect(sd, (LPSOCKADDR)&serv, sizeof(serv))==-1);
  
  printf("INPUT >> ");
  gets(str);
  if(strlen(str)==0)strcpy(str, "How are you?");
  
  send(sd, str, strlen(str), 0);
  printf("client-send: %s\n", str);
  
  n=recv(sd, str, MAXLINE, 0);  //n = 字串長度 
  str[n] = '\0';
  printf("client-recv: %s\n", str);   

  while(1){
    sd = socket(AF_INET, SOCK_STREAM, 0);
    connect(sd, (LPSOCKADDR)&serv, sizeof(serv));   
  /**************************************************/
    do {
      printf("INPUT >> ");
      gets(str);
    }while(strlen(str)==0);
    if(strcmp(str, "quit")==0||strcmp(str, "exit")==0) break;
  /**************************************************/
    send(sd, str, strlen(str), 0);
    printf("client-send: %s\n", str);
  
    n=recv(sd, str,MAXLINE,0);  //n = 字串長度 
    str[n] = '\0';
    printf("client-recv: %s\n", str);
        
  }
  closesocket(sd);
  WSACleanup();
}
