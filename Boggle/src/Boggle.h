/*This is the class definition for Boggle, including functions and variable declarations. 
It is done by Yue Liu(yuel3) and Jiyuan Cui(jiyuanc)*/
#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "grid.h"
#include "hashset.h"
#include "vector.h"
#include "bogglegui.h"
#include "shuffle.h"
#include "random.h"

using namespace std;

class Boggle {
public:
    Boggle(Lexicon& dictionary, string boardText = "");
    char getLetter(int row, int col) const;
    bool checkWord(string word) const;
    bool humanWordSearch(string word) ;
    Set<string> computerWordSearch();
    int getScoreHuman() const;
    int getScoreComputer() const;
    friend ostream& operator<<(ostream& out, Boggle& boggle);
    //present human results
    Set<string>* humanResults();

private:
    //helper functions for human and computer search
    bool searchHelper(string & word, int i, int j) const;
    void dfs(Set<string>& result, HashSet<int>& visited, int i, int j, string & partial);
    //pravite member variables
    Set<string> formed;
    Grid<string> board;
    Lexicon dict;
    int humanScore;
    int computerScore;
    Vector<string> computerResults;
};

#endif // _boggle_h
