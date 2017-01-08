/*implementation of the five mile stones, all of which were done in recursion. This assignment was completed by Yue Liu(yuel3)
and Jiyuan Cui(jiyuanc) together*/
#include <stdexcept>
#include <math.h>
#include "recursion.h"
#include "map.h"
#include "vector.h"
#include "set.h"
#include "gwindow.h"
#include "gobjects.h"
#include "tokenscanner.h"
#include "hashset.h"
#include "random.h"
using namespace std;
/* milestone 1*/
int gcd(int a, int b) {
    // base case
    if(b == 0){
        return a;
    }
    // output path
    cout<<"gcd("<<a<<", "<<b<<") = gcd("<<b<<", "<<a%b<<")"<<endl;
    // recursive call
    return gcd(b, a%b);
}
/* milestone 2*/
void serpinskii(GWindow &w, int leftX, int leftY, int size, int order) {
    // invalid inputs
    if(order == 0){
        return;
    }   
    if(order < 0){
        throw invalid_argument( "received negative order" );        
    }
    // base case, just draw a triangle
    if(order == 1){
        w.drawLine(leftX, leftY, leftX+size, leftY);
        w.drawLine(leftX, leftY, leftX+(double)size/2, leftY+(sqrt(3)*size)/2);
        w.drawLine(leftX+size, leftY, leftX+(double)size/2, leftY+(sqrt(3)*size)/2);
    }
    //find new size and new order for next call
    int newSize = size/2;
    int newOrder = order-1;
    //recursively call
    serpinskii(w, leftX, leftY, newSize, newOrder);
    serpinskii(w, leftX+newSize, leftY, newSize, newOrder);
    serpinskii(w, leftX+(newSize/2), leftY+(newSize/2)*sqrt(3), newSize, newOrder);
    
}
/* milestone 3*/
int floodFill(GBufferedImage& image, int x, int y, int newColor) {
    int currentColor = image.getRGB(x,y);
    // base case
    if(currentColor == newColor)
        return 0;
    //update pixel value
    image.setRGB(x,y,newColor);
    // traverse up, down, left, right
    int pixelCount = 1;
    int dx[] = {1,-1,0,0};
    int dy[] = {0,0,1,-1};

    for(int i = 0; i < 4; i++){
        int newX = x+dx[i];
        int newY = y+dy[i];
        // if the neighbor is inbound and in the same color region, recursively call on the neighbor
        if(image.inBounds(newX, newY) && 
            image.getRGB(newX,newY) == currentColor){
            //get all the pixel counts 
            pixelCount += floodFill(image, newX, newY, newColor);
        }
    }
    return pixelCount;
}
/* helper function for milestone 4: do a dfs*/
void dfs(Map<string, Vector<string>> & prereqMap,string goal, HashSet<string> & visited){
    // if not visited: mark as visited, then recursively call on its neighbors
    if(!visited.contains(goal)){
        visited.add(goal);

        Vector<string> prerequisites = prereqMap.get(goal);
        for(string direct_pre : prerequisites){
            dfs(prereqMap, direct_pre, visited);
        }
        // after done with traversing neighbors, yell out the current goal
        cout<<goal<<endl;
    }
}
/* milestone 4 */
void personalCurriculum(Map<string, Vector<string>> & prereqMap,string goal) {
    // initialize a hashmap and do dfs
    HashSet<string> visited;
    dfs(prereqMap, goal, visited);
}
/* milestone 5 */
string generate(Map<string, Vector<string> > & grammar, string symbol) {
    // base case
    if(! grammar.containsKey(symbol)){
        return symbol;
    }
    string ret = "";
    // get current symbol, randomly choose a rule
    Vector<string> rules = grammar.get(symbol);
    int randIndex = randomInteger(0, rules.size()-1);
    string randomRule = rules[randIndex];
    // tokenize the rule and for each part of the rule, recursively call on it
    TokenScanner scanner(randomRule);
    while (scanner.hasMoreTokens()) {
        string token = scanner.nextToken();
        ret += generate(grammar, token);
    }
    return ret;
}
