#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024

int main(int argc, char** argv) {
    WSADATA wsadata;
    SOCKET  sd;
    struct  sockaddr_in serv;
    int     n, time = 0, serv_len = sizeof(serv);
    char    str[1024] = "How are you?";
// �I�s WSAStartup() ���U WinSock DLL ���ϥ�
    WSAStartup(0x101,(LPWSADATA) &wsadata);
    sd=socket(AF_INET, SOCK_DGRAM, 0);
// ���s�u�@�ǳơA�]�t���g sockaddr_in ���c (serv) �C
// ���e���Gserver �� IP ���}�Aport number �����C
    serv.sin_family       = AF_INET;
    serv.sin_addr.s_addr  = inet_addr("127.0.0.1");
    serv.sin_port         = htons(5678);
// �ǰehow are you��echo server
    sendto(sd, str, strlen(str)+1, 0, (LPSOCKADDR)&serv, serv_len);
    printf("client: client->server: %s\n" ,str);
// ��echo server����
    while((n = recvfrom(sd, str, MAXLINE, 0, (LPSOCKADDR)&serv, &serv_len)) <= 0) time++;
    // n=recvfrom(sd, str, MAXLINE, 0, (LPSOCKADDR)&serv, &serv_len);
    str[n]='\0';
    printf("client: server->client: %s\n",str);
// ����TCP socket
    closesocket(sd);
// ���� WinSock DLL ���ϥ�
    WSACleanup();
    return 0;
}
