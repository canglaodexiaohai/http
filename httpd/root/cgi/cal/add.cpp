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
    int x,y,z,count;
    sscanf(buf,"year=%d&month=%d&day=%d&count=%d",&x,&y,&z,&count);
    Date d(x,y,z);
    if(!d.IsInvalid())
      exit(1);
    if(count>0)
      d += (count);
    else 
      d -= (-count);
    printf("<html>\n");
    cout<<"<h1>"<<d<<"</h1>"<<endl;
    printf("</html>");
		return 0;
}
