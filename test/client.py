import socket

clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
clientsocket.connect(('localhost',13002))
b = ""
clientsocket.send("tweet s@g myTweets ")
x= clientsocket.recv(1024)
print x.split()