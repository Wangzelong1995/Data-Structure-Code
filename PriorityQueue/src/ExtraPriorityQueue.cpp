/*this is the implementation of heap priority queue class with extension: copy constructor and operator = were implemented*/
/*this was done by Yue Liu and Jiyuan Cui*/
#include "ExtraPriorityQueue.h"

/*constructor*/
ExtraPriorityQueue::ExtraPriorityQueue() {
    arrayHead = new PQEntry[capacity+1];
    SIZE = 0;
}
/*destrctor*/       
ExtraPriorityQueue::~ExtraPriorityQueue() {
    delete [] arrayHead;
    arrayHead = NULL;
}
/*change priority of elements in the queue*/
void ExtraPriorityQueue::changePriority(string value, int newPriority) {
    int i = 1;

    while(i <= SIZE){
        // if the value is found
        if(arrayHead[i].value == value){
            // if the newPriority is more urgent, then set the new priority and keep swapping up
            if(arrayHead[i].priority >= newPriority){
                arrayHead[i].priority = newPriority;
                heapifyUp(i);
                return;
            }
            //else throw an exception
            else{
                throw "Invalud new priority value";
            }
        }
        ++i;
    }
    //if not found, throw an exception
    if(i > SIZE){
        throw "Value not found in the queue";
    }
}
/*clear the hidden array*/
void ExtraPriorityQueue::clear() {
    delete[] arrayHead;
    capacity = 10;
    SIZE = 0;
    arrayHead = new PQEntry[capacity+1];
}
/*remove and return the most urgent element*/
string ExtraPriorityQueue::dequeue() {
    //if empty, throw an exception
    if(SIZE == 0){
        throw "Invalid operation: dequeue an empty queue";
    }
    //put last element to the root and swapping down if needed
    string ret = arrayHead[1].value;
    arrayHead[1] = arrayHead[SIZE];
    // update size
    --SIZE;
    heapifyDown(1);
    return ret;
}
/*helper function for swapping up elements from index*/
void ExtraPriorityQueue::heapifyUp(int index){
    PQEntry temp;
    int parent = index/2;
    // if the parent is less urgent, swap itself with its parent
    while(arrayHead[index] < arrayHead[parent] && parent > 0){
        temp = arrayHead[index];
        arrayHead[index] = arrayHead[parent];
        arrayHead[parent] = temp;
        // update
        index = parent;
        parent = index/2;
    }
}
/*helper function for swapping down elements from index*/
void ExtraPriorityQueue::heapifyDown(int index){
    PQEntry temp;

    int left = index*2;
    int right = index*2+1;
    // if anyone of the children is more urgent than current one, swap down
    while((arrayHead[index] > arrayHead[left] || arrayHead[index] > arrayHead[right]) && 
        (right<=SIZE && left <= SIZE)){
        // if left child is more urgent, swap with left
        if(arrayHead[left] < arrayHead[right]){
            temp = arrayHead[index];
            arrayHead[index] = arrayHead[left];
            arrayHead[left] = temp;
            index = left;
        }
        else{ // if right child is more urgent, swap with right
            temp = arrayHead[index];
            arrayHead[index] = arrayHead[right];
            arrayHead[right] = temp;
            index = right;
        }
        // update
        left = index*2;
        right = index*2+1;
    }
}
/*push stuff into the queue*/
void ExtraPriorityQueue::enqueue(string value, int priority) {
    PQEntry toAppend(value, priority);
    //if not over the capacity, append to the tail
    if(SIZE < capacity){
        arrayHead[SIZE+1] = toAppend;
    }
    else{// else double the capacity, copy over the element, delete the old one
        capacity = 2*capacity;
        PQEntry *newHead = new PQEntry[capacity+1];
        for(int i = 1; i <= SIZE; i++){
            newHead[i] = arrayHead[i];
        }
        newHead[SIZE+1] = toAppend;
        delete [] arrayHead;
        arrayHead = newHead;
    }
    ++SIZE;
    // swap up if needed
    heapifyUp(SIZE);
}
/*check if the queue is empty*/
bool ExtraPriorityQueue::isEmpty() const {
    return SIZE == 0;
}
/*return the value of most urgent element*/
string ExtraPriorityQueue::peek() const {
    if(SIZE == 0){
        throw "Invalid operation for empty queue";
    }
    return arrayHead[1].value;
}
/*return the priority of most urgent element*/
int ExtraPriorityQueue::peekPriority() const {
    if(SIZE == 0){
        throw "Invalid operation for empty queue";
    }
    return arrayHead[1].priority;
}

/*return the size of queue*/
int ExtraPriorityQueue::size() const {
    return SIZE;
}
/*print out all the elements in the queue*/
ostream& operator<<(ostream& out, const ExtraPriorityQueue& queue) {
    if(queue.SIZE == 0){
        out<<"{}";
        return out;
    }
    out<<"{";
    for(int i = 1; i < queue.SIZE; ++i){
        out<<queue.arrayHead[i]<<", ";
    }

    out<<queue.arrayHead[queue.SIZE]<<"}";
    return out;
}

/*helper class for getting the capacity*/
int ExtraPriorityQueue::cap() const{
    return capacity;
}
/*helper class for getting the array pointer*/
PQEntry* ExtraPriorityQueue::head() const{
    return arrayHead;
}
/*helper function for setting the array pointer*/
void ExtraPriorityQueue::setHead(PQEntry * pt){
    arrayHead = pt;    
}
/*helper function for swap two ExtraPriorityQueue objects*/
void ExtraPriorityQueue::swap(ExtraPriorityQueue& other){
    
    SIZE = other.size();
    capacity = other.cap();

    PQEntry * temp = arrayHead;
    arrayHead = other.head();
    other.setHead(temp);
}
/*copy constructor*/
ExtraPriorityQueue::ExtraPriorityQueue(const ExtraPriorityQueue & other){
    SIZE = other.size();
    capacity = other.cap();
    PQEntry* temp = other.head();
    arrayHead = new PQEntry[capacity+1];
    for(int i = 1; i <= SIZE; ++i){
        arrayHead[i] = temp[i];
    }
}
/*operator = using the "copy and swap" method*/    
ExtraPriorityQueue& ExtraPriorityQueue::operator=(ExtraPriorityQueue rhs){
    this->swap(rhs);
    return *this;
}
