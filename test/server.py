#Kevin Li kl1983
#Socket Homework 1

#import socket module
from socket import *
serverSocket = socket(AF_INET, SOCK_STREAM)
#Prepare a sever socket
#Fill in start
def test():
    return "You worked!"
port = 80
host = 'localhost'  #get my ipconfig ip to use as server
serverSocket.bind((host,port)) # Create server using ip and port 80
serverSocket.listen(5)

#Fill in end
while True:
    #Establish the connection
    print 'Ready to serve...'
    connectionSocket, addr = serverSocket.accept()  # create connection
    try:
        message = connectionSocket.recv(1024) # get Information from client
        if message == "hello":
            connectionSocket.send(test())

        '''
       f = open(filename[1:])
        outputdata = f.read()
        connectionSocket.send('HTTP/1.1 200 OK\r\n\r\n')
        for i in range(0, len(outputdata)):
            connectionSocket.send(outputdata[i])
        '''
        connectionSocket.close()
    except IOError:
        connectionSocket.send('404 Not Found')
        connectionSocket.close()
        break # if a 404 error is thrown close the connection and server


serverSocket.close()
