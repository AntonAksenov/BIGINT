#include <iostream>
#include "big_integer.h"
#include <algorithm>
#include <assert.h>

big_integer &big_integer::minimise() {
    while (!digits.empty() && digits.back() == 0) {
        digits.pop_back();
    }
    if (digits.empty()) {
        isNegative = false;
    }
    return *this;
}

big_integer::big_integer() : isNegative(false), digits() {}

big_integer::big_integer(big_integer const &other) : isNegative(other.isNegative), digits(other.digits) {}

big_integer::big_integer(int a) : isNegative(a < 0), digits(){
        if (a) {
            digits.push_back(static_cast<uint32_t>(std::abs(static_cast<int64_t>(a))));
        }
}

big_integer add_long_int_unsigned(big_integer const &a, uint32_t const &b) {
    big_integer res;
    res.digits.resize(a.digits.size() + 1);

    uint32_t carry = b;

    for (size_t i = 0; i < a.digits.size(); i++) {
        res.digits[i] = (static_cast<uint32_t>((static_cast<uint64_t>(carry) + a.digits[i]) & UINT32_MAX));
        carry = static_cast<uint32_t>((static_cast<uint64_t>(carry) + a.digits[i]) >> 32);
    }
    res.digits[a.digits.size()] = carry;
    return res.minimise();
}

big_integer mul_long_int_unsigned(big_integer const &a, uint32_t const &b) {
    big_integer res;
    res.digits.resize(a.digits.size());

    uint32_t carry = 0;
    for (size_t i = 0; i < a.digits.size(); i++) {
        uint64_t tmp_res = static_cast<uint64_t>(a.digits[i]) * b + carry;
        res.digits[i] = static_cast<uint32_t>(tmp_res & UINT32_MAX);
        carry = static_cast<uint32_t>(tmp_res >> 32);
    }
    res.digits.push_back(carry);
    res.isNegative = false;
    return res.minimise();
}

big_integer::big_integer(std::string const &str) {
    uint32_t arr[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};
    //static std::vector<uint32_t> ten_pow{1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};
    static std::vector<uint32_t> ten_pow(arr, arr + 10);

    size_t end = str.length() - 1;
    size_t start = 0;
    if (str[0] == '-') {
        start++;
    }
    while (start + 9 <= end) {
        std::string s = str.substr(start, 9);
        *this = mul_long_int_unsigned(*this, ten_pow[9]);
        *this = add_long_int_unsigned(*this, static_cast<uint32_t>(stoi(s)));
        start += 9;
    }
    if (end - start + 1) {
        *this = mul_long_int_unsigned(*this, ten_pow[end - start + 1]);
        *this = add_long_int_unsigned(*this, static_cast<uint32_t>(stoi(str.substr(start))));
    }
    minimise();
    isNegative = (str[0] == '-');
}

big_integer::~big_integer() {}

big_integer &big_integer::operator=(big_integer const &other) {
    big_integer tmp(other);
    isNegative = tmp.isNegative;
    swap(digits, tmp.digits);
    return *this;
}

big_integer abs(big_integer const &a) {
    big_integer res(a);
    res.isNegative = false;
    return res;
}

big_integer &big_integer::operator+=(big_integer const &rhs) {
    if (isNegative != rhs.isNegative) {
        *this -= -rhs;
        return *this;
    }
    vector res(std::max(digits.size(), rhs.digits.size()) + 1);
    uint64_t carry = 0;
    for (size_t i = 0; i < std::min(digits.size(), rhs.digits.size()); i++) {
        carry += static_cast<uint64_t>(digits[i]) + rhs.digits[i];
        res[i] = static_cast<uint32_t>(carry) & UINT32_MAX;
        carry >>= 32;
    }
    for (size_t i = digits.size(); i < rhs.digits.size(); i++) {
        carry += static_cast<uint64_t>(rhs.digits[i]);
        res[i] = static_cast<uint32_t>(carry) & UINT32_MAX;
        carry >>= 32;
    }
    for (size_t i = rhs.digits.size(); i < digits.size(); i++) {
        carry += static_cast<uint64_t>(digits[i]);
        res[i] = static_cast<uint32_t>(carry) & UINT32_MAX;
        carry >>= 32;
    }
    res[res.size() - 1] = carry;
    swap(res, digits);
    return minimise();
}

big_integer &big_integer::operator-=(big_integer const &rhs) {
    if (isNegative != rhs.isNegative) {
        *this += -rhs;
        return *this;
    }
    const big_integer *min;
    const big_integer *max;
    if (abs(*this) <= abs(rhs)) {
        min = this;
        max = &rhs;
    } else {
        min = &rhs;
        max = this;
    }
    vector res(max->digits.size(), 0);
    bool borrow = false;
    for (size_t i = 0; i < res.size(); i++) {
        int64_t cur = max->digits[i];
        int64_t sub;
        if (i >= min->digits.size()) {
            sub = static_cast<int64_t>(borrow);
        } else {
            sub = static_cast<int64_t>(borrow) + min->digits[i];
        }
        cur -= sub;
        if (cur < 0) {
            cur += 1ull << 32;
            borrow = true;
        } else {
            borrow = false;
        }
        res[i] = static_cast<uint32_t>(cur);
    }
    isNegative = *this < rhs;
    swap(res, digits);
    return minimise();
}

big_integer &big_integer::operator*=(big_integer const &rhs) {
    isNegative ^= rhs.isNegative;
    vector res(digits.size() + rhs.digits.size(), 0);
    for (size_t i = 0; i < digits.size(); i++) {
        uint32_t carry = 0;
        for (size_t j = 0; j < rhs.digits.size(); j++) {
            uint64_t cur = static_cast<uint64_t>(digits[i]) * rhs.digits[j] + res[i + j] + carry;
            res[i + j] = static_cast<uint32_t> (cur & UINT32_MAX);
            carry = static_cast<uint32_t> (cur >> 32);
        }
        res[i + rhs.digits.size()] += carry;
    }
    swap(res, digits);
    return minimise();
}

big_integer &big_integer::operator/=(big_integer const &rhs) {
    if (rhs == 0) throw "division by zero";
    bool newSign = isNegative ^rhs.isNegative;

    if (*this == 0 || abs(rhs) > abs(*this)) {
        *this = 0;
    } else if (!(abs(rhs) == 1)) {
        if (rhs.digits.size() == 1) {
            uint64_t carry = 0;
            for (size_t i = digits.size(); i > 0; i--) {
                carry = (carry << 32) + digits[i - 1];
                digits[i - 1] = static_cast<uint32_t>(carry / rhs.digits[0]);
                carry %= rhs.digits[0];
            }
            minimise();
        } else {
            *this = long_unsigned_division(*this, rhs);
        }
    }
    isNegative = newSign;
    return *this;
}

big_integer long_unsigned_division(big_integer const &l, big_integer const &r) {
    uint32_t scaling_factor = static_cast<uint32_t>((static_cast<uint64_t> (UINT32_MAX) + 1) /
                                                    (static_cast<uint64_t>(r.digits[r.digits.size() - 1]) + 1));

    big_integer a(mul_long_int_unsigned(l, scaling_factor));
    big_integer b(mul_long_int_unsigned(r, scaling_factor));

    vector result(a.digits.size() - b.digits.size() + 1);

    big_integer cur_prefix;
    cur_prefix.digits.resize(b.digits.size() - 1);
    for (size_t i = a.digits.size() - b.digits.size() + 1, index = 0; i < a.digits.size(); i++, index++) {
        cur_prefix.digits[index] = a.digits[i];
    }
    uint64_t b1 = b.digits[b.digits.size() - 1];
    for (size_t i = result.size(); i > 0; i--) {

        cur_prefix.digits.push_front(a.digits[i - 1]);
        //cur_prefix.digits.insert(cur_prefix.digits.begin(), a.digits[i - 1]);

        uint64_t a2 = cur_prefix.digits[cur_prefix.digits.size() - 1];
        if (cur_prefix.digits.size() > b.digits.size()) {
            a2 = (a2 << 32) + cur_prefix.digits[cur_prefix.digits.size() - 2];
        }
        uint64_t quotient = a2 / b1;
        if (quotient >= UINT32_MAX) quotient = UINT32_MAX;

        big_integer mul_dq = mul_long_int_unsigned(b, static_cast<uint32_t>(quotient));

        while (cur_prefix < mul_dq) {
            quotient--;
            mul_dq -= b;
        }
        result[i - 1] = static_cast<uint32_t>(quotient & UINT32_MAX);
        cur_prefix -= mul_dq;
    }
    swap(a.digits, result);
    return a.minimise();
}

big_integer &big_integer::operator%=(big_integer const &rhs) {
    big_integer q = *this / rhs;
    return *this -= rhs * q;
}

void big_integer::toTwosComplement() {
    if (isNegative) {
        isNegative = false;
        for (size_t i = 0; i < digits.size(); i++) {
            digits[i] = ~digits[i];
        }
        *this += 1;
    }
}

void big_integer::toSignedForm() {
    if (digits[digits.size() - 1] >> 31) {
        for (size_t i = 0; i < digits.size(); i++) {
            digits[i] = ~digits[i];
        }
        isNegative = false;
        *this += 1;
        isNegative = true;
    }
}

big_integer &big_integer::operator&=(big_integer const &rhs) {
    vector res(std::max(digits.size(), rhs.digits.size()));
    toTwosComplement();
    big_integer rhsCopy = rhs;
    rhsCopy.toTwosComplement();
    for (size_t i = 0; i < std::min(digits.size(), rhs.digits.size()); i++) {
        res[i] = digits[i] & rhsCopy.digits[i];
    }
    for (size_t i = digits.size(); i < rhs.digits.size(); i++) {
        res[i] = digits[i];
    }
    for (size_t i = rhs.digits.size(); i < digits.size(); i++) {
        res[i] = rhsCopy.digits[i];
    }
    swap(res, digits);
    toSignedForm();
    return minimise();
}

big_integer &big_integer::operator|=(big_integer const &rhs) {
    toTwosComplement();
    big_integer rhsCopy = rhs;
    rhsCopy.toTwosComplement();
    digits.resize(std::min(digits.size(), rhsCopy.digits.size()));
    for (size_t i = 0; i < std::min(digits.size(), rhsCopy.digits.size()); i++) {
        digits[i] |= rhsCopy.digits[i];
    }
    toSignedForm();
    return minimise();
}

big_integer &big_integer::operator^=(big_integer const &rhs) {
    vector res(std::max(digits.size(), rhs.digits.size()));
    toTwosComplement();
    big_integer rhsCopy = rhs;
    rhsCopy.toTwosComplement();
    for (size_t i = 0; i < std::min(digits.size(), rhs.digits.size()); i++) {
        res[i] = digits[i] ^ rhsCopy.digits[i];
    }
    for (size_t i = digits.size(); i < rhs.digits.size(); i++) {
        res[i] = digits[i];
    }
    for (size_t i = rhs.digits.size(); i < digits.size(); i++) {
        res[i] = rhsCopy.digits[i];
    }
    swap(res, digits);
    toSignedForm();
    return minimise();
}

big_integer big_integer::honNegation() {
    big_integer a(*this);
    for (size_t i = 0; i < digits.size(); i++) {
        a.digits[i] = ~a.digits[i];
    }
    return a;
}

big_integer &big_integer::operator<<=(int rhs) {
    if (rhs < 0) {
        return *this >>= -rhs; // ?
    }
    uint32_t bigShift = static_cast<uint32_t>(rhs) / 32;
    uint32_t smallShift = static_cast<uint32_t>(rhs) % 32;
    bool wasNegative = isNegative;
    toTwosComplement();
    vector res(digits.size());
    uint32_t carry = 0;
    for (size_t i = 0; i < res.size(); i++) {
        uint64_t cur = (static_cast<uint64_t>(digits[i]) << smallShift) + carry;
        res[i] = static_cast<uint32_t>(cur & UINT32_MAX);
        carry = static_cast<uint32_t>(cur >> 32);
    }
    digits.clear();
    for (size_t i = 0; i < bigShift; i++) {
        digits.push_back(0);
    }
    for (size_t i = 0; i < res.size(); i++) {
        digits.push_back(res[i]);
    }
    digits.push_back(0);
    toSignedForm();
    minimise();
    if (wasNegative) {
        *this = -honNegation() - 1;
    }
    return *this;
}

big_integer &big_integer::operator>>=(int rhs) {
    if (rhs < 0) {
        return *this <<= -rhs; // ?
    }
    uint32_t bigShift = static_cast<uint32_t>(rhs) / 32;
    uint32_t smallShift = static_cast<uint32_t>(rhs) % 32;
    vector res(digits.size() - bigShift);
    uint64_t carry = 0;
    for (size_t i = res.size(); i > 0; i--) {
        uint64_t tmp = static_cast<uint64_t >(digits[i - 1]) << (32 - smallShift);
        res[i - 1] = static_cast<uint32_t>((tmp >> 32) + carry);
        carry = tmp & UINT32_MAX;
    }
    swap(res, digits);
    minimise();
    if (isNegative) {
        *this -= 1;
    }
    return *this;
}

big_integer big_integer::operator+() const {
    big_integer res = *this;
    return res;
}

big_integer big_integer::operator-() const {
    big_integer res = *this;
    res.isNegative = !isNegative;
    return res;
}

big_integer big_integer::operator~() const {
    return -(*this) - 1;
}

big_integer &big_integer::operator++() {
    *this += 1;
    return *this;
}

big_integer big_integer::operator++(int) {
    big_integer r = *this;
    ++*this;
    return r;
}

big_integer &big_integer::operator--() {
    *this -= 1;
    return *this;
}

big_integer big_integer::operator--(int) {
    big_integer r = *this;
    --*this;
    return r;
}

bool operator==(big_integer const &a, big_integer const &b) {
    return ((a.isNegative == b.isNegative) && (a.digits == b.digits)) || (a.digits.size() == 0 && b.digits.size() == 0);
}

bool operator!=(big_integer const &a, big_integer const &b) {
    return !(a == b);
}

bool operator>(big_integer const &a, big_integer const &b) {
    if (a == b) { return false; }
    if (a.isNegative) {
        if (b.isNegative) { // a <= 0, b <= 0, !(a == b == 0)
            if (a.digits.size() > b.digits.size()) {
                return false;
            } else if (a.digits.size() < b.digits.size()) {
                return true;
            } else {
                // continue
            }
        } else { // a <= 0, b >= 0, !(a == b == 0)
            return false;
        }
    } else {
        if (b.isNegative) { // a >= 0, b <= 0, !(a == b == 0)
            return true;
        } else { // a <= 0, b >= 0, !(a == b == 0)
            if (a.digits.size() < b.digits.size()) {
                return false;
            } else if (a.digits.size() > b.digits.size()) {
                return true;
            } else {
                // continue
            }
        }
    }
    for (size_t i = a.digits.size() - 1; i >= 0; i--) {
        if (a.digits[i] != b.digits[i]) {
            return (a.digits[i] > b.digits[i]) ^ a.isNegative;
        }
    }
}

bool operator<(big_integer const &a, big_integer const &b) {
    return (a != b) && !(a > b);
}

bool operator<=(big_integer const &a, big_integer const &b) {
    return a == b || a < b;
}

bool operator>=(big_integer const &a, big_integer const &b) {
    return a == b || a > b;
}

big_integer operator+(big_integer a, big_integer const &b) {
    return a += b;
}

big_integer operator-(big_integer a, big_integer const &b) {
    return a -= b;
}

big_integer operator*(big_integer a, big_integer const &b) {
    return a *= b;
}

big_integer operator/(big_integer a, big_integer const &b) {
    return a /= b;
}

big_integer operator%(big_integer a, big_integer const &b) {
    return a %= b;
}

big_integer operator&(big_integer a, big_integer const &b) {
    return a &= b;
}

big_integer operator|(big_integer a, big_integer const &b) {
    return a |= b;
}

big_integer operator^(big_integer a, big_integer const &b) {
    return a ^= b;
}

big_integer operator<<(big_integer a, int bits) {
    return a <<= bits;
}

big_integer operator>>(big_integer a, int bits) {
    return a >>= bits;
}

std::string big_integer::debugToString()const {
    std::string str = "";
    for (size_t i = 0; i < digits.size(); i++) {
        uint32_t dig = digits[i];
        for (size_t i = 0; i < 32; i++) {
            if (dig % 2 == 0) {
                str = "0 " + str;
            } else {
                str = "1 " + str;
            }
            dig /= 2;
        }
        str = "\n" + str;
        //str = std::to_string(digits[i]) + " " + str;
    }
    str += "\n";
    str = (isNegative ? "- " : "  ") + str;
    return str;
}

std::string to_string(big_integer const &a) {
    //a.digits.toString();
    if (a.digits.size() == 0) {
        return "0";
    }
    big_integer b(a);
    std::string res = "";
    while (b != 0) {
        big_integer c = b % 10;
        if (c.digits.size() == 0) {
            res = '0' + res;
        } else {
            res = std::to_string(c.digits[0]) + res;
        }
        b /= 10;
    }
    if (a.isNegative) {
        res = '-' + res;
    }
    return res;
}

std::ostream &operator<<(std::ostream &s, big_integer const &a) {
    return s << to_string(a);
}
