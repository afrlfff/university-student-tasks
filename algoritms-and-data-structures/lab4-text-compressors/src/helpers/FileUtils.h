#pragma once

#include <fstream>
#include <sstream>
#include <codecvt>
#include <locale>


/**
 * FileUtils.
 * 
 * Brief:
 * - Class defines static methods to work with files
*/
class FileUtils
{
private:
    FileUtils() = default;
public:
    static std::ifstream OpenFileRead(const char* filepath);
    static std::ifstream OpenFileRead(const wchar_t* filepath);
    static std::ofstream OpenFileWrite(const char* filepath);
    static std::ofstream OpenFileWrite(const wchar_t* filepath);
    static std::ifstream OpenFileBinaryRead(const char* filepath);
    static std::ifstream OpenFileBinaryRead(const wchar_t* filepath);
    static std::ofstream OpenFileBinaryWrite(const char* filepath);
    static std::ofstream OpenFileBinaryWrite(const wchar_t* filepath);
    
    // =======================================================

    template <typename fileType>
    static void CloseFile(fileType& file);

    // =======================================================

    template <typename valueType>
    static inline const valueType ReadValueBinary(std::ifstream& file);
    template <typename valueType>
    static inline void AppendValueBinary(std::ofstream& file, const valueType number);

    // =======================================================
    // additional

    static inline const size_t FileSize(const char* filepath);
    static inline const size_t FileSize(const wchar_t* filepath);

    static inline const bool IsTextFile(const char* filepath);
    static inline const bool IsTextFile(const wchar_t* filepath);

    // determines if the end of the file after reading last character (basic .eof() determine it after reading one more character)
    static inline const bool EndOfBinaryFile(std::ifstream& file);
};


// START IMPLEMENTATION
// ==========================================================================================================

std::ifstream FileUtils::OpenFileRead(const char* filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Failed to open file " + std::string(filepath));
    }
    return file;
}

std::ifstream FileUtils::OpenFileRead(const wchar_t* filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Failed to open file!");
    }
    return file;
}

std::ofstream FileUtils::OpenFileWrite(const char* filepath)
{
    std::ofstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Failed to open file!");
    }
    return file;
}

std::ofstream FileUtils::OpenFileWrite(const wchar_t* filepath)
{
    std::ofstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Failed to open file!");
    }
    return file;
}

std::ifstream FileUtils::OpenFileBinaryRead(const char* filepath)
{
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Failed to open file " + std::string(filepath));
    }
    return file;
}

std::ifstream FileUtils::OpenFileBinaryRead(const wchar_t* filepath)
{
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Failed to open file!");
    }
    return file;
}

std::ofstream FileUtils::OpenFileBinaryWrite(const char* filepath)
{
    std::ofstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Failed to open file " + std::string(filepath));
    }
    return file;
}

std::ofstream FileUtils::OpenFileBinaryWrite(const wchar_t* filepath)
{
    std::ofstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Failed to open file!");
    }
    return file;
}

// ==========================================================================================================

template <typename fileType>
void FileUtils::CloseFile(fileType& file)
{
    if (file.is_open()) {
        file.close();
    }
}

// ==========================================================================================================

template <typename valueType>
const valueType FileUtils::ReadValueBinary(std::ifstream& file)
{
    valueType value;
    file.read(reinterpret_cast<char*>(&value), sizeof(valueType));    
    return value;
}

template <typename valueType>
void FileUtils::AppendValueBinary(std::ofstream& file, const valueType value)
{
    file.write(reinterpret_cast<const char*>(&value), sizeof(valueType));
}

// ==========================================================================================================

// returns size of file in bytes 
const size_t FileUtils::FileSize(const char* filepath)
{
    std::ifstream file(filepath);

    file.seekg(0, std::ios::end);
    size_t result = file.tellg();

    CloseFile(file);

    return result;
}

// returns size of file in bytes
const size_t FileUtils::FileSize(const wchar_t* filepath)
{
    std::wifstream file(filepath);

    file.seekg(0, std::ios::end);
    size_t result = file.tellg();
    
    CloseFile(file);

    return result;
}

// ==========================================================================================================

// returns true if file has .txt extension
const bool FileUtils::IsTextFile(const char* filepath)
{
    std::string filepath_string = std::string(filepath);

    if ((filepath_string[filepath_string.size() - 4] == '.') &&
        (filepath_string[filepath_string.size() - 3] == 't') &&
        (filepath_string[filepath_string.size() - 2] == 'x') &&
        (filepath_string[filepath_string.size() - 1] == 't'))
    {
        return true;
    }

    return false;
}

// returns true if file has .txt extension
const bool FileUtils::IsTextFile(const wchar_t* filepath)
{
    std::wstring filepath_string = std::wstring(filepath);

    if ((filepath_string[filepath_string.size() - 4] == L'.') &&
        (filepath_string[filepath_string.size() - 3] == L't') &&
        (filepath_string[filepath_string.size() - 2] == L'x') &&
        (filepath_string[filepath_string.size() - 1] == L't'))
    {
        return true;
    }

    return false;
}

// ==========================================================================================================

// returns true if file is empty
const bool FileUtils::EndOfBinaryFile(std::ifstream& file)
{
    char c;
    file.read(&c, 1);
    
    if (file.eof()) {
        file.seekg(-1, std::ios_base::cur);
        return true;
    } else {
        file.seekg(-1, std::ios_base::cur);
        return false;
    }
}

// ==========================================================================================================
// END IMPLEMENTATION