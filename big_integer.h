#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <limits>
#include <string>
#include <cmath>
#include <iostream>
#include <vector>


struct big_integer {

    big_integer honNegation();
    std::string debugToString();

    big_integer();
    big_integer(int a);
    big_integer(big_integer const &other);
    explicit big_integer(std::string const &str);
    ~big_integer();

    big_integer &operator=(big_integer const &other);

    big_integer &operator+=(big_integer const &rhs);
    big_integer &operator-=(big_integer const &rhs);
    big_integer &operator*=(big_integer const &rhs);
    big_integer &operator/=(big_integer const &rhs);
    big_integer &operator%=(big_integer const &rhs);

    big_integer &operator&=(big_integer const &rhs);
    big_integer &operator|=(big_integer const &rhs);
    big_integer &operator^=(big_integer const &rhs);

    big_integer &operator<<=(int rhs);
    big_integer &operator>>=(int rhs);

    big_integer operator+() const;
    big_integer operator-() const;
    big_integer operator~() const;

    big_integer &operator++();
    big_integer operator++(int);
    big_integer &operator--();
    big_integer operator--(int);

    friend bool operator==(big_integer const &a, big_integer const &b);
    friend bool operator!=(big_integer const &a, big_integer const &b);
    friend bool operator<(big_integer const &a, big_integer const &b);
    friend bool operator>(big_integer const &a, big_integer const &b);
    friend bool operator<=(big_integer const &a, big_integer const &b);
    friend bool operator>=(big_integer const &a, big_integer const &b);

    friend std::string to_string(big_integer const &a);
    friend big_integer abs(big_integer const &a);

private:
    bool isNegative;
    std::vector<uint32_t> digits;

    big_integer& minimise();
    void toTwosComplement();
    void toSignedForm();

    friend big_integer long_unsigned_division(big_integer const &a, big_integer const &b);
    friend big_integer add_long_int_unsigned(big_integer const &a, uint32_t const &b);
    friend big_integer mul_long_int_unsigned(big_integer const &a, uint32_t const &b);
};

big_integer operator+(big_integer a, big_integer const &b);
big_integer operator-(big_integer a, big_integer const &b);
big_integer operator*(big_integer a, big_integer const &b);
big_integer operator/(big_integer a, big_integer const &b);
big_integer operator%(big_integer a, big_integer const &b);

big_integer operator&(big_integer a, big_integer const &b);
big_integer operator|(big_integer a, big_integer const &b);
big_integer operator^(big_integer a, big_integer const &b);
big_integer operator<<(big_integer a, int bits);
big_integer operator>>(big_integer a, int bits);

std::ostream &operator<<(std::ostream &s, big_integer const &a);

big_integer add_long_int_unsigned(big_integer const &a, uint32_t const &b);
big_integer mul_long_int_unsigned(big_integer const &a, uint32_t const &b);
big_integer long_unsigned_division(big_integer const &a, big_integer const &b);
big_integer abs(big_integer const &a);

#endif // BIG_INTEGER_H
