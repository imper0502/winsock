#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024

int main(int argc, char** argv) {
  //�ŧi
  WSADATA  wsadata;
  SOCKET  sd;
  struct sockaddr_in serv;
  int n;
  char  str[1024]="How are you?";
  
  // �I�s WSAStartup() ���U WinSock DLL ���ϥ�
  WSAStartup(0x101,(LPWSADATA) &wsadata);
  
  // �}�Ҥ@�� TCP socket.
  sd = socket(AF_INET, SOCK_STREAM, 0); 

  // ���s�u�@�ǳơA�]�t��g sockaddr_in ���c (serv) �C
  // ���e���Gserver �� IP ��}�Aport number �����C
  serv.sin_family       = AF_INET;
  serv.sin_addr.s_addr  = inet_addr("127.0.0.1");
  serv.sin_port         = htons(5554);
  
  // �s���� echo server
  connect(sd, (LPSOCKADDR) &serv, sizeof(serv)); 
  
  //�ǰehow are you��echo server
  send(sd, str, strlen(str)+1, 0); 
  printf("client: client->server: %s\n" ,str);
  
  //��echo server����
  n=recv(sd, str, MAXLINE, 0); 
  str[n]='\0';
  printf("client: server->client: %s\n",str);
  
  //����TCP socket
  closesocket(sd);
  
  // ���� WinSock DLL ���ϥ�
  WSACleanup();  
  return 0;
}
