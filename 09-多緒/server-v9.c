// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Server v9
// 作者: 吳昱成
// 最佳的瀏覽效果，請用Notepad++ 開啟。
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// =============================================================================
#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>
// =============================================================================
#define MAXLINE 1024
// Main program of a thread ====================================================
void *ThreadMain(void *arg);
// Structure of arguments to pass to client thread =============================
struct ThreadArgs
{
  int _clntSock;
};
// =============================================================================
int main(int argc, char *argv[])
{
  // 宣告:執行緒部分
  DWORD                                                                threadID;
  struct ThreadArgs                                                 *threadArgs;
  threadArgs = (struct ThreadArgs *)malloc(sizeof(struct ThreadArgs));

  // 宣告windows socket 部分
  WSADATA                                                               wsadata;
  SOCKET                                                        serv_sd, cli_sd;
  struct sockaddr_in                                          servAddr, cliAddr;
  unsigned int                                                      cliAddr_len;
  int                                                                         n;
  char                                                             str[MAXLINE];

  // 呼叫 WSAStrartup() 註冊 WinSock DLL 的使用=================================
  int nResult = WSAStartup(MAKEWORD(2, 0), (LPWSADATA)&wsadata);
  if(nResult!=0)
  {
    printf("WSA Initialization failed: %d\n", nResult);
    WSACleanup();
    return 0;
  }

  // 開啟 TCP socket============================================================
  LPHOSTENT hp;
  hp = (serv_sd = socket(AF_INET, SOCK_STREAM, 0));
  if(hp == SOCKET_ERROR)
  {
    printf("get hp error, code: %d\n", WSAGetLastError());
  }

  //指定 socket 的 IP 位址和 port number========================================
  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family      = AF_INET;
  servAddr.sin_addr.s_addr = 0;
  servAddr.sin_port        = htons(IPPORT_ECHO);                                // 指定IPPORT_ECHO 為 echo port

  //bind========================================================================
  hp = bind(serv_sd, (LPSOCKADDR) &servAddr, sizeof(servAddr));
  if(hp < 0)
  {
    printf("get hp error, code: %d\n", WSAGetLastError());
  }

  // 呼叫 listen() 使 socket進入[監聽]狀態，並設定
  // 最大同時可接受的連結要求
  // hp = listen(serv_sd, 5);
  // if(hp < 0) {
    // fprintf(stderr, "\necho_srv: listen() error!!!\n");
    // exit(1);
  // }

  for(;;)
  {
    cliAddr_len = sizeof(cliAddr);
    hp = (cli_sd = accept(serv_sd, (LPSOCKADDR) &cliAddr, &cliAddr_len));
    if(hp <0)
    {
      fprintf(stderr, "\necho_srv: listen() error!!!\n");
    }
    threadArgs = (struct ThreadArgs *)malloc(sizeof(struct ThreadArgs));
    threadArgs->_clntSock = cli_sd;
    hp = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ThreadMain, threadArgs, 0, (LPDWORD)&threadID);
    if(hp==0)
    {
      printf("thead create failed.\n");
    }
    printf("with thread %ld\n", threadID);
  }
}
// =============================================================================
void *ThreadMain(void *threadArgs)
{
  int clntSock = ((struct ThreadArgs *)threadArgs)->_clntSock;
  free(threadArgs);
  char buf[MAXLINE];
  int n;
  n = recv(clntSock, buf, MAXLINE, 0);
  if(n<0)
  {
    printf("recv failed.\n");
  }

  while(n>0)
  {
    // TO DO ===================================================================

    // =========================================================================
    if(send(clntSock, buf, strlen(buf), 0) != strlen(buf))
    {
      printf("send faild.\n");
    }
    n = recv(clntSock, buf, MAXLINE, 0);
    if(n<0)
    {
      printf("recv failed.\n");
    }
  }
  closesocket(clntSock);
  return (NULL);
}