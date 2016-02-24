# Twitter 2.0

Twitter 2.0 is a remake of Twitter using the Python programmign language and the Flask Framework.
It will be a multi-stage project and this is only the frontend part 1.
Database is a directory used for storing all the users info and tweets.
There are a bunch of files in the Database directory that can be removed for a fresh start. 
Register a new user to begin. Else   Email: s@g   password: "a" is already useable for testing
After Flask is installed, the program can be run using webInterface.py. 
	(Debug Mode is left on intentionally and secret key is not random for easier testing)

# Twitter 2.0 Features

		- Logging and Registering: For each user create a txt with the user's Name, email and password		
		- Logout : All session variables are cleared upon visiting this page.
		- Follow : Signed in users (verified with a session['authenticated'] ) are able to follow other
					users by entering their email.
		- Followers : By going to the '/followers' page, signed in users will be able to see the people they
						are being followed by and who they follow
		- PostTweet : This functionality is implemented on the home page and is visible only to users signed in. 
						All friends will have a copy of this message stored in a recentTweet txt file
		
		- View Tweets: All the tweets the USER has posted will be displayed. Recent tweets will be in the home page
		
				Features to be implemented at a later time:
						- Private Follow Request 
						- Block users
						- Profile page 
						- Connecting to server 
						- Organizing CSS scripts 
						- Implement # functionality
			
# List of Funtions:
		
		- Header: Contains the navbar, bootstrap links, and the flash message
		
		- Follow: Loads a simple form input to follow another user. Used together with 
					the function friend() which checks if the user exists before adding 
					the email to the follow text file.
					
		- Home : Includes a form input for textarea used as a tweet with a maximum 
				length of 100 characters. Utilizes bootstraps columns to display recent messages from people you follow
				For my implementation, if a user has no followers only his own tweets will show. 
				Else the tweets his followers make will be displayed instead.
				
		- writeToFile(email,password,name): The purpose of this function is to store a new users 
											information into a txt file to use for verification.
		- friendRequest(friendEmail) : 	A function that verifies and adds the email you wish to follow			
										into your own follow txt file and adds the user to the friends 
										followers txt file.
		- verifyUser(email, password = None): 	Using two parameters to make this function reusable so that	
												I am able to check if an email for register already exists
												and login is able to check if the entered credentials is correct.
		- writeTweetAll(tweet) : Takes in a string entered in the homepage and writes that to every follower's 
								 text file. The message is also written to the user's tweet file.
		- Profile : This function uses both listFriends and myTweets. You by default see your own profile
					with the names of people you follow and people you are followed by. You have the ability
					to view your followers/following peers profile and tweets (if tweets exist). 
					
		- ListFriends : Using the listFollowers() function, this rendered page will show both followers 
						and those you follow
		- myTweets: HTML page that flushes all the data found in usersTweet
		
		- logreg : Using the jinja template we are able to use the same page for both login and 
					register. Uses: VerifyUser and writeToFile functions.
		
		- Delete : This function removes the user from all his followers, people he follows and then
					every file related to him will be deleted.
		
		- Tweeting : Embedded into the home page and uses the writeTweetAll which creates two new files	
					 one for your own tweets and one which contain tweets from all friends
					 
#Database Files:
	Consists of a series of text files created by functions to keep track of users information
	
		Sample File:
		
		- kevin@gmail.com   : a file named in this manner  x@y will be the users email,name and password
		- kevin@gmail.comfollowing : contain lines of user emails 
		- kevin@gmail.comfollowers : Similar to the following txt file, this one only shows the people you follow
		- kevin@gmail.commyTweets : a text file containing ONLY the users tweets
		- kevin@gmail.comFriendTweets : contains tweets from all your friends, but does not have timing control ATM
		