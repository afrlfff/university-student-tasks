#pragma once

#include <string>
#include <cstdint>
#include <map>
#include <queue>

#include "FileUtils.h"
#include "CodecUTF8.h"
#include "HuffmanTree.h"
#include "TextTools.h"
#include "BinaryUtils.h"

#include "CodecRLE.h"


class CodecHA : public CodecRLE
{
private:
    CodecHA() = default;
public:
    static void Encode(const char* inputPath, const char* outputPath);
    static void Decode(const char* inputPath, const char* outputPath);
protected:
    struct data_local {
        uint8_t alphabetLength;
        std::vector<std::tuple<char32_t, size_t, std::string>> huffmanCodesCanonicalTuple;
        std::string encodedStr;
        data_local(const uint8_t& _alphabetLength, const std::vector<std::tuple<char32_t, size_t, std::string>>& _huffmanCodesCanonicalTuple, const std::string& _encodedStr) : alphabetLength(_alphabetLength), huffmanCodesCanonicalTuple(_huffmanCodesCanonicalTuple), encodedStr(_encodedStr) {}
    };
    struct data {
        std::queue<data_local> queueLocalData;
        data(const std::queue<data_local>& _queueLocalData) : queueLocalData(_queueLocalData) {}
    };

    static data GetData(const std::u32string& inputStr);
    static std::u32string DecodeHA(FILE* inputFile);
};


// START IMPLEMENTATION

CodecHA::data CodecHA::GetData(const std::u32string& inputStr)
{
    std::queue<data_local> queueLocalData;

    const size_t strLengthToStart = 100; // bugger value = optimize time = compression worse
    const size_t strLengthToAppend = 50; // bugger value = optimize time = compression worse
    const int maxHuffmanCodeLength = 8; // I think the best value is 8
    const size_t maxSizeOfLocalString = 100000; // to limit RAM consumption
    int huffmanCodeLengthCounter;
    std::u32string localString; // local string to make huffman algorithm
    size_t stringPointer = 0; // point to the end of localString within an inputStr

    std::set<char32_t> alphabetSet;
    std::map<char32_t, size_t> charCountsMap;
    std::vector<std::pair<char32_t, double>> charFrequenciesVector;

    double frequency; // temporary variable


    // get all the data_local
    while (stringPointer < inputStr.size()) {
        // inicialization
        localString = inputStr.substr(stringPointer, strLengthToStart);
        stringPointer += strLengthToStart;

        alphabetSet = GetAlphabetSet(localString);
        charCountsMap = GetCharCountsMap(localString);
        charFrequenciesVector.clear();
        for (auto it = alphabetSet.begin(); it != alphabetSet.end(); ++it) {
            frequency = static_cast<double>(charCountsMap[*it]) / localString.size();
            // round frequency to make better compression
            frequency = std::trunc(frequency * maxSizeOfLocalString) / maxSizeOfLocalString;
            charFrequenciesVector.push_back(std::make_pair(*it, frequency));
        }
        // sort vector by frequencies
        std::sort(charFrequenciesVector.begin(), charFrequenciesVector.end(), [](const auto& a, const auto& b) {
            return a.second < b.second;
        });
        
        // find the best size of local string to make a Huffman tree
        while (true) {
            // get huffmanCodeLengthCounter
            HuffmanTree tree = BuildHuffmanTree(charFrequenciesVector, alphabetSet.size());
            huffmanCodeLengthCounter = tree.GetHeight();
            if (huffmanCodeLengthCounter > maxHuffmanCodeLength) {
                // reset data to the last modification where huffmanCodeLengthCounter <= maxHuffmanCodeLength

                localString.erase(localString.size() - strLengthToAppend); // remove last strLengthToAppend characters
                stringPointer -= strLengthToAppend;

                alphabetSet = GetAlphabetSet(localString);
                charCountsMap = GetCharCountsMap(localString);
                charFrequenciesVector.clear();
                for (auto it = alphabetSet.begin(); it != alphabetSet.end(); ++it) {
                    frequency = static_cast<double>(charCountsMap[*it]) / localString.size();
                    // round frequency to make better compression
                    frequency = std::trunc(frequency * maxSizeOfLocalString) / maxSizeOfLocalString;
                    charFrequenciesVector.push_back(std::make_pair(*it, frequency));
                }
                // sort vector by frequencies
                std::sort(charFrequenciesVector.begin(), charFrequenciesVector.end(), [](const auto& a, const auto& b) {
                    return a.second < b.second;
                });

                break;
            } else if (stringPointer >= inputStr.size()) {
                break;
            }

            // modify the data
            // expand localString
            if (localString.size() >= maxSizeOfLocalString) break;
            localString += inputStr.substr(stringPointer, strLengthToAppend);
            stringPointer += strLengthToAppend;

            // expand alphabetSet and charCountsMap
            for (size_t i = localString.size() - strLengthToAppend; i < localString.size(); ++i) {
                alphabetSet.insert(localString[i]);
                ++charCountsMap[localString[i]];
            }
            // get charFrequenciesVector
            charFrequenciesVector.clear();
            for (auto it = alphabetSet.begin(); it != alphabetSet.end(); ++it) {
                frequency = static_cast<double>(charCountsMap[*it]) / localString.size();
                // round frequency to make better compression
                frequency = std::trunc(frequency * maxSizeOfLocalString) / maxSizeOfLocalString;
                charFrequenciesVector.push_back(std::make_pair(*it, frequency));
            }
            // sort vector by frequencies
            std::sort(charFrequenciesVector.begin(), charFrequenciesVector.end(), [](const auto& a, const auto& b) {
                return a.second < b.second;
            });
        }

        
        HuffmanTree tree = BuildHuffmanTree(charFrequenciesVector, alphabetSet.size());
        std::vector<std::tuple<char32_t, size_t, std::string>> huffmanCodesCanonicalTuple = GetHuffmanCodesCanonicalTuple(tree, alphabetSet.size());

        // make huffman codes map
        std::map<char32_t, std::string> huffmanCodesMap;
        for (auto elem : huffmanCodesCanonicalTuple) {
            huffmanCodesMap[std::get<0>(elem)] = std::get<2>(elem);
        }

        std::string encodedStr;
        // encode string with huffman codes
        for (size_t i = 0; i < localString.size(); ++i) {
            encodedStr += huffmanCodesMap[localString[i]];
        }

        queueLocalData.push(data_local(alphabetSet.size(), huffmanCodesCanonicalTuple, encodedStr));
        
        // temporary
        std::cout << "local string size: " << localString.size() << std::endl;
    }
    
    return data(queueLocalData);
}

std::u32string CodecHA::DecodeHA(FILE* inputFile)
{
    uint64_t numberOfLocalData = FileUtils::ReadValueBinary<uint64_t>(inputFile);
    std::u32string decodedStr;

    for (uint64_t i = 0; i < numberOfLocalData; ++i) {
        uint8_t alphabetLength = FileUtils::ReadValueBinary<uint8_t>(inputFile);
        std::u32string alphabet = CodecUTF8::DecodeString32FromBinaryFile(inputFile, alphabetLength);

        std::string lengthsOfCodes = CodecRLE::decode_stringWithOnlyNumbers(inputFile);

        // get huffman codes map
        std::map<std::string, char32_t> huffmanCodesMap;
        std::string binRepresentation; // temporary variable
        std::string temp; // temporary variable
        bool isCodeUsed;
        for (size_t j = 0; j < lengthsOfCodes.size(); ++j) {
            int lengthOfCode = lengthsOfCodes[j] - '0';
            // find the code of length = lengthOfCode
            // which has the smallest number (considering the code as the binary representation)
            // which was not used before and which satisfy the huffman rule
            for (int k = 0; k < 1000000000; ++k) {
                // find the code of length = lengthOfCode
                // which was not used before
                binRepresentation = GetBinaryStringFromNumber(k, lengthOfCode);
                if (huffmanCodesMap.find(binRepresentation) == huffmanCodesMap.end()) {
                    // check if the code satisfies the huffman rule
                    // (if there are no any codes from which the current code begins)
                    isCodeUsed = false;
                    for (int l = lengthOfCode - 1; l > 0; --l) {
                        temp = binRepresentation.substr(0, l);
                        if (huffmanCodesMap.find(temp) != huffmanCodesMap.end()) {
                            isCodeUsed = true;
                            break;
                        }
                    }
                    if (!isCodeUsed) {
                        huffmanCodesMap[binRepresentation] = alphabet[j];
                        break;
                    }
                }
            }
        }       

        // read encoded string
        uint64_t encodedStrLength = FileUtils::ReadValueBinary<uint64_t>(inputFile);
        std::string encodedStr; encodedStr.reserve(encodedStrLength);
        for (uint64_t j = 0; j < encodedStrLength - encodedStrLength % 8; j += 8) {
            encodedStr += GetBinaryStringFromNumber<uint8_t>(FileUtils::ReadValueBinary<uint8_t>(inputFile), 8);
        }
        if (encodedStrLength % 8 != 0) {
            encodedStr += GetBinaryStringFromNumber<uint8_t>(FileUtils::ReadValueBinary<uint8_t>(inputFile), encodedStrLength % 8);
        }

        // decode encoded string
        std::u32string decodedStrLocal;
        std::string currentCode; currentCode.reserve(8);
        for (size_t j = 0; j < encodedStr.size(); ++j) {
            currentCode.push_back(encodedStr[j]);
            if (huffmanCodesMap.find(currentCode) != huffmanCodesMap.end()) {
                decodedStrLocal.push_back(huffmanCodesMap[currentCode]);
                currentCode.clear();
            }
        }

        decodedStr += decodedStrLocal;
    }

    return decodedStr;
}

void CodecHA::Encode(const char* inputPath, const char* outputPath)
{
    FILE* outputFile = FileUtils::OpenFileBinaryWrite(outputPath);

    data encodingData = GetData(FileUtils::ReadContentToU32String(inputPath));
    
    FileUtils::AppendValueBinary(outputFile, static_cast<uint64_t>(encodingData.queueLocalData.size()));
    while (!encodingData.queueLocalData.empty()) {
        data_local dataLocal = encodingData.queueLocalData.front();

        FileUtils::AppendValueBinary(outputFile, dataLocal.alphabetLength);

        std::vector<std::tuple<char32_t, size_t, std::string>> huffmanCodesCanonicalTuple = dataLocal.huffmanCodesCanonicalTuple;
        // write alphabet and lengths of huffman codes (effectively)
        std::string lengthsOfCodes; lengthsOfCodes.reserve(dataLocal.alphabetLength);
        for (size_t i = 0; i < huffmanCodesCanonicalTuple.size(); ++i) {
            // add alphabet's character
            CodecUTF8::EncodeChar32ToBinaryFile(outputFile, std::get<0>(huffmanCodesCanonicalTuple[i]));
            // add length of its huffman code
            lengthsOfCodes.push_back(std::get<1>(huffmanCodesCanonicalTuple[i]) + '0');
        }
        CodecRLE::encodeData_stringWithOnlyNumbers(outputFile, CodecRLE::getData_stringWithOnlyNumbers(lengthsOfCodes));
        
        // write encoded string effectively
        std::string encodedStr = dataLocal.encodedStr;
        FileUtils::AppendValueBinary(outputFile, static_cast<uint64_t>(encodedStr.size()));
        for (size_t i = 0; i < encodedStr.size() - encodedStr.size() % 8; i += 8) {
            FileUtils::AppendValueBinary(outputFile, GetNumberFromBinaryString<uint8_t>(encodedStr.substr(i, 8)));
        }
        if (encodedStr.size() % 8 != 0) {
            FileUtils::AppendValueBinary(outputFile, static_cast<uint8_t>(GetNumberFromBinaryString<uint8_t>(encodedStr.substr(encodedStr.size() - encodedStr.size() % 8))));
        }

        encodingData.queueLocalData.pop();
    }

    FileUtils::CloseFile(outputFile);
}

void CodecHA::Decode(const char* inputPath, const char* outputPath)
{
    FILE* inputFile = FileUtils::OpenFileBinaryRead(inputPath);
    std::ofstream outputFile = FileUtils::OpenFile<std::ofstream>(outputPath);

    std::string decodedStr = CodecUTF8::EncodeString32ToString(DecodeHA(inputFile));
    FileUtils::AppendStr(outputFile, decodedStr);

    FileUtils::CloseFile(outputFile);
    FileUtils::CloseFile(inputFile);
}

// END IMPLEMENTATION