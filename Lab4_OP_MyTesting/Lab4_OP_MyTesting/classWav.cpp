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


void Wav::ReadWav () 
{
    ifstream fin(originalF, ios::binary);
    ofstream fout(finalF, ios::binary);
    
    if (!fin) Error(0);

    else {
        //~~~~~"AUDIO_HEADER"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        //chunkId
        fin.read((char*)&header.chunkId, sizeof(header.chunkId));
        if (header.chunkId != 1179011410) { Error(11); }
        fout.write((char*)&header.chunkId, sizeof(header.chunkId));
        
        //chunkSize
        fin.read((char*)&header.chunkSize, sizeof(header.chunkSize));
        if (header.chunkSize != sizeF - 8) { Error(12); }

        header.chunkSize = 36 + (header.chunkSize - 36) * 2;

        fout.write((char*)&header.chunkSize, sizeof(header.chunkSize));
        
        //format
        fin.read((char*)&header.format, sizeof(header.format));
        if (header.format != 1163280727) { Error(13); }
        fout.write((char*)&header.format, sizeof(header.format));
        
        //~~~~~"AUDIO_SUBCHUNK1"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        //subchunk1Id
        fin.read((char*)&property.subchunk1Id, sizeof(property.subchunk1Id));
        if (property.subchunk1Id != 544501094) { Error(21);}
        fout.write((char*)&property.subchunk1Id, sizeof(property.subchunk1Id));
        
        //subchunk1Size
        fin.read((char*)&property.subchunk1Size, sizeof(property.subchunk1Size));
        if (property.subchunk1Size != 16)      { Error(22);}
        fout.write((char*)&property.subchunk1Size, sizeof(property.subchunk1Size));
        
        //audioFormat
        fin.read((char*)&property.audioFormat, sizeof(property.audioFormat));
        if (property.audioFormat != 1)         { Error(23);}
        fout.write((char*)&property.audioFormat, sizeof(property.audioFormat));
        
        //numChannels
        fin.read((char*)&property.numChannels, sizeof(property.numChannels));
        if (property.numChannels < 1)          { Error(24);}
        fout.write((char*)&property.numChannels, sizeof(property.numChannels));
        
        //sampleRate
        fin.read((char*)&property.sampleRate, sizeof(property.sampleRate));
        if (property.sampleRate < 20000  ||  property.sampleRate > 20000000) { Error(25);}
        fout.write((char*)&property.sampleRate, sizeof(property.sampleRate));
        
        //byteRate
        fin.read((char*)&property.byteRate, sizeof(property.byteRate));
        fout.write((char*)&property.byteRate, sizeof(property.byteRate));
        
        //blockAlign
        fin.read((char*)&property.blockAlign, sizeof(property.blockAlign));
        fout.write((char*)&property.blockAlign, sizeof(property.blockAlign));
        
        //bitsPerSample
        fin.read((char*)&property.bitsPerSample, sizeof(property.bitsPerSample));
        fout.write((char*)&property.bitsPerSample, sizeof(property.bitsPerSample));
        
        //Checking_Errors
        if (property.byteRate != property.sampleRate * property.numChannels * (property.bitsPerSample / 8)) { Error(26);}
        if (property.blockAlign != property.numChannels * (property.bitsPerSample / 8))                     { Error(27);}
        if (property.bitsPerSample < 0  ||  property.bitsPerSample % 8 != 0)                                { Error(28);}
        
        //~~~~~"AUDIO_SUBCHUNK2"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        //subchunk2Id
        fin.read((char*)&data.subchunk2Id, sizeof(data.subchunk2Id));
        if (data.subchunk2Id != 1635017060) { Error(31);}
        fout.write((char*)&data.subchunk2Id, sizeof(data.subchunk2Id));
        
        //subchunk2Size
        fin.read((char*)&data.subchunk2Size, sizeof(data.subchunk2Size));
        data.subchunk2Size *= 2;
        fout.write((char*)&data.subchunk2Size, sizeof(data.subchunk2Size));
        
        //numSamples
        cout << "read from file = " << data.subchunk2Size / 2 << endl;
        data.subchunk2Size = sizeF - 44;
        cout << "sizeF - 44 = " << data.subchunk2Size << endl;

        data.numSamples = data.subchunk2Size / (property.bitsPerSample / 8);
        
        //music
        data.music = new int8_t[data.numSamples];

        for (int i = 0; i < data.numSamples; i++) {
            fin.read((char*)&data.music[i], property.bitsPerSample / 8);
            fout.write((char*)&data.music[i], property.bitsPerSample / 8);
            fout.write((char*)&data.music[i], property.bitsPerSample / 8);
        }
    }
}


//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------


void Wav::Error(int errorCase){
    
    switch (errorCase) {
        //~~~~~"FILE_OPENING"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        case 0:
            cout << "Unable to open original file ('sound.wav')!" << endl;
            exit(0);
            
        //~~~~~"AUDIO_HEADER"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        case 11:
            cout << "ChunkId is not correct ('RIFF')!" << endl;
            exit(0);

        case 12:
            cout << "ChunkSize does not equal size: " << header.chunkSize << " != " << sizeF << " !" << endl;
            exit(0);
            
        case 13:
            cout << "Format is not correct ('WAVE')!" << endl;
            exit(0);
        
        //~~~~~"AUDIO_SUBCHUNK1"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        case 21:
            cout << "Subchunk1Id is not correct ('fmt')!" << endl;
            exit(0);
            
        case 22:
            cout << "SubchunkSize is not equal 16!" << endl;
            exit(0);
            
        case 23:
            cout << "AudioFormat is not equal 1!" << endl;
            exit(0);
            
        case 24:
            cout << "nNumChannels is lower than 1!" << endl;
            exit(0);
        
        case 25:
            cout << "SampleRate (" << property.sampleRate << ") is out of hearing range!" << endl;
            exit(0);
        
        case 26:
            cout << "ByteRate is not equal " << property.sampleRate * property.numChannels * (property.bitsPerSample/8) << "!"<< endl;
            cout << property.sampleRate * property.numChannels * (property.bitsPerSample/8) << " = ";
            cout << property.sampleRate << " * " << property.numChannels << " * (" << property.bitsPerSample << " / 8);" << endl;
            cout << "Formula: ByteRate = SampleRate * NumChannels * BitsPerSample/8" << endl;
            exit(0);
            
        case 27:
            cout << "blockAlign is not equal " << property.numChannels * (property.bitsPerSample / 8) << "!"<< endl;
            cout << property.numChannels * (property.bitsPerSample / 8) << " = ";
            cout << property.numChannels << " * (" << property.bitsPerSample << " / 8);" << endl;
            cout << "Formula: blockAlign = numChannels * bitsPerSample/8" << endl;
            exit(0);
            
        case 28:
            cout << "BitsPerSample (" << property.bitsPerSample << ") is not correct!" << endl;
            exit(0);
            
        //~~~~~"AUDIO_SUBCHUNK2"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        case 31:
            cout << "SubChunk2ID is not correct ('data')!" << endl;
            exit(0);
            
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        default:
            exit(0);
    }
}
