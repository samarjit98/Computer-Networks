import os
import sys
import socket
import threading
 
def reader(cfd):
	data = cfd.recv(256)
	print("Received: {}".format(data.decode()))

def writer(cfd):
	data = input("Data: ")
	cfd.sendall(data.encode())

def main():
	sfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	servip = ('localhost', 5001)
	sfd.bind(servip)
	sfd.listen(100)

	while True:
		cfd, _ = sfd.accept()
	
		if(os.fork()==0):
			sfd.close()
			tid1 = threading.Thread(target=reader, args=(cfd,))
			tid2 = threading.Thread(target=writer, args=(cfd,))
			tid1.start()
			tid2.start()
			tid1.join()
			tid2.join()
			cfd.close()
		else:
			cfd.close()

if __name__=="__main__":
	main()