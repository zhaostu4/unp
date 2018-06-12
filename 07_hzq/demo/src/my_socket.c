#include "../inc/my_socket.h"

void sig_chld(int signo)
{
	pid_t	pid;
	int		stat;                                                                      //0.信号处理函数只运行一次
	while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)  //1.有尚未终止的子进程在运行时不要阻塞
		printf("child %d terminated\n", pid);
	return;
}

void child_deal(int connfd){
    char		sendline[4096], recvline[4096];
    char *str=NULL;
    struct login_user user;
    user.m_name[0]=0;
    user.m_passwd[0]=0;
    user.m_sess[0]=0;
    user.next=NULL;
    //printf("connection from %s\n",sock_ntop((SA *) &cliaddr, len));
    while(1){
        if (readline(connfd, recvline, 4095) == 0){
            printf("str_cli: server terminated prematurely");
            exit(0);
        }
        else{
            printf("recive: %s \n",(char *)recvline);
            if(str=strstr( (const char *)recvline,(const char *)"login?")){
                my_login(connfd,str,&user);
            }
            else if(str=strstr( (const char *)recvline,(const char *)"time?")){
                my_time(connfd,str,&user);
            }
            else if(str=strstr( (const char *)recvline,(const char *)"quit")){
                my_quit(connfd,str,&user);
            }
            else{
                snprintf(sendline, sizeof(sendline), "fomat error\n");
                write(connfd, sendline, strlen(sendline));
            }
        }
    }
}
void my_login(int connfd,char *buf,struct login_user *user){
    char		sendline[4096];
    char name_buf[MAX_M_LEN] = {0};    
    char passwd_buf[MAX_M_LEN]={0}; 
    if(jiexi(name_buf,passwd_buf,buf)){
        //printf("jiexi success\n");
    }else{
        strcpy(sendline,"jiexi failed\n");
        write(connfd, sendline, strlen(sendline));
        return ;
    }
    while(struct_login_user!=NULL){
        if((strcmp(name_buf,struct_login_user->m_name)==0)&&(strcmp(passwd_buf,struct_login_user->m_passwd)==0)){
            strcpy(user->m_name,name_buf);
            strcpy(user->m_passwd,passwd_buf);
            strcpy(user->m_sess,name_buf);
            strcat(user->m_sess,passwd_buf);
            strcat(user->m_sess,"abcdefghijk");
            break;
        }
        struct_login_user=struct_login_user->next;
    }
    //printf("%d:%d,%d",__LINE__,strcmp(name_buf,struct_login_user->m_name),strcmp(passwd_buf,struct_login_user->m_passwd));
    memset(sendline,0,sizeof(sendline));
    if(strlen(user->m_sess)>0){
        printf("%d,session:%s",__LINE__,user->m_sess);
        printf("pipei success\n");
        strcpy(sendline,"sess?");
        strcat(sendline,user->m_sess);
        strcat(sendline,"\n");
    }else{
        strcpy(sendline,"login failed\n");
    }   
    write(connfd, sendline, strlen(sendline));
    //fflush((FILE *)connfd);
}
void my_time(int connfd,char *buf,struct login_user *user){
    time_t				ticks;
    char		sendline[4096];
    buf+=5;
    if(buf[strlen(buf)-1]=='\n')buf[strlen(buf)-1]=0;
    //printf("line:%d\n",__LINE__);
    //printf("sess:%s\n",user->m_sess);
    if(strcmp(buf,user->m_sess)==0&&strlen(user->m_name)>0){
        ticks = time(NULL);
        snprintf(sendline, sizeof(sendline), "%.24s\r\n", ctime(&ticks));
    }else{
        printf("recv:%s\n",buf);
        printf("sess:%s\n",user->m_sess);
        strcpy(sendline,"session error\n");
    }
    write(connfd, sendline, strlen(sendline));
}
void my_quit(int connfd,char *buf,struct login_user *user){
    char		sendline[4096];
    buf+=5;
    if(buf[strlen(buf)-1]=='\n')buf[strlen(buf)-1]=0;
    if(strcmp(buf,user->m_sess)==0&&strlen(user->m_name)>0){
        exit(0);
    }else{
        printf("recv:%s\n",buf);
        printf("sess:%s\n",user->m_sess);
        strcpy(sendline,"session error\n");
    }
    write(connfd, sendline, strlen(sendline));
}
int jiexi( char name_buf[], char passwd_buf[],char *buf){
    int i,j;
    buf+=6;
    for(i=0,j=0;i<MAX_M_LEN;i++,j++){
            if(buf[j]==','||buf[j]==0){
                name_buf[i]=0;
                break;
            }else{
                name_buf[i]=buf[j];
            }
        }

        if(buf[j]!=','){
            printf("name if too long, it must less than %d bytes \n",MAX_M_LEN);
            return 0;
        }else{
            j++;
        }
        for(i=0;i<MAX_M_LEN;i++,j++){
            if(buf[j]==','||buf[j]==0||buf[j]=='\n'||buf[j]=='\r'){
                passwd_buf[i]=0;
                break;
            }else{
                passwd_buf[i]=buf[j];
            }
        }
        passwd_buf[i]=0;
        if(i>=MAX_M_LEN){
            printf("passwd if too long, it must less than %d bytes \n",MAX_M_LEN);
            printf("len:%d,MAX_M_LEN:%d",i,MAX_M_LEN);
            puts(passwd_buf);
            return 0;
        }
        return 1;
}