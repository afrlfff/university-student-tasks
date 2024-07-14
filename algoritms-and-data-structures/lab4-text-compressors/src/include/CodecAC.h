#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <queue>
#include <map>
#include <cmath> // for std::trunc

#include "FileUtils.h"
#include "CodecUTF8.h"
#include "TextTools.h"

class CodecAC
{
private:
    CodecAC() = default;
public:
    static void Encode(const char* inputPath, const char* outputPath);
    static void Decode(const char* inputPath, const char* outputPath);
protected:
    struct data_local {
        uint8_t alphabetLength;
        std::vector<std::pair<char32_t, double>> charFrequenciesVector; // alphabet and frequencies
        uint64_t resultValue;
        data_local(uint8_t _alphabetLength, std::vector<std::pair<char32_t, double>> _charFrequenciesVector, uint64_t _resultValue) : alphabetLength(_alphabetLength), charFrequenciesVector(_charFrequenciesVector), resultValue(_resultValue) {}
    };
    struct data {
        uint64_t strLength;
        std::queue<data_local> queueLocalData;
        data(const uint64_t& _strLength, const std::queue<data_local>& _queueLocalData) : strLength(_strLength), queueLocalData(_queueLocalData) {}
    };

    static data GetData(const std::u32string& inputStr);
    static std::string DecodeAC(FILE* inputFile);
};


// START IMPLEMENTATION

CodecAC::data CodecAC::GetData(const std::u32string& inputStr)
{
    std::queue<data_local> queueLocalData;
    const int minNumOfCharsToStart = 10; // as I have tested, if I will encode string with 13 characters,
                                         // result long double value will never get out of range
    std::u32string currentStr; // substring of inputStr that will be encoded
    size_t stringPointer = 0; // pointer to the next character within inputStr after "currentStr" substring 
    

    std::set<char32_t> alphabetSet;
    std::map<char32_t, size_t> charCountsMap;
    std::vector<std::pair<char32_t, double>> charFrequenciesVector, charFrequenciesVectorPrev;
    std::vector<double> segments;
    long double leftBound, rightBound, distance, resultValueDouble;
    uint64_t resultValue, resultValuePrev;
    size_t index; // tempary variable
    double frequency; // temporary variable
    double segment; // temporary variable
    size_t i; // temporary variable

    
    // encode all the inputStr by local parts (currentStr's)
    while (stringPointer < inputStr.size()) {
        // Inicialize currentStr and update stringPointer
        currentStr = inputStr.substr(stringPointer, minNumOfCharsToStart);
        stringPointer += currentStr.size();
        // initialize alphabet
        alphabetSet = GetAlphabetSet(currentStr);
        // inicialize charFrequenciesVector
        charCountsMap = GetCharCountsMap(currentStr);
        charFrequenciesVector.clear();
        for (auto it = alphabetSet.begin(); it != alphabetSet.end(); ++it) {
            frequency = static_cast<double>(charCountsMap[*it]) / static_cast<double>(currentStr.size());
            // leave in frequencies only 2 digits after the decimal point
            frequency = std::round(frequency * 100) / 100;
            charFrequenciesVector.push_back(std::make_pair(*it, frequency));
        }
        charFrequenciesVectorPrev = charFrequenciesVector;
        // sort charFrequenciesVector by frequencies
        std::sort(charFrequenciesVector.begin(), charFrequenciesVector.end(), [](const auto& a, const auto& b) {
            return a.second < b.second;
        });
        // Initialize segments (vector of bounds from 0 to 1)
        segments.clear(); segments.push_back(0.0);
        for (i = 1; i < alphabetSet.size(); ++i) {
            segment = charFrequenciesVector[i - 1].second + segments[i - 1];
            // leave only 2 digits after the decimal point
            segment = std::round(segment * 100) / 100;
            
            segments.push_back(segment);
        }
        segments.push_back(1.0);


        // find best resultValue on currentStr
        while (true) {
            leftBound = 0, rightBound = 1, distance;
            // get final left and right bounds
            for (char32_t c : currentStr) {
                // find index of c
                for (index = 0; index < alphabetSet.size(); ++index) {
                    if (charFrequenciesVector[index].first == c) {
                        break;
                    }
                }

                distance = rightBound - leftBound;
                rightBound = leftBound + segments[index + 1] * distance;
                leftBound = leftBound + segments[index] * distance;
            }
            resultValueDouble = rightBound / static_cast<long double>(2.0) + leftBound / static_cast<long double>(2.0);
            resultValueDouble = std::round(resultValueDouble * 1e17) / 1e17;
            // leave only significant fractional part of a number
            resultValuePrev = resultValue;
            resultValue = resultValueDouble * 1e17;


            if (resultValue % 10 == 0) {
                // will push one more character in currentStr and make one more iteration 

                // special case
                if (stringPointer >= inputStr.size()) {
                    queueLocalData.push(data_local(charFrequenciesVector.size(), charFrequenciesVector, resultValue));
                    break;  
                }

                // increase currentStr
                currentStr.push_back(inputStr[stringPointer]);
                // change alphabet
                alphabetSet.insert(inputStr[stringPointer]);
                ++charCountsMap[inputStr[stringPointer]];
                // change charFrequenciesVector
                charFrequenciesVectorPrev = charFrequenciesVector;
                charFrequenciesVector.clear();
                for (auto it = alphabetSet.begin(); it != alphabetSet.end(); ++it) { 
                    frequency = static_cast<double>(charCountsMap[*it]) / static_cast<double>(currentStr.size());
                    // leave in frequencies only 2 digits after the decimal point
                    frequency = std::round(frequency * 100) / 100;
                    charFrequenciesVector.push_back(std::make_pair(*it, frequency));
                }
                // sort vector by frequencies
                std::sort(charFrequenciesVector.begin(), charFrequenciesVector.end(), [](const auto& a, const auto& b) {
                    return a.second < b.second;
                });
                // inicialize segments (array of bounds from 0 to 1)
                segments.clear(); segments.push_back(0.0);
                for (i = 1; i < alphabetSet.size(); ++i) {
                    segment = charFrequenciesVector[i - 1].second + segments[i - 1];
                    // leave only 2 digits after the decimal point
                    segment = std::round((segment * 100)) / 100;
                    
                    segments.push_back(segment);
                }
                segments.push_back(1.0);

                ++stringPointer;
            } else {
                // means that result value may be out of range
                // so will save the data from last iteration
                queueLocalData.push(data_local(charFrequenciesVectorPrev.size(), charFrequenciesVectorPrev, resultValuePrev));
                break;
            }
        }

        stringPointer += currentStr.size();
    }

    return data(inputStr.size(), queueLocalData);
}

std::string CodecAC::DecodeAC(FILE* inputFile)
{
    std::string result;
    std::u32string result_local;

    uint8_t alphabetLength;
    std::u32string alphabet;
    std::vector<double> frequencies;
    std::vector<double> segments;
    long double resultValue, resultValueLocal;
    long double leftBound, rightBound, distance;
    size_t stringPointer = 0, index;
    double segment;

    uint64_t strLength = FileUtils::ReadValueBinary<uint64_t>(inputFile);

    while (stringPointer < strLength) {
        // read values
        alphabetLength = FileUtils::ReadValueBinary<uint8_t>(inputFile);
        alphabet.clear(); frequencies.clear();
        for (uint8_t i = 0; i < alphabetLength; ++i) {
            alphabet.push_back(CodecUTF8::DecodeChar32FromBinaryFile(inputFile));
            frequencies.push_back(static_cast<double>(FileUtils::ReadValueBinary<uint8_t>(inputFile)) / 100.0);
        }
        resultValue = static_cast<long double>(FileUtils::ReadValueBinary<uint64_t>(inputFile)) / static_cast<long double>(1e17);


        // inicialize segments
        segments.clear(); segments.push_back(0.0);
        for (size_t i = 1; i < alphabet.size(); ++i) {
            segment = frequencies[i - 1] + segments[i - 1];
            // leave only 2 digits after the decimal point
            segment = std::trunc((segment * 100.0)) / 100.0;
            
            segments.push_back(segment);
        }
        segments.push_back(1.0);

        // decode
        result_local.clear();
        leftBound = 0, rightBound = 1, distance;
        while (resultValueLocal != resultValue) {
            // find index of segment contains resultValue
            for (size_t j = 0; j < alphabet.size(); ++j) {
                if (resultValue >= (leftBound + segments[j] * (rightBound - leftBound)) && 
                    resultValue < (leftBound + segments[j + 1] * (rightBound - leftBound))) {
                    index = j;
                    break;
                }
            }
            result_local.push_back(alphabet[index]);
            ++stringPointer;

            distance = rightBound - leftBound;
            rightBound = leftBound + segments[index + 1] * distance;
            leftBound = leftBound + segments[index] * distance;
        }
        result += CodecUTF8::EncodeString32ToString(result_local);
    }

    return result;
}

void CodecAC::Encode(const char* inputPath, const char* outputPath)
{
    FILE* outputFile = FileUtils::OpenFileBinaryWrite(outputPath);

    data encodingData = GetData(FileUtils::ReadContentToU32String(inputPath));
    FileUtils::AppendValueBinary(outputFile, encodingData.strLength);

    while (!encodingData.queueLocalData.empty()) {
        auto elem = encodingData.queueLocalData.front();

        FileUtils::AppendValueBinary(outputFile, elem.alphabetLength);
        for (auto& pair : elem.charFrequenciesVector) {
            CodecUTF8::EncodeChar32ToBinaryFile(outputFile, pair.first);
            FileUtils::AppendValueBinary(outputFile, static_cast<uint8_t>(pair.second * 100.0));
        }
        FileUtils::AppendValueBinary(outputFile, elem.resultValue);

        encodingData.queueLocalData.pop();
    }
    FileUtils::CloseFile(outputFile);
}

void CodecAC::Decode(const char* inputPath, const char* outputPath)
{
    FILE* inputFile = FileUtils::OpenFileBinaryRead(inputPath);
    std::ofstream outputFile = FileUtils::OpenFile<std::ofstream>(outputPath);

    std::string decodedStr = DecodeAC(inputFile);
    FileUtils::AppendStr(outputFile, decodedStr);

    FileUtils::CloseFile(outputFile);
    FileUtils::CloseFile(inputFile);
}

// END IMPLEMENTATION