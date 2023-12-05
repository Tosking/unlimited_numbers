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

    void makeNegative(int n) {}
    Unumber difForDivision(Unumber first, Unumber other) const{
        Unumber result;
        int carry = 0;
        for (int i = 0; i < first.digits.size(); i++) {
            int diff = first.digits[i] - carry;
            if (i < other.digits.size()) {
                diff -= other.digits[i];
            }
            if (diff < 0) {
                carry = 1;
                diff += 10;
            } else {
                carry = 0;
            }
            result.digits.push_back(diff);
        }
        result.removeLeadingZeros();
        return result;
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

    Unumber operator+(const Unumber& other) const {
        Unumber result;
        int carry = 0;
        int i = 0;
        while (i < digits.size() || i < other.digits.size() || carry > 0) {
            if (i < digits.size()) {
                carry += digits[i];
            }
            if (i < other.digits.size()) {
                carry += other.digits[i];
            }
            result.digits.push_back(carry % 10);
            carry /= 10;
            i++;
        }
        return result;
    }

    Unumber operator-(const Unumber& other) const {
        if(toInteger() != 0 && other.toInteger() != 0){
            return Unumber(toInteger() - other.toInteger());
        }
        Unumber result;
        Unumber bigger;
        Unumber smaller;
        int carry = 0;
        if(other.digits.size() > digits.size()){
            bigger = other;
            smaller = *this;
        }
        else {
            smaller = other;
            bigger = *this;
        }
        for (int i = 0; i < bigger.digits.size(); i++) {
            int diff = bigger.digits[i] - carry;
            if (i < smaller.digits.size()) {
                if (abs(diff) < abs(smaller.digits[i])) {
                    carry = (diff >= 0) - (diff < 0);
                } else {
                    carry = 0;
                }
                diff -= smaller.digits[i];
                diff *= (carry >= 0) - (carry < 0);
            }
            result.digits.push_back(diff);
        }
        if(other > *this){
            for_each(result.digits.begin(), result.digits.end(), [](int& n) {n *= -1;});
        }
        result.removeLeadingZeros();
        return result;
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
        if(toInteger() != 0 && other.toInteger() != 0){
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
        while(temp > Unumber(0)){
            Unumber tempDiv2 = temp / Unumber(2);
            Unumber tempDif = temp - tempDiv2;
            cout << temp - tempDiv2 << " " << temp  << " " << tempDiv2 << endl;
            if(tempDif > tempDiv2){
                result += "1";
            }
            else {
                result += "0";
            }
            temp = tempDiv2;
        }
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
    Unumber num1 = Unumber(-321678327);
    Unumber num2 = Unumber(-128);

    //Unumber sum = num1 + num2;
    //cout << "Sum: " << sum << endl;

    //Unumber diff = num1 - num2;
    //cout << "Difference: " << diff << endl;

    //Unumber mult = num2 / Unumber(2);
    cout << "Mult: " << num2.toBinary() << endl;

    return 0;
}