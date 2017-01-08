/*This is the implementation of huffman tree algorithm with extension 1. It is done by Yue Liu(yuel3) and Jiyuan Cui(jiyuanc)*/
/* Core idea: 

we can write a tree as a vector of 0/1, so one node will only take one bit. for node at position i, its left child should
be at index 2*i, its right child should be at index 2*i+1, here 0 means null pointer, 1 means non-NULL

then we use a queue to store all the characters, one byte per character

suppose a tree is:
                        A
                    B        C
                D      E   F    G
                     H   I
there will be a vector and a queue representing it: 
the vector:
dummy,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0

the queue:
A B C D E F G H I

2. We can the read the vector and queue back to rebuild the encoding tree
*/
#include "encoding.h"
#include "filelib.h"
#include "pqueue.h"
#include <fstream>
#include "bitstream.h"
#include "queue.h"
/*first step: build the frequency table based on the input*/
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    // read chars, increment the frequency
    int c = input.get();
    while(c != -1){
        freqTable[c] += 1;
        c = input.get();
    }
    // add eof sign
    freqTable[PSEUDO_EOF] = 1;

    return freqTable;          
}
/*step 2: build encoding tree*/
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode*> pq;
    // push all the HuffmanNode into the queue
    for(int character : freqTable){
        HuffmanNode* node = new HuffmanNode(character, freqTable[character], NULL, NULL);
        pq.enqueue(node, node->count);
    }

    HuffmanNode* leftNode;
    HuffmanNode* rightNode;
    HuffmanNode* parent;

    while(pq.size() > 1){
        // get the left and right children
        leftNode = pq.dequeue();
        rightNode = pq.dequeue();
        // generate parent and assign pointers
        parent = new HuffmanNode(NOT_A_CHAR, leftNode->count+rightNode->count, leftNode, rightNode);
        //put parent in the queue
        pq.enqueue(parent, parent->count);
    }    
    return pq.dequeue(); 
}
/*helper function for building the encoding map*/
void buildHelper(HuffmanNode* root, Map<int, string> & encodingMap, string currentStr){
    // base case
    if(root == NULL) {
        return;
    }
    if(root->isLeaf()){
        encodingMap[root->character] = currentStr;
    }
    // recursive calls   
    buildHelper(root->zero, encodingMap, currentStr+"0");
    buildHelper(root->one, encodingMap, currentStr+"1");
}
/*step 3: build encoding map*/
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    string startStr = "";

    buildHelper(encodingTree, encodingMap, startStr);
    return encodingMap;             
}
/*step 4: encode data*/
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {

    int character = input.get();
    // read and write all the stuff before EOF
    while(character != -1){
        string encodedData = encodingMap[character];
        for(char c: encodedData){
            output.writeBit(c-'0');
        }
        character = input.get();
    }
    // write EOF
    string value = encodingMap[256];
    for(char c : value){
        output.writeBit(c-'0');
    }
}

/*step 5: decode data*/
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    int bit;
    HuffmanNode* pt = encodingTree;
    
    if(pt == NULL){
        return;
    }
    while(true){
        //if pt is a leaf, write out the character, move the pointer back to root
        if(pt->isLeaf()){
            if(pt->character == PSEUDO_EOF){
                input.clear();
                break;
            }
            output.put(pt->character);
            pt = encodingTree;
        }
        // read one more bit, move the pointer correspondingly
        bit = input.readBit();
        if(bit == 0){
            pt = pt->zero;
        }else if(bit == 1){
            pt = pt->one;
        }
    }
}
/*function to write the encoding tree to the header*/
void writeTree(HuffmanNode* tree, obitstream& output){
    if(tree == NULL){
        return;
    }

    Queue<HuffmanNode*> q;
    Vector<int> bitV;
    Queue<int> charQ;
    // do bfs on the tree
    q.enqueue(tree);

    while(!q.isEmpty()){
        HuffmanNode* front = q.dequeue();
        // if the current node is not null, mark 1 in the vector and push the character to queue
        if(front){
            charQ.add(front->character);
            bitV.add(1);
            q.enqueue(front->zero);
            q.enqueue(front->one);
        }else {
            //else mark it 0
            bitV.add(0);
        }
    }
    output<<bitV;
    output<<charQ;
}

/*execute all the stuff and compress a file*/
void compress(istream& input, obitstream& output) {
    Map<int, int> FrequencyTable = buildFrequencyTable(input);

    HuffmanNode* tree = buildEncodingTree(FrequencyTable);
    Map<int, string> encodingMap = buildEncodingMap(tree);
    rewindStream(input);

    //write header
    writeTree(tree, output);
    encodeData(input, encodingMap, output);
}
/*Function for reading the tree from the header*/
void readTree(ibitstream& input, Vector<HuffmanNode*> nodes){
    Vector<int> bitV;
    Queue<int> charV;
    //read the vector and queue back
    input>>bitV;
    input>>charQ;
    // rebuild the tree nodes
    for(int i = 0; i < bitV.size(); i++){
        if(bitV[i] == 1){
            nodes.add(new HuffmanNode(charQ.dequeue(), -1, NULL, NULL));
        }
    }
    // rearrange the pointers
    for(int i = 1; i < bitV.size() && 2*i+1 < bitV.size(); i++){
        if(nodes[i] != NULL){
            nodes[i]->zero = nodes[2*i];
            nodes[i]->one = nodes[2*i+1];
        }
    }
}

/*execute all the stuff and decompress a file*/
void decompress(ibitstream& input, ostream& output) {
    // read and rebuild tree
    Vector<HuffmanNode*> nodes;
    HuffmanNode* dummy, tree;
    nodes.add(dummy);
    readTree(input, nodes);
    
    if(nodes.size()>1){
        HuffmanNode* tree = nodes[1];
    }
    // decode data    
    decodeData(input, tree, output);
}
/*delete the huffman tree*/
void freeTree(HuffmanNode* node) {
    // base case
    if(node == NULL)
        return;
    if(node->isLeaf()){
        delete node;
    }
    // recursive calls
    freeTree(node->zero);
    freeTree(node->one);
    // set children pointers to null
    node->zero = NULL;
    node->one = NULL;
}
