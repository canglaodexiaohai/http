#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<fcntl.h>
#include<error.h>
#include<ctype.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<sys/sendfile.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#define MAX 1024
#define MAIN_PAGE "index.html"
#define PAGE_404 "root/404.html"
//*******************************
int startup(int port);
int get_line(int sock,char line[],int size);
void echoError(int sock,int status_code);
int exe_cgi(int sock,char* method,char* path,char* query_string);
int echo_resource(int sock,char* path,int size );
void clearHeader(int sock);
//*******************************

static void Usage(const char *argv)
{
    printf("Usage:%s port\n",argv);
}

int startup(int port)
{
    int sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock<0){
        perror("socket");
        exit(2);
    }

    int opt=1;
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    struct sockaddr_in local;
    local.sin_family=AF_INET;
    local.sin_addr.s_addr=htonl(INADDR_ANY);
    local.sin_port=htons(port);
    if(bind(sock,(struct sockaddr*)&local,sizeof(local))<0){
        perror("bind");
        exit(3);
    }
    
    if(listen(sock,5)<0){
        perror("listen");
        exit(4);
    }

    return sock;
}
int get_line(int sock,char line[],int len)
{
    char c='a';
    int i=0;
    while(c != '\n'&& i < len-1)
    {
        ssize_t s = recv(sock, &c, 1, 0);
        if(s>0){
            if(  c == '\r' )
            {
                recv(sock,&c,1,MSG_PEEK);
                if(c=='\n')
                {
                    recv(sock,&c,1,0);
                }
                else
                {
                    c='\n';
                }
            }
            line[i++]=c;               
        }
        else 
        {
            break;             
        }        
    } 
    line[i] = '\0';
    return i;
}
void echo_404(int sock)
{ 
    char line[MAX];
    char* path="root/404.html";
    struct stat st;
    stat(PAGE_404,&st);

    sprintf(line,"HTTP/1.1 404 Not Found\r\n");

    send(sock,line,strlen(line),0);
    char *p=path + strlen(path) - 1;
    while(*p!='.')
    {
        p--;
    }
    if(strcmp(p,".css")==0)
    {
        sprintf(line,"Content-Type: text/css\r\n");   
    }
    else if(strcmp(p,".js")==0)
    {
        sprintf(line,"Content-Type: application/x-javascript\r\n");
    }
    else
    {
        sprintf(line,"Content-Type: text/html;application/x-csi;application/x-jpg\r\n");
    }
    send(sock,line,strlen(line),0);



    sprintf(line,"Content-Length: %d\r\n",st.st_size);
    send(sock,line,strlen(line),0);

    sprintf(line,"\r\n");
    send(sock,line,strlen(line),0);

    int fd=open(PAGE_404,O_RDONLY);
    sendfile(sock,fd,NULL,st.st_size);
    close(fd);
}
void echoError(int sock,int status_code)
{
    switch(status_code)
    {
        case 400:
            break;
        case 404:
            echo_404(sock);
            break;
        case 403:
            break;
        case 500:
            break;
        case 503:
            break;
        default:
            break;
    }
}
int exe_cgi(int sock,char* method,char* path,char* query_string)
{
   //printf("CCCCCCCCCCCCCCCCCCCCCCCCCCCCC\n");
    char line[MAX];
    int content_length=-1;
    char method_env[MAX/16]={0};
    char query_string_env[MAX]={0};
    char content_length_env[MAX]={0};

    if(strcasecmp(method,"GET")==0)
    {
        clearHeader(sock);        
    }
    else
    {
        do{
            get_line(sock,line,sizeof(line));
            if(strncmp(line,"Content-Length: ",16)==0)
            {
                content_length=atoi(line+16);                
               // printf("length=%d\n",content_length);
            }            
        }while(strcmp(line,"\n"));
        if(content_length==-1)
        {
            return 400;
        }
    }
    int input[2];
    int output[2];
    if(pipe(input)<0)
    {
        return 404;
    }
    if(pipe(output)<0)
    {
        close(input[1]);
        close(input[0]);
        return 404;
    }

    pid_t id=fork();
    if(id<0)
    {
        return 500;         
    }
    else if(id==0)
    {
        close(input[1]);
        close(output[0]);

        dup2(input[0],0);
        dup2(output[1],1);
        
        sprintf(method_env,"METHOD=%s",method);
        putenv(method_env);

        if(strcasecmp(method,"GET")==0)
        {
            sprintf(query_string_env,"QUERY_STRING=%s",query_string);
            putenv(query_string_env);
        }        
        else
        {
            sprintf(content_length_env,"CONTENT_LENGTH=%d",content_length);
            putenv(content_length_env);
        }
        execl(path,path,NULL);
        exit(1);      
    }
    else
    {
        close(input[0]);
        close(output[1]);        
        int i=0;
        char c='\0';
        if(strcasecmp(method,"POST")==0)
        {
            for(;i<content_length;++i)
            {
                read(sock,&c,1);
                write(input[1],&c,1);                 
            }
                    
        }

        sprintf(line,"HTTP/1.0 200 OK\r\n");
        send(sock,line,strlen(line),0);

        sprintf(line,"Content-Type: text/html;charset=utf-8\r\n");
        send(sock,line,strlen(line),0);

        sprintf(line,"\r\n");
        send(sock,line,strlen(line),0);

        while(read(output[0],&c,1)>0)
        {
            send(sock,&c,1,0); 
            //printf("%c",c);
        }

        waitpid(id,NULL,0);

        close(input[1]);
        close(output[0]);    
    }
    return 200;
}
int echo_resource(int sock,char* path,int size )
{
    char line[MAX];
    clearHeader(sock);
    int fd=open(path,O_RDONLY);
    //printf("path:%s\n",path);
    if(fd<0)
    {
        return 500;    
    }
    sprintf(line,"HTTP/1.0 200 OK\r\n");
    send(sock,line,strlen(line),0);
    char *p=path + strlen(path) - 1;
    while(*p!='.')
    {
        p--;
    }
    if(strcmp(p,".css")==0)
    {
        sprintf(line,"Content-Type: text/css\r\n");   
    }
    else if(strcmp(p,".js")==0)
    {
        sprintf(line,"Content-Type: application/x-javascript\r\n");
    }
    else
    {
        sprintf(line,"Content-Type: text/html;application/x-csi;application/x-jpg\r\n");
    }
    send(sock,line,strlen(line),0);

    sprintf(line,"Content-Length: %d\r\n",size);
    send(sock,line,strlen(line),0);

    sprintf(line,"\r\n");
    send(sock,line,strlen(line),0);
    sendfile(sock,fd,NULL,size);
    close(fd);
    return 200;
}
void clearHeader(int sock)
{
    char line[MAX];
    do
    {
        get_line(sock,line,sizeof(line));
    }while(strcmp(line,"\n"));
}
void *handlerRequest(void *arg)
{
    printf("get a new client!!!!!!!!!!!!!!!!\n");
    int64_t sock=(int64_t)arg;
    char line[MAX];
    char method[MAX/16];
    char url[MAX];
    int i=0;
    int j=0;
    char path[MAX];
    int status_code=200;
    int cgi=0;
    char *query_string=NULL;
    //method url http_version
    get_line(sock,line,MAX);
   // printf("%s\n",line);           
    while(i<sizeof(method)-1 && j<sizeof(line) && !isspace(line[j]))                
    {
        method[i++]=line[j++];
    }  
    method[i]='\0';    
   // printf("method:%s\n",method);
    i=0;
    while(j<sizeof(line)&&isspace(line[j]))
    {
        j++;       
    }

    while(i<sizeof(url)-1&&j<sizeof(line)&&!isspace(line[j]))
    {      
        url[i]=line[j];
        ++i;
        ++j; 
    }
    url[i]='\0';
    //printf("url:%s",url);

    if(strcasecmp(method,"GET")==0)
    {
        query_string=url;
        while(*query_string)
        {
            if(*query_string=='?')
            {
                *query_string='\0';
                query_string++;
                cgi=1;
                break;                           
            }
            query_string++;            
        }
    }
    else if(strcasecmp(method,"POST")==0)
    {
        cgi=1;  
    }
    else
    {
        clearHeader(sock);
        status_code=400;
        goto end;
    }
    sprintf(path,"root%s",url);
    if(path[strlen(path)-1]=='/')
    {
        strcat(path,MAIN_PAGE);
    }
    struct stat st;
    if(stat(path,&st)<0)
    {
        clearHeader(sock);
        status_code=404;
        goto end;
    }
    else
    {
        if(S_ISDIR(st.st_mode))
        {
            strcat(path,"/");
            strcat(path,MAIN_PAGE);
            stat(path,&st);
        }
        else if((st.st_mode&S_IXUSR)||(st.st_mode&S_IXGRP)||(st.st_mode&S_IXOTH))
        {
            //printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
            cgi=1;
        }
        else
        {}

        printf("method :%s,path :%s,query_string :%s\n",method,path,query_string);
        if(cgi)
        {
            //printf("BBBBBBBBBBBBBBBBBBBBBBBB\n");
            status_code=exe_cgi(sock,method,path,query_string);
        }
        else
        {
            status_code=echo_resource(sock,path,st.st_size);
        }
    }
end:
    if(status_code!=200)
    {
        echoError(sock,status_code);
    }
    close(sock);
}
//./http 8080
int main(int argc,char * argv[])
{
    if(argc!=2) {
        Usage(argv[0]);
        return 1;
    }

    signal(SIGPIPE,SIG_IGN);
    int listen_sock=startup(atoi(argv[1]));

    while(1)
    {
        struct sockaddr_in client;
        socklen_t len=sizeof(client);
        int64_t sock=accept(listen_sock,(struct sockaddr*)&client,&len);
        if(sock<0){
            perror("accept");
            continue;
        }
        pthread_t tid;
        pthread_create(&tid,NULL,handlerRequest,(void *)sock);
        pthread_detach(tid);
    }

    return 0;
}
