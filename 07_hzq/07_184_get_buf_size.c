#include "all_head.h"
#include	<netinet/tcp.h>		/* for TCP_xxx defines */

union val {
  int				        i_val;
  long			          l_val;
  struct linger		   linger_val;
  struct timeval	 timeval_val;
} val;

int main(int argc, char const *argv[])
{
    int					         fd;
	socklen_t			    len;
	struct sock_opts    *ptr;

    fd = socket(AF_INET6, SOCK_STREAM, 0);
    len=sizeof(val);

    if (getsockopt(fd, SOL_SOCKET, SO_RCVBUF,&val, &len) == -1) {//已获取的选项值放在val指针所指向的地址
				err_ret("getsockopt error");//val是一个结构体
    }else{
        printf("接收缓冲区的大小是:%ld\n",val.l_val);
    }

     if (getsockopt(fd, SOL_SOCKET, SO_SNDBUF,&val, &len) == -1) {//已获取的选项值放在val指针所指向的地址
				err_ret("getsockopt error");//val是一个结构体
    }else{
        printf("发送缓冲区的大小是:%ld\n",val.l_val);
    }

    close(fd);
    return 0;
}



