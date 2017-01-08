#include <iostream>
#include <string>
#include <istream>
#include <fstream>
#include <vector>
#include <chrono>
#include <random>
#include <functional>

using namespace std;

int main(){

	// vector<int> v;
	// v.push_back(1);
	// v.push_back(2);
	// v.push_back(3);
	// v.push_back(4);

	// random_device rd;
 //    mt19937 gen(rd());

	// discrete_distribution<> second (v.begin(),v.end());
	// cout<<second(gen)<<endl;
	ifstream input("/Users/yueliu/Desktop/projects/cs106/serafini_starter/Ngrams/res/tiny.txt");

	string word;
	string dummy;
	ifstream copy("/Users/yueliu/Desktop/projects/cs106/serafini_starter/Ngrams/res/tiny.txt");
	// ifstream temp("/Users/yueliu/Desktop/projects/cs106/serafini_starter/Ngrams/res/constitution.txt");
	int N = 3;
	int something = N-1;
	int round = 0;
	int location;
	int count = 0;
	input.seekg(0,input.end);
	int length = input.tellg();
	input.seekg(0,input.beg);
	
	while(true){
		
		while(something>0){
			if(input.tellg() >= length-2 || input.tellg() == -1){
				input.clear();
				input.seekg(0, std::ios::beg);
			}
			input>>word;
			something--;
			cout<<word<<endl;
		}
		cout<<"-------------"<<endl;
		something = N-1;
		// cout<<"input location: "<<input.tellg()<<endl;
		if(input.tellg() >= length-2 || input.tellg() == -1)
		{
			cout<<input.tellg()<<endl;
			input.clear();
			input.seekg(0, std::ios::beg);
		}
		input>>word;
		cout<<word<<endl;
		copy>>dummy;
		location = copy.tellg();
		// cout<<"location: "<<location<<endl;
		// if(round ==20)
			// break;
		if(location == -1)
			break;
		round++;
		input.clear();
    	input.seekg(location, std::ios::beg);
    
	}
	cout<<"length : "<<length<<endl;	
	return 0;
}