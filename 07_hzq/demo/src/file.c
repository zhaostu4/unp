/*
  功能：本文件定义配置文件读取的接口
  日期：2018-06-11
  作者：hzq
*/
#include "../inc/all_head.h"
#include "../inc/file.h"

struct login_user *struct_login_user=NULL;

struct login_user * read_file(void){
    char buf[512]={0};               //文件读取缓冲区
    char name_buf[MAX_M_LEN] = {0};    
    char passwd_buf[MAX_M_LEN]={0};   
    ssize_t read_size;                  //读取字节数
    int i,j;
    struct login_user *struct_login_user=NULL;
    int fd = open(MY_FILE_PATH,O_RDONLY);
    if(fd == -1)
    {
        perror("open error");
        return NULL;
    }
    while(1){
        memset(buf,0,sizeof(buf));
        read_size=readline(fd, (void *)buf, sizeof(buf));

        if(read_size<=0){                                //读取完毕
            break;
        }
        if(read_size>MAX_M_LEN*2+1){   //配置文件错误
            printf("file:%s error\n", MY_FILE_PATH);
            return NULL;
        }

        for(i=0,j=0;i<MAX_M_LEN;i++,j++){
            if(buf[j]==':'||buf[j]==0){
                name_buf[i]=0;
                break;
            }else{
                name_buf[i]=buf[j];
            }
        }

        if(buf[j]!=':'){
            printf("name if too long, it must less than %d bytes \n",MAX_M_LEN);
            break;
        }else{
            j++;
        }
        for(i=0;i<MAX_M_LEN;i++,j++){
            if(buf[j]==':'||buf[j]==0||buf[j]=='\n'||buf[j]=='\r'){
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
            break;
        }
        struct_login_user=insert_user(struct_login_user,name_buf,passwd_buf);
    }
    if(fd!=-1){
        close(fd);
        fd=-1;
    }
    
    return struct_login_user;
}
struct login_user * insert_user(struct login_user *struct_login_user,char *name, char *passwd){
    struct login_user *head=NULL;
    struct login_user *p1=NULL;
    if(strlen(name)<=0||strlen(passwd)<=0||strlen(name)>MAX_M_LEN||strlen(passwd)>=MAX_M_LEN){
        printf("name length:%d\n",(int)strlen(name));
        printf("passwd length:%d\n",(int)strlen(passwd));
        printf("error name or passwd\n");
    }else{
        p1=( struct login_user *)malloc(sizeof( struct login_user));
        p1->next=NULL;
        strcpy(p1->m_name,name);
        strcpy(p1->m_passwd,passwd);
        memset(p1->m_sess,0,sizeof(p1->m_sess));
        if(struct_login_user==NULL){
            head=p1;
        }else{
            head=struct_login_user;
            while(struct_login_user->next!=NULL){
                struct_login_user=struct_login_user->next;
            }
            struct_login_user->next=p1;
        }
    }
    return head;
}
void free_login_user(struct login_user *struct_login_user){
    struct login_user *p1=NULL;
    while(struct_login_user!=NULL){
        p1=struct_login_user;
        free(p1);
        struct_login_user=struct_login_user->next;
    }
}

