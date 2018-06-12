#ifndef MY_SOCKET_H_
#define MY_SOCKET_H_

#include "./all_head.h"


void sig_chld(int signo);
void child_deal(int connfd);
void my_login(int connfd,char *buf,struct login_user *user);
void my_time(int connfd,char *buf,struct login_user *user);
void my_quit(int connfd,char *buf,struct login_user *user);
int jiexi( char name_buf[], char passwd_buf[],char *buf);



#endif