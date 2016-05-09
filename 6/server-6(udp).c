#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024    /* 字串緩衝區長度 */

int main() {
  // 宣告=============================================
  WSADATA wsadata;
  SOCKET  serv_sd;
  struct  sockaddr_in  serv, cli;
  int     n, cli_len = sizeof(cli);
  char    str[MAXLINE];

  // 呼叫 WSAStrartup() 註冊 WinSock DLL 的使用=======
  int nResult = WSAStartup(0x101, (LPWSADATA)&wsadata);
  if(nResult!=0) {
    printf("WSA Initialization failed: %d\n", nResult);
    WSACleanup();
    return;
  }
  
  // 開啟 UDP socket==================================
  LPHOSTENT hp = (serv_sd = socket(AF_INET, SOCK_DGRAM, 0));
  if(hp == SOCKET_ERROR) {
    printf("get hp error, code: %d\n", WSAGetLastError());
  }
  
  //指定 socket 的 IP 位址和 port number==============
  serv.sin_family      = AF_INET;
  serv.sin_addr.s_addr = 0;
  serv.sin_port        = htons(5678); // 指定IPPORT_ECHO 為 echo port

  //bind==============================================
  hp = bind(serv_sd, (LPSOCKADDR) &serv, sizeof(serv));
  if(hp < 0) {
    printf("get hp error, code: %d\n", WSAGetLastError());
  }
  
  // 查詢本機名稱=====================================
  printf("What is local host's name?\n");
  hp = gethostname((LPSTR)str, MAXLINE);
  if(hp != 0) {
    printf("get hp error, code: %d\n\n", WSAGetLastError());
  }else {
    printf("local host name: %s\n", str);
    
  }
  
  // 查詢本機 IP資訊====================================
  printf("What is %s's IP?\n", str);
  hp = gethostbyname(str);
  if(hp == NULL){
    printf("get hp error, code: %d\n", WSAGetLastError());
  }else {
    printf("host name: %s\n", hp->h_name);
    printf("host nickname: %s\n", hp->h_aliases[0]);
    printf("host IP: %s\n", inet_ntoa(*(LPIN_ADDR)(hp->h_addr)));
  }

  // 工作區================================================
  while (1) {
    printf("server: waiting for client\n");

    n=recvfrom(serv_sd, str, MAXLINE, 0, (LPSOCKADDR)&cli, &cli_len);
    str[n]='\0';
    printf("server: client->server: %s\n",str);

    if(strcmp(str,"How are you?\0")==0)
    strcpy(str,"Fine, thank you!");
    else strcpy(str,"What?");

    sendto(serv_sd, str, strlen(str), 0, (LPSOCKADDR)&cli, cli_len);
    // 顯示送去client 的字串
    printf("server: server->client: %s\n",str);
  }

  //========================================================
  closesocket(serv_sd);
  
  //結束 WinSock DLL 的使用=================================
  WSACleanup();
  return;
}