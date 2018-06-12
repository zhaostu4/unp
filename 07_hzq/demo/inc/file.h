/*
  功能：本文件定义配置文件读取的接口
  日期：2018-06-11
  作者：hzq
*/
#ifndef MY_FILE_H_
#define  MY_FILE_H_

#define MAX_M_LEN 20                 //用户名与密码最大长度
# define MY_FILE_PATH "./data/user"    //配置为文件路径
/**
* 登录用户信息结构体
* @remark
* 读取用户信息保存到这个结构体
* @note
*/
struct login_user
{
    char m_name[MAX_M_LEN+1];         //用户名限长20字节
    char m_passwd[MAX_M_LEN+1];      //密码限长20字节
    char m_sess[MAX_M_LEN*2+1];       //最大session长度
    struct login_user * next;
};
extern struct login_user *struct_login_user;

struct login_user * read_file(void);
void free_login_user(struct login_user *struct_login_user);
struct login_user * insert_user(struct login_user *struct_login_user,char *name, char *passwd);
#endif