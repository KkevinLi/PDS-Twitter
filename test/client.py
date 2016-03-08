import socket

clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
clientsocket.connect(('192.168.1.11',8080))
clientsocket.send('hello')
print clientsocket.recv(1024)
