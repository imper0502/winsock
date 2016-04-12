/* server*/

#include <stdio.h> 
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024

void main() {
  /**宣告區**************************************/ 
  WSADATA wsadata;
  SOCKET serv_sd, cli_sd_a, cli_sd_b; 
  struct sockaddr_in serv, cli_a, cli_b;
  
  boolean calc = 0;
  int cli_len_a, cli_len_b, n, step = 0;
  double x = 0, y = 0, ans = 0; 
  char str[MAXLINE], tmp[MAXLINE], op;
  
  /**初始化區************************************/ 
  /*
  **呼叫 WSAStrartup() 註冊 WinSock DLL 的使用
  */
  if(WSAStartup(0x101, (LPWSADATA)&wsadata) != 0) {
    fprintf(stderr, "echo_srv: Can't use WinSock DLL.\n");
    exit(1);
  }
  /*
  **開啟 TCP socket
  **/
  if((serv_sd = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR) {
    fprintf(stderr, "echo_srv: Can't open TCP socket.\n");
    exit(1);
  }
  /*
  **指定socket 的 IP & port
  **/
  serv.sin_family = AF_INET;
  serv.sin_addr.s_addr = 0;
  serv.sin_port = htons(IPPORT_ECHO);
  
  /**工作區**************************************/ 
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
  while(1) {
    printf("Server: waiting for client...\n");
    while(1) {
    if((cli_sd_a = accept(serv_sd, (LPSOCKADDR)&cli_a, &cli_len_a)) == SOCKET_ERROR) {
      fprintf(stderr, "echo_srv: accept() error!!!\n");
      closesocket(cli_sd_a);
    }else {
     
        if((n = recv(cli_sd_a, str, MAXLINE, 0))==0) {
          fprintf(stderr, "echo_srv: Connection closed.\n");
          break;
        }else if (n == SOCKET_ERROR) {
          fprintf(stderr, "echo_srv: recv() error!!!\n");
          break;
        }
        str[n] = '\0';
        printf("Server <<< cli-A: %s", str);
        strcpy(str, "What's up?\n");
        /********************************************/
        if(send(cli_sd_a, str, strlen(str), 0) == SOCKET_ERROR) {
          fprintf(stderr, "echo_srv: Connection closed.\n");
          break;
        }
        printf("Server >>> cli-A: %s", str);
        break;
      }
    }
    while(1) {
    if((cli_sd_b = accept(serv_sd, (LPSOCKADDR)&cli_b, &cli_len_b)) == SOCKET_ERROR) {
      fprintf(stderr, "echo_srv: accept() error!!!\n");
      closesocket(cli_sd_b);
    }else {
      
        if((n = recv(cli_sd_b, str, MAXLINE, 0))==0) {
          fprintf(stderr, "echo_srv: Connection closed.\n");
          break;
        }else if (n == SOCKET_ERROR) {
          fprintf(stderr, "echo_srv: recv() error!!!\n");
          break;
        }
        str[n] = '\0';
        printf("Server <<< cli-B: %s", str);
        strcpy(str, "What's up?\n");
        /********************************************/
        if(send(cli_sd_b, str, strlen(str), 0) == SOCKET_ERROR) {
          fprintf(stderr, "echo_srv: Connection closed.\n");
          break;
        }
        printf("Server >>> cli-B: %s", str);
        break;
      }
    }
  }
  closesocket(cli_sd_a);
  closesocket(cli_sd_b);
  closesocket(serv_sd);
  WSACleanup();
}
