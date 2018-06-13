#include "./inc/all_head.h"
#include "./inc/file.h"
#include	<netinet/tcp.h>		/* for TCP_NODELAY value */

int main(){
    int					          listenfd, connfd;
	pid_t				        childpid;
	socklen_t			     clilen;
	struct sockaddr_in	cliaddr, servaddr;
	void				        sig_chld(int);       //函数指针
    void * optval=NULL;
    socklen_t optlen=sizeof(optval);
    struct_login_user=read_file();
   
    chdir("./");   //改变工作路径
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(listenfd, IPPROTO_TCP, TCP_NODELAY, optval, optlen);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	listen(listenfd, LISTENQ);

	signal(SIGCHLD, sig_chld);
    for ( ; ; ) {
		clilen = sizeof(cliaddr);
		if ( (connfd = accept(listenfd, (SA *) &cliaddr, &clilen)) < 0) {
			if (errno == EINTR)
				continue;		/* back to for() */
			else
				err_sys("accept error");
		}

		if ( (childpid = fork()) == 0) {	/* child process */
			close(listenfd);	/* close listening socket */
			child_deal(connfd);	/* process the request */
			exit(0);
		}
		close(connfd);			/* parent closes connected socket */
	}
    free_login_user(struct_login_user);
    printf("end of the file\n");
    return 0;
}