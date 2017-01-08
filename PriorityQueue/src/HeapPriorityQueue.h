/*this is the heap priority queue class definition, including member functions and variables*/
/*this was done by Yue Liu and Jiyuan Cui*/
#ifndef _heappriorityqueue_h
#define _heappriorityqueue_h

#include <iostream>
#include <string>
#include "PQEntry.h"
using namespace std;

/*
 * For documentation of each member, see VectorPriorityQueue.h.
 */
class HeapPriorityQueue {
public:
    HeapPriorityQueue();
    ~HeapPriorityQueue();
    void changePriority(string value, int newPriority);
    void clear();
    //dequeue running time O(log(n))
    string dequeue();
    //enqueue running time O(log(n))
    void enqueue(string value, int priority);
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    friend ostream& operator <<(ostream& out, const HeapPriorityQueue& queue);

private:
    PQEntry *arrayHead;
    int capacity = 5;
    int SIZE;
    void heapifyUp(int index);
    void heapifyDown(int index);
};

#endif
