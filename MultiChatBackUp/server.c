#include "networks.h"
#define ADDRESS "./unixsock"

int clients[100];
int curr = 0;
int exiting = 0;
int portno;
char* portstring;
int msqid;

struct mymesg{
	long mtype;
	int processno;
};

void* process(void* argt);
static void sig_backup(int sigint);
//static void sig_startup(int sigint);

int main(int argc, char* argv[]){
	signal(SIGINT, sig_backup);
	//signal(SIGUSR1, sig_startup);
	//kill(getpid(), SIGUSR1);

	portno = atoi(argv[1]);

	int lfd, cfd;
	struct sockaddr_in servip;

	if((lfd = socket(AF_INET, SOCK_STREAM, 0))<0){
		perror("Socket error!"); exit(0);
	}

	bzero(&servip, sizeof(servip));
	servip.sin_family = AF_INET;
	servip.sin_addr.s_addr = inet_addr("127.0.0.1");
	servip.sin_port = htons(atoi(argv[1]));

	if(bind(lfd, (struct sockaddr*)&servip, sizeof(servip))<0){
		perror("Bind error!"); exit(0);
	}
	listen(lfd, LISTENQ);

	char addr[20];
	inet_ntop(AF_INET, &(servip.sin_addr), addr, 20);

	printf("%s\n", addr);

	while(1){
		cfd = accept(lfd, (struct sockaddr*)NULL, NULL);
		
		if(cfd>0){
			clients[curr] = cfd; curr++;
			pthread_t tid;
			pthread_create(&tid, NULL, process, (void*)&cfd);
		}
	}
	return 0;
}

void* process(void* argt){
	int cfd = *((int*)argt);
	printf("%d\n", cfd);

	while(1){
		char buff[20];
		read(cfd, &buff, 20);
		printf("%s\n", buff);

		for(int i=0; i<curr; i++){
			if(cfd!=clients[i])write(clients[i], buff, strlen(buff)+1);
		}
	}
}
/*
static void sig_startup(int sigint){
	struct mymesg buff;
	msgrcv(msqid, &buff, sizeof(struct mymesg), portno, 0);
	kill(buff.processno, SIGUSR2);

	msqid = msgget(1357, 0666 | IPC_CREAT);

	int usfd;
	struct sockaddr_un userv_addr;
  	int userv_len,ucli_len;

  	if((usfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
  		perror("Unix socket error!"); exit(0);
  	}

  	bzero(&userv_addr,sizeof(userv_addr));
  	userv_addr.sun_family = AF_UNIX;
   	strcpy(userv_addr.sun_path, portstring);

	userv_len = sizeof(userv_addr);

	if(connect(usfd,(struct sockaddr *)&userv_addr,userv_len)==-1){
		perror("Connect error!"); exit(0);
	}

	read(usfd, &curr; sizeof(int));

	for(int i=0; i<curr; i++){
		clients[i] = recv_fd(usfd);
	}

	close(usfd);
}
*/
static void sig_backup(int sigint){
	int usfd;
	struct sockaddr_un userv_addr;
  	int userv_len,ucli_len;

  	if((usfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
  		perror("Unix socket error!"); exit(0);
  	}

  	bzero(&userv_addr,sizeof(userv_addr));
  	userv_addr.sun_family = AF_UNIX;
   	strcpy(userv_addr.sun_path, ADDRESS);

	userv_len = sizeof(userv_addr);

	if(connect(usfd,(struct sockaddr *)&userv_addr,userv_len)==-1){
		perror("Connect error!"); exit(0);
	}

	for(int i=0; i<curr; i++){
		char buff[20] = "Backup Time";
		write(clients[i], buff, 20);
	}

	write(usfd, &curr, sizeof(int));

	for(int i=0; i<curr; i++){
		send_fd(usfd, clients[i]);
	}
	exiting = 1;

	close(usfd);
	exit(0);
}