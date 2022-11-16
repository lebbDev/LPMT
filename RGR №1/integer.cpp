#include "integer.h"

Iterator Integer::begin()
{
    Iterator it;
    it.ptr_ = number_;
    return it;
}

Iterator Integer::end()
{
    Iterator it;
    it.ptr_ = &number_[length_];
    return it;
}

std::ostream& operator<<(std::ostream& stream, const Pair& p)
{
    stream << '(';
    stream << p.rank << ',' << p.digit << ')';
    return stream;
}

Pair operator+(const Pair& p1, const Pair& p2)
{
    Pair res;
    res.rank = p1.rank;
    res.digit = p1.digit + p2.digit;
    return res;
}

Integer::Integer()
{
    length_ = 0;
    number_ = nullptr;
}

int lenOutZero(int num)
{
    int len = 0;

    for (int i = num; i > 0; i /= 10)
    {
        if (!(i % 10 == 0)) len++;
    }
    
    return len;
}

void QuickSort(Pair* pair, int len)
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
}

Integer::Integer(int num)
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

Integer::Integer(const Integer& num)
{
    length_ = num.length_;
    number_ = new Pair[length_];

    for (int i = 0; i < length_; i++)
    {
        number_[i] = num.number_[i];
    }
}

Integer::~Integer()
{
    delete[] number_;
}

Integer& Integer::operator=(const Integer& num)
{
    if (this == &num) return *this;

    length_ = num.length_;
    delete[] number_;
    number_ = new Pair[length_];

    for (int i = 0; i < length_; i++)
    {
        number_[i] = num.number_[i];
    }
    return *this;
}

bool Integer::operator==(const Integer& num)
{
    if (this == &num) return true;
    if (length_ != num.length_) return false;

    for (int i = 0; i < length_; i++)
    {
        if (number_[i].rank != num.number_[i].rank || number_[i].digit != num.number_[i].digit)
            return false;
    }
    return true;
}

bool Integer::operator!=(const Integer& num)
{
    return !(*this == num);
}

Pair* ExtendArr(Pair* num, int len)
{
    Pair* tmp = new Pair[len];

    for (int i = 0; i < len - 1; i++)
    {
        tmp[i] = num[i];
    }
    delete[] num;
    return tmp;
}

Integer operator+(const Integer& num1, const Integer& num2)
{
    int maxLen;
    if (num1.length_ > num2.length_)
        maxLen = num1.length_;
    else
        maxLen = num2.length_;

    Integer res;

    res.number_ = new Pair[maxLen * 2 + 1];

    bool isDebt = false;
    int i = 0, j = 0, k = 0;

    printf("i j k\n");
    while (true)
    {
        printf("%d %d %d\n", i, j, k);
        if (i == num1.length_ || j == num2.length_)
        {
            if (i != j)
            {
                if (i < j)
                {
                    if (isDebt)
                        res.number_[k] = num1.number_[i] + res.number_[k];
                    else
                        res.number_[k] = num1.number_[i];
                    k++;
                }
                else
                {
                    if (isDebt)
                        res.number_[k] = num2.number_[j] + res.number_[k];
                    else
                        res.number_[k] = num2.number_[j];
                    k++;
                }
            }
            break;
        }


        if (num1.number_[i].rank != num2.number_[j].rank)
        {
            if (num1.number_[i].rank > num2.number_[j].rank)
            {
                res.number_[k] = num2.number_[j];
                j++;
                k++;
                continue;
            }
            else
            {
                res.number_[k] = num2.number_[i];
                i++;
                k++;
                continue;
            }
        }
        else
        {
            res.number_[k].rank = num1.number_[i].rank;
            res.number_[k].digit = num1.number_[i].digit + num2.number_[j].digit;

            if (res.number_[k].digit >= 10)
            {
                res.number_[k].digit = res.number_[k].digit % 10;
                res.number_[k + 1].digit++;
                isDebt = true;
            }
        }
        k++;
        i++;
        j++;
    }
    
    Integer resF;
    resF.length_ = k;
    resF.number_ = new Pair[resF.length_];

    for (int i = 0; i < k; i++)
    {
        //printf("%d", res.number_[i].digit);
        resF.number_[i] = res.number_[i];
    }

    return resF;
}


/* Integer operator+(const Integer& num1, const Integer& num2)
{
    int maxLen;
    if (num1.length_ > num2.length_)
        maxLen = num1.length_;
    else
        maxLen = num2.length_;

    Integer res;
    res.length_ = maxLen;
    printf("%d\n", maxLen);
    res.number_ = new Pair[maxLen];

    int debt = 0;
    bool isLost = false;
    bool isBorder = false;

    for (int i = 0, f = 0, s = 0;; i++)
    {
        printf("%d %d %d\n", i, f, s);
        if (i == maxLen || f == num1.length_ || s == num2.length_)
        {
            if (isBorder || isLost)
            {
                res.number_ = ExtendArr(res.number_, maxLen + 1);
                res.length_++;
                if (f < s)
                    res.number_[i] = num1.number_[f];
                else
                    res.number_[i] = num2.number_[s];
            }
            break;
        }
        
        while (num1.number_[f].rank != num2.number_[s].rank)
        {
            if (num1.number_[f].rank > num2.number_[s].rank)
            {
                res.number_[i] = num2.number_[s];
                s++, i++;
                //isSearchRank = true;
                continue;
            }
            else if (num1.number_[f].rank < num2.number_[s].rank)
            {
                res.number_[i] = num1.number_[f];
                f++, i++;
                //isSearchRank = true;
                continue;
            }

            if (f == num1.length_ || s == num2.length_)
            {
                isBorder = true;
                break;
            }
        }
        
        if (isBorder) continue;

        //isSearchRank = false;

        res.number_[i].rank = num1.number_[f].rank;
        res.number_[i].digit = num1.number_[f].digit + num2.number_[s].digit + debt;
        //printf("%d %d\n", res.number_[i].rank, res.number_[i].digit);
        if (res.number_[i].digit >= 10)
        {
            res.number_[i].digit = res.number_[i].digit % 10;
            debt = 1;
            if (i + 1 >= maxLen)
            {
                res.number_ = ExtendArr(res.number_, maxLen + 1);
                res.length_++;

                res.number_[i + 1].rank = num1.number_[i].rank + 1;
                res.number_[i + 1].digit = debt;
            }
        }
        if (!isBorder)
        {
            s++, f++;
            if (f == num1.length_ || s == num2.length_)
                isLost = true;
             if ((f != s) && (f == num1.length_ || s == num2.length_))
                isBorder = true;
        }
        
    }
    
    //std::cout<<res<<std::endl;
    return res;
} 
 */

Pair& Integer::operator[](int index)
{
    return number_[index];
}

std::ostream& operator<<(std::ostream& stream, const Integer& num)
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
    return stream;
}

/* Pair* removeZeros(Pair* num, int len)
{   if (num.)
    for (int i = len - 1; i >= 0; i--)
        {
           res.number_[i].rank = i;
        }
} */

int strlen(char* str)
{
    int len = 0;
    while (str[len] != '\0')
    {
        len++;
    }
    return len;
}

std::istream& operator>>(std::istream& stream, Integer& num)
{
    try
    {
        while (stream.peek() == ' ')
            stream.ignore();
    
        char* s = new char[1000];
        stream>>s;

        if (stream.fail()) throw 1;

        Integer res;
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
    catch (int y)
    {
        stream.setstate(std::ios::failbit);
        return stream;
    }
}