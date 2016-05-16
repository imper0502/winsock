#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024    /* �r��w�İϪ��� */
#define clientNum 1
int main() {
  // �ŧi=============================================
  WSADATA wsadata;
  SOCKET  serv_sd;
  struct  sockaddr_in  serv, cli[clientNum];
  int     n, cli_len[clientNum], i, j, k;
  char    str[MAXLINE], tmp[15];

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
  char broadcast = 'a';
  setsockopt(serv_sd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
  
  //���w socket �� IP ��}�M port number==============
  serv.sin_family      = AF_INET;
  serv.sin_addr.s_addr = 0;
  serv.sin_port        = htons(5679); // ���wIPPORT_ECHO �� echo port
  
  cli_len[i] = sizeof(cli[i]); 
  printf("Server will broadcast.\n");
  cli[0].sin_family = AF_INET;
  cli[0].sin_addr.s_addr = inet_addr("10.3.200.127");
  cli[0].sin_port = htons(5679);

  //bind==============================================
  hp = bind(serv_sd, (LPSOCKADDR) &serv, sizeof(serv));
  if(hp < 0) {
	printf("Bind error!\n");
    printf("get hp error, code: %d\n", WSAGetLastError());
  }
  
  // �d�ߥ�����T=====================================
  printf("What is local host's name & IP?\n");
  hp = gethostname((LPSTR)str, MAXLINE);
  if(hp != 0) {
    printf("get hp error, code: %d\n\n", WSAGetLastError());
  }else {
    printf("local host name: %s\n", str);
  }
  hp = gethostbyname(str);
  if(hp == NULL){
    printf("get hp error, code: %d\n", WSAGetLastError());
  }else {
    // printf("host name: %s\n", hp->h_name);
    // printf("host nickname: %s\n", hp->h_aliases[0]);
    printf("host IP: %s\n", inet_ntoa(*(LPIN_ADDR)(hp->h_addr)));
  }

  // �u�@��================================================
  while(1) {
  	memset(tmp, i%10+'0', sizeof(tmp));
    sendto(serv_sd, tmp, strlen(tmp), 0, (LPSOCKADDR)&cli[0], cli_len[0]);
    printf("Server broadcast: %s\n", tmp);// ��ܰe�hclient ���r��
  	sleep(1);
	i++;
  }

  //========================================================
  closesocket(serv_sd);
  
  //���� WinSock DLL ���ϥ�=================================
  WSACleanup();
  return;
}
