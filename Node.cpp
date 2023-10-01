#include <iostream>
using namespace std;

class Node{
    public:
        string Block;
        Node* Next;
        Node* Prev;

        Node(){
            Block = "";
            Next = nullptr;
            Prev = nullptr;
        }
};