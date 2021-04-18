#pragma once
#include "Libs.hpp"
#include "structuresWavParts.hpp"


class Wav {
    
    RiffHeader header;
    SubChunkFMT format;
    SubChunkData data;
    string originalF = "";
    string finalF;
    int sizeF;
    
public:
    Wav();
    void ReadWav();
    void getAllData();
    void ChangeWav();
    void Error(int);
    int getSizeF();
    
    template <typename T>
    void CheckData(T data);
};

