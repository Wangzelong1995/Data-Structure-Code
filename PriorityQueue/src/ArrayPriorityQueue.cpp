/*this is the implementation of array priority queue class*/
/*this was done by Yue Liu and Jiyuan Cui*/
#include "ArrayPriorityQueue.h"
/*constructor*/
ArrayPriorityQueue::ArrayPriorityQueue() {
    SIZE = 0;
    arrayHead = new PQEntry[capacity];
}
/*destrctor*/  
ArrayPriorityQueue::~ArrayPriorityQueue() {
    delete [] arrayHead;
    arrayHead = NULL;
}
/*change priority of elements in the queue*/
void ArrayPriorityQueue::changePriority(string value, int newPriority) {
    int i = 0;
    while(i < SIZE){
        // if the value is found
        if(arrayHead[i].value == value){
            //if the newPriority is more urgent, then set the new priority
            if(arrayHead[i].priority >= newPriority){
                arrayHead[i].priority = newPriority;
                return;
            }//else throw an exception
            else{
                throw "Invalid new priority value";
            }
        }
        i++;
    }
    //if not found, throw an exception
    if(i == SIZE){
        throw "Value not found in the queue";
    }
    
}
/*clear the hidden array*/
void ArrayPriorityQueue::clear() {
    delete [] arrayHead;
    capacity = 10;
    SIZE = 0;
    arrayHead = new PQEntry[capacity];
}
/*helper function for finding the most urgent index*/
int ArrayPriorityQueue::findUrgent() const{
    int ret = 0;
    
    PQEntry toReturn = arrayHead[ret];
    
    for(int i = 1; i < SIZE; i++){
        if(arrayHead[i] < toReturn){
            ret = i;
            toReturn = arrayHead[ret];
        }
    }
    return ret;
}
/*helper function for shifting elements with index+1 and so on after removing the most urgent element*/
void ArrayPriorityQueue::shift(int index){
    for(int i = index+1; i < SIZE; ++i){
        arrayHead[i-1] = arrayHead[i];
    }
}
/*remove and return the most urgent element*/
string ArrayPriorityQueue::dequeue() {
    if(SIZE == 0){
        throw "Try to dequeue an empty queue";
    }
    // find the most urgent index
    int urgentIndex = findUrgent();
    PQEntry toReturn = arrayHead[urgentIndex];
    // shift all the elements behind the urgent element
    shift(urgentIndex);
    --SIZE;

    return toReturn.value;
}
/*push stuff into the queue*/
void ArrayPriorityQueue::enqueue(string value, int priority) {
    PQEntry toAppend(value, priority);
    //if not over the capacity, append to the tail
    if(SIZE < capacity){
        arrayHead[SIZE] = toAppend;
    }
    else{// else double the capacity, copy over the element, delete the old one
        capacity = 2*capacity;
        PQEntry *newHead = new PQEntry[capacity];
        for(int i = 0; i < SIZE; i++){
            newHead[i] = arrayHead[i];
        }
        newHead[SIZE] = toAppend;
        delete [] arrayHead;
        arrayHead = newHead;
    }
    //update size
    ++SIZE;
}
/*check if the queue is empty*/
bool ArrayPriorityQueue::isEmpty() const {
    return SIZE == 0;
}
/*return the value of most urgent element*/
string ArrayPriorityQueue::peek() const {
    if(SIZE == 0){
        throw "Invalid operation for empty queue";
    }
    int urgentIndex = findUrgent();
    return arrayHead[urgentIndex].value;
}
/*return the priority of most urgent element*/
int ArrayPriorityQueue::peekPriority() const {
    if(SIZE == 0){
        throw "Invalid operation for empty queue";
    }
    int urgentIndex = findUrgent();
    return arrayHead[urgentIndex].priority;
}
/*return the size of queue*/
int ArrayPriorityQueue::size() const {
    return SIZE;
}
/*print out all the elements in the queue*/
ostream& operator<<(ostream& out, const ArrayPriorityQueue& queue) {
    if(queue.SIZE == 0){
        out<<"{}";
        return out;
    }
    out<<"{";
    for(int i = 0; i < queue.SIZE-1; ++i){
        out<<queue.arrayHead[i]<<", ";
    }
    out<<queue.arrayHead[queue.SIZE-1]<<"}";
    return out;
}
