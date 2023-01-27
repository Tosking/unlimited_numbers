#ifndef UNUMBER_H_
#define UNUMBER_H_
#include <string>
#include <vector>
using std::vector;
using std::string;

class Unumber{
private:
    vector<int> chunks;
    int sign;
    bool is_number(const string& s);
    int *slice(int num)const;
    void check_sign();
public:
    Unumber();
    Unumber(int num);
    Unumber(string str);
    ~Unumber();
    Unumber operator+(Unumber temp)const;
    Unumber operator-(Unumber temp)const;
    Unumber operator*(Unumber temp)const;
    Unumber operator/(Unumber temp)const;
    int size();
    string to_binary();
    void set(int num);
    void set(string &str);
    void print()const;
};



#endif // UNUMBER_H_
