#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <unistd.h>

using namespace std;

class Unumber {
private:
    vector<int> digits;
    void removeLeadingZeros() {
        while (digits.size() > 1 && digits.back() == 0) {
            digits.pop_back();
        }
    }

    bool ifConvertAvailable() const {
        if(*this <= Unumber((int)1e9) && *this >= Unumber((int)-1e9))
            return true;
        return false;
    }

    void removeLeadingDigits(){
        for(int i = 0; i < digits.size(); i++){
            if(abs(digits[i]) > 9){
                if(i + 1 >= digits.size()){
                    digits.push_back(0);
                }
                digits[i + 1] += floor(digits[i] / 10);
                digits[i] = digits[i] % 10;
            }
        }
    }

public:
    Unumber() {
    }

    Unumber(int num) {
        if (num == 0) {
            digits.push_back(0);
        } else {
            while (num != 0) {
                digits.push_back(num % 10);
                num /= 10;
            }
        }
    }

    Unumber(string str) {
        int sign = 1;
        if(str[0] == '-'){
            sign = -1;
            str = str.substr(1);
        }
        for(int i = str.length() - 1; i >= 0; i--){
            if(!isdigit(str[i])){
                digits = vector<int> {0};
                return;
            }
            digits.push_back((str[i] - '0') * sign);
        }
    }

    Unumber operator+(const Unumber& other) const {
        Unumber op1 = *this;
        Unumber op2 = other;
        if((op1 < Unumber(0) && op2 < Unumber(0)) || (op1 < Unumber(0) && op2 > Unumber(0))){
            op1 = op1 * Unumber(-1);
            op2 = op2 * Unumber(-1);
        }

        Unumber result;
        int carry = 0;
        int i = 0;
        while (i < op1.digits.size() || i < op2.digits.size() || carry > 0) {
            if (i < digits.size()) {
                carry += op1.digits[i];
            }
            if (i < op2.digits.size()) {
                carry += op2.digits[i];
            }
            if(carry < 0){
                result.digits.push_back(10 + carry);
                carry = -1;
            }
            else if(carry > 10){
                result.digits.push_back(carry - 10);
                carry = 1;
            }
            else{
                result.digits.push_back(carry);
                carry = 0;
            }
            i++;
        }
        if(other < Unumber(0) && (other * Unumber(-1)) > *this || op1 > *this){
            for_each(result.digits.begin(), result.digits.end(), [](int& n) {
                if(n > 0)
                    n *= -1;
            });
        }
        else{
            for_each(result.digits.begin(), result.digits.end(), [](int& n) {
                if(n < 0)
                    n *= -1;
            });
        }
        result.removeLeadingZeros();
        return result;
    }

    Unumber operator-(const Unumber& other) const {
        return *this + (other * Unumber(-1));
    }

    Unumber operator*(const Unumber& other) const {
        Unumber result;
        result.digits.assign(digits.size() + other.digits.size(), 0);
        for (int i = 0; i < digits.size(); i++) {
            int carry = 0;
            for (int j = 0; j < other.digits.size() || carry > 0; j++) {
                int prod = result.digits[i + j] + carry + digits[i] * (j < other.digits.size() ? other.digits[j] : 0);
                result.digits[i + j] = prod % 10;
                carry = prod / 10;
            }
        }
        result.removeLeadingZeros();
        return result;
    }

    bool operator>(const Unumber& other) const {
        if(digits[digits.size() - 1] < 0 && other.digits[other.digits.size() - 1] >= 0)
            return false;
        else if(digits[digits.size() - 1] >= 0 && other.digits[other.digits.size() - 1] < 0)
            return true;
        else if(digits.size() > other.digits.size())
            return true;
        else if(digits.size() < other.digits.size())
            return false;
        else
            for(int i = digits.size() - 1; i >= 0; i--)
                if(digits[i] > other.digits[i])
                    return true;
                else if(digits[i] < other.digits[i])
                    return false;
        return false;
    }

    bool operator>=(const Unumber& other) const {
        if(digits[digits.size() - 1] < 0 && other.digits[other.digits.size() - 1] > 0)
            return false;
        else if(digits[digits.size() - 1] > 0 && other.digits[other.digits.size() - 1] < 0)
            return true;
        if(digits.size() > other.digits.size())
            return true;
        else if(digits.size() < other.digits.size())
            return false;
        else
            for(int i = digits.size() - 1; i >= 0; i--)
                if(digits[i] > other.digits[i])
                    return true;
                else if(digits[i] < other.digits[i])
                    return false;
        return true;
    }

    bool operator<(const Unumber& other) const {
        return other > *this;
    }

    bool operator<=(const Unumber& other) const {
        return other >= *this;
    }

    int toInteger() const{
        if(*this > Unumber(2147483647) && *this < Unumber(-2147483647)){
            return 0;
        }
        int result = 0;
        for(int i = 0; i < digits.size(); i++){
            result += digits[i] * pow(10, i);
        }
        return result;
    }

    Unumber operator/(const Unumber& other) const {
        if(ifConvertAvailable() && other.ifConvertAvailable()){
            if(other.toInteger() != 0)
                return Unumber(toInteger() / other.toInteger());
        }
        Unumber result;
        if (other.digits.size() == 1 && other.digits[0] == 0) {
            return Unumber();
        }

        Unumber dividend = *this < 0 ? *this * Unumber(-1) : *this;
        Unumber divider = other < 0 ? other * Unumber(-1) : other;
        result.digits.resize(dividend.digits.size());

        Unumber remainder;
        for (int i = dividend.digits.size() - 1; i >= 0; i--) {
            remainder.digits.insert(remainder.digits.begin(), dividend.digits[i]);
            remainder.removeLeadingZeros();

            int mult = 0;
            while (remainder >= divider) {
                remainder = remainder - divider;
                //sleep(1);
                mult++;
            }

            result.digits[i] = mult;
        }
        result.removeLeadingDigits();
        result.removeLeadingZeros();
        return result;
    }

    string toBinary(){
        string result = "";
        if(*this < Unumber(0))
            result += "-";
        Unumber temp = *this < 0 ? *this * Unumber(-1) : *this;
        int i = 0;
        while(temp > Unumber(0)){
            Unumber tempDiv2 = temp / Unumber(2);
            Unumber tempDif = temp - tempDiv2;
            cout<< ++i << ": " << tempDif << " " << temp  << " " << tempDiv2 << endl;
            if(tempDif > tempDiv2){
                result += "1";
            }
            else {
                result += "0";
            }
            temp = tempDiv2;
        }
        reverse(result.begin(), result.end());
        return result;
    }

    friend ostream& operator<<(ostream& os, const Unumber& num) {
        int sign = 1;
        for (int i = num.digits.size() - 1; i >= 0; i--) {
            os << num.digits[i] * sign;
            if(num < 0)
                sign = -1;
        }
        return os;
    }
};

int main() {
    Unumber num1 = Unumber(321678327);
    Unumber num2 = Unumber("132321789783");

    //Unumber sum = num1 + num2;
    //cout << "Sum: " << sum << endl;

    //Unumber diff = num1 - num2;
    //cout << "Difference: " << diff << endl;

    //Unumber mult = num2 / Unumber(2);
    cout << "Mult: " << num2.toBinary() << endl;

    return 0;
}