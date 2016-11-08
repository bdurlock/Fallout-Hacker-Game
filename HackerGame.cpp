#include "HackerGame.h"
#include <cstring>

using namespace std;

const int NumberOfWordsToDisplay = 12; //Controls number of possible passwords
									   //MUST BE EVEN!!!
const int WordLength = 4; //Length of possible passwords
						  //Most represent what is used in text file
const string WordListFileName = "WordList.txt"; //File that possible passwords are loaded from



HackerGame::HackerGame()
{
	PossibleWords = GetPossibleWords();
	Password = PossibleWords.at(rand() % PossibleWords.size());
	DisplayString = BuildDisplayString(PossibleWords);
	AttemptsLeft = 4;
	GameWon = false;
}


HackerGame::~HackerGame()
{
}


int HackerGame::TestEntry(char word1[])
{
	string word(word1);
	//Check Entries Left
	if (AttemptsLeft <= 0)
	{
		cout << "No Attempts Left!" << endl << "Game Over!!" << endl;
		return -1;
	}

	//Capitalize entry
	std::transform(word.begin(), word.end(), word.begin(), ::toupper);

	if (!word.compare(Password))
	{
		GameWon = true;
		return WordLength;
	}
	else
	{
		AttemptsLeft -= 1;

		//calculate likeness
		int likeness = 0;

		for (auto i = 0; i < WordLength; ++i)
		{
			if (word[i] == Password[i])
			{
				likeness++;
			}
		}
		return likeness;
	}
	
}


vector<string> HackerGame::GetPossibleWords()
{
	std::srand(unsigned(std::time(0)));

	std::vector <std::string> PossibleWords; // Vector to hold our words we read in.
	std::string str; // Temp string to

	std::ifstream fin(WordListFileName); // Open it up

	if (!fin.good()) //Check file
	{
		cout << "File Failed to open!" << endl;
		return PossibleWords;
	}

	while (fin >> str) //Read in words
	{
		PossibleWords.push_back(str);
	}
	fin.close(); // Close that file!

	// Shuffle words in the vector
	std::random_shuffle(PossibleWords.begin(), PossibleWords.end());

	//Keep only NumberOfWordsToDisplay words
	PossibleWords.resize(NumberOfWordsToDisplay);

	//Capitalize Words
	for (string& word : PossibleWords)
	{
		std::transform(word.begin(), word.end(), word.begin(), ::toupper);
	}

	return PossibleWords;
}


string HackerGame::BuildDisplayString(vector<string> Words)
{
	string DisplayString;
	int WordLocation = 0;
	
	//Header
	DisplayString += "ROBCO INDUSTRIES (TM) TERMALINK PROTOCOL \nENTER PASSWORD NOW \n\n"; 

	//lines
	for (int i = 0; i < NumberOfWordsToDisplay; i += 2)
	{
		///First Column
		//Display Address
		DisplayString += "0xF4F";
		if (i < 10)
		{
			DisplayString += "0";
			DisplayString += to_string(i);
		}
		else
		{
			DisplayString += to_string(i);
		}
		DisplayString += "   ";

		WordLocation = (rand() % 19);

		for (int j = 0; j < WordLocation; ++j)
		{
			DisplayString += genRandom();
		}

		DisplayString += "  ";
		DisplayString += Words.at(i);
		DisplayString += "  ";

		for (int j = WordLocation + 9; j < 27; ++j)
		{
			DisplayString += genRandom();
		}

		DisplayString += "   ";


		/// Second Column
		//Display Address
		DisplayString += "0xF4F";
		if (i < 10)
		{
			DisplayString += "0";
			DisplayString += to_string(i + 1);
		}
		else
		{
			DisplayString += to_string(i + 1);
		}
		DisplayString += "   ";

		WordLocation = (rand() % 19);

		for (int j = 0; j < WordLocation; ++j)
		{
			DisplayString += genRandom();
		}

		DisplayString += "  ";
		DisplayString += Words.at(i + 1);
		DisplayString += "  ";

		for (int j = WordLocation + 9; j < 27; ++j)
		{
			DisplayString += genRandom();
		}

		DisplayString += "\n";

	}
	strcpy(this->Display, DisplayString.c_str());
	return DisplayString;

}
