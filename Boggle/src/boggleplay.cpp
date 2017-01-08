/*this file contains all the functions for conducting a boggle game.
It is done by Yue Liu(yuel3) and Jiyuan Cui(jiyuanc)*/
#include "Boggle.h"
#include "strlib.h"
#include "simpio.h"
#include "console.h"

using namespace std;
/*check if the input text is valid: only contains English letters*/
bool valid(string & text){
	for(char c: text){
		if((c < 'a' && c > 'Z') || (c > 'z') || (c < 'A')){
			return false;
		}
	}
	return true;
}
/*helper function for handling invalid user input*/
void handleInput(string & text){
	while(true){
		if(text.length() != 16 || ! valid(text)){
			cout<<"That is not a valid 16-letter board string. Try again."<<endl;
			text = getLine("Type the 16 letters to appear on the board:");
		}
		else{
			break;
		}
	}
}
/*helper function for displaying human results in boggle, including words and score*/
void displayHuman(Boggle & b){
    cout<<endl;
    string stuff = "";
    Set<string>* humanSet = b.humanResults();
    cout<<"Your words ("<<humanSet->size()<<"): "<<humanSet->toString()<<endl;
    cout<<"Your score: "<<b.getScoreHuman()<<endl;
}
/*helper function for doing human player part of the game*/
void humanPart(string & userInput, Boggle& game){
	BoggleGUI::setAnimationDelay(100);
	while(true){
		//get user input
		userInput = getLine("Type a word (or Enter to stop):");
		if(userInput == ""){
			break;
		}
		clearConsole();
		cout<<"==================== (console cleared) ===================="<<endl;
		// if the game is invalid, yell the player
		if(!game.checkWord(userInput)){
			cout<<"You must enter an unfound 4+ letter word from the dictionary."<<endl;
			BoggleGUI::setStatusMessage("You must enter an unfound 4+ letter word from the dictionary.");
		}
		else{//else do the search, if we do find the word, congrat the player
			if(game.humanWordSearch(userInput)){
				cout<<"You found a new word! \""<<toUpperCase(userInput)<<"\""<<endl;
				BoggleGUI::setStatusMessage("You found a new word!\""+toUpperCase(userInput)+"\"");
			}
			else{// else...we are sorry
				cout<<"That word can't be formed on this board."<<endl;
				BoggleGUI::setStatusMessage("That word can't be formed on this board.");
			}
		}
		// print out the letter board and human results
		cout<<game;
		displayHuman(game);
	}
	cout<<endl;
}
/*helper function for displaying computer word search results, including words and scores*/
void displayComputer(Boggle & b){
	Set<string> computerSet = b.computerWordSearch();
	cout<<"My words ("<<computerSet.size()<<"): "<<computerSet.toString()<<endl;
	cout<<"My score: "<<b.getScoreComputer()<<endl;

}
/*compare human score with computer score, admire or laugh at human lol*/
void compareScore(Boggle & b){
	if(b.getScoreComputer() > b.getScoreHuman()){
		cout<<"Ha ha ha, I destroyed you. Better luck next time, puny human!"<<endl;
		BoggleGUI::setStatusMessage("Ha ha ha, I destroyed you. Better luck next time, puny human!");
	}
	else{
		cout<<"WOW, you defeated me! Congratulations!"<<endl;
		BoggleGUI::setStatusMessage("WOW, you defeated me! Congratulations!");
	}
}
/*main function: play one game*/
void playOneGame(Lexicon& dictionary) {
    BoggleGUI::initialize(4, 4);
	bool randomGen = getYesOrNo("Do you want to generate a random board?");
	//get board text
	string boardText = "";
	if(!randomGen){
		boardText = getLine("Type the 16 letters to appear on the board: ");
		handleInput(boardText);
	}
	// initialize boggle object
	Boggle game(dictionary, boardText);
	clearConsole();
	cout<<"==================== (console cleared) ===================="<<endl;
	cout<<"It's your turn!"<<endl;
	BoggleGUI::setStatusMessage("It's your turn!");
	// play human part
	cout<<game;
	displayHuman(game);
	string userInput;
	humanPart(userInput, game);
	//play computer part
	cout<<"It's my turn!"<<endl;
	BoggleGUI::setStatusMessage("It's my turn!");
	displayComputer(game);
	// compare results
	compareScore(game);
}