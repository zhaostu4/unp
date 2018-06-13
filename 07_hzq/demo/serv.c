#include "./inc/all_head.h"
#include "./inc/file.h"
#include	<netinet/tcp.h>		/* for TCP_NODELAY value */
#define	MAXFD	64

extern int	daemon_proc;	/* defined in error.c */

int daemon_init(const char *pname, int facility);

int main(int argc,char **argv){
    int					          listenfd, connfd;
	pid_t				        childpid;
	socklen_t			     clilen;
	struct sockaddr_in	cliaddr, servaddr;
	void				        sig_chld(int);       //函数指针
    void * optval=NULL;
    socklen_t optlen=sizeof(optval);
    struct_login_user=read_file();
   
    //chdir("./");   //改变工作路径
    daemon_init(argv[0],0);
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

int daemon_init(const char *pname, int facility)
{
	int		i;
	pid_t	pid;

	if ( (pid = fork()) < 0)
		return (-1);
	else if (pid)
		_exit(0);			/* parent terminates */

	/* child 1 continues... */

	if (setsid() < 0)			/* become session leader ，创建新的会话 */
		return (-1);

	signal(SIGHUP, SIG_IGN); /* 忽略掉SIGHUP信号  */
	if ( (pid = fork()) < 0)  /* 将来即使打开了一个终端设备，也不会自动获得控制终端 */
		return (-1);
	else if (pid)
		_exit(0);			/* child 1 terminates */

	/* child 2 continues... */

	daemon_proc = 1;			/* for err_XXX() functions */

	chdir("/");				/* change working directory ，改变工作目录 */

	/* close off file descriptors，关闭所有文件描述符  */
	for (i = 0; i < MAXFD; i++)
		close(i);

	/* redirect stdin, stdout, and stderr to /dev/null */
	open("/dev/null", O_RDONLY);
	open("/dev/null", O_RDWR);
	open("/dev/null", O_RDWR);

	openlog(pname, LOG_PID, facility);

	return (0);				/* success */
}