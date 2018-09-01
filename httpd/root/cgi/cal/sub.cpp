#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include"date.hpp"
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
    /*int x,y;
	  sscanf(buf,"x=%d&y=%d",&x,&y);
		printf("<html>\n");
		printf("<h1>%d + %d = %d </h1>\n",x,y,x+y);
		printf("</html>\n");*/
    int x,y,z,a,b,c;
    sscanf(buf,"year1=%d&month1=%d&day1=%d&year2=%d&month2=%d&day2=%d",&x,&y,&z,&a,&b,&c);
    Date d1(x,y,z);
    if(!d1.IsInvalid())
      exit(1);
    Date d2(a,b,c);
    if(!d2.IsInvalid())
      exit(2);
    int ret=d2-d1;
    printf("<html>\n");
    cout<<"<h1>"<<ret<<"</h1>"<<endl;
    printf("</html>");
		return 0;
}
