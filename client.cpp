#include <string.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <netdb.h>
#include <string>
#include <algorithm>
#include "message.h"
#include <vector>

#define MSG "UDP port has been created for the client"
#define BUFLEN 356

using namespace std;

// paddr: print the IPF address in a standard decimal dotted format
void paddr(unsigned char *a){
	printf("%d.%d.%d.%d\n", a[0], a[1], a[2], a[3]);
}


int main(int argc, char *argv[])
{
/*****   Variable Declaration   *****/	
	char server_host[20];
	int tcp_port;
 	hostent *hp;
 	UDP_Cntr_MSG ctrl_msg;
 	Game_MSG game_msg;

	// My variables
	int sk, sk2;		// socket descriptor
	sockaddr_in remote;	// socket address for remote side
	sockaddr_in send;
	socklen_t rlen = sizeof(remote);
	char buf[BUFLEN];	// buffer for repsonse from remote
	int mesglen;	// actual length of message 
	
 	
/*****   Parse Command Line Arguments (remote host, remote port)   *****/
	if ( (argc != 5))
	{
		cout << "Usage: " << argv[0];
		cout << " -h <remote-host> -p <remote tcp_port>" << endl;
		return -1;
	}

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-h") == 0)
			strcpy (server_host, argv[++i]);
		
		if (strcmp(argv[i], "-p") == 0)
			tcp_port = atoi(argv[++i]);
	}
	
	cout <<"Client is running...\n";

	cout  << "remote host: " << server_host <<", remote TCP port: "<< tcp_port <<endl;
	

/*****   Set Up TCP Connection   *****/
	// create TCP socket
	sk = socket(AF_INET,SOCK_STREAM,0);
	remote.sin_family = AF_INET;
	
	// get IP address of server  via DNS
	hp = gethostbyname(server_host);
	memcpy(&remote.sin_addr,hp->h_addr,hp->h_length);
	remote.sin_port = tcp_port;

	// connect to the server program
	if(connect(sk, (struct sockaddr *)&remote, sizeof(remote)) < 0) {
		cout << "connection error!\n";
		close(sk);
		exit(1);
	}
	else
		cout << "Connected to Server!" << endl;


/*****   Read from TCP Connection   *****/
	
	// read the control message from the TCP socket and echo its contents
	cout << "Reading TCP socket for UDP set up info..." << endl;
	recv(sk, &ctrl_msg, sizeof(UDP_Cntr_MSG), 0);
	cout << "UDP port on Server: "<< ctrl_msg.port << endl;

	
/*****   Set Up UDP Connection   *****/	

	// create a UDP socket
	sk2 = socket(AF_INET,SOCK_DGRAM,0);

	// configure a sockaddr_in data structure for sending to the server; note that the IP address field (sin_addr)
	// can be obtained from information returned from the sockaddr data structure returned by the earlier accept() call
	cout << "Server IP: ";
	for ( int i=0; hp->h_addr_list[i] != 0; i++)
		paddr((unsigned char*) hp->h_addr_list[i]);
	remote.sin_port = ctrl_msg.port;
	

	
/*****   Confirm UDP Connection and Start Game   *****/
	//set up msg to let server know that UDP port has been created for the client
	
	
	//send msg to server
	if (sendto(sk2,MSG,strlen(MSG),0,(struct sockaddr *)&remote, sizeof(remote)) < 0){
		perror("sendto failed");
	}	
	
/*****   Start Game   *****/

	//recv first game msg
	cout << "Receiving first game message...\n" << endl;


	//print game display and attempts left to console
	recv(sk, &game_msg, sizeof(game_msg), 0);
	cout << game_msg.Display << endl;
	cout << "Attempts left: " << game_msg.AttemptsLeft << endl;

	//start game loop
	while (!game_msg.IsGameWon && game_msg.AttemptsLeft > 0){

		//Collect guess from user
		cout << "Entry: ";
		string entry;
		cin >> entry;

		//Load guess into game_msg and send to server
		strcpy(game_msg.Entry, entry.c_str());
		sendto(sk2,game_msg.Entry,strlen(game_msg.Entry),0,(struct sockaddr *)&remote, sizeof(remote));

		//recv game msg from server after it has checked the guess
		recvfrom(sk2, &game_msg, sizeof(game_msg), 0, (struct sockaddr *)&remote, &rlen);

		//check winning conditions
		if(game_msg.Likeness < 4){
			cout << "Likeness: " << game_msg.Likeness << endl;
			cout << "Attempts left: " << game_msg.AttemptsLeft << endl;

			//if game is not over, print the likeness of the previous guess
			// and the number of attempts left to console
		}
		else{
			game_msg.IsGameWon = true;
		}
	}
	//Handle end Conditions
	if(game_msg.IsGameWon)
		cout << "Access Granted!" << endl;

	//Close sockets
	cout << "Closing TCP and UDP sockets..." << endl;
	close(sk);
	close(sk2);
	
	return 0;
}
	
