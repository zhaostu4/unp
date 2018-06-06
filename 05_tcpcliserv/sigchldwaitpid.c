#include	"unp.h"

void
sig_chld(int signo)
{
	pid_t	pid;
	int		stat;
                                                    //0.信号处理函数只运行一次
	while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)//1.有尚未终止的子进程在运行时不要阻塞
		printf("child %d terminated\n", pid);
	return;
}
