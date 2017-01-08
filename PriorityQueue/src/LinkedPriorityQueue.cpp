/*this is the implementation of array priority queue class*/
/*this was done by Yue Liu and Jiyuan Cui*/
#include "LinkedPriorityQueue.h"
/*constructor*/
LinkedPriorityQueue::LinkedPriorityQueue() {
    front = NULL;
}
/*destrctor*/ 
LinkedPriorityQueue::~LinkedPriorityQueue() {
    clear();
}
// rearrange the linked list after changing the priority
void LinkedPriorityQueue::rearrange(ListNode * pt){
    // prev1 is the position which is right before the position the node should be
    ListNode *prev1 = findPos(pt);
    // prev1 is the position which is right before the position the node is at
    ListNode *prev2 = front;

    //if current node is already at the front, after the change it should still be at the front
    // if the current is already at the place it should be, do nothing
    if((prev1->next == pt && *prev1 < *pt) || pt == front){
        return;
    }
    //put prev2 to the place it should be
    while(prev2->next != pt){
        prev2 = prev2->next;
    }
    //post1 is the place that current node.next should point to
    ListNode *post1 = prev1->next;
    //post2 is the place that current node.next is pointing to
    ListNode *post2 = pt->next;
    
    //if after change, the node should be the front(but it is not know)
    if(prev1 == front){
        if(*pt < *front){
            pt->next = prev1;
            prev2->next = post2;
            front = pt;
            return;
        }
    }
    //if not the above case 
    prev1->next = pt;
    pt->next = post1;
    prev2->next = post2;
}
/*change priority of elements in the queue*/
void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    ListNode *pt = front;
    while(pt != NULL){
        // if the value is found
        if(pt->value == value){
            //if the newPriority is more urgent, then set the new priority, rearrage the linked list
            if(pt->priority >= newPriority){
                pt->priority = newPriority;
                rearrange(pt);
                return;
            }//otherwise throw an exception
            else{
                throw "Invalid new priority value";
            }
        }
        pt = pt->next;
    }
    //if not found, throw exception
    if(pt == NULL) {
        throw "Value not found";
    }
}
/*delete all the stuff*/
void LinkedPriorityQueue::clear() {
    ListNode *temp = front;
    ListNode *temp2;
    while(temp != NULL){
        temp2 = temp->next;
        delete temp;
        temp = temp2;
    }
    //set front to null
    front = NULL;
}
/*remove and return the most urgent element*/
string LinkedPriorityQueue::dequeue() {
    if(front == NULL){
        throw "Dequeue an empty queue";
    }
    // return and delete the front node, repoint front to current second
    ListNode *temp = front->next;
    string ret = front->value;
    delete front;
    front = temp;
    return ret;
}
/*find the position that is right before toInsert, considering priority*/
ListNode* LinkedPriorityQueue::findPos(ListNode *toInsert) const{
    ListNode *pt = front;
    while(pt){
        //if front is null, return the front
        if(pt->next == NULL){
            break;
        }
        if(*(pt->next) >= *toInsert){
            break;
        }
        pt = pt->next;
    }
    return pt;
}
/*push stuff into the queue*/
void LinkedPriorityQueue::enqueue(string value, int priority) {
    //new node to be inserted into the list
    ListNode *toInsert = new ListNode(value, priority);
    //find the postion to help insert the new node
    ListNode *insertPos = findPos(toInsert);
    if (insertPos)
    {
        // if the new node should be inserted as the new front
        if(insertPos == front){
            if(*toInsert < *front){
                toInsert->next = insertPos;
                front = toInsert;
                return;
            }
        }
        //if not, normal insertion work
        ListNode *post = insertPos->next;
        insertPos->next = toInsert;
        toInsert->next = post;
    }//if the list is empty, point front to the new node
    else {
        front = toInsert;
    }

}
/*check if the queue is empty*/
bool LinkedPriorityQueue::isEmpty() const {
    return front == NULL; 
}
/*return the value of most urgent element*/
string LinkedPriorityQueue::peek() const {
    
    return front->value;
}
/*return the priority of most urgent element*/
int LinkedPriorityQueue::peekPriority() const {
    
    return front->priority;   
}
/*return the size of queue*/
int LinkedPriorityQueue::size() const {
    int count = 0;
    ListNode *pt = front;
    // loop the list and increment the counter
    while(pt != NULL){
        pt = pt->next;
        ++count;
    }
    return count;
}
/*print out all the elements in the queue*/
ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    ListNode *pt = queue.front;
    if(pt == NULL){
        out<<"{}";
        return out;
    }
    out<<"{";
    while(pt->next!=NULL){
        out<<*pt<<", ";
        pt = pt->next;
    }
    out<<*pt<<"}";
    
    return out;
}
