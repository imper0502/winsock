// =============================================================================
// my server
// �@��: �d�R��
// =============================================================================

// ���J�禡�w===================================================================
#include <stdio.h>
#include <string.h>
#include <winsock.h>
// �w�q����=====================================================================
#define CONNECTNUMBER                                                        128
#define MAXLINE                                                             1024//�r��w�İϪ���
// �D�{��=======================================================================
int main() {
  // �ŧi��&��l�ư�============================================================
  WSADATA                                                               wsadata;
  SOCKET                               tcp_sd, udp_sd, client_sd[CONNECTNUMBER];
  struct sockaddr_in                              server, client[CONNECTNUMBER];
  int                                                 client_len[CONNECTNUMBER];
  char                                              str[CONNECTNUMBER][MAXLINE];
  int                                                                   i, j, k;
  int                                                                      m, n;// for recv() & recvfrom()
  // �I�s WSAStrartup() ���U WinSock DLL ���ϥ�
  int nResult = WSAStartup(0x101, (LPWSADATA)&wsadata);
  if(nResult!=0) {
    printf("WSA Initialization failed: %d\n", nResult);
    WSACleanup();
    return 0;
  }
  // �}�� TCP socket
  hp = (tcp_sd = socket(AF_INET, SOCK_STREAM, 0));
  if(hp == SOCKET_ERROR) {
	printf("get hp error, code: %d\n", WSAGetLastError());
    fprintf(stderr, "echo@server: Can't open TCP socket.\n");
    return 0;
  }
  // �}�� UDP socket
  LPHOSTENT hp = (udp_sd = socket(AF_INET, SOCK_DGRAM, 0));
  if(hp == SOCKET_ERROR) {
	printf("get hp error, code: %d\n", WSAGetLastError());
	fprintf(stderr, "echo@server: Can't open UDP socket.\n");
	return 0;
  }
  // �]�w�s���\��
  char broadcast = 'a';
  setsockopt(udp_sd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
  
  // �]�w��}���c===============================================================
  // �]�wserver IP & port
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = 0;
  server.sin_port = htons(5554);                                 //"IPPORT_ECHO"
  // �]�wclient IP & port�C
  client_len[0] = sizeof(client[0]);
  client[0].sin_family = AF_INET;
  client[0].sin_addr.s_addr = inet_addr("255.255.255.255");
  client[0].sin_port = htons(5555);

  // �d�ߥ���(Server)��T=======================================================
  hp = gethostname((LPSTR)str, MAXLINE);
  if(hp != 0) {
    printf("get hp error, code: %d\n\n", WSAGetLastError());
  }else {
    printf("local host name: %s\n", str);
  }
  hp = gethostbyname(str);
  if(hp == NULL){
    printf("get hp error, code: %d\n", WSAGetLastError());
  }else {
    printf("host IP: %s\n", inet_ntoa(*(LPIN_ADDR)(hp->h_addr)));
  }
  
  // �u�@�� ====================================================================
  if(bind(tcp_sd, (LPSOCKADDR)&server, sizeof(server)) < 0) {
    fprintf(stderr, "echo@server: Can't bind local address.\n");
    return 0;
  }
  // �I�s listen() �� socket�i�J[��ť]���A�A�ó]�w
  // �̤j�P�ɥi�������s���n�D
  if(listen(tcp_sd, CONNECTNUMBER) < 0) {
    fprintf(stderr, "echo@server: listen() error!!!\n");
    return 0;
  }
  for(int i; i<CONNECTNUMBER; i++) {
    client_len[i] = sizeof(client[i]);
  }
 
  // ===========================================================================
  
  
  
  
  
  
  
  // ===========================================================================
  closesocket(udp_sd);
  closesocket(tcp_sd);
  for(int i; i<CONNECTNUMBER; i++) {
    closesocket(client_len[i]);
  }
  WSACleanup();
}//main end=====================================================================