/* include connect_timeo */
#include	"all_head.h"
static void	connect_alarm(int);
int connect_timeo(int sockfd, const SA *saptr, socklen_t salen, int nsec)
{
	Sigfunc	*sigfunc;
	int		n;

	sigfunc = signal(SIGALRM, connect_alarm);//1.建立信号处理函数
	if (alarm(nsec) != 0)                                        //2.设置报警时钟
		err_msg("connect_timeo: alarm was already set");

	if ( (n = connect(sockfd, saptr, salen)) < 0) {
		close(sockfd);
		if (errno == EINTR)                                    //3.检查是否被打断过
			errno = ETIMEDOUT;
	}
	alarm(0);					/* turn off the alarm */
	Signal(SIGALRM, sigfunc);	/* restore previous signal handler，恢复原来的信号处理函数 */

	return(n);
}

static void connect_alarm(int signo)
{
	return;		/* just interrupt the connect() */
}
/* end connect_timeo */

void Connect_timeo(int fd, const SA *sa, socklen_t salen, int sec)
{
	if (connect_timeo(fd, sa, salen, sec) < 0)
		err_sys("connect_timeo error");
}
