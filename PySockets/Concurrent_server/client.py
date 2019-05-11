import os
import sys
import socket
import threading

def reader(cfd):
	while True:
		data = cfd.recv(256)
		print("Received: {}".format(data.decode()))

def writer(cfd):
	while True:
		data = input("Data: ")
		cfd.sendall(data.encode())

def main():
	sfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	servip = ('localhost', 5001)
	sfd.connect(servip)

	tid1 = threading.Thread(target=reader, args=(sfd,))
	tid2 = threading.Thread(target=writer, args=(sfd,))
	tid1.start()
	tid2.start()
	tid1.join()
	tid2.join()

	sfd.close()

if __name__=="__main__":
	main()