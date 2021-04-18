#include "classWav.hpp"

Wav::Wav() {
    
    originalF = "sound.wav";
    finalF = "final.wav";
    
    sizeF = getSizeF();
}


int Wav::getSizeF() {
    
    int size;
    
    fstream file(originalF);
    file.seekg(0, ios::end);
    size = file.tellg();
    file.close();
    
    return size;
}


//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------


void Wav::ReadWav () {
    
    ifstream fin(originalF, ios::binary);
    
    if (!fin) {Error(0);}
    else {
        fin.read((char*)&header.chunkId, sizeof(header.chunkId));
        if (header.chunkId != 1179011410) { Error(1);}
        
        fin.read((char*)&header.chunkSize, sizeof(header.chunkSize));
        if (header.chunkSize != sizeF - 8) { Error(2);}
        
        fin.read((char*)&header.format, sizeof(header.format));
        if (header.format != 1163280727)   { Error(3);}
    }
}


//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------


void Wav::Error(int errorCase){
    
    switch (errorCase) {
        case 0:
            cout << "Unable to open original file ('sound.wav')!" << endl;
            exit(0);
            
        case 1:
            cout << "ChunkId is not correct ('RIFF')!" << endl;
            exit(0);
            
        case 2:
            cout << "ChunkSize does not equal size: " << header.chunkSize << " != " << sizeF << " !" << endl;
            exit(0);
            
        case 3:
            cout << "Format is not correct ('WAVE')!" << endl;
            exit(0);
            
        default:
            exit(0);
    }
}



/*
 int32_t chunkId;   // Завжди містить значення 0x52494646 (літери "RIFF")
 int32_t chunkSize; // 36 + розмір другого підрозділу в байтах
                    // Іншими словами 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
                    // Це розмір всього файла мінус 8 байтів які займають поля chunkId та chunkSize
 int32_t format;    // Для wav-файла це завжди 0x57415645 (літери "WAVE")
};

struct SubChunkFMT {
 int32_t subchunk1Id;   // Завжди 0x666d7420 – літери "fmt "
 int32_t subchunk1Size; // Завжди 16 для аудіо PCM. Це розмір частини підрозділу, що слідує після цього числа
 int16_t audioFormat;   // PCM = 1
 int16_t numChannels;   // Mono = 1, Stereo = 2
 int32_t sampleRate;    // Наприклад 44100
 int32_t byteRate;      // == SampleRate * NumChannels * BitsPerSample/8
 int16_t blockAlign;    // == NumChannels * BitsPerSample/8
 int16_t bitsPerSample; // 8 bits = 8, 16 bits = 16, etc.
};

struct SubChunkData {
 int32_t subchunk2Id;   // 0x64617461 – літери "data"
 int32_t subchunk2Size; // == NumSamples * NumChannels * BitsPerSample/8, кількість байтів аудіоданих
};
 */
