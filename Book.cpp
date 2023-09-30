#include <string>
using namespace std;

class Book{
    public:
        string BookTitle;
        string Author;
        string ISBN;
        int Quantity;
        int Available;
        int Price;

        Book(string booktitle="", string author="", string isbn="", int price=0,int available=0, int quantity=0){
            BookTitle = booktitle;
            Author = author;
            ISBN = isbn;
            Price = price;
            Available = available;
            Quantity = quantity;
        }

        void setBookTitle(string booktitle){
            BookTitle = booktitle;
        }
        void setAuthor(string author){
            Author = author;
        }
        void setISBN(string isbn){
            ISBN = isbn;
        }
        void setPrice(int price){
            Price = price;
        }
        void setAvailable(int available){
            Available = available;
        }
        void setQuantity(int quantity){
            Quantity = quantity;
        }

        string getBookName() const { return BookTitle; }
        string getAuthor() const { return Author; }
        string getISBN() const { return ISBN; }
        double getPrice() const { return Price; }
        int getAvailable() const { return Available; }
        int getQuantity() const { return Quantity; }
        
        void Display(){
            cout << "Book Title: " << BookTitle << endl;
            cout << "Author: " << Author << endl;
            cout << "ISBN: " << ISBN << endl;
            cout << "Price: " << Price << endl;
            cout << "Available: " << Available << endl;
            cout << "Quantity: " << Quantity << endl;
        }

};