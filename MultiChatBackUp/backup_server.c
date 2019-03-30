#include "networks.h"
#define ADDRESS "./unixsock"

int clients[100];
int curr_cli;

void* process(void* argt){
	int cfd = *((int*)argt);
	printf("%d\n", cfd);

	while(1){
		char buff[20];
		read(cfd, &buff, 20);
		printf("%s\n", buff);

		for(int i=0; i<curr_cli; i++){
			if(cfd!=clients[i])write(clients[i], buff, strlen(buff)+1);
		}
	}
}

int recv_fd(int socket){
  int sent_fd, available_ancillary_element_buffer_space;
  struct msghdr socket_message;
  struct iovec io_vector[1];
  struct cmsghdr *control_message = NULL;
  char message_buffer[1];
  char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];

  /* start clean */
  memset(&socket_message, 0, sizeof(struct msghdr));
  memset(ancillary_element_buffer, 0, CMSG_SPACE(sizeof(int)));

  /* setup a place to fill in message contents */
  io_vector[0].iov_base = message_buffer;
  io_vector[0].iov_len = 1;
  socket_message.msg_iov = io_vector;
  socket_message.msg_iovlen = 1;

  /* provide space for the ancillary data */
  socket_message.msg_control = ancillary_element_buffer;
  socket_message.msg_controllen = CMSG_SPACE(sizeof(int));

  if(recvmsg(socket, &socket_message, MSG_CMSG_CLOEXEC) < 0)
   return -1;

  if(message_buffer[0] != 'F')
  {
   /* this did not originate from the above function */
   return -1;
  }

  if((socket_message.msg_flags & MSG_CTRUNC) == MSG_CTRUNC)
  {
   /* we did not provide enough space for the ancillary element array */
   return -1;
  }

  /* iterate ancillary elements */
   for(control_message = CMSG_FIRSTHDR(&socket_message);
       control_message != NULL;
       control_message = CMSG_NXTHDR(&socket_message, control_message))
  {
   if( (control_message->cmsg_level == SOL_SOCKET) &&
       (control_message->cmsg_type == SCM_RIGHTS) )
   {
    sent_fd = *((int *) CMSG_DATA(control_message));
    return sent_fd;
   }
  }

  return -1;
}


int main(int argc, char** argv){
	unlink(ADDRESS);
	int lfd, cfd;
	struct sockaddr_un servip;
	

	if((lfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
		perror("Socket error!"); exit(0);
	}
	bzero(&servip,sizeof(servip));
  	servip.sun_family = AF_UNIX;
   	strcpy(servip.sun_path, ADDRESS);

	if(bind(lfd, (struct sockaddr*)&servip, sizeof(servip)) < 0){
		perror("Bind error!"); exit(0);
	}
	listen(lfd, LISTENQ);

	while(1){
		cfd = accept(lfd, NULL, NULL);
		
		if(cfd > 0){
			if(fork()==0){
				int curr;
				read(cfd, &curr, sizeof(int));
				curr_cli = curr;

				for(int i=0; i<curr; i++){
					clients[i] = recv_fd(cfd);
				}
				pthread_t tid[curr];
				for(int i=0; i<curr; i++){
					pthread_create(&tid[i], NULL, process, (void*)&clients[i]);
				}
			}
			else{
				close(cfd);
			}
		}
	}
	return 0;
}