#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024

int main(int argc, char** argv) {
    WSADATA wsadata;
    SOCKET  sd;
    struct  sockaddr_in serv;
    int     i, n, serv_len = sizeof(serv);
    char    str[1024] = "How are you?";
// 呼叫 WSAStartup() 註冊 WinSock DLL 的使用
    WSAStartup(0x101,(LPWSADATA) &wsadata);
    sd=socket(AF_INET, SOCK_DGRAM, 0);
// 為連線作準備，包含填寫 sockaddr_in 結構 (serv) 。
// 內容有：server 的 IP 位址，port number 等等。
    serv.sin_family       = AF_INET;
    serv.sin_addr.s_addr  = inet_addr("127.0.0.1");
    serv.sin_port         = htons(5678);
// 傳送how are you至echo server
for(i=0; i<MAXLINE; i++) {
	sendto(sd, str, strlen(str)+1, 0, (LPSOCKADDR)&serv, serv_len);
    printf("client: client->server: %s\n" ,str);
// 由echo server接收
    n=recvfrom(sd, str, MAXLINE, 0, (LPSOCKADDR)&serv, &serv_len);
    str[n]='\0';
    printf("client: server->client: %s\n",str);
}
// 關閉TCP socket
    closesocket(sd);
// 結束 WinSock DLL 的使用
    WSACleanup();
    return 0;
}
