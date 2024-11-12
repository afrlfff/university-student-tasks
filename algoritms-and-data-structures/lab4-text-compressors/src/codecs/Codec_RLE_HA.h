#pragma once

#include <cstdint>

#include "../helpers/FileUtils.h"
#include "../helpers/StringL.h"
#include "../helpers/Array.h"

#include "CodecRLE.h"
#include "CodecHA.h"

/**
 * Codec_RLE_HA (encoder - decoder).
 * 
 * Brief:
 * - Class defines static methods to encode / decode any string given in StringL class using RLE and HA methods in order
 * 
 * Parameters:
 * - charType - The unsigned type of the characters in the string (unsigned char, char16_t/unsigned short , char32_t/unsigned int).
 * 
 * Memory usage:
 * if use protected methods:        θ((sizeof(inputStr)) + θ(inputStr.size()) + O(1)
 * if use only public methods:      O(1)
 * 
 */
template <typename charType>
class Codec_RLE_HA: CodecRLE<charType>, 
                    CodecHA<charType>
{
private:
    Codec_RLE_HA() = default;
public:
    static void Encode(const StringL<charType>& inputStr, std::ofstream& outputFile, const bool useUTF8);
    static StringL<charType> Decode(std::ifstream& inputFile, const bool useUTF8);
protected:
    struct data {
        typename CodecHA<charType>::data dataHA;
        data() = default;
    };
};


// START IMPLEMENTATION

template <typename charType>
void Codec_RLE_HA<charType>::Encode(const StringL<charType>& inputStr, std::ofstream& outputFile, const bool useUTF8)
{
    // ==== GET DATA ====
    Codec_RLE_HA<charType>::data data;

    auto rleData = CodecRLE<charType>::encodeToData(inputStr);
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
}

template <typename charType>
StringL<charType> Codec_RLE_HA<charType>::Decode(std::ifstream& inputFile, const bool useUTF8)
{
    // decode HA
    StringL<charType> strHA = CodecHA<charType>::Decode(inputFile, useUTF8);
    std::cout << "\tHA done." << std::endl;

    // decode RLE
    auto dataRLE = CodecRLE<charType>::data::fromString(strHA);
    strHA.free_memory();
    StringL<charType> decodedStr = CodecRLE<charType>::decodeData(dataRLE);
    dataRLE.encodedNumbers.free_memory();
    dataRLE.encodedChars.free_memory();
    std::cout << "\tRLE done." << std::endl;

    return decodedStr;
}


// END IMPLEMENTATION