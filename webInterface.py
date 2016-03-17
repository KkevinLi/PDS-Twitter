from flask import Flask, render_template, request, redirect, url_for, flash, session
import os, datetime,socket

app = Flask(__name__)
#restarting flask will automatically reset session because of a new key
app.secret_key = os.urandom(32)

@app.route('/',methods=['GET','POST'])
@app.route('/<path:user>',methods = ['GET','POST'])
def home(states="good",user=None):
    messageList = []
    myTweet=[]
    if session.get('authenticated') == True:
        if request.method =='POST':
            text = request.form["tweet"]
            writeTweetAll(text)

        messageFile = os.path.realpath('.')+"/database/"+ session['email'] + "FriendTweets.txt"
        if (os.path.isfile(messageFile)):
            with open(messageFile,"r") as foo:
                messageList = foo.readlines()
        myMessageFile = os.path.realpath('.')+"/database/"+ session['email'] + "myTweets.txt"
        with open(myMessageFile,"a+") as foo2:
            myTweet = foo2.readlines()

    return render_template('home.html',messageList=messageList,myTweet=myTweet,user=user,states=states)

@app.route('/follow',methods = ['GET','POST'])
def friend():
    message = "Cannot follow yourself"
    states = "fail"
    if session.get('authenticated') != True:
        flash("You must be signed in to follow others")
        return redirect(url_for('login'))
    if request.method=='POST':
        if(request.form["friendEmail"] != session["email"]):
            x = validate("friend ", request.form["friendEmail"], session["email"])
            if x== "pass":
                 message = ("Successfully added " + request.form["friendEmail"])
                 states = "pass"
            else:
                message = ("Unable to follow " + request.form["friendEmail"] + " User does not exist or you are already following him!")
        else:
            message = "Cannot follow yourself"
        flash(message)
        return render_template('follow.html',states=states)
    return render_template('follow.html')

@app.route('/tweets', methods = ['GET','POST'])
def displayTweets(tweetToLookUp=None):
    if session.get('authenticated') != True:
        flash("Only signed in users can view tweets")
        return redirect(url_for('home'))
    if tweetToLookUp == None:
        tweetToLookUp = session['email']
    messageList = []
    messageFile = os.path.realpath('.')+"/database/"+ tweetToLookUp + "myTweets.txt"
    with open(messageFile,"r") as foo:
        messageList = foo.readlines()
    return render_template('myTweets.html',messageList=messageList,tweetToLookUp=tweetToLookUp)

@app.route('/login', methods = ['GET','POST'])
def login():
    if session.get('authenticated') == True:
        flash("You are already logged in!")
        return redirect(url_for('home'))
    if request.method=='POST':
        x = validate("login ", request.form["email"],request.form["password"])
        if x == "pass":
            session['authenticated']=True
            session['email']= request.form["email"]
            return redirect(url_for('home'))
        else:
            flash("Incorrect Username or Password")
            return render_template('logreg.html', action="Failure",title="Login Page",states="fail")
    return render_template("logreg.html",registerPage="/register")

@app.route('/register', methods = ['GET','POST'])
def register():
    if session.get('authenticated') == True:
        return redirect(url_for('home'))
    if request.method=='POST':
        x = validate("register ", request.form["email"], request.form["password"], request.form["name"])
        if x == "fail":
            flash("The email you have entered is already in use")
            return render_template('logreg.html', action="Failure",title="Register Page",type = "register",states="fail")
        else:
            session['username']=request.form["name"]
            session['email']= request.form["email"]
            session['authenticated']=True
            flash("Welcome "+session['username'] + " You have successfully registered for Twitter 2.0!")
            return redirect(url_for('home'))
    return render_template("logreg.html",type="register", loginPage="/login")   #by default we have the standard registration page

@app.route("/about")
def about():
    return render_template("AboutPage.html")

@app.route("/profile", methods = ['GET','POST'])
def profile(user=None):
     if session.get('authenticated') == True:
        if request.method == "POST":
            if request.form["btn"] == "Look up Tweets":
                try:
                    return displayTweets(request.form["tweetLookUp"].strip())
                except Exception as e:
                    flash(request.form["tweetLookUp"] + " has no Tweets to share!")
            else:
                return listFollowers(request.form["lookup"].strip())
        return listFollowers(session["email"])
     flash("You must sign in before viewing profile")
     return render_template('home.html', states="fail")

@app.route("/logout")
def logout():
    if session.get('authenticated') == True:
        session.clear()
    return redirect(url_for('home'))

@app.route("/delete")
def delete():

    if session.get('authenticated') == True:
        mypath= os.path.realpath('.')+"/database/"
        myFile = os.path.realpath('.')+"/database/"+ session['email'] +"following.txt"
        myFollowers = os.path.realpath('.')+"/database/"+ session['email'] +"followers.txt"
 # For all the people YOU follow remove yourself from THEIR follower's list
        with open(myFile,'a+') as followFile:
            for pplIFollow in followFile:
                friendFile= os.path.realpath('.')+"/database/"+ pplIFollow.strip() +"followers.txt"
                temp = []
                with open(friendFile,'r') as followerFile:
                    for myself in followerFile:
                        if myself.strip() != session['email']:
                            temp.append(myself)
                with open(friendFile,'w') as rewrite:
                    for friends in temp:
                        rewrite.write(friends)
#For all the PEOPLE following YOU, remove yourself from their following list
        with open(myFollowers,'a+') as followersFile:
            for followers in followersFile:
                friendFollowing = os.path.realpath('.')+"/database/"+ followers.strip() +"following.txt"
                temp = []
                with open(friendFollowing,'r') as followingFile:
                    for emails in followingFile:
                        if emails.strip() != session['email']:
                            temp.append(emails)
                with open(friendFollowing,'w') as rewrite:
                    for friends in temp:
                        rewrite.write(friends)
# Removes all files related to the user
    try:
        os.chdir(mypath)
        for s,a,files in os.walk(mypath):
            for breakdown in files:
                if session['email'].strip() in breakdown:
                     print breakdown
                     os.remove(breakdown)
        os.chdir("..")  # go back to previous location
    except Exception as e:
        pass
    return redirect(url_for('logout'))

def validate(request,email,password=" ",name = " "):
    clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    clientsocket.connect(('localhost',13002))
    clientsocket.send(request + email + " " + password + " " + name + " ")
    return clientsocket.recv(1024)

def writeTweetAll(tweet):
    myTweet = os.path.realpath('.')+ "/database/" + session['email'] + "myTweets.txt"
    friends= os.path.realpath('.')+"/database/"+ session['email'] + "followers.txt"
#For all users who follow you, write into their file the tweet you made
    with open(friends,'a+') as foo:
        for line in foo:
            if(line.strip() != ""):
                with open((os.path.realpath('.')+ "/database/"+line.strip()+"FriendTweets.txt") ,"a+") as textStore:
                    textStore.write(session['email'] + ": " + tweet + "\n")
    with open(myTweet,'a+') as myFile:
        myFile.write(session['email'] + ": " + tweet + "\n")

def listFollowers(whoToLookUp):
    if session.get('authenticated') != True:
        flash("You must first sign in")
        return redirect(url_for('login'))
    followingList = []
    followersList = []
    following= os.path.realpath('.')+"/database/"+ whoToLookUp +"following.txt"
    followers= os.path.realpath('.')+"/database/"+ whoToLookUp +"followers.txt"
    with open(following,"a+") as foo:
        followingList = foo.readlines()
    with open(followers,"a+") as foo2:
        followersList = foo2.readlines()
    return render_template('listFriends.html',following=followingList,followers=followersList,whoToLookUp=whoToLookUp)


if __name__ == '__main__':
    app.run(debug=True)
