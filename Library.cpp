#include <iostream>
#include <fstream>
#include <string>
#include "Book.cpp"
using namespace std;

class Library{
    public:
        Book books[100];
        Customer *customers;

        Library():customers(nullptr){
            fstream booksfile;
            booksfile.open("books.csv", ios::in);
            if(booksfile){
                string data="";
                getline(booksfile,data);    //for ignoring labels line

                for(int obj_no=0; obj_no < 100; obj_no++){
                    data="";
                    getline(booksfile,data);
                    string str="";
                    int comma_count=0;

                    for(int i=0; i<data.length(); i++){
                        if(data[i]!=',' && i!=data.length()-1)  //because length function is giving 1 extra length when reading from file
                            str+=data[i];
                        else{
                            // cout<<comma_count<<"|"<<str<<endl;
                            if(comma_count==0){
                                books[obj_no].setBookTitle(str);
                            }
                            else if(comma_count==1){
                                books[obj_no].setAuthor(str);
                            }
                            else if(comma_count==2){
                                books[obj_no].setISBN(str);
                            }
                            else if(comma_count==3){
                                books[obj_no].setQuantity(stoi(str));
                            }
                            else if(comma_count==4){
                                books[obj_no].setPrice(stoi(str));
                            }
                            else if(comma_count==5){
                                books[obj_no].setAvailable(stoi(str));
                            }
                            ++comma_count;
                            str="";
                        }
                    }
                }
                // books[99].Display();
                booksfile.close();
            }
            else{
                cout<<"File not found!"<<endl;
            }
        }

        void getCustomers_ptr(Customer *Customers){
            customers=Customers;
        }

        void loginAsUser(){
            cin.ignore();
            int index=-10;
            while(index==-10){
                cout<<"Enter Username: ";
                string username;
                getline(cin,username);
                index=SearchUserName(username);
                if(index!=-10){
                    while(1){
                        cout<<"Enter Password: ";
                        string password;
                        getline(cin,password);
                        if(password==customers[index].getPassword()){
                            cout<<"\nLogged in as "<<customers[index].getName()<<"\n\n";
                            break;
                        }
                        else
                            cout<<"\nInvalid Password!\nTry Again\n"<<endl;
                    }
                }
                else
                    cout<<"\nUser not found!\nTry Again\n"<<endl;
            }
        }

        int SearchUserName(string u_name){
            for(int i=0; i<150; ++i){
                if(u_name==customers[i].getUsername())
                    return i;
            }
            return -10;
        }
    
};