/*
 * CS 106B Trailblazer
 * This file declares the graph type used in trailblazer
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @author Chris Piech
 * @version 2016/11/30 (initial version for 14au)
 */


#include "RoadGraph.h"
#include "point.h"
#include <math.h>

/*
 * Makes a new RoadGraph based on BasicGraph data
 */
RoadGraph::RoadGraph(BasicGraph *data) {
    this->data = data;
    maxRateCached = false;
}


/*
 * Returns all of the verticies that can be reached by a
 * a direct path from v.
 */
Set<Vertex *> RoadGraph::getNeighbors(Vertex* v) {
    return data->getNeighbors(v);
}

/*
 * Returns a pointer to the edge that connects start to end
 */
Edge * RoadGraph::getEdge(Vertex *start, Vertex *end) {
    return data->getEdge(start, end);
}

/*
 * The geodesic distance between the two nodes. This is the
 * distance that a crow would travel if it flew from start to
 * end. Units are not standarized, but are consistent within
 * a graph.
 */
double RoadGraph::getCrowFlyDistance(Vertex *start, Vertex *end) {
    Point* a = (Point*) start->extraData;
    Point* b = (Point*) end->extraData;
    return pointDistance(a, b);
}

/*
 * Returns the maximum speed of any edge on the road graph.
 */
double RoadGraph::getMaxRoadSpeed() {
    if(maxRateCached) return maxRate;
    maxRate = 0;
    for(Edge * edge : data->getEdgeSet()) {
        double time = edge->cost;
        Point* a = (Point*) edge->start->extraData;
        Point* b = (Point*) edge->end->extraData;
        double dist = pointDistance(a, b) - 1;
        if(dist <= 3) continue;
        double rate = dist / time;
        if(rate > maxRate) {
            maxRate = rate;
        }
    }
    maxRateCached = true;
    return maxRate;
}

/*
 * The number of pixels that separate a and b
 */
double RoadGraph::pointDistance(Point *a, Point *b){
    double dx = a->getX() - b->getX();
    double dy = a->getY() - b->getY();
    return fmax(sqrt(dx * dx + dy * dy), 0) - 1;
}

/*
 * Reset the "touched node count" and the node colorings. This
 * is called for you before your search algorithms execute.
 */
void RoadGraph::resetAnnotations() {
    data->resetData();
}
