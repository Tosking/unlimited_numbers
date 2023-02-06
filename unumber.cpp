#include "unumber.h"
#include <algorithm>
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

Unumber::~Unumber(){}


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
    else{
        sign = 1;
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

void Unumber::set(string &str){
    if(str[0] == '-'){
        sign = -1;
        str[0] = '0';
    }
    else{
        sign = 1;
    }
    if(is_number(str)){
        int blocks = ceil((float)str.length() / 8);
        vector<string> result;
        result.resize(blocks);
        for(int i = 0; i < (int)str.length(); i++){
            result[(i + (8 - str.length() % 8)) / 8] += str[i];
        }
        for(int i = blocks - 1; i >= 0; i--){
            chunks.push_back(stoi(result[i]) * sign);
        }
    }
}

void Unumber::print()const{
    for(int i = chunks.size() - 1; i >= 0 ; i--){
        //if(i != (int)chunks.size() - 1)
            //for(int k = 9 - (int)floor(log10(chunks[i])); k > 0; k--)
                //cout << "0";
        cout << chunks[i] << "|";
    }
    cout << endl;
}

Unumber Unumber::operator+(Unumber temp)const{
   vector<int> chunks1 = this->chunks;
   vector<int> chunks2 = temp.chunks;
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
   result.check_sign();
   return result;
}

Unumber Unumber::operator-(Unumber temp)const{
   vector<int> chunks1 = this->chunks;
   vector<int> chunks2 = temp.chunks;
   Unumber result;

   int chunks_num = (chunks1.size() > chunks2.size()) ? chunks1.size() : chunks2.size();
   int summ = 0;
   result.chunks.resize(chunks_num + 1);
   for(int i = 0; i < chunks_num; i++){
       summ = chunks1[i] - chunks2[i] + result.chunks[i];
       if(summ > (int)1e9){
           int *sliced = slice(summ);
           result.chunks[i] = sliced[0];
           result.chunks[i + 1] = sliced[1];
       }
       else{
           result.chunks[i] = summ;
       }
   }
   result.check_sign();
   return result;
}

void Unumber::check_sign(){
    if(chunks[chunks.size() - 1] == 0){
        for(int i = chunks.size() - 1; i < 0; i--)
            chunks[i - 1] = chunks[i];
        chunks.resize(chunks.size() - 1);
    }
    for(int k = 0; k < 2; k++)
        if(chunks.size() != 0){
            bool positive = (chunks.size() - 1 < 0) ? 0 : 1;
            for(int i = 0; i < (int)chunks.size() - 1; i++){
                if(chunks[i] < 0 && positive){
                    chunks[i] = (int)1e9 + chunks[i];
                    chunks[i + 1] -= 1;
                }
                else if(chunks[i] > 0 && !positive){
                    chunks[i] = (int)1e9 - chunks[i];
                    chunks[i + 1] += 1;
                }
            }
        }
}

string Unumber::to_binary(){
    string result;
    vector<double> temp;
    for(int i = 0; i < chunks.size(); i++)
        temp.push_back(chunks[i]);
    while(chunks[chunks.size() - 1] >= 1){
        for(int i = 0; i < chunks.size(); i++){
            temp[i] /= 2;
            if(temp[i] > 1 && temp[i] != 0 && i != 0){
                temp[i - 1] += 5e8;
                temp[i] = 0;
            }
            if(i == 0 && ((int)temp[i] + 1) % 2 != 0){
                result += "1";
                temp[i] = floor(temp[i]);
            }
            else if(i == 0){
                result += "0";
            }
        }
        if(temp[0] == 0)
            break;
    }
    reverse(result.begin(), result.end());
    return result;
}
