#pragma once

#include <string>
#include <set>
#include <map>
#include <algorithm>
#include <cmath>
#include <vector>

#include "FileUtils.h"
#include "CodecUTF8.h"
#include "StringL.h"
#include "Array.h"

/**
 * TextUtils.
 * 
 * Brief:
 * - Class defines static methods which are useful for text processing
*/
class TextUtils {
public:
    // returns text entropy
    static double GetTextEntropy(const char* filepath);

    // returns ordered alphabet from the string
    template <typename charType>
    static Array<charType> GetAlphabet(const StringL<charType>& str);

    // returns map of characters and their frequencies
    template <typename charType>
    static std::map<charType, size_t> GetCharCountsMap(const StringL<charType>& str);

    // returns array of frequencies in order of alphabet
    template <typename charType>
    static Array<double> GetFrequencies(const StringL<charType>& str, const Array<charType>& alphabet);

    // returns array of integer frequencies in order of alphabet
    template <typename charType>
    static Array<uint32_t> GetFrequenciesInt(const StringL<charType>& str, const Array<charType>& alphabet);
};

// START IMPLEMENTATION

double TextUtils::GetTextEntropy(const char* filepath)
{
    std::map<char32_t, uint32_t> charCounts;
    uint32_t strLength = 0;

    if (FileUtils::IsTextFile(filepath)) {
        std::ifstream file = FileUtils::OpenFileBinaryRead(filepath);
        char32_t c;
        // Calculate count of each character
        while (true) {
            c = CodecUTF8::DecodeCharFromBinaryFile<char32_t>(file);
            if (file.eof()) break;
            ++strLength;
            ++charCounts[c];
        }
        FileUtils::CloseFile(file);
    } else {
        std::ifstream file = FileUtils::OpenFileBinaryRead(filepath);
        unsigned char c;
        // Calculate count of each character
        while (true) {
            c = FileUtils::ReadValueBinary<unsigned char>(file);
            if (file.eof()) break;
            ++strLength;
            ++charCounts[c];
        }
        FileUtils::CloseFile(file);
    }

    // Calculate probabilities and entropy
    double entropy = 0.0;
    for (const auto& pair : charCounts) {
        double probability = static_cast<double>(pair.second) / strLength;
        entropy -= probability * std::log2(probability);
    }

    return entropy;
}

template <typename charType>
Array<charType> TextUtils::GetAlphabet(const StringL<charType>& str)
{
    std::set<charType> charsSet(str.begin(), str.end());
    Array<charType> alphabet(charsSet.size());

    for (charType c : charsSet) { 
        alphabet.push_back(c);
    }

    std::sort(alphabet.begin(), alphabet.end());
    return alphabet;
}

template <typename charType>
Array<double> TextUtils::GetFrequencies(const StringL<charType>& str, const Array<charType>& alphabet)
{
    std::map<charType, size_t> charCounts = GetCharCountsMap<charType>(str);
    Array<double> frequencies(alphabet.size());
    for (size_t i = 0; i < alphabet.size(); ++i) {
        frequencies.push_back(static_cast<double>(charCounts[alphabet[i]]) / static_cast<double>(str.size()));
    }
    return frequencies;
}

template <typename charType>
Array<uint32_t> TextUtils::GetFrequenciesInt(const StringL<charType>& str, const Array<charType>& alphabet)
{
    std::map<charType, size_t> charCounts = GetCharCountsMap<charType>(str);
    Array<uint32_t> frequencies(alphabet.size());
    for (size_t i = 0; i < alphabet.size(); ++i) {
        frequencies.push_back(charCounts[alphabet[i]]);
    }
    return frequencies;
}

template <typename charType>
std::map<charType, size_t> TextUtils::GetCharCountsMap(const StringL<charType>& str)
{
    std::map<charType, size_t> charCounts;

    size_t countAll = 0;
    for (const charType& c : str) {
        ++charCounts[c];
        ++countAll;
    }

    return charCounts;
}

// END IMPLEMENTATION