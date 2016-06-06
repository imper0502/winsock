// =============================================================================
// my server
// 作者: 吳昱成
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
  SOCKET                               tcp_sd, udp_sd, client_sd[CONNECTNUMBER];
  struct sockaddr_in                              server, client[CONNECTNUMBER];
  int                                                 client_len[CONNECTNUMBER];
  char                                              str[CONNECTNUMBER][MAXLINE];
  int                                                                   i, j, k;
  int                                                                      m, n;// for recv() & recvfrom()
  // 呼叫 WSAStrartup() 註冊 WinSock DLL 的使用
  int nResult = WSAStartup(0x101, (LPWSADATA)&wsadata);
  if(nResult!=0) {
    printf("WSA Initialization failed: %d\n", nResult);
    WSACleanup();
    return 0;
  }
  // 開啟 TCP socket
  hp = (tcp_sd = socket(AF_INET, SOCK_STREAM, 0));
  if(hp == SOCKET_ERROR) {
	printf("get hp error, code: %d\n", WSAGetLastError());
    fprintf(stderr, "echo@server: Can't open TCP socket.\n");
    return 0;
  }
  // 開啟 UDP socket
  LPHOSTENT hp = (udp_sd = socket(AF_INET, SOCK_DGRAM, 0));
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
  server.sin_port = htons(5554);                                 //"IPPORT_ECHO"
  // 設定client IP & port。
  client_len[0] = sizeof(client[0]);
  client[0].sin_family = AF_INET;
  client[0].sin_addr.s_addr = inet_addr("255.255.255.255");
  client[0].sin_port = htons(5555);

  // 查詢本機(Server)資訊=======================================================
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
    printf("host IP: %s\n", inet_ntoa(*(LPIN_ADDR)(hp->h_addr)));
  }
  
  // 工作區 ====================================================================
  if(bind(tcp_sd, (LPSOCKADDR)&server, sizeof(server)) < 0) {
    fprintf(stderr, "echo@server: Can't bind local address.\n");
    return 0;
  }
  // 呼叫 listen() 使 socket進入[監聽]狀態，並設定
  // 最大同時可接受的連結要求
  if(listen(tcp_sd, CONNECTNUMBER) < 0) {
    fprintf(stderr, "echo@server: listen() error!!!\n");
    return 0;
  }
  for(int i; i<CONNECTNUMBER; i++) {
    client_len[i] = sizeof(client[i]);
  }
 
  // ===========================================================================
  
  
  
  
  
  
  
  // ===========================================================================
  closesocket(udp_sd);
  closesocket(tcp_sd);
  for(int i; i<CONNECTNUMBER; i++) {
    closesocket(client_len[i]);
  }
  WSACleanup();
}//main end=====================================================================