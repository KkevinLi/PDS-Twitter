import socket

clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
clientsocket.connect(('localhost',13002))
clientsocket.send("friend hellow kevin i")
x= clientsocket.recv(1024)
if x == "pass":
	print "Hell yea!"
else:
	print x
