#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024

int main(int argc, char** argv) {
  //宣告
  WSADATA  wsadata;
  SOCKET  sd;
  struct sockaddr_in serv;
  int n;
  char  str[1024]="How are you?";
  
  // 呼叫 WSAStartup() 註冊 WinSock DLL 的使用
  WSAStartup(0x101,(LPWSADATA) &wsadata);
  
  // 開啟一個 TCP socket.
  sd = socket(AF_INET, SOCK_STREAM, 0); 

  // 為連線作準備，包含填寫 sockaddr_in 結構 (serv) 。
  // 內容有：server 的 IP 位址，port number 等等。
  serv.sin_family       = AF_INET;
  serv.sin_addr.s_addr  = inet_addr("127.0.0.1");
  serv.sin_port         = htons(5554);
  
  // 連接至 echo server
  connect(sd, (LPSOCKADDR) &serv, sizeof(serv)); 
  
  //傳送how are you至echo server
  send(sd, str, strlen(str)+1, 0); 
  printf("client: client->server: %s\n" ,str);
  
  //由echo server接收
  n=recv(sd, str, MAXLINE, 0); 
  str[n]='\0';
  printf("client: server->client: %s\n",str);
  
  //關閉TCP socket
  closesocket(sd);
  
  // 結束 WinSock DLL 的使用
  WSACleanup();  
  return 0;
}
