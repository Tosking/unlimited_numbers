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

int *Unumber::slice(int num)const{
    int *sliced = new int[2];
    sliced[0] = num % (int)1e9;
    sliced[1] = num / (int)1e9;
    return sliced;
}

void Unumber::set(int num){
    if(num < 0){
        sign = -1;
        num *= -1;
    }
    if(num < 1e9){
        chunks.push_back(num * sign);
    }
    else{
        int *sliced = slice(num);
        chunks.push_back(sliced[0] * sign);
        chunks.push_back(sliced[1] * sign);
    }
}

void Unumber::set(string str){
    if(str[0] == '-'){
        sign = -1;
        str[0] = '0';
    }
    else{
        sign = 1;
    }
    if(is_number(str)){
        int blocks = ceil((float)str.length() / 8);
        for(int i = blocks; i > 0; i--){
            chunks.push_back(stoi(str.substr(i * 8, 8)) * sign);
        }
    }
}

void Unumber::print()const{
    for(int i = 0; i < (int)chunks.size(); i++){
        if(i != (int)chunks.size() - 1)
            for(int k = 9 - (int)floor(log10(chunks[i])); k > 0; k--)
                cout << "0";
        cout << chunks[i];
    }
    cout << endl;
}

Unumber Unumber::operator+(Unumber *temp)const{
   vector<int> chunks1 = this->chunks;
   vector<int> chunks2 = temp->chunks;
   Unumber result;

   int chunks_num = (chunks1.size() > chunks2.size()) ? chunks1.size() : chunks2.size();
   int summ = 0;
   result.chunks.resize(chunks_num + 1);
   for(int i = 0; i < chunks_num; i++){
       summ = chunks1[i] + chunks2[i] + result.chunks[i];
       if(summ > (int)1e9){
           int *sliced = slice(summ);
           result.chunks[i] = sliced[0];
           result.chunks[i + 1] = sliced[1];
       }
       else{
           result.chunks[i] = summ;
       }
   }
   return result;
}
