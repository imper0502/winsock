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
// Main program of a thread=====================================================
void *ThreadMain(void *arg);
// Structure of arguments to pass to client thread==============================
struct ThreadArgs
{
  SOCKET                                                                _cli_sd;
};
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
char msg_10[MAXLINE] = "�Ҧ��H�������벼�F�C���G�p�U:\n";
char msg_11[MAXLINE] = "��\n";
char msg_bt[MAXLINE] = "";
// �����ܼ�
int                                                             peopleN, Num[5];
// �D�{��=======================================================================
int main() {
  // �ŧi��&��l�ư�============================================================
  // �ŧi:���������
  DWORD                                                                threadID;
  struct ThreadArgs                                                 *threadArgs;
  // WSA
  WSADATA                                                               wsadata;
  SOCKET                                                 tcp_sd, udp_sd, cli_sd;// cli_sd is for TCP.
  struct sockaddr_in                                     serv, cli_tcp, cli_udp;// cli_tcp is a buffer.
  int                                                      n, serv_len, cli_len;
  // otherwise
  char                                                          str[5][MAXLINE];
  char                                                               voting, ch;

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
  fgets(str[0], MAXLINE, stdin);                                                // �]���|Ū��W�@��Enter
  fgets(str[0], MAXLINE, stdin);                                                // �ҥH�b���Ƥ@��Ū��

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

  strcpy(msg_bt, "�����벼�}�l�C\n");
  strcat(msg_bt, msg_3);
  strcat(msg_bt, msg_4);
  strcat(msg_bt, msg_5);
  strcat(msg_bt, msg_6);
  strcat(msg_bt, msg_7);

  // �}�l�벼===================================================================
  voting = 1;
  memset(Num, 0, 5);
  while(voting==1){
    // udp �s��
    sendto(udp_sd, msg_bt, strlen(msg_bt), 0, (LPSOCKADDR)&cli_udp, sizeof(cli_udp));
    // TCP part ==============================================================
    // �s�� tcp_sd �쥻��
    serv_len = sizeof(serv);
    hp = bind(tcp_sd, (LPSOCKADDR)&serv, serv_len);
    if(hp < 0) {
      printf("get hp error, code: %d\n", WSAGetLastError());
      return 0;
    }
    // �I�s listen() �� socket�i�J[��ť]���A�A�ó]�w
    // �̤j�P�ɥi�������s���n�D
    if(listen(tcp_sd, peopleN) < 0) {
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
    // non-block mode
    u_long iMode = 1;
    ioctlsocket(tcp_sd, FIONBIO, &iMode);
    
    threadArgs = (struct ThreadArgs *)malloc(sizeof(struct ThreadArgs));
    threadArgs->_cli_sd = cli_sd;
    hp = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ThreadMain, threadArgs, 0, (LPDWORD)&threadID);
    if(hp==0){
      printf("thead create failed.\n");
    }
    Num[0] = Num[1]+Num[2]+Num[3]+Num[4];
    if(peopleN == Num[0]){
      voting = 0;
      break;
    }
  }
  if(voting==0){
    sprintf(str[1], "%d", Num[1]);
    sprintf(str[2], "%d", Num[2]);
    sprintf(str[3], "%d", Num[3]);
    sprintf(str[4], "%d", Num[4]);

    strcat(msg_10, "�ﶵA ");
    strcat(msg_10, str[1]);
    strcat(msg_10, msg_11);
    strcat(msg_10, "�ﶵB ");
    strcat(msg_10, str[2]);
    strcat(msg_10, msg_11);
    strcat(msg_10, "�ﶵC ");
    strcat(msg_10, str[3]);
    strcat(msg_10, msg_11);
    strcat(msg_10, "�ﶵD ");
    strcat(msg_10, str[4]);
    strcat(msg_10, msg_11);
    strcat(msg_10, '\0');
  }
  int i=0;
  while(voting==0){
    sleep(1);    
    sendto(udp_sd, msg_10, strlen(msg_10), 0, (LPSOCKADDR)&cli_udp, sizeof(cli_udp));
    if((i++)==10) break;
  }
  system("pause");

  // ===========================================================================
  closesocket(udp_sd);
  closesocket(tcp_sd);
  closesocket(cli_sd);

  WSACleanup();
}//main end=====================================================================

// ThreadMain===================================================================
void *ThreadMain(void *threadArgs){
  //
  SOCKET                                                                 cli_sd;
  char                                                         ch, buf[MAXLINE];
  int                                                                         n;
  //
  cli_sd = ((struct ThreadArgs *)threadArgs)->_cli_sd;
  free(threadArgs);
  
  // tcp ��
  n = recv(cli_sd, &ch, 1, 0);
  if(n > 0){
    printf("%d\t%c", cli_sd, ch);
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
    printf("\t%d\t%d\t%d\t%d\n", Num[1], Num[2], Num[3], Num[4]);
  }
  // tcp �e
  send(cli_sd, msg_9, strlen(msg_9), 0);

  closesocket(cli_sd);
  return(NULL);
}
