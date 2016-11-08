#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

#include "HackerGame.h"
#include "message.h"

#define ENGMSG "."
#define BUFLEN 356

using namespace std;

int main(int argc, char *argv[])
{
/*****   Variable Declaration   *****/
	string entry;
	HackerGame CurrentGame;
	int likeness;
	UDP_Cntr_MSG ctrl_msg;
	Game_MSG game_msg;

	// my TCP variables
	int sk, sk2, sk3;	// socket descriptors
	sockaddr_in remote;	// socket address for remote
	sockaddr_in local;	// socket address for us
	char buf[BUFLEN];	// buffer from remote
	char retbuf[BUFLEN];	// buffer to remote
	socklen_t rlen = sizeof(remote);	// length of remote address
	socklen_t len = sizeof(local);	// length of local address
	int moredata = 1;	// keep processing or quit
	int mesglen;		// actual length of message	

	
/*****   Set Up TCP Connection   *****/
	cout << "Creating TCP Socket..." << endl;
	// create the TCP socket
	sk = socket(AF_INET,SOCK_STREAM,0);
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = 0;	

	// bind a name to the TCP socket, letting the OS choose the port number
	bind(sk,(struct sockaddr *)&local,sizeof(local));

	// get the port number of the socket from the OS and print it to command prompt;
	// the port number will be a command-line parameter to the client program
	getsockname(sk,(struct sockaddr *)&local,&len);
	cout << "TCP socket has port " << local.sin_port << "\n";

	// configure the TCP socket (using listen) and accept a connection request from the base
	listen(sk, 1);
	sk2 = accept(sk, (struct sockaddr *)0, (socklen_t *)0);
	close(sk);
	cout << "A new client is connected to the server!" << endl;	

	
/*****   Set Up UDP Connection   *****/	

	
	// create and bind a UDP socket, letting the OS choose the port number
	sk3 = socket(AF_INET,SOCK_DGRAM,0);
	remote.sin_family = AF_INET;
	remote.sin_addr.s_addr = INADDR_ANY;
	remote.sin_port = 0;
	bind(sk3,(struct sockaddr *)&remote,sizeof(remote));

	// get the port number assigned by the OS and print to console
	getsockname(sk3,(struct sockaddr *)&remote,&rlen);
	cout << "UDP socket has port " << remote.sin_port << "\n";
	
	
/*****   Send UDP Port Number Using TCP Connection   *****/	

	
	// put the port number in the control message and send it to the client
	cout << "Sending UDP port number to client using TCP connection..." << endl;
	ctrl_msg.port = remote.sin_port;
	send(sk2, &ctrl_msg, sizeof(ctrl_msg), 0);

	
/*****   Confirm Client UDP Connection  *****/

	//Read from UDP Socket to make sure that client has had a change to set up their UDP Socket
	int recieved = 1;
	while (recieved == 1) {
		mesglen = recvfrom(sk3, buf, BUFLEN, 0, (struct sockaddr *)&remote, &rlen);
		if (mesglen > 0){
			buf[mesglen] = 0;
			printf("received message: \"%s\"\n", buf);
			recieved = 0;
		}
	}

	//display password (optional)
	cout << "Password: " << CurrentGame.GetPassword() << endl;

/*****   Start Game  *****/	
		
	//Start Game Loop
	string display = CurrentGame.GetDisplayString();
	strcpy(game_msg.Display, display.c_str());
	game_msg.AttemptsLeft = CurrentGame.GetAttemptsLeft();
	game_msg.IsGameWon = CurrentGame.IsGameWon();
	send(sk2, &game_msg, sizeof(game_msg), 0);
	cout << "Starting game..." << endl;	

	while(!CurrentGame.IsGameWon() && CurrentGame.GetAttemptsLeft() > 0){
		//Set up message to be sent to client
	
		//send msg to Client
		
		
		//recv msg containing a guess from client
		mesglen = recvfrom(sk3, buf, BUFLEN, 0, (struct sockaddr *)&remote, &rlen);
		if (mesglen > 0){
			buf[mesglen] = 0;	
			strcpy(game_msg.Entry, buf);
			cout << "Entry: " << game_msg.Entry << endl;
		}
		
		//Check the guess using TestEntry()
		likeness = CurrentGame.TestEntry(game_msg.Entry);
		cout << "Likeness: " << likeness << endl;
		game_msg.Likeness = likeness;
		game_msg.AttemptsLeft = CurrentGame.GetAttemptsLeft();
		sendto(sk3, &game_msg, sizeof(game_msg), 0,(struct sockaddr *)&remote, rlen);

	}
	//End Game Loop
	
	
	//handle win conditions
	if(CurrentGame.IsGameWon()){
		
		cout << "Access Granted!" << endl;
		game_msg.IsGameWon = CurrentGame.IsGameWon();
		sendto(sk3, &game_msg, sizeof(game_msg), 0, (struct sockaddr *)&remote, rlen);
	
	}
	//handle lose conditions
	if(CurrentGame.GetAttemptsLeft() == 0){	
		
		cout << "Access Denied!" << endl;
		game_msg.IsGameWon = CurrentGame.IsGameWon();
		sendto(sk3, &game_msg, sizeof(game_msg), 0, (struct sockaddr *)&remote, rlen);

	}
	//Close sockets
	cout << "Closing TCP and UDP sockets..." << endl;
	close(sk2);
	close(sk3);	

	return 0;
}
