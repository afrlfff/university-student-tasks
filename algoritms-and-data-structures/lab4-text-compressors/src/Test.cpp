// main file

#include <iostream>
#include <string>
#include <vector>
#include <filesystem> // C++ 17 and more
#include <cstdlib>
#include <chrono>

#include "helpers/TextUtils.h"
#include "compressor/FileCompressor.h"
#include"compressor/CompressorSettings.h"

namespace fs = std::filesystem;
const fs::path INPUT_DIR = fs::current_path() / "..\\input";
const fs::path OUTPUT_DIR = fs::current_path() / "..\\output";


// HELPER FUNCTIONS
std::string GetFileExtension(const std::string& path);
std::string GetFileName(const std::string& path);
void ClearOutputDirectory();
void MakeResultsFile(const std::vector<std::string>& paths, const std::vector<std::vector<long>>& times);


int main()
{
    ClearOutputDirectory();

    std::vector<std::vector<long>> TIMES;
    const std::vector<std::string> PATHS = {
        //"..\\input\\raw\\blackwhite.raw",
        //"..\\input\\raw\\blackwhite_2.raw",
        //"..\\input\\raw\\blackwhite_3.raw",
        //"..\\input\\raw\\blackwhite_4.raw",
        //"..\\input\\raw\\gray.raw",
        //"..\\input\\raw\\color.raw",
        //"..\\input\\txt\\russian_1mb.txt",
        //"..\\input\\txt\\enwik7.txt"
        //"..\\input\\txt\\small2.txt"
    };

    std::string compressionMethod = "LZ77";

    //CompressorSettings::SetLZ77SearchBufferSize(16384);

    std::cout << "Start..." << std::endl;

    // TEST CODE
    for (auto path : PATHS)
    {
        std::string pathToEncoded = "..\\output\\encoded\\" + GetFileName(path) + ".bin";
        std::string pathToDecoded = "..\\output\\decoded\\" + GetFileName(path) + "." + GetFileExtension(path);

        auto start = std::chrono::steady_clock::now();

        std::cout << "Encoding..." << std::endl;
        FileCompressor::Compress(path.c_str(), pathToEncoded.c_str(), compressionMethod);
        std::cout << "Done." << std::endl;

        auto end = std::chrono::steady_clock::now();
        long encodingTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        start = std::chrono::steady_clock::now();

        std::cout << "Decoding..." << std::endl;
        FileCompressor::Decompress(pathToEncoded.c_str(), pathToDecoded.c_str(), compressionMethod);
        std::cout << "Done." << std::endl;

        end = std::chrono::steady_clock::now();
        long decodingTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        TIMES.push_back({encodingTime, decodingTime});
    }

    MakeResultsFile(PATHS, TIMES);

    return 0;
}


// START IMPLEMETATION

std::string GetFileExtension(const std::string& path)
{
    size_t pointIndex = 0;
    for (size_t i = 0; i < path.size(); ++i) {
        if (path[i] == '.') pointIndex = i;
    }

    return path.substr(pointIndex + 1, path.size()); 
}

std::string GetFileName(const std::string& path)
{
    size_t lastSlashIndex = 0, pointIndex = 0;
    for (size_t i = 0; i < path.size(); ++i) {
        if (path[i] == '\\') lastSlashIndex = i;
        else if (path[i] == '.') pointIndex = i;
    }

    return path.substr(lastSlashIndex + 1, pointIndex - lastSlashIndex - 1); 
}

void ClearOutputDirectory()
{
    fs::remove_all(OUTPUT_DIR / "encoded");
    fs::remove_all(OUTPUT_DIR / "decoded");

    fs::create_directory(OUTPUT_DIR / "encoded");
    fs::create_directory(OUTPUT_DIR / "decoded");
}

void MakeResultsFile(const std::vector<std::string>& paths, const std::vector<std::vector<long>>& times)
{
    std::ofstream file("..\\output\\results.txt");
    file << "fileName | entropy | startSize[bytes] | encodedSize[bytes] | EncodingTime[ms] | DecodingTime[ms]" << std::endl;

    for (size_t i = 0; i < paths.size(); ++i) {
        std::string pathToOriginal = paths[i];
        std::string pathToEncoded = "..\\output\\encoded\\" + GetFileName(pathToOriginal) + ".bin";

        double textEntropy = TextUtils::GetTextEntropy(pathToOriginal.c_str());

        file << '\n';
        file << GetFileName(pathToOriginal) + ' ' + 
                std::to_string(textEntropy) + ' ' + 
                std::to_string(fs::file_size(pathToOriginal)) + ' ' + 
                std::to_string(fs::file_size(pathToEncoded)) + ' ' + 
                std::to_string(times[i][0]) + ' ' + 
                std::to_string(times[i][1]);
    }

    file.close();
}

// END IMPLEMENTATION