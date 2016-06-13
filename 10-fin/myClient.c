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
// 定義字串=====================================================================
char msg_1[MAXLINE] = "歡迎使用投票程式\n正在連線伺服器中...\n";
char msg_2[MAXLINE] = "連線成功! \n";
char msg_3[MAXLINE] = "請按a, b, c, d 鍵，進行投票。\n";
char msg_4[MAXLINE] = "感謝你的投票！\n等待投票結果中...\n";
char msg_5[MAXLINE] = "";
char msg_6[MAXLINE] = "";
char msg_7[MAXLINE] = "";
char msg_8[MAXLINE] = "";
// 主程式=======================================================================
int main() {
  // 宣告區&初始化區============================================================
  WSADATA                                                               wsadata;
  SOCKET                                                         tcp_sd, udp_sd;
  struct sockaddr_in                                         serv, cli, cli_buf;// cli_buf is a buffer.
  int                                                      n, serv_len, cli_len;
  char                                                          str[MAXLINE]="";
  
   
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
  
  // 工作區=====================================================================
  
  printf("%s", msg_1);
  
  
  // 傳輸開始===================================================================
  // UDP part ==================================================================
  // 連結 udp_sd 到本機
  cli_len = sizeof(cli);
  hp = bind(udp_sd, (LPSOCKADDR)&cli, cli_len);
  if(hp < 0) {
    printf("get hp error, code: %d\n", WSAGetLastError());
    printf("Bind error!\n");
  }
  cli_len = sizeof(cli_buf);
  n=recvfrom(udp_sd, str, MAXLINE, 0, (LPSOCKADDR)&cli_buf, &cli_len);
  if(n>0){
    printf("%s", msg_2);
    str[n]='\0';
    printf("%s\n",str);
  }
  // TCP part ==================================================================
  // 連接tcp 至 server
  serv_len = sizeof(serv);
  hp = connect(tcp_sd, (LPSOCKADDR)&serv, serv_len); 
  if(hp < 0) {
    printf("get hp error, code: %d\n", WSAGetLastError());
  }
  printf("%s", msg_3);
  // 鍵盤控制
  while(1){
    if(_kbhit()){
	  	// 取得輸入的字元
	  	char ch = _getch();
	  	strcpy(str, &ch);
      if(ch=='a'||ch=='b'||ch=='c'||ch=='d'){
        send(tcp_sd, str, MAXLINE, 0);
        printf("%s", msg_4);
        break;
      }
	  }
  }    
  // TCP receive
  
  n = recv(tcp_sd, str, MAXLINE, 0); 
  str[n]='\0';
  if(n>0){
    printf("%s", str);
  }
  system("pause");
  //關閉 socket
  closesocket(tcp_sd);
  closesocket(udp_sd);
  // 結束 WinSock DLL 的使用
  WSACleanup();  
  return 0;
}