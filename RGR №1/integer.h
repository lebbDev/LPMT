#include <iostream>

struct Pair
{
    int rank;
    int digit;
    friend Pair operator+(const Pair& p1, const Pair& p2);
    friend std::ostream& operator<<(std::ostream& stream, const Pair& p);
};

class Iterator
{
    Pair* ptr_;
public:
    Pair &operator*()
    {
        return *ptr_;
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
    friend class Integer;
};

class Integer
{
    int length_;
    Pair* number_;

public:
    Integer();
    Integer(int num);
    Integer(const Integer& num);
    ~Integer();
    Integer& operator=(const Integer& num);
    friend Integer operator+(const Integer& num1, const Integer& num2);
    Integer operator-(const Integer& num);
    Integer operator*(const Integer& num);
    Integer operator/(const Integer& num);
    Integer operator%(const Integer& num);
    Pair& operator[](int index);
    bool operator==(const Integer& num);
    bool operator!=(const Integer& num);
    bool operator<(const Integer& num);
    bool operator>(const Integer& num);
    bool operator<=(const Integer& num);
    bool operator>=(const Integer& num);
    friend std::ostream& operator<<(std::ostream& stream, const Integer& num);
    friend std::istream& operator>>(std::istream& stream, Integer& num);
    Iterator begin();
    Iterator end();
};