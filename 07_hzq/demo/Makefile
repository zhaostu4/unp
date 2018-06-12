
serv: serv.c ./src/error.c ./src/file.c ./src/all_head.c ./src/my_socket.c
		gcc serv.c ./src/error.c ./src/file.c ./src/all_head.c ./src/my_socket.c -o serv
		
cli: cli.c ./src/error.c ./src/file.c ./src/all_head.c 
		gcc cli.c ./src/error.c ./src/file.c ./src/all_head.c -o cli
# serv.o serv.c
# 		gcc serv.c -o serv.o
# ./src/error.o: ./src/error.c
# 		gcc ./src/error.c -o ./src/error.o
# ./src/file.o: ././src/file.c ./src/all_head.o
# 		gcc ./src/file.c -o ./src/file.o
clean:
	-rm  $(TARGET) $(OBJS) #-的意思是忽略删除所带来的错误,后面的删除继续执行
