/* server*/

#include <stdio.h> 
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024

void main() {
	/**宣告區******************************************/ 
	WSADATA wsadata;
	SOCKET serv_sd, cli_sd;	
	struct sockaddr_in serv, cli;
	
	int cli_len, n, m;			// cli_len:  
	char str[MAXLINE]; // str:傳輸用暫存 tmp:輸入用暫存 
	
	/**初始化區****************************************/ 
	WSAStartup(0x101, &wsadata);	// 初始化WSA 
	serv_sd = socket(AF_INET, SOCK_STREAM, 0);	//初始化(SOCKET)serv_sd
	//初始化(SOCKET ADDRess)serv 
	serv.sin_family = AF_INET;
	serv.sin_port = htons(4321);
	serv.sin_addr.s_addr = 0;	//I'm the localhost!
	
	/**工作區*****************************************/ 
	bind(serv_sd, (LPSOCKADDR)&serv, sizeof(serv));
		
	while(1) {
		listen(serv_sd, 5);	//監聽 (SOCKET)serv_sd 
		cli_len = sizeof(cli);
		printf("Server: waiting for client...\n");

		cli_sd = accept(serv_sd, (LPSOCKADDR)&cli, &cli_len); //連結到serv_sd 的，就是cli_sd，建立通道! 
		n = recv(cli_sd, str, MAXLINE, 0);	//接收從cli_sd送至serv_sd的字串str；n:str的長度 
		str[n] = '\0'; //'\0'要自己補上 
		printf("Server-recv:  %s\n", str);
		
		/**字串處理*****************************************/
		m = strcmp(str, "How are you?");	//比對 
		if(m==0) { //0:完全相符 
			strcpy(str, "Fine, thank you.");
		}	else {
			strcpy(str, "What's up?");
		}
		
		/**************************************************/
		send(cli_sd, str, strlen(str), 0); //送字串到cli_sd 
		printf("Server-reply: %s\n", str);
		
	}
	closesocket(cli_sd); //關閉 
	closesocket(serv_sd);
	WSACleanup();
}
