#pragma once

#include <cstdint>

#include "../helpers/FileUtils.h"
#include "../helpers/StringL.h"
#include "../helpers/Array.h"

#include "CodecBWT.h"
#include "CodecRLE.h"

/**
 * Codec_BWT_RLE (encoder - decoder).
 * 
 * Brief:
 * - Class defines static methods to encode / decode any string given in StringL class using BWT and RLE methods in order
 * 
 * Parameters:
 * - charType - The unsigned type of the characters in the string (unsigned char, char16_t/unsigned short , char32_t/unsigned int).
 * 
 * Memory usage:
 * ...
 * 
 */
template <typename charType>
class Codec_BWT_RLE: CodecBWT<charType>, 
                     CodecRLE<charType>
{
private:
    Codec_BWT_RLE() = default;
public:
    static void Encode(const StringL<charType>& inputStr, std::ofstream& outputFile, const bool useUTF8);
    static StringL<charType> Decode(std::ifstream& inputFile, const bool useUTF8);
protected:
    struct data {
        uint32_t indexBWT;
        typename CodecRLE<charType>::data dataRLE;
        data() = default;
    };
};


// START IMPLEMENTATION

template <typename charType>
void Codec_BWT_RLE<charType>::Encode(const StringL<charType>& inputStr, std::ofstream& outputFile, const bool useUTF8)
{
    // ==== GET DATA ====
    Codec_BWT_RLE<charType>::data data;

    auto bwtData = CodecBWT<charType>::encodeToData(inputStr);
    data.indexBWT = bwtData.index;
    std::cout << "\tBWT done." << std::endl;

    auto rleData = CodecRLE<charType>::encodeToData(bwtData.encodedStr);
    bwtData.encodedStr.free_memory();
    data.dataRLE = rleData;
    std::cout << "\tRLE done." << std::endl;

    // ==== WRITE DATA ====
    CodecRLE<charType>::encodeData(outputFile, data.dataRLE, useUTF8);
    FileUtils::AppendValueBinary(outputFile, data.indexBWT);
}

template <typename charType>
StringL<charType> Codec_BWT_RLE<charType>::Decode(std::ifstream& inputFile, const bool useUTF8)
{
    // decode RLE
    StringL<charType> strRLE = CodecRLE<charType>::Decode(inputFile, useUTF8);
    std::cout << "\tRLE done." << std::endl;

    // decode BWT
    uint32_t index = FileUtils::ReadValueBinary<uint32_t>(inputFile);
    StringL<charType> decodedStr = CodecBWT<charType>::decodeData(typename CodecBWT<charType>::data(index, strRLE.size(), strRLE));
    std::cout << "\tBWT done." << std::endl;

    return decodedStr;
}


// END IMPLEMENTATION