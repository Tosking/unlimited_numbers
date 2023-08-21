#include "unumber.h"
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <cctype>

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

void Unumber::set(int num){
    int temp = num;
    int sign = 1;
    if(temp < 0){
        sign = -1;
        temp *= -1;
    }
    while(temp > 0){
        dig.push_back((temp % 10) * sign);
        temp /= 10;
    }
}

void Unumber::set(string &str){
    string temp = str;
    int sign = 1;
    if(temp[0] == '-'){
        sign = -1;
        temp.erase(0, 1);
    }
    while(!temp.empty()){
        if(isdigit(temp.back()))
            dig.push_back((temp.back() - '0') * sign);
        temp.pop_back();
    }
}

void Unumber::print()const{
    cout << (int)dig[dig.size() - 1];
    for(int i = (dig.size() - 2); i >= 0; i--)
        cout << (int)abs(dig[i]);
    cout << endl;
}

void Unumber::check_sign(){
    bool positive = true;
    if(dig[dig.size() - 1] < 0){
        positive = false;
    }
    while(dig[dig.size() - 1] == 0)
        dig.pop_back();
    if(positive){
        for(int i = 0; i < (dig.size() - 1); i++){
            if(dig[i] < 0){
                dig[i] = 10 + dig[i];
                dig[i + 1]--;
            }
        }
    }
    else{
        for(int i = 0; i < (dig.size() - 1); i++){
            if(dig[i] > 0){
                dig[i] = -10 + dig[i];
                dig[i + 1]++;
            }
        }
    }
}

Unumber Unumber::operator+(Unumber temp)const{
    vector<int8_t> dig1 = this->dig;
    vector<int8_t> dig2 = temp.dig;
    Unumber result;

    vector<int8_t> dig_max = (dig1.size() > dig2.size()) ? dig1 : dig2;
    vector<int8_t> dig_min = (dig1.size() > dig2.size()) ? dig2 : dig1;

    int8_t sum = 0;

    for(int i = 0; i < dig_max.size(); i++){
        if(i < dig_min.size())
            sum += dig_max[i] + dig_min[i];
        else
            sum += dig_max[i];
        if(abs(sum) > 9){
            result.dig.push_back(sum % 10);
            sum /= 10;
        }
        else{
            result.dig.push_back(sum);
            sum = 0;
        }
    }
    result.check_sign();
    return result;
}

Unumber Unumber::operator+(int temp)const{
    Unumber result(temp);
    return result + *this;
}

Unumber Unumber::operator-(Unumber temp)const{
    Unumber un = *this;
    Unumber result = temp;
    for(int i = 0; i < result.dig.size(); i++)
        result.dig[i] *= -1;
    result = un + result;
    result.check_sign();
    return result;
}

Unumber Unumber::operator-(int temp)const{
    Unumber result(temp);
    return result - *this;
}