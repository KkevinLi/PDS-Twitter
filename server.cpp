/*
Kevin Li
PDS Part 2
A Unix C++ simple server passing messages between Python client. Client sends a short request
and the server parses and sends back permission of "pass" and "fail" before client is able
to continue such as login.

*/

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
#include <vector>
using namespace std;

#define	MAXLINE		4096	// max text line length
#define	BUFFSIZE	8192    // buffer size for reads and writes
#define  SA struct sockaddr
#define	LISTENQ		1024	// 2nd argument to listen()
#define PORT_NUM        13002

// Used in all functions to get the path to the txt file when given a username. Returns path to username's file
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

//Simple check to see if a txt file exist
bool filePathExist(const string& path){
	ifstream theFile((path + ".txt").c_str());
	return theFile.good();

}

//Adds myName if does not exist to friends followers and his name to 
//my following list. Check will be done if I exist through login
string writeTweetAll(const string& myName, const string& tweet, time_t* ticks){
	string path, followers, followerPath;
	path = getPath(myName.c_str(), path);
	fstream myfile, friendFile;
	myfile.open((path + "followers.txt").c_str(), fstream::in | fstream::out | fstream::app);
	myfile.seekg(0, ios::beg);
	while (myfile >> followers){
		followerPath = getPath(followers.c_str(), followerPath);
		friendFile.open((followerPath + "FriendTweets.txt").c_str(), fstream::out | fstream::app);
		friendFile << (myName + " " + tweet + " (" + ctime(ticks) + ")<end>");
		friendFile.close();
	}
	myfile.close();
	myfile.open((path + "myTweets.txt").c_str(), fstream::in | fstream::out | fstream::app);
	myfile << (tweet +  " (" + ctime(ticks) + ")<end>");
	return "pass";
}

//Checks if friend exist and if he does follow him
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

//Versatile Function used to get text from file. Used in Profile to get list of following/followers
//Display all tweets from yourself and friendTweet file
string getTweets(const string& user, const string& mineOrFriends){
	string path, buffer,tweets;
	path = getPath(user.c_str(), path);
	fstream myfile;
	myfile.open(((path + mineOrFriends + ".txt").c_str()), fstream::in | fstream::out | fstream::app);
	myfile.seekg(0, ios::beg);
	while (!myfile.eof()){
		getline(myfile, buffer);
		tweets += buffer;
	}
	return tweets;
}

//Similar to delete, rewrite my following file except for person to unfollow
void unfollow(const string& unfollowee, const string& user){
	string path, fpath, ppl_i_follow, followers;
	path = getPath(user.c_str(), path);
	vector<string> temp, temp2;
	fstream myfile,friendFile;

	myfile.open(((path + "following.txt").c_str()), fstream::in);
	while (myfile >> ppl_i_follow){
		if (ppl_i_follow != unfollowee)
			temp.push_back(ppl_i_follow);
	}
	myfile.close();

	myfile.open(((path + "following.txt").c_str()), fstream::out);
	for (int i = 0; i < temp.size(); ++i){
		myfile << (temp[i] + "\r\n");
	}
	myfile.close();
	path = getPath(unfollowee.c_str(), path);
	friendFile.open(((fpath + "followers.txt").c_str()), fstream::in);
	while (friendFile >> followers){
		if (followers != user)
			temp2.push_back(followers);
	}
	friendFile.close();
	friendFile.open(((path + "followers.txt").c_str()), fstream::out);
	for (int i = 0; i < temp2.size(); ++i){
		friendFile << (temp2[i] + "\r\n");
	}
	friendFile.close();
}


// For all my followers remove myself and for all the people I follow remove myself from their txt file
void deleteAccount(const string& user){
	string path,fpath, ppl_i_follow,followers;
	path = getPath(user.c_str(), path);
	string textfiles[5] = { ".txt", "followers.txt", "following.txt", "myTweets.txt", "FriendTweets.txt" };
	vector<string> temp,temp2;
	fstream myfile, friendFile;
	myfile.open(((path + "following.txt").c_str()), fstream::in);
	while (myfile >> ppl_i_follow){
		temp.push_back(ppl_i_follow);
	}
	for (int i = 0; i < temp.size(); ++i){
		fpath = getPath(temp[i].c_str(), fpath);
		friendFile.open(((fpath + "followers.txt").c_str()), fstream::in);
		while (friendFile >> followers){
			if (followers != user)
				temp2.push_back(followers);
		}
		friendFile.close();
		friendFile.open(((fpath + "followers.txt").c_str()), fstream::out);
		for (int i = 0; i < temp2.size(); ++i){
			friendFile << (temp2[i] + "\r\n");
		}
		friendFile.close();
	}
	myfile.close();
	temp.clear();
	temp2.clear();
// For all my FOLLOWERS remove myself from their FOLLOWING list

	myfile.open(((path + "followers.txt").c_str()), fstream::in);
	while (myfile >> followers){
		temp.push_back(followers);
	}
	for (int i = 0; i < temp.size(); ++i){
		fpath = getPath(temp[i].c_str(), fpath);
		friendFile.open(((fpath + "following.txt").c_str()), fstream::in);
		while (friendFile >> ppl_i_follow){
			if (ppl_i_follow != user)
				temp2.push_back(ppl_i_follow);
		}
		friendFile.close();
		friendFile.open(((fpath + "following.txt").c_str()), fstream::out);
		for (int i = 0; i < temp2.size(); ++i){
			friendFile << (temp2[i] + "\r\n");
		}
		friendFile.close();
	}
	myfile.close();

	for (int i = 0; i < 5; ++i){
		remove((path + textfiles[i]).c_str());
	}
}


//From the client's message, parse the command and use the data to execute the command given
void parser(char* buffer, int connfd,time_t * ticks){
	istringstream iss(buffer);
	string value;
	string request, name, pass, id;
	iss >> request >> name >> pass >> id;

	if (request == "login")
		value = loginVerification(name, pass);
	else if (request == "register")
		value = registerAccountCheck(name, pass, id);
	else if (request == "friend")
		value = friendRequest(name, pass);	//Pass will be the current users email and name is the person he is adding
	else if (request == "tweet")
		value = writeTweetAll(name, pass, ticks);	// pass is the tweet message for all of "names" followers
	else if (request == "getTweets"){
		string test = getTweets(name, pass);
		write(connfd, test.c_str(), test.size());
		return;
	}
	else if (request == "unfollow"){
		unfollow(name, pass);  // name is person to unfollow pass is myself
		return;
	}
	else if (request == "delete"){
		deleteAccount(name);
		return;
	}
	write(connfd, value.c_str(), 4);
}

int main() {

	int			listenfd, connfd;  // Unix file descriptors
	struct sockaddr_in	servaddr;          // Note C use of struct
	char		buff[MAXLINE];
	time_t		ticks;
	
	// 1. Create the socket
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Unable to create a socket");
		exit(1);
	}
	
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
	
		fprintf(stderr, "Ready to connect.\n");
		if ((connfd = accept(listenfd, (SA *)NULL, NULL)) == -1) {
			perror("accept failed");
			exit(4);
		}
		fprintf(stderr, "Connected\n");
		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));

		// Only 500 bytes is needed to be read. Messages sent between are either "pass" or "fail"
		read(connfd, buff, 500);	
		parser(buff, connfd, &ticks);
		close(connfd);
	}	
/*
		strcpy(buffer, "test");
		fd = write(clientSocket, buffer, strlen(buffer));
		cout << "Confirmation code  " << fd << endl;

		cout << "buffer has:  " << strlen(buffer) << endl;

		cout << "buffer has:  " << sizeof(buffer) << endl;
		*/
	
	return 0;
}