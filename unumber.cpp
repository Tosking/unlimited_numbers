#include "unumber.h"
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool Unumber::is_number(const string& s){
    string::const_iterator it = s.begin();
    while (it != s.end() && isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

Unumber::Unumber(){}

Unumber::Unumber(int num){
    set(num);
}

Unumber::Unumber(string str){
    set(str);
}

int *Unumber::slice(int num){
    int *sliced = new int[2];
    sliced[1] = num % (int)1e9;
    if(sliced[1] == 0)
        return sliced;
    sliced[0] = num - (sliced[1] * 1e9);
    return sliced;
}

void Unumber::set(int num){
    if(num < 0){
        is_positive = false;
        num *= -1;
    }
    if(num < 1e9){
        chunks.push_back(num);
    }
    else{
        int *sliced = slice(num);
        chunks.push_back(sliced[0]);
        chunks.push_back(sliced[1]);
    }
}

void Unumber::set(string str){
    if(is_number(str)){
        int blocks = ceil((float)str.length() / 8);
        for(int i = 0; i < blocks; i++){
            chunks.push_back(stoi(str.substr(i * 8, 8)));
        }
    }
}

void Unumber::print()const{
    for(int i = 0; i < (int)chunks.size(); i++){
        if(i != (int)chunks.size() - 1)
            for(int i = 0; i <= 9 - (int)floor(log10(chunks[i])); i++)
                cout << "0";
        cout << chunks[i];
    }
    cout << endl;
}
