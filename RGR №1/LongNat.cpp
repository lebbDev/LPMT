#include "LongNat.h"

Iterator LongNat::begin()
{
    Iterator it;
    it.ptr_ = number_;
    return it;
}

Iterator LongNat::end()
{
    Iterator it;
    it.ptr_ = &number_[length_];
    return it;
}

Pair operator+(const Pair& p1, const Pair& p2)
{
    Pair res;
    res.rank = p1.rank;
    res.digit = p1.digit + p2.digit;
    return res;
}

LongNat::LongNat()
{
    length_ = 0;
    number_ = nullptr;
}

int lenOutZero(const LongNat& num)
{
    int len = 0;
    for (int i = 0; i < num.length_; i++)
        if (num.number_[i].digit != 0) len++;

    return len;
}

int lenOutZero(int num)
{
    int len = 0;
    for (int i = num; i > 0; i /= 10)
        if (!(i % 10 == 0)) len++;
    
    return len;
}

LongNat::LongNat(int num)
{
    length_ = lenOutZero(num);
    number_ = new Pair[length_];
    
    for (int ost, i = 0, j = 0; num > 0; i++, num /= 10)
    {
        if ((ost = num % 10) == 0) continue;
        
        number_[j].rank = i;
        number_[j].digit = ost;
        j++;
    }
}

LongNat::LongNat(const LongNat& num)
{
    length_ = num.length_;
    
    if (length_ != 0)
    {
        number_ = new Pair[length_];

        for (int i = 0; i < length_; i++)
            number_[i] = num.number_[i];
    }
    else
        number_ = nullptr;
}

LongNat::~LongNat()
{
    delete[] number_;
}

LongNat& LongNat::operator=(const LongNat& num)
{
    if (this == &num) return *this;

    length_ = num.length_;
    delete[] number_;

    if (length_ != 0)
    {
        number_ = new Pair[length_];

        for (int i = 0; i < length_; i++)
            number_[i] = num.number_[i];
    }
    else
        number_ = nullptr;
    
    return *this;
}

bool operator==(const LongNat& num1, const LongNat& num2)
{
    if (&num1 == &num2) return true;
    if (num1.length_ != num2.length_) return false;

    for (int i = 0; i < num1.length_; i++)
    {
        if (num1.number_[i].rank != num2.number_[i].rank || num1.number_[i].digit != num2.number_[i].digit)
            return false;
    }
    return true;
}

bool operator!=(const LongNat& num1, const LongNat& num2)
{
    return !(num1 == num2);
}

bool operator<(const LongNat& num1, const LongNat& num2)
{
    if (num1.length_ == 0)
        return true;
    else if (num2.length_ == 0)
        return false;

    if (num1.number_[num1.length_ - 1].rank < num2.number_[num2.length_ - 1].rank)
        return true;
    else if (num1.number_[num1.length_ - 1].rank > num2.number_[num2.length_ - 1].rank)
        return false;
    else
        for (int i = num1.number_[num1.length_ - 1].rank; i >= 0; i--)
        {
            if (GetDig(num1, i) < GetDig(num2, i))
                return true;
            if (GetDig(num1, i) > GetDig(num2, i))
                return false;
        }

    return false;
}

bool operator<=(const LongNat& num1, const LongNat& num2)
{
    return (num1 < num2 || num1 == num2);
}

bool operator>(const LongNat& num1, const LongNat& num2)
{
    return !(num1 <= num2);
}

bool operator>=(const LongNat& num1, const LongNat& num2)
{
    return !(num1 < num2);
}

int& min(int num1, int num2)
{
    if (num1 > num2)
        return num2;
    else
        return num1;
}

int& max(int num1, int num2)
{
    if (num1 < num2)
        return num2;
    else
        return num1;
}

int GetDig(const LongNat& num, int index)
{
    if (index > num.number_[num.length_ - 1].rank || index < 0)
        return -1;

    for (int i = 0; i < num.length_; i++)
    {
        if (num.number_[i].rank > index)
            break;
        if (num.number_[i].rank == index)
            return num.number_[i].digit;
    }
    return 0;
}

LongNat operator+(const LongNat& num1, const LongNat& num2)
{
    LongNat curLong;
    if (num1.number_[num1.length_ - 1].rank + 1 > num2.number_[num2.length_ - 1].rank + 1)
        curLong.length_ = (num1.number_[num1.length_ - 1].rank + 1) * 2;
    else
        curLong.length_ = (num2.number_[num2.length_ - 1].rank + 1) * 2;

    curLong.number_ = new Pair[curLong.length_];

    for (int m = 0; m < curLong.length_; m++)
    {
        curLong.number_[m].digit = 0;
        curLong.number_[m].rank = m;
    }

    int carry = 0, k = 0;
    int minlen = min(num1.number_[num1.length_ - 1].rank + 1, num2.number_[num2.length_ - 1].rank + 1);
    int maxlen = max(num1.number_[num1.length_ - 1].rank + 1, num2.number_[num2.length_ - 1].rank + 1);

    for (int i = 0; i < minlen; i++, k++)
    {
        int cur_sum = carry + GetDig(num1, i) + GetDig(num2, i);
        curLong.number_[k].digit = cur_sum % 10;
        carry = cur_sum / 10;
    }

    for (int i = minlen; i < maxlen; i++, k++)
    {
        if (num1.number_[num1.length_ - 1].rank > num2.number_[num2.length_ - 1].rank)
        {
            int cur_sum = carry + GetDig(num1, i);
            curLong.number_[k].digit = cur_sum % 10;
            carry = cur_sum / 10;
        }
        else
        {
            int cur_sum = carry + GetDig(num2, i);
            curLong.number_[k].digit = cur_sum % 10;
            carry = cur_sum / 10;
        }
    }
    if (carry != 0)
        curLong.number_[k].digit = carry;

    LongNat res;
    res.length_ = lenOutZero(curLong);

    res.number_ = new Pair[res.length_];

    for (int i = 0, j = 0; j < curLong.length_; j++)
    {
        if (curLong.number_[j].digit == 0) continue;

        res.number_[i] = curLong.number_[j];
        i++;
    }
    return res;
}

LongNat operator-(const LongNat& num1, const LongNat& num2)
{
    if (num1 <= num2)
        throw 3;

    int len1, len2;
    len1 = num1.number_[num1.length_ - 1].rank + 1;
    len2 = num2.number_[num2.length_ - 1].rank + 1;

    LongNat curLong;
    if (len1 > len2)
        curLong.length_ = len1 + 1;
    else
        curLong.length_ = len2 + 1;

    curLong.number_ = new Pair[curLong.length_];

    for (int m = 0; m < curLong.length_; m++)
    {
        curLong.number_[m].digit = 0;
        curLong.number_[m].rank = m;
    }

    int carry = 0, k = 0;

    for (int i = 0; i < len2; i++, k++)
    {
        int cur_diff = GetDig(num1, i) - GetDig(num2, i) - carry;
        if (cur_diff < 0)
        {
            cur_diff += 10;
            carry = 1;
        }
        else {
            carry = 0;
        }
        curLong.number_[k].digit = cur_diff;
    }

    for (int i = len2; i < len1; i++, k++)
    {
        int dig = GetDig(num1, i);
        if (dig - carry < 0)
        {
            curLong.number_[k].digit = dig + 10 - carry;
            carry = 1;
        }
        else
        {
            curLong.number_[k].digit = dig - carry;
            carry = 0;
        }
    }

    LongNat res;
    res.length_ = lenOutZero(curLong);

    res.number_ = new Pair[res.length_];

    for (int i = 0, j = 0; j < curLong.length_; j++)
    {
        if (curLong.number_[j].digit == 0) continue;

        res.number_[i] = curLong.number_[j];
        i++;
    }
    return res;
}

LongNat operator*(const LongNat& num1, const LongNat& num2)
{
    if (num1.length_ == 0 || num2.length_ == 0)
        throw 2;

    LongNat curLong;
    curLong.length_ = 1 + (num1.number_[num1.length_ - 1].rank + 1 * num2.number_[num2.length_ - 1].rank + 1);
    curLong.number_ = new Pair[curLong.length_];

    for (int m = 0; m < curLong.length_; m++)
    {
        curLong.number_[m].digit = 0;
        curLong.number_[m].rank = m;
    }

    for (int i = 0; i < num1.number_[num1.length_ - 1].rank + 1; i++)
    {
        int carry = 0;
        for (int j = 0; j < num2.number_[num2.length_ - 1].rank + 1; j++)
        {
            int cur = curLong.number_[i + j].digit + GetDig(num1, i) * GetDig(num2, j) + carry;
            carry = cur / 10;
            curLong.number_[i + j].digit = cur % 10;
        }
        int cur = num2.number_[num2.length_ - 1].rank + 1;
        while (carry)
        {
            curLong.number_[i + cur].digit = carry % 10;
            cur++;
            carry /= 10;
        }
    }
    
    LongNat res;
    res.length_ = lenOutZero(curLong);

    if (res.length_ == 0)
    {
        res.number_ = nullptr;
        return res;
    }

    res.number_ = new Pair[res.length_];

    for (int i = 0, j = 0; j < curLong.length_; j++)
    {
        if (curLong.number_[j].digit == 0) continue;

        res.number_[i] = curLong.number_[j];
        i++;
    }
    return res;
}

LongNat operator/(const LongNat& num1, const LongNat& num2)
{
    if (num2 > num1 || num2.length_ == 0)
        throw 4;

    LongNat current;

    current.length_ = 1 + (num1.number_[num1.length_ - 1].rank - num2.number_[num2.length_ - 1].rank);
    current.number_ = new Pair[current.length_];
    
    for (int m = 0; m < current.length_; m++)
    {
        current.number_[m].digit = 0;
        current.number_[m].rank = m;
    }

    for (int i = current.length_ - 1; i >= 0; i--)
    {
        while (num2 * current <= num1) {
            current.number_[i].digit++;
        }
        current.number_[i].digit--;
    }

    LongNat res;
    res.length_ = lenOutZero(current);
    res.number_ = new Pair[res.length_];

    for (int i = 0, j = 0; j < current.length_; j++)
    {
        if (current.number_[j].digit == 0) continue;

        res.number_[i] = current.number_[j];
        i++;
    }
    return res;
}

LongNat operator%(const LongNat& num1, const LongNat& num2)
{
    LongNat res = num1 - (num1 / num2) * num2;
    return res;
}

Pair& LongNat::operator[](int index)
{
    return number_[index];
}

std::ostream& operator<<(std::ostream& stream, const LongNat& num)
{
    for (int i = num.length_ - 1; i >= 0; i--)
    {
        stream << num.number_[i].digit;
        if (i > 0)
        {
            if (num.number_[i - 1].rank != num.number_[i].rank - 1)
            {
                int k = num.number_[i].rank - 1;
                while (k > num.number_[i - 1].rank)
                {
                    stream << '0';
                    k--;
                }
            }
        }
    }

    if (num.number_[0].rank > 0)
    {
        int m = 0;
        int j = num.number_[0].rank;

        while (m < j)
        {
            stream << '0';
            m++;
        }
    }
    return stream;
}

int strlen(char* str)
{
    int len = 0;
    while (str[len] != '\0')
        len++;

    return len;
}

std::istream& operator>>(std::istream& stream, LongNat& num)
{
    while (stream.peek() == ' ')
        stream.ignore();

    char* s = new char[1000];
    stream >> s;

    if (stream.fail())
        throw 1;

    LongNat res;
    int len = strlen(s);

    res.number_ = new Pair[len];
    int k = 0;

    for (int i = 0, j = len - 1; i < len; i++, j--)
    {
        if (s[j] == '0') continue;

        res.number_[k].rank = i;
        res.number_[k].digit = s[j] - '0';
        k++;
    }
    res.length_ = k;

    num = res;
    delete[] s;
    return stream;
}

/* void QuickSort(Pair* pair, int len)
{
    if (len <= 1) return;

    int l = 0, r = len - 1;
    Pair mid = pair[len / 2];

    do {
        while (pair[l].digit < mid.digit)
        {
            l++;
        }
        while (pair[r].digit > mid.digit)
        {
            r--;
        }

        if (l <= r)
        {
            Pair tmp = pair[l];
            pair[l] = pair[r];
            pair[r] = tmp;

            l++;
            r--;
        }
    } while (l <= r);

    if (r > 0)
        QuickSort(pair, r + 1);
    
    if (l < len)
        QuickSort(&pair[l], len - l);
}

Pair* SortPair(Pair* pair, int len)
{
    QuickSort(pair, len);
    return pair;
} */