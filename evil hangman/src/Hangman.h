/*This is the class definition of a hangman game object*/
#ifndef _hangman_h
#define _hangman_h

#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <set>

using namespace std;

class Hangman{
public:
	Hangman(string & dictionary);
	void updateCurrentList();
	void setGuessTime(int g);
	void constructPatternMap(string & letter);
	bool validLength(int length) const;
	int getGuess() const;
	void printAttempts() const;
	void printListSize() const;
	void printPattern() const;
	bool validLetter(string & letter) const;
	bool judgeTheGame() const;
	void initializePattern(int L);

private:
	int guess;
	unordered_map<int, set<string>> dict;
	set<string> current;
	unordered_map<string,set<string>> patternMap;
	unordered_map<string, int> patternCounter;
	set<string> letters;
	string currentPattern;
	string defaultPattern;
};

#endif // _hangman_h