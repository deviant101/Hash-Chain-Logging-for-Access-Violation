#pragma once
#include <string>
using namespace std;

class User{
    public:
        string Name;
        string Username;
        string Passwords;

    User(string name="", string user_n="", string pass=""){
        Name = name;
        Username = user_n;
        Passwords = pass;
    }
    void setName(string name){
        Name = name;
    }
    void setUsername(string user_n){
        Username = user_n;
    }
    void setPassword(string pass){
        Passwords = pass;
    }

    string getName(){ return Name; }
    string getUsername(){ return Username; }
    string getPassword(){ return Passwords; }
};