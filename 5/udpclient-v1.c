#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024

int main(int argc, char** argv) {  

   SOCKET        	sd;     
   struct sockaddr_in serv;
   char  		str[1024]="How are you?";
   WSADATA 		wsadata;
   int n, serv_len;

   WSAStartup(0x101,(LPWSADATA) &wsadata); // 呼叫 WSAStartup() 註冊 WinSock DLL 的使用
  
   sd=socket(AF_INET, SOCK_DGRAM, 0); //開啟一個 TCP socket.
   
   //為連線作準備，包含填寫 sockaddr_in 結構 (serv) 。
   //內容有：server 的 IP 位址，port number 等等。
   serv.sin_family       = AF_INET;
   serv.sin_addr.s_addr  = inet_addr("127.0.0.1");
   serv.sin_port         = htons(5678);

   serv_len = sizeof(serv);
   sendto(sd, str, strlen(str)+1, 0, (LPSOCKADDR)&serv, serv_len); //傳送how are you至echo server 
   printf("client: client->server: %s\n" ,str); 
   
   n=recvfrom(sd, str, MAXLINE, 0, (LPSOCKADDR)&serv, &serv_len); //由echo server接收 
   str[n]='\0';
   printf("client: server->client: %s\n",str);    
  
   closesocket(sd); //關閉TCP socket
    
   WSACleanup();  // 結束 WinSock DLL 的使用
   
   return 0;
}


