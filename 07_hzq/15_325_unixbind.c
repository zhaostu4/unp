#include	"all_head.h"

int main(int argc, char **argv)
{
	int					sockfd;
	socklen_t			len;
	struct sockaddr_un	addr1, addr2;

	if (argc != 2)
		err_quit("usage: unixbind <pathname>");

	sockfd = Socket(AF_LOCAL, SOCK_STREAM, 0);  //AF_LOCAL以前叫AF_UNIX

	unlink(argv[1]);		/* OK if this fails ,为防止路径以存在先将它删除 */

	bzero(&addr1, sizeof(addr1));
	addr1.sun_family = AF_LOCAL;
	strncpy(addr1.sun_path, argv[1], sizeof(addr1.sun_path)-1);//避免路径名过长溢出结构
	Bind(sockfd, (SA *) &addr1, SUN_LEN(&addr1));

	len = sizeof(addr2);
	Getsockname(sockfd, (SA *) &addr2, &len);
	printf("bound name = %s, returned len = %d\n", addr2.sun_path, len);
	
	exit(0);
}
//运行：./unixbind /tmp/moose