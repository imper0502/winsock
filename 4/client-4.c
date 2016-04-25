#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024

int main(int argc, char** argv) {
  WSADATA     wsadata;
  SOCKET         sd;
  struct sockaddr_in serv;
  char     str[1024]="How are you?";

  int n;

  WSAStartup(0x101,(LPWSADATA) &wsadata); // �I�s WSAStartup() ���U WinSock DLL ���ϥ�

  sd = socket(AF_INET, SOCK_STREAM, 0); //�}�Ҥ@�� TCP socket.

  //���s�u�@�ǳơA�]�t��g sockaddr_in ���c (serv) �C
  //���e���Gserver �� IP ��}�Aport number �����C
  serv.sin_family       = AF_INET;
  serv.sin_addr.s_addr  = inet_addr("127.0.0.1");
  serv.sin_port         = htons(IPPORT_ECHO);

  connect(sd, (LPSOCKADDR) &serv, sizeof(serv)); // �s���� echo server

  send(sd, str, strlen(str)+1, 0); //�ǰehow are you��echo server
  printf("client: client->server: %s\n" ,str);

  n=recv(sd, str, MAXLINE, 0); //��echo server����
  str[n]='\0';
  printf("client: server->client: %s\n",str);

  closesocket(sd); //����TCP socket
  WSACleanup();  // ���� WinSock DLL ���ϥ�
  return 0;
}


