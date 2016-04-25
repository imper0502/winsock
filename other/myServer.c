/********** 
** server
***********/
#include <stdio.h> 
#include <string.h>
#include <winsock.h>

#define CONNECTNUMBER 2
#define MAXLINE 1024 //字串緩衝區長度

int main() {
  // 宣告 
  WSADATA wsadata;
  SOCKET server_sd, 
         client_sd[CONNECTNUMBER]; 
	struct sockaddr_in server,
                     client[CONNECTNUMBER];
  int client_len[CONNECTNUMBER],  
      tmp[CONNECTNUMBER];
  char str[CONNECTNUMBER][MAXLINE], 
       tempStr[MAXLINE]; 
  
  // 呼叫 WSAStrartup() 註冊 WinSock DLL 的使用
  if(WSAStartup(0x101, (LPWSADATA)&wsadata) != 0) {
    fprintf(stderr, "echo@server: Can't use WinSock DLL.\n");
    return;
  }
  
  // 開啟 TCP socket
  if((server_sd = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR) {
    fprintf(stderr, "echo@server: Can't open TCP socket.\n");
    return;
  }
  
  // 指定socket 的 IP & port
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = 0;
  server.sin_port = htons(IPPORT_ECHO);
  
  //======== 工作區 ==================================
  if(bind(serv_sd, (LPSOCKADDR)&serv, sizeof(serv)) < 0) {
    fprintf(stderr, "echo@server: Can't bind local address.\n");
    return;
  }
  
  // 呼叫 listen() 使 socket進入[監聽]狀態，並設定
  // 最大同時可接受的連結要求
  if( listen(serv_sd, CONNECTNUMBER) < 0) {
    fprintf(stderr, "echo@server: listen() error!!!\n");
    return;
  }
  
  for(int i; i<CONNECTNUMBER; i++) {
    client_len[i] = sizeof(client[i]);
  }
  while(1) {
    // 等accept() return
    for(int i; i<CONNECTNUMBER; i++) {
      printf("echo@server: waiting for client...\n");
      if((client_sd[i] = accept(server_sd, (LPSOCKADDR)&client[i], &client_len[i])) == SOCKET_ERROR) {
        fprintf(stderr, "echo@server: accept() error!!!\n");
        closesocket(cli_sd_a);
      }else {
        printf("echo@server: client is online.\n");
        strcpy( str[i], "SENDMODE");//RECVMODE
        if(send(client_sd[i], str[i], strlen(str[i]), 0) == SOCKET_ERROR) {
          fprintf(stderr, "echo@server: Connection closed.\n");
          break;
        }else {
          printf("Server >>> client[%d]: %s\n", i,  str[i]);
        }
      }
    }
    // accept() OK!
    // 開始溝通
    while(1) {
      for(int i; i<CONNECTNUMBER; i++) {
        // 接收
        if((tmp[i] = recv(client_sd[i], str[i], MAXLINE, 0))==0) {
          fprintf(stderr, "echo@server: Connection closed.\n");
          break;
        }else if (n == SOCKET_ERROR) {
          fprintf(stderr, "echo@server: recv() error!!!\n");
          break;
        }
        // 如果有收到，修正str結尾，補一個NULL
        if(tmp[i]>1) str_a[n-1] = '\0';
        else str[i][1] = '\0';
        // show str
        printf("Server <<< client[%d]: %s\n\n", i, str[i]);
        //===================================
        
        // TO DO:
          
        //===================================
        // 傳送出去
        if(send(cli_sd[i], str[i], strlen(str[i]), 0) == SOCKET_ERROR) {
          fprintf(stderr, "echo@server: Connection closed.\n");
          break;
        }else if(send(cli_sd_b, str_a, strlen(str_a), 0) == SOCKET_ERROR) {
          fprintf(stderr, "echo@server: Connection closed.\n");
          break;
        }else {
          printf("Server >>> client[%d]: %s\n", i, str[i]);
        }
        // 判斷是否跳出
        if(strcmp(str[i], "over")==0){      	
        }
      }
    }//while 2-1 end
  }
  for(int i; i<CONNECTNUMBER; i++) {
    closesocket(cli_sd[i]);
  }
  WSACleanup();
}//main end

int main() {

  // 呼叫 WSAStrartup() 註冊 WinSock DLL 的使用
  int nResult = WSAStartup(0x101, (LPWSADATA)&wsadata);
  if(nResult!=0) {
    printf("WSA Initialization failed: %d\n", nResult);
    WSACleanup();
    return;
  }
  
  // 開啟 TCP socket
  LPHOSTENT hp = (serv_sd = socket(AF_INET, SOCK_STREAM, 0));
  if(hp == SOCKET_ERROR) {
    printf("get hp error, code: %d\n", WSAGetLastError());
  }
  
  //指定 socket 的 IP 位址和 port number
  serv.sin_family      = AF_INET;
  serv.sin_addr.s_addr = 0;
  serv.sin_port        = htons(IPPORT_ECHO); // 指定IPPORT_ECHO 為 echo port

  //bind(serv_sd, &serv, sizeof(serv)) ;
  hp = bind(serv_sd, (LPSOCKADDR) &serv, sizeof(serv));
  if(hp < 0) {
    printf("get hp error, code: %d\n", WSAGetLastError());
  }
  
  // 查詢本機名稱
  printf("What is local host's name?\n");
  hp = gethostname((LPSTR)str, MAXLINE);
  if(hp != 0) {
    printf("get hp error, code: %d\n\n", WSAGetLastError());
  }else {
    printf("local host name: %s\n", str);
    
  }
  
  // 查詢本機 IP資訊
  printf("What is %s's IP?\n", str);
  hp = gethostbyname(str);
  if(hp == NULL){
    printf("get hp error, code: %d\n", WSAGetLastError());
  }else {
    printf("host name: %s\n", hp->h_name);
    printf("host nickname: %s\n", hp->h_aliases[0]);
    printf("host IP: %s\n", inet_ntoa(*(LPIN_ADDR)(hp->h_addr)));
  }
  
  cli_len = sizeof(cli);

  cli_sd = accept(serv_sd, (LPSOCKADDR) &cli, &cli_len);

  getpeername(cli_sd, (LPSOCKADDR)&cli, &cli_len);
  printf("client's IP: %s\n", inet_ntoa(cli.sin_addr));
  printf("client's port: %d\n\n", ntohs(cli.sin_port));
