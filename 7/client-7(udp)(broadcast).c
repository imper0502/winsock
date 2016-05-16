#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024

int main(int argc, char** argv) {
  //�ŧi======================================
  WSADATA  wsadata;
  SOCKET  sd;
  struct sockaddr_in serv, cli;
  int n, serv_len = sizeof(serv);
  char  str[MAXLINE]="";
  
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

  cli.sin_family       = AF_INET;
  cli.sin_addr.s_addr  = 0;
  cli.sin_port         = htons(1234);
  
  hp = bind(sd, (LPSOCKADDR) &cli, sizeof(cli));
  if(hp < 0) printf("Bind error!\n");
  
  // �u�@��========================================
  while(1){
    n=recvfrom(sd, str, MAXLINE, 0, (LPSOCKADDR)&serv, &serv_len);
    str[n]='\0';
    printf("Server(from %s): %s\n", inet_ntoa(serv.sin_addr), str);
  }
  // ���� socket
  closesocket(sd);
  
  // ���� WinSock DLL ���ϥ�
  WSACleanup();  
  return 0;
}
