#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<signal.h>
#include<pthread.h>
#include <ifaddrs.h>
typedef struct req{
	struct req *p_pre;
	char msg[1024];
	struct req *p_next;	
}req;//待用
void getip(char *);

typedef struct{
	int sfd,rfd,fsize;
	char fname[48];
}ftran;//文件传送信息

//全局变量
int sockfd=0;
char msg[1024]={0};
ftran fti={0};
char ip[16]={0};
char rip[16]={0};

//获取本机ip
void getip(char *ip){
	struct ifaddrs * ifAddrStruct=NULL;  
	struct ifaddrs * ifa=NULL;  
	void * tmpAddrPtr=NULL;  

	getifaddrs(&ifAddrStruct); 

	for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {  
		if(ifa ->ifa_addr->sa_family==AF_INET){
			tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;  
			char addressBuffer[INET_ADDRSTRLEN];  
			inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
			if(strcmp(ifa->ifa_name,"lo"))
				strcpy(ip,addressBuffer);
			//printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);   
		} 
	}  
	if (ifAddrStruct!=NULL)
		freeifaddrs(ifAddrStruct);
	return;
}


//文件发送请求,检查文件存在,可用
void *fsend(void *pv){
	printf("%s[%d]同意接收\n",rip,fti.rfd);
	//创建socket
	int ssock=socket(AF_INET,SOCK_STREAM,0);
	if(-1==ssock){
		perror("socket"),pthread_exit(NULL);
	}
	//准备通信地址
	struct sockaddr_in saddr;
	saddr.sin_family=AF_INET;
	saddr.sin_port=htons(11111);
	saddr.sin_addr.s_addr=inet_addr(rip);
	//连接
	int res=connect(ssock,(struct sockaddr *)&saddr,sizeof(saddr));
	if(-1==res)
		perror("connect"),pthread_exit(NULL);
	//打开文件
	FILE *ff=fopen(fti.fname,"rb");
	if(!ff)
		perror("fopen"),pthread_exit(NULL);
	//开始分段读取发送
	char buf[1024];
	int times=fti.fsize/1024;
	int lasts=fti.fsize%1024;
	char s[4];
	while(times--){
		fread(buf,1024,1,ff);
		while(1){
			printf("%d\n",times);
			send(ssock,buf,1024,0);
			recv(ssock,s,2,0);printf("%s\n",s);
			if(!strcmp(s,"0"))
				break;
		}
	}
	fread(buf,lasts,1,ff);
	while(1){
		send(ssock,buf,lasts,0);
		recv(ssock,s,2,0);printf("%s\n",s);
		if(!strcmp(s,"0"))
			break;
	}
	printf("完成发送\n");
	//关闭资源
	fclose(ff);
	close(ssock);
	pthread_exit(NULL);
}


//文件接收,确认,创建socket
void *frecv(void *pv){
	//创建socket,使用socket函数
	int rsock=socket(AF_INET,SOCK_STREAM,0);
	if(-1==rsock)
		perror("socker"),pthread_exit(NULL);
	//2.准备通信地址,使用服务器的地址
	struct sockaddr_in raddr;
	raddr.sin_family=AF_INET;
	raddr.sin_port=htons(11111);
	raddr.sin_addr.s_addr=inet_addr(ip);
	//绑定socket和通信地址,使用bind函数
	int res=bind(rsock,(struct sockaddr *)&raddr,sizeof(raddr));
	if(-1==res)
		perror("bind"),pthread_exit(NULL);
	//监听,使用listen函数
	res=listen(rsock,1);
	if(-1==res)
		perror("listen"),pthread_exit(NULL);
	//accpet等待同意
	struct sockaddr_in send_addr;
	socklen_t len=sizeof(send_addr);
	int sfd=accept(rsock,(struct sockaddr *)&send_addr,&len);
	if(-1==sfd)
		perror("accept"),pthread_exit(NULL);
	//发送开始
	char *ip=inet_ntoa(send_addr.sin_addr);
	printf("%s开始发送\n",ip);
	FILE *ff=fopen(fti.fname,"wb");
	if(!ff)
		perror("fopen"),pthread_exit(NULL);

	char  buf[1024]={0};
	int times=fti.fsize/1024;
	int lasts=fti.fsize%1024;
	while(times--){
		while(1){
		printf("%d\n",times);
			int hasr=recv(sfd,buf,1024,0);printf("%d\n",hasr);
			if(hasr==1024){
				fwrite(buf,1024,1,ff);
				send(sfd,"0",2,0);
				break;
			}
			else
				send(sfd,"1",2,0);
		}
	}
	while(1){
		int hasr=recv(sfd,buf,lasts,0);printf("%d\n",hasr);
		if(hasr==lasts){
			fwrite(buf,lasts,1,ff);
			send(sfd,"0",2,0);
			break;
		}
		else
			send(sfd,"1",2,0);
	}
	fclose(ff);
	sleep(1);
	close(sfd);
	close(rsock);
	printf("完成接收\n");
	pthread_exit(NULL);
}


//对收到的消息进行解析
void *task(void *pv){
	while(1){
		char buf[1024]={0};
		int res=recv(*(int *)pv,buf,sizeof(buf),0);
		if(-1==res)
			perror("recv"),exit(-1);
		if(!strncmp(buf,"!",1)){
			printf("%s\n",buf);
			char fname[48]={0};
			char tmp[16]={0};
			int i=0,j=0,k=0,l=strlen(buf);
			while(1){//找到最后一个/j
				if(i>l)
					break;
				if('/'==buf[i])
					j=i;
				i++;	
			}
			i=0;
			while(1){//找到第一个#i
				if('#'==buf[i])
					break;
				i++;
			}
			while(1){//找到第一个@k
				if('@'==buf[k])
					break;
				k++;
			}
			int rfd=atoi(buf+1);//接收fd
			strncpy(fname,buf+j+1,i-j-1);//文件名
			strcpy(tmp,buf+i+1);
			int fsize=atoi(tmp);//文件大小
			strcpy(tmp,buf+k+1);//发送方fd
			fti.sfd=atoi(tmp);
			fti.rfd=rfd;
			fti.fsize=fsize;
			strcpy(fti.fname,fname);
			printf("FD%s请求发送%d字节的文件:%s,@%s接收:\n",tmp,fsize,fname,tmp);
			continue;//继续接收消息
		}
		if(!strncmp(buf,"@",1)){
			pthread_t thread;
			int i=0,j=0;
			for(;i<strlen(buf);i++){
				if('@'==buf[i])
					j=i;
			}
			strcpy(rip,buf+j+1);
			pthread_create(&thread,NULL,fsend,(void *)buf);
			pthread_detach(thread);
			continue;
		}
		if(!*buf){//处理服务器发来的\0
			close(sockfd);
			exit(0);
		}
		printf("%s\n",buf);
	}
}


int main(void){
	//创建socket,使用socket函数
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==sockfd){
		perror("socker"),exit(-1);
	}
	//准备通信地址,使用服务器的地址
	getip(ip);
	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(8888);
	addr.sin_addr.s_addr=inet_addr("172.30.3.65");
	//连接socket和通信地址,使用connect函数
	int res=connect(sockfd,(struct sockaddr *)&addr,sizeof(addr));
	if(-1==res){
		perror("connect"),exit(-1);
	}
	//创建线程接收转发
	pthread_t thread;
	int errno=pthread_create(&thread,NULL,task,(void *)&sockfd);
	if(errno){
		printf("create:%s\n",strerror(errno));
	}
	//分离线程
	errno=pthread_detach(thread);
	if(errno){
		printf("detach:%s\n",strerror(errno));
	}
	//进行通信,使用send/recv函数
	while(1){
		char buf[1024]={0};
		fgets(buf,1024,stdin);
		if(strlen(buf)==1023&&buf[1022]!='\n'){
			scanf("%*[^\n]");
			scanf("%*c");
		}
		res=0;
		int len=strlen(buf);
		while(1){//去掉换行符
			if(res>len)
				break;
			else if('\n'==buf[res]){
				buf[res]='\0';
				break;
			}
			else
				res++;
		}
		if(!strncmp(buf,"!",1)){
			char *p_str=buf,*p_end=buf;
			strcpy(msg,buf);
			while(1){//找到第一个/
				if('/'==p_str[0])
					break;
				p_str++;
			}
			strcpy(fti.fname,p_str);
			fti.sfd=sockfd;
			fti.rfd=atoi(buf+1);
			//准备要发送的文件
			FILE *fd=fopen(fti.fname,"r");
			if(!fd){
				perror("fopen");
				continue;
			}
			fseek(fd,0,SEEK_END);
			int fsize=ftell(fd);
			fti.fsize=fsize;
			fclose(fd);
			char csize[12];
			sprintf(csize,"#%d",fsize);
			strcat(buf,csize);
			//发送文件传送请求
			send(sockfd,buf,strlen(buf),0);
			continue;
		}
		if(!strncmp(buf,"@",1)){
			pthread_t thread;
			pthread_create(&thread,NULL,frecv,(void *)&fti);
			pthread_detach(thread);
			//同意文件传送请求
			send(sockfd,buf,strlen(buf),0);
			continue;
		}
		res=send(sockfd,buf,strlen(buf),0);
		if(-1==res){
			perror("send"),exit(-1);
		}
		//判断 当客户端发送bye时 则下线
		if(!strcmp(msg,"bye")){
			break;
		}
	}
	//关闭socket,使用close函数
	res=close(sockfd);
	if(-1==res){
		perror("close"),exit(-1);
	}
	printf("关闭了socket\n");
	return 0;
}


