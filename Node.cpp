#include <iostream>
using namespace std;

class Node{
    public:
        string Block;
        Node* Next;

        Node(){
            Block = "";
            Next = NULL;
        }
};