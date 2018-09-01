#include"comm.h"

MYSQL* my_connect()
{
  MYSQL* myfd=mysql_init(NULL);
  if(NULL == mysql_real_connect(myfd, "127.0.0.1", "root","", "jcy", 3306, NULL, 0))
  {
    std::cout << "connect error" << std::endl;
  }else{
    //std::cout <<"connect successs"<<std::endl;
  }

  return myfd; 
}

int my_insert(MYSQL *myfd, const std::string &name,\
    const std::string &pass)
{
  std::string sql = "INSERT INTO message VALUES (\"";
  sql += name;
  sql += "\",\"";
  sql += pass;
  sql += "\")";

  //std::cout << sql << std::endl;
  return mysql_query(myfd, sql.c_str());

}
int my_select(MYSQL *myfd,const std::string &name,const std::string &pass)
{
  //std::cout<<"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"<<std::endl;
  std::string sql="select * from message where name=\"";
  sql+=name;
  sql+="\"";
  //std::cout<<sql<<std::endl;
  mysql_query(myfd,sql.c_str());
  MYSQL_RES *result=mysql_store_result(myfd);
  int lines=mysql_num_rows(result);
  int cols=mysql_num_fields(result);
  MYSQL_ROW row=mysql_fetch_row(result);
  //std::cout<<"lines="<<lines;
  if(lines!=0)
  {
    if(row[1]!=pass)
    {
      //free(result);
      return 1;//存在，密码不对.
    }
    else 
    {
      //free(result);
      return 2;//存在。密码正确
    }
  }
  //free(result);
  return 0;//不存在
}
void my_close(MYSQL *myfd)
{
  mysql_close(myfd);
}

