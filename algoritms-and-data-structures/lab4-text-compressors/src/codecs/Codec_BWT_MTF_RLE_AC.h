#pragma once

#include <cstdint>

#include "../helpers/FileUtils.h"
#include "../helpers/CodecUTF8.h"
#include "../helpers/StringL.h"
#include "../helpers/Array.h"

#include "CodecBWT.h"
#include "CodecMTF.h"
#include "CodecRLE.h"
#include "CodecAC.h"

/**
 * Codec_BWT_MTF_RLE_AC (encoder - decoder).
 * 
 * Brief:
 * - Class defines static methods to encode / decode any string given in StringL class using BWT, MTF, RLE and AC methods in order
 * 
 * Parameters:
 * - charType - The unsigned type of the characters in the string (unsigned char, char16_t/unsigned short , char32_t/unsigned int).
 * 
 * Memory usage:
 * ...
 * 
 */
template <typename charType>
class Codec_BWT_MTF_RLE_AC: CodecBWT<charType>, 
                            CodecMTF<charType>, 
                            CodecRLE<charType>, 
                            CodecAC<charType>
{
private:
    Codec_BWT_MTF_RLE_AC() = default;
public:
    static void Encode(const StringL<charType>& inputStr, std::ofstream& outputFile, const bool useUTF8);
    static StringL<charType> Decode(std::ifstream& inputFile, const bool useUTF8);
protected:
    struct data {
        uint32_t indexBWT;
        uint32_t alphabetLengthMTF;
        Array<charType> alphabetMTF;
        typename CodecAC<charType>::data dataAC;
        data() = default;
    };

    static data encodeToData(const StringL<charType>& inputStr);
    static void encodeData(std::ofstream& outputFile, const data& data, const bool useUTF8);
};


// START IMPLEMENTATION

template <typename charType>
typename Codec_BWT_MTF_RLE_AC<charType>::data Codec_BWT_MTF_RLE_AC<charType>::encodeToData(const StringL<charType>& inputStr)
{
    Codec_BWT_MTF_RLE_AC<charType>::data data;

    auto bwtData = CodecBWT<charType>::encodeToData(inputStr);
    data.indexBWT = bwtData.index;

        // TEMPORARY
    /*     std::cout << std::endl;
        std::cout << bwtData.index << std::endl;
        std::cout << bwtData.encodedStrLength << std::endl;
        for (const charType c : bwtData.encodedStr) {
            std::cout << c << " ";    
        }
        std::cout << std::endl; */
        // END TEMPORARY

    auto mtfData = CodecMTF<charType>::encodeToData(bwtData.encodedStr);
    data.alphabetLengthMTF = mtfData.alphabetLength;
    data.alphabetMTF = mtfData.alphabet;
    bwtData.encodedStr.free_memory();

        // TEMPORARY
    /*     std::cout << mtfData.alphabetLength << std::endl;
        for (const charType c : mtfData.alphabet) {
            std::cout << c << " ";    
        }
        std::cout << std::endl;
        std::cout << mtfData.inputStrLength << std::endl;
        for (const uint32_t c : mtfData.codes) {
            std::cout << c << " ";
        }
        std::cout << std::endl; */
        // END TEMPORARY
        // TEMPORARY
        /* auto temp1 = mtfData.toString();
        for (const charType c : temp1) {
            std::cout << static_cast<int>(c) << " ";
        }
        std::cout << std::endl; */
        // END TEMPORARY

    auto rleData = CodecRLE<charType>::encodeToData(mtfData.toString());
    mtfData.codes.free_memory();
    mtfData.alphabet.free_memory();

        // TEMPORARY
        /* std::cout << rleData.inputStrLength << std::endl;
        for (const int8_t value : rleData.encodedNumbers) {
            std::cout << static_cast<int>(value) << " ";
        }
        std::cout << std::endl;
        for (const charType c : rleData.encodedChars) {
            std::cout << static_cast<int>(c) << " ";
        }
        std::cout << std::endl;
        // END TEMPORARY
        // TEMPORARY
        auto temp2 = rleData.toString();
        for (const charType c : temp2) {
            std::cout << static_cast<int>(c) << " ";
        }
        std::cout << std::endl; */
        // END TEMPORARY

    auto acData = CodecAC<charType>::encodeToData(rleData.toString());
    data.dataAC = acData;

        // TEMPORARY
        /* std::cout << acData.inputStrLength << std::endl;
        std::cout << acData.alphabetLength << std::endl;
        for (const auto elem : acData.alphabet) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
        for (const auto elem : acData.frequencies) {
            std::cout << elem << " ";
        }
        std::cout << std::endl; */
        // END TEMPORARY

    return data;
}

template <typename charType>
void Codec_BWT_MTF_RLE_AC<charType>::encodeData(std::ofstream& outputFile, const data& data, const bool useUTF8)
{
    CodecAC<charType>::encodeData(outputFile, data.dataAC, useUTF8);
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
void Codec_BWT_MTF_RLE_AC<charType>::Encode(const StringL<charType>& inputStr, std::ofstream& outputFile, const bool useUTF8)
{
    // ==== GET DATA ====
    Codec_BWT_MTF_RLE_AC<charType>::data data;

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
    auto acData = CodecAC<charType>::encodeToData(rleStr);
    rleStr.free_memory();
    data.dataAC = acData;
    std::cout << "\tAC done." << std::endl;

    // ==== WRITE DATA ====
    CodecAC<charType>::encodeData(outputFile, data.dataAC, useUTF8);
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
StringL<charType> Codec_BWT_MTF_RLE_AC<charType>::Decode(std::ifstream& inputFile, const bool useUTF8)
{
    // decode AC
    StringL<charType> strAC = CodecAC<charType>::Decode(inputFile, useUTF8);
    std::cout << "\tAC done." << std::endl;

    // decode RLE
    auto dataRLE = CodecRLE<charType>::data::fromString(strAC);
    strAC.free_memory();
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