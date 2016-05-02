#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024    /* �r��w�İϪ��� */

void main()
{
	SOCKET	serv_sd;        /* socket �y�z�l */
  	int   	cli_len, n;
  	char  	str[MAXLINE];

  	struct 	sockaddr_in serv, cli;
  	WSADATA wsadata;
	   	
    WSAStartup(0x101, &wsadata); //�I�s WSAStartup() ���U WinSock DLL ���ϥ�
   	
  	// serv_sd=socket(AF_INET, SOCK_STREAM, 0);// �}�� TCP socket
    serv_sd=socket(AF_INET, SOCK_DGRAM, 0);
   	//���w socket �� IP ��}�M port number
   	serv.sin_family      = AF_INET;
   	serv.sin_addr.s_addr = 0;
   	serv.sin_port        = htons(5678);	// ���w IPPORT_ECHO �� echo port

    //bind(serv_sd, &serv, sizeof(serv)) ;
    bind(serv_sd, (LPSOCKADDR) &serv, sizeof(serv));
    	
   	
   	cli_len = sizeof(cli);
   	
   	while (1) {
   		printf("server: waiting for client\n");
       	
        n=recvfrom(serv_sd, str, MAXLINE, 0, (LPSOCKADDR)&cli, &cli_len);
        str[n]='\0';
        printf("server: client->server: %s\n",str);    	// ��ܱq client �ǨӪ��r��		
        
		if(strcmp(str,"How are you?\0")==0) 
			strcpy(str,"Fine, thank you!");
        else strcpy(str,"What?");
        
        sendto(serv_sd, str, strlen(str), 0, (LPSOCKADDR)&cli, cli_len);
        printf("server: server->client: %s\n",str);		// ��ܰe�hclient ���r��	
    }
    
	//���� WinSock DLL ���ϥ�
   	closesocket(serv_sd);
   	closesocket(cli_sd);
   	WSACleanup();
}
