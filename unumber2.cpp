#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <unistd.h>
#include <cstdint>

using namespace std;

class Unumber {
private:
    vector<int8_t> digits;
    bool negative = false;
    void removeLeadingZeros() {
        while (digits.size() > 1 && digits.back() == 0) {
            digits.pop_back();
        }
    }

    bool ifConvertAvailable() const {
        if(*this <= Unumber((int)1e9))
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
        if (num < 0){
            negative = true;
            num *= -1;
        }
        } else {
            while (num != 0) {
                digits.push_back(num % 10);
                num /= 10;
            }
        }
    }

    Unumber(string str) {
        if(str[0] == '-'){
            negative = true;
            str = str.substr(1);
        }
        for(int i = str.length() - 1; i >= 0; i--){
            if(!isdigit(str[i])){
                throw invalid_argument("can't convert string");
            }
            digits.push_back(str[i] - '0');
        }
    }

    Unumber operator+(const Unumber &other) const {
        if(other.negative){
            Unumber temp = other;
            temp.negative = false;
            return *this - temp;
        }
        if (digits.size() < other.digits.size()) {
            return other + *this;
        }

        Unumber result = *this;
        int carry = 0;
        for (int i = 0; i < digits.size(); ++i) {
            result.digits[i] += (i < other.digits.size() ? other.digits[i] : 0) + carry;
            carry = result.digits[i] / 10;
            result.digits[i] %= 10;
        }

        while (carry) {
            result.digits.push_back(carry % 10);
            carry /= 10;
        }

        if(negative && other.negative){
            result.negative = true;
        }

        return result;
    }

    Unumber operator-(const Unumber &other) const {
        if(other.negative){
            Unumber temp = other;
            temp.negative = false;
            return *this + temp;
        }
        if (digits.size() < other.digits.size()) {
            Unumber result = other - *this;
            result.negative = true;
            return result;
        }


        Unumber result = *this;
        int carry = 0;
        for (int i = 0; i < other.digits.size() || carry; ++i) {
            result.digits[i] -= (i < other.digits.size() ? other.digits[i] : 0) + carry;
            carry = result.digits[i] < 0;
            if (carry) {
                result.digits[i] += 10;
            }
        }

        result.removeLeadingZeros();
        return result;
    }

    Unumber operator*(const Unumber& other) const {
        Unumber result;
        Unumber op1 = *this;
        Unumber op2 = other;
        int sign = 1;
        if(op1.negative){
            op1.negative = false;
            sign *= -1;
        }
        if(op2.negative){
            op2.negative = false;
            sign *= -1;
        }
        result.digits.assign(op1.digits.size() + op2.digits.size(), 0);
        for (int i = 0; i < op1.digits.size(); i++) {
            int carry = 0;
            for (int j = 0; j < op2.digits.size() || carry > 0; j++) {
                int prod = result.digits[i + j] + carry + op1.digits[i] * (j < op2.digits.size() ? op2.digits[j] : 0);
                result.digits[i + j] = prod % 10;
                carry = prod / 10;
            }
        }
        result.negative = sign == -1 ? 1 : 0;
        result.removeLeadingZeros();
        return result;
    }

    bool operator>(const Unumber& other) const {
        if(negative && !other.negative)
            return false;
        else if(!negative && other.negative)
            return true;
        else if(digits.size() > other.digits.size())
            return negative ? false : true;
        else if(digits.size() < other.digits.size())
            return negative ? true : false;
        else
            for(int i = digits.size() - 1; i >= 0; i--)
                if(digits[i] > other.digits[i])
                    return negative ? false : true;
                else if(digits[i] < other.digits[i])
                    return negative ? true : false;
        return false;
    }

    bool operator>=(const Unumber& other) const {
        if(negative && !other.negative)
            return false;
        else if(!negative && other.negative)
            return true;
        else if(digits.size() > other.digits.size())
            return negative ? false : true;
        else if(digits.size() < other.digits.size())
            return negative ? true : false;
        else
            for(int i = digits.size() - 1; i >= 0; i--)
                if(digits[i] > other.digits[i])
                    return negative ? false : true;
                else if(digits[i] < other.digits[i])
                    return negative ? true : false;
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
            throw invalid_argument("can't convert number into 32 bit representation");
        }
        int result = 0;
        for(int i = 0; i < digits.size(); i++){
            result += digits[i] * pow(10, i);
        }
        return result;
    }

    Unumber operator/(const Unumber& other) const {
        Unumber result;
        if (other.digits.size() == 1 && other.digits[0] == 0) {
            throw invalid_argument("division by zero");
        }

        Unumber dividend = *this;
        Unumber divider = other;
        int sign = 1;
        if(dividend.negative){
            dividend.negative = false;
            sign *= -1;
        }
        if(divider.negative){
            divider.negative = false;
            sign *= -1;
        }

        
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
        result.negative = sign == -1 ? 1 : 0;
        return result;
    }

    string toBinary(){
        string result = "";
        Unumber temp = *this;
        temp.negative = false;
        int i = 0;
        while(temp > Unumber(0)){
            Unumber tempDiv2 = temp / Unumber(2);
            Unumber tempDif = temp - tempDiv2;
            //cout<< ++i << ": " << tempDif << " " << temp  << " " << tempDiv2 << endl;
            if(tempDif > tempDiv2){
                result += "1";
            }
            else {
                result += "0";
            }
            temp = tempDiv2;
        }
        if(negative)
            result += "-";
        reverse(result.begin(), result.end());
        return result;
    }

    friend ostream& operator<<(ostream& os, const Unumber& num) {
        int sign = 1;
        if(num.negative)
            os << "-";
        for (int i = num.digits.size() - 1; i >= 0; i--) {
            os << (int)num.digits[i] * sign;
        }
        return os;
    }
    int getBytes(){
        return 8 * digits.size();
    }
};

int main() {
    Unumber num1 = Unumber("32718981839");
    Unumber num2 = Unumber("-1378727843291287384629");

    cout << "Sum: " <<  num1 + num2 << endl;

    cout << "Diff: " << num1 - num2 << endl;

    cout << "Mult: " << num2 * num1 << endl;

    cout << "Div: " << num2 / num1 << endl;

    cout << "Num2 Binary: " << num2.toBinary() << endl;

    cout << "Num2 Bytes: " << num2.getBytes() << endl;
    

    return 0;
}