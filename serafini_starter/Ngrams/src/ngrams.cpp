/*This program includes the main and helper functions for n-gram generation, which is based on frequency and
weighted random sampling*/
#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "vector.h"
#include "strlib.h"
#include "simpio.h"
#include "hashmap.h"
#include "random.h"
#include "filelib.h"
#include <random>

using namespace std;
// helper for validating filename and open file
void checkFileName(string & filename, ifstream & f){
	while(!fileExists(filename)){
		cout<<"Unable to find that file.  Try again."<<endl;
		filename = getLine("Input file name?");
	}
	f.open(filename);
}
// helper function for building the map : first N-1 words --> a map{Nth word: frequency}
void build(HashMap<Vector<string>, HashMap<string, int>> & map, Vector<string> & window, ifstream & input,ifstream & copy, int N){
	int count = N-1;
	// dummy is for holding dummy values from ifstream "copy", which is used to adjust the cursor location
	string dummy;
	string word;
	string target;
	int locationToReturn;
	// get the length of the doc: for terminating the while loop
	input.seekg(0,input.end);
	int length = input.tellg();
	rewindStream(input);
	
	while(true){
		// read first N-1 words, if eof, move the cursor to the beginning of the doc
		while(count > 0){
			if(input.tellg() >= length-2 || input.tellg() == -1){
				rewindStream(input);
			}
			input >> word;
			window.add(word);
			count--;
		}

		/* if location of cursor is at second last character of the doc, or the reading fails, move the cursor to
		 the beginning*/
		if(input.tellg() >= length-2 || input.tellg() == -1){
			rewindStream(input);
		}
		//read taget word, which is the Nth word of the N gram; move dummy cursor one step forward
		input >> target;
		copy >> dummy;
		//map first N-1 words to the other map, which maps the Nth word to its frequency
		if(! map.containsKey(window)){

			HashMap<string, int> freq;
			freq.put(target, 1);
			map.put(window, freq);
		}
		else{
			 if(map[window].containsKey(target))
			 	map[window][target]++;
			 else
			 	map[window][target] = 1;
		}
		// find the location of the dummy cursor, if it reaches the end of file, complete the building process
		locationToReturn = copy.tellg();
		if(locationToReturn == -1){
			break;
		}
		//restore the value of count for reading next N gram
		count = N-1;
		//move cursor back to the location of the dummy cursor
		input.clear();
    	input.seekg(locationToReturn, std::ios::beg);
		
		// clear the window vector for next N gram
		window.clear();
	}
		
}
// give a weight vector sample an index based on the weights
int randomSelection(Vector<int> & weights){

	random_device rd;
    mt19937 gen(rd());

	discrete_distribution<> engine (weights.begin(),weights.end());
	
	return engine(gen);
}
// generate the text
void generate(int num, HashMap<Vector<string>, HashMap<string, int>> & map, int N){

	int round = 1;
	Vector<Vector<string>> keys = map.keys();
	// randomly select a key to start
	int startIndex = randomInteger(0,keys.size()-1);
	Vector<string> start = keys[startIndex];

	cout<<"... ";

	while(num > 0){
		// find the corresponding map of the key
		HashMap<string, int> freq = map.get(start);
		// get all the frequency, select the target word index based on the frequency
		Vector<int> values = freq.values();
		Vector<string> subKeys = freq.keys();
		int index = randomSelection(values);
		// generate all the words
		if(round == 1){
			for(string w : start){
				cout<<w<<" ";
			}
		}
		cout<<subKeys[index]<<" ";
		// update the first N-1 words for next round of generation
		for(int i = 0; i < N-2; i++){
			start[i] = start[i+1];
		}

		start[N-2] = subKeys[index];
		
		if(round == 1)
			num -= N;
		else
			num--;
		round++;
	}
	cout<<" ..."<<endl;
}

int main(){
	cout<<"Welcome to CS 106B Random Writer ('N-Grams')."<<endl;
	cout<<"This program makes random text based on a document."<<endl;
	cout<<"give me an input file and an 'N' value for groups of words, and I'll create random text for you."<<endl;
	// get inputs and validate them
	string filename = getLine("Input file name?");
	ifstream input;
	checkFileName(filename, input);
	// this stream is for remembering position
	ifstream copy(filename);
	int N = getInteger("Value of N?");

	while(N < 2){
		cout<<"N must be 2 or greater."<<endl;
		N = getInteger("Value of N?");
	}
	
	HashMap<Vector<string>, HashMap<string, int>> map;
	Vector<string> window;
	// build the map
	build(map, window,input,copy,N);
	// generate stuff
	while(true){

		int num = getInteger("# of random words to generate (0 to quit)?");
		
		while(num < N && num != 0){
			cout<<"must be at least "<<N<<" words."<<endl;
			num = getInteger("# of random words to generate (0 to quit)?");
		}
		if(num == 0)
			break;
		generate(num, map, N);

	}
	cout << "Exiting." << endl;
    return 0;
}
