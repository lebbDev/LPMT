#include "Object.h"
#include "LongNat.h"

Object::Object()
{
	valuePointer = nullptr;
	type = ValueType::Integer;
}

Object::Object(void* ptr, ValueType t)
{
	valuePointer = ptr;
	type = t;
}

void Object::clear()
{
	if (valuePointer)
	{
		switch (type)
		{
		case ValueType::Integer: delete (int*)valuePointer; break;
		case ValueType::LongNat: delete (LongNat*)valuePointer; break;
		}
	}
	valuePointer = nullptr;
	type = ValueType::Integer;
}

Object::Object(const Object& obj)
{
	type = obj.type;
	switch (type)
	{
	case ValueType::Integer: valuePointer = new int(*(int*)obj.valuePointer); break;
	case ValueType::LongNat: valuePointer = new LongNat(*(LongNat*)obj.valuePointer); break;
	}
}

Object::Object(Object&& obj)
{
	type = obj.type;
	obj.type = ValueType::Integer;

	valuePointer = obj.valuePointer;
	obj.valuePointer = nullptr;
}

Object& Object::operator=(const Object& obj)
{
	if (this == &obj) return *this;

	clear();

	type = obj.type;
	switch (type)
	{
	case ValueType::Integer: valuePointer = new int(*(int*)obj.valuePointer); break;
	case ValueType::LongNat: valuePointer = new LongNat(*(LongNat*)obj.valuePointer); break;
	}

	return *this;
}

Object& Object::operator=(Object&& obj)
{
	if (this == &obj) return *this;

	clear();

	type = obj.type;
	obj.type = ValueType::Integer;

	valuePointer = obj.valuePointer;
	obj.valuePointer = nullptr;

	return *this;
}

Object::~Object()
{
	clear();
}

ValueType Object::get_type()
{
	return type;
}

void* Object::get_ptr()
{
	return valuePointer;
}

void Object::set(void* ptr, ValueType _type)
{
	clear();

	valuePointer = ptr;
	type = _type;
}

int Object::getDigitLong(int index)
{
	if (index > (*(LongNat*)valuePointer).GetRealLength() || index <= 0)
		throw 1;

	for (int i = 0; i < (*(LongNat*)valuePointer).GetLength(); i++)
	{
		if ((*(LongNat*)valuePointer)[i].rank == index - 1)
			return (*(LongNat*)valuePointer)[i].digit;
	}

	return 0;
}

Object Object::operator+(const Object& obj)
{
	int* int_ptr1 = nullptr;
	LongNat* ln_ptr1 = nullptr;

	switch (type)
	{
	case ValueType::Integer: int_ptr1 = (int*)valuePointer; break;
	case ValueType::LongNat: ln_ptr1 = (LongNat*)valuePointer; break;
	}

	int* int_ptr2 = nullptr;
	LongNat* ln_ptr2 = nullptr;

	switch (obj.type)
	{
	case ValueType::Integer: int_ptr2 = (int*)obj.valuePointer; break;
	case ValueType::LongNat: ln_ptr2 = (LongNat*)obj.valuePointer; break;
	}

	Object new_obj;

	if (int_ptr1 && int_ptr2)
	{
		new_obj.set(new int(*int_ptr1 + *int_ptr2), ValueType::Integer);
	}
	else if (ln_ptr1 && int_ptr2)
	{
		new_obj.set(new LongNat(*ln_ptr1 + LongNat(*int_ptr2)), ValueType::LongNat);
	}
	else if (int_ptr1 && ln_ptr2)
	{
		new_obj.set(new LongNat(LongNat(*int_ptr1) + *ln_ptr2), ValueType::LongNat);
	}
	else if (ln_ptr1 && ln_ptr2)
	{
		new_obj.set(new LongNat(*ln_ptr1 + *ln_ptr2), ValueType::LongNat);
	}

	return new_obj;
}

Object Object::operator-(const Object& obj)
{
	int* int_ptr1 = nullptr;
	LongNat* ln_ptr1 = nullptr;

	switch (type)
	{
	case ValueType::Integer: int_ptr1 = (int*)valuePointer; break;
	case ValueType::LongNat: ln_ptr1 = (LongNat*)valuePointer; break;
	}

	int* int_ptr2 = nullptr;
	LongNat* ln_ptr2 = nullptr;

	switch (obj.type)
	{
	case ValueType::Integer: int_ptr2 = (int*)obj.valuePointer; break;
	case ValueType::LongNat: ln_ptr2 = (LongNat*)obj.valuePointer; break;
	}

	Object new_obj;

	if (int_ptr1 && int_ptr2)
	{
		new_obj.set(new int(*int_ptr1 - *int_ptr2), ValueType::Integer);
	}
	else if (ln_ptr1 && int_ptr2)
	{
		new_obj.set(new LongNat(*ln_ptr1 - LongNat(*int_ptr2)), ValueType::LongNat);
	}
	else if (int_ptr1 && ln_ptr2)
	{
		new_obj.set(new LongNat(LongNat(*int_ptr1) - *ln_ptr2), ValueType::LongNat);
	}
	else if (ln_ptr1 && ln_ptr2)
	{
		new_obj.set(new LongNat(*ln_ptr1 - *ln_ptr2), ValueType::LongNat);
	}

	return new_obj;
}

Object Object::operator*(const Object& obj)
{
	int* int_ptr1 = nullptr;
	LongNat* ln_ptr1 = nullptr;

	switch (type)
	{
	case ValueType::Integer: int_ptr1 = (int*)valuePointer; break;
	case ValueType::LongNat: ln_ptr1 = (LongNat*)valuePointer; break;
	}

	int* int_ptr2 = nullptr;
	LongNat* ln_ptr2 = nullptr;

	switch (obj.type)
	{
	case ValueType::Integer: int_ptr2 = (int*)obj.valuePointer; break;
	case ValueType::LongNat: ln_ptr2 = (LongNat*)obj.valuePointer; break;
	}

	Object new_obj;

	if (int_ptr1 && int_ptr2)
	{
		new_obj.set(new int(*int_ptr1 * *int_ptr2), ValueType::Integer);
	}
	else if (ln_ptr1 && int_ptr2)
	{
		new_obj.set(new LongNat(*ln_ptr1 * LongNat(*int_ptr2)), ValueType::LongNat);
	}
	else if (int_ptr1 && ln_ptr2)
	{
		new_obj.set(new LongNat(LongNat(*int_ptr1) * *ln_ptr2), ValueType::LongNat);
	}
	else if (ln_ptr1 && ln_ptr2)
	{
		new_obj.set(new LongNat(*ln_ptr1 * *ln_ptr2), ValueType::LongNat);
	}

	return new_obj;
}

Object Object::operator/(const Object& obj)
{
	int* int_ptr1 = nullptr;
	LongNat* ln_ptr1 = nullptr;

	switch (type)
	{
	case ValueType::Integer: int_ptr1 = (int*)valuePointer; break;
	case ValueType::LongNat: ln_ptr1 = (LongNat*)valuePointer; break;
	}

	int* int_ptr2 = nullptr;
	LongNat* ln_ptr2 = nullptr;

	switch (obj.type)
	{
	case ValueType::Integer: int_ptr2 = (int*)obj.valuePointer; break;
	case ValueType::LongNat: ln_ptr2 = (LongNat*)obj.valuePointer; break;
	}

	Object new_obj;

	if (int_ptr1 && int_ptr2)
	{
		new_obj.set(new int(*int_ptr1 / *int_ptr2), ValueType::Integer);
	}
	else if (ln_ptr1 && int_ptr2)
	{
		new_obj.set(new LongNat(*ln_ptr1 / LongNat(*int_ptr2)), ValueType::LongNat);
	}
	else if (int_ptr1 && ln_ptr2)
	{
		new_obj.set(new LongNat(LongNat(*int_ptr1) / *ln_ptr2), ValueType::LongNat);
	}
	else if (ln_ptr1 && ln_ptr2)
	{
		new_obj.set(new LongNat(*ln_ptr1 / *ln_ptr2), ValueType::LongNat);
	}

	return new_obj;
}

Object Object::operator%(const Object& obj)
{
	int* int_ptr1 = nullptr;
	LongNat* ln_ptr1 = nullptr;

	switch (type)
	{
	case ValueType::Integer: int_ptr1 = (int*)valuePointer; break;
	case ValueType::LongNat: ln_ptr1 = (LongNat*)valuePointer; break;
	}

	int* int_ptr2 = nullptr;
	LongNat* ln_ptr2 = nullptr;

	switch (obj.type)
	{
	case ValueType::Integer: int_ptr2 = (int*)obj.valuePointer; break;
	case ValueType::LongNat: ln_ptr2 = (LongNat*)obj.valuePointer; break;
	}

	Object new_obj;

	if (int_ptr1 && int_ptr2)
	{
		new_obj.set(new int(*int_ptr1 % *int_ptr2), ValueType::Integer);
	}
	else if (ln_ptr1 && int_ptr2)
	{
		new_obj.set(new LongNat(*ln_ptr1 % LongNat(*int_ptr2)), ValueType::LongNat);
	}
	else if (int_ptr1 && ln_ptr2)
	{
		new_obj.set(new LongNat(LongNat(*int_ptr1) % *ln_ptr2), ValueType::LongNat);
	}
	else if (ln_ptr1 && ln_ptr2)
	{
		new_obj.set(new LongNat(*ln_ptr1 % *ln_ptr2), ValueType::LongNat);
	}

	return new_obj;
}

bool Object::operator==(const Object& obj)
{
	int* int_ptr1 = nullptr;
	LongNat* ln_ptr1 = nullptr;

	switch (type)
	{
	case ValueType::Integer: int_ptr1 = (int*)valuePointer; break;
	case ValueType::LongNat: ln_ptr1 = (LongNat*)valuePointer; break;
	}

	int* int_ptr2 = nullptr;
	LongNat* ln_ptr2 = nullptr;

	switch (obj.type)
	{
	case ValueType::Integer: int_ptr2 = (int*)obj.valuePointer; break;
	case ValueType::LongNat: ln_ptr2 = (LongNat*)obj.valuePointer; break;
	}

	if (int_ptr1 && int_ptr2)
	{
		return *int_ptr1 == *int_ptr2;
	}
	else if (ln_ptr1 && ln_ptr2)
	{
		return *ln_ptr1 == *ln_ptr2;
	}
	else if (ln_ptr1 && int_ptr2 || int_ptr1 && ln_ptr2)
		return false;

	throw 1;
}

bool Object::operator!=(const Object& obj)
{
	int* int_ptr1 = nullptr;
	LongNat* ln_ptr1 = nullptr;

	switch (type)
	{
	case ValueType::Integer: int_ptr1 = (int*)valuePointer; break;
	case ValueType::LongNat: ln_ptr1 = (LongNat*)valuePointer; break;
	}

	int* int_ptr2 = nullptr;
	LongNat* ln_ptr2 = nullptr;

	switch (obj.type)
	{
	case ValueType::Integer: int_ptr2 = (int*)obj.valuePointer; break;
	case ValueType::LongNat: ln_ptr2 = (LongNat*)obj.valuePointer; break;
	}

	if (int_ptr1 && int_ptr2)
	{
		return *int_ptr1 != *int_ptr2;
	}
	else if (ln_ptr1 && ln_ptr2)
	{
		return *ln_ptr1 != *ln_ptr2;
	}
	else if (ln_ptr1 && int_ptr2 || int_ptr1 && ln_ptr2)
		return true;

	throw 1;
}

bool Object::operator<(const Object& obj)
{
	int* int_ptr1 = nullptr;
	LongNat* ln_ptr1 = nullptr;

	switch (type)
	{
	case ValueType::Integer: int_ptr1 = (int*)valuePointer; break;
	case ValueType::LongNat: ln_ptr1 = (LongNat*)valuePointer; break;
	}

	int* int_ptr2 = nullptr;
	LongNat* ln_ptr2 = nullptr;

	switch (obj.type)
	{
	case ValueType::Integer: int_ptr2 = (int*)obj.valuePointer; break;
	case ValueType::LongNat: ln_ptr2 = (LongNat*)obj.valuePointer; break;
	}

	if (ln_ptr1 && ln_ptr2)
	{
		return *ln_ptr1 < *ln_ptr2;
	}
	else if (int_ptr1 && int_ptr2)
	{
		return *int_ptr1 < *int_ptr2;
	}

	throw 1;
}

bool Object::operator<=(const Object& obj)
{
	int* int_ptr1 = nullptr;
	LongNat* ln_ptr1 = nullptr;

	switch (type)
	{
	case ValueType::Integer: int_ptr1 = (int*)valuePointer; break;
	case ValueType::LongNat: ln_ptr1 = (LongNat*)valuePointer; break;
	}

	int* int_ptr2 = nullptr;
	LongNat* ln_ptr2 = nullptr;

	switch (obj.type)
	{
	case ValueType::Integer: int_ptr2 = (int*)obj.valuePointer; break;
	case ValueType::LongNat: ln_ptr2 = (LongNat*)obj.valuePointer; break;
	}

	if (ln_ptr1 && ln_ptr2)
	{
		return *ln_ptr1 <= *ln_ptr2;
	}
	else if (int_ptr1 && int_ptr2)
	{
		return *int_ptr1 <= *int_ptr2;
	}

	throw 1;
}

bool Object::operator>(const Object& obj)
{
	int* int_ptr1 = nullptr;
	LongNat* ln_ptr1 = nullptr;

	switch (type)
	{
	case ValueType::Integer: int_ptr1 = (int*)valuePointer; break;
	case ValueType::LongNat: ln_ptr1 = (LongNat*)valuePointer; break;
	}

	int* int_ptr2 = nullptr;
	LongNat* ln_ptr2 = nullptr;

	switch (obj.type)
	{
	case ValueType::Integer: int_ptr2 = (int*)obj.valuePointer; break;
	case ValueType::LongNat: ln_ptr2 = (LongNat*)obj.valuePointer; break;
	}

	if (ln_ptr1 && ln_ptr2)
	{
		return *ln_ptr1 > *ln_ptr2;
	}
	else if (int_ptr1 && int_ptr2)
	{
		return *int_ptr1 > *int_ptr2;
	}

	throw 1;
}

bool Object::operator>=(const Object& obj)
{
	int* int_ptr1 = nullptr;
	LongNat* ln_ptr1 = nullptr;

	switch (type)
	{
	case ValueType::Integer: int_ptr1 = (int*)valuePointer; break;
	case ValueType::LongNat: ln_ptr1 = (LongNat*)valuePointer; break;
	}

	int* int_ptr2 = nullptr;
	LongNat* ln_ptr2 = nullptr;

	switch (obj.type)
	{
	case ValueType::Integer: int_ptr2 = (int*)obj.valuePointer; break;
	case ValueType::LongNat: ln_ptr2 = (LongNat*)obj.valuePointer; break;
	}

	if (ln_ptr1 && ln_ptr2)
	{
		return *ln_ptr1 >= *ln_ptr2;
	}
	else if (int_ptr1 && int_ptr2)
	{
		return *int_ptr1 >= *int_ptr2;
	}

	throw 1;
}
