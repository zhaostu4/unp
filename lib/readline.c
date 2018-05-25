/* include readline */
#include	"unp.h"

static int	read_cnt;
static char	*read_ptr;        //使用静态变量实现跨相继函数调用的状态信息维护，会导致这些函数变得不可重入
static char	read_buf[MAXLINE];//或者说非线程安全了，后续会改进成线程安全的版本

static ssize_t my_read(int fd, char *ptr)   
{

	if (read_cnt <= 0) {
again:
		if ( (read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
			if (errno == EINTR)
				goto again;
			return(-1);
		} else if (read_cnt == 0)
			return(0);
		read_ptr = read_buf;
	}

	read_cnt--;
	*ptr = *read_ptr++;
	return(1);
}

ssize_t readline(int fd, void *vptr, size_t maxlen)
{
	ssize_t	n, rc;
	char	c, *ptr;

	ptr = vptr;
	for (n = 1; n < maxlen; n++) {
		if ( (rc = my_read(fd, &c)) == 1) {  //注意这里使用的是my_read()
			*ptr++ = c;
			if (c == '\n')
				break;	/* newline is stored, like fgets() */
		} else if (rc == 0) {
			*ptr = 0;
			return(n - 1);	/* EOF, n - 1 bytes were read */
		} else
			return(-1);		/* error, errno set by read() */
	}

	*ptr = 0;	/* null terminate like fgets() */
	return(n);
}

ssize_t readlinebuf(void **vptrptr)   //这个新函数能展露内部缓冲区状态
{
	if (read_cnt)
		*vptrptr = read_ptr;
	return(read_cnt);
}
/* end readline */

ssize_t Readline(int fd, void *ptr, size_t maxlen)
{
	ssize_t		n;

	if ( (n = readline(fd, ptr, maxlen)) < 0)
		err_sys("readline error");
	return(n);
}
