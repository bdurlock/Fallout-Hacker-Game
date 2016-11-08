all:server client
server: server.cpp HackerGame.h HackerGame.cpp message.h
	g++ -std=c++11 server.cpp HackerGame.cpp -o server
client: client.cpp message.h
	g++ client.cpp -std=c++11 -o client

