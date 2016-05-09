#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024

int main(int argc, char** argv) {
  //�ŧi======================================
  WSADATA  wsadata;
  SOCKET  sd1, sd2;
  struct sockaddr_in serv1, serv2;
  int n, serv_len = sizeof(serv1);
  char  str[MAXLINE]="How are you?";
  
  // �I�s WSAStartup() ���U WinSock DLL ���ϥ�
  int nResult = WSAStartup(0x101, (LPWSADATA)&wsadata);
  if(nResult!=0) {
    printf("WSA Initialization failed: %d\n", nResult);
    WSACleanup();
    return;
  }
  
  // �}�� UDP socket==================================
  LPHOSTENT hp = (sd1 = socket(AF_INET, SOCK_DGRAM, 0));
  if(hp == SOCKET_ERROR) {
    printf("get hp error, code: %d\n", WSAGetLastError());
  }

  // ���s�u�@�ǳơA�]�t��g sockaddr_in ���c (serv) �C
  // ���e���Gserver �� IP ��}�Aport number �����C
  serv1.sin_family       = AF_INET;
  serv1.sin_addr.s_addr  = inet_addr("127.0.0.1");
  serv1.sin_port         = htons(5678);
  
  // �}�� UDP socket==================================
  hp = (sd2 = socket(AF_INET, SOCK_DGRAM, 0));
  if(hp == SOCKET_ERROR) {
    printf("get hp error, code: %d\n", WSAGetLastError());
  }

  // ���s�u�@�ǳơA�]�t��g sockaddr_in ���c (serv) �C
  // ���e���Gserver �� IP ��}�Aport number �����C
  serv2.sin_family       = AF_INET;
  serv2.sin_addr.s_addr  = inet_addr("127.0.0.1");
  serv2.sin_port         = htons(5678);
  
  // �u�@��========================================
  
  // �ǰehow are you��echo server
  sendto(sd1, str, strlen(str)+1, 0, (LPSOCKADDR)&serv1, serv_len);
  printf("client: client->server: %s\n" ,str);
  sendto(sd2, str, strlen(str)+1, 0, (LPSOCKADDR)&serv2, serv_len);
  printf("client: client->server: %s\n" ,str);
  
  // ��echo server����
  while(1){
    n=recvfrom(sd1, str, MAXLINE, 0, (LPSOCKADDR)&serv1, &serv_len);
    str[n]='\0';
    printf("server: %s\n",str);
  }
  // ���� socket
  closesocket(sd1);
  closesocket(sd2);
  // ���� WinSock DLL ���ϥ�
  WSACleanup();  
  return 0;
}