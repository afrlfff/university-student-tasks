#pragma once

#include <cstdint>

#include "../helpers/FileUtils.h"
#include "../helpers/CodecUTF8.h"
#include "../helpers/StringL.h"
#include "../helpers/Array.h"

#include "CodecBWT.h"
#include "CodecMTF.h"
#include "CodecRLE.h"
#include "CodecHA.h"

/**
 * Codec_BWT_MTF_RLE_HA (encoder - decoder).
 * 
 * Brief:
 * - Class defines static methods to encode / decode any string given in StringL class using BWT, MTF, RLE and HA methods in order
 * 
 * Parameters:
 * - charType - The unsigned type of the characters in the string (unsigned char, char16_t/unsigned short , char32_t/unsigned int).
 * 
 * Memory usage:
 * ...
 * 
 */
template <typename charType>
class Codec_BWT_MTF_RLE_HA: CodecBWT<charType>, 
                            CodecMTF<charType>, 
                            CodecRLE<charType>, 
                            CodecHA<charType>
{
private:
    Codec_BWT_MTF_RLE_HA() = default;
public:
    static void Encode(const StringL<charType>& inputStr, std::ofstream& outputFile, const bool useUTF8);
    static StringL<charType> Decode(std::ifstream& inputFile, const bool useUTF8);
protected:
    struct data {
        uint32_t indexBWT;
        uint32_t alphabetLengthMTF;
        Array<charType> alphabetMTF;
        typename CodecHA<charType>::data dataHA;
        data() = default;
    };
};


// START IMPLEMENTATION

template <typename charType>
void Codec_BWT_MTF_RLE_HA<charType>::Encode(const StringL<charType>& inputStr, std::ofstream& outputFile, const bool useUTF8)
{
    // ==== GET DATA ====
    Codec_BWT_MTF_RLE_HA<charType>::data data;

    auto bwtData = CodecBWT<charType>::encodeToData(inputStr);
    data.indexBWT = bwtData.index;
    std::cout << "\tBWT done." << std::endl;

    auto mtfData = CodecMTF<charType>::encodeToData(bwtData.encodedStr);
    bwtData.encodedStr.free_memory();
    data.alphabetLengthMTF = mtfData.alphabetLength;
    data.alphabetMTF = mtfData.alphabet;
    std::cout << "\tMTF done." << std::endl;
    
    StringL<charType> mtfStr = mtfData.toString();
    mtfData.codes.free_memory();
    mtfData.alphabet.free_memory();
    auto rleData = CodecRLE<charType>::encodeToData(mtfStr);
    mtfStr.free_memory();
    std::cout << "\tRLE done." << std::endl;

    StringL<charType> rleStr = rleData.toString();
    rleData.encodedNumbers.free_memory();
    rleData.encodedChars.free_memory();
    auto haData = CodecHA<charType>::encodeToData(rleStr);
    rleStr.free_memory();
    data.dataHA = haData;
    std::cout << "\tHA done." << std::endl;

    // ==== WRITE DATA ====
    CodecHA<charType>::encodeData(outputFile, data.dataHA, useUTF8);
    FileUtils::AppendValueBinary(outputFile, data.alphabetLengthMTF);
    if (useUTF8) {
        for (const charType c : data.alphabetMTF)
            CodecUTF8::EncodeCharToBinaryFile(outputFile, c);
    } else {
        for (const charType c : data.alphabetMTF)
            FileUtils::AppendValueBinary(outputFile, c);
    }
    FileUtils::AppendValueBinary(outputFile, data.indexBWT);
}

template <typename charType>
StringL<charType> Codec_BWT_MTF_RLE_HA<charType>::Decode(std::ifstream& inputFile, const bool useUTF8)
{
    // decode HA
    StringL<charType> strHA = CodecHA<charType>::Decode(inputFile, useUTF8);
    std::cout << "\tHA done." << std::endl;

    // decode RLE
    auto dataRLE = CodecRLE<charType>::data::fromString(strHA);
    strHA.free_memory();
    StringL<charType> strRLE = CodecRLE<charType>::decodeData(dataRLE);
    dataRLE.encodedNumbers.free_memory();
    dataRLE.encodedChars.free_memory();
    std::cout << "\tRLE done." << std::endl;

    // decode MTF
    uint32_t alphabetLengthMTF = FileUtils::ReadValueBinary<uint32_t>(inputFile);
    Array<charType> alphabetMTF(alphabetLengthMTF);
    if (useUTF8) {
        while (alphabetMTF.size() < alphabetLengthMTF) {
            alphabetMTF.push_back(CodecUTF8::DecodeCharFromBinaryFile<charType>(inputFile));
        }
    } else {
        while (alphabetMTF.size() < alphabetLengthMTF) {
            alphabetMTF.push_back(FileUtils::ReadValueBinary<charType>(inputFile));
        }
    }
    uint32_t strLengthtMTF = strRLE.size();
    Array<uint32_t> codesMTF = CodecMTF<charType>::data::codesFromString(strRLE);
    strRLE.free_memory();
    StringL<charType> strMTF = CodecMTF<charType>::decodeData(typename CodecMTF<charType>::data(alphabetLengthMTF, alphabetMTF, strLengthtMTF, codesMTF));
    alphabetMTF.free_memory();
    codesMTF.free_memory();
    std::cout << "\tMTF done." << std::endl;

    // decode BWT
    uint32_t index = FileUtils::ReadValueBinary<uint32_t>(inputFile);
    StringL<charType> decodedStr = CodecBWT<charType>::decodeData(typename CodecBWT<charType>::data(index, strMTF.size(), strMTF));
    std::cout << "\tBWT done." << std::endl;

    return decodedStr;
}


// END IMPLEMENTATION