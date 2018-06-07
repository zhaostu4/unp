#include "all_head.h"
void str_cli(FILE *fp, int sockfd);
void str_cli1(FILE *fp, int sockfd);
void str_cli2(FILE *fp, int sockfd);
#ifndef MAXLINE
#define MAXLINE 4096
#endif
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

	printf("input: ");
	while (fgets(sendline, MAXLINE, fp) != NULL) {   //从标准输入读数据
		
		writen(sockfd, sendline, strlen(sendline));
		printf("date is writed: ");
		if (readline(sockfd, recvline, MAXLINE) == 0){
			printf("str_cli: server terminated prematurely");
			exit(0);
		}
		
		printf("recive:");
		fputs(recvline, stdout);
	}
}
void str_cli1(FILE *fp, int sockfd)
{
	int			maxfdp1;
	fd_set		rset;
	char		sendline[4096], recvline[4096];

	FD_ZERO(&rset);                		//初始化描述符集
	for ( ; ; ) {                 					 //
		FD_SET(fileno(fp), &rset); 	//fileno把标准文件指针转化为对应描述符
		FD_SET(sockfd, &rset);
		maxfdp1 = max(fileno(fp), sockfd) + 1; //maxdfp1:描述符个数，从0开始，所以要加1
		select(maxfdp1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(sockfd, &rset)) {	/* socket is readable */
			if (readline(sockfd, recvline, MAXLINE) == 0)
				printf("str_cli: server terminated prematurely");
			fputs(recvline, stdout);
		}

		if (FD_ISSET(fileno(fp), &rset)) {  /* input is readable */
			if (fgets(sendline, MAXLINE, fp) == NULL)
				return;		/* all done */
			writen(sockfd, sendline, strlen(sendline));
		}
	}
}
void str_cli2(FILE *fp, int sockfd)
{
	int			maxfdp1, stdineof;
	fd_set		rset;
	char		buf[MAXLINE];
	int		n;

	stdineof = 0;             //标志为0，主循环中就select标准输入的可读性
	FD_ZERO(&rset);
	for ( ; ; ) {
		if (stdineof == 0)    //标志为0，就设置标准输入的可读性
			FD_SET(fileno(fp), &rset);
		FD_SET(sockfd, &rset);//套接字描述符总是设置可读
		maxfdp1 = max(fileno(fp), sockfd) + 1;
		                      //一直阻塞到其中一个可读
		select(maxfdp1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(sockfd, &rset)) {	/* 套接字可读 */
			if ( (n =read(sockfd, buf, MAXLINE)) == 0) {//对缓冲区而不是对文本进行操作
				if (stdineof == 1)
					return;		/* normal termination */
				else{
					printf("str_cli: server terminated prematurely");
					exit(0);
				}
			}
			write(fileno(stdout), buf, n);
		}

		if (FD_ISSET(fileno(fp), &rset)) {  /* 标准输入变得可读 */
			if ( (n = read(fileno(fp), buf, MAXLINE)) == 0) {
				stdineof = 1;
				shutdown(sockfd, SHUT_WR);	/* 发送 FIN */
				FD_CLR(fileno(fp), &rset);
				continue;
			}
			writen(sockfd, buf, n);
		}
	}
}
