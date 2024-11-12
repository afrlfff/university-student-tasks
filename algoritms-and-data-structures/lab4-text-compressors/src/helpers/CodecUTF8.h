#pragma once

#include <cstdint>

/**
 * CodecUTF8.
 * 
 * Brief:
 * - Class contains methods to encode / decode any unsigned char types to file using utf-8 encoding
 * 
 * Important:
 * - Use only unsigned chars for class methods. Signed chars may cause incorrect work
 * - When use decode methods you should specify the type of the char which you specified when was using encode methods
 */
class CodecUTF8 {
public:
    // encodes any unsigned <charType> value to std::string
    template <typename charType>
    static inline void EncodeCharToString(std::string& str, const charType& code_point);

    // decodes std::string to encdoed unsigned <charType> value
    template <typename charType>
    static inline const charType DecodeCharFromString(const std::string& encoded);

    // encodes any unsigned <charType> value to file (using utf-8 encoding)
    template <typename charType>
    static inline void EncodeCharToBinaryFile(std::ofstream& file, const charType& c);

    // decodes any unsigned <charType> value from file (using utf-8 encoding)
    template <typename charType>
    static inline const charType DecodeCharFromBinaryFile(std::ifstream& file);
private:
    // encodes any unsigned <charType> value to std::string
    template <typename charType>
    static void encodeCharToString_(std::string& str, const charType& code_point);

    // decodes std::string to eny encoded unsigned <charType> value
    template <typename charType>
    static void decodeCharFromString_(const std::string& str, charType& code_point);

    // encodes any unsigned <charType> value to file (using utf-8 encoding)
    template <typename charType>
    static void encodeCharToBinaryFile_(std::ofstream& file, const charType& c);

    // decodes any unsigned <charType> value from file (using utf-8 encoding)
    template <typename charType>
    static void decodeCharFromBinaryFile_(std::ifstream& file, charType& c);
};

// START IMPLEMENTATION

// ==== PRIVATE

// encodes any unsigned <charType> value to std::string
template<typename charType>
void CodecUTF8::encodeCharToString_(std::string& str, const charType& code_point)
{
    if (code_point <= 0x007F) {
        str.push_back(static_cast<char>(code_point));
    } else if (code_point <= 0x07FF) {
        //Use uint8_t or unsigned char. Regular char may be signed
        //and the sign bit may cause defect during bit manipulation
        uint8_t b2 = 0b10000000 | (code_point & 0b111111);
        uint8_t b1 = 0b11000000 | (code_point >> 6);
         
        str.push_back(static_cast<char>(b1));
        str.push_back(static_cast<char>(b2));
    } else if (code_point <= 0xFFFF) {
        uint8_t b3 = 0b10000000 | (code_point & 0b111111);
        uint8_t b2 = 0b10000000 | ((code_point >> 6) & 0b111111);
        uint8_t b1 = 0b11100000 | (code_point >> 12);
         
        str.push_back(static_cast<char>(b1));
        str.push_back(static_cast<char>(b2));
        str.push_back(static_cast<char>(b3));
    } else if (code_point <= 0x10FFFF) {
        uint8_t b4 = 0b10000000 | (code_point & 0b111111);
        uint8_t b3 = 0b10000000 | ((code_point >> 6) & 0b111111);
        uint8_t b2 = 0b10000000 | ((code_point >> 12) & 0b111111);
        uint8_t b1 = 0b11110000 | (code_point >> 18);
         
        str.push_back(static_cast<char>(b1));
        str.push_back(static_cast<char>(b2));
        str.push_back(static_cast<char>(b3));
        str.push_back(static_cast<char>(b4));
    }
}

// decodes std::string to eny encoded unsigned <charType> value
template<typename charType>
void CodecUTF8::decodeCharFromString_(const std::string& str, charType& code_point)
{
    //For unsigned bytes use uint8_t and not char.
    //char may be signed in some platforms. Using
    //char can actually introduce defects.
    uint8_t bytes[] = {0, 0, 0, 0};
 
    // decode
    if (str.size() == 0) {
        throw std::runtime_error("Incorrect string given to UTF8 decoding method.");
    }
    bytes[0] = str[0];

    if ((bytes[0] & 0b10000000) == 0)
    {
        code_point = bytes[0];
    }
    else if ((bytes[0] & 0b11100000) == 0b11000000)
    {
        if (str.size() < 2) {
            throw std::runtime_error("Incorrect string given to UTF8 decoding method.");
        }
        bytes[1] = str[1];

        if ((bytes[1] & 0b11000000) != 0b10000000) {
            //Error. Not a follow-on byte.
            throw std::runtime_error("Can't decode byte in UTF-8");
        }

        //Clear the UTF-8 marker bits
        bytes[0] = bytes[0] & 0b00011111;
        bytes[1] = bytes[1] & 0b00111111;

        code_point = (bytes[0] << 6) | bytes[1];
    }
    else if ((bytes[0] & 0b11110000) == 0b11100000) 
    {
        if (str.size() < 3) {
            throw std::runtime_error("Incorrect string given to UTF8 decoding method.");
        }
        bytes[1] = str[1];
        bytes[2] = str[2];

        if ((bytes[1] & 0b11000000) != 0b10000000) {
            //Error. Not a follow-on byte.
            throw std::runtime_error("Can't decode byte in UTF-8");
        }

        if ((bytes[2] & 0b11000000) != 0b10000000) {
            //Error. Not a follow-on byte.
            throw std::runtime_error("Can't decode byte in UTF-8");
        }

        //Clear the UTF-8 marker bits
        bytes[0] = bytes[0] & 0b00001111;
        bytes[1] = bytes[1] & 0b00111111;
        bytes[2] = bytes[2] & 0b00111111;

        code_point = (bytes[0] << 12) | (bytes[1] << 6) | bytes[2];
    } 
    else if ((bytes[0] & 0b11111000) == 0b11110000) 
    {
        if (str.size() < 4) {
            throw std::runtime_error("Incorrect string given to UTF8 decoding method.");
        }
        bytes[1] = str[1];
        bytes[2] = str[2];
        bytes[3] = str[3];

        if ((bytes[1] & 0b11000000) != 0b10000000) {
            //Error. Not a follow-on byte.
            throw std::runtime_error("Can't decode byte in UTF-8");
        } if ((bytes[2] & 0b11000000) != 0b10000000) {
            //Error. Not a follow-on byte.
            throw std::runtime_error("Can't decode byte in UTF-8");
        } if ((bytes[3] & 0b11000000) != 0b10000000) {
            //Error. Not a follow-on byte.
            throw std::runtime_error("Can't decode byte in UTF-8");
        }

        //Clear the UTF-8 marker bits
        bytes[0] = bytes[0] & 0b00000111;
        bytes[1] = bytes[1] & 0b00111111;
        bytes[2] = bytes[2] & 0b00111111;
        bytes[3] = bytes[3] & 0b00111111;

        code_point = (bytes[0] << 18) | (bytes[1] << 12) | (bytes[2] << 6) | bytes[3];
    } 
    else 
    {
        throw std::runtime_error("Can't decode byte in UTF-8");
    }
}

// encodes any unsigned <charType> value to file (using utf-8 encoding)
template<typename charType>
void CodecUTF8::encodeCharToBinaryFile_(std::ofstream& file, const charType& code_point)
{
    if (code_point <= 0x007F) {
        uint8_t ch = static_cast<uint8_t>(code_point);

        file.write(reinterpret_cast<char*>(&ch), 1);
    } else if (code_point <= 0x07FF) {
        //Use uint8_t or unsigned char. Regular char may be signed
        //and the sign bit may cause defect during bit manipulation
        uint8_t b2 = 0b10000000 | (code_point & 0b111111);
        uint8_t b1 = 0b11000000 | (code_point >> 6);

        file.write(reinterpret_cast<char*>(&b1), 1);
        file.write(reinterpret_cast<char*>(&b2), 1);
    } else if (code_point <= 0xFFFF) {
        uint8_t b3 = 0b10000000 | (code_point & 0b111111);
        uint8_t b2 = 0b10000000 | ((code_point >> 6) & 0b111111);
        uint8_t b1 = 0b11100000 | (code_point >> 12);
         
        file.write(reinterpret_cast<char*>(&b1), 1);
        file.write(reinterpret_cast<char*>(&b2), 1);
        file.write(reinterpret_cast<char*>(&b3), 1);
    } else if (code_point <= 0x10FFFF) {
        uint8_t b4 = 0b10000000 | (code_point & 0b111111);
        uint8_t b3 = 0b10000000 | ((code_point >> 6) & 0b111111);
        uint8_t b2 = 0b10000000 | ((code_point >> 12) & 0b111111);
        uint8_t b1 = 0b11110000 | (code_point >> 18);
        
        file.write(reinterpret_cast<char*>(&b1), 1);
        file.write(reinterpret_cast<char*>(&b2), 1);
        file.write(reinterpret_cast<char*>(&b3), 1);
        file.write(reinterpret_cast<char*>(&b4), 1);
    }
}

// decodes any unsigned <charType> value from file (using utf-8 encoding)
template <typename charType>
void CodecUTF8::decodeCharFromBinaryFile_(std::ifstream& file, charType& c)
{
    //For unsigned bytes use uint8_t and not char.
    //char may be signed in some platforms. Using
    //char can actually introduce defects.
    uint8_t bytes[] = {0, 0, 0, 0};
 
    // decode
    file.read(reinterpret_cast<char*>(&bytes[0]), 1);

    if ((bytes[0] & 0b10000000) == 0)
    {
        c = bytes[0];
    }
    else if ((bytes[0] & 0b11100000) == 0b11000000)
    {
        file.read(reinterpret_cast<char*>(&bytes[1]), 1);

        if ((bytes[1] & 0b11000000) != 0b10000000) {
            //Error. Not a follow-on byte.
            throw std::runtime_error("Can't decode byte in UTF-8");
        }

        //Clear the UTF-8 marker bits
        bytes[0] = bytes[0] & 0b00011111;
        bytes[1] = bytes[1] & 0b00111111;

        c = (bytes[0] << 6) | bytes[1];
    }
    else if ((bytes[0] & 0b11110000) == 0b11100000)
    {
        file.read(reinterpret_cast<char*>(&bytes[1]), 1);
        file.read(reinterpret_cast<char*>(&bytes[2]), 1);

        if ((bytes[1] & 0b11000000) != 0b10000000) {
            //Error. Not a follow-on byte.
            throw std::runtime_error("Can't decode byte in UTF-8");
        }

        if ((bytes[2] & 0b11000000) != 0b10000000) {
            //Error. Not a follow-on byte.
            throw std::runtime_error("Can't decode byte in UTF-8");
        }

        //Clear the UTF-8 marker bits
        bytes[0] = bytes[0] & 0b00001111;
        bytes[1] = bytes[1] & 0b00111111;
        bytes[2] = bytes[2] & 0b00111111;

        c = (bytes[0] << 12) | (bytes[1] << 6) | bytes[2];
    }
    else if ((bytes[0] & 0b11111000) == 0b11110000)
    {
        file.read(reinterpret_cast<char*>(&bytes[1]), 1);
        file.read(reinterpret_cast<char*>(&bytes[2]), 1);
        file.read(reinterpret_cast<char*>(&bytes[3]), 1);

        if ((bytes[1] & 0b11000000) != 0b10000000) {
            //Error. Not a follow-on byte.
            throw std::runtime_error("Can't decode byte in UTF-8");
        } if ((bytes[2] & 0b11000000) != 0b10000000) {
            //Error. Not a follow-on byte.
            throw std::runtime_error("Can't decode byte in UTF-8");
        } if ((bytes[3] & 0b11000000) != 0b10000000) {
            //Error. Not a follow-on byte.
            throw std::runtime_error("Can't decode byte in UTF-8");
        }

        //Clear the UTF-8 marker bits
        bytes[0] = bytes[0] & 0b00000111;
        bytes[1] = bytes[1] & 0b00111111;
        bytes[2] = bytes[2] & 0b00111111;
        bytes[3] = bytes[3] & 0b00111111;

        c = (bytes[0] << 18) | (bytes[1] << 12) | (bytes[2] << 6) | bytes[3];
    }
    else
    {
        //We can't decode this byte
        throw std::runtime_error("Can't decode byte in UTF-8");
    }
}


// ==== PUBLIC

template <typename charType>
void CodecUTF8::EncodeCharToString(std::string& str, const charType& code_point)
{
    encodeCharToString_(str, code_point);
}

// decodes std::string to encdoed unsigned <charType> value
template <typename charType>
const charType CodecUTF8::DecodeCharFromString(const std::string& encoded)
{
    charType c;
    decodeCharFromString_(encoded, c);
    return c;
}

// encodes any unsigned <charType> value to file (using utf-8 encoding)
template<typename charType>
void CodecUTF8::EncodeCharToBinaryFile(std::ofstream& file, const charType& code_point)
{
    encodeCharToBinaryFile_(file, code_point);
}

// decodes any unsigned <charType> value from file (using utf-8 encoding)
template <typename charType>
const charType CodecUTF8::DecodeCharFromBinaryFile(std::ifstream& file)
{
    charType c;
    decodeCharFromBinaryFile_(file, c);
    return c;
}

// END IMPLEMENTATION