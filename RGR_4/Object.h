#pragma once

enum class ValueType { Integer, LongNat };

class Object
{
	void* valuePointer;
	ValueType type;

public:
	Object();
	Object(void* ptr, ValueType t);
	void clear();
	Object(const Object& obj);
	Object(Object&& obj);
	Object& operator=(const Object& obj);
	Object& operator=(Object&& obj);
	~Object();
	ValueType get_type();
	void* get_ptr();
	int getDigitLong(int index);
	void set(void* ptr, ValueType _type);
	Object operator+(const Object& obj);
	Object operator-(const Object& obj);
	Object operator*(const Object& obj);
	Object operator/(const Object& obj);
	Object operator%(const Object& obj);
	bool operator==(const Object& obj);
	bool operator!=(const Object& obj);
	bool operator<(const Object& obj);
	bool operator<=(const Object& obj);
	bool operator>(const Object& obj);
	bool operator>=(const Object& obj);
};