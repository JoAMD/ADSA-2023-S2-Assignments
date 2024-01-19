#include <iostream>
#include <algorithm>
#include <sstream>
#include <cmath>

using namespace std;

string addition(string i1, string i2, int base);
string subtraction(string i1, string i2, int base);
string schoolMethodMult(string a, string b, int base);
string division(string deno, string numer, int base);

void pad_zeros(string* i1, string* i2) {
    string i1V = *i1;
    string i2V = *i2;
    if (i1V.length() > i2V.length()) {
        //pad zeros to i2
        (*i2).insert(0, i1V.length() - i2V.length(), '0');
    }
    else if (i2V.length() > i1V.length()) {
        //pad zeros to i1
        (*i1).insert(0, i2V.length() - i1V.length(), '0');
    }
}

string remove_leading_zeros_in_reversed_string(string s) {
    if (s.length() == 0) {
        return s;
    }
    else if (s[s.length() - 1] != '0') {
        return s;
    }

    int s_length = -1;
    int i;
    for (i = s.length() - 1; i >= 0; --i) {
        if (s[i] != '0') {
            s_length = i + 1;
            break;
        }
    }

    if (s_length != -1) {
        return s.substr(0, s_length);
    }
    return "0";
}

string remove_leading_zeros(string s) {
    if (s.length() == 0) {
        return s;
    }
    else if (s[0] != '0') {
        return s;
    }

    int start_idx = -1;
    for (long unsigned int i = 0; i < s.length(); ++i) {
        if (s[i] != '0') {
            start_idx = i;
            break;
        }
    }
    if (start_idx != -1) {
        return s.substr(start_idx, s.length());
    }
    return "0"; // todo: test....
}

// void pad_zeros_indiv(string* s, int requiredLength) {
//     string sV = *s;
//     (*s).insert(0, requiredLength - sV.length(), '0');
// }

string karatsuba(string i1, string i2, int base) {
    
    //cout << "karatsuba i1=" << i1 << " i2=" << i2 << endl;

    // cout << "|i1 L=" << i1.length() << endl;
    // cout << "i1[0]=" << i1[0] << endl; 
    i1 = remove_leading_zeros(i1); // not needed
    // cout << "|i1 L=" << i1.length() << endl;
    i2 = remove_leading_zeros(i2); // not needed
    
    pad_zeros(&i1, &i2);
    // cout << "|i1 L=" << i1.length() << endl;

    int sLength = i1.length();

    if (sLength <= 1) {
        // do school method mult here .... 
        string resultBaseCase = schoolMethodMult(i1, i2, base);
        //cout << "base case: i1=" << i1 << " i2=" << i2 << endl; 
        //need to handle base (10 for decimal taken here) //todo: check other similar places
        // int resultBaseCaseInt = stoi(i1) * stoi (i2);
        // string resultBaseCase = "";
        // if (resultBaseCaseInt >= base) {
        //     resultBaseCase += "1";
        //     resultBaseCase += to_string(resultBaseCaseInt - 4);
        // }
        // else {
        //     resultBaseCase += to_string(resultBaseCaseInt);
        // }
        return resultBaseCase; //will not work.., what if i1 is 100 digits and i2 is 1 digit
    }

    //cout << "karatsuba i1=" << i1 << " i2=" << i2 << endl;

    int mid = ceil(sLength / 2); // todo: no need of ceil .. already takes floor
    int k = sLength - mid;
    //cout << "mid=" << mid << endl;

    string a1 = i1.substr(0, mid);
    string a0 = i1.substr(mid, k);
    
    string b1 = i2.substr(0, mid);
    string b0 = i2.substr(mid, k);

    //cout << "p2 mult\n";
    string p2 = karatsuba(a1, b1, base);

    string p1_1 = addition(a1, a0, base);
    string p1_0 = addition(b1, b0, base);

    //cout << "p1 mult\n";
    string p1 = karatsuba(p1_1, p1_0, base);
    
    //cout << "p0 mult\n";
    string p0 = karatsuba(a0, b0, base);

    string t2 = p2; //not deep copy ...
    t2.insert(t2.length(), 2 * k, '0');

    // cout <<
    string t1_0 = addition(p2, p0, base);
    //cout << " t1_0=" << t1_0 << endl;

    string t1 = subtraction(p1, t1_0, base);
    //cout << " t1=" << t1 << endl;
    //cout << t1.length() << "=t1length\n";

    t1.insert(t1.length(), k, '0');

    string t0 = p0; //not deep copy ...

    //cout << t1.length() << "=t1length\n";
    string add = addition(t2, t1, base);
    string product = addition(add, t0, base);
    //cout << "karatsuba i1=" << i1 << " i2=" << i2 << " product=" << product << " || add= " << add << " p2=" << p2 << " p1=" << p1 << " p0=" << p0 << " t1_0=" << t1_0 << " t2=" << t2 << " t1=" << t1 << " t0=" << t0 << endl;

    return product;

}

bool continue_subtracting(string numer, string deno) {
    if (numer.length() < deno.length()) {
        return false;
    }

    if (numer.length() > deno.length()) {
        return true;
    }

    for (long unsigned int i = 0; i < numer.length(); ++i) {
        int numerVal = numer[i] - '0';
        int denoVal = deno[i] - '0';
        if (numerVal == denoVal) {
            continue;
        }
        if (numerVal < denoVal) {
            return false;
        }
        if (numerVal > denoVal) {
            return true;
        }
    }

    return true;

}

string division(string numer, string deno, int base) {
    string numer_part = "";
    string quotient = "";

    for (long unsigned int i = 0; i < numer.length(); ++i) {
        
        numer_part += numer[i];
        long unsigned int counter = 0;

        numer_part = remove_leading_zeros(numer_part);

        //cout << "numer_part=" << numer_part << endl;
        
        while (continue_subtracting(numer_part, deno)) {
            numer_part = subtraction(numer_part, deno, base);
            counter++;
        }
        //cout << "numer_part=" << numer_part << endl;


        //cout << "counter=" << counter << endl;

        quotient += to_string(counter);
    }

    quotient = remove_leading_zeros(quotient);

    return quotient;
}

/* i1 - i2 */
string subtraction(string i1, string i2, int base) {
    // i1 should be greater or equal to i2 ...

    pad_zeros(&i1, &i2);

    int carry = 0;
    string diff = "";
    for (int i = i2.length() - 1; i >= 0; --i) {
        int i1V = i1[i] - '0';
        int i2V = i2[i] - '0';
        if (i1V >= i2V + carry) {
            diff += to_string(i1V - i2V - carry);
            carry = 0;
        }
        else {
            //borrow
            if (i <= 0) {
                //cout << "i1 is less than i2 in subtraction!" << " i: " << i << " i1V: " << i1V << " i2V: " << i2V << "\n";
                return "-1";
            }
            diff += to_string(i1V + base - i2V - carry);
            carry = 1;

            // else {
            //     int i1VPrev = i1[i - 1] - '0';
            //     i1[i - 1] = to_string(i1VPrev - 1)[0]; //todo: test all cases working?
            //     diff += to_string(i1V + base - i2V);
            // }

        }
    }

    diff = remove_leading_zeros_in_reversed_string(diff); // can be removed

    reverse(diff.begin(), diff.end());

    return diff;
}

string schoolMethodMult(string a, string b, int base) {
    // (check a > b)

    int carry = 0;
    string fullProduct = "";
    string partialProduct = "";
    long unsigned int iStart = b.length() - 1;
    for (int i = iStart; i >= 0; --i) {
        
        carry = 0;
        partialProduct = "";
        int bV = b[i] - '0';
        //cout << "bV=" << bV << endl;
        
        for (int j = a.length() - 1; j >= 0; --j) {
            int aV = a[j] - '0';
            //cout << "aV=" << aV << endl;
            int res = aV * bV + carry;
            //cout << "res=" << res << endl;
            int currentVal;
            if (res >= base) {
                int quo = res / base;
                int rem = res % base;
                currentVal = rem;
                carry = quo;
                // cout << ", carry " << carry << ", ";
            }
            else {
                // cout << ", carry 0, ";
                carry = 0;
                currentVal = res;
            }
            partialProduct += to_string(currentVal);
            //cout << "currentVal=" << currentVal << endl;
            //cout << "carry=" << carry << endl;
            //cout << "partialProduct=" << partialProduct << endl;
        }

        // cout << "i - iStart=" << i - iStart << endl;
        //cout << "carry!=0" << (carry!=0) << endl;
        //cout << "partialProduct.length()=" << partialProduct.length() << endl;

        if (carry != 0) {
            partialProduct += to_string(carry);
        }
        
        //cout << "partialProduct.length()=" << partialProduct.length() << endl;
        //cout << "partialProduct[2]=" << partialProduct[2] << endl;

        reverse(partialProduct.begin(), partialProduct.end());

        for (long unsigned int z = 0; z < iStart - i; ++z) {
            partialProduct += "0";
        }

        //cout << "partialProduct=" << partialProduct << endl;
        fullProduct = addition(fullProduct, partialProduct, base);
        //cout << "fullProduct=" << fullProduct << endl;
    }

    return fullProduct;

}

string addition(string i1, string i2, int base) {

    //cout << i1.length() << endl;
    //cout << i2.length() << endl;
    pad_zeros(&i1, &i2);
    //cout << i1.length() << endl;
    //cout << i2.length() << endl;

    int carry = 0;
    string sum = "";
    for (int i = i2.length() - 1; i >= 0; --i) {
        int i2V = i2[i] - '0';
        int i1V = i1[i] - '0';
        // cout << "i1V=" << i1V << " i2V=" << i2V << endl;
        int resultInt = i1V + i2V + carry;
        // cout << "resultInt=" << resultInt << " ";
        // string result = to_string(resultInt);
        int currentVal;
        if (resultInt >= base) {
            // cout << ", carry 1, ";
            carry = 1;
            currentVal = resultInt - base;
        }
        else {
            // cout << ", carry 0, ";
            carry = 0;
            currentVal = resultInt;
        }
        // cout << "currentVal=" << currentVal << endl;
        sum += to_string(currentVal);
    }

    if (carry != 0) {
        sum += to_string(carry);
    }

    sum = remove_leading_zeros_in_reversed_string(sum);

    reverse(sum.begin(), sum.end());

    //cout << "Sum of Adding " << i1 << " and " << i2 << " = " << sum << endl;

    return sum;
}

// void passByRefVal(string* s) {
//     string sV = *s;
//     *s = sV + "222";
// }
enum DebugMode {
    Addition = 0,
    Karatsuba = 1,
    NoDebug = 2
};

int main(int argc, char* argv[]) {

    // string s = "65";
    // cout << s[0] << endl;
    // cout << s[1] << endl;
    // cout << s[2] << endl;
    // cout << s[3] << endl;
    // cout << s.length() << endl;
    // reverse(s.begin(), s.end());
    // cout << s.length() << endl;

    // cout << "1: (9) 87 * 54 = " << schoolMethodMult("87", "54", 9) << endl;
    // cout << "2: (4) 231 * 12 = " << schoolMethodMult("231", "12", 4) << endl;
    // cout << "3: (10) 387 * 54 = " << schoolMethodMult("387", "54", 10) << endl;

    // cout << "4: (4) 12 * 231 = " << schoolMethodMult("12", "231", 4) << endl;
    // cout << "5: (9) 54 * 87 = " << schoolMethodMult("54", "87", 9) << endl;

    // cout << 4 % 3;
    // cout << 4 / 3;

    // string p2 = "2";
    // string p1 = "2";
    // cout << addition(p1, p2, 4) << endl;

    // string a1 = "whale";
    // string b1 = "notwhale";
    // b1 = a1;
    // b1 += b1;
    // cout << a1 << endl;
    // cout << b1 << endl;

    // string t2 = "3";
    // string t1 = "8";
    // t1.insert(t1.length(), 1, '0');
    // t2.insert(t2.length(), 2, '0');
    // cout << addition(t2, t1, 10) << endl;

    // string s = "3300000";

    //cout << s.length() << endl;
    // s = remove_leading_zeros_in_reversed_string(s);
    //cout << s.length() << endl;
    //cout << s << endl;


    // string s11 = "1";
    // string s22 = "3";
    // cout << addition(s11, s22, 10);

    // string s = "34";
    // int n = 9;
    // char c = (char)n;
    // // s.push_back(c);
    // s += to_string(n);
    // cout << c << endl;
    // cout << s << endl;
    // cout << s[0] << s[1] << endl;
    // return 0;

    //add, mul, div
    // cout << "I1 I2 B" << endl;

    // string sss = "qwe1233";
    // int mid = 3;
    // cout << sss.substr(0, mid) << " " << sss.substr(mid, sss.length() - mid);
    // cout << sss;

    // passByRefVal(&sss);
    // cout << sss;

    string i1, i2, b;
    cin >> i1 >> i2 >> b;
    int base = stoi(b);

    //school method addition

    // cout << i1.length() << "\n";
    // cout << i2.length() << "\n";

    // cout << i1 << "\n" << i2 << "\n";
    // cout << i1.length() << "\n";
    // cout << i2.length() << "\n";

    string sum = addition(i1, i2, base);
    string product = karatsuba(i1, i2, base);
    string quotient = division(i1, i2, base);
    // string diff = subtraction(i1, i2, base);

    // cout << sum << " " << diff << " 0" << endl;
    cout << sum << " " << product << " " << quotient << endl;

    return 0;
}