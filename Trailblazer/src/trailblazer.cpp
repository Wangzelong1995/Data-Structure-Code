/*These are implementations of bfs, dijkstras, a-start and alternative path. It was done by Yue Liu and Jiyuan Cui*/
#include "trailblazer.h"
#include "queue.h"
#include "set.h"
#include "pqueue.h"
#include "point.h"
#include "hashset.h"

using namespace std;

// this makes Path an alias for the type Vector<Vertex*>
typedef Vector<Vertex*> Path;

// the minimum difference for an alternate route
const double SUFFICIENT_DIFFERENCE = 0.2;

/*Implementation of bfs*/
Path breadthFirstSearch(RoadGraph& graph, Vertex* start, Vertex* end) {
    // set up
    Path emptyPath;

    Queue<Path> path_q;
    Set<Vertex*> visited;
    Path initial_path;
    initial_path.add(start);
    path_q.enqueue(initial_path);
    //traverse the graph
    while(!path_q.isEmpty()){
        Path curPath = path_q.dequeue();
        Vertex* curNode = curPath[curPath.size()-1];
        curNode->setColor(GREEN);
        if(curNode == end){
            return curPath;
        }
        visited.add(curNode);
        for(Vertex* neighbor: graph.getNeighbors(curNode)){
            if(! visited.contains(neighbor)){
                neighbor->setColor(YELLOW);
                Path new_path = curPath;
                new_path.add(neighbor);
                path_q.enqueue(new_path);
            }
        }
    }
    return emptyPath;
}
/*Implementation of dijkstras*/
Path dijkstrasAlgorithm(RoadGraph& graph, Vertex* start, Vertex* end) {
    //set up
    Path emptyPath;
    PriorityQueue<Path> path_pq;    
    Set<Vertex*> visited;
    Path initial_path;
    initial_path.add(start);
    path_pq.enqueue(initial_path, 0);

    while(!path_pq.isEmpty()){
        double cost = path_pq.peekPriority();
        Path curPath = path_pq.dequeue();
        Vertex* curNode = curPath[curPath.size()-1];
        curNode->setColor(GREEN);
        if(curNode == end){
            return curPath;
        }
        visited.add(curNode);
        for(Vertex* neighbor: graph.getNeighbors(curNode)){
            if(!visited.contains(neighbor)){
                //get new priority, which is the length of new path
                double newPriority = cost+graph.getEdge(curNode, neighbor)->cost;
                Path new_path = curPath;
                new_path.add(neighbor);
                path_pq.enqueue(new_path, newPriority);
                neighbor->setColor(YELLOW);
            }
        }
    }
    return emptyPath;
}

/*Helper for a star function, with parameters to specify which edge to ignore in the graph, there are three more parameters:

two end points of the edge to be ignored, and a variable to hold the best cost

The last variable is included so that I don't have to compute the path cost again after I find a path in alternative path*/
Path aStarHelper(RoadGraph& graph, Vertex* start, Vertex* end, Vertex* ignoreS, Vertex* ignoreE, double & costHolder){
    // set up
    Path emptyPath;
    PriorityQueue<Path> path_pq;
    Set<Vertex*> visited;
    Path initial_path;
    initial_path.add(start);
    path_pq.enqueue(initial_path, graph.getCrowFlyDistance(start, end)/graph.getMaxRoadSpeed());
    //map path to its length
    Map<Path, double> path2length;
    path2length[initial_path] = 0;

    while(!path_pq.isEmpty()){
        Path curPath = path_pq.dequeue();
        //get the length of the current path
        double length = path2length[curPath];
        Vertex* curNode = curPath[curPath.size()-1];
        curNode->setColor(GREEN);
        //if the current end is target, return the path and update cost
        if(curNode == end){
            costHolder = length;
            return curPath;
        }
        visited.add(curNode);
        for(Vertex* neighbor: graph.getNeighbors(curNode)){
            if(! visited.contains(neighbor)){
                //if the edge beging considered should be ignored, skip this iteration
                if(curNode == ignoreS && neighbor == ignoreE){
                    continue;
                }
                Path new_path = curPath;
                new_path.add(neighbor);
                //new priority is heuristic+path length
                double newLength = length+graph.getEdge(curNode, neighbor)->cost;
                double heuristics = graph.getCrowFlyDistance(neighbor, end)/graph.getMaxRoadSpeed(); 
                double newPriority = newLength+heuristics;
                //map new path to new length
                path2length[new_path] = newLength;
                path_pq.enqueue(new_path, newPriority);
                neighbor->setColor(YELLOW);
            }
        }
    }
    return emptyPath;
}
/*implementation of aStar algorithm*/
Path aStar(RoadGraph& graph, Vertex* start, Vertex* end) {
    double costHolder;
    return aStarHelper(graph, start, end, NULL, NULL, costHolder);
}
/*helper function for computing path difference*/
double difference(HashSet<Vertex*>& bestNodes, Path& P){
    if(bestNodes.size() == 0)
        return 0;

    double count = 0;
    for(Vertex* node : P){
        if(! bestNodes.contains(node)){
            count+=1;
        }
    }
    return count/bestNodes.size();
}

/*implementation for computing alternative path*/
Path alternativeRoute(RoadGraph& graph, Vertex* start, Vertex* end) {
    //get shortest path
    Path shortestPath =  aStar(graph, start, end);
    //use hashset to store all the nodes in shortest path
    HashSet<Vertex*> bestNodes;
    for(Vertex* bestNode: shortestPath){
        bestNodes.add(bestNode);
    }
    //find the least costly paths that are sufficiently different witht  the shortest path
    double costHolder1 = 0;
    double costHolder2 = 0;
    Path alternative = aStarHelper(graph, start, end, shortestPath[0], shortestPath[1], costHolder1);
    //iterate through all the possible alternative paths:
    for(int i = 1; i < shortestPath.size()-1; ++i){
        Path memberPath = aStarHelper(graph, start, end, shortestPath[i], shortestPath[i+1],costHolder2);
        
        if(difference(bestNodes, memberPath) > SUFFICIENT_DIFFERENCE){
            if(costHolder2 < costHolder1){
                alternative = memberPath;
                costHolder1 = costHolder2;
            }
        }
    }
    return alternative;
}