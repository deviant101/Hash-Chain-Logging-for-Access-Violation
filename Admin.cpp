#include <iostream>
#include <fstream>
#include "User.cpp"

class Admin:public User{
    public:
        string PhoneNumber;
        string CNIC;

    Admin(){
        fstream adminStream;
        adminStream.open("Admin.csv",ios::in);
        if(adminStream.is_open()){
            string data="";
            string str="";
            int comma_count = 0;
            getline(adminStream,data);
            for(int i=0;i<data.length();i++){
                if(data[i]!=',' && i!=data.length()-1)
                    str+=data[i];
                else{
                    if(comma_count==0){
                        Name=str;
                    }
                    else if(comma_count==1){
                        Username=str;
                    }
                    else if(comma_count==2){
                        Passwords=str;
                    }
                    else if(comma_count==3){
                        CNIC=str;
                    }
                    else if(comma_count==4){
                        PhoneNumber=str;
                    }
                    str="";
                    ++comma_count;
                }
            }
        }
        else{
            cout<<"File not found!"<<endl;
        }
        adminStream.close();
    }

    void setPhoneNumber(string phone){
        PhoneNumber=phone;
    }
    void setCNIC(string cnic){
        CNIC=cnic;
    }

    string getPhoneNumber(){
        return PhoneNumber;
    }
    string getCNIC(){
        return CNIC;
    }
    void viewProfile(){
        cout<<"Name: "<<Name<<endl;
        cout<<"Username: "<<Username<<endl;
        cout<<"Password: "<<Passwords<<endl;
        cout<<"CNIC: "<<CNIC<<endl;
        cout<<"Phone Number: "<<PhoneNumber<<"\n\n";
    }
};