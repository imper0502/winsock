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
// 定義字串=====================================================================
char message_1[MAXLINE] = "";
char message_2[MAXLINE] = "";
char message_3[MAXLINE] = "";
char message_4[MAXLINE] = "";
char message_5[MAXLINE] = "";
char message_6[MAXLINE] = "";
char message_7[MAXLINE] = "";
char message_8[MAXLINE] = "";
// 主程式=======================================================================
int main() {
  // 宣告區&初始化區============================================================
  WSADATA                                                               wsadata;
  SOCKET                                                 tcp_sd, udp_sd, cli_sd;// cli_sd is for TCP.
  struct sockaddr_in                                     serv, cli_tcp, cli_udp;// cli_tcp is a buffer.
  int                                                      n, serv_len, cli_len;
  char                                                             str[MAXLINE];
  
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
  // 連結 tcp_sd 到本機
  serv_len = sizeof(serv);
  hp = bind(tcp_sd, (LPSOCKADDR)&serv, serv_len);
  if(hp < 0) {
    printf("get hp error, code: %d\n", WSAGetLastError());
    return 0;
  }
  while(1){ 
    // TCP part ================================================================
    // 呼叫 listen() 使 socket進入[監聽]狀態，並設定
    // 最大同時可接受的連結要求
    if(listen(tcp_sd, CONNECTNUMBER) < 0) {
      fprintf(stderr, "server: listen() error!!!\n");
      return 0;
    }
    while(1){
      // 設定tcp client長度
      cli_len = sizeof(cli_tcp);
      // 把listen 從 serv_sd 到的 tcp client accept 到 cli_sd >>> 建立通道
      cli_sd = accept(tcp_sd, (LPSOCKADDR)&cli_tcp, &cli_len);
      // tcp 收
      n = recv(cli_sd, str, MAXLINE, 0);
      str[n]='\0';   
      // tcp 送
      send(cli_sd, str, strlen(str), 0);
      // UDP part ==============================================================
      // udp 送
      sendto(udp_sd, str, strlen(str), 0, (LPSOCKADDR)&cli_udp, sizeof(cli_udp));
    }
  }
  // ===========================================================================
  closesocket(udp_sd);
  closesocket(tcp_sd);
  closesocket(cli_sd);

  WSACleanup();
}//main end=====================================================================