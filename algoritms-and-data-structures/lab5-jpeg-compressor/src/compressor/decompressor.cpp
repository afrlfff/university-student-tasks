
#include <iostream>
#include <cstdint>

#include "../../../lab_4_text_compressors/src/helpers/FileUtils.h"
#include "../../../lab_4_text_compressors/src/codecs/Codec_RLE_HA.h"
#include "../../../lab_4_text_compressors/src/helpers/StringL.h"

int main(int argc, const char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Error: wrong number of arguments. Usage: <program_name> <input_file> <output_file>" << std::endl;
        return 1;
    }

    //const char* inputPath = "../../intermediate/colored_compressed.bin";
    //const char* outputPath = "../../intermediate/colored_decompressed.bin";
    const char* inputPath = argv[1];
    const char* outputPath = argv[2];

    std::ifstream inputFile = FileUtils::OpenFileBinaryRead(inputPath);
    std::ofstream outputFile = FileUtils::OpenFileBinaryWrite(outputPath);

    uint16_t width = FileUtils::ReadValueBinary<uint16_t>(inputFile);
    uint16_t height = FileUtils::ReadValueBinary<uint16_t>(inputFile);
    uint32_t y_zigzags_number = FileUtils::ReadValueBinary<uint32_t>(inputFile);
    uint32_t cb_zigzags_number = FileUtils::ReadValueBinary<uint32_t>(inputFile);

    FileUtils::AppendValueBinary(outputFile, width);
    FileUtils::AppendValueBinary(outputFile, height);
    FileUtils::AppendValueBinary(outputFile, y_zigzags_number);
    FileUtils::AppendValueBinary(outputFile, cb_zigzags_number);

    std::cout << "Start decoding..." << std::endl;
    StringL<unsigned char> decodedStr = Codec_RLE_HA<unsigned char>::Decode(inputFile, false);
    std::cout << "Done decoding." << std::endl;

    StringL<unsigned char> inputString(y_zigzags_number * 64 + 2 * cb_zigzags_number * 64);
    for (const auto& c : decodedStr) {
        FileUtils::AppendValueBinary(outputFile, static_cast<int8_t>(c - 128));
    }

    FileUtils::CloseFile(outputFile);
    FileUtils::CloseFile(inputFile);

    return 0;
}