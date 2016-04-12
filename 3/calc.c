#include <stdio.h> 
#include <string.h>

#define MAXLINE 81

void main() {
	int i = 0, x = 0, y = 0, ans = 0;
	char op = '@';
	char str[MAXLINE];
	while(1) {
		printf(">> ");
	  fgets(str, MAXLINE, stdin);
	  if(strcmp(str, "quit\n")==0) break;
		for( i=strlen(str); i>=0; i--) {
	  	if(op == '@'){
	  		if(str[i]>='0'&&str[i]<'9') {
	  			y = 10*y + (str[i]-'0');
				}else if(str[i] == '+'||str[i] == '-'||str[i] == '*'||str[i] == '/') {				
				  op = str[i];
				}
			}else {
				if(str[i]>=48&&str[i]<58) {
					x = 10*x + (str[i]-'0');
				}
			}
		}
		switch(op) {
			case '+':
				ans = x+y;
				break;
		  case '-':
		  	ans = x-y;
				break;
			case '*':
				ans = x*y;
				break;
			case '/':
				ans = x/y;
				break;
		  default:
		  	printf("ERROR\n");
		}
		printf("ANS: %d\n", ans);
		x=y=0;
		op='@';
		ans = 0;
	}
}
