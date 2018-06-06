#include	"all_head.h"

int
main(int argc, char **argv)
{
	int					i, maxi, maxfd, listenfd, connfd, sockfd;
	int					nready, client[FD_SETSIZE];
	ssize_t				n;
	fd_set				rset, allset;
	char				buf[MAXLINE];
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);		//1.创建一个监听套接字

	bzero(&servaddr, sizeof(servaddr));				//2.设置地址：协议、IP、端口
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(9877);
													//3.绑定监听套接字
	bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	listen(listenfd, LISTENQ);						//4.监听监听套接字

	maxfd = listenfd;			/* initialize */
	maxi = -1;					/* client[] 数组索引 */
	for (i = 0; i < FD_SETSIZE; i++)
		client[i] = -1;			/* -1 indicates available entry */
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);
/* end fig01 */

/* include fig02 */
	for ( ; ; ) {
		rset = allset;		//5.select等待新的连接，或数据、FIN、RST
		nready = select(maxfd+1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(listenfd, &rset)) {	/* new client connection */
			printf("a new clilet connection\n");
			clilen = sizeof(cliaddr);
			connfd = accept(listenfd, (SA *) &cliaddr, &clilen);

#ifdef	NOTDEF
			printf("new client: %s, port %d\n",
			Inet_ntop(AF_INET, &cliaddr.sin_addr, 4, NULL),
			ntohs(cliaddr.sin_port));
#endif

			for (i = 0; i < FD_SETSIZE; i++)	//加入新的连接到数组
				if (client[i] < 0) {
					client[i] = connfd;			/* save descriptor */
					break;
				}
			if (i == FD_SETSIZE){				//连接的客户太多就退出
				printf("too many clients");
				exit(0);
			}

			FD_SET(connfd, &allset);	/* add new descriptor to set */
			if (connfd > maxfd)
				maxfd = connfd;			/* for select */
			if (i > maxi)
				maxi = i;				/* max index in client[] array */

			if (--nready <= 0)			//没有更多的可读描述符了
				continue;				/* no more readable descriptors */
		}

		for (i = 0; i <= maxi; i++) {	/* check all clients for data */
			if ( (sockfd = client[i]) < 0)
				continue;
			if (FD_ISSET(sockfd, &rset)) {//如果这个套接字字可读
				if ( (n = read(sockfd, buf, MAXLINE)) == 0) {//但是收到的是RES或FIN
						/*4connection closed by client */
					close(sockfd);		  //就关闭这个连接
					printf("a connection closed\n");
					FD_CLR(sockfd, &allset);
					client[i] = -1;
				} else{
					printf("recive:%s\n",buf);
					writen(sockfd, buf, n);
				}

				if (--nready <= 0)
					break;				/* 如果没有更多的可读描述符了 */
			}
		}
	}
}
/* end fig02 */
