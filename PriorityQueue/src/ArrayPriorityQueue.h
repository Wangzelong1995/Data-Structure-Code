/*this is the array priority queue class definition, including member functions and variables*/
/*this was done by Yue Liu and Jiyuan Cui*/
#ifndef _arraypriorityqueue_h
#define _arraypriorityqueue_h

#include <iostream>
#include <string>
#include "PQEntry.h"
using namespace std;

class ArrayPriorityQueue {
public:
    ArrayPriorityQueue();
    ~ArrayPriorityQueue();
    void changePriority(string value, int newPriority);
    void clear();
    //dequeue running time: O(n)
    string dequeue();
    //enqueue running time: O(1)
    void enqueue(string value, int priority);
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    friend ostream& operator <<(ostream& out, const ArrayPriorityQueue& queue);

private:
    PQEntry *arrayHead;
    int capacity = 5;
    int SIZE;
    int findUrgent() const;
    void shift(int index);

};

#endif
