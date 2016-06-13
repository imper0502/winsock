// =============================================================================
// my client
// �@��: �d�R��
// �̨Ϊ��s���ĪG�A�Х�Notepad++ �}�ҡC
// =============================================================================

// ���J�禡�w===================================================================
#include <stdio.h>
#include <string.h>
#include <winsock.h>
// �w�q����=====================================================================
#define MAXLINE                                                             1024//�r��w�İϪ���
// �w�q�r��=====================================================================
char msg_1[MAXLINE] = "�w��ϥΧ벼�{��\n���b�s�u���A����...\n";
char msg_2[MAXLINE] = "�s�u���\! \n";
char msg_3[MAXLINE] = "�Ы�a, b, c, d ��A�i��벼�C\n";
char msg_4[MAXLINE] = "�P�§A���벼�I\n���ݧ벼���G��...\n";
char msg_5[MAXLINE] = "";
char msg_6[MAXLINE] = "";
char msg_7[MAXLINE] = "";
char msg_8[MAXLINE] = "";
// �D�{��=======================================================================
int main() {
  // �ŧi��&��l�ư�============================================================
  WSADATA                                                               wsadata;
  SOCKET                                                         tcp_sd, udp_sd;
  struct sockaddr_in                                         serv, cli, cli_buf;// cli_buf is a buffer.
  int                                                      n, serv_len, cli_len;
  char                                                          str[MAXLINE]="";
  
   
  // �I�s WSAStartup() ���U WinSock DLL ���ϥ�
  int nResult = WSAStartup(0x101, (LPWSADATA)&wsadata);
  if(nResult!=0) {
    printf("WSA Initialization failed: %d\n", nResult);
    WSACleanup();
    return 0;
  }
  // �}�� TCP socket.
  LPHOSTENT hp = (tcp_sd = socket(AF_INET, SOCK_STREAM, 0)); 
  if(hp == SOCKET_ERROR) {
    printf("get hp error, code: %d\n", WSAGetLastError());
  }
  // �}�� UDP socket
  hp = (udp_sd = socket(AF_INET, SOCK_DGRAM, 0));
  if(hp == SOCKET_ERROR) {
    printf("get hp error, code: %d\n", WSAGetLastError());
  }
  // ���s�u�@�ǳơA�]�t��g sockaddr_in ���c (serv) �C
  // ���e���Gserver �� IP ��}�Aport number �����C
  // TCP server information (server)
  serv.sin_family       = AF_INET;
  serv.sin_addr.s_addr  = inet_addr("127.0.0.1");
  serv.sin_port         = htons(5554);  
  // UDP client information (localhost)
  cli.sin_family       = AF_INET;
  cli.sin_addr.s_addr  = 0;
  cli.sin_port         = htons(5555);
  
  // �u�@��=====================================================================
  
  printf("%s", msg_1);
  
  
  // �ǿ�}�l===================================================================
  // UDP part ==================================================================
  // �s�� udp_sd �쥻��
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
  // �s��tcp �� server
  serv_len = sizeof(serv);
  hp = connect(tcp_sd, (LPSOCKADDR)&serv, serv_len); 
  if(hp < 0) {
    printf("get hp error, code: %d\n", WSAGetLastError());
  }
  printf("%s", msg_3);
  // ��L����
  while(1){
    if(_kbhit()){
	  	// ���o��J���r��
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
  //���� socket
  closesocket(tcp_sd);
  closesocket(udp_sd);
  // ���� WinSock DLL ���ϥ�
  WSACleanup();  
  return 0;
}