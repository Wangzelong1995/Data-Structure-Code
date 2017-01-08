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

#pragma once

#include "set.h"
#include "basicgraph.h"
#include "point.h"

class RoadGraph {
public:
    /*
     * Makes a new RoadGraph based on BasicGraph data
     */
    RoadGraph(BasicGraph * data);

    /*
     * Returns all of the verticies that can be reached by a
     * a direct path from v.
     */
    Set<Vertex *> getNeighbors(Vertex * v);

    /*
     * Returns a pointer to the edge that connects start to end
     */
    Edge * getEdge(Vertex * start, Vertex * end);

    /*
     * Returns the maximum speed of any edge on the road graph.
     */
    double getMaxRoadSpeed();

    /*
     * The geodesic distance between the two nodes. This is the
     * distance that a crow would travel if it flew from start to
     * end. Units are not standarized, but are consistent within
     * a graph.
     */
    double getCrowFlyDistance(Vertex * start, Vertex * end);

    /*
     * Reset the "touched node count" and the node colorings. This
     * is called for you before your search algorithms execute.
     */
    void resetAnnotations();
private:

    /*
     * The number of pixels that separate a and b
     */
    double pointDistance(Point * a, Point * b);

    // underlying data
    BasicGraph * data;

    // the saved max rate of the graph
    double maxRate;
    bool maxRateCached;
};
