// Test Cases Run to test locks
/*	
	for (int i = 0; i< 10; ++i){
	thread c(friendRequest,"s@g","ssss@g");	
		c.detach();
		
	thread f(unfollow,"ssss@g","s@g");	
		f.detach();
		thread d(friendRequest,"ssss@g","s@g");	
		d.detach();	
	thread x(friendRequest,"s@g","ssss@g");	
		x.detach();
		
		thread a(loginVerification,"ssss@g","a");	
		a.detach();
		thread q(loginVerification,"s@g","a");	
		q.detach();
		thread b(registerAccountCheck,"kevinli@gmail","a","keviii");	
		b.detach();
		thread r(writeTweetAll,"s@g","Can everybody see this",&ticks);	
		r.detach();
		thread z(writeTweetAll,"s@g","Can everybody see this",&ticks);	
		z.detach();
		thread l(writeTweetAll,"ssss@g","Can everybody see this",&ticks);	
		l.detach();
		thread h(writeTweetAll,"kevinli@gmail","Can everybody see this",&ticks);	
		h.detach();
//	thread rr(getTweets,"kevinli@gmail","Can everybody see this",&ticks);	
	//rr.detach();
	//thread hh(getTweets,"kevinli@gmail","Can everybody see this",&ticks);	
		//hh.detach();

	}
	*/
/*	
	for(int i = 0; i< 2000; ++i){
		try{
		string x = "kevi";
		string y = "a";
		string z = "name";
		thread a(registerAccountCheck,x+to_string(i),y,z);		
		a.detach();
		}
		catch(exception& e){
			cout << "Standard Exception " << e.what() << endl;
		}
	}	

	for(int i = 0; i< 2000; ++i){
		try{
		string x = "kevi";
		string y = "a";
		thread a(loginVerification,x+to_string(i),y);
		a.detach();
		}
		catch(exception& e){
			cout << "Standard Exception " << e.what() << endl;
		}
	}

	for(int i = 0; i< 2000; ++i){
		try{
		string x = "kevi";
		string y = "a";
		thread a(deleteAccount,x+to_string(i));
		a.detach();
		}
		catch(exception& e){
			cout << "At deleteeeeee exception " << e.what() << endl;
		}
	}
	
	*/
	