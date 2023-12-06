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

    void changeSign(){
        for(int i = 0; i < digits.size(); i++)
            digits[i] *= -1;
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

    Unumber operator+(const Unumber &other) const {
        if(other < Unumber(0)){
            Unumber temp = other;
            temp.changeSign();
            return *this - temp;
        }
        if (digits.size() < other.digits.size()) {
            return other + *this;
        }
        if(*this < Unumber(0) && *this < Unumber(0)){
            Unumber op1 = *this;
            Unumber op2 = other;
            op1.changeSign();
            op2.changeSign();
            op1 = op1 + op2;
            op1.changeSign();
            return op1;
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

        return result;
    }

    Unumber operator-(const Unumber &other) const {
        if (digits.size() < other.digits.size()) {
            Unumber result = other - *this;
            result.changeSign();
            return result;
        }
        if(other < Unumber(0)){
            Unumber temp = other;
            temp.changeSign();
            return *this + temp;
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
        if(op1 < Unumber(0)){
            op1.changeSign();
            sign *= -1;
        }
        if(op2 < Unumber(0)){
            op2.changeSign();
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
        for(int i = 0; i < result.digits.size(); i++)
            result.digits[i] *= sign;
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
        Unumber result;
        if (other.digits.size() == 1 && other.digits[0] == 0) {
            return Unumber();
        }

        Unumber dividend = *this;
        Unumber divider = other;
        int sign = 1;
        if(dividend < Unumber(0)){
            dividend.changeSign();
            sign *= -1;
        }
        if(divider < Unumber(0)){
            divider.changeSign();
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
        for(int i = 0; i < result.digits.size(); i++)
            result.digits[i] *= sign;
        return result;
    }

    string toBinary(){
        string result = "";
        Unumber temp = *this < 0 ? *this * Unumber(-1) : *this;
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
        if(*this < Unumber(0))
            result += "-";
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
    Unumber num1 = Unumber("32718981839");
    Unumber num2 = Unumber("1378727843291287384629");

    cout << "Sum: " <<  num1 + num2 << endl;

    cout << "Diff: " << num1 - num2 << endl;

    cout << "Mult: " << num2 * num1 << endl;

    cout << "Div: " << num2 / num1 << endl;

    cout << "Num2 Binary: " << num2.toBinary() << endl;

    return 0;
}