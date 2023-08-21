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

void Unumber::_debug()const{
    for(int i = chunks.size() - 1; i >= 0 ; i--){
        //if(i != (int)chunks.size() - 1)
            //for(int k = 9 - (int)floor(log10(chunks[i])); k > 0; k--)
                //cout << "0";
        cout << chunks[i] << "|";
    }
    cout << " size:" << chunks.size();
    cout << endl;
}

Unumber Unumber::operator+(Unumber temp)const{
   vector<int> chunks1 = this->chunks;
   vector<int> chunks2 = temp.chunks;
   Unumber result;

   vector<int> chunks_max = (chunks1.size() > chunks2.size()) ? chunks1 : chunks2;
   vector<int> chunks_min = (chunks1.size() > chunks2.size()) ? chunks2 : chunks1;
   int summ = 0;
   result.chunks.resize(chunks_max.size() + 1);
   for(int i = 0; i < chunks_max.size(); i++){
        if(chunks_min.size() > i)
            summ = chunks_max[i] + chunks_min[i] + result.chunks[i];
        else
            summ = chunks_max[i] + result.chunks[i];
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

    vector<int> chunks_max = (chunks1.size() > chunks2.size()) ? chunks1 : chunks2;
    vector<int> chunks_min = (chunks1.size() > chunks2.size()) ? chunks2 : chunks1;
    int summ = 0;
    result.chunks.resize(chunks_max.size());
    for(int i = 0; i < chunks_max.size(); i++){
        if(chunks_min.size() > i)
            summ = chunks_max[i] - chunks_min[i] + result.chunks[i];
        else
            summ = chunks_max[i] - result.chunks[i];
        result.chunks[i] = summ;
    }
    result.check_sign();
    return result;
}

Unumber Unumber::operator-(int temp)const{
    vector<int> chunks1 = this->chunks;
    Unumber result = *this;

    int chunks_num = chunks1.size();
    int summ = 0;
    summ = chunks1[0] - temp;
    result.chunks[0] = summ;
    result.check_sign();
    return result;
}

Unumber Unumber::operator*(int temp)const{
    vector<int> chunks1 = this->chunks;
    Unumber result;

    int summ = 0;
    result.chunks.resize(chunks1.size() + 1);
    for(int i = 0; i < chunks1.size(); i++){
        summ = chunks1[i] * temp + result.chunks[i];
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

Unumber Unumber::operator*(Unumber temp)const{
    vector<int> chunks1 = this->chunks;
    vector<int> chunks2 = temp.chunks;
    Unumber result;
    
    vector<int> chunks_max = (chunks1.size() > chunks2.size()) ? chunks1 : chunks2;
    vector<int> chunks_min = (chunks1.size() > chunks2.size()) ? chunks2 : chunks1;

    result.chunks.resize(chunks1.size() + chunks2.size());

    for(int i = 0; i < chunks2.size(); i++){
        result = (*this) * chunks2[i] + result;
    }

    return result;
}

Unumber Unumber::operator/(Unumber temp)const{
    vector<int> chunks1 = this->chunks;
    vector<int> chunks2 = temp.chunks;
    Unumber result;
    

    int chunks_num = (chunks1.size() > chunks2.size()) ? chunks1.size() : chunks2.size();
    double div = 0;
    /*
        Деление происходит по алгоритму вычитывания нулей и деление на div,
        где у числа 456732 div = 4.56732, у числа 1.2e26 div = 1.2 и так далее.
        div вычисляется исходя из 2-х последних блоков числа
    */



    /*for(int i = chunks2.size() - 1; i < chunks1.size(); i++)
        result.chunks.push_back(chunks1[i]);
    if(result.chunks.size() == 0)
        return result;
   
    if(chunks2.size() <= 1){
        double temp_chunk = chunks2[chunks2.size()-1];
        while(temp_chunk > 10)
            temp_chunk /= 10;
        div = temp_chunk;
    }
    else{
        for(int i = chunks2.size() - 1; i > (chunks2.size() - 3); i--){
            double temp_chunk = chunks2[i];
            int nuls = 0;
            while(temp_chunk > 10 / (10 * nuls * (chunks2.size() - i))){
                temp_chunk /= 10;
                nuls++;
            }
            div += temp_chunk;
        }
    }
    cout << div << endl;
    for(int i = result.chunks.size() - 1; i >= 0; i--){
        long double result_chunk = 0;
        if(i > 0)
            result_chunk = 1e9 + result.chunks[i];
        long double temp_chunk = result_chunk / div;
        result.chunks[i] = (int)temp_chunk;
        if(temp_chunk - (int)temp_chunk != 0 && i < result.chunks.size() - 1){
            temp_chunk -= (int)temp_chunk;
            while(temp_chunk - (int)temp_chunk != 0 && temp_chunk < 1e8)
                temp_chunk *= 10;
            result.chunks[i - 1] += (int)temp_chunk;
        }
    }
    
    for(int i = 0; i < result.chunks.size(); i++){
        long double result_chunk = 0;
        if(i > 0)
            result_chunk = 1e9 + result.chunks[i];
        long double temp_chunk = result_chunk / chunks2[0];
        result.chunks[i] = (int)temp_chunk;
        if(temp_chunk - (int)temp_chunk != 0 && i > 0){
            temp_chunk -= (int)temp_chunk;
            while(temp_chunk - (int)temp_chunk != 0 && temp_chunk < 1e8)
                temp_chunk *= 10;
            cout << (int)temp_chunk << endl;
            result.chunks[i - 1] += (int)temp_chunk;
        }
    }*/
    return result; 
    

}

void Unumber::check_sign(){
    if(chunks.size() > 1){
        bool positive = (chunks[chunks.size() - 1] < 0) ? 0 : 1;
        for(int i = 0; i < chunks.size(); i++){
            if(chunks[i] < 0 && positive){
                (*this)._debug();
                cout << i << " " << chunks[i] << endl;
                chunks[i] = (int)1e9 + chunks[i];
                chunks[i + 1] -= 1;
            }
            else if(chunks[i] > 0 && !positive){
                cout << i << " 123 " << chunks[i] << endl;
                chunks[i] = -(int)1e9 + chunks[i];
                chunks[i + 1] += 1;
            }
        }
        if(chunks[chunks.size() - 1] == 0){
            chunks.pop_back();
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
