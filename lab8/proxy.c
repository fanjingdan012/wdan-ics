/*
 * proxy.c - Web proxy for Lab 8
 * 
 * My student ID: 09302010035
 * My name: Wu Yanhong
 */
#include <stdio.h>
#include "csapp.h"
#include "proxy.h"
#define   LOG_FILE      "proxy.log"
#define   CACHENUM		200							//Cache block number.
#define	  CACHELEN		102400						//One cache block's size.
int listenfd, *connfd, port, clientlen;
fd_set read_set,ready_set;
static sem_t mutex;
team_struct team = {
	/* Full name */
	"Wu Yanhong",
	/* Student ID */
	"093010035"
};
//The struct used to act as cache.
typedef struct{
	char time[MAXLINE];
	char uri[MAXLINE];
	char entity[CACHELEN];
}cache_struct;
static cache_struct cache[CACHENUM];
static sem_t cachesem[CACHENUM];
/*============================================================
 * function declarations
 *============================================================*/

int  find_target_address(char * uri,
			 char * target_address,
			 char * path,
			 int  * port);
void  format_log_entry(char * logstring,
		       int sock,
		       char * uri,
		       int size);
void save_log(char* log){
	FILE* f=fopen(LOG_FILE,"a+");
	fwrite(log,1,strlen(log),f);
	fflush(f);
	fclose(f);
	return;	
}
void doit(int fd){
	int clientfd,port;
	char buf[MAXLINE],method[MAXLINE],uri[MAXLINE],version[MAXLINE];
	char target[MAXLINE],path[MAXLINE];
	char message[MAXLINE];
	char buf2[MAXLINE],buf3[MAXLINE];
	char log[MAXLINE];
	rio_t rio,rio2;
	Rio_readinitb(&rio,fd);
	Rio_readlineb(&rio,buf,MAXLINE);
	sscanf(buf,"%s %s %s",method,uri,version);
	find_target_address((char*)&uri,(char*)&target,(char*)&path,(int*)&port);
	port=80;
	//Make the title of request head.
	strcpy(message,method);
	strcat(message," ");
	if ((strlen(path)==0)||(path[0]!='/')) strcat(message,"/");
	strcat(message,path);
	strcat(message," ");
	version[strlen(version)-1]='0';			//Change HTTP/1.1 by HTTP/1.0
	strcat(version,"\r\n");
	strcat(message,version);
	clientfd= Open_clientfd(target,port);
	Rio_readinitb(&rio2,clientfd);
	int tsize;
	int flag1=0,count=0;
	char* pt;
	//Send the title of request head.
	Rio_writen(clientfd,message,strlen(message));
	//Read the request head and send it.
	while ((tsize=Rio_readlineb(&rio,buf2,MAXLINE))>2){		
		//prevent the connection still connected aftered the browser stoped.
		char* tt=strstr(buf2,"Keep-Alive");
		if (tt==NULL)
		Rio_writen(clientfd,buf2,strlen(buf2));
		//If the request has entity,get its length.
		pt=strstr(buf2,"Content-Length:");
		if (pt!=NULL){
			pt+=15;	
			while (*pt==' ') pt++;
				flag1=atoi(pt);
			}
	}
		//Send the request head received.
		if (Rio_writen(clientfd,buf2,strlen(buf2))==-1){
			Close(clientfd); 
			Close(fd);
			return;
		}
		ssize_t size;
		//If the request has entity,get and send it.
		while (count<flag1){
			Rio_readnb(&rio,buf2,flag1);
			if (Rio_writen(clientfd,buf2,flag1)==-1){	
				Close(clientfd); 
				Close(fd);
				return;
			}
			count+=flag1;
		}
		int clen=-1;
		char update[MAXLINE];
		char lens[MAXLINE];		
		//Get the response head.
		while ((size=Rio_readlineb(&rio2,buf3,MAXLINE))!=0) {
			if (Rio_writen(fd,buf3,size)==-1){
				Close(clientfd); 
				Close(fd);
				return;
			}
			//Get the response entity's length.
			if (strstr(buf3,"Content-Length")!=NULL){	
				memcpy(&lens[0],&buf3[16],strlen(buf3)-15);
				lens[strlen(lens)-2]='\0';
				clen=atoi(&lens[0]); 
			}
			//Get the request page's last modified time.
			if (strstr(buf3,"Last-Modified")!=NULL){
				memcpy(&update[0],&buf3[15],strlen(buf3)-14);
				update[strlen(update)-2]='\0';
			}
			if (strcmp(buf3,"\r\n")==0) break;
		}
		int dest=0;
		int j;
		for (j=0;j<strlen(uri);j++){
			dest+=(int)uri[j];
		}
		//Hash the uri to get the cache index.
		dest=dest%CACHENUM;
		int ifcache=0;
		//Check if there is a cache hit.
		P(&cachesem[dest]);
			if ((strcmp(cache[dest].uri,uri)==0)&&(strcmp(cache[dest].time,update)==0)) ifcache=1;
		V(&cachesem[dest]);
		if (ifcache){
			//Cache hit.
			P(&cachesem[dest]);
			//printf("Cache hit.\n");
			if (Rio_writen(fd,cache[dest].entity,clen)==-1){
				Close(clientfd); 
				Close(fd);
				return;
			}	
			V(&cachesem[dest]);
		}else{
			//Cache miss.
			ssize_t tot_read=0;
			int ifsave=0;
			char threadcache[CACHELEN];int here=0;
			if (clen<CACHELEN) ifsave=1;
			//Get the response entity.
			while ((size=Rio_readnb(&rio2,buf3,MAXLINE))!=0) {
				if (Rio_writen(fd,buf3,size)==-1){
					Close(clientfd); 
					Close(fd);
					return;
				}
				//Save the response entity if its length is no larger than CACHELEN.
				if (ifsave){
					memcpy(&threadcache[here],&buf3[0],size);	
					here+=size;
				}
				tot_read+=size;
			}
			if (ifsave){
				//Save cache.
				P(&cachesem[dest]);
				memcpy(&cache[dest].uri[0],&uri[0],strlen(uri));
				cache[dest].uri[strlen(uri)]='\0';
				memcpy(&cache[dest].time[0],&update[0],strlen(update));
				cache[dest].time[strlen(update)]='\0';
				memcpy(&cache[dest].entity[0],&threadcache[0],tot_read);
				V(&cachesem[dest]);
			}
			if (tot_read>0){
				//Log request.
				P(&mutex);
				format_log_entry(log,fd,target,tot_read);
				save_log(log);
				V(&mutex);
			}
		}
		//Close connection.
		Close(fd);
		Close(clientfd);
	return;
}
int command(){
	//Change the port number.
	int now;
	if (scanf("%d",&now)==-1){
		printf("Input error.\n");
		return -1;
	}
	FD_CLR(listenfd,&read_set);
	if ((listenfd=Open_listenfd(now))==-1){
		printf("Please change another port.\n");
		return -1;
	}
	port=now;
	FD_SET(listenfd,&read_set);
	return 0;	
}
void *thread(void *vargp){
	//Multi-threads.
	int confd=*((int*)vargp);
	Pthread_detach(pthread_self());
	Free(vargp);
	doit(confd);
	return NULL;
}
/* main function for the proxy program */
int main(int argc, char *argv[])
{    
    /*****************************************
     * fill in proxy code here ;o)           
     ******************************************/
    struct sockaddr_in clientaddr;
    if (argc !=2 ){
    	fprintf(stderr,"usage: %s <port>\n", argv[0]);
    	exit(1);
    }
    signal(SIGPIPE, SIG_IGN);
    //The rwlock of log file.
   	Sem_init(&mutex,0,1);
   	int j;
   	//The rwlock of cache.
   	for (j=0;j<CACHENUM;j++)
   		Sem_init(&cachesem[j],0,1);
    port = atoi(argv[1]);
    listenfd=Open_listenfd(port);
   	FD_ZERO(&read_set);
   	FD_SET(STDIN_FILENO,&read_set);
   	FD_SET(listenfd,&read_set);
    while(1){
    	ready_set=read_set;
    	clientlen=sizeof(clientaddr);
 		//Check whether the file descripter is the net connection or not.
 		Select(listenfd+1,&ready_set,NULL,NULL,NULL);
 		if (FD_ISSET(STDIN_FILENO,&ready_set)){
 			//Change port.
 			command();
 		}
 		if (FD_ISSET(listenfd,&ready_set)){
    		connfd=Malloc(sizeof(int));
    		*connfd=Accept(listenfd,(SA*)&clientaddr, &clientlen);
    		pthread_t tid;
    		//Create a new thread.
    		Pthread_create(&tid,NULL,thread,connfd);
    	}
    }
    return 0;
}


/*============================================================
 * url parser:
 *    find_target_address()
 *        Given a url, copy the target web server address to
 *        target_address and the following path to path.
 *        target_address and path have to be allocated before they 
 *        are passed in and should be long enough (use MAXLINE to be 
 *        safe)
 *
 *        Return the port number. 0 is returned if there is
 *        any error in parsing the url.
 *
 *============================================================*/

/*find_target_address - find the host name from the uri */
int  find_target_address(char * uri, char * target_address, char * path,
                         int  * port)

{
  //  printf("uri: %s\n",uri);  

	if (strncasecmp(uri, "http://", 7) == 0) {
		char * hostbegin, * hostend, *pathbegin;
		int    len;
	       
		/* find the target address */
		hostbegin = uri+7;
		hostend = strpbrk(hostbegin, " :/\r\n");
		if (hostend == NULL){
		  hostend = hostbegin + strlen(hostbegin);
		}
	
		len = hostend - hostbegin;

		strncpy(target_address, hostbegin, len);
		target_address[len] = '\0';

		/* find the port number */
		if (*hostend == ':')   *port = atoi(hostend+1);

		/* find the path */

		pathbegin = strchr(hostbegin, '/');
	
		if (pathbegin == NULL) {
	  	path[0] = '\0';
	  
		}else {
	  		pathbegin++;	
	  		strcpy(path, pathbegin);
		}
		return 0;
    }
    target_address[0] = '\0';
    return -1;
}


/*============================================================
 * log utility
 *    format_log_entry
 *       Copy the formatted log entry to logstring
 *============================================================*/

void format_log_entry(char * logstring, int sock, char * uri, int size)
{
    time_t  now;
    char    buffer[MAXLINE];
    struct  sockaddr_in addr;
    unsigned  long  host;
    unsigned  char a, b, c, d;
    int len = sizeof(addr);

    now = time(NULL);
    strftime(buffer, MAXLINE, "%a %d %b %Y %H:%M:%S %Z", localtime(&now));

    if (getpeername(sock, (struct sockaddr *) &addr,(socklen_t *)&len)) {
		unix_error("Can't get peer name");
    }

    host = ntohl(addr.sin_addr.s_addr);
    a = host >> 24;
    b = (host >> 16) & 0xff;
    c = (host >> 8) & 0xff;
    d = host & 0xff;

    sprintf(logstring, "%s: %d.%d.%d.%d %s\n", buffer, a,b,c,d, uri);
}
