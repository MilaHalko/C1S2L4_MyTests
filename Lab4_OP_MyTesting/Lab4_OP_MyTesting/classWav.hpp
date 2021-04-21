#pragma once
#include "Libs.hpp"
#include "structuresWavParts.hpp"


class Wav {
    
    RiffHeader header;
    SubChunkFMT property;
    SubChunkData data;
    string originalF = "";
    string finalF;
    int sizeF;
    float scale;

    int8_t* music_8;
    int16_t* music_16;
    int8_t* newMusic_8;
    int16_t* newMusic_16;

    int8_t interpolate(int32_t x0, int8_t y0, int32_t x1, int8_t y1, float x) {
        return y0 + ((x - x0) / (x1 - x0)) * (y1 - y0);
    }
    int16_t interpolate(int32_t x0, int16_t y0, int32_t x1, int16_t y1, float x) {
        return y0 + ((x - x0) / (x1 - x0)) * (y1 - y0);
    }
public:
    Wav();
    int getSizeF();
    void setScale(float scale) {this->scale = scale;}
    
    void ReadWav();
    void InterpolationResize();
    void WriteWav();
    
    void Error(int);
};

