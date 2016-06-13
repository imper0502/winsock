// =============================================================================
// my server
// �@��: �d�R��
// �̨Ϊ��s���ĪG�A�Х�Notepad++ �}�ҡC
// =============================================================================

// ���J�禡�w===================================================================
#include <stdio.h>
#include <string.h>
#include <winsock.h>
// �w�q����=====================================================================
#define CONNECTNUMBER                                                        128
#define MAXLINE                                                             1024//�r��w�İϪ���
// �w�q�r��=====================================================================
char msg_1[MAXLINE] = "�벼�{�����A�ݡC�@�̡G�d�R��\n";
char msg_2[MAXLINE] = "�п�J�`�벼�H��: ";
char msg_3[MAXLINE] = "�п�Jĳ�D: ";
char msg_4[MAXLINE] = "�п�J�ﶵA: ";
char msg_5[MAXLINE] = "�п�J�ﶵB: ";
char msg_6[MAXLINE] = "�п�J�ﶵC: ";
char msg_7[MAXLINE] = "�п�J�ﶵD: ";
char msg_8[MAXLINE] = "�}�l�벼�C�е��ݧ벼����...\n";
char msg_9[MAXLINE] = "�P�§A���벼�C�е��ݧ벼���G�C\n";
char msg_10[MAXLINE] = "";
char msg_11[MAXLINE] = "";
char msg_12[MAXLINE] = "�Ҧ��H�������벼�F�C���G�p�U:\n";
// �����ܼ�
int                                                             peopleN, Num[5];

// �D�{��=======================================================================
int main() {
  // �ŧi��&��l�ư�============================================================
  WSADATA                                                               wsadata;
  SOCKET                                                 tcp_sd, udp_sd, cli_sd;// cli_sd is for TCP.
  struct sockaddr_in                                     serv, cli_tcp, cli_udp;// cli_tcp is a buffer.
  int                                                      n, serv_len, cli_len;
  char                                                          str[5][MAXLINE];
  char                                                                       ch;
  
  // �I�s WSAStrartup() ���U WinSock DLL ���ϥ�
  int nResult = WSAStartup(0x101, (LPWSADATA)&wsadata);
  if(nResult!=0){
    printf("WSA Initialization failed: %d\n", nResult);
    WSACleanup();
    return 0;
  }
  // �}�� TCP socket
  LPHOSTENT hp = (tcp_sd = socket(AF_INET, SOCK_STREAM, 0));
  if(hp == SOCKET_ERROR){
    printf("get hp error, code: %d\n", WSAGetLastError());
    fprintf(stderr, "Can't open TCP socket.\n");
    return 0;
  }
  // �}�� UDP socket
  hp = (udp_sd = socket(AF_INET, SOCK_DGRAM, 0));
  if(hp == SOCKET_ERROR) {
    printf("get hp error, code: %d\n", WSAGetLastError());
    fprintf(stderr, "Can't open UDP socket.\n");
    return 0;
  }
  // �]�w�s���\��
  char broadcast = 'a';
  setsockopt(udp_sd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));

  // �]�w��}���c===============================================================
  // �]�wserver IP & port to sockaddr_in server for TCP Socket
  serv.sin_family = AF_INET;
  serv.sin_addr.s_addr = 0;                                                     // ����
  serv.sin_port = htons(5554);                                                  // "IPPORT_ECHO"
  // cli_tcp ��buffer�A�ҥH���ݭn�]�w
  // �]�wclient IP & port to sockaddr_in client for UDP Socket
  cli_udp.sin_family = AF_INET;
  cli_udp.sin_addr.s_addr = inet_addr("255.255.255.255");                       // �s��
  cli_udp.sin_port = htons(5555);                                               // �Υt�@��port
  
  // �u�@�� ====================================================================
  
    // ����w��T��
  printf("%s", msg_1);
  // �n�D��J
  printf("%s", msg_2);
  fscanf(stdin, "%d", &peopleN);
  
  printf("%s", msg_3);
  fgets(str[0], MAXLINE, stdin);
  fgets(str[0], MAXLINE, stdin);
  
  printf("%s", msg_4);
  fgets(str[1], MAXLINE, stdin);
  printf("%s", msg_5);
  fgets(str[2], MAXLINE, stdin);
  printf("%s", msg_6);
  fgets(str[3], MAXLINE, stdin);
  printf("%s", msg_7);
  fgets(str[4], MAXLINE, stdin);
  printf("%s", msg_8); 
  
  // �r��B�z
  strcpy(msg_3, "ĳ�D: ");
  strcat(msg_3, str[0]);  
  strcpy(msg_4, "(A) ");
  strcat(msg_4, str[1]);  
  strcpy(msg_5, "(B) ");
  strcat(msg_5, str[2]);  
  strcpy(msg_6, "(C) ");
  strcat(msg_6, str[3]);  
  strcpy(msg_7, "(D) ");
  strcat(msg_7, str[4]);  

  strcpy(str[0], "�����벼�}�l�C\n");
  strcat(str[0], msg_3);  
  strcat(str[0], msg_4);  
  strcat(str[0], msg_5);  
  strcat(str[0], msg_6);  
  strcat(str[0], msg_7);  
  
  // �}�l�벼===================================================================
  memset(Num, 0, 5);
  // UDP part ==================================================================
  // udp �s��
  sendto(udp_sd, str[0], strlen(str[0]), 0, (LPSOCKADDR)&cli_udp, sizeof(cli_udp));

  // TCP part ==================================================================
  // �s�� tcp_sd �쥻��
  serv_len = sizeof(serv);
  hp = bind(tcp_sd, (LPSOCKADDR)&serv, serv_len);
  if(hp < 0) {
    printf("get hp error, code: %d\n", WSAGetLastError());
    return 0;
  }
  // �I�s listen() �� socket�i�J[��ť]���A�A�ó]�w
  // �̤j�P�ɥi�������s���n�D
  if(listen(tcp_sd, 3) < 0) {
    fprintf(stderr, "server: listen() error!!!\n");
    return 0;
  }
  // �]�wtcp client����
  cli_len = sizeof(cli_tcp);
  // ��listen �q serv_sd �쪺 tcp client accept �� cli_sd >>> �إ߳q�D
  cli_sd = accept(tcp_sd, (LPSOCKADDR)&cli_tcp, &cli_len);
  if(cli_sd < 0) {
    printf("get hp error, code: %d\n", WSAGetLastError());
    return 0;
  }
  // tcp ��
  memset(str[0], '\0', MAXLINE);
  n = recv(cli_sd, &ch, 1, 0);
  
  if(n > 0){
    printf("%c", ch);
    switch(ch){
      case 'a':
        Num[1]++;
	      break; 
      case 'b':
        Num[2]++;
	      break;
      case 'c':
        Num[3]++;
	      break;
      case 'd':
        Num[4]++;
	      break;
	  }  
  }
  // tcp �e
  send(cli_sd, msg_9, strlen(msg_9), 0);
  system("pause");

  // ===========================================================================
  closesocket(udp_sd);
  closesocket(tcp_sd);
  closesocket(cli_sd);

  WSACleanup();
}//main end=====================================================================