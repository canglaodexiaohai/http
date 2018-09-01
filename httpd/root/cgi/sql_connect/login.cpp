#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<iostream>
#include"comm.h"

int main()
{
  char buff[1024];
  if(getenv("METHOD"))
  {
    if(strcasecmp(getenv("METHOD"), "GET") == 0)
    {
      strcpy(buff, getenv("QUERY_STRING"));
    }
    else
    {
      int i=0;
      int len = atoi(getenv("CONTENT_LENGTH"));
      for(; i < len; i++)
      {
        read(0, buff+i, 1);
      }
      buff[i] = 0;
    }
    //std::cout<<buff<<std::endl;
    //name=wangwu&phone=123456&hobby=123456
    strtok(buff, "=&");
    std::string name = strtok(NULL, "=&");
    strtok(NULL, "=&");
    std::string pass = strtok(NULL, "=&");
    //std::cout<<"name="<<name;

    MYSQL* myfd = my_connect();
    int ret=my_select(myfd, name,pass);
    //std::cout<<"ret="<<ret<<std::endl;
    if(ret==0)
    {
      std::cout<<"<html>"<<std::endl;
      std::cout<<"<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">"<<std::endl;
      std::cout<<"用户不存在,请重新登录"<<std::endl;
      std::cout<<"<meta http-equiv=\"refresh\" content=\"2;url=http://193.112.13.118:8080\">"<<std::endl;       
      std::cout<<"</html>"<<std::endl;
    }
    else if(ret==1)
    {
      std::cout<<"<html>"<<std::endl;
      std::cout<<"<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">"<<std::endl;
      std::cout<<"密码不正确，请重新登录"<<std::endl;
      std::cout<<"<meta http-equiv=\"refresh\" content=\"1;url=http://193.112.13.118:8080\">"<<std::endl;       
      std::cout<<"</html>"<<std::endl;
    }
    else 
    {
      std::cout<<"<html>"<<std::endl;
      std::cout<<"<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">"<<std::endl;
      std::cout<<"登录成功"<<std::endl;
      std::cout<<"<meta http-equiv=\"refresh\" content=\"1;url=http://193.112.13.118:8080/main.html\">"<<std::endl;       
      std::cout<<"</html>"<<std::endl;
    }
    my_close(myfd);
  }
  return 0;
}
