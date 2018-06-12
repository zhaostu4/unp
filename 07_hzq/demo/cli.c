#include "./inc/all_head.h"

void str_cli(FILE *fp, int sockfd);
#ifndef MAXLINE
#define MAXLINE 4096
#endif
char session[256]={0};
int main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr;

	if (argc != 2){
		printf("usage: tcpcli <IPaddress>\n");
		exit(0);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(9877);
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	str_cli(stdin, sockfd);		/* do it all */

	exit(0);
}
void str_cli(FILE *fp, int sockfd)
{
	char	sendline[MAXLINE], recvline[MAXLINE];
    char  *str;
	memset(session,0,sizeof(session));
	while (1) {   //从标准输入读数据
		printf("> ");
        if(fgets(sendline, MAXLINE, fp) == NULL)//从标准输入读取命令
            break;
        else{   //如果是time?或quit命令就加上session
            if(strstr(sendline,"time")!=NULL&&session!=NULL)
            {
                strcpy(sendline,"time?");
                strcat(sendline,session);
                //strcat(sendline,"\n");
            }
            if(strstr(sendline,"quit")){
                strcpy(sendline,"quit?");
                strcat(sendline,session);
                //strcat(sendline,"\n");
            }
        }
        if(strstr(sendline,"\n")==NULL)
            strcat(sendline,"\n");
        //printf("send:%s\n",sendline);
		writen(sockfd, sendline, strlen(sendline));
        printf("> ");

		if (readline(sockfd, recvline, MAXLINE) == 0){
			printf("str_cli: server terminated prematurely\n");
			exit(0);
		}
        if(str=strstr(recvline,"sess?")){    //如果收到服务器发送的session
            str+=5;
            memset(session,0,sizeof(session));
            strcpy(session,str);
            //fputs("login success,session saved\n", stdout);
            //printf("session:%s\n",session);
            continue;
        }
		fputs(recvline, stdout);
	}
}