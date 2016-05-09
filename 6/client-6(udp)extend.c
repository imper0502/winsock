#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024

int main(int argc, char** argv) {
  //�ŧi======================================
  WSADATA  wsadata;
  SOCKET  sd;
  struct sockaddr_in serv;
  int n, serv_len = sizeof(serv), i;
  char  str[MAXLINE]="How are you?", tmp[MAXLINE];

  // �I�s WSAStartup() ���U WinSock DLL ���ϥ�
  int nResult = WSAStartup(0x101, (LPWSADATA)&wsadata);
  if(nResult!=0) {
    printf("WSA Initialization failed: %d\n", nResult);
    WSACleanup();
    return;
  }

  // �}�� UDP socket==================================
  LPHOSTENT hp = (sd = socket(AF_INET, SOCK_DGRAM, 0));
  if(hp == SOCKET_ERROR) {
    printf("get hp error, code: %d\n", WSAGetLastError());
  }

  // ���s�u�@�ǳơA�]�t��g sockaddr_in ���c (serv) �C
  // ���e���Gserver �� IP ��}�Aport number �����C
  serv.sin_family       = AF_INET;
  serv.sin_addr.s_addr  = inet_addr("127.0.0.1");
  serv.sin_port         = htons(5678);

  // �u�@��========================================

  // �ǰehow are you��echo server ���U
  for(i=0; i<2; i++) {
    
    sendto(sd, str, strlen(str)+1, 0, (LPSOCKADDR)&serv, serv_len);
    printf("client: client->server: %s\n" ,str);
    
    n=recvfrom(sd, tmp, MAXLINE, 0, (LPSOCKADDR)&serv, &serv_len);
    str[n]='\0';
    
    if(strcmp(tmp,"Are you the last?(y/n)")==0) {
      printf("server: %s", tmp);
      fgets(tmp, MAXLINE, stdin);
      sendto(sd, tmp, strlen(tmp)+1, 0, (LPSOCKADDR)&serv, serv_len);
    }
  }

  
  // ��echo server����
  while(1){
    n=recvfrom(sd, str, MAXLINE, 0, (LPSOCKADDR)&serv, &serv_len);
    str[n]='\0';
    printf("server: %s\n",str);
    if(strcmp(str,"Are you the last?(y/n)\n")==0) {
      fgets(str, MAXLINE, stdin);
      sendto(sd, str, strlen(str)+1, 0, (LPSOCKADDR)&serv, serv_len);
    }
  }
  // ���� socket
  closesocket(sd);

  // ���� WinSock DLL ���ϥ�
  WSACleanup();
  return 0;
}