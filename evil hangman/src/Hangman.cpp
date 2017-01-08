/*This is the class implementation of a hangman object*/
#include "Hangman.h"
/*construct a hangman object from a dictionary: read the dict file and construct a dictionary that maps 
word length to the set of all the words with that length*/
Hangman::Hangman(string & dictionary){
	ifstream infile(dictionary);

	string word;
	int length;

	while(getline(infile, word)){
		length = word.length();
		unordered_map<int, set<string>>::const_iterator got = dict.find(length);
		// map the stuff
		if(got == dict.end()){
			set<string> words_set;
			words_set.insert(word);
			dict[length] = words_set;
		}
		else{
			dict[length].insert(word);
		}
	}
}

/*initialize the pattern to be "---" */
void Hangman::initializePattern(int L){
	currentPattern = "";

	for(int i = 0; i< L; ++i){
		currentPattern += "-";
	}
	defaultPattern = currentPattern;
	current = dict[L];
}
/*set the number of attempts the player have*/
void Hangman::setGuessTime(int g){
	guess = g;
}

/*update two maps: patternCounter (map pattern to its occurance) and patternMap (map a pattern e.g. --E-- to a
set of words that have this pattern)*/
void Hangman::constructPatternMap(string & letter){
	//record the new guess made by user
	string humanGuess = letter;
	letters.insert(humanGuess);
	//clear patternCounter and patternMap of last round
	if(! patternCounter.empty()){
		patternCounter.clear();
	}

	if(! patternMap.empty()){
		patternMap.clear();
	}
	//based on user's new guess: compute different word families and their corresponding size
	for(auto it = current.begin(); it != current.end(); ++it){
		string pattern = currentPattern;
		for(size_t i = 0; i < it->length(); ++i){
			string currentWord = *it;
			if(currentWord[i] == letter[0]){
				pattern[i] = letter[0];
			}
		}
		auto got = patternMap.find(pattern);
		//put the current word to the set corresponding to its pattern
		if(got != patternMap.end()){
			got->second.insert(*it);
		}
		else{
			set<string> wordsOfThisPattern;
			wordsOfThisPattern.insert(*it);
			patternMap[pattern] = wordsOfThisPattern;
		}
		//increment the frequence of the pattern 
		patternCounter[pattern] += 1;
	}
}
/*update the word list*/
void Hangman::updateCurrentList(){
	string largest_pattern = "";
	int freq = 0;
	//find the word family with largest number of occurance
	for(auto it = patternCounter.begin(); it != patternCounter.end(); ++it){
		if(it->second > freq){
			largest_pattern = it->first;
			freq = it->second;
		}
	}
	current = patternMap[largest_pattern];
	defaultPattern = currentPattern;
	currentPattern = largest_pattern;

	//if does the new pattern does not contain the letter user guessed, reduce # of remaining guesses by 1
	if(currentPattern == defaultPattern){
		guess--;
	}
}
/*validate the length input by user*/
bool Hangman::validLength(int length) const{
	auto got = dict.find(length);
	return got != dict.end();
}
/*get the remaining # of guess*/
int Hangman::getGuess() const{
	return guess;
}
/*print the letters that have been guessed*/
void Hangman::printAttempts() const{
	if(letters.empty()){
		cout<<"Haven't made any guess"<<endl;
	}
	else{
		for(auto it = letters.begin(); it != letters.end(); ++it){
			cout<<*it;
		}
	}
	cout<<" "<<endl;
}
/*print the size of current word list*/
void Hangman::printListSize() const{
	cout<<"Words Remaining: "<<current.size()<<endl;
}
/*print the current pattern that the user managed to get:  e.g.  --a-b-*/
void Hangman::printPattern() const{
	cout<<"Current word: "<<currentPattern<<endl;
}
/*validate the user-input letter*/
bool Hangman::validLetter(string & letter) const{
	//must be a single letter
	if(letter.length() != 1){
		cout<<"You must enter a single letter !"<<endl;
		return false;
	}
	//must be a letter
	if(letter[0] < 65 || letter[0] >122 || (letter[0] > 90 && letter[0] < 97)){
		cout<<"You must enter an English letter !"<<endl;
		return false;
	}
	//must be a letter that has not been guessed
	if(letters.find(letter) != letters.end()){
		cout<<"You have guessed this letter !"<<endl;
		return false;
	}
	return true;
}
/*check if the game should end and what the result is*/
bool Hangman::judgeTheGame() const{
	if(guess == 0){
		if(current.size() >= 1){
			cout<<"You lost, the word I chose is: "<<*(current.begin())<<endl;
		}
		return true;
	}
	else{
		if(current.size() == 1){
			if(currentPattern == *(current.begin())){
				cout<<"Congrats! You find the word: "<<*(current.begin())<<endl;
				return true;
			}
		}
	}
	return false;		
}