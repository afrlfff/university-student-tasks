#pragma once

#include "StringL.h"

/**
 * BinaryUtils.
 * 
 * Brief:
 * - Class defines static methods to convert number to binary string and vice versa
 * 
 * Details:
 * - Class work with StringL class
 */
class BinaryUtils {
private:
    BinaryUtils() = default;
public:
    // returns  decimal number from binary string
    template <typename valueType>
    static inline valueType GetNumberFromBinaryString(const StringL<char>& binaryString);

    // returns binary string of specified length from decimal number
    template <typename valueType>
    static inline StringL<char> GetBinaryStringFromNumber(valueType number, size_t codeLength = sizeof(valueType) * 8);
};


template <typename valueType>
valueType BinaryUtils::GetNumberFromBinaryString(const StringL<char>& binaryString)
{
    valueType result = 0;
    for (size_t i = 0; i < binaryString.size(); ++i) {
        if (binaryString[i] == '1') {
            result |= (1 << (binaryString.size() - i - 1));
        }
    }
    return result;
}

template <typename valueType>
StringL<char> BinaryUtils::GetBinaryStringFromNumber(valueType number, size_t codeLength)
{
    StringL<char> result(codeLength, '0');
    for (valueType i = 0; i < codeLength; ++i) {
        if (number & (1 << (codeLength - i - 1))) {
            result.assign(i, '1');
        }
    }
    return result;
}