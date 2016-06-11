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
// 主程式=======================================================================
int main() {
  // 宣告區&初始化區============================================================
  WSADATA                                                               wsadata;
  SOCKET                                  tcp_sd, udp_sd, cli_sd[CONNECTNUMBER];
  struct sockaddr_in                              server, client[CONNECTNUMBER];
  int                                                    cli_len[CONNECTNUMBER];
  char                                              str[CONNECTNUMBER][MAXLINE];
  int                                                                   i, j, k;
  int                                                          n[CONNECTNUMBER];// for recv() & recvfrom()
  // 呼叫 WSAStrartup() 註冊 WinSock DLL 的使用
  if(WSAStartup(0x101, (LPWSADATA)&wsadata)!=0) {
    printf("WSA Initialization failed: %d\n", nResult);
    WSACleanup();
    return 0;
  }
  // 開啟 TCP socket
  LPHOSTENT hp = (tcp_sd = socket(AF_INET, SOCK_STREAM, 0));
  if(hp == SOCKET_ERROR) {
    printf("get hp error, code: %d\n", WSAGetLastError());
    fprintf(stderr, "echo@server: Can't open TCP socket.\n");
    return 0;
  }
  // 開啟 UDP socket
  hp = (udp_sd = socket(AF_INET, SOCK_DGRAM, 0));
  if(hp == SOCKET_ERROR) {
    printf("get hp error, code: %d\n", WSAGetLastError());
    fprintf(stderr, "echo@server: Can't open UDP socket.\n");
    return 0;
  }
  // 設定廣播功能
  char broadcast = 'a';
  setsockopt(udp_sd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));

  // 設定位址結構===============================================================
  // 設定server IP & port
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = 0;
  server.sin_port = htons(5554);                                                //"IPPORT_ECHO"
  // 設定client IP & port。
  client[0].sin_family = AF_INET;
  client[0].sin_addr.s_addr = inet_addr("255.255.255.255");
  client[0].sin_port = htons(5555);

  // 工作區 ====================================================================
  if(bind(tcp_sd, (LPSOCKADDR)&server, sizeof(server)) < 0) {
    printf("get hp error, code: %d\n", WSAGetLastError());
    return 0;
  }
  if(bind(udp_sd, (LPSOCKADDR)&server, sizeof(server)) < 0) {
    printf("get hp error, code: %d\n", WSAGetLastError());
    return 0;
  }
  // 呼叫 listen() 使 socket進入[監聽]狀態，並設定
  // 最大同時可接受的連結要求
  if(listen(tcp_sd, CONNECTNUMBER) < 0) {
    fprintf(stderr, "echo@server: listen() error!!!\n");
    return 0;
  }
  // 設定client長度
  for(int i; i<CONNECTNUMBER; i++) {
    cli_len[i] = sizeof(client[i]);
  }
  // ===========================================================================
  
  cli_sd[0] = accept(tcp_sd, (LPSOCKADDR) &client[0], &cli_len[0]);
  // tcp 收
  n[0] = recv(cli_sd, str[0], MAXLINE, 0);
  str[n[0]]='\0';   
  // tcp 送
  send(cli_sd[0], str[0], strlen(str[0]), 0);
  // =========================================================================== 
  // udp 收
  n[0] = recvfrom(udp_sd, str[0], MAXLINE, 0, (LPSOCKADDR)&client[0], &cli_len[0]);
  str[n[0]]='\0';
  // udp 送
  sendto(udp_sd, str[0], strlen(str[0]), 0, (LPSOCKADDR)&client[0], cli_len[0]);
  // ===========================================================================
  closesocket(udp_sd);
  closesocket(tcp_sd);
  for(int i; i<CONNECTNUMBER; i++) {
    closesocket(client_len[i]);
  }
  WSACleanup();
}//main end=====================================================================