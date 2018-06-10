#include	"../07_hzq/all_head.h"
#include    "/usr/include/netinet/sctp.h"
//#include    "string.h"
int main(int argc, char **argv)
{
	int sock_fd,msg_flags;
	char readbuf[BUFFSIZE];
	struct sockaddr_in servaddr, cliaddr;
	struct sctp_sndrcvinfo sri;
	struct sctp_event_subscribe evnts;
	int stream_increment=1;
	socklen_t len;
	size_t rd_sz;

	if (argc == 2)                                         //需要注意几点：
		stream_increment = atoi(argv[1]);    /*1.默认情况下服务器响应所用的流号是在其接收消息的流号上加1
                                                                    该参数决定是否增长外来流号*/
                                                                   /* 创建一个SCTP一到多流式套接字 */
        sock_fd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
	bzero(&servaddr, sizeof(servaddr));      /* 网际套接字清零 */
	servaddr.sin_family = AF_INET;           /* 2.需要使用通配地址：将在任何关联中使用所有可用的本地地址 */
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	bind(sock_fd, (SA *) &servaddr, sizeof(servaddr));
	
	bzero(&evnts, sizeof(evnts));                /* 3.修改套接字通知预定 */
	evnts.sctp_data_io_event = 1;               /* 虽然默认开启，但是显式开启好一点 */
	setsockopt(sock_fd, IPPROTO_SCTP, SCTP_EVENTS,
		   &evnts, sizeof(evnts));

	listen(sock_fd, LISTENQ);                  /* 4.开启外来关联 */
	for ( ; ; ) {
		len = sizeof(struct sockaddr_in);     /* 5.服务器将阻塞在等待来自任何一个远程对端的消息 */
		rd_sz = sctp_recvmsg(sock_fd, readbuf, sizeof(readbuf),
			     (SA *)&cliaddr, &len,
			     &sri,&msg_flags);
		if(stream_increment) {                    /* 检查流号 */
			sri.sinfo_stream++;                      /* 6.sctp_get_no_strms()：获取最大流号 */
			if(sri.sinfo_stream >= sctp_get_no_strms(sock_fd,(SA *)&cliaddr, len)) 
				sri.sinfo_stream = 0;
		}
		sctp_sendmsg(sock_fd, readbuf, rd_sz, 
			     (SA *)&cliaddr, len,
			     sri.sinfo_ppid,                        /* 7.指定将随数据块传递的净荷协议标识ID(来自sir结构) */
			     sri.sinfo_flags,
			     sri.sinfo_stream,                    /* 8.timetolive=0,标识无限长生命周期 */
			     0, 0);                                      /* 9.context=0,表禁止上下文 */
	}
}
