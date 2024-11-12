#pragma once

#include <cstdint>

#include "../helpers/FileUtils.h"
#include "../helpers/CodecUTF8.h"
#include "../helpers/StringL.h"
#include "../helpers/Array.h"


/**
 * CodecLZ77 (encoder - decoder).
 * 
 * Brief:
 * - Class defines static methods to encode / decode any string given in StringL class using LZ77 method
 * - It also defines methods to use a class with other codecs. Those methods defined in "protected"
 * 
 * Parameters:
 * - charType - The unsigned type of the characters in the string (unsigned char, char16_t/unsigned short , char32_t/unsigned int).
 * 
 * Memory usage:
 * ...
 * 
 */
template <typename charType>
class CodecLZ77
{
public:
    static void Encode(const StringL<charType>& text, std::ofstream& outputFile, const bool useUTF8);
    static StringL<charType> Decode(std::ifstream& inputFile, const bool useUTF8);
private:
    CodecLZ77() = default;
    static int find(const StringL<charType>& target, const StringL<charType>& original, const uint32_t startIndex, const uint32_t endIndex);

    const static uint32_t searchBufferSize = 16384;
    const static uint32_t lookaheadBufferSize = 128;
protected:
    struct data {
        uint32_t inputStrLength;
        Array<uint16_t> lengths;
        Array<uint16_t> offsets;
        StringL<charType> chars;

        data() = default;
        data(const uint32_t inputStrLength_, const Array<uint16_t>& lengths_, const Array<uint16_t>& offsets_, const StringL<charType> chars_) :
            inputStrLength(inputStrLength_), lengths(lengths_), offsets(offsets_), chars(chars_) {}

        const StringL<charType> toString() const;
        static const data fromString(const StringL<charType>& str, const uint32_t inputStrLength);
    };

    static data encodeToData(const StringL<charType>& inputStr);
    static void encodeData(std::ofstream& outputFile, const data& data, const bool useUTF8);
    static StringL<charType> decodeData(const data& data);
};


// START IMPLEMENTATION

// ==== PUBLIC ====

template <typename charType>
void CodecLZ77<charType>::Encode(const StringL<charType>& text, std::ofstream& outputFile, const bool useUTF8)
{
    FileUtils::AppendValueBinary(outputFile, static_cast<uint32_t>(text.size()));

    const uint32_t searchBufferSize = CodecLZ77<charType>::searchBufferSize;
    const uint32_t lookaheadBufferSize = CodecLZ77<charType>::lookaheadBufferSize;

    uint32_t i = 0; // pointer within a text

    // encoding
    while (i < text.size())
    {
        StringL<charType> maxStr(lookaheadBufferSize); // maximum string from lookahead buffer in search buffer
        bool flag = true; // if maxStr found in searchBuffer

        // define search buffer bounds
        uint32_t searchBufferStart = (i > searchBufferSize) ? (i - searchBufferSize) : (0u);
        uint32_t searchBufferEnd = i;
    
        // define offset and length
        uint32_t offset = 0;
        uint16_t length = 0;

        // find maximum string from lookahead buffer in search buffer
        while (flag && (i < text.size()) && (maxStr.size() < lookaheadBufferSize))
        {
            // increase maxStr
            maxStr.push_back(text[i]);

            int32_t index = find(maxStr, text, searchBufferStart, searchBufferEnd);
            if (index == -1ll)
            {
                break;
            }

            ++length;
            offset = searchBufferEnd - index;
            ++i;
        }

        // write length, offset and character if needed
        FileUtils::AppendValueBinary(outputFile, static_cast<uint16_t>(offset));
        FileUtils::AppendValueBinary(outputFile, length);
        if (length == 0) {
            if (useUTF8) {
                CodecUTF8::EncodeCharToBinaryFile(outputFile, text[i++]);
            } else {
                FileUtils::AppendValueBinary(outputFile, text[i++]);
            }
        }
    }
}

template <typename charType>
StringL<charType> CodecLZ77<charType>::Decode(std::ifstream& inputFile, const bool useUTF8)
{
    uint32_t inputStrLength = FileUtils::ReadValueBinary<uint32_t>(inputFile);

    StringL<charType> decoded(inputStrLength);
    uint32_t stringPointer = 0; // pointer within abstract input text (text before encoding)

    uint16_t offset, length;
    while (decoded.size() < inputStrLength)
    {
        offset = FileUtils::ReadValueBinary<uint16_t>(inputFile);
        length = FileUtils::ReadValueBinary<uint16_t>(inputFile);
        if (length == 0)
        {
            if (useUTF8) {
                decoded.push_back(CodecUTF8::DecodeCharFromBinaryFile<charType>(inputFile));
            } else {
                decoded.push_back(FileUtils::ReadValueBinary<charType>(inputFile));
            }
            ++stringPointer;
        }
        else
        {
            uint32_t start = stringPointer - offset;
            uint32_t end = stringPointer - offset + length;
            stringPointer += length;
            for (uint32_t j = start; j < end; ++j) {
                decoded.push_back(decoded[j]);
            }
        }
    }

    return decoded;
}

// ==== PRIVATE ====

/**
 * Searches for the target string within the original string, bounded by the specified start and end indexes.
 * Returns the index of the first occurrence of the target string, or -1 if not found.
 */
template <typename charType>
int CodecLZ77<charType>::find(const StringL<charType>& target, const StringL<charType>& original, const uint32_t startIndex, const uint32_t endIndex)
{
    uint32_t i = startIndex; // pointer within an original 
    uint32_t j = 0; // pointer within a target
    while (i < endIndex)
    {
        if (original[i] == target[j])
        {
            ++j;
            if (j == target.size()) return i - j + 1;
        }
        else
        {
            i -= j;
            j = 0u;
        }

        ++i;
    }

    return -1;
}

// ==== PROTECTED ====

template <typename charType>
typename CodecLZ77<charType>::data CodecLZ77<charType>::encodeToData(const StringL<charType>& text)
{
    const uint32_t searchBufferSize = CodecLZ77<charType>::searchBufferSize;
    const uint32_t lookaheadBufferSize = CodecLZ77<charType>::lookaheadBufferSize;

    Array<uint16_t> lengths(text.size());
    Array<uint16_t> offsets(text.size());
    StringL<charType> chars(text.size());

    uint32_t i = 0; // pointer within a text

    // encoding
    while (i < text.size())
    {
        StringL<charType> maxStr(lookaheadBufferSize); // maximum string from lookahead buffer in search buffer
        bool flag = true; // if maxStr found in searchBuffer

        // define search buffer bounds
        uint32_t searchBufferStart = (i > searchBufferSize) ? (i - searchBufferSize) : (0u);
        uint32_t searchBufferEnd = i;
    
        // define offset and length
        uint32_t offset = 0;
        uint16_t length = 0;

        // find maximum string from lookahead buffer in search buffer
        while (flag && (i < text.size()) && (maxStr.size() < lookaheadBufferSize))
        {
            // increase maxStr
            maxStr.push_back(text[i]);

            int index = find(maxStr, text, searchBufferStart, searchBufferEnd);
            if (index == -1)
            {
                break;
            }

            ++length;
            offset = searchBufferEnd - index;
            ++i;
        }

        // save offset, length and character if needed
        offsets.push_back(offset);
        lengths.push_back(length);
        if (length == 0) { chars.push_back(text[i++]); }
    }
    
    return data(text.size(), lengths, offsets, chars);
}

template <typename charType>
void CodecLZ77<charType>::encodeData(std::ofstream& outputFile, const data& data, const bool useUTF8)
{
    FileUtils::AppendValueBinary(outputFile, data.inputStrLength);

    size_t charsPointer = 0;

    for (uint32_t i = 0; i < data.lengths.size(); ++i)
    {
        FileUtils::AppendValueBinary(outputFile, data.offsets[i]);
        FileUtils::AppendValueBinary(outputFile, data.lengths[i]);
        if (data.lengths[i] == 0) {
            if (useUTF8) {
                CodecUTF8::EncodeCharToBinaryFile(outputFile, data.chars[charsPointer++]);
            } else {
                FileUtils::AppendValueBinary(outputFile, data.chars[charsPointer++]);
            }
        }
    }
}

template <typename charType>
StringL<charType> CodecLZ77<charType>::decodeData(const data& data)
{
    StringL<charType> decoded(data.inputStrLength);
    size_t charsPointer = 0;
    uint32_t stringPointer = 0; // pointer within abstract input text (text before encoding)

    uint32_t i = 0;
    while (decoded.size() < data.inputStrLength)
    {
        if (data.lengths[i] == 0)
        {
            decoded.push_back(data.chars[charsPointer++]);
            ++stringPointer;
        }
        else
        {
            uint32_t start = stringPointer - data.offsets[i];
            uint32_t end = stringPointer - data.offsets[i] + data.lengths[i];
            stringPointer += data.lengths[i];
            for (uint32_t j = start; j < end; ++j) {
                decoded.push_back(decoded[j]);
            }
        }
        ++i;
    }

    return decoded;
}


template <typename charType>
const StringL<charType> CodecLZ77<charType>::data::toString() const
{
    StringL<charType> result(offsets.size() + lengths.size() + chars.size());
    size_t charsPointer = 0;

    if (std::is_same<charType, unsigned char>::value)
    {
        for (size_t i = 0; i < lengths.size(); ++i)
        {
            result.push_back(static_cast<unsigned char>(offsets[i] >> 8));
            result.push_back(static_cast<unsigned char>(offsets[i] & 0b11111111));
            result.push_back(static_cast<unsigned char>(lengths[i] >> 8));
            result.push_back(static_cast<unsigned char>(lengths[i] & 0b11111111));
            if (lengths[i] == 0) { result.push_back(chars[charsPointer++]); }
        }
    }
    else if ((std::is_same<charType, char16_t>::value) || (std::is_same<charType, unsigned short>::value))
    {
        for (size_t i = 0; i < lengths.size(); ++i)
        {
            result.push_back(static_cast<char16_t>(offsets[i]));
            result.push_back(static_cast<char16_t>(lengths[i]));
            if (lengths[i] == 0) { result.push_back(chars[charsPointer++]); }
        }
    }
    else if ((std::is_same<charType, char32_t>::value) || (std::is_same<charType, unsigned int>::value))
    {
        for (size_t i = 0; i < lengths.size(); ++i)
        {
            result.push_back(static_cast<char32_t>(offsets[i]));
            result.push_back(static_cast<char32_t>(lengths[i]));
            if (lengths[i] == 0) { result.push_back(chars[charsPointer++]); }
        }
    }
    else
    {
        throw std::runtime_error("Error: Unsupported char type in CodecLZ77::data::toString()");
    }

    return result;
}

template <typename charType>
const typename CodecLZ77<charType>::data CodecLZ77<charType>::data::fromString(const StringL<charType>& str, const uint32_t inputStrLength)
{
    data result;
    result.inputStrLength = inputStrLength;
    result.offsets.resize(inputStrLength);
    result.lengths.resize(inputStrLength);
    result.chars.resize(inputStrLength);

    uint16_t offset, length;
    size_t i = 0; // pointer within a string
    if (std::is_same<charType, unsigned char>::value)
    {
        while (i < str.size())
        {
            offset = (str[i] << 8) + str[i + 1];
            length = (str[i + 2] << 8) + str[i + 3];
            result.offsets.push_back(offset);
            result.lengths.push_back(length);
            i += 4;
            if (length == 0) { result.chars.push_back(str[i++]); }
        }
    }
    else if (std::is_same<charType, char16_t>::value)
    {
        while (i < str.size())
        {
            length = static_cast<uint16_t>(str[i++]);
            offset = static_cast<uint16_t>(str[i++]);
            result.offsets.push_back(length);
            result.lengths.push_back(offset);
            if (length == 0) { result.chars.push_back(str[i++]); }
        }
    }
    else
    {
        while (i < str.size())
        {
            length = static_cast<uint16_t>(str[i++]);
            offset = static_cast<uint16_t>(str[i++]);
            result.offsets.push_back(length);
            result.lengths.push_back(offset);
            if (length == 0) { result.chars.push_back(str[i++]); }
        }
    }

    return result;
}

// END IMPLEMENTATION