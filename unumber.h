#ifndef UNUMBER_H_
#define UNUMBER_H_
#include <string>
#include <vector>
using std::vector;
using std::string;

class Unumber{
private:
    vector<int> chunks;
    bool is_positive;
    bool is_number(const string& s);
    int *slice(int num);
public:
    Unumber();
    Unumber(int num);
    Unumber(string str);
    ~Unumber();
    void set(int num);
    void set(string str);
    void print()const;
};



#endif // UNUMBER_H_
