// =============================================================================
// my server
// 作者: 吳昱成
// 最佳的瀏覽效果，請用Notepad++ 開啟。
// =============================================================================

// 載入函式庫===================================================================
#include <stdio.h>
#include <string.h>
#include <winsock.h>
// 定義巨集=====================================================================
#define CONNECTNUMBER                                                        128
#define MAXLINE                                                             1024//字串緩衝區長度
// Main program of a thread=====================================================
void *ThreadMain(void *arg);
// Structure of arguments to pass to client thread==============================
struct ThreadArgs
{
  SOCKET                                                                _cli_sd;
};
// 定義字串=====================================================================
char msg_1[MAXLINE] = "投票程式伺服端。作者：吳昱成\n";
char msg_2[MAXLINE] = "請輸入總投票人數: ";
char msg_3[MAXLINE] = "請輸入議題: ";
char msg_4[MAXLINE] = "請輸入選項A: ";
char msg_5[MAXLINE] = "請輸入選項B: ";
char msg_6[MAXLINE] = "請輸入選項C: ";
char msg_7[MAXLINE] = "請輸入選項D: ";
char msg_8[MAXLINE] = "開始投票。請等待投票完成...\n";
char msg_9[MAXLINE] = "感謝你的投票。請等待投票結果。\n";
char msg_10[MAXLINE] = "所有人都完成投票了。結果如下:\n";
char msg_11[MAXLINE] = "票\n";
char msg_bt[MAXLINE] = "";
// 全域變數
int                                                             peopleN, Num[5];
// 主程式=======================================================================
int main() {
  // 宣告區&初始化區============================================================
  // 宣告:執行緒部分
  DWORD                                                                threadID;
  struct ThreadArgs                                                 *threadArgs;
  // WSA
  WSADATA                                                               wsadata;
  SOCKET                                                 tcp_sd, udp_sd, cli_sd;// cli_sd is for TCP.
  struct sockaddr_in                                     serv, cli_tcp, cli_udp;// cli_tcp is a buffer.
  int                                                      n, serv_len, cli_len;
  // otherwise
  char                                                          str[5][MAXLINE];
  char                                                               voting, ch;

  // 呼叫 WSAStrartup() 註冊 WinSock DLL 的使用
  int nResult = WSAStartup(0x101, (LPWSADATA)&wsadata);
  if(nResult!=0){
    printf("WSA Initialization failed: %d\n", nResult);
    WSACleanup();
    return 0;
  }
  // 開啟 TCP socket
  LPHOSTENT hp = (tcp_sd = socket(AF_INET, SOCK_STREAM, 0));
  if(hp == SOCKET_ERROR){
    printf("get hp error, code: %d\n", WSAGetLastError());
    fprintf(stderr, "Can't open TCP socket.\n");
    return 0;
  }
  // 開啟 UDP socket
  hp = (udp_sd = socket(AF_INET, SOCK_DGRAM, 0));
  if(hp == SOCKET_ERROR) {
    printf("get hp error, code: %d\n", WSAGetLastError());
    fprintf(stderr, "Can't open UDP socket.\n");
    return 0;
  }
  // 設定廣播功能
  char broadcast = 'a';
  setsockopt(udp_sd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));

  // 設定位址結構===============================================================
  // 設定server IP & port to sockaddr_in server for TCP Socket
  serv.sin_family = AF_INET;
  serv.sin_addr.s_addr = 0;                                                     // 本機
  serv.sin_port = htons(5554);                                                  // "IPPORT_ECHO"
  // cli_tcp 當buffer，所以不需要設定
  // 設定client IP & port to sockaddr_in client for UDP Socket
  cli_udp.sin_family = AF_INET;
  cli_udp.sin_addr.s_addr = inet_addr("255.255.255.255");                       // 廣播
  cli_udp.sin_port = htons(5555);                                               // 用另一個port

  // 工作區 ====================================================================
  // 顯示歡迎訊息
  printf("%s", msg_1);
  // 要求輸入
  printf("%s", msg_2);
  fscanf(stdin, "%d", &peopleN);

  printf("%s", msg_3);
  fgets(str[0], MAXLINE, stdin);                                                // 因為會讀到上一個Enter
  fgets(str[0], MAXLINE, stdin);                                                // 所以在重複一次讀取

  printf("%s", msg_4);
  fgets(str[1], MAXLINE, stdin);
  printf("%s", msg_5);
  fgets(str[2], MAXLINE, stdin);
  printf("%s", msg_6);
  fgets(str[3], MAXLINE, stdin);
  printf("%s", msg_7);
  fgets(str[4], MAXLINE, stdin);
  printf("%s", msg_8);

  // 字串處理
  strcpy(msg_3, "議題: ");
  strcat(msg_3, str[0]);
  strcpy(msg_4, "(A) ");
  strcat(msg_4, str[1]);
  strcpy(msg_5, "(B) ");
  strcat(msg_5, str[2]);
  strcpy(msg_6, "(C) ");
  strcat(msg_6, str[3]);
  strcpy(msg_7, "(D) ");
  strcat(msg_7, str[4]);

  strcpy(msg_bt, "網路投票開始。\n");
  strcat(msg_bt, msg_3);
  strcat(msg_bt, msg_4);
  strcat(msg_bt, msg_5);
  strcat(msg_bt, msg_6);
  strcat(msg_bt, msg_7);

  // 開始投票===================================================================
  voting = 1;
  memset(Num, 0, 5);
  while(voting==1){
    // udp 廣播
    sendto(udp_sd, msg_bt, strlen(msg_bt), 0, (LPSOCKADDR)&cli_udp, sizeof(cli_udp));
    // TCP part ==============================================================
    // 連結 tcp_sd 到本機
    serv_len = sizeof(serv);
    hp = bind(tcp_sd, (LPSOCKADDR)&serv, serv_len);
    if(hp < 0) {
      printf("get hp error, code: %d\n", WSAGetLastError());
      return 0;
    }
    // 呼叫 listen() 使 socket進入[監聽]狀態，並設定
    // 最大同時可接受的連結要求
    if(listen(tcp_sd, peopleN) < 0) {
      fprintf(stderr, "server: listen() error!!!\n");
      return 0;
    }
    
    // 設定tcp client長度
    cli_len = sizeof(cli_tcp);
    // 把listen 從 serv_sd 到的 tcp client accept 到 cli_sd >>> 建立通道
    cli_sd = accept(tcp_sd, (LPSOCKADDR)&cli_tcp, &cli_len);
    if(cli_sd < 0) {
      printf("get hp error, code: %d\n", WSAGetLastError());
      return 0;
    }
    // non-block mode
    u_long iMode = 1;
    ioctlsocket(tcp_sd, FIONBIO, &iMode);
    
    threadArgs = (struct ThreadArgs *)malloc(sizeof(struct ThreadArgs));
    threadArgs->_cli_sd = cli_sd;
    hp = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ThreadMain, threadArgs, 0, (LPDWORD)&threadID);
    if(hp==0){
      printf("thead create failed.\n");
    }
    Num[0] = Num[1]+Num[2]+Num[3]+Num[4];
    if(peopleN == Num[0]){
      voting = 0;
      break;
    }
  }
  if(voting==0){
    sprintf(str[1], "%d", Num[1]);
    sprintf(str[2], "%d", Num[2]);
    sprintf(str[3], "%d", Num[3]);
    sprintf(str[4], "%d", Num[4]);

    strcat(msg_10, "選項A ");
    strcat(msg_10, str[1]);
    strcat(msg_10, msg_11);
    strcat(msg_10, "選項B ");
    strcat(msg_10, str[2]);
    strcat(msg_10, msg_11);
    strcat(msg_10, "選項C ");
    strcat(msg_10, str[3]);
    strcat(msg_10, msg_11);
    strcat(msg_10, "選項D ");
    strcat(msg_10, str[4]);
    strcat(msg_10, msg_11);
    strcat(msg_10, '\0');
  }
  int i=0;
  while(voting==0){
    sleep(1);    
    sendto(udp_sd, msg_10, strlen(msg_10), 0, (LPSOCKADDR)&cli_udp, sizeof(cli_udp));
    if((i++)==10) break;
  }
  system("pause");

  // ===========================================================================
  closesocket(udp_sd);
  closesocket(tcp_sd);
  closesocket(cli_sd);

  WSACleanup();
}//main end=====================================================================

// ThreadMain===================================================================
void *ThreadMain(void *threadArgs){
  //
  SOCKET                                                                 cli_sd;
  char                                                         ch, buf[MAXLINE];
  int                                                                         n;
  //
  cli_sd = ((struct ThreadArgs *)threadArgs)->_cli_sd;
  free(threadArgs);
  
  // tcp 收
  n = recv(cli_sd, &ch, 1, 0);
  if(n > 0){
    printf("%d\t%c", cli_sd, ch);
    switch(ch){
      case 'a':
        Num[1]++;
	      break;
      case 'b':
        Num[2]++;
	        break;
      case 'c':
        Num[3]++;
	      break;
      case 'd':
        Num[4]++;
	      break;
    }
    printf("\t%d\t%d\t%d\t%d\n", Num[1], Num[2], Num[3], Num[4]);
  }
  // tcp 送
  send(cli_sd, msg_9, strlen(msg_9), 0);

  closesocket(cli_sd);
  return(NULL);
}
