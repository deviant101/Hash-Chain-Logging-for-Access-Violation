#include <iostream>
#include <fstream>
#include <string>
#include <termios.h>
#include <unistd.h>

#include "Book.cpp"
#include "Admin.cpp"
using namespace std;

class Library{
    public:
        Book books[100];
        Customer *customers;
        Admin admin;

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
                        string password=maskPassword();
                        // getline(cin,password);
                        if(password==customers[index].getPassword()){
                            cout<<"\nLogged in as "<<customers[index].getName()<<"\n\n";
                            viewUser_Profile(index);
                            // RechargeCredit(index);
                            // Pay_LibraryFine(index);
                            // ReturnBook(index);
                            // BorrowBook(index);
                            changeUserPassword(index);

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

        void loginAsAdmin(){
            cin.ignore();
            while(1){
                cout<<"Enter Username: ";
                string username="";
                getline(cin,username);
                if(username==admin.getUsername()){
                    while(1){
                        cout<<"Enter Password: ";
                        string password=maskPassword();
                        // getline(cin,password);
                        if(password==admin.getPassword()){
                            cout<<"\nLogged in as "<<admin.getName()<<"\n\n";
                            admin.viewProfile();
                            changeAdminPassword();

                            break;
                        }
                        else
                            cout<<"\nInvalid Password!\nTry Again\n"<<endl;
                    }
                    break;
                }
                else
                    cout<<"\nUser not found!\nTry Again\n"<<endl;
            }
        }
        void changeUserPassword(int index){
            string pass="";
            while(1){
                cout<<"Enter old Password: ";
                // getline(cin,pass);
                pass=maskPassword();
                if(pass==customers[index].getPassword()){
                    cout<<"Enter new Password: ";
                    string newpass=maskPassword();
                    if(newpass==customers[index].getPassword()){
                        cout<<"\nCannot use old password!\n"
                            <<"Try again\n\n";
                        continue;
                    }
                    // getline(cin,newpass);
                    customers[index].setPassword(newpass);
                    cout<<"\nPassword Changed!\n\n";
                    break;
                }                    
                else
                    cout<<"\nInvalid Password!\nTry Again\n"<<endl;
            }
        }

        void changeAdminPassword(){
            string pass="";
            while(1){
                cout<<"Enter old Password: ";
                // getline(cin,pass);
                pass=maskPassword();
                if(pass==admin.getPassword()){
                    cout<<"Enter new Password: ";
                    string newpass=maskPassword();
                    if(newpass==admin.getPassword()){
                        cout<<"\nCannot use old password!\n"
                            <<"Try again\n\n";
                        continue;
                    }
                    // getline(cin,newpass);
                    admin.setPassword(newpass);
                    cout<<"\nPassword Changed!\n\n";
                    break;
                }                    
                else
                    cout<<"\nInvalid Password!\nTry Again\n"<<endl;
            }
        }

        void RechargeCredit(int index){
            while(1){
                cout<<"Enter amount to be recharged: ";
                int amount;
                cin>>amount;
                if(amount>=0){
                    customers[index].setBalance(customers[index].getBalance()+amount);
                    return;
                }
                else
                    cout<<"\nInvalid Amount!\nTry Again\n"<<endl;
            }
        }
        
        void Pay_LibraryFine(int index){

            int fine=customers[index].getFine();
            cout<<"Your Current Fine is "<<fine<<endl;
            if(customers[index].getBalance()>=fine){
                customers[index].setBalance(customers[index].getBalance()-fine);
                customers[index].setFine(0);
                cout<<"\nYou have Successfully payed your fine"<<endl;
                cout<<"Your new Credit Balance is: "<<customers[index].getBalance()<<"\n\n";
            }
            else{
                cout<<"\nPayment Failed!\n"
                    <<"Insufficient Balance!\n\n"
                    <<"Would you like to Recharge your credit Balance?(y/n): ";
                char choice;
                cin>>choice;
                if(choice=='y' || choice=='Y'){
                    RechargeCredit(index);
                    Pay_LibraryFine(index);
                }
            }
        }

        void viewUser_Profile(int index){
            cout<<"\t"<<customers[index].getName()<<" Profile"<<"\n\n";
            cout<<"Full Name:\t"<<customers[index].getName()<<endl
                <<"Username:\t"<<customers[index].getUsername()<<endl
                <<"Password:\t"<<customers[index].getPassword()<<endl
                <<"Credit Balance:\t"<<customers[index].getBalance()<<endl
                <<"Library Fine:\t"<<customers[index].getFine()<<endl;
            int bookindex=searchBook_ISBN(customers[index].getISBN());
            string bookname="N/A";
            if(bookindex!=-1)
                bookname=books[bookindex].getBookName();
            cout<<"Book Borrowed:\t"<<bookname<<"\n\n";
        }

        void BorrowBook(int index){
            while(1){
                string isbn="";
                cout<<"Enter Book ISBN: ";
                getline(cin,isbn);
                int bookindex=searchBook_ISBN(isbn);
                if(bookindex!=-1){
                    if(books[bookindex].getAvailable()>0){
                        books[bookindex].setAvailable(books[bookindex].getAvailable()-1);
                        cout<<endl<<books[bookindex].getBookName()<<" Book Borrowed!\n\n";
                        customers[index].setISBN(books[bookindex].getISBN());
                        break;
                    }
                    else{
                        cout<<endl<<books[bookindex].getBookName()<<" is not available!\nTry Again Later\n"<<endl;
                        break;
                    }
                }
                else
                    cout<<"\nInvalid Book ISBN!\nTry Again\n\n";
            }
        }

        void ReturnBook(int index){
                int bookindex=searchBook_ISBN(customers[index].getISBN());
                books[bookindex].setAvailable(books[bookindex].getAvailable()+1);
                cout<<endl<<books[bookindex].getBookName()<<" Book Returned!\n\n";
                customers[index].setISBN("nill");
        }

        int searchBook_ISBN(string str){
            for(int i=0; i<100; i++){
                if(books[i].getISBN()==str)
                    return i;
            }
            return -1;
        }

        int SearchUserName(string u_name){
            for(int i=0; i<150; ++i){
                if(u_name==customers[i].getUsername())
                    return i;
            }
            return -10;
        }
        ////////////////////////////////////////////////////////////////////////
        string maskPassword()
        {
            //masking password with stars *
            char* pass=new char[30];
            string passwd="";
            char ch;
            int z=0,y=0;
            while(1){
                ch=getch();
                if(ch==10){
                    pass[z]+='\0';
                    cout<<"\n";
                    break;
                }
                else if(ch==127){
                    cout<<"\b \b";
                    z--;
                    pass[z]='\0';
                }
                else{
                    cout<<"*";
                    pass[z]=ch;
                    z++;
                }
            }  
            for(int i=0;i<z;i++)
                passwd+=pass[i];
            return passwd;
        }
        char getch()
        {
            int a=0;
            struct termios oter, nter;
            int arr[10];
            char c;
            tcgetattr(STDIN_FILENO, &oter); // Save current terminal settings
            nter = oter;
            for(int i=0;i<10;i++)
            {
                arr[i]=1;
            }
            nter.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
            nter.c_cc[VMIN] = 1; // Wait for at least one character
            if(arr[3]==2)
                arr[3]=1;
            arr[1]=2;
            nter.c_cc[VTIME] = 0; // Disable timeout
            for(int i=0;i<10;i++)
                arr[i]=2;
            tcsetattr(STDIN_FILENO, TCSANOW, &nter); // Apply new terminal settings
            c = getchar(); // Read a single 
            int b=9;
            tcsetattr(STDIN_FILENO, TCSANOW, &oter); // Restore original terminal settings
            return c;
        }
    
};