ROOT_PATH=$(shell pwd)
INCLUDE=-I$(ROOT_PATH)/../../../lib/include
LIB=-L$(ROOT_PATH)/../../../lib/lib -lmysqlclient -lssl -lcrypto


.PHONY:all
all:insert_cgi login

insert_cgi:comm.cpp insert_cgi.cpp
	g++ -o $@ $^ $(INCLUDE) $(LIB)
login:login.cpp comm.cpp
	g++ -o $@ $^ $(INCLUDE) $(LIB)
.PHONY:clean
clean:
	rm -f login insert_cgi
