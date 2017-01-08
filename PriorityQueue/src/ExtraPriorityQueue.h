/*this is the definition of heap priority queue class with extension: copy constructor and operator = */
/*this was done by Yue Liu and Jiyuan Cui*/
#ifndef _extrapriorityqueue_h
#define _extrapriorityqueue_h

#include <iostream>
#include <string>
#include "PQEntry.h"
#include <algorithm>
using namespace std;

/*
 * For documentation of each member, see VectorPriorityQueue.h.
 */
class ExtraPriorityQueue {
public:
    ExtraPriorityQueue();
    ExtraPriorityQueue(const ExtraPriorityQueue& other);
    
    ExtraPriorityQueue& operator=(ExtraPriorityQueue rhs);
    ~ExtraPriorityQueue();
    void changePriority(string value, int newPriority);
    void clear();
    string dequeue();
    void enqueue(string value, int priority);
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    int cap() const;
    PQEntry* head() const;
    void setHead(PQEntry * pt);
    friend ostream& operator <<(ostream& out, const ExtraPriorityQueue& queue);

private:
    PQEntry *arrayHead;
    int capacity = 10;
    int SIZE;
    void heapifyUp(int index);
    void heapifyDown(int index);
    void swap(ExtraPriorityQueue& other);
    
};

#endif
