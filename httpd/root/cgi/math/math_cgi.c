/*************************************************************************
	> File Name: math_cgi.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月07日 星期二 01时03分50秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
    int len=-1;
    char buf[1024];
    if(getenv("METHOD"))
    {
        if(strcasecmp(getenv("METHOD"),"GET")==0)
        {
            strcpy(buf,getenv("QUERY_STRING"));
        }
        else
        {
            len=atoi(getenv("CONTENT_LENGTH"));
            int i=0;
            for(;i<len;++i)
            {
                read(0,buf+i,1);
            }
            buf[i]='\0';
        }
        //printf("hello CGI\n");
    }    
    int x,y;
    sscanf(buf,"x=%d&y=%d",&x,&y);
    printf("<html>\n");
    printf("<h1>%d + %d = %d </h1>\n",x,y,x+y);
    printf("</html>\n");
    return 0;
}
