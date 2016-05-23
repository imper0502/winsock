#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024

int main(int argc, char** argv) {
  //�ŧi�P��l��=====================================
  WSADATA  wsadata;
  SOCKET  sd;  
  struct sockaddr_in serv, cli; 
  int n, serv_len = sizeof(serv), cli_len = sizeof(cli), i;
  char  str[MAXLINE]="";

  // �I�s WSAStartup() ���U WinSock DLL ���ϥ�
  int nResult = WSAStartup(0x101, (LPWSADATA)&wsadata);
  if(nResult!=0) {
    printf("WSA Initialization failed: %d\n", nResult);
    WSACleanup();
    return 0;
  }

  // �}�� UDP socket
  LPHOSTENT hp = (sd = socket(AF_INET, SOCK_DGRAM, 0));
  if(hp == SOCKET_ERROR) {
    printf("get hp error, code: %d\n", WSAGetLastError());
  }

  // �]�w����
  cli.sin_family       = AF_INET;
  cli.sin_addr.s_addr  = 0;
  cli.sin_port         = htons(55549);
  
  // unsigned short port = 0;
  // printf("YOU CAN SET YOUR CHANNEL. (1~9, default: select a port.) >>");
  // fgets(str, MAXLINE, stdin);
  // str[strlen(str)-1]='\0'; 
  // if(strlen(str) == 0) {
  //   printf("Use default.\nYOU CAN CHANGE PORT. >>");
  //   fgets(str, MAXLINE, stdin);
  //   str[strlen(str)-1]='\0'; 
  //   if(strlen(str) == 0) {
  //     printf("Use default: 5554");
  //   }else {
  //     port = (u_short)atoi(str); 
  //     cli.sin_port = htons(port);
  //   }
  // }else {
  //   port = 0;
  //   for(i=0; i<4; i++) {
  //     port = (u_short)atoi(str)+10*port;    
  //   }
  //   cli.sin_port = htons(port);
  // }
  
  // ��J�s����}
  struct ip_mreq multicastRequest;
  multicastRequest.imr_multiaddr.s_addr = inet_addr("224.1.1.10");
  multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY);
  
  // �[�J�s��
  hp = setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *)&multicastRequest, sizeof(multicastRequest));
  if(hp < 0) {
	  printf("setsockopt() failed.\n");
  }
  
  // Bind
  hp = bind(sd, (LPSOCKADDR) &cli, cli_len);
  if(hp < 0) {
	  printf("Bind error!\n");
  }
  
  // �u�@��========================================
  while(1){
    n = recvfrom(sd, str, MAXLINE, 0, (LPSOCKADDR)&serv, &serv_len);
    str[n]='\0';
    printf("Server(from %s:%i): %s\n", inet_ntoa(serv.sin_addr), ntohs(serv.sin_port), str);
  }
  // ���� socket
  closesocket(sd);

  // ���� WinSock DLL ���ϥ�
  WSACleanup();
  return 0;
}
