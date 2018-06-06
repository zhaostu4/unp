#include	"unp.h"

void my_str_cli(FILE *fp, int sockfd);

int main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr;

	if (argc != 2)
		err_quit("usage: tcpcli <IPaddress>");

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

	my_str_cli(stdin, sockfd);		/* do it all */

	exit(0);
}
void my_str_cli(FILE *fp, int sockfd)
{
	char	sendline[MAXLINE], recvline[MAXLINE];

	printf("我在等服务器给我发消息，-_-!\n");
	while (Fgets(sendline, MAXLINE, fp) != NULL) {   //从标准输入读数据

		Writen(sockfd, sendline, strlen(sendline));

		if (Readline(sockfd, recvline, MAXLINE) == 0)
			err_quit("str_cli: server terminated prematurely");
		
		printf("recive:");
		Fputs(recvline, stdout);
	}
}
