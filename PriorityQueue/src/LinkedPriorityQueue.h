/*this is the linked list priority queue class definition, including member functions and variables*/
/*this was done by Yue Liu and Jiyuan Cui*/
#ifndef _linkedpriorityqueue_h
#define _linkedpriorityqueue_h

#include <cstddef>    // for NULL
#include <iostream>
#include <string>
#include "ListNode.h"
#include "PQEntry.h"
using namespace std;

/*
 * For documentation of each member, see VectorPriorityQueue.h.
 */
class LinkedPriorityQueue {
public:
    LinkedPriorityQueue();
    ~LinkedPriorityQueue();
    void changePriority(string value, int newPriority);
    void clear();
    //dequeue running time O(1)
    string dequeue();
    //enqueue running time O(n)
    void enqueue(string value, int priority);
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    friend ostream& operator <<(ostream& out, const LinkedPriorityQueue& queue);

private:
    ListNode *front;
    void rearrange(ListNode * pt);
    ListNode* findPos(ListNode *toInsert) const;

};

#endif
