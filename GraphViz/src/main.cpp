/*This program is an implementation of graph visualization algorithm based on force and energy, basically it wants
to achieve the minimum of potential energy for the system*/
#include <iostream>
#include "SimpleGraph.h"
#include <cmath>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
using namespace std;

const double PI = 3.14159265358979323;
const double k = 0.001;

void Welcome();
void handleInput(string & filename);
void readGraph(ifstream& input);
void initializePos(SimpleGraph& graph);
int adjustPos(SimpleGraph& graph, int duration, time_t start_time);
/*helper function for checking users' invalid input*/
void handleInput(string & filename, ifstream& input){
	while(!input.good()){
		cout<<"file not exist, please specify a valid file name"<<endl;
		input.clear();
		getline(cin, filename);
		input.open(filename);
	}
}
/*load graph data including nodes and edges from a file */
void readGraph(ifstream& input, SimpleGraph& graph){
	int count = 0;
	int n; 
    while (true) {
        string line;
        getline(input, line);	
        
        if (input.fail()) break;
        istringstream ss(line);
        // get # of nodes
        if (count == 0){
        	ss>>n;
        }//load all the edges by reading line by line
        else{
        	size_t start;
        	size_t end;
        	ss>>start;
        	ss>>end;
        	Edge e;
        	e.start = start;
        	e.end = end;
        	
        	graph.edges.push_back(e);
        }
        ++count;
    }
    // load all the nodes
    for(int i = 0; i < n; i++){
    	Node graphNode;
    	graph.nodes.push_back(graphNode);
    }
}
/*initialize all the nodes on unit circle*/
void initializePos(SimpleGraph& graph){
	int numOfNodes = graph.nodes.size();

	for(int i = 0; i < numOfNodes; i++){
		graph.nodes[i].x = cos((2*PI*i)/numOfNodes);
		graph.nodes[i].y = sin((2*PI*i)/numOfNodes);
	}
}
/*get computation duration from user input*/
void getTime(int& duration){
	cin >> duration;

	while(duration <= 0){
		cout<<"invalid input, please enter an integer that is at least 1:"<<endl;
		cin>>duration;
	}
}
/*compute Euclidean distance given 2 sets of coordinates*/
double dist(double x0,double y0,double x1,double y1){
	return sqrt(pow(x0-x1,2)+pow(y0-y1,2));
}
/* in iterations, adjust the position of nodes by updating the deltaX and deltaY*/
int adjustPos(SimpleGraph& graph, int duration, time_t start_time){
	InitGraphVisualizer(graph);
	while(difftime(time(NULL), start_time) < duration){
		
		int numOfNodes = graph.nodes.size();
		vector<double> deltaXs(numOfNodes, 0);
		vector<double> deltaYs(numOfNodes, 0);
		// update deltaX
		for(int i = 0; i < numOfNodes; i++){
			for(int j = i+1; j < numOfNodes; j++){
				double F_repel = k/dist(graph.nodes[i].x, graph.nodes[i].y, graph.nodes[j].x, graph.nodes[j].y);
				double theta = atan2(graph.nodes[j].y-graph.nodes[i].y, graph.nodes[j].x-graph.nodes[i].x);
				deltaXs[i] -= F_repel*cos(theta);
				deltaYs[i] -= F_repel*sin(theta);
				deltaXs[j] += F_repel*cos(theta);
				deltaYs[j] += F_repel*sin(theta);
			}
		}
		// update deltaYs
		for(Edge e: graph.edges){
			size_t startIndex = e.start;
			size_t endIndex = e.end;

			Node start = graph.nodes[startIndex];
			Node end = graph.nodes[endIndex];
			double F_attrct = k * (pow((start.x-end.x),2)+ pow((start.y - end.y),2));
			double theta = atan2(end.y-start.y, end.x-start.x);
			deltaXs[startIndex] += F_attrct*cos(theta);
			deltaYs[startIndex] += F_attrct*sin(theta);
			deltaXs[endIndex] -= F_attrct*cos(theta);
			deltaYs[endIndex] -= F_attrct*sin(theta);
		}
		// update positions
		for(int i = 0; i < numOfNodes; i++){
			graph.nodes[i].x += deltaXs[i];
			graph.nodes[i].y += deltaYs[i];
		}

		DrawGraph(graph);
	}
	return 0;
}
/* Prints a message to the console welcoming the user and
 * describing the program. */
void Welcome() {
    cout << "Welcome to CS106L GraphViz!" << endl;
    cout << "This program uses a force-directed graph layout algorithm" << endl;
    cout << "to render sleek, snazzy pictures of various graphs." << endl;
    cout << endl;
}

/* Main method*/
int main() {
    Welcome();
    while(true){
	   	cout<<"Please specify the graph file you want to open"<<endl;
	    
	    string filename;
	    cin>>filename;
	    ifstream input(filename.c_str());
	    handleInput(filename, input);
	    SimpleGraph graph;
	    readGraph(input, graph);
	    initializePos(graph);
	   
	    cout<<"Please specify how long you want the optimization to run, it should be an integer: "<<endl;
	    int duration;
	    getTime(duration);
	    time_t start = time(NULL);
	    adjustPos(graph, duration, start);
	    input.clear();
	    cout<<"Do one more round? Y/N"<<endl;
	    string next;
	    cin>>next;
	    if(next != "Y"){
	    	cout<<"done with the stuff, please hit ctrl+c"<<endl;
	    	break;
	    }
    }
    return 0;
}