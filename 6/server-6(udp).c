#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024    /* �r��w�İϪ��� */

int main() {
  // �ŧi=============================================
  WSADATA wsadata;
  SOCKET  serv_sd;
  struct  sockaddr_in  serv, cli;
  int     n, cli_len = sizeof(cli);
  char    str[MAXLINE];

  // �I�s WSAStrartup() ���U WinSock DLL ���ϥ�=======
  int nResult = WSAStartup(0x101, (LPWSADATA)&wsadata);
  if(nResult!=0) {
    printf("WSA Initialization failed: %d\n", nResult);
    WSACleanup();
    return;
  }
  
  // �}�� UDP socket==================================
  LPHOSTENT hp = (serv_sd = socket(AF_INET, SOCK_DGRAM, 0));
  if(hp == SOCKET_ERROR) {
    printf("get hp error, code: %d\n", WSAGetLastError());
  }
  
  //���w socket �� IP ��}�M port number==============
  serv.sin_family      = AF_INET;
  serv.sin_addr.s_addr = 0;
  serv.sin_port        = htons(5678); // ���wIPPORT_ECHO �� echo port

  //bind==============================================
  hp = bind(serv_sd, (LPSOCKADDR) &serv, sizeof(serv));
  if(hp < 0) {
    printf("get hp error, code: %d\n", WSAGetLastError());
  }
  
  // �d�ߥ����W��=====================================
  printf("What is local host's name?\n");
  hp = gethostname((LPSTR)str, MAXLINE);
  if(hp != 0) {
    printf("get hp error, code: %d\n\n", WSAGetLastError());
  }else {
    printf("local host name: %s\n", str);
    
  }
  
  // �d�ߥ��� IP��T====================================
  printf("What is %s's IP?\n", str);
  hp = gethostbyname(str);
  if(hp == NULL){
    printf("get hp error, code: %d\n", WSAGetLastError());
  }else {
    printf("host name: %s\n", hp->h_name);
    printf("host nickname: %s\n", hp->h_aliases[0]);
    printf("host IP: %s\n", inet_ntoa(*(LPIN_ADDR)(hp->h_addr)));
  }

  // �u�@��================================================
  while (1) {
    printf("server: waiting for client\n");

    n=recvfrom(serv_sd, str, MAXLINE, 0, (LPSOCKADDR)&cli, &cli_len);
    str[n]='\0';
    printf("server: client->server: %s\n",str);

    if(strcmp(str,"How are you?\0")==0)
    strcpy(str,"Fine, thank you!");
    else strcpy(str,"What?");

    sendto(serv_sd, str, strlen(str), 0, (LPSOCKADDR)&cli, cli_len);
    // ��ܰe�hclient ���r��
    printf("server: server->client: %s\n",str);
  }

  //========================================================
  closesocket(serv_sd);
  
  //���� WinSock DLL ���ϥ�=================================
  WSACleanup();
  return;
}