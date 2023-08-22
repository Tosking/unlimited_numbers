#ifndef UNUMBER_H_
#define UNUMBER_H_
#include <string>
#include <vector>
#include <cstdint>
using std::vector;
using std::string;

class Unumber{
private:
    vector<int8_t> dig;
    bool is_number(const string& s);
    void check_sign();
public:
    Unumber();
    Unumber(int num);
    Unumber(string str);
    ~Unumber();
    Unumber operator+(Unumber temp)const;
    Unumber operator+(int temp)const;
    Unumber operator-(Unumber temp)const;
    Unumber operator-(int temp)const;
    Unumber operator*(Unumber temp)const;
    Unumber operator*(int temp)const;
    Unumber operator/(Unumber temp)const;
    Unumber operator/(int temp)const;
    bool operator>(Unumber temp)const;
    bool operator<(Unumber temp)const;
    int size();
    string to_binary();
    void set(int num);
    void set(string &str);
    void print()const;
    void _debug()const;
};



#endif // UNUMBER_H_
