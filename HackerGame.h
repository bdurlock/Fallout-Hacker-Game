
#include <iostream>
#include <stdio.h>
#include <vector>
#include <fstream> //ifstream
#include <cstdlib> //rand, srand
#include <string>
#include <algorithm> //random_shuffle
#include <ctime> //time
#include <locale> //toUpper

static const char RandomChars[] = "!@#$%^&*{}[]()<>?|.,:;";

class HackerGame
{
	//Public Member Variables
public:
	char Display[1000];



//Private Member Variables
private:
	int AttemptsLeft;
	bool GameWon;
	std::vector <std::string> PossibleWords; // Vector of displayed possible words
	std::string Password; //Password
	std::string DisplayString; //Gameboard to be displayed


//Public Member Functions
public:

	//Constructor
	HackerGame();

	//Destructor
	~HackerGame();

	//Returns the possible passwords list
	std::vector <std::string> GetPossiblePasswords() { return PossibleWords; }

	//Returns the password for the current game
	std::string GetPassword() { return Password; }
	
	//return string version of display board
	std::string GetDisplayString() {return DisplayString;}


	//Returns how many attempts are left in this game
	int GetAttemptsLeft() { return AttemptsLeft; }

	//Returns false if the game is not won yet, true if it is
	bool IsGameWon() { return GameWon; }

	//Tests the input word against the password
	//if the game is not won, the likeness (0-4) of the word is returned
	//returns -1 if it is called when there are no attempts left
	int TestEntry(char word1[]);

//Private Member Functions
private:

	//Reads all words in from a file, shuffles them, and selects the first 20
	std::vector<std::string> GetPossibleWords();

	//Inserts possible words into the game board display
	std::string BuildDisplayString(std::vector<std::string> Words);

	// Random character generator function.
	char genRandom() { return RandomChars[rand() % 22]; } 


	
};

