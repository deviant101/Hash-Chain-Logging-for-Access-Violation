#include <iostream>
#include <fstream>
#include "Customer.cpp"
#include "Library.cpp"
using namespace std;

int main(){

    Library library;             //remove comment
    Customer customers[150];
    fstream customersFile;
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



    return 0;
}