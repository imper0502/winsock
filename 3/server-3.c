/**** 
** 
** server
**
****/

#include <stdio.h> 
#include <string.h>
#include <winsock.h>
#define MAXLINE 1024

void main() {
  /*
  ** 宣告
  **/ 
  WSADATA wsadata;
  SOCKET serv_sd, cli_sd_a, cli_sd_b; 
  struct sockaddr_in serv, cli_a, cli_b;
  int cli_len_a, cli_len_b, n, m;
  char str_a[MAXLINE], str_b[MAXLINE]; 
  
  /*
  ** 呼叫 WSAStrartup() 註冊 WinSock DLL 的使用
  **/
  if(WSAStartup(0x101, (LPWSADATA)&wsadata) != 0) {
    fprintf(stderr, "echo_srv: Can't use WinSock DLL.\n");
    exit(1);
  }
  
  /*
  ** 開啟 TCP socket
  **/
  if((serv_sd = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR) {
    fprintf(stderr, "echo_srv: Can't open TCP socket.\n");
    exit(1);
  }
  
  /*
  ** 指定socket 的 IP & port
  **/
  serv.sin_family = AF_INET;
  serv.sin_addr.s_addr = 0;
  serv.sin_port = htons(IPPORT_ECHO);
  
  /*
  ** 工作區
  **/
  if(bind(serv_sd, (LPSOCKADDR)&serv, sizeof(serv)) < 0) {
    fprintf(stderr, "echo_srv: Can't bind local address.\n");
    exit(1);
  }
  
  /*
  ** 呼叫 listen() 使 socket進入[監聽]狀態，並設定
  ** 最大同時可接受的連結要求
  **/
  if( listen(serv_sd, 5) < 0) {
    fprintf(stderr, "echo_srv: listen() error!!!\n");
    exit(1);
  }
  
  cli_len_a = sizeof(cli_a);
  cli_len_b = sizeof(cli_b);
  
  /*
  ** 工作區
  **/ 
  while(1) {
    // 等accept() return
    printf("Server: waiting for client...\n");
    if((cli_sd_a = accept(serv_sd, (LPSOCKADDR)&cli_a, &cli_len_a)) == SOCKET_ERROR) {
      fprintf(stderr, "echo_srv: accept() error!!!\n");
      closesocket(cli_sd_a);
    }else if((cli_sd_b = accept(serv_sd, (LPSOCKADDR)&cli_b, &cli_len_b)) == SOCKET_ERROR) {
      fprintf(stderr, "echo_srv: accept() error!!!\n");
      closesocket(cli_sd_b);
    }else{
      // accept() OK!
      while(1) {
        // 先接收
        if((n = recv(cli_sd_a, str_a, MAXLINE, 0))==0) {
          fprintf(stderr, "echo_srv: Connection closed.\n");
          break;
        }else if (n == SOCKET_ERROR) {
          fprintf(stderr, "echo_srv: recv() error!!!\n");
          break;
        }
        // 如果有收到，補一個NULL，修正str結尾
        //if(str_a[n-1] != '\n') str[n++] = '\n';
        str_a[n] = '\0';
        
        if((m = recv(cli_sd_b, str_b, MAXLINE, 0))==0) {
          fprintf(stderr, "echo_srv: Connection closed.\n");
          break;
        }else if (m == SOCKET_ERROR) {
          fprintf(stderr, "echo_srv: recv() error!!!\n");
          break;
        }             
        //if(str_b[m-1] != '\n') str[m++] = '\n';
        str_b[m] = '\0';
        // show str
        printf("Server <<< cli-A: %s", str_a);
        printf("Server <<< cli-B: %s", str_b);
        /************************************/
        
        // TO DO HERE IN str
        
        /************************************/
        // 傳送出去
        if(send(cli_sd_a, str_b, strlen(str_b), 0) == SOCKET_ERROR) {
          fprintf(stderr, "echo_srv: Connection closed.\n");
          break;
        }
        printf("Server >>> cli-A: %s", str_b);
        
        // 傳送出去
        if(send(cli_sd_b, str_a, strlen(str_a), 0) == SOCKET_ERROR) {
          fprintf(stderr, "echo_srv: Connection closed.\n");
          break;
        }
        printf("Server >>> cli-B: %s", str_a);
      }
    }
  }
  closesocket(cli_sd_a);
  closesocket(cli_sd_b);
  closesocket(serv_sd);
  WSACleanup();
}
