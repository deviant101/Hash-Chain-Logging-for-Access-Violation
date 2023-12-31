#include <iostream>
#include <fstream>
#include <string>
#include <termios.h>
#include <unistd.h>

#include "Book.cpp"
#include "Admin.cpp"
#include "sha256.cpp"
#include "Chain.cpp"
using namespace std;

const string logBackup="./Backup/LogsBackup.csv";
const string hashBackup="./Backup/HashesBackup.csv";

class Library{
    public:
        Book books[100];
        Customer *customers;
        Admin admin;
        fstream logfileStream;
        fstream HashfileStream;
        Chain hashChain;
        Chain logChain;
        

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
            else
                cout<<"File not found!"<<endl;

            //Loading Logs
            logfileStream.open("Logs.csv",ios::in | ios::app);
            if(logfileStream){
                while(1){
                    string data="";
                    getline(logfileStream,data);
                    if(logfileStream.eof())
                        break;
                    logChain.append(data);
                }
                logfileStream.close();
            }
            else
                cout<<"File not found!"<<endl;

            //Loading Hashes
            HashfileStream.open("Hash.csv",ios::in | ios::app);
            if(HashfileStream){
                while(1){
                    string data="";
                    getline(HashfileStream,data);
                    if(HashfileStream.eof())
                        break;
                    hashChain.append(data);
                }
                HashfileStream.close();
            }
            else
                cout<<"File not found!"<<endl;

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
                            create_Log(customers[index].getName(),"Log in","Logged in Successfully");
                            cout<<"\nLogged in as "<<customers[index].getName()<<"\n\n";
                            while(1){
                                cout<<"\n1 - View Profile"
                                    <<"\n2 - Recharge Credit"
                                    <<"\n3 - Pay Library Fine"
                                    <<"\n4 - Return Book"
                                    <<"\n5 - Borrow Book"
                                    <<"\n6 - Change Password"
                                    <<"\n7 - Log Out"
                                    <<"\nSelect any Option: ";
                                int choice=0;
                                cin>>choice;
                                cin.ignore();
                                if(choice==1){
                                    viewUser_Profile(index);
                                }
                                else if(choice==2){
                                    RechargeCredit(index);
                                }
                                else if(choice==3){
                                    Pay_LibraryFine(index);
                                }
                                else if(choice==4){
                                    ReturnBook(index);
                                }
                                else if(choice==5){
                                    BorrowBook(index);
                                }
                                else if(choice==6){
                                    changeUserPassword(index);
                                }
                                else if(choice==7){
                                    create_Log(customers[index].getName(),"Logging Out","Logged Out Successfully");
                                    break;
                                }
                                else
                                    cout<<"Invalid! Try again!"<<endl;
                            
                            // cout<<"Hash: "<<hashChain.Tail->Prev->Block<<endl;
                            // logChain.printLast();
                            // cout<<"Log: "<<logChain.Tail->Prev->Block<<endl;
                            // hashChain.printLast();
                            }

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
                            // changeAdminPassword();
                            // viewUserProfile_as_Admin();
                            // cutFine();
                            Add_more_books_stock();

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
                        create_Log(customers[index].getName(),"Password Change Attempt","Old Password");
                        continue;
                    }
                    // getline(cin,newpass);
                    customers[index].setPassword(newpass);
                    cout<<"\nPassword Changed!\n\n";
                    create_Log(customers[index].getName(),"Password Change Attempt","Password Changed Successfully");
                    break;
                }                    
                else{
                    cout<<"\nInvalid Password!\nTry Again\n"<<endl;
                    create_Log(customers[index].getName(),"Password Change Attempt","Invalid Old Password!");
                }
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

        void viewUserProfile_as_Admin(){
            int index=-10;
            while(index==-10){
                string username="";
                cout<<"Enter username to Search: ";
                getline(cin,username);
                index=SearchUserName(username);
                if(index!=-10){
                    cout<<"\n\t"<<customers[index].getName()<<" Profile"<<"\n\n";
                    cout<<"Full Name:\t"<<customers[index].getName()<<endl
                        <<"Username:\t"<<customers[index].getUsername()<<endl
                        <<"Credit Balance:\t"<<customers[index].getBalance()<<endl
                        <<"Library Fine:\t"<<customers[index].getFine()<<endl;
                    int bookindex=searchBook_ISBN(customers[index].getISBN());
                    string bookname="N/A";
                    if(bookindex!=-10)
                        bookname=books[bookindex].getBookName();
                    cout<<"Book Borrowed:\t"<<bookname<<"\n\n";
                }
                else
                    cout<<"\nUser not found!\nTry Again\n"<<endl;
            }
        }

        void cutFine(){
            int index=-10;
            while(index==-10){
                string username="";
                cout<<"Enter username to Search: ";
                getline(cin,username);
                index=SearchUserName(username);
                if(index!=-10){
                    cout<<customers[index].getName()<<" Library Fine:\t"<<customers[index].getFine()<<endl;
                    if(customers[index].getBalance()>=customers[index].getFine()){
                        customers[index].setBalance(customers[index].getBalance()-customers[index].getFine());
                        customers[index].setFine(0);
                        cout<<"\nFine Cleared!\n\n";
                    }
                    else
                        cout<<"\nInsufficient Balance!\n\n";
                }
                else
                    cout<<"\nUser not found!\nTry Again\n"<<endl;
            }
        }

        void Add_more_books_stock(){
            string isbn="";
            int index=-10;
            while(index==-10){
                cout<<"Enter ISBN: ";
                getline(cin,isbn);
                index=searchBook_ISBN(isbn);
                if(index!=-10){
                    while(1){
                        int book_no=0;
                        cout<<"\nHow many more "<<books[index].getBookName()<<" books to add in availability stock: ";
                        cin>>book_no;
                        if(book_no>0){
                            books[index].setAvailable(books[index].getAvailable()+book_no);
                            books[index].setQuantity(books[index].getQuantity()+book_no);
                            cout<<"\nAdded "<<book_no<<" more "<<books[index].getBookName()<<" books in stock!\n"
                                <<"Total Availability: "<<books[index].getAvailable()<<" books in stock\n\n";
                            break;
                        }
                        else
                            cout<<"\nInvalid Input!\nTry again\n"<<endl;
                    }
                }
                else
                    cout<<"\nBook not found!\nTry again\n"<<endl;
            }
        }

        void RechargeCredit(int index){
            while(1){
                cout<<"Enter amount to be recharged: ";
                int amount;
                cin>>amount;
                if(amount>=0){
                    customers[index].setBalance(customers[index].getBalance()+amount);
                    create_Log(customers[index].getName(),"Credit Recharge","Successfully Recharged");
                    return;
                }
                else{
                    cout<<"\nInvalid Amount!\nTry Again\n"<<endl;
                    create_Log(customers[index].getName(),"Credit Recharge","Invalid Amount!");
                }
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
                create_Log(customers[index].getName(),"Fine Payment","Fine Payed Successfully");
            }
            else{
                cout<<"\nPayment Failed!\n"
                    <<"Insufficient Balance!\n\n"
                    <<"Would you like to Recharge your credit Balance?(y/n): ";
                create_Log(customers[index].getName(),"Fine Payment","Payment Failed");
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
            if(bookindex!=-10)
                bookname=books[bookindex].getBookName();
            cout<<"Book Borrowed:\t"<<bookname<<"\n\n";

            create_Log(customers[index].getName(),"Viewed Profile Details","Successfully Viewded Profile Details");

        }

        void BorrowBook(int index){
            while(1){
                string isbn="";
                cout<<"Enter Book ISBN: ";
                getline(cin,isbn);
                int bookindex=searchBook_ISBN(isbn);
                if(bookindex!=-10){
                    if(books[bookindex].getAvailable()>0){
                        books[bookindex].setAvailable(books[bookindex].getAvailable()-1);
                        cout<<endl<<books[bookindex].getBookName()<<" Book Borrowed!\n\n";
                        customers[index].setISBN(books[bookindex].getISBN());
                        create_Log(customers[index].getName(),"Borrow Book","Book Borrowed Successfully");
                        break;
                    }
                    else{
                        cout<<endl<<books[bookindex].getBookName()<<" is not available!\nTry Again Later\n"<<endl;
                        create_Log(customers[index].getName(),"Borrow Book","Book not Available");
                        break;
                    }
                }
                else
                    cout<<"\nInvalid Book ISBN!\nTry Again\n\n";
                    create_Log(customers[index].getName(),"Borrow Book","Invalid Book ISBN!");
                    
            }
        }

        void ReturnBook(int index){
                int bookindex=searchBook_ISBN(customers[index].getISBN());
                books[bookindex].setAvailable(books[bookindex].getAvailable()+1);
                cout<<endl<<books[bookindex].getBookName()<<" Book Returned!\n\n";
                customers[index].setISBN("nill");
                create_Log(customers[index].getName(),"Return Boook","Successfully Returned");
        }

        int searchBook_ISBN(string str){
            for(int i=0; i<100; i++){
                if(books[i].getISBN()==str)
                    return i;
            }
            return -10;
        }

        int SearchUserName(string u_name){
            for(int i=0; i<150; ++i){
                if(u_name==customers[i].getUsername())
                    return i;
            }
            return -10;
        }

        void create_Log(string user, string action, string result){
            
            time_t currentTime;
            time(&currentTime);
            string currentTimeString = ctime(&currentTime);
            string log="Time: "+copyTime(currentTimeString)+", User: "+user+", Requested Action: "+action+", Result: "+result;

            logfileStream.open("Logs.csv",ios::out | ios::app);
            logfileStream<<log<<endl;
            logChain.append(log);

            HashfileStream.open("Hash.csv",ios::out | ios::app);
            string salt="SALT";
            if(hashChain.Tail!=nullptr)
                salt=hashChain.Tail->Block;
            log+=salt;
            HashfileStream<<sha256(log)<<endl;
            hashChain.append(sha256(log));

            logfileStream.close();
            HashfileStream.close();
            Backup_for_Recovery();
        }

        string copyTime(string time){
            string tempTime="";
            for(int i=0 ; i<time.length()-1; ++i)
                tempTime+=time[i];
            return tempTime;
        }

        void Backup_for_Recovery(){
            fstream bacupkLogs;
            bacupkLogs.open(logBackup,ios::out | ios::trunc);
            if(bacupkLogs){
                Node *current=logChain.Head;
                while(current!=nullptr){
                    bacupkLogs<<current->Block<<endl;
                    current=current->Next;
                }
            }
            else
                cout<<"\nFailed to backup\n\n";

            fstream backupHashes;
            backupHashes.open(hashBackup,ios::out | ios::trunc);
            if(backupHashes){
                Node *current=hashChain.Head;
                while(current!=nullptr){
                    backupHashes<<current->Block<<endl;
                    current=current->Next;
                }
            }
            else
                cout<<"\nFailed to backup\n\n";   
        }
        void Recovery()[
            
        ]

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