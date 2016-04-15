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
    fprintf(stderr, "\necho_srv: Can't use WinSock DLL.\n");
    exit(1);
  }
  
  /*
  ** 開啟 TCP socket
  **/
  if((serv_sd = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR) {
    fprintf(stderr, "\necho_srv: Can't open TCP socket.\n");
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
    fprintf(stderr, "\necho_srv: Can't bind local address.\n");
    exit(1);
  }
  
  /*
  ** 呼叫 listen() 使 socket進入[監聽]狀態，並設定
  ** 最大同時可接受的連結要求
  **/
  
  if( listen(serv_sd, 5) < 0) {
    fprintf(stderr, "\necho_srv: listen() error!!!\n");
    exit(1);
  }
  
  cli_len_a = sizeof(cli_a);
  cli_len_b = sizeof(cli_b);
    
  while(1) {
    // 等accept() return
    printf("Server: waiting for client-A...\n");
    if((cli_sd_a = accept(serv_sd, (LPSOCKADDR)&cli_a, &cli_len_a)) == SOCKET_ERROR) {
      fprintf(stderr, "\necho_srv: accept() error!!!\n");
      closesocket(cli_sd_a);
    }else {
      printf("Server: client-A is online.\n");
      strcpy( str_a, "Hello!");
      if(send(cli_sd_a, str_a, strlen(str_a), 0) == SOCKET_ERROR) {
        fprintf(stderr, "\necho_srv: Connection closed.\n");
        break;
      }else {
        printf("Server >>> cli-A: %s\n", str_a);
      }
      printf("Server: waiting for client-B...\n");
      if((cli_sd_b = accept(serv_sd, (LPSOCKADDR)&cli_b, &cli_len_b)) == SOCKET_ERROR) {
        fprintf(stderr, "\necho_srv: accept() error!!!\n");
        closesocket(cli_sd_b);
      }else {
        printf("Server: client-B is online.\n");
        strcpy( str_b, "Hey!");
        if(send(cli_sd_b, str_b, strlen(str_b), 0) == SOCKET_ERROR) {
          fprintf(stderr, "\necho_srv: Connection closed.\n");
          break;
        }else {
          printf("Server >>> cli-B: %s\n", str_b);
        }
        // accept() OK!
        while(1) {
          // 接收
          if((n = recv(cli_sd_a, str_a, MAXLINE, 0))==0) {
            fprintf(stderr, "\necho_srv: Connection closed.\n");
            break;
          }else if (n == SOCKET_ERROR) {
            fprintf(stderr, "\necho_srv: recv() error!!!\n");
            break;
          }
          // 如果有收到，修正str結尾，補一個NULL
          if(n>1) str_a[n-1] = '\0';
          else str_a[1] = '\0';
          // show str
          printf("Server <<< cli-A: %s\n\n", str_a);
          //===================================
            
          // TO DO:
            
          //===================================
          // 傳送出去
          if(send(cli_sd_a, str_a, strlen(str_a), 0) == SOCKET_ERROR) {
            fprintf(stderr, "\necho_srv: Connection closed.\n");
            break;
          }else if(send(cli_sd_b, str_a, strlen(str_a), 0) == SOCKET_ERROR) {
            fprintf(stderr, "\necho_srv: Connection closed.\n");
            break;
          }else {
            printf("Server >>> cli-A: %s\n", str_a);
            printf("Server >>> cli-B: %s\n", str_a);
          }
					//===========================================
          // 判斷是否跳出
          if(strcmp(str_a, "over")==0){
          	
          }  
          //===========================================
        }//while 2-1 end
        
        while(1) {
        	if(strcmp(str_a, "over")==0){
          	
          }
				}//while 2-2 end
        //===================================
            
        // TO DO:
            
        //===================================
      }
      //===================================
            
      // TO DO:
            
      //===================================
    }
    //===================================
            
    // TO DO:
            
    //===================================
  }//while 1 end
  closesocket(cli_sd_a);
	closesocket(cli_sd_b);
  closesocket(serv_sd);
  WSACleanup();
}//main end
