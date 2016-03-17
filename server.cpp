#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sstream>
#include <fstream>
#include <stdio.h>       // perror, snprintf
#include <stdlib.h>      // exit
#include <unistd.h>      // close, write
#include <string.h>      // strlen
#include <strings.h>     // bzero
#include <sys/socket.h>  // socket, AF_INET, SOCK_STREAM, bind, listen, accept
#include <netinet/in.h>  // servaddr, INADDR_ANY, htons
using namespace std;

#define	MAXLINE		4096	// max text line length
#define	BUFFSIZE	8192    // buffer size for reads and writes
#define  SA struct sockaddr
#define	LISTENQ		1024	// 2nd argument to listen()
#define PORT_NUM        13002

string getPath(const string& append, string& path){
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL){
		strcat(cwd, ("/database/" + append).c_str());
		path = string(cwd);
		return path;
	}
	else
		perror("getcwd() error");
}
bool filePathExist(const string& path){
	ifstream theFile((path + ".txt").c_str());
	return theFile.good();

}
//Adds myName if does not exist to friends followers and his name to 
//my following list. Check will be done if I exist through login

string friendRequest(const string& friendName, const string& myName){
	string path, checkIfAdded;
	int numberOfFail = 0;
	int numberOfMyFail = 0;
	fstream myfile;
	path = getPath(friendName.c_str(), path);
	if (filePathExist(path)) {
		myfile.open((path + "followers.txt").c_str(), fstream::in | fstream::out | fstream::app);
		myfile.seekg(0, ios::beg);
		while (myfile >> checkIfAdded){
			if (checkIfAdded == myName){
				numberOfFail++;
				break;
			}
		}
		myfile.clear();
		if (!numberOfFail){
			myfile << (myName + "\r\n");
		}	myfile.close();
	}
	else
		return "fail"; // Could not add friend because he does not exist 
	// Same Concept, used to add friend to my followers
	path = getPath(myName.c_str(), path);
	myfile.open((path + "following.txt").c_str(), fstream::in | fstream::out | fstream::app);
	myfile.seekg(0, ios::beg);
	while (myfile >> checkIfAdded){
		if (checkIfAdded == friendName){
			numberOfMyFail++;
			break;
		}
	}
	myfile.clear();
	if (numberOfMyFail < 1)
		myfile << (friendName + "\r\n");
	return (numberOfFail + numberOfMyFail < 2) ? "pass" : "fail";
}

//Used to check if info sent from client matches text files
string loginVerification(const string& username, const string& password){
	string user, pass, path, result = "fail";
	fstream myfile;
	path = getPath(username.c_str(), path);
	if (filePathExist(path)){
		myfile.open((path + ".txt").c_str());
		myfile >> user >> pass;
		myfile.close();
		if (username == user && pass == password) { result = "pass"; }
	}
	return result;
}
// Checks if file name exist and if it doesnt regsiter user
string registerAccountCheck(const string& username, const string& password, const string& name){
	string path, result = "fail";
	fstream myfile;
	path = getPath(username.c_str(), path);
	if (!filePathExist(path)) {
		myfile.open((path + ".txt").c_str(), fstream::out | fstream::app);
		myfile << username + " " + password + " " + name;
		result = "pass";
	}
	return result;
}
void createFile(const string& filename){
	fstream myfile;
	string path;
	path = getPath(filename.c_str(), path);
	myfile.open((path + ".txt").c_str(), fstream::in | fstream::out | fstream::app);

}
void parser(char* buffer, int connfd){
	istringstream iss(buffer);
	string value;
	string request, name, pass, id;
	iss >> request >> name >> pass >> id;

	if (request == "login")
		value = ((loginVerification(name, pass)));
	else if (request == "register")
		value = registerAccountCheck(name, pass, id);
	else if (request == "friend")
		//Pass will be the current users email and name is the person he is adding
		value = friendRequest(name, pass);
	write(connfd, value.c_str(), 4);
	cout << "\n" << value << endl;

}
//C
string loginVerification(const string& username, const string& password){
	string user, pass, path, result = "fail";
	fstream myfile;
	path = getPath(username.c_str(), path);
	if (filePathExist(path)){
		myfile.open((path + ".txt").c_str());
		myfile >> user >> pass;
		myfile.close();
		if (username == user && pass == password) { result = "pass"; }
	}
	return result;
}
string registerAccountCheck(const string& username, const string& password, const string& name){
	string path, result = "fail";
	fstream myfile;
	path = getPath(username.c_str(), path);
	if (!filePathExist(path)) {
		myfile.open((path + ".txt").c_str(), fstream::out | fstream::app);
		myfile << username + " " + password + " " + name;
		result = "pass";
	}
	return result;
}
void createFile(const string& filename){
	fstream myfile;
	string path;
	path = getPath(filename.c_str(), path);
	myfile.open((path + ".txt").c_str(), fstream::in | fstream::out | fstream::app);

}
void parser(char* buffer, int connfd){
	istringstream iss(buffer);
	string value;
	string request,name,pass,id;
	iss >> request >> name >> pass >> id;
	if (request == "login")
		value = ((loginVerification(name, pass)));
			
	else if (request == "register")
		value = registerAccountCheck(name, pass, id);

	else if (request == "friend")
		//Pass will be the current users email and name is the person he is adding
		value = friendRequest(name, pass);
	write(connfd, value.c_str(), 4);
	cout << "\n" << value << endl;
		
 }

int main() {
//	string text;
//	fstream myfile;
//	string x = DBDIR + string("ke@gmasssssss.txt");
//	myfile.open(x.c_str(), fstream::out | fstream::app);
//	myfile.clear();
//	myfile.seekg(0, ios::beg);
	int n;
	string xy;
	xy= friendRequest("ke@gma", "kevin");
	cout << xy << endl;
	int			listenfd, connfd;  // Unix file descriptors
	struct sockaddr_in	servaddr;          // Note C use of struct
	char		buff[MAXLINE];
	time_t		ticks;

	// 1. Create the socket
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Unable to create a socket");
		exit(1);
	}

	// 2. Set up the sockaddr_in

	// zero it.  
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET; // Specify the family
	// use any network card present
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT_NUM);	// daytime server

	// 3. "Bind" that address object to our listening file descriptor
	if (bind(listenfd, (SA *)&servaddr, sizeof(servaddr)) == -1) {
		perror("Unable to bind port");
		exit(2);
	}

	// 4. Tell the system that we are going to use this sockect for
	//    listening and request a queue length
	if (listen(listenfd, LISTENQ) == -1) {
		perror("Unable to listen");
		exit(3);
	}


	for (;;) {
		// 5. Block until someone connects.
		//    We could provide a sockaddr if we wanted to know details of whom
		//    we are talking to.
		//    Last arg is where to put the size of the sockaddr if
		//    we asked for one
		fprintf(stderr, "Ready to connect.\n");
		if ((connfd = accept(listenfd, (SA *)NULL, NULL)) == -1) {
			perror("accept failed");
			exit(4);
		}
		fprintf(stderr, "Connected\n");
		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));

		/*		int len = strlen(buff);
		if (len != write(connfd, buff, strlen(buff))) {
		perror("write to connection failed");
		}
		*/	// We had a connection.  Do whatever our task is.
		cout << buff << endl;

		read(connfd, buff, 500);
		parser(buff, connfd);
		cout << "Confirmation code  " << connfd << endl;
		cout << "Server received:  " << buff << endl;


		// 6. Close the connection with the current client and go back
		//    for another.
		close(connfd);
	}	//receive a message from a client	
/*
		strcpy(buffer, "test");
		fd = write(clientSocket, buffer, strlen(buffer));
		cout << "Confirmation code  " << fd << endl;

		cout << "buffer has:  " << strlen(buffer) << endl;

		cout << "buffer has:  " << sizeof(buffer) << endl;
		*/
	
=======

	/*
	strcpy(buffer, "test");
	fd = write(clientSocket, buffer, strlen(buffer));
	cout << "Confirmation code  " << fd << endl;

	cout << "buffer has:  " << strlen(buffer) << endl;

	cout << "buffer has:  " << sizeof(buffer) << endl;
	*/

>>>>>>> cServer
	return 0;
}