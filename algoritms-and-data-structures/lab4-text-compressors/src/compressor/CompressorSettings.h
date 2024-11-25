#pragma once

struct CompressorSettings
{
public:
    static void SetHuffmanBlockSize(const size_t size) { HuffmanBlockSize_ = size; }
    static void SetLZ77SearchBufferSize(const size_t size) { LZ77searchBufferSize_ = size; }
    static const size_t GetHuffmanBlockSize() { return HuffmanBlockSize_; }
    static const size_t GetLZ77SearchBufferSize() { return LZ77searchBufferSize_; }
private:
    static size_t HuffmanBlockSize_;
    static size_t LZ77searchBufferSize_;
};

// Set default values
size_t CompressorSettings::HuffmanBlockSize_ = 10000;
size_t CompressorSettings::LZ77searchBufferSize_ = 32768;