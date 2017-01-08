/* this file contains function implementations of Boggle class
It is done by Yue Liu(yuel3) and Jiyuan Cui(jiyuanc)*/
#include "Boggle.h"
// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

// letters on every cube in 5x5 "Big Boggle" version (extension)
static string BIG_BOGGLE_CUBES[25] = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};
//constructor of Boggle object
Boggle::Boggle(Lexicon& dictionary, string boardText) {
    // initialize the board, humanScore, computerScore and dictionary
    board.resize(4,4);
    dict = dictionary;
    humanScore = 0;
    computerScore = 0;
    //if user self-defines the text, put each letter in the grid
    if(boardText != ""){
        boardText = toUpperCase(boardText);
        
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                board.set(i,j,boardText.substr(i*4+j, 1));
            }
        }
        BoggleGUI::labelAllCubes(boardText);
    }
    else{// else randomize the given cubes: first shuffle the string array, then for each string
    //choose the letter that faces on 
        shuffle(CUBES, 16);
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                string currentStr = CUBES[i*4+j];
                int letterIndex = randomInteger(0,3);

                board.set(i,j,currentStr.substr(letterIndex, 1));
                BoggleGUI::labelCube(i, j, stringToChar(board[i][j]), false);
            }
        }
    }
}
/* return the letter on (row, col), if out of bound, throw an exception */
char Boggle::getLetter(int row, int col) const{
    if(board.inBounds(row, col)){
        return stringToChar(board[row][col]);
    }
    else{
        throw "index out of bound !";
    }
}
/* check if a word is valid according to 3 criterias: length, whether it's in dictionary, whether it's already
been generated */
bool Boggle::checkWord(string word) const{
    word = toUpperCase(word);
    if(word.length() >= 4 && dict.contains(word) && !formed.contains(word)){
        return true;
    }

    return false; 
}
/* search if the given word can be formed in the grid*/
bool Boggle::humanWordSearch(string word) {
    BoggleGUI::clearHighlighting();
    word = toUpperCase(word);
    //if the word is valid
    if(checkWord(word)){
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                BoggleGUI::setHighlighted(i, j, true);
                //starting from each cell, check if the word can be formed, if so, record it, update human score
                if(searchHelper(word,i,j)){
                    humanScore += (word.length()-3);
                    BoggleGUI::setScore(humanScore, BoggleGUI::HUMAN);
                    recordWord(word, BoggleGUI::HUMAN);
                    formed.add(word);
                    return true;
                }
                BoggleGUI::setHighlighted(i, j, false);
            }
        }
    }
    return false;
}
/*helper function for human word search*/
bool Boggle::searchHelper(string & word, int i, int j) const{
    //base case1: empty string, return true
    if(word == ""){
        return true;
    }
    //base case2: if the first letter of the word is not the current cell letter or out of bound, return false
    if(!board.inBounds(i,j) || word.substr(0,1) != board[i][j]  ){
        return false;
    }
    //recursive: 
    for(int dx = -1; dx <= 1; dx++){
        for(int dy = -1; dy <= 1; dy++){
            string next = word.substr(1);
            //check if the substring starting from the second letter can be formed, if so, return true
            if(searchHelper(next, i+dx, j+dy)){
                BoggleGUI::setHighlighted(i, j, true);
                return true;
            }
        }
    }
    return false;
}
/*return human score*/
int Boggle::getScoreHuman() const{
    return humanScore;
}
/*get computer word search results*/
Set<string> Boggle::computerWordSearch() {
    Set<string> result;
    HashSet<int> visited;
    //for each cell in the board, start from it, do dfs
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            string initial = "";
            dfs(result,visited, i, j, initial);
        }
    }
    BoggleGUI::setScore(computerScore, BoggleGUI::COMPUTER);
    
    return result;        
}
/*helper function for computer word search*/
void Boggle::dfs(Set<string>& result, HashSet<int>& visited, int i, int j, string & partial){
    //base case
    if(!board.inBounds(i,j) || visited.contains(i*4+j) || !dict.containsPrefix(partial))
        return;
    // mark the current cell visited
    visited.add(i*4+j);
    //generate new word with one more letter
    string newWord = partial+board[i][j];
    //if the new word is valid, add it to the result set, update score
    if(checkWord(newWord)){
        result.add(newWord);
        formed.add(newWord);
        BoggleGUI::recordWord(newWord, BoggleGUI::COMPUTER);
        computerScore += (newWord.length()-3);
    }
    // for all the neighbors, revursively call the function
    for(int dx = -1; dx < 2; dx++){
        for(int dy = -1; dy < 2; dy++){
            dfs(result, visited, i+dx, j+dy, newWord);
        }
    }
    //unmark the current cell letter
    visited.remove(i*4+j);
}
/*return the computer score*/
int Boggle::getScoreComputer() const{
    return computerScore;
}
/*over-write << operator for boggle object, print the associated grid*/
ostream& operator<<(ostream& out, Boggle& boggle){
    for(int i = 0; i < 4; i++){
        string row = "";
        for(int j = 0; j < 4; j++){
            row+=boggle.board[i][j];
        } 
        out<<row<<endl;
    }
    return out;
}
/*helper function for displaying results: return a pointer to the human results set*/
Set<string>* Boggle::humanResults() {
    return &formed;
}