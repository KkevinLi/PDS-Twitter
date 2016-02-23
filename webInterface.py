from flask import Flask, render_template, request, redirect, url_for, flash, session
import os

app = Flask(__name__)
#restarting flask will automatically reset session because of a new key
#app.secret_key = os.urandom(32)
app.secret_key = "temp"

#still testing additional features
@app.route('/',methods=['GET','POST'])
@app.route('/home',methods=['GET','POST'])
@app.route('/home/<user>',methods = ['GET','POST'])
def home(user=None):
    messageList = []
    if request.method =='POST':
        text = request.form["tweet"]
        writeTweetAll(text)
    if session.get('authenticated') == True:
        messageFile = os.path.realpath('.')+"/database/"+ session['email'] + "FriendTweets.txt"
        if (os.path.isfile(messageFile)):
            with open(messageFile,"r") as foo:
                messageList = foo.readlines()
        else:
            messageFile = os.path.realpath('.')+"/database/"+ session['email'] + "myTweets.txt"
            with open(messageFile,"a+") as foo:
                messageList = foo.readlines()

    return render_template('home.html',messageList=messageList,user=user)

@app.route('/follow',methods = ['GET','POST'])
def friend():
    if session.get('authenticated') != True:
        flash("You must be signed in to follow others")
        return redirect(url_for('login'))
    if request.method=='POST':
        if verifyUser(request.form["friendEmail"]):
            friendRequest(request.form["friendEmail"])
        else:
            flash("Unable to follow " + request.form["friendEmail"] + " User does not exist !")
    return render_template('follow.html')

@app.route('/followers')
def listFollowers():
    if session.get('authenticated') != True:
        flash("You must first sign in")
        return redirect(url_for('login'))
    followingList = []
    followersList = []
    following= os.path.realpath('.')+"/database/"+ session['email'] +"following.txt"
    followers= os.path.realpath('.')+"/database/"+ session['email'] +"followers.txt"
    with open(following,"a+") as foo:
        followingList = foo.readlines()
    with open(followers,"a+") as foo2:
        followersList = foo2.readlines()
    return render_template('listFriends.html',following=followingList,followers=followersList)

@app.route('/tweets', methods = ['GET','POST'])
def displayTweets(friendsTweet=None):
    if session.get('authenticated') != True:
        flash("Only signed in users can view tweets")
        return redirect(url_for('home'))
    messageList = []
    messageFile = os.path.realpath('.')+"/database/"+ session['email'] + "myTweets.txt"
    with open(messageFile,"r") as foo:
        messageList = foo.readlines()
        return render_template('myTweets.html',messageList=messageList)

@app.route('/login', methods = ['GET','POST'])
def login():
    if session.get('authenticated') == True:
        flash("You are already logged in!")
        return redirect(url_for('home'))
    if request.method=='POST':
        if verifyUser(request.form["email"],request.form["password"]):  #If the email was verfied, user will be logged in
            session['authenticated']=True
            session['email']= request.form["email"]
            return redirect(url_for('home'))
        else:
            return render_template('logreg.html', action="Failure",title="Login Page")
    return render_template("logreg.html",registerPage="/register")

@app.route('/register', methods = ['GET','POST'])
def register():
    if session.get('authenticated') == True:
        return redirect(url_for('home'))
    if request.method=='POST':
        if (verifyUser(request.form["email"])):     #If true, the email is in use and we send an error page
            flash("The email you have entered is already in use")
            return render_template('logreg.html', action="Failure",title="Register Page",type = "register")
        else:
            writeToFile(request.form["email"],request.form["password"],request.form["name"])
            session['username']=request.form["name"]
            session['email']= request.form["email"]
            session['authenticated']=True
            return redirect(url_for('home'))
    return render_template("logreg.html",type="register", loginPage="/login")   #by default we have the standard registration page

@app.route("/about")
def about():
    return render_template("AboutPage.html")

@app.route("/logout")
def logout():
    if session.get('authenticated') == True:
        session.clear()
    return redirect(url_for('home'))

def writeToFile(email,password,name):
     filePath = os.path.realpath('.')+"/database/"+email +".txt"
     with open(filePath,"a+") as foo:
        foo.write(email+";"+password+";"+name +"\n")

def friendRequest(friendEmail):
    myFile= os.path.realpath('.')+"/database/"+ session['email'] +"following.txt"
    friendFile= os.path.realpath('.')+"/database/"+ friendEmail +"followers.txt"
    with open(myFile,"a+") as foo:
        if friendEmail+"\n" not in foo:
            foo.write(friendEmail +  "\n")
            flash("Successfully added " + friendEmail)
    with open(friendFile,"a+") as foo2:
        if session['email'] +  "\n" not in foo2:
            foo2.write(session['email'] +  "\n")

def verifyUser(email,password=None):
    filePath = os.path.realpath('.')+"/database/"+email +".txt"
    if password == None:        #Used for the register verification
        return os.path.exists(filePath)
    else:
        try:                    #Check Login verification
            with open(filePath,"r") as f:
                for line in f:
                    userData = line.strip().split(";")
                    if (email == userData[0] and password ==userData[1]):
                        return True
                flash("Invalid password and username combination")
                return False
        except Exception as e:
            flash("Email address does not exist in database")

def writeTweetAll(tweet):
    myTweet = os.path.realpath('.')+ "/database/" + session['email'] + "myTweets.txt"
    friends= os.path.realpath('.')+"/database/"+ session['email'] + "followers.txt"
#For all users who follow you, write into their file the tweet you made
    with open(friends,'a+') as foo:
        numFriends = 0
        for line in foo:
            if(line.strip() != ""):
                numFriends += 1
                with open((os.path.realpath('.')+ "/database/"+line.strip()+"FriendTweets.txt") ,"a+") as textStore:
                    textStore.write(session['email'] + ": " + tweet + "\n")
        session["numberOfFollowers"] = numFriends
    with open(myTweet,'a+') as myFile:
        myFile.write(tweet + "\n")

'''
For later use

@app.route('/tester/<content>')
def test2(content):
    return render_template("header.html",content=content)

@app.route('/tester')
def test():
    return redirect(url_for('test2',content="kevin"))

'''

if __name__ == '__main__':
    app.run(debug=True)
