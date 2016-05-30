#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024    /* �r��w�İϪ��� */

int main() {
  // �ŧi==============================================
  WSADATA wsadata;
  SOCKET  serv_sd, cli_sd;
  struct  sockaddr_in  serv, cli;
  int     n, cli_len = sizeof(cli);
  char    str[MAXLINE];

  // �I�s WSAStrartup() ���U WinSock DLL ���ϥ�========
  int nResult = WSAStartup(0x101, (LPWSADATA)&wsadata);
  if(nResult!=0) {
    printf("WSA Initialization failed: %d\n", nResult);
    WSACleanup();
    return;
  }
  
  // �}�� TCP socket===================================
  LPHOSTENT hp = (serv_sd = socket(AF_INET, SOCK_STREAM, 0));
  if(hp == SOCKET_ERROR) {
    printf("get hp error, code: %d\n", WSAGetLastError());
  }
  
  //���w socket �� IP ��}�M port number===============
  serv.sin_family      = AF_INET;
  serv.sin_addr.s_addr = 0;
  serv.sin_port        = htons(IPPORT_ECHO); // ���wIPPORT_ECHO �� echo port

  //bind===============================================
  hp = bind(serv_sd, (LPSOCKADDR) &serv, sizeof(serv));
  if(hp < 0) {
    printf("get hp error, code: %d\n", WSAGetLastError());
  }
  
  // �d�ߥ����W��======================================
  printf("What is local host's name?\n");
  hp = gethostname((LPSTR)str, MAXLINE);
  if(hp != 0) {
    printf("get hp error, code: %d\n\n", WSAGetLastError());
  }else {
    printf("local host name: %s\n", str);
  }
  
  // �d�ߥ��� IP��T===================================
  printf("What is %s's IP?\n", str);
  hp = gethostbyname(str);
  if(hp == NULL){
    printf("get hp error, code: %d\n", WSAGetLastError());
  }else {
    printf("host name: %s\n", hp->h_name);
    printf("host nickname: %s\n", hp->h_aliases[0]);
    printf("host IP: %s\n", inet_ntoa(*(LPIN_ADDR)(hp->h_addr)));
  }
  
  // �I�s listen() �� socket�i�J[��ť]���A�A�ó]�w
  // �̤j�P�ɥi�������s���n�D===========================
  if( listen(serv_sd, 5) < 0) {
    fprintf(stderr, "\necho_srv: listen() error!!!\n");
    exit(1);
  }

  // �u�@��=============================================
  while (1) {
	printf("\nserver: waiting for client...\n");
	
	cli_sd = accept(serv_sd, (LPSOCKADDR) &cli, &cli_len);
	
	getpeername(cli_sd, (LPSOCKADDR)&cli, &cli_len);
	printf("client's IP: %s\n", inet_ntoa(cli.sin_addr));
	printf("client's port: %d\n\n", ntohs(cli.sin_port));
	
	n = recv(cli_sd, str, MAXLINE, 0);
	str[n]='\0';
		// ��ܱq client �ǨӪ��r��
	printf("server: client->server: %s\n",str);     
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>TO DO>>>
	if(strcmp(str,"How are you?\0")==0)
		strcpy(str,"Fine, thank you!");
	else strcpy(str,"What?");
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<TO DO<<<
	send(cli_sd, str, strlen(str), 0);
		// ��ܰe�hclient ���r��
	printf("server: server->client: %s\n",str);   
  }

  //���� WinSock DLL ���ϥ�
  closesocket(serv_sd);
  closesocket(cli_sd);
  WSACleanup();
  return;
}