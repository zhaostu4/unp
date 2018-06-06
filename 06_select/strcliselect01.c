#include	"unp.h"

void str_cli(FILE *fp, int sockfd)
{
	int			maxfdp1;
	fd_set		rset;
	char		sendline[MAXLINE], recvline[MAXLINE];

	FD_ZERO(&rset);                //初始化描述符集
	for ( ; ; ) {//这是死循环啊，为什么会退出？？
		FD_SET(fileno(fp), &rset); //fileno把标准文件指针转化为对应描述符
		FD_SET(sockfd, &rset);
		maxfdp1 = max(fileno(fp), sockfd) + 1; //maxdfp1:描述符个数，从0开始，所以要加1
		Select(maxfdp1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(sockfd, &rset)) {	/* socket is readable */
			if (Readline(sockfd, recvline, MAXLINE) == 0)
				err_quit("str_cli: server terminated prematurely");
			Fputs(recvline, stdout);
		}

		if (FD_ISSET(fileno(fp), &rset)) {  /* input is readable */
			if (Fgets(sendline, MAXLINE, fp) == NULL)
				return;		/* all done */
			Writen(sockfd, sendline, strlen(sendline));
		}
	}
}
