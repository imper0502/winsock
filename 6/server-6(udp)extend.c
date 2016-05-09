#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024    /* �r��w�İϪ��� */
#define clientNum 2
int main() {
  // �ŧi=============================================
  WSADATA wsadata;
  SOCKET  serv_sd;
  struct  sockaddr_in  serv, cli[clientNum];
  int     n, cli_len[clientNum], onlineClientNum = 0, i, j, k;
  char    str[MAXLINE], tmp[20];

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
  for(i=0; i<clientNum; i++){
	cli_len[i] = sizeof(cli[i]); 
    printf("server: waiting for client%d...\n",i+1);
    hp = (n=recvfrom(serv_sd, str, MAXLINE, 0, (LPSOCKADDR)&cli[i], &cli_len[i]));
	if(hp==NULL) {
		printf("get hp error, code: %d\n", WSAGetLastError());
		break;
	}
	str[n]='\0';
    printf("client(%s:%d)->server: %s\n", inet_ntoa(cli[i].sin_addr), ntohs(cli[i].sin_port), str);
	onlineClientNum++;
  }
  printf("the number of online people: %d\n", onlineClientNum);
  
  // if(strcmp(str,"How are you?\0")==0)
  //   strcpy(str,"Fine, thank you!");
  // else 
  // strcpy(str,"What?");

  while(1) {
	for(i=0; i<100; i++){
      for(j=0; j<10; j++) {
  	    memset(tmp, j+'0', sizeof(tmp));
	    for(k=0; k<clientNum; k++){
          sendto(serv_sd, tmp, strlen(str), 0, (LPSOCKADDR)&cli[k], cli_len[k]);
          printf("server: send to client%d: %s\n", k+1, tmp);// ��ܰe�hclient ���r��
	    }
  	    sleep(1);
      }	
	}  
  }

  //========================================================
  closesocket(serv_sd);
  
  //���� WinSock DLL ���ϥ�=================================
  WSACleanup();
  return 0;
}
