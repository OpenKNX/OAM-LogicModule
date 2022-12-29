#pragma once

#include <cstdint>
#include <ctime>

class LogicValue
{
  public:
    LogicValue(bool value);
    LogicValue(uint8_t value);
    LogicValue(uint16_t value);
    LogicValue(uint32_t value);
    // LogicValue(uint64_t value);
    LogicValue(int8_t value);
    LogicValue(int16_t value);
    LogicValue(int32_t value);
    // LogicValue(int64_t value);
    // LogicValue(double value);
    LogicValue(float value);
    LogicValue(const char* value);

    operator bool() const;
    operator uint8_t() const;
    operator uint16_t() const;
    operator uint32_t() const;
    // operator uint64_t() const;
    operator int8_t() const;
    operator int16_t() const;
    operator int32_t() const;
    // operator int64_t() const;
    // operator double() const;
    operator float() const;
    operator const char*() const;

    LogicValue& operator=(const bool value);
    LogicValue& operator=(const uint8_t value);
    LogicValue& operator=(const uint16_t value);
    LogicValue& operator=(const uint32_t value);
    // LogicValue& operator=(const uint64_t value);
    LogicValue& operator=(const int8_t value);
    LogicValue& operator=(const int16_t value);
    LogicValue& operator=(const int32_t value);
    // LogicValue& operator=(const int64_t value);
    // LogicValue& operator=(const double value);
    LogicValue& operator=(const float value);
    LogicValue& operator=(const char* value);

    LogicValue& operator++();
    // LogicValue& operator++(int);

    LogicValue& operator+=(const int32_t value);
    LogicValue& operator+=(const float value);

    bool operator>(LogicValue const& value);
    bool operator<(LogicValue const& value);
    bool operator>=(LogicValue const& value);
    bool operator<=(LogicValue const& value);

    LogicValue operator+(LogicValue const& value);
    LogicValue operator-(LogicValue const& value);
    LogicValue operator*(LogicValue const& value);
    LogicValue operator/(LogicValue const& value);

    LogicValue operator-(const int32_t value);
    LogicValue operator-(const float value);

  private:
    bool boolValue() const;
    uint8_t ucharValue() const;
    uint16_t ushortValue() const;
    uint32_t uintValue() const;
    // uint64_t ulongValue() const;
    int8_t charValue() const;
    int16_t shortValue() const;
    int32_t intValue() const;
    // int64_t longValue() const;
    // double doubleValue() const;
    float floatValue() const;
    const char* stringValue() const;

    void setInternal(const uint32_t value);
    void setInternal(const int32_t value);
    void setInternal(const float value);
    void setInternal(const char* value);

    union Value
    {
        bool boolValue;
        uint8_t ucharValue;
        uint16_t ushortValue;
        uint32_t uintValue;
        // uint64_t ulongValue;
        int8_t charValue;
        int16_t shortValue;
        int32_t intValue;
        // int64_t longValue;
        // double doubleValue;
        float floatValue;
        const char* stringValue;
    };
    enum ValueType
    {
        BoolType,
        UCharType,
        UShortType,
        UIntType,
        // ULongType,
        CharType,
        ShortType,
        IntType,
        // LongType,
        // DoubleType,
        FloatType,
        StringType,
    };

    ValueType _type;
    Value _value;
};