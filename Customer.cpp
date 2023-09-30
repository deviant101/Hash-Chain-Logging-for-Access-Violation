#include <iostream>
#include "User.cpp"
using namespace std;

class Customer : public User{
    public:
        int Fine;
        int Balance;
        string ISBN;

        Customer(string name="", string username="", string password="", string isbn="",int fine=0, int balance=0):User(name, username, password){
            Fine = fine;
            Balance = balance;
            ISBN = isbn;
        }

        void setFine(int fine){
            Fine = fine;
        }
        void setBalance(int balance){
            Balance = balance;
        }
        void setISBN(string isbn){
            ISBN = isbn;
        }

        int getFine() const { return Fine; }
        int getBalance() const { return Balance; }
        string getISBN() const { return ISBN; }

        void Display() const{
            cout << "Name: " << Name << endl;
            cout << "Username: " << Username << endl;
            cout << "Password: " << Passwords << endl;
            cout << "Fine: " << Fine << endl;
            cout << "Balance: " << Balance << endl;
            cout << "ISBN: " << ISBN << endl;
        }
};