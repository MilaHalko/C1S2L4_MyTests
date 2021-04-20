#pragma once
#include "Libs.hpp"

struct RiffHeader {
    int32_t chunkId;
    int32_t chunkSize;
    int32_t format;
};

struct SubChunkFMT {
    int32_t subchunk1Id;
    int32_t subchunk1Size;
    int16_t audioFormat;
    int16_t numChannels;
    int32_t sampleRate;
    int32_t byteRate;
    int16_t blockAlign;
    int16_t bitsPerSample;
};

struct SubChunkData {
    int32_t subchunk2Id;
    int32_t subchunk2Size;
    int64_t *music;
    int numSamples;
};


