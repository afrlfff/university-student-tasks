#pragma once
#include <string>


// returns  decimal number from binary string
template <typename valueType>
valueType GetNumberFromBinaryString(const std::string& binaryString)
{
    valueType result = 0;
    for (size_t i = 0; i < binaryString.size(); ++i) {
        if (binaryString[i] == '1') {
            result |= (1 << (binaryString.length() - i - 1));
        }
    }
    return result;
}

// returns binary string of specified length from decimal number
template <typename valueType>
std::string GetBinaryStringFromNumber(valueType number, size_t codeLength = sizeof(valueType))
{
    std::string result(codeLength, '0');
    for (valueType i = 0; i < codeLength; ++i) {
        if (number & (1 << (codeLength - i - 1))) {
            result[i] = '1';
        }
    }
    return result;
} 


