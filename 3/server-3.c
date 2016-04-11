/* server*/

#include <stdio.h> 
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024

void main() {
  /**�ŧi��**************************************/ 
  WSADATA wsadata;
  SOCKET serv_sd, cli_sd; 
  struct sockaddr_in serv, cli;
  
  boolean calc = 0;
  int cli_len, n, step = 0;
  double x = 0, y = 0, ans = 0; 
  char str[MAXLINE], tmp[MAXLINE], op;          // str:�ǿ�μȦs tmp:ans�μȦs 
  
  /**��l�ư�************************************/ 
  
  if(WSAStartup(0x101, (LPWSADATA)&wsadata) != 0) {
    fprintf(stderr, "echo_srv: Can't use WinSock DLL.\n");
    exit(1);
  }
  
  if((serv_sd = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR) {
    fprintf(stderr, "echo_srv: Can't open TCP socket.\n");
    exit(1);
  }
  serv.sin_family = AF_INET;                    //��l��(SOCKET ADDRess)serv 
  serv.sin_addr.s_addr = 0;                     //I'm the localhost!
  serv.sin_port = htons(IPPORT_ECHO);
  
  /**�u�@��**************************************/ 
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
        /**²���p���&�r��B�z***********************/
        if(calc) {
          switch (step) {
            case 1:
              x = atof(str);
              strcpy(str, "\n������J�B��Ÿ�(+, -, *, /, ^): ");
              step = 2;
              break;
            case 2:
              if(strcmp(str, "+\n")==0) {
                op = '+';
                strcpy(str, "\n�п�J�ĤG�ӼƦr: ");
              }
              else if(strcmp(str, "-\n")==0) {
                op = '-';
                strcpy(str, "\n�п�J�ĤG�ӼƦr: ");
              }
              else if(strcmp(str, "*\n")==0) {
                op = '*';
                strcpy(str, "\n�п�J����: ");
              }
              else if(strcmp(str, "/\n")==0) {
                op = '/';
                strcpy(str, "\n�п�J����: ");
              }
              else if(strcmp(str, "^\n")==0) {
                op = '^';
                strcpy(str, "\n�п�J�����: ");
              }
              else {
                strcpy(str, "\n��J���~!\n���s��J�B��Ÿ�(+, -, *, /, ^): ");
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
              strcat(str, "\n�n�~��B���J\"c\".\n�n���}��J\"q\".");
              step = 4;
              break;
            case 4:
              if(strcmp(str, "c\n")==0) {
                strcpy(str, "\n�}�l�i��A���N�G�ƪ��|�h�B��C\n�п�J�Ĥ@�ӼƦr�C");
                ans = 0;
                step = 1;
              }else if(strcmp(str, "q\n")==0) {
                strcpy(str, "\n���}\n�^��@��Ҧ�");
                ans = 0;
                calc = 0;
                step = 0;
              }else {
                sprintf(tmp, "%f", ans);
                strcpy(str, "\nAns= ");
                strcat(str, tmp);
                strcat(str, "\n�n�~��B���J\"c\".\n�n���}��J\"q\".");
              }
              break;
          }
        }else {
          if(strcmp(str, "How are you?\n")==0) {        //���;0:�����۲� 
            strcpy(str, "Fine, thank you.");
          }else if(strcmp(str, "calc\n")==0){
            strcpy(str, "\nOK!\n�}�l�i����N�G�ƪ��|�h�B��C\n�п�J�Ĥ@�ӼƦr�C");
            calc = 1;
            step = 1;
          }else {
            strcpy(str, "What's up?(��J\"calc\" �i�J²���p����Ҧ�)");
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
