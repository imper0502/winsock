/* server*/

#include <stdio.h> 
#include <string.h>
#include <winsock.h>

#define MAXLINE 1024

void main() {
	/**�ŧi��******************************************/ 
	WSADATA wsadata;
	SOCKET serv_sd, cli_sd;	
	struct sockaddr_in serv, cli;
	
	int cli_len, n, m;			// cli_len:  
	char str[MAXLINE]; // str:�ǿ�μȦs tmp:��J�μȦs 
	
	/**��l�ư�****************************************/ 
	WSAStartup(0x101, &wsadata);	// ��l��WSA 
	serv_sd = socket(AF_INET, SOCK_STREAM, 0);	//��l��(SOCKET)serv_sd
	//��l��(SOCKET ADDRess)serv 
	serv.sin_family = AF_INET;
	serv.sin_port = htons(4321);
	serv.sin_addr.s_addr = 0;	//I'm the localhost!
	
	/**�u�@��*****************************************/ 
	bind(serv_sd, (LPSOCKADDR)&serv, sizeof(serv));
		
	while(1) {
		listen(serv_sd, 5);	//��ť (SOCKET)serv_sd 
		cli_len = sizeof(cli);
		printf("Server: waiting for client...\n");

		cli_sd = accept(serv_sd, (LPSOCKADDR)&cli, &cli_len); //�s����serv_sd ���A�N�Ocli_sd�A�إ߳q�D! 
		n = recv(cli_sd, str, MAXLINE, 0);	//�����qcli_sd�e��serv_sd���r��str�Fn:str������ 
		str[n] = '\0'; //'\0'�n�ۤv�ɤW 
		printf("Server-recv:  %s\n", str);
		
		/**�r��B�z*****************************************/
		m = strcmp(str, "How are you?");	//��� 
		if(m==0) { //0:�����۲� 
			strcpy(str, "Fine, thank you.");
		}	else {
			strcpy(str, "What's up?");
		}
		
		/**************************************************/
		send(cli_sd, str, strlen(str), 0); //�e�r���cli_sd 
		printf("Server-reply: %s\n", str);
		
	}
	closesocket(cli_sd); //���� 
	closesocket(serv_sd);
	WSACleanup();
}
