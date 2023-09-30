#include <iostream>
#include <fstream>
#include "Customer.cpp"
#include "Library.cpp"
using namespace std;

void LoadCustomers(fstream& customersFile, Customer customers[150]);

int main(){

    Library library;             //remove comment
    Customer customers[150];
    fstream customersFile;
    LoadCustomers(customersFile, customers);
    library.getCustomers_ptr(customers);
    while(1){
        cout<<"1 - Login as User\n"
            <<"2 - Login as Admin\n"
            <<"3 - Exit\n";
        int choice=0;
        cout<<"Select Option: ";
        cin>>choice;
        cout<<endl;
        if(choice==1)
            library.loginAsUser();
        else if(choice==2)
            library.loginAsAdmin();
        // else if(choice==2){
        //     library.loginAsAdmin();
        // }
        if(choice==3)
            break;
    }


    return 0;
}

void LoadCustomers(fstream& customersFile, Customer customers[150]){

    customersFile.open("customers.csv",ios::in);
    if(customersFile){
        string data="";
        getline(customersFile,data);    //for ignoring labels

        for(int customer_no=0; customer_no<150; customer_no++){
            data="";
            getline(customersFile,data);
            int comma_count=0;
            string str="";

            for(int i=0; i<data.length(); i++){
                if(data[i]!=',' && i!=data.length()-1)  //giving extra
                    str+=data[i];
                else{
                    if(comma_count==0){
                        customers[customer_no].setName(str);

                    }
                    else if(comma_count==1){
                        customers[customer_no].setUsername(str);
                    }
                    else if(comma_count==2){
                        customers[customer_no].setPassword(str);
                    }
                    else if(comma_count==3){
                        customers[customer_no].setFine(stoi(str));
                    }
                    else if(comma_count==4){
                        customers[customer_no].setISBN(str);
                    }
                    else if(comma_count==5){
                        customers[customer_no].setBalance(stoi(str));
                    }
                    str="";
                    ++comma_count;
                }
            }
        }
        // customers[149].Display();
        customersFile.close();
    }
    else{
        cout<<"File not found!"<<endl;
    }

}