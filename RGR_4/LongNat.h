#pragma once
#include <iostream>

struct Pair
{
    int rank;
    int digit;
    friend Pair operator+(const Pair& p1, const Pair& p2);
};

class Iterator
{
    Pair* ptr_;
public:
    int &operator*()
    {
        return ptr_->digit;
    }
    bool operator==(const Iterator &it)
    {
        return (ptr_ == it.ptr_);
    }
    bool operator!=(const Iterator &it)
    {
        return (ptr_ != it.ptr_);
    }
    Iterator &operator++()
    {
        ptr_++;
        return *this;
    }
    Iterator operator++(int)
    {
        Iterator old = *this;
        ptr_++;
        return old;
    }
    friend class LongNat;
};

class LongNat
{
    int length_;
    Pair* number_;

public:
    LongNat();
    LongNat(int num);
    LongNat(const LongNat& num);
    ~LongNat();
    LongNat& operator=(const LongNat& num2);
    friend int lenOutZero(const LongNat& num);
    friend int GetDig(const LongNat& num, int index);
    int GetLength();
    int GetRealLength();
    friend LongNat operator+(const LongNat& num1, const LongNat& num2);
    friend LongNat operator-(const LongNat& num1, const LongNat& num2);
    friend LongNat operator*(const LongNat& num1, const LongNat& num2);
    friend LongNat operator/(const LongNat& num1, const LongNat& num2);
    friend LongNat operator%(const LongNat& num1, const LongNat& num2);
    Pair& operator[](int index);
    operator bool();
    friend bool operator==(const LongNat& num1, const LongNat& num2);
    friend bool operator!=(const LongNat& num1, const LongNat& num2);
    friend bool operator<(const LongNat& num1, const LongNat& num2);
    friend bool operator>(const LongNat& num1, const LongNat& num2);
    friend bool operator<=(const LongNat& num1, const LongNat& num2);
    friend bool operator>=(const LongNat& num1, const LongNat& num2);
    friend std::ostream& operator<<(std::ostream& stream, const LongNat& num);
    friend std::istream& operator>>(std::istream& stream, LongNat& num);
    Iterator begin();
    Iterator end();
};