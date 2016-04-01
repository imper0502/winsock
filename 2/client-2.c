/* client*/

#include <stdio.h> 
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024

void main() {
	/**宣告區******************************************/
	WSADATA wsadata;
	SOCKET sd;
	struct sockaddr_in serv;
	
	int n;
	char str[MAXLINE];
	/**初始化區****************************************/ 
	WSAStartup(0x101, &wsadata);
	
	sd = socket(AF_INET, SOCK_STREAM, 0);
	
	serv.sin_family = AF_INET;
	serv.sin_port = htons(4321);
	serv.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	/**工作區*****************************************/ 
	connect(sd, (LPSOCKADDR)&serv, sizeof(serv));
	
	printf("INPUT SOMETHING >> ");
	gets(str);
	if(strlen(str)==0)strcpy(str, "How are you?");
	
	send(sd, str, strlen(str), 0);
	printf("client-send: %s\n", str);
	
	n=recv(sd, str,MAXLINE,0);	//n = 字串長度 
	str[n] = '\0';
	printf("client-recv: %s\n", str);		

	while(1){
		sd = socket(AF_INET, SOCK_STREAM, 0);
		connect(sd, (LPSOCKADDR)&serv, sizeof(serv));		
	/**************************************************/
		do {
			printf("INPUT SOMETHING >> ");
			gets(str);
		}while(strlen(str)==0);
		if(strcmp(str, "quit")==0||strcmp(str, "exit")==0) break;
	/**************************************************/
		send(sd, str, strlen(str), 0);
		printf("client-send: %s\n", str);
	
		n=recv(sd, str,MAXLINE,0);	//n = 字串長度 
		str[n] = '\0';
		printf("client-recv: %s\n", str);
				
	}
	closesocket(sd);
	WSACleanup();
}
