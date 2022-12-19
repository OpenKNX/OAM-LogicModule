#include "LogicValue.h"

#include <cstring>
#include <cstdlib>
#include <ctime>

LogicValue::LogicValue(bool value)
{
    _value.boolValue = value;
    _type = BoolType;
}

LogicValue::LogicValue(uint8_t value)
{
    _value.ucharValue = value;
    _type = UCharType;
}

LogicValue::LogicValue(uint16_t value)
{
    _value.ushortValue = value;
    _type = UShortType;
}

LogicValue::LogicValue(uint32_t value)
{
    _value.uintValue = value;
    _type = UIntType;
}

LogicValue::LogicValue(uint64_t value)
{
    _value.ulongValue = value;
    _type = ULongType;
}

LogicValue::LogicValue(int8_t value)
{
    _value.charValue = value;
    _type = CharType;
}

LogicValue::LogicValue(int16_t value)
{
    _value.shortValue = value;
    _type = ShortType;
}

LogicValue::LogicValue(int32_t value)
{
    _value.intValue = value;
    _type = IntType;
}

LogicValue::LogicValue(int64_t value)
{
    _value.longValue = value;
    _type = LongType;
}

LogicValue::LogicValue(double value)
{
    _value.doubleValue = value;
    _type = DoubleType;
}

LogicValue::LogicValue(const char* value)
{
    _value.stringValue = value;
    _type = StringType;
}

LogicValue::operator bool() const
{
    return boolValue();
}

LogicValue::operator uint8_t() const
{
    return ucharValue();
}

LogicValue::operator uint16_t() const
{
    return ushortValue();
}

LogicValue::operator uint32_t() const
{
    return uintValue();
}

LogicValue::operator uint64_t() const
{
    return ulongValue();
}

LogicValue::operator int8_t() const
{
    return charValue();
}

LogicValue::operator int16_t() const
{
    return shortValue();
}

LogicValue::operator int32_t() const
{
    return intValue();
}

LogicValue::operator int64_t() const
{
    return longValue();
}

LogicValue::operator double() const
{
    return doubleValue();
}

LogicValue::operator const char*() const
{
    return stringValue();
}

LogicValue& LogicValue::operator=(const bool value)
{
    _value.boolValue = value;
    _type = BoolType;
    return *this;
}

LogicValue& LogicValue::operator=(const uint8_t value)
{
    _value.ucharValue = value;
    _type = UCharType;
    return *this;
}

LogicValue& LogicValue::operator=(const uint16_t value)
{
    _value.ushortValue = value;
    _type = UShortType;
    return *this;
}

LogicValue& LogicValue::operator=(const uint32_t value)
{
    _value.uintValue = value;
    _type = UIntType;
    return *this;
}

LogicValue& LogicValue::operator=(const uint64_t value)
{
    _value.ulongValue = value;
    _type = ULongType;
    return *this;
}

LogicValue& LogicValue::operator=(const int8_t value)
{
    _value.charValue = value;
    _type = CharType;
    return *this;
}

LogicValue& LogicValue::operator=(const int16_t value)
{
    _value.shortValue = value;
    _type = ShortType;
    return *this;
}

LogicValue& LogicValue::operator=(const int32_t value)
{
    _value.intValue = value;
    _type = IntType;
    return *this;
}

LogicValue& LogicValue::operator=(const int64_t value)
{
    _value.longValue = value;
    _type = LongType;
    return *this;
}

LogicValue& LogicValue::operator=(const double value)
{
    _value.doubleValue = value;
    _type = DoubleType;
    return *this;
}

LogicValue& LogicValue::operator=(const char* value)
{
    _value.stringValue = value;
    _type = StringType;
    return *this;
}

void LogicValue::setInternal(const uint64_t value)
{
    switch (_type)
    {
        case BoolType:
            _value.boolValue = (value != 0);
            break;
        case UCharType:
            _value.ucharValue = value;
            break;
        case UShortType:
            _value.ushortValue = value;
            break;
        case UIntType:
            _value.uintValue = value;
            break;
        case ULongType:
            _value.ulongValue = value;
            break;
        case CharType:
            _value.charValue = value;
            break;
        case ShortType:
            _value.shortValue = value;
            break;
        case IntType:
            _value.intValue = value;
            break;
        case LongType:
            _value.longValue = value;
            break;
        case DoubleType:
            _value.doubleValue = value;
            break;
    }
}

void LogicValue::setInternal(const int64_t value)
{
    switch (_type)
    {
        case BoolType:
            _value.boolValue = (value != 0);
            break;
        case UCharType:
            _value.ucharValue = value;
            break;
        case UShortType:
            _value.ushortValue = value;
            break;
        case UIntType:
            _value.uintValue = value;
            break;
        case ULongType:
            _value.ulongValue = value;
            break;
        case CharType:
            _value.charValue = value;
            break;
        case ShortType:
            _value.shortValue = value;
            break;
        case IntType:
            _value.intValue = value;
            break;
        case LongType:
            _value.longValue = value;
            break;
        case DoubleType:
            _value.doubleValue = value;
            break;
    }
}

void LogicValue::setInternal(const double value)
{
    switch (_type)
    {
        case BoolType:
            _value.boolValue = (value != 0);
            break;
        case UCharType:
            _value.ucharValue = value;
            break;
        case UShortType:
            _value.ushortValue = value;
            break;
        case UIntType:
            _value.uintValue = value;
            break;
        case ULongType:
            _value.ulongValue = value;
            break;
        case CharType:
            _value.charValue = value;
            break;
        case ShortType:
            _value.shortValue = value;
            break;
        case IntType:
            _value.intValue = value;
            break;
        case LongType:
            _value.longValue = value;
            break;
        case DoubleType:
            _value.doubleValue = value;
            break;
    }
}

void LogicValue::setInternal(const char* value)
{
    switch (_type)
    {
        case CharType:
            _value.stringValue = value;
            break;
    }
}

LogicValue& LogicValue::operator++()
{
    switch (_type)
    {
        case BoolType:
            _value.boolValue = !_value.boolValue;
            return *this;
        case UCharType:
        case UShortType:
        case UIntType:
        case ULongType:
            setInternal(ulongValue() + 1);
            return *this;
        case CharType:
        case ShortType:
        case IntType:
        case LongType:
            setInternal(longValue() + 1);
            return *this;
        case DoubleType:
            setInternal(doubleValue() + 1);
            return *this;
    }
    return *this;
}

LogicValue& LogicValue::operator+=(const int32_t value)
{
    switch (_type)
    {
        case UCharType:
        case UShortType:
        case UIntType:
        case ULongType:
            setInternal(ulongValue() + value);
            return *this;
        case CharType:
        case ShortType:
        case IntType:
        case LongType:
            setInternal(longValue() + value);
            return *this;
        case DoubleType:
            setInternal(doubleValue() + value);
            return *this;
    }
    return *this;
}

LogicValue& LogicValue::operator+=(const float value)
{
    switch (_type)
    {
        case UCharType:
        case UShortType:
        case UIntType:
        case ULongType:
            setInternal(ulongValue() + value);
            return *this;
        case CharType:
        case ShortType:
        case IntType:
        case LongType:
            setInternal(longValue() + value);
            return *this;
        case DoubleType:
            setInternal(doubleValue() + value);
            return *this;
    }
    return *this;
}

LogicValue LogicValue::operator+(LogicValue const& value)
{
    if (_type == DoubleType || value._type == DoubleType) 
    {
        LogicValue lValue = doubleValue() + value.doubleValue();
        return lValue;
    } 
    else
    {
        LogicValue lValue = longValue() + value.longValue();
        return lValue;
    } 
}

LogicValue LogicValue::operator-(LogicValue const& value)
{
    if (_type == DoubleType || value._type == DoubleType) 
    {
        LogicValue lValue = doubleValue() - value.doubleValue();
        return lValue;
    } 
    else
    {
        LogicValue lValue = longValue() - value.longValue();
        return lValue;
    } 
}

LogicValue LogicValue::operator*(LogicValue const& value)
{
    if (_type == DoubleType || value._type == DoubleType) 
    {
        LogicValue lValue = doubleValue() * value.doubleValue();
        return lValue;
    } 
    else
    {
        LogicValue lValue = longValue() * value.longValue();
        return lValue;
    } 
}

LogicValue LogicValue::operator/(LogicValue const& value)
{
    if (_type == DoubleType || value._type == DoubleType) 
    {
        LogicValue lValue = doubleValue() / value.doubleValue();
        return lValue;
    } 
    else
    {
        LogicValue lValue = longValue() / value.longValue();
        return lValue;
    } 
}




LogicValue LogicValue::operator-(const int32_t value)
{
    LogicValue lValue = false;
    switch (_type)
    {
        case UCharType:
        case UShortType:
        case UIntType:
        case ULongType:
            lValue._type = _type;
            lValue.setInternal(ulongValue() - value);
            return lValue;
        case CharType:
        case ShortType:
        case IntType:
        case LongType:
            lValue._type = _type;
            setInternal(longValue() - value);
            return lValue;
        case DoubleType:
            lValue._type = _type;
            setInternal(doubleValue() - value);
            return lValue;
    }
    return lValue;
}

LogicValue LogicValue::operator-(const float value)
{
    LogicValue lValue = false;
    switch (_type)
    {
        case UCharType:
        case UShortType:
        case UIntType:
        case ULongType:
            lValue._type = _type;
            lValue.setInternal(ulongValue() - value);
            return lValue;
        case CharType:
        case ShortType:
        case IntType:
        case LongType:
            lValue._type = _type;
            setInternal(longValue() - value);
            return lValue;
        case DoubleType:
            lValue._type = _type;
            setInternal(doubleValue() - value);
            return lValue;
    }
    return lValue;
}

bool LogicValue::operator>(LogicValue const& value)
{
    if (_type == DoubleType || value._type == DoubleType)
        return (doubleValue() >= value.doubleValue());
    else
        return (longValue() >= value.longValue());
}

bool LogicValue::operator<(LogicValue const& value)
{
    if (_type == DoubleType || value._type == DoubleType)
        return (doubleValue() <= value.doubleValue());
    else
        return (longValue() <= value.longValue());
}

bool LogicValue::operator>=(LogicValue const& value)
{
    if (_type == DoubleType || value._type == DoubleType)
        return (doubleValue() >= value.doubleValue());
    else
        return (longValue() >= value.longValue());
}

bool LogicValue::operator<=(LogicValue const& value)
{
    if (_type == DoubleType || value._type == DoubleType)
        return (doubleValue() <= value.doubleValue());
    else
        return (longValue() <= value.longValue());
}

bool LogicValue::boolValue() const
{
    switch (_type)
    {
        case BoolType:
            return _value.boolValue;
        case UCharType:
        case UShortType:
        case UIntType:
        case ULongType:
        case CharType:
        case ShortType:
        case IntType:
        case LongType:
            return longValue() != 0;
        case DoubleType:
            return _value.doubleValue != 0;
        case StringType:
            return strcmp(_value.stringValue, "true") == 0 || strcmp(_value.stringValue, "True") == 0 || longValue() != 0 || doubleValue() != 0;
    }
    return 0;
}

uint8_t LogicValue::ucharValue() const
{
    switch (_type)
    {
        case UCharType:
            return _value.ucharValue;
        case BoolType:
        case UShortType:
        case UIntType:
        case ULongType:
            return (uint8_t)ulongValue();
        case CharType:
        case ShortType:
        case IntType:
        case LongType:
        case DoubleType:
        case StringType:
            return (uint8_t)longValue();
    }
    return 0;
}

uint16_t LogicValue::ushortValue() const
{
    switch (_type)
    {
        case UShortType:
            return _value.ushortValue;
        case BoolType:
        case UCharType:
        case UIntType:
        case ULongType:
            return (uint16_t)ulongValue();
        case CharType:
        case ShortType:
        case IntType:
        case LongType:
        case DoubleType:
        case StringType:
            return (uint16_t)longValue();
    }
    return 0;
}

uint32_t LogicValue::uintValue() const
{
    switch (_type)
    {
        case UIntType:
            return _value.uintValue;
        case BoolType:
        case UCharType:
        case UShortType:
        case ULongType:
            return (uint32_t)ulongValue();
        case CharType:
        case ShortType:
        case IntType:
        case LongType:
        case DoubleType:
        case StringType:
            return (uint32_t)longValue();
    }
    return 0;
}

uint64_t LogicValue::ulongValue() const
{
    switch (_type)
    {
        case ULongType:
            return _value.ulongValue;
        case BoolType:
            return _value.boolValue ? 1 : 0;
        case UCharType:
            return (uint64_t)_value.ucharValue;
        case UShortType:
            return (uint64_t)_value.ushortValue;
        case UIntType:
            return (uint64_t)_value.uintValue;
        case CharType:
            return (uint64_t)_value.charValue;
        case ShortType:
            return (uint64_t)_value.shortValue;
        case IntType:
            return (uint64_t)_value.intValue;
        case LongType:
            return (uint64_t)_value.longValue;
        case DoubleType:
            return (uint64_t)(_value.doubleValue + 0.5);
        case StringType:
            // return (uint64_t)strtoul(_value.stringValue, NULL, 0);
            return 0;
    }
    return 0;
}

int8_t LogicValue::charValue() const
{
    switch (_type)
    {
        case CharType:
            return _value.charValue;
        case BoolType:
        case UCharType:
        case UShortType:
        case UIntType:
        case ULongType:
            return (int8_t)ulongValue();
        case ShortType:
        case IntType:
        case LongType:
        case DoubleType:
        case StringType:
            return (int8_t)longValue();
    }
    return 0;
}

int16_t LogicValue::shortValue() const
{
    switch (_type)
    {
        case ShortType:
            return _value.shortValue;
        case BoolType:
        case UCharType:
        case UShortType:
        case UIntType:
        case ULongType:
            return (int16_t)ulongValue();
        case CharType:
        case IntType:
        case LongType:
        case DoubleType:
        case StringType:
            return (int16_t)longValue();
    }
    return 0;
}

int32_t LogicValue::intValue() const
{
    switch (_type)
    {
        case IntType:
            return _value.shortValue;
        case BoolType:
        case UCharType:
        case UShortType:
        case UIntType:
        case ULongType:
            return (int32_t)ulongValue();
        case CharType:
        case ShortType:
        case LongType:
        case DoubleType:
        case StringType:
            return (int32_t)longValue();
    }
    return 0;
}

int64_t LogicValue::longValue() const
{
    switch (_type)
    {
        case LongType:
            return _value.longValue;
        case BoolType:
            return _value.boolValue ? 1 : 0;
        case UCharType:
            return (int64_t)_value.ucharValue;
        case UShortType:
            return (int64_t)_value.ushortValue;
        case UIntType:
            return (int64_t)_value.uintValue;
        case ULongType:
            return (int64_t)_value.uintValue;
        case CharType:
            return (int64_t)_value.charValue;
        case ShortType:
            return (int64_t)_value.shortValue;
        case IntType:
            return (int64_t)_value.intValue;
        case DoubleType:
            return (int64_t)(_value.doubleValue + 0.5);
        case StringType:
            // return strtol(_value.stringValue, NULL, 0);
            return 0;
    }
    return 0;
}

double LogicValue::doubleValue() const
{
    switch (_type)
    {
        case DoubleType:
            return _value.doubleValue;
         case BoolType:
            return _value.boolValue ? 1 : 0;
        case UCharType:
            return _value.ucharValue;
        case UShortType:
            return _value.ushortValue;
        case UIntType:
            return _value.uintValue;
        case ULongType:
            return _value.uintValue;
        case CharType:
            return _value.charValue;
        case ShortType:
            return _value.shortValue;
        case IntType:
            return _value.intValue;
        case LongType:
            return _value.longValue;
        case StringType:
            // return strtod(_value.stringValue, NULL);
            return 0;
    }
    return 0;
}

const char* LogicValue::stringValue() const
{
    switch (_type)
    {
        case DoubleType:
        case BoolType:
        case UCharType:
        case UShortType:
        case UIntType:
        case ULongType:
        case CharType:
        case ShortType:
        case IntType:
        case LongType:
            return ""; // we would have to manage the memory for the string otherwise. Maybe later.
        case StringType:
            return _value.stringValue;
    }
    return 0;
}

LogicValue::LogicValue(float value)
{
    _value.doubleValue = value;
    _type = DoubleType;
}

LogicValue& LogicValue::operator=(const float value)
{
    _value.doubleValue = value;
    _type = DoubleType;
    return *this;
}

LogicValue::operator float() const
{
    return doubleValue();
}