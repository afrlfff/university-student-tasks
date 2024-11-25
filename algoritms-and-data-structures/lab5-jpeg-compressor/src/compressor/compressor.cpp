#include <iostream>
#include <cstdint>

#include "../../../lab_4_text_compressors/src/helpers/FileUtils.h"
#include "../../../lab_4_text_compressors/src/codecs/Codec_RLE_HA.h"
#include "../../../lab_4_text_compressors/src/helpers/StringL.h"

int main(int argc, const char* argv[])
{
    if (argc != 3) {
        std::cerr << "Error: wrong number of arguments. Usage: <program_name> <input_file> <output_file>" << std::endl;
        return 1;
    }
    //const char* inputPath = "../../intermediate/colored_to_compress.bin";
    //const char* outputPath = "../../intermediate/colored_compressed.bin";

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

    StringL<unsigned char> inputString(y_zigzags_number * 64 + 2 * cb_zigzags_number * 64);
    for (uint32_t i = 0; i < y_zigzags_number; ++i) {
        for (int j = 0; j < 64; ++j) {
            inputString.push_back(
                FileUtils::ReadValueBinary<int8_t>(inputFile) + 128
            );
        }
    }
    for (uint32_t i = 0; i < cb_zigzags_number; ++i) {
        for (int j = 0; j < 64; ++j) {
            inputString.push_back(
                FileUtils::ReadValueBinary<int8_t>(inputFile) + 128
            );
            inputString.push_back(
                FileUtils::ReadValueBinary<int8_t>(inputFile) + 128
            );
        }
    }

    std::cout << "Start encoding..." << std::endl;
    Codec_RLE_HA<unsigned char>::Encode(inputString, outputFile, false);
    std::cout << "Done encoding." << std::endl;

    FileUtils::CloseFile(outputFile);
    FileUtils::CloseFile(inputFile);

    return 0;
}