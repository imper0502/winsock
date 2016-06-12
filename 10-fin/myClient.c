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
// �D�{��=======================================================================
int main() {
  // �ŧi��&��l�ư�============================================================
  WSADATA                                                               wsadata;
  SOCKET                                                         tcp_sd, udp_sd;
  struct sockaddr_in                                                  serv, cli;
  int                         n, serv_len = sizeof(serv), cli_len = sizeof(cli);
  char                                              str[MAXLINE]="How are you?";
  
  // �I�s WSAStartup() ���U WinSock DLL ���ϥ�
  if(WSAStartup(0x101, (LPWSADATA)&wsadata)!=0) {
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
  // ===========================================================================
  // ���s�u�@�ǳơA�]�t��g sockaddr_in ���c (serv) �C
  // ���e���Gserver �� IP ��}�Aport number �����C
  serv.sin_family       = AF_INET;
  serv.sin_addr.s_addr  = inet_addr("127.0.0.1");
  serv.sin_port         = htons(5554);  
  
  cli.sin_family       = AF_INET;
  cli.sin_addr.s_addr  = 0;
  cli.sin_port         = htons(5554);
  
  // ===========================================================================
  
  hp = bind(udp_sd, (LPSOCKADDR)&cli, cli_len);
  if(hp < 0) {
    printf("get hp error, code: %d\n", WSAGetLastError());
    printf("Bind error!\n");
  }
  // TCP part ==================================================================
  // �s���� server
  connect(tcp_sd, (LPSOCKADDR)&serv, sizeof(serv)); 
  //�ǰehow are you��echo server
  send(tcp_sd, str, strlen(str)+1, 0); 
  printf("client->server: %s\n" ,str);
  //��echo server����
  n=recv(tcp_sd, str, MAXLINE, 0); 
  str[n]='\0';
  printf("server->client: %s\n",str);
  
  // UDP part ==================================================================
  n=recvfrom(udp_sd, str, MAXLINE, 0, (LPSOCKADDR)&cli, &cli_len);
  str[n]='\0';
  printf("server->client: %s\n",str);
  
  //����TCP socket
  closesocket(tcp_sd);
  closesocket(udp_sd);
  // ���� WinSock DLL ���ϥ�
  WSACleanup();  
  return 0;
}