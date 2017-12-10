//实现基于tcp协议的通信模型
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<signal.h>
#include<pthread.h>
#include<ifaddrs.h>
//定义用来表示客户端信息的数据结构
typedef struct clinfo{ 
	int fd;
	char ip[16];
	pthread_t thread;
	struct clinfo *p_next;
}clinfo;

//定义用来存放在线客户端的单向循环链表
typedef struct{
	clinfo head;
}cilink;

//全局变量
cilink lk={0};
int sockfd=0;
char ip[16]={0};

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

//服务器关闭函数
void server_close(int signum){
	printf("正在关闭服务器...\n");
	//关闭客户端fd
	struct clinfo *p_head=&lk.head;
	struct clinfo *p_tmp=p_head->p_next;
	while(1){
		if(p_tmp==p_head)
			break;
		int res=pthread_cancel(p_tmp->thread);
		if(res){
			printf("pthread_cancle:%s\n",strerror(res));
		}
		send(p_tmp->fd,"server close!",14,0);
		res=close(p_tmp->fd);
		if(-1==res){
			perror("close"),exit(-1);
		}
		p_tmp=p_tmp->p_next;
	}
	printf("已关闭所有在线客户端\n");
	//关闭socket，使用close函数
	int res=close(sockfd);
	if(-1==res){
		perror("close"),exit(-1);
	}
	printf("成功关闭服务器\n");
	exit(0);
}

//客户端下线删除链表节点
void delcl(int fd){
	struct clinfo *p_tmp=&lk.head;
	struct clinfo *p_tmp2=p_tmp->p_next;
	while(1){
		if(fd==p_tmp2->fd){
			p_tmp->p_next=p_tmp2->p_next;
			free(p_tmp2);
			break;
		}
		else{
			p_tmp=p_tmp2;
			p_tmp2=p_tmp2->p_next;
		}	
	}
	return;
}

//消息转发,遍历循环链表
int radio(char *ip,int fd,char *msg){
	int cno=0;//客户端数量
	char buf[1024]={0};
	sprintf(buf,"%s[%d]:%s",ip,fd,msg);
	int len=strlen(buf);
	struct clinfo *p_head=&lk.head;
	struct clinfo *p_tmp=p_head->p_next;
	while(1){
		if(p_tmp==p_head)
			break;
		if(p_tmp->fd!=fd)
			send(p_tmp->fd,buf,len,0);
		p_tmp=p_tmp->p_next;
		cno++;
	}
	return cno-1;
}

//文件传输函数
void ftran(int sfd,char *msg){
	int rfd=atoi(msg+1);
	struct clinfo *p_head=&lk.head;
	struct clinfo *p_tmp=p_head->p_next;
	while(1){
		if(p_tmp==p_head){//链表中没发现目标ip
			send(sfd,"目标不在线\n",20,0);
			return;
		}
		if(rfd==(p_tmp->fd)){
			send(sfd,"等待对方回应\n",20,0);
			send(rfd,msg,strlen(msg),0);
			break;
		}
		p_tmp=p_tmp->p_next;
	}
	return;
}

//消息接收线程执行的函数
void *task(void *pv){
	struct clinfo *info=pv;
	int res,pfd=info->fd;
	char ip[20];//客户端ip
	char sinfo[32]={0};
	strcpy(ip,info->ip);
	pthread_t thread=pthread_self();
	info->thread=thread;
	printf("客户端%s[%d]连接成功...\n",ip,pfd);
	radio(ip,pfd,"上线");
	//针对每个客户端不断通信
	while(1){
		char buf[1024]={0};
		res=recv(pfd,buf,sizeof(buf),0);
		printf("线程%lu>>%s[%d]:%s\n",thread,ip,pfd,buf);
		if(-1==res)
			perror("recv"),exit(-1);
		//判断是否请求发送文件
		if(!strncmp(buf,"!",1)){
			//添加发送方信息
			sprintf(sinfo,"@%d",pfd);
			strcat(buf,sinfo);
			ftran(pfd,buf);//文件传输请求
			continue;//继续接收消息
		}
		if(!strncmp(buf,"@",1)){
			//添加接收方信息
			sprintf(sinfo,"@%s",ip);
			strcat(buf,sinfo);
			ftran(pfd,buf);//文件传输回应
			continue;
		}
		//判断客户端发来的是否"bye",或下线发来的\0
		if(!strcmp(buf,"bye")||!*buf){
			radio(ip,pfd,"下线");
			printf("%s[%d]已下线\n",ip,pfd);
			break;//跳出无限循环
		}
		//向其他用户广播消息
		res=radio(ip,pfd,buf);
		printf("向%d个客户端转发了消息\n",res);
	}
	//break执行之后跳到这里,关闭pfd,使用close函数
	res=close(pfd);
	if(-1==res){
		perror("close"),exit(-1);
	}
	//从链表中删除
	delcl(pfd);
	//终止线程
	pthread_exit(NULL);
}

int main(void){
	//初始化在线客户端循环链表
	lk.head.p_next=&lk.head;
	//创建socket,使用socket函数
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==sockfd){
		perror("socket"),exit(-1);
	}
	printf("创建socket成功\n");
	//准备通信地址,使用结构体类型
	getip(ip);
	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(17904);
	addr.sin_addr.s_addr=inet_addr(ip);
	//绑定socket和通信地址,使用bind函数
	int res=bind(sockfd,(struct sockaddr *)&addr,sizeof(addr));
	if(-1==res){
		perror("bind"),exit(-1);
	}
	printf("绑定成功\n");
	//监听,使用listen函数
	res=listen(sockfd,100);
	if(-1==res){
		perror("listen"),exit(-1);
	}
	printf("监听成功\n");
	//通过自定义信号处理方始来关闭服务器
	printf("关闭服务器请按ctrl+c\n");
	if(SIG_ERR==signal(SIGINT,server_close)){
		perror("signal"),exit(-1);
	}
	while(1){
		//响应客户端的连接请求,使用accept函数
		struct sockaddr_in recv_addr;
		socklen_t len=sizeof(recv_addr);
		int fd=accept(sockfd,(struct sockaddr *)&recv_addr,&len);
		if(-1==fd){
			perror("accept"),exit(-1);
		}
		char *ip=inet_ntoa(recv_addr.sin_addr);
		//保存已连入的ip信息
		struct clinfo *info=malloc(sizeof(struct clinfo));
		info->fd=fd;
		strcpy(info->ip,ip);
		info->p_next=lk.head.p_next;
		lk.head.p_next=info;
		//创建线程和客户端通信,pthread_create函数
		pthread_t thread;
		int errno=pthread_create(&thread,NULL,task,info);
		if(errno){
			printf("create:%s\n",strerror(errno));
		}
		//分离线程,由系统释放;
		errno=pthread_detach(thread);
		if(errno){
			printf("detach:%s\n",strerror(errno));
		}
	}
	return 0;
}
