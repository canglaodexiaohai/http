#pragma once

#include<iostream>
#include<mysql.h>
#include<string>

MYSQL* my_connect();//连接数据库
int my_insert(MYSQL* myfd,const std::string& name,
              const std::string& pass);
int my_select(MYSQL* myfd,const std::string& name,const std::string &pass);
void my_close(MYSQL* myfd);

