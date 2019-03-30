#include "networks.h"
#define SOCKPATH "./udsocket"

int send_fd(int socket, int fd_to_send){
  struct msghdr socket_message;
  struct iovec io_vector[1];
  struct cmsghdr *control_message = NULL;
  char message_buffer[1];
  /* storage space needed for an ancillary element with a paylod of length is CMSG_SPACE(sizeof(length)) */
  char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];
  int available_ancillary_element_buffer_space;

  /* at least one vector of one byte must be sent */
  message_buffer[0] = 'F';
  io_vector[0].iov_base = message_buffer;
  io_vector[0].iov_len = 1;

  /* initialize socket message */
  memset(&socket_message, 0, sizeof(struct msghdr));
  socket_message.msg_iov = io_vector;
  socket_message.msg_iovlen = 1;

  /* provide space for the ancillary data */
  available_ancillary_element_buffer_space = CMSG_SPACE(sizeof(int));
  memset(ancillary_element_buffer, 0, available_ancillary_element_buffer_space);
  socket_message.msg_control = ancillary_element_buffer;
  socket_message.msg_controllen = available_ancillary_element_buffer_space;

  /* initialize a single ancillary data element for fd passing */
  control_message = CMSG_FIRSTHDR(&socket_message);
  control_message->cmsg_level = SOL_SOCKET;
  control_message->cmsg_type = SCM_RIGHTS;
  control_message->cmsg_len = CMSG_LEN(sizeof(int));
  *((int *) CMSG_DATA(control_message)) = fd_to_send;

  return sendmsg(socket, &socket_message, 0);
}

int main(int argc, char* argv[]){
	int cfd, usfd;

	if((cfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("Socket error!"); exit(0);
	}
	struct sockaddr_in servip;
	servip.sin_family = AF_INET;
	servip.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	servip.sin_port = htons(atoi(argv[1]));
	if(connect(cfd, (struct sockaddr*)&servip, sizeof(struct sockaddr_in)) < 0){
		perror("Connect error!"); exit(0);
	}

	if((usfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
		perror("Unix socket error!"); exit(0);
	}
	struct sockaddr_un unixip;
	unixip.sun_family = AF_UNIX;
	strcpy(unixip.sun_path, SOCKPATH);
	if(connect(usfd, (struct sockaddr*)&unixip, sizeof(struct sockaddr_un)) < 0){
		perror("Unix connect error!"); exit(0);
	}
	printf("%d\n", cfd);
	send_fd(usfd, cfd);

	return 0;
}