#include "networks.h"
#define SOCKPATH "./udsocket"

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

int main(int argc, char* argv[]){
	unlink(SOCKPATH);
	
	int cfd, usfd, nusfd;
	if((usfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
		perror("Unix socket error!"); exit(0);
	}
	struct sockaddr_un unixip;
	unixip.sun_family = AF_UNIX;
	strcpy(unixip.sun_path, SOCKPATH);

	if(bind(usfd, (struct sockaddr*)&unixip, sizeof(struct sockaddr_un)) < 0){
		perror("Unix bind error!"); exit(0);
	}
	listen(usfd, LISTENQ);

	nusfd = accept(usfd, NULL, NULL);
	cfd = recv_fd(nusfd);
	printf("%d\n", cfd);
	char buff[10];
	read(cfd, buff, sizeof(buff));
	printf("%s\n", buff);

	return 0;
}
