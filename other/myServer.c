/********** 
** server
***********/
#include <stdio.h> 
#include <string.h>
#include <winsock.h>

#define CONNECTNUMBER 2
#define MAXLINE 1024 //�r��w�İϪ���

int main() {
  // �ŧi 
  WSADATA wsadata;
  SOCKET server_sd, 
         client_sd[CONNECTNUMBER]; 
	struct sockaddr_in server,
                     client[CONNECTNUMBER];
  int client_len[CONNECTNUMBER],  
      tmp[CONNECTNUMBER];
  char str[CONNECTNUMBER][MAXLINE], 
       tempStr[MAXLINE]; 
  
  // �I�s WSAStrartup() ���U WinSock DLL ���ϥ�
  if(WSAStartup(0x101, (LPWSADATA)&wsadata) != 0) {
    fprintf(stderr, "echo@server: Can't use WinSock DLL.\n");
    return;
  }
  
  // �}�� TCP socket
  if((server_sd = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR) {
    fprintf(stderr, "echo@server: Can't open TCP socket.\n");
    return;
  }
  
  // ���wsocket �� IP & port
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = 0;
  server.sin_port = htons(IPPORT_ECHO);
  
  //======== �u�@�� ==================================
  if(bind(serv_sd, (LPSOCKADDR)&serv, sizeof(serv)) < 0) {
    fprintf(stderr, "echo@server: Can't bind local address.\n");
    return;
  }
  
  // �I�s listen() �� socket�i�J[��ť]���A�A�ó]�w
  // �̤j�P�ɥi�������s���n�D
  if( listen(serv_sd, CONNECTNUMBER) < 0) {
    fprintf(stderr, "echo@server: listen() error!!!\n");
    return;
  }
  
  for(int i; i<CONNECTNUMBER; i++) {
    client_len[i] = sizeof(client[i]);
  }
  while(1) {
    // ��accept() return
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
    // �}�l���q
    while(1) {
      for(int i; i<CONNECTNUMBER; i++) {
        // ����
        if((tmp[i] = recv(client_sd[i], str[i], MAXLINE, 0))==0) {
          fprintf(stderr, "echo@server: Connection closed.\n");
          break;
        }else if (n == SOCKET_ERROR) {
          fprintf(stderr, "echo@server: recv() error!!!\n");
          break;
        }
        // �p�G������A�ץ�str�����A�ɤ@��NULL
        if(tmp[i]>1) str_a[n-1] = '\0';
        else str[i][1] = '\0';
        // show str
        printf("Server <<< client[%d]: %s\n\n", i, str[i]);
        //===================================
        
        // TO DO:
          
        //===================================
        // �ǰe�X�h
        if(send(cli_sd[i], str[i], strlen(str[i]), 0) == SOCKET_ERROR) {
          fprintf(stderr, "echo@server: Connection closed.\n");
          break;
        }else if(send(cli_sd_b, str_a, strlen(str_a), 0) == SOCKET_ERROR) {
          fprintf(stderr, "echo@server: Connection closed.\n");
          break;
        }else {
          printf("Server >>> client[%d]: %s\n", i, str[i]);
        }
        // �P�_�O�_���X
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

  // �I�s WSAStrartup() ���U WinSock DLL ���ϥ�
  int nResult = WSAStartup(0x101, (LPWSADATA)&wsadata);
  if(nResult!=0) {
    printf("WSA Initialization failed: %d\n", nResult);
    WSACleanup();
    return;
  }
  
  // �}�� TCP socket
  LPHOSTENT hp = (serv_sd = socket(AF_INET, SOCK_STREAM, 0));
  if(hp == SOCKET_ERROR) {
    printf("get hp error, code: %d\n", WSAGetLastError());
  }
  
  //���w socket �� IP ��}�M port number
  serv.sin_family      = AF_INET;
  serv.sin_addr.s_addr = 0;
  serv.sin_port        = htons(IPPORT_ECHO); // ���wIPPORT_ECHO �� echo port

  //bind(serv_sd, &serv, sizeof(serv)) ;
  hp = bind(serv_sd, (LPSOCKADDR) &serv, sizeof(serv));
  if(hp < 0) {
    printf("get hp error, code: %d\n", WSAGetLastError());
  }
  
  // �d�ߥ����W��
  printf("What is local host's name?\n");
  hp = gethostname((LPSTR)str, MAXLINE);
  if(hp != 0) {
    printf("get hp error, code: %d\n\n", WSAGetLastError());
  }else {
    printf("local host name: %s\n", str);
    
  }
  
  // �d�ߥ��� IP��T
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
