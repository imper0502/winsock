#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024    /* 字串緩衝區長度 */

void main()
{
	SOCKET	serv_sd;        /* socket 描述子 */
  	int   	cli_len, n;
  	char  	str[MAXLINE];

  	struct 	sockaddr_in serv, cli;
  	WSADATA wsadata;
	   	
    WSAStartup(0x101, &wsadata); //呼叫 WSAStartup() 註冊 WinSock DLL 的使用
   	
  	// serv_sd=socket(AF_INET, SOCK_STREAM, 0);// 開啟 TCP socket
    serv_sd=socket(AF_INET, SOCK_DGRAM, 0);
   	//指定 socket 的 IP 位址和 port number
   	serv.sin_family      = AF_INET;
   	serv.sin_addr.s_addr = 0;
   	serv.sin_port        = htons(5678);	// 指定 IPPORT_ECHO 為 echo port

    //bind(serv_sd, &serv, sizeof(serv)) ;
    bind(serv_sd, (LPSOCKADDR) &serv, sizeof(serv));
    	
   	
   	cli_len = sizeof(cli);
   	
   	while (1) {
   		printf("server: waiting for client\n");
       	
        n=recvfrom(serv_sd, str, MAXLINE, 0, (LPSOCKADDR)&cli, &cli_len);
        str[n]='\0';
        printf("server: client->server: %s\n",str);    	// 顯示從 client 傳來的字串		
        
		if(strcmp(str,"How are you?\0")==0) 
			strcpy(str,"Fine, thank you!");
        else strcpy(str,"What?");
        
        sendto(serv_sd, str, strlen(str), 0, (LPSOCKADDR)&cli, cli_len);
        printf("server: server->client: %s\n",str);		// 顯示送去client 的字串	
    }
    
	//結束 WinSock DLL 的使用
   	closesocket(serv_sd);
   	closesocket(cli_sd);
   	WSACleanup();
}
