#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024

void main() {
    WSADATA wsadata;
    SOCKET  serv_sd;
    struct  sockaddr_in serv, cli;
    int     n, cli_len = sizeof(cli);
    char    str[MAXLINE];
//呼叫 WSAStartup() 註冊 WinSock DLL 的使用
    WSAStartup(0x101, &wsadata);
// serv_sd=socket(AF_INET, SOCK_STREAM, 0);// 開啟 TCP socket
    serv_sd=socket(AF_INET, SOCK_DGRAM, 0);
//指定 socket 的 IP 位址和 port number
    serv.sin_family      = AF_INET;
    serv.sin_addr.s_addr = 0;
// 指定 IPPORT_ECHO 為 echo port
    serv.sin_port        = htons(5678);
//bind(serv_sd, &serv, sizeof(serv)) ;
    bind(serv_sd, (LPSOCKADDR) &serv, sizeof(serv));
    while (1) {
        printf("server: waiting for client\n");

        n=recvfrom(serv_sd, str, MAXLINE, 0, (LPSOCKADDR)&cli, &cli_len);
        str[n]='\0';
        printf("server: client->server: %s\n",str);

        if(strcmp(str,"How are you?\0")==0)
        strcpy(str,"Fine, thank you!");
        else strcpy(str,"What?");

        sendto(serv_sd, str, strlen(str), 0, (LPSOCKADDR)&cli, cli_len);
// 顯示送去client 的字串
        printf("server: server->client: %s\n",str);
    }
//結束 WinSock DLL 的使用
    closesocket(serv_sd);

    WSACleanup();
}
