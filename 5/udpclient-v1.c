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

   WSAStartup(0x101,(LPWSADATA) &wsadata); // �I�s WSAStartup() ���U WinSock DLL ���ϥ�
  
   sd=socket(AF_INET, SOCK_DGRAM, 0); //�}�Ҥ@�� TCP socket.
   
   //���s�u�@�ǳơA�]�t��g sockaddr_in ���c (serv) �C
   //���e���Gserver �� IP ��}�Aport number �����C
   serv.sin_family       = AF_INET;
   serv.sin_addr.s_addr  = inet_addr("127.0.0.1");
   serv.sin_port         = htons(5678);

   serv_len = sizeof(serv);
   sendto(sd, str, strlen(str)+1, 0, (LPSOCKADDR)&serv, serv_len); //�ǰehow are you��echo server 
   printf("client: client->server: %s\n" ,str); 
   
   n=recvfrom(sd, str, MAXLINE, 0, (LPSOCKADDR)&serv, &serv_len); //��echo server���� 
   str[n]='\0';
   printf("client: server->client: %s\n",str);    
  
   closesocket(sd); //����TCP socket
    
   WSACleanup();  // ���� WinSock DLL ���ϥ�
   
   return 0;
}


