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
            validate("tweet ", session['email'], text)
        
        userFriendsTweet = (validate("getTweets ",session['email'],"FriendTweets")).replace("<br>"," ")
        for i in userFriendsTweet.split("<end>"):
            messageList.append(i) 
        userTweet = (validate("getTweets ",session['email'],"myTweets")).replace("<br>"," ")
        for i in userTweet.split("<end>"):
            myTweet.append(i)
   
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
        return render_template('follow.html',states=states,action="Follow")
    return render_template('follow.html',action="Follow")

@app.route('/unfollow',methods = ['GET','POST'])
def unfriend():
    message = "Cannot unfollow yourself"
    states = "fail"
    if session.get('authenticated') != True:
        flash("You must be signed in to unfollow others")
        return redirect(url_for('login'))
    if request.method=='POST':
        if(request.form["friendEmail"] != session["email"]):
            validate("unfollow ", request.form["friendEmail"], session["email"])
            message = ("Successfully removed " + request.form["friendEmail"])
            states = "pass"
            flash(message)
        return render_template('follow.html',states=states, action="Unfollow")
    return render_template('follow.html',action="unfollow")
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

@app.route('/tweets', methods = ['GET','POST'])
def displayTweets(tweetToLookUp=None):
    if session.get('authenticated') != True:
        flash("Only signed in users can view tweets")
        return redirect(url_for('home'))
    if tweetToLookUp == None:
        tweetToLookUp = session['email']
    messageList = []
    messages = validate("getTweets ",tweetToLookUp,"myTweets").replace("<br>"," ")
    for i in messages.split("<end>"):
        messageList.append(i)
    return render_template('myTweets.html',messageList=messageList,tweetToLookUp=tweetToLookUp)


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

@app.route("/about")
def about():
    return render_template("AboutPage.html")

@app.route("/logout")
def logout():
    if session.get('authenticated') == True:
        session.clear()
    return redirect(url_for('home'))

@app.route("/delete")
def delete():
    if session.get('authenticated') == True:
        validate("delete ",session['email'])
    return redirect(url_for('logout'))

def validate(request,email,password=" ",name = " "):
    clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    clientsocket.connect(('localhost',13002))
    clientsocket.send(request + email + " " + password + " " + name + " ")
    return clientsocket.recv(4096)

def listFollowers(whoToLookUp):
    if session.get('authenticated') != True:
        flash("You must first sign in")
        return redirect(url_for('login'))
    followingList = []
    followersList = []
    following = validate("getTweets ", whoToLookUp, "following").split()
    
    for i in following:
        followingList.append(i)
   
    followers = validate("getTweets ", whoToLookUp, "followers").split()
    for i in followers:
        followersList.append(i)
   
    return render_template('listFriends.html',following=followingList,followers=followersList,whoToLookUp=whoToLookUp)


if __name__ == '__main__':
    app.run(debug=True)
