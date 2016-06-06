// Server v9
#include <stdio.h>      /* for printf(), fprintf() */
#include <stdlib.h>     /* for exit() */
#include <winsock.h>    /* for socket(),... */

#define MAXLINE 1024

void *ThreadMain(void *arg);             /* Main program of a thread */

/* Structure of arguments to pass to client thread */
struct ThreadArgs {
    int _clntSock;
};

int main(int argc, char *argv[]) {
  // 宣告:執行緒部分
  DWORD  threadID;                 /* Thread ID from CreateThread() */
  struct ThreadArgs *threadArgs;   /* Pointer to argument structure for thread */
  threadArgs = (struct ThreadArgs *)malloc(sizeof(struct ThreadArgs));
  
  // 宣告windows socket 部分
  WSADATA wsadata;
  SOCKET  serv_sd, cli_sd;
  struct  sockaddr_in  servAddr, cliAddr;
  // int servSock, clntSock;
  unsigned int cliAddr_len;
  int n;
  char    str[MAXLINE];

  // 呼叫 WSAStrartup() 註冊 WinSock DLL 的使用========
  int nResult = WSAStartup(MAKEWORD(2, 0), (LPWSADATA)&wsadata);
  if(nResult!=0) {
    printf("WSA Initialization failed: %d\n", nResult);
    WSACleanup();
    return 0;
  }
  
  // 開啟 TCP socket===================================
  LPHOSTENT hp;
  hp = (serv_sd = socket(AF_INET, SOCK_STREAM, 0));
  if(hp == SOCKET_ERROR) {
    printf("get hp error, code: %d\n", WSAGetLastError());
  }
  
  //指定 socket 的 IP 位址和 port number===============
  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family      = AF_INET;
  servAddr.sin_addr.s_addr = 0;
  servAddr.sin_port        = htons(IPPORT_ECHO); // 指定IPPORT_ECHO 為 echo port

  //bind===============================================
  hp = bind(serv_sd, (LPSOCKADDR) &servAddr, sizeof(servAddr));
  if(hp < 0) {
    printf("get hp error, code: %d\n", WSAGetLastError());
  }
  
  // 呼叫 listen() 使 socket進入[監聽]狀態，並設定
  // 最大同時可接受的連結要求===========================
  // hp = listen(serv_sd, 5);
  // if(hp < 0) {
    // fprintf(stderr, "\necho_srv: listen() error!!!\n");
    // exit(1);
  // }
  
  // 查詢本機名稱======================================
  // printf("What is local host's name?\n");
  // hp = gethostname((LPSTR)str, MAXLINE);
  // if(hp != 0) {
    // printf("get hp error, code: %d\n\n", WSAGetLastError());
  // }else {
    // printf("local host name: %s\n", str);
  // }
  
  // 查詢本機 IP資訊===================================
  // printf("What is %s's IP?\n", str);
  // hp = gethostbyname(str);
  // if(hp == NULL){
    // printf("get hp error, code: %d\n", WSAGetLastError());
  // }else {
    // printf("host name: %s\n", hp->h_name);
    // printf("host nickname: %s\n", hp->h_aliases[0]);
    // printf("host IP: %s\n", inet_ntoa(*(LPIN_ADDR)(hp->h_addr)));
  // }
  
  for(;;) {
	cliAddr_len = sizeof(cliAddr);
	hp = (cli_sd = accept(serv_sd, (LPSOCKADDR) &cliAddr, &cliAddr_len));
	if(hp <0) {
		fprintf(stderr, "\necho_srv: listen() error!!!\n");
	}
	threadArgs = (struct ThreadArgs *)malloc(sizeof(struct ThreadArgs));
	threadArgs->_clntSock = cli_sd;
    hp = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ThreadMain, threadArgs, 0, (LPDWORD)&threadID);
	if(hp==0) {
		printf("thead create failed.\n");
	}
	printf("with thread %ld\n", threadID);
  }
}

void *ThreadMain(void *threadArgs) {
	int clntSock = ((struct ThreadArgs *)threadArgs)->_clntSock;
    free(threadArgs);
	char buf[MAXLINE];
	int n;
	
    n = recv(clntSock, buf, MAXLINE, 0);
	if(n<0) printf("recv failed.\n");
	
	while(n>0) {
		//>>>>>>>>>>>>>>>>TO DO >>>
		
		//<<<<<<<<<<<<<<<<DO END <<
		if(send(clntSock, buf, strlen(buf), 0) != strlen(buf)) printf("send faild.\n");
		n = recv(clntSock, buf, MAXLINE, 0);
	    if(n<0) printf("recv failed.\n");
	}
    closesocket(clntSock);
    return (NULL);
}
