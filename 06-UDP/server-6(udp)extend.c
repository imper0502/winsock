#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024    /* 字串緩衝區長度 */
#define clientNum 5  // for extend A
int main() {
  // 宣告=============================================
  WSADATA wsadata;
  SOCKET  serv_sd;
  struct  sockaddr_in  serv, cli[clientNum];
  int     n, cli_len[clientNum], onlineClientNum = 0, i, j, k;
  char    str[MAXLINE], tmp[30] = "Are you the last?(y/n)";

  // 呼叫 WSAStrartup() 註冊 WinSock DLL 的使用=======
  int nResult = WSAStartup(0x101, (LPWSADATA)&wsadata);
  if(nResult!=0) {
    printf("WSA Initialization failed: %d\n", nResult);
    WSACleanup();
    return;
  }

  // 開啟 UDP socket==================================
  LPHOSTENT hp = (serv_sd = socket(AF_INET, SOCK_DGRAM, 0));
  if(hp == SOCKET_ERROR) {
    printf("get hp error, code: %d\n", WSAGetLastError());
  }

  //指定 socket 的 IP 位址和 port number==============
  serv.sin_family      = AF_INET;
  serv.sin_addr.s_addr = 0;
  serv.sin_port        = htons(5678); // 指定IPPORT_ECHO 為 echo port

  //bind==============================================
  hp = bind(serv_sd, (LPSOCKADDR) &serv, sizeof(serv));
  if(hp < 0) {
    printf("get hp error, code: %d\n", WSAGetLastError());
  }

  // 查詢本機資訊=====================================
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
///////////////////////////////////////////////////////////////////////////// for extend B
  // 工作區================================================
  for(i=0; i<clientNum; i++){
    // 等待註冊
    cli_len[i] = sizeof(cli[i]);
    printf("server: waiting for client%d...\n",i+1);
    n=recvfrom(serv_sd, str, MAXLINE, 0, (LPSOCKADDR)&cli[i], &cli_len[i]);
    str[n]='\0';
    printf("client(%s:%d)->server: %s\n", inet_ntoa(cli[i].sin_addr), ntohs(cli[i].sin_port), str);
    // 檢查client是否重複
    for(j=0; j<i; j++) {
      // if(inet_ntoa(cli[i].sin_addr)==inet_ntoa(cli[j].sin_addr))
      if(ntohs(cli[i].sin_port)==ntohs(cli[j].sin_port)){
        i=j;
        break;
      }
    }
    // 詢問是不是最後一個人
    if(i>0) {
      onlineClientNum=i+1;
      sendto(serv_sd, tmp, strlen(tmp), 0, (LPSOCKADDR)&cli[i], cli_len[i]);
      printf("server ask client%d: %s\n", i+1, tmp);
      memset(str, '\0', sizeof(str));
      hp = (n=recvfrom(serv_sd, str, MAXLINE, 0, (LPSOCKADDR)&cli[i], &cli_len[i]));
      if(hp!=NULL){
        str[n] = '\0';
        printf("client%d recive: %s\n", i+1, str);
        if(strcmp(str,"y\n\0")==0) break;
      }
    }else {
      onlineClientNum = 1;
    }
  }
  printf("the number of online people: %d\n", onlineClientNum);
//////////////////////////////////////////////////////////////////////////////////////////

  while(1) {
    for(i=0; i<100; i++){
        for(j=0; j<10; j++) {
          memset(tmp, j+'0', sizeof(tmp));
          for(k=0; k<onlineClientNum; k++){
            sendto(serv_sd, tmp, strlen(tmp), 0, (LPSOCKADDR)&cli[k], cli_len[k]);
            printf("server: send to client%d: %s\n", k+1, tmp);// 顯示送去client 的字串
          }
          sleep(1);
        }
    }
  }

  //========================================================
  closesocket(serv_sd);

  //結束 WinSock DLL 的使用=================================
  WSACleanup();
  return 0;
}
