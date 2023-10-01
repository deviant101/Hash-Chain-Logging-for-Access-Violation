#include "Node.cpp"
using namespace std;

class Chain{
    public:
        Node *Head;
        Node *Tail;

        Chain(){
            Head = nullptr;
            Tail = nullptr;
        }

        void append(string data){
            Node *temp = new Node;
            temp->Block = data;
            if(Head == nullptr){
                Head = temp;
                Tail = temp;
            }
            else{
                Tail->Next = temp;
                Tail = temp;
            }
        }
        void printLast(){
            cout<<Tail->Block<<endl;
        }



};