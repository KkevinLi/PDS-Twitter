import socket
import thread

clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
clientsocket.connect(('172.16.20.73',13002))
b = ""
clientsocket.send("login s@g a")
x= clientsocket.recv(1024)
print x.split()