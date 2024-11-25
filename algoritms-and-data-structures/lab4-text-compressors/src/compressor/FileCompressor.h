#pragma once

#include "../codecs/CodecRLE.h"
#include "../codecs/CodecMTF.h"
#include "../codecs/CodecBWT.h"
#include "../codecs/CodecAC.h"
#include "../codecs/CodecHA.h"
#include "../codecs/CodecLZ77.h"
#include "../codecs/Codec_BWT_RLE.h"
#include "../codecs/Codec_BWT_MTF_RLE_AC.h"
#include "../codecs/Codec_BWT_MTF_AC.h"
#include "../codecs/Codec_BWT_MTF_HA.h"
#include "../codecs/Codec_BWT_MTF_RLE_HA.h"
#include "../codecs/Codec_RLE_HA.h"
#include "../codecs/Codec_LZ77_HA.h"

#include "../helpers/FileUtils.h"
#include "../helpers/CodecUTF8.h"
#include "../helpers/StringL.h"

typedef unsigned char char8;
typedef unsigned short char16;
typedef unsigned int char32;

/**
 * FileCompressor (compressor - decompressor).
 * 
 * Brief:
 * - Class defines static methods to compress / decompress any file
 * 
 * Details:
 * - From .txt files class reads content using utf-8, from other files class reads content by 1 byte and then saves it in string
 * - For .txt files class automatically determines the type of the string (char8, char16, char32) by maximum character in file
 * - Possible codec types: "RLE", "MTF", "BWT", "AC", "HA", "LZ77", "BWT+RLE", "BWT+MTF+RLE+AC", "BWT+MTF+AC", "BWT+MTF+HA", "BWT+MTF+RLE+HA", "RLE+HA", "LZ77+HA"
 */
class FileCompressor
{
public:
    static void Compress(const char* inputPath, const char* outputPath, const std::string& codecType);
    static void Decompress(const char* inputPath, const char* outputPath, const std::string& codecType);
private:
    FileCompressor() = default;

    template <typename charType>
    static void compress(const char* inputPath, std::ofstream& outputFile, const std::string& codecType, const bool useUTF8);
    template <typename charType>
    static void decompress(std::ifstream& inputFile, const char* outputPath, const std::string& codecType, const bool useUTF8);

    template <typename charType>
    static void writeStringLToFile(const char* outputPath, const StringL<charType>& str, const bool useUTF8);
    template <typename charType>
    static StringL<charType> readContentToStringL(const char* filepath, const bool useUTF8);
    static const std::string checkStringType(const char* filepath);
};

void FileCompressor::Compress(const char* inputPath, const char* outputPath, const std::string& codecType)
{
    bool useUTF8 = FileUtils::IsTextFile(inputPath) ? true : false;

    std::ofstream outputFile = FileUtils::OpenFileBinaryWrite(outputPath);
    FileUtils::AppendValueBinary(outputFile, useUTF8);

    if (useUTF8) {
        std::string stringType = checkStringType(inputPath);
        if (stringType == "string8") {
            FileUtils::AppendValueBinary(outputFile, static_cast<uint8_t>(8));
            compress<char8>(inputPath, outputFile, codecType, useUTF8);
        } else if (stringType == "string16") {
            FileUtils::AppendValueBinary(outputFile, static_cast<uint8_t>(16));
            compress<char16>(inputPath, outputFile, codecType, useUTF8);
        } else {
            FileUtils::AppendValueBinary(outputFile, static_cast<uint8_t>(32));
            compress<char32>(inputPath, outputFile, codecType, useUTF8);
        }
    } else {
        compress<char8>(inputPath, outputFile, codecType, useUTF8);
    }

    FileUtils::CloseFile(outputFile);
}

void FileCompressor::Decompress(const char* inputPath, const char* outputPath, const std::string& codecType)
{
    std::ifstream inputFile = FileUtils::OpenFileBinaryRead(inputPath);
    
    bool useUTF8 = FileUtils::ReadValueBinary<bool>(inputFile);

    if (useUTF8) {
        uint8_t stringType = FileUtils::ReadValueBinary<uint8_t>(inputFile);

        if (stringType == 8) {
            decompress<char8>(inputFile, outputPath, codecType, useUTF8);
        } else if (stringType == 16) {
            decompress<char16>(inputFile, outputPath, codecType, useUTF8);
        } else {
            decompress<char32>(inputFile, outputPath, codecType, useUTF8);
        }
    } else {
        decompress<char8>(inputFile, outputPath, codecType, useUTF8);
    }

    FileUtils::CloseFile(inputFile);
}

template <typename charType>
void FileCompressor::compress(const char* inputPath, std::ofstream& outputFile, const std::string& codecType, const bool useUTF8)
{
    StringL<charType> inputStr = readContentToStringL<charType>(inputPath, useUTF8);

    if (codecType == "RLE") {
        CodecRLE<charType>::Encode(inputStr, outputFile, useUTF8);
    } else if (codecType == "MTF") {
        CodecMTF<charType>::Encode(inputStr, outputFile, useUTF8);
    } else if (codecType == "BWT") {
        CodecBWT<charType>::Encode(inputStr, outputFile, useUTF8);
    } else if (codecType == "AC") {
        CodecAC<charType>::Encode(inputStr, outputFile, useUTF8);
    } else if (codecType == "HA") {
        CodecHA<charType>::Encode(inputStr, outputFile, useUTF8);
    } else if (codecType == "LZ77") {
        CodecLZ77<charType>::Encode(inputStr, outputFile, useUTF8);  
    } else if (codecType == "BWT+RLE") {
        Codec_BWT_RLE<charType>::Encode(inputStr, outputFile, useUTF8);
    } else if (codecType == "BWT+MTF+AC") {
        Codec_BWT_MTF_AC<charType>::Encode(inputStr, outputFile, useUTF8);
    } else if (codecType == "BWT+MTF+HA") {
        Codec_BWT_MTF_HA<charType>::Encode(inputStr, outputFile, useUTF8);
    } else if (codecType == "BWT+MTF+RLE+AC") {
        Codec_BWT_MTF_RLE_AC<charType>::Encode(inputStr, outputFile, useUTF8);
    } else if (codecType == "BWT+MTF+RLE+HA") {
        Codec_BWT_MTF_RLE_HA<charType>::Encode(inputStr, outputFile, useUTF8);
    } else if (codecType == "RLE+HA") {
        Codec_RLE_HA<charType>::Encode(inputStr, outputFile, useUTF8);
    } else if (codecType == "LZ77+HA") {
        Codec_LZ77_HA<charType>::Encode(inputStr, outputFile, useUTF8);
    } else {
        throw std::invalid_argument("Unknown codec type: " + codecType);
    }
}

template <typename charType>
void FileCompressor::decompress(std::ifstream& inputFile, const char* outputPath, const std::string& codecType, const bool useUTF8)
{
    StringL<charType> decodedStr;
    if (codecType == "RLE") {
        decodedStr = CodecRLE<charType>::Decode(inputFile, useUTF8);
    } else if (codecType == "MTF") {
        decodedStr = CodecMTF<charType>::Decode(inputFile, useUTF8);
    } else if (codecType == "BWT") {
        decodedStr = CodecBWT<charType>::Decode(inputFile, useUTF8);
    } else if (codecType == "AC") {
        decodedStr = CodecAC<charType>::Decode(inputFile, useUTF8);
    } else if (codecType == "HA") {
        decodedStr = CodecHA<charType>::Decode(inputFile, useUTF8);
    } else if (codecType == "LZ77") {
        decodedStr = CodecLZ77<charType>::Decode(inputFile, useUTF8);
    } else if (codecType == "BWT+RLE") {
        decodedStr = Codec_BWT_RLE<charType>::Decode(inputFile, useUTF8);
    } else if (codecType == "BWT+MTF+AC") {
        decodedStr = Codec_BWT_MTF_AC<charType>::Decode(inputFile, useUTF8);
    } else if (codecType == "BWT+MTF+HA") {
        decodedStr = Codec_BWT_MTF_HA<charType>::Decode(inputFile, useUTF8);
    } else if (codecType == "BWT+MTF+RLE+AC") {
        decodedStr = Codec_BWT_MTF_RLE_AC<charType>::Decode(inputFile, useUTF8);
    } else if (codecType == "BWT+MTF+RLE+HA") {
        decodedStr = Codec_BWT_MTF_RLE_HA<charType>::Decode(inputFile, useUTF8);
    } else if (codecType == "RLE+HA") {
        decodedStr = Codec_RLE_HA<charType>::Decode(inputFile, useUTF8);
    } else if (codecType == "LZ77+HA") {
        decodedStr = Codec_LZ77_HA<charType>::Decode(inputFile, useUTF8);
    } else {
        FileUtils::CloseFile(inputFile);
        throw std::runtime_error("Unknown codec type: " + codecType);
    }
    writeStringLToFile(outputPath, decodedStr, useUTF8);
}

const std::string FileCompressor::checkStringType(const char* filepath)
{
    std::ifstream f = FileUtils::OpenFileBinaryRead(filepath);

    char c;
    std::string result = "string8";
    std::string str;

    while (!FileUtils::EndOfBinaryFile(f))
    {
        str.clear();
        CodecUTF8::EncodeCharToString(str, CodecUTF8::DecodeCharFromBinaryFile<char32_t>(f));

        if (str.size() == 2) {
            result = "string16";
        }
        else if (str.size() == 4) {
            result = "string32";
            break;
        }
    }

    FileUtils::CloseFile(f);
    return result;
}

template <typename charType>
void FileCompressor::writeStringLToFile(const char* outputPath, const StringL<charType>& str, const bool useUTF8)
{
    std::ofstream outputFile = FileUtils::OpenFileBinaryWrite(outputPath);

    if (useUTF8) {
        for (const auto& c : str) {
            CodecUTF8::EncodeCharToBinaryFile(outputFile, c);
        }
    } else {
        for (const auto& c : str) {
            FileUtils::AppendValueBinary(outputFile, c);
        }
    }

    FileUtils::CloseFile(outputFile);
}

template <typename charType>
StringL<charType> FileCompressor::readContentToStringL(const char* filepath, const bool useUTF8)
{
    size_t size = FileUtils::FileSize(filepath);

    StringL<charType> str(size);

    std::ifstream f = FileUtils::OpenFileBinaryRead(filepath);

    charType c;
    if (useUTF8) {
        while (true) {
            c = CodecUTF8::DecodeCharFromBinaryFile<charType>(f);
            if (f.eof()) break;
            str.push_back(c);
        }
    } else {
        while (true) {
            c = FileUtils::ReadValueBinary<charType>(f);
            if (f.eof()) break;
            str.push_back(c);
        }
    }

    str.fit_to_size();
    FileUtils::CloseFile(f);

    return str;
}