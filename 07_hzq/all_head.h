/* Use standard echo server; baseline measurements for nonblocking version */
#include    <stdio.h>
//#include <readline/readline.h>
//#include <readline/history.h>
#include	<sys/types.h>	/* basic system data types */
#include	<sys/socket.h>	/* basic socket definitions */
#include	<sys/time.h>	/* timeval{} for select() */
#include    <poll.h>
//#include    <sys/stropts.h>
#include	<time.h>		/* timespec{} for pselect() */
#include	<sys/time.h>	/* includes <time.h> unsafely */
#include	<time.h>		/* old system? */
#include	<netinet/in.h>	/* sockaddr_in{} and other Internet defns */
#include	<arpa/inet.h>	/* inet(3) functions */
#include	<errno.h>
#include	<fcntl.h>		/* for nonblocking */
#include	<netdb.h>
#include	<signal.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/stat.h>	/* for S_xxx file mode constants */
#include	<sys/uio.h>		/* for iovec{} and readv/writev */
#include	<unistd.h>
#include	<sys/wait.h>
#include	<sys/un.h>		/* for Unix domain sockets */
#include	<stdarg.h>		/* ANSI C header file */
#include	<syslog.h>		/* for syslog() */

#define	min(a,b)	((a) < (b) ? (a) : (b))
#define	max(a,b)	((a) > (b) ? (a) : (b))
#define	LISTENQ		1024	/* 2nd argument to listen() */

/* Miscellaneous constants */
#define	MAXLINE		4096	/* max text line length */
#define	BUFFSIZE	8192	/* buffer size for reads and writes */

/* Define some port number that can be used for our examples */
#define	SERV_PORT		 9877			/* TCP and UDP */
#define	SA	struct sockaddr
ssize_t readline(int fd, void *vptr, size_t maxlen);
/* Write "n" bytes to a descriptor. */
ssize_t	writen(int fd, const void *vptr, size_t n);
//void err_quit(const char *fmt, ...);
//void err_ret(const char *fmt, ...);

#ifndef INFTIM
#define INFTIM -1
#endif

#ifndef OPEN_MAX
#define OPEN_MAX 2048
#endif

#include "all_head.c"
#include "error.c"