#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <conio.h>
#include <direct.h>
#include <sys/stat.h>
#include <cctype>
using namespace std;

void makeDir(){
    struct stat st;
    if(stat("users",&st)!=0) _mkdir("users");
}

string hashPass(const string& p){
    unsigned long long h=0;
    for(size_t i=0;i<p.size();i++) h=h*131+p[i];
    stringstream s;
    s<<hex<<h;
    return s.str();
}

bool validUser(const string& u){
    if(u.size()<3||u.size()>20) return false;
    for(size_t i=0;i<u.size();i++)
        if(!isalnum(u[i])&&u[i]!='_') return false;
    return true;
}

bool validPass(const string& p){
    if(p.size()<6) return false;
    bool U=0,L=0,D=0;
    for(size_t i=0;i<p.size();i++){
        if(isupper(p[i])) U=1;
        if(islower(p[i])) L=1;
        if(isdigit(p[i])) D=1;
    }
    return U&&L&&D;
}

bool existsUser(const string& u){
    ifstream f(("users/"+u+".dat").c_str());
    return f.good();
}

string getPass(string msg){
    cout<<msg;
    string p;
    char ch;
    while((ch=_getch())!=13){
        if(ch==8){
            if(!p.empty()){
                p.erase(p.size()-1);
                cout<<"\b \b";
            }
        }else{
            p+=ch;
            cout<<"*";
        }
    }
    cout<<endl;
    return p;
}

bool regUser(){
    string u,p,c;
    cout<<"\n--- Register ---\nUsername: ";
    getline(cin,u);

    if(!validUser(u)){
        cout<<"Invalid Username\n";
        return false;
    }
    if(existsUser(u)){
        cout<<"User Already Exists\n";
        return false;
    }

    p=getPass("Password: ");
    if(!validPass(p)){
        cout<<"Weak Password (need upper, lower, digit, min 6)\n";
        return false;
    }

    c=getPass("Confirm: ");
    if(p!=c){
        cout<<"Passwords Do Not Match\n";
        return false;
    }

    ofstream f(("users/"+u+".dat").c_str());
    if(!f){
        cout<<"File Error\n";
        return false;
    }
    f<<u<<endl<<hashPass(p)<<endl;
    cout<<"Registered Successfully\n";
    return true;
}

string loginUser(){
    string u,p;
    cout<<"\n--- Login ---\nUsername: ";
    getline(cin,u);

    if(!existsUser(u)){
        cout<<"User Not Found\n";
        return "";
    }

    p=getPass("Password: ");
    ifstream f(("users/"+u+".dat").c_str());
    string su,sp;
    getline(f,su);
    getline(f,sp);

    if(u==su && hashPass(p)==sp){
        cout<<"Login Successful\n";
        return u;
    }
    cout<<"Incorrect Password\n";
    return "";
}

void dashboard(string u){
    int ch;
    while(true){
        cout<<"\nUser: "<<u<<"\n1.Profile\n2.Logout\nChoice: ";
        cin>>ch;
        cin.ignore();
        if(ch==1) cout<<"Username: "<<u<<endl;
        else if(ch==2) return;
        else cout<<"Invalid Choice\n";
    }
}

int main(){
    makeDir();
    int ch;
    while(true){
        cout<<"\n1.Register\n2.Login\n3.Exit\nChoice: ";
        cin>>ch;
        cin.ignore();

        if(ch==1) regUser();
        else if(ch==2){
            string u=loginUser();
            if(!u.empty()) dashboard(u);
        }
        else if(ch==3) break;
        else cout<<"Invalid Choice\n";
    }
    return 0;
}
