/*this is the main function of the evil hangman game*/
#include "Hangman.h"
#include <ctype.h>
/*validate if the user input length is valid*/
void checkLength(int & length, Hangman & game){
	while(! game.validLength(length)){
		cout<<"Invalid word length, no word with such length, please re-enter length:"<<endl;
		cin>>length;
	}
}
/*validate if the user input guess time is valid*/
void checkGuess(int & guess){
	while(guess <= 0){
		cout<<"Invalid guess number, it should be positive integer, please re-enter:"<<endl;
		cin>>guess;
	}
}
/*validate if the user input guess letter is valid*/
void checkLetter(string & letter, Hangman & game){
	while(! game.validLetter(letter)){
		cin>>letter;
	}
}
/*get all the user input: whether display, length of words, # of guesses to make*/
void getInfo(int & length, int & guess, char & display, Hangman & game){
	cout<<"Please enter a word length: 	"<<endl;
	cin>>length;
	//validation
	checkLength(length, game);
	//game initialization
	game.initializePattern(length);

	cout<<"Please enter how many guess you are gonna make:"<<endl;

	cin>>guess;
	//validation
	checkGuess(guess);
	game.setGuessTime(guess);

	cout<<"Do you want to see # of words remaining in the list? Y/N"<<endl;
	cin>>display;
}
/*game printing work*/
void printStuff(char display, Hangman& game){
	cout<<"You have "<<game.getGuess()<<" attempts remaining."<<endl;

	cout<<"You've guessed these letters: "<<endl;
	game.printAttempts();
	game.printPattern();

	if(toupper(display) == 'Y'){
		game.printListSize();
	}
}

int main(){
	cout<<"Welcome to Evil Hangman!"<<endl;
	string dictionary = "dictionary.txt";
	//build the Hangman object
	Hangman game(dictionary);

	while(true){
		int length, guess;
		char display;

		getInfo(length, guess, display, game);

		while(true){
			printStuff(display, game);
			
			cout<<"Enter the letter of your guess: "<<endl;
			string letter;
			cin >> letter;
			checkLetter(letter, game);

			game.constructPatternMap(letter);
			game.updateCurrentList();
			//check if the game comes to an end, if so print result
			if(game.judgeTheGame()){
				game.printPattern();
				break;
			}
		}
		// check if play one more round
		cout<<"Play one more game? Y/N: ";
		char oneMore;
		cin>>oneMore;
		if(toupper(oneMore) != 'Y'){
			break;
		}
	}
	return 0;
}