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
    
public:
    Wav();
    int getSizeF();
    
    void ReadWav();
    void getAllData();
    void ChangeWav();
    
    void Error(int);
};

