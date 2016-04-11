/* server*/

#include <stdio.h> 
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024

void main() {
  /**宣告區**************************************/ 
  WSADATA wsadata;
  SOCKET serv_sd, cli_sd; 
  struct sockaddr_in serv, cli;
  
  boolean calc = 0;
  int cli_len, n, step = 0;
  double x = 0, y = 0, ans = 0; 
  char str[MAXLINE], tmp[MAXLINE], op;          // str:傳輸用暫存 tmp:ans用暫存 
  
  /**初始化區************************************/ 
  
  if(WSAStartup(0x101, (LPWSADATA)&wsadata) != 0) {
    fprintf(stderr, "echo_srv: Can't use WinSock DLL.\n");
    exit(1);
  }
  
  if((serv_sd = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR) {
    fprintf(stderr, "echo_srv: Can't open TCP socket.\n");
    exit(1);
  }
  serv.sin_family = AF_INET;                    //初始化(SOCKET ADDRess)serv 
  serv.sin_addr.s_addr = 0;                     //I'm the localhost!
  serv.sin_port = htons(IPPORT_ECHO);
  
  /**工作區**************************************/ 
  if(bind(serv_sd, (LPSOCKADDR)&serv, sizeof(serv)) < 0) {
    fprintf(stderr, "echo_srv: Can't bind local address.\n");
    exit(1);
  }
  
  if( listen(serv_sd, 5) < 0) {
    fprintf(stderr, "echo_srv: listen() error!!!\n");
    exit(1);
  }
  
  cli_len = sizeof(cli);
  
  while(1) {
    printf("Server: waiting for client...\n");
    if((cli_sd = accept(serv_sd, (LPSOCKADDR)&cli, &cli_len)) == SOCKET_ERROR) {
      fprintf(stderr, "echo_srv: accept() error!!!\n");
      closesocket(cli_sd);
    }else {
      while(1) {
        if((n = recv(cli_sd, str, MAXLINE, 0))==0) {
          fprintf(stderr, "echo_srv: Connection closed.\n");
          break;
        }else if (n == SOCKET_ERROR) {
          fprintf(stderr, "echo_srv: recv() error!!!\n");
          break;
        }
        str[n] = '\0';
        printf("Server <<< %s\n", str);
        /**簡易計算機&字串處理***********************/
        if(calc) {
          switch (step) {
            case 1:
              x = atof(str);
              strcpy(str, "\n直接輸入運算符號(+, -, *, /, ^): ");
              step = 2;
              break;
            case 2:
              if(strcmp(str, "+\n")==0) {
                op = '+';
                strcpy(str, "\n請輸入第二個數字: ");
              }
              else if(strcmp(str, "-\n")==0) {
                op = '-';
                strcpy(str, "\n請輸入第二個數字: ");
              }
              else if(strcmp(str, "*\n")==0) {
                op = '*';
                strcpy(str, "\n請輸入乘數: ");
              }
              else if(strcmp(str, "/\n")==0) {
                op = '/';
                strcpy(str, "\n請輸入分母: ");
              }
              else if(strcmp(str, "^\n")==0) {
                op = '^';
                strcpy(str, "\n請輸入次方數: ");
              }
              else {
                strcpy(str, "\n輸入錯誤!\n重新輸入運算符號(+, -, *, /, ^): ");
                break;
              }
              step = 3;
              break;  
            case 3:
              y = atof(str);
              if(op == '+') {
                ans = x+y;
              }
              else if(op == '-') {
                ans = x-y;
              }
              else if(op == '*') {
                ans = x*y;
              }
              else if(op == '/') {
                ans = x/y;
              }
              else if(op == '^') {
                ans = 1;
                int i;
                for(i=0; i<y; i++) {
                  ans *= x;
                }
              }
              sprintf(tmp, "%f", ans);
              strcpy(str, "\nAns= ");
              strcat(str, tmp);
              strcat(str, "\n要繼續運算輸入\"c\".\n要離開輸入\"q\".");
              step = 4;
              break;
            case 4:
              if(strcmp(str, "c\n")==0) {
                strcpy(str, "\n開始進行，任意二數的四則運算。\n請輸入第一個數字。");
                ans = 0;
                step = 1;
              }else if(strcmp(str, "q\n")==0) {
                strcpy(str, "\n離開\n回到一般模式");
                ans = 0;
                calc = 0;
                step = 0;
              }else {
                sprintf(tmp, "%f", ans);
                strcpy(str, "\nAns= ");
                strcat(str, tmp);
                strcat(str, "\n要繼續運算輸入\"c\".\n要離開輸入\"q\".");
              }
              break;
          }
        }else {
          if(strcmp(str, "How are you?\n")==0) {        //比對;0:完全相符 
            strcpy(str, "Fine, thank you.");
          }else if(strcmp(str, "calc\n")==0){
            strcpy(str, "\nOK!\n開始進行任意二數的四則運算。\n請輸入第一個數字。");
            calc = 1;
            step = 1;
          }else {
            strcpy(str, "What's up?(輸入\"calc\" 進入簡易計算機模式)");
          }
        }
        /********************************************/
        if(send(cli_sd, str, strlen(str), 0) == SOCKET_ERROR) {
          fprintf(stderr, "echo_srv: Connection closed.\n");
          break;
        }
        printf("Server >>> %s\n", str);
      }
    }
  }
  closesocket(cli_sd);
  closesocket(serv_sd);
  WSACleanup();
}
