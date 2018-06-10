#include	"../07_hzq/all_head.h"
#include    "string.h"


void dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen);

int main(int argc, char **argv)
{
	int					sockfd;				
	struct sockaddr_in	servaddr;
	struct sockaddr_in cliaddr;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);     //首次使用数据报类型

	bzero(&servaddr, sizeof(servaddr));	
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	bind(sockfd, (SA *) &servaddr, sizeof(servaddr));

	dg_echo(sockfd, (SA *) &cliaddr, sizeof(cliaddr));
}
void dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen)
{
	int			n;
	socklen_t	len;
	char		mesg[MAXLINE];

	for ( ; ; ) {
		len = clilen;
		n = recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);

		//printf("got a message\n");
		mesg[n]=0;
		mesg[n+1]=0;
		printf("%s->%d:%s\n", inet_ntoa( ((struct sockaddr_in*)pcliaddr)->sin_addr ),ntohs(((struct sockaddr_in*)pcliaddr)->sin_port),mesg);

		strcpy(mesg,"recived!");
		sendto(sockfd, mesg, sizeof(mesg), 0, pcliaddr, len);
	}
}

