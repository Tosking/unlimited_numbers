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
    sliced[0] = num % (int)1e9;
    if(sliced[0] == 0)
        return sliced;
    sliced[1] = num - (sliced[0] * 1e9);
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
        int blocks = ceil(str.length() / 8);
        string *sliced = new string[blocks];
        for(int i = str.length(); i >= 0; i--){
            sliced[(int)floor(i / 8)][i % 8] = str[i - 1];
        }
        for(int i = 0; i < blocks; i++){
            chunks.push_back(stoi(sliced[i]));
        }
    }
}

void Unumber::print()const{
    for(int i = 0; i < chunks.size(); i++){
        cout << chunks[i];
    }
}
