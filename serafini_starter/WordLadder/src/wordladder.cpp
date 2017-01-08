/*This program contains main and helper functions for finding a word ladder given two words in the dictionary, which
is based on bfs */
#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "strlib.h"
#include "lexicon.h"
#include "simpio.h"
#include "stack.h"
#include "queue.h"
#include "hashset.h"
#include "vector.h"
#include "filelib.h"

using namespace std;
// helper function for handling several invalid inputs
void handleInput(string& word1, string& word2, Lexicon & d, bool & continuePlaying){
	word1 = toLowerCase(word1);
	word2 = toLowerCase(word2);
	// indicator for all the possible invalid cases
	bool valid = ((word1.length() == word2.length()) 
		&& (d.contains(word1)) && (d.contains(word2)) 
		&& (!equalsIgnoreCase(word1, word2)));
	// report invalid cases and get new inputs
	while(! valid){
		if(word1.length() != word2.length()){
			cout<<"The two words must be the same length."<<endl;
		}
		if(! d.contains(word1) || ! d.contains(word2)){
			cout<<"The two words must be found in the dictionary."<<endl;
		}
		if(equalsIgnoreCase(word1, word2)){
			cout<<"The two words must be different."<<endl;
		}
		
		word1 = toLowerCase(getLine("Word #1 (or Enter to quit): "));
		
		if(word1 == "")
		{
			continuePlaying = false;
			break;
		}
		word2 = toLowerCase(getLine("Word #2 (or Enter to quit): "));
		if(word2 == ""){
			continuePlaying = false;
			break;
		}

		valid = ((word1.length() == word2.length()) 
		&& (d.contains(word1)) && (d.contains(word2)) 
		&& (!equalsIgnoreCase(word1, word2)));	
	}
}
// helper function for finding neighbors of a word
void findNeighbors(Vector<string>& v, string & current, Lexicon& d){
	/* for each character in the word, replace it with another character, check if it's in dictionary
	 if it does, put it into the vector */
	for(int i = 0; i < current.length(); i++){
		string temp = current;
		for(int j = 0 ; j < 26; j++){
			if(('a'+j) != temp[i]){
				temp.replace(i,1,charToString('a'+j));
			}
			if(d.contains(temp)){
				v.add(temp);
			}
		}
	}
}
// do bfs and return a stack of string which contains the path
Stack<string> bfs(Lexicon & d, string & word, string & target){
	// typical bfs set up
	Queue<Stack<string>> q;
	Stack<string> s;
	HashSet<string> visited;

	s.push(word);
	q.enqueue(s);
	// traverse the graph
	while(!q.isEmpty()){
		Stack<string> partial_ladder = q.dequeue();
		string topWord = partial_ladder.peek();
		visited.add(topWord);

		Vector<string> neighbors;
		findNeighbors(neighbors, topWord, d);

		for(string neighbor: neighbors){
			// if not visited
			if (! visited.contains(neighbor)){
				visited.add(neighbor);
				// if find target, return the stack
				if(equalsIgnoreCase(neighbor, target)){
					partial_ladder.push(neighbor);
					return partial_ladder;
				}
				else{ //else update stack and put into queue
					Stack<string> partial_ladder_copy;
					Stack<string> temp1 = partial_ladder;
					Stack<string> temp2;
					string top_value;

					while(!temp1.isEmpty()){
						top_value = temp1.peek();
						temp1.pop();
						temp2.push(top_value);
					}
					while(!temp2.isEmpty()){
						top_value = temp2.peek();
						temp2.pop();
						partial_ladder_copy.push(top_value);
					}
					partial_ladder_copy.push(neighbor);
					q.enqueue(partial_ladder_copy);
				}
			}
		}
	}
	// if nothing found, return an empty stack
	return Stack<string>();
}
// helper function for displaying the result
void display(Stack<string>& s, string & word1, string & word2){
	if(s.isEmpty()){
		cout<<"No word ladder found from "<<word2<<" back to "<<word1<<endl;
		return;
	}

	cout<<"A ladder back from "<<word2<<" to "<<word1<<":"<<endl;
	while(! s.isEmpty()){
		string w = s.peek();
		s.pop();
		if(s.isEmpty())
			cout<<w<<endl;
		else
			cout<<w<<" ";
	}
}
// helper function for checking file existance
void checkFileName(string & filename){
	while(!fileExists(filename)){
		cout<<"Unable to open that file.  Try again."<<endl;
		filename = getLine("Dictionary file name?");
	}
}

int main() {
    cout << "Welcome to CS 106B Word Ladder."<<endl;
    cout<<"Please give me two English words, and I will change the first into the second by changing one letter at a time."<<endl;	
	// check file existance and load data
	string filename = getLine("Dictionary file name?");
	checkFileName(filename);
    Lexicon dictionary(filename);

	while(true){
		// get inputs and validate, process them
		string word1 = getLine("Word #1 (or Enter to quit):");
		if(word1 == "")
			break;

		string word2 = getLine("Word #2 (or Enter to quit):");
		if(word2 == "")
			break;

		bool continuePlaying = true;
		handleInput(word1, word2, dictionary, continuePlaying);
		
		if(!continuePlaying)
			break;
		// do bfs
		Stack<string> result = bfs(dictionary, word1, word2);
		// display result
		display(result, word1, word2);
	}

    cout << "Have a nice day." << endl;
    return 0;
}
