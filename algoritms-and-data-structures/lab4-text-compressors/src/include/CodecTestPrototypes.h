// !!!!!!!!!!!!
// File just for testing compression algorithms
// not for using in a program
// !!!!!!!!!!!!

#pragma once
#include <string>
#include <vector>
#include <map>
#include <set> // makes ordered set
#include <algorithm> // sorting
#include <cstdint> // for int8_t, int16_t ...
#include <utility> // for std::pair

#include "SuffixArray.h"
#include "HuffmanTree.h"
#include "BinaryUtils.h"

// Text tools

std::string GetAlphabet(const std::string& str)
{
    const char* charStr = str.c_str();
    std::set<char32_t> charsSet(charStr, charStr + str.size());

    std::string alphabet; alphabet.reserve(charsSet.size() + 1);

    for (char32_t c : charsSet) { 
        alphabet.push_back(c);
    }

    std::sort(alphabet.begin(), alphabet.end());
    return alphabet;
}

std::map<char, double> GetCharFrequenciesMap(const std::string& alphabet, const size_t& size, const std::string& str)
{
    std::map<char, double> charFrequencies;

    size_t countAll = 0;
    for (char c : str) {
        ++charFrequencies[c];
        ++countAll;
    } for (size_t i = 0; i < size; i++) {
        charFrequencies[alphabet[i]] /= static_cast<double>(countAll);
    }

    return charFrequencies;
}

// Codecs

std::string EncodeRLE_toString(const std::string& inputStr)
{
    std::string encodedStr;

    int countIdent = 1; // current count of repeating identical characters
    int countUnique = 1; // current count of repeating unique characters
    std::string uniqueSeq(1, inputStr[0]); // last sequence of unique characters
    bool flag = false; // show if previous character was part of sequence
    char32_t prev = inputStr[0]; // previous character

    int maxPossibleNumber = 127; // maximum possible value of int8_t

    // start RLE
    for (size_t i = 1; i < inputStr.size(); ++i)
    {
        if (inputStr[i] == prev) 
        {
            // record last sequence of unique symbols if it exists
            if (countUnique > 1) {
                uniqueSeq.pop_back(); // because "prev" was read as unique
                --countUnique; // because "prev" was read as unique

                countUnique = (countUnique == 1) ? -1 : countUnique; // to avoid -1
                encodedStr += (std::to_string(-1 * countUnique) + uniqueSeq);

                countUnique = 1;
            }

            if (flag) { countIdent = 1; flag = false; } 
            else { ++countIdent; }
            
            countUnique = 0;
            uniqueSeq.clear();
            uniqueSeq.reserve(maxPossibleNumber);
        }
        else 
        {
            // record last sequence of identical symbols if it exists
            if (countIdent > 1) {
                if (countIdent >= maxPossibleNumber) {
                    for (int i = 0; i < (countIdent / maxPossibleNumber); i++) {
                        encodedStr += (std::to_string(maxPossibleNumber) + std::string(1, prev));
                    }
                }
                if (countIdent % maxPossibleNumber != 0) {
                    encodedStr += (std::to_string(countIdent % maxPossibleNumber) + std::string(1, prev));
                }
                flag = true;
                countIdent = 1;
            } else if (countIdent == 0) {
                countIdent = 1;
            }

            if (flag) {
                countUnique = 1;
                uniqueSeq.clear();
                uniqueSeq.push_back(inputStr[i]);
                flag = false;
            } else {
                if (countUnique == 0) {
                    countUnique = 1;
                    uniqueSeq.clear();
                    uniqueSeq.push_back(prev);
                }

                ++countUnique;
                uniqueSeq.push_back(inputStr[i]);
            }
            countIdent = 1;

            // limit length of sequence
            if (countUnique == maxPossibleNumber) {
                encodedStr += (std::to_string(-1 * countUnique) + uniqueSeq);
                flag = true;
                countUnique = 0;
                uniqueSeq.clear();
            }
        }
        prev = inputStr[i];
    }

    // record last sequence which was lost in the loop
    if (countIdent > 1) {
        if (countIdent >= maxPossibleNumber) {
            for (int i = 0; i < (countIdent / maxPossibleNumber); ++i) {
                encodedStr += (std::to_string(maxPossibleNumber) + std::string(1, prev));
            }
        }
        if (countIdent % maxPossibleNumber != 0) {
            encodedStr += (std::to_string(countIdent % maxPossibleNumber) + std::string(1, prev));
        }
    }
    if (countUnique > 0) { 
        countUnique = (countUnique == 1) ? -1 : countUnique; // to avoid -1
        encodedStr += (std::to_string(-1 * countUnique) + uniqueSeq);
    }

    return encodedStr;
}

std::string EncodeHA_toString(const std::u32string& inputStr)
{
    std::u32string alphabet = GetAlphabet(inputStr);
    std::map<char32_t, size_t> charCountsMap = GetCharCountsMap(inputStr);
    std::vector<std::pair<char32_t, double>> charFrequenciesVector;
    double frequency; // temporary variable
    for (auto c : alphabet) {
        frequency = static_cast<double>(charCountsMap[c]) / static_cast<double>(inputStr.size());
        // leave in frequencies only 2 digits after the decimal point
        frequency = std::round(frequency * 100) / 100;
        charFrequenciesVector.push_back(std::make_pair(c, frequency));
    }
    // sort by vector frequencies
    std::stable_sort(charFrequenciesVector.begin(), charFrequenciesVector.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });

    HuffmanTree tree = BuildHuffmanTree(charFrequenciesVector, alphabet.size());
    std::map<char32_t, std::string> huffmanCodesMap = GetHuffmanCodesMap(tree, alphabet.size());

    std::string encodedStr;
    // write alphabet size
    encodedStr += std::to_string(alphabet.size()) + "\n";
    // write alphabet
    for (auto c : alphabet) {
        encodedStr.push_back(static_cast<char>(c));
    }
    encodedStr.push_back('\n');
    // write huffman codes
    for (int i = 0; i < alphabet.size(); ++i) {
        encodedStr += huffmanCodesMap[alphabet[i]] + " ";
    } encodedStr += "\n";
    // write length of the string
    encodedStr += std::to_string(inputStr.size()) + "\n";
    // write encoded string
    for (size_t i = 0; i < inputStr.size(); ++i) {
        encodedStr += huffmanCodesMap[inputStr[i]];
    }
    return encodedStr;
}

std::string EncodeHA_canonical_toString(const std::u32string& inputStr)
{
    std::u32string alphabet = GetAlphabet(inputStr);
    std::map<char32_t, size_t> charCountsMap = GetCharCountsMap(inputStr);
    std::vector<std::pair<char32_t, double>> charFrequenciesVector;
    double frequency; // temporary variable
    for (auto c : alphabet) {
        frequency = static_cast<double>(charCountsMap[c]) / static_cast<double>(inputStr.size());
        // leave in frequencies only 2 digits after the decimal point
        frequency = std::round(frequency * 100) / 100;
        charFrequenciesVector.push_back(std::make_pair(c, frequency));
    }
    // sort by vector frequencies
    std::sort(charFrequenciesVector.begin(), charFrequenciesVector.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });

    HuffmanTree tree = BuildHuffmanTree(charFrequenciesVector, alphabet.size());
    std::vector<std::tuple<char32_t, size_t, std::string>> huffmanCodesCanonicalTuple = GetHuffmanCodesCanonicalTuple(tree, alphabet.size());
    std::map<char32_t, std::string> huffmanCodesMap;
    for (auto elem : huffmanCodesCanonicalTuple) {
        huffmanCodesMap[std::get<0>(elem)] = std::get<2>(elem);
    }

    /* // sort tuple by length and priority of huffman codes
    std::sort(huffmanCodesCanonicalTuple.begin(), huffmanCodesCanonicalTuple.end(), [](const auto& a, const auto& b) {
        if (std::get<1>(a) == std::get<1>(b)) {
            if (GetNumberFromBinaryString(std::get<2>(a)) < GetNumberFromBinaryString(std::get<2>(b))) {
                return 1;
            } else {
                return 0;
            }
        }
        else if (std::get<1>(a) < std::get<1>(b)) {
            return 1;
        }
        return 0;
    }); */

    std::string encodedStr;
    // write alphabet size
    encodedStr += std::to_string(alphabet.size()) + "\n";
    // write alphabet
    for (auto elem : huffmanCodesCanonicalTuple) {
        encodedStr += std::string(1, static_cast<char>(std::get<0>(elem))) + " ";
        encodedStr += std::to_string(std::get<1>(elem)) + " ";
        encodedStr += std::get<2>(elem) + "\n";
    }
    // write length of the string
    encodedStr += std::to_string(inputStr.size()) + "\n";
    // write encoded string
    for (size_t i = 0; i < inputStr.size(); ++i) {
        encodedStr += huffmanCodesMap[inputStr[i]];
    }
    return encodedStr;
}


std::string EncodeBWTNaive_toString(const std::string& inputStr)
{
    std::vector<std::string> permutations; permutations.reserve(inputStr.size());
    for (size_t i = 0; i < inputStr.size(); ++i) {
        permutations.push_back(inputStr.substr(i) + inputStr.substr(0, i));
    }
    std::sort(permutations.begin(), permutations.end());

    std::string encodedStr; encodedStr.reserve(inputStr.size());
    for (size_t i = 0; i < inputStr.size(); ++i) {
        encodedStr.push_back(permutations[i][inputStr.size() - 1]);
    }
    return encodedStr;
}

std::u32string EncodeBWT_toString(const std::u32string& inputStr)
{
    std::u32string encodedStr; encodedStr.reserve(inputStr.size());

    std::vector<unsigned int> suffixArray = buildSuffixArray(inputStr);
    for (size_t i = 0; i < suffixArray.size(); ++i) {
        size_t ind = (suffixArray[i] > 0) ? (suffixArray[i] - 1) : (inputStr.size() - 1);
        encodedStr.push_back(inputStr[ind]);
    }
    return encodedStr;
}

std::u32string DecodeBWT_toString(const std::u32string& inputStr, size_t index)
{
    std::vector<std::pair<char32_t, size_t>> P; P.reserve(inputStr.size());
    for (size_t i = 0; i < inputStr.size(); ++i) {
        P.push_back(std::make_pair(inputStr[i], i));
    }
    std::sort(P.begin(), P.end());

    std::u32string decodedStr; decodedStr.reserve(inputStr.size());
    for (size_t i = 0; i < inputStr.size(); ++i) {
        index = P[index].second;
        decodedStr.push_back(inputStr[index]);
    }

    return decodedStr;
} 





struct LZ77_match {
    int offset;
    int length;
    char c;
    LZ77_match(int offset, int length, char c) : offset(offset), length(length), c(c) {}
};

// return start and length of the maximum prefix ({0, 0} if not found)
std::pair<int, int> findMaximumPrefix(const std::string inputStr, int stringPointer, const int SEARCH_BUFFER_SIZE, const int lOOKAHEAD_BUFFER_SIZE) {
    std::pair<int, int> prefixData = { 0, 0 };
    std::string prefix; prefix.reserve(lOOKAHEAD_BUFFER_SIZE);

    for (int i = 0; i < lOOKAHEAD_BUFFER_SIZE; ++i) {
        prefix.push_back(inputStr[stringPointer + i]);
        size_t prefixStart;

        // find prefix in the window
        if (stringPointer - SEARCH_BUFFER_SIZE < 0) {
            prefixStart = inputStr.substr(0, stringPointer).find(prefix);
        } else {
            prefixStart = inputStr.substr(stringPointer - SEARCH_BUFFER_SIZE, stringPointer).find(prefix);
        }

        if (prefixStart == std::string::npos) {
            return prefixData;
        } else {
            prefixData = { static_cast<int>(stringPointer - prefixStart), static_cast<int>(prefix.size()) };
        }
    }

    return prefixData;
}

std::string EncodeLZ77_toString(const std::string& inputStr) {
    const int SEARCH_BUFFER_SIZE = 32000;
    const int lOOKAHEAD_BUFFER_SIZE = 256; // maximum length of prefix
    int stringPointer = 0; // point at the first character in the string after hte window 

    // get all the matches
    std::vector<LZ77_match> matches;
    while (stringPointer < inputStr.size())
    {
        std::pair<int, int> prefixData = findMaximumPrefix(inputStr, stringPointer, SEARCH_BUFFER_SIZE, lOOKAHEAD_BUFFER_SIZE);
        if (prefixData.first == 0 && prefixData.second == 0) {
            matches.push_back(LZ77_match(0, 0, inputStr[stringPointer]));
            ++stringPointer;
        } else {
            matches.push_back(LZ77_match(prefixData.first, prefixData.second, '\0'));
            stringPointer += prefixData.second;
        }
    }

    //return matches;
    // encode matches
    std::string encodedStr;
    encodedStr += std::to_string(inputStr.size()) + "\n";
    for (int i = 0; i < matches.size(); ++i) {
        encodedStr += std::to_string(matches[i].offset) + " ";
    }
    encodedStr += "\n";
    for (int i = 0; i < matches.size(); ++i) {
        encodedStr += std::to_string(matches[i].length) + " ";
    }
    encodedStr += "\n";
    for (int i = 0; i < matches.size(); ++i) {
        encodedStr.push_back(matches[i].c);
    }
    return encodedStr;
}

// END IMPLEMENTATION
