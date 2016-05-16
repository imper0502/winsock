#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024

int main(int argc, char** argv) {
  //宣告======================================
  WSADATA  wsadata;
  SOCKET  sd;
  struct sockaddr_in serv, cli;
  int n, serv_len = sizeof(serv);
  char  str[MAXLINE]="";
  
  // 呼叫 WSAStartup() 註冊 WinSock DLL 的使用
  int nResult = WSAStartup(0x101, (LPWSADATA)&wsadata);
  if(nResult!=0) {
    printf("WSA Initialization failed: %d\n", nResult);
    WSACleanup();
    return;
  }
  
  // 開啟 UDP socket==================================
  LPHOSTENT hp = (sd = socket(AF_INET, SOCK_DGRAM, 0));
  if(hp == SOCKET_ERROR) {
    printf("get hp error, code: %d\n", WSAGetLastError());
  }

  // 為連線作準備，包含填寫 sockaddr_in 結構 (serv) 。
  // 內容有：server 的 IP 位址，port number 等等。
  //serv.sin_family       = AF_INET;
  //serv.sin_addr.s_addr  = inet_addr("10.3.200.77");
  //serv.sin_port         = htons(5679);
  
  cli.sin_family       = AF_INET;
  cli.sin_addr.s_addr  = inet_addr("10.3.200.77");
  cli.sin_port         = htons(5679);
  
  hp = bind(sd, (LPSOCKADDR) &cli, sizeof(cli));
  if(hp < 0) printf("Bind error!\n");
  
  // 工作區========================================
  while(1){
    n=recvfrom(sd, str, MAXLINE, 0, (LPSOCKADDR)&serv, &serv_len);
    str[n]='\0';
    printf("Server(from %s): %s\n", inet_ntoa(serv.sin_addr), str);
  }
  // 關閉 socket
  closesocket(sd);
  
  // 結束 WinSock DLL 的使用
  WSACleanup();  
  return 0;
}
