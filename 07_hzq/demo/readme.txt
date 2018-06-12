服务端程序：
1.采用TCP. 配置文件，端口认证用户名，密码

2.支持如下命令：
REQ:LOGIN?NAME=HZQ,PASSWD=HZQ
RESP:SESS=abcdefg

REQ:TIME?SESS=abcdefg
RESP:TIME

REQ:QUIT?SESS=abcdefg
RESP:RESULT=abcdefg

