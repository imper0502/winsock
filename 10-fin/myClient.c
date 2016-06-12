// =============================================================================
// my client
// 作者: 吳昱成
// 最佳的瀏覽效果，請用Notepad++ 開啟。
// =============================================================================

// 載入函式庫===================================================================
#include <stdio.h>
#include <string.h>
#include <winsock.h>
// 定義巨集=====================================================================
#define MAXLINE                                                             1024//字串緩衝區長度
// 主程式=======================================================================
int main() {
  // 宣告區&初始化區============================================================
  WSADATA                                                               wsadata;
  SOCKET                                                         tcp_sd, udp_sd;
  struct sockaddr_in                                         serv, cli, cli_buf;
  int                                                      n, serv_len, cli_len;
  char                                              str[MAXLINE]="How are you?";
  
   
  // 呼叫 WSAStartup() 註冊 WinSock DLL 的使用
  int nResult = WSAStartup(0x101, (LPWSADATA)&wsadata);
  if(nResult!=0) {
    printf("WSA Initialization failed: %d\n", nResult);
    WSACleanup();
    return 0;
  }
  // 開啟 TCP socket.
  LPHOSTENT hp = (tcp_sd = socket(AF_INET, SOCK_STREAM, 0)); 
  if(hp == SOCKET_ERROR) {
    printf("get hp error, code: %d\n", WSAGetLastError());
  }
  // 開啟 UDP socket
  hp = (udp_sd = socket(AF_INET, SOCK_DGRAM, 0));
  if(hp == SOCKET_ERROR) {
    printf("get hp error, code: %d\n", WSAGetLastError());
  }
  // 為連線作準備，包含填寫 sockaddr_in 結構 (serv) 。
  // 內容有：server 的 IP 位址，port number 等等。
  // TCP server information (server)
  serv.sin_family       = AF_INET;
  serv.sin_addr.s_addr  = inet_addr("127.0.0.1");
  serv.sin_port         = htons(5554);  
  // UDP client information (localhost)
  cli.sin_family       = AF_INET;
  cli.sin_addr.s_addr  = 0;
  cli.sin_port         = htons(5555);
  
  // 連接tcp 至 server
  serv_len = sizeof(serv);
  hp = connect(tcp_sd, (LPSOCKADDR)&serv, serv_len); 
  if(hp < 0) {
    printf("get hp error, code: %d\n", WSAGetLastError());
  }
  // 連結 udp_sd 到本機
  cli_len = sizeof(cli);
  hp = bind(udp_sd, (LPSOCKADDR)&cli, cli_len);
  if(hp < 0) {
    printf("get hp error, code: %d\n", WSAGetLastError());
    printf("Bind error!\n");
  }
  // 傳輸開始===================================================================
  // TCP part ==================================================================
  //傳送how are you至echo server
  send(tcp_sd, str, strlen(str)+1, 0); 
  printf("client->server: %s\n" ,str);
  //由echo server接收
  n=recv(tcp_sd, str, MAXLINE, 0); 
  str[n]='\0';
  printf("server->client: %s\n",str);
  
  // UDP part ==================================================================
  n=recvfrom(udp_sd, str, MAXLINE, 0, (LPSOCKADDR)&cli_buf, &sizeof(cli_buf));
  str[n]='\0';
  printf("server->client: %s\n",str);
  
  //關閉TCP socket
  closesocket(tcp_sd);
  closesocket(udp_sd);
  // 結束 WinSock DLL 的使用
  WSACleanup();  
  return 0;
}