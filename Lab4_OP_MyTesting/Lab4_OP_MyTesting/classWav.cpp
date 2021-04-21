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
    
    
    if (!fin) Error(0);

    else {
        //~~~~~"AUDIO_HEADER"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        //chunkId
        fin.read((char*)&header.chunkId, sizeof(header.chunkId));
        if (header.chunkId != 1179011410) { Error(11); }
        
        //chunkSize
        fin.read((char*)&header.chunkSize, sizeof(header.chunkSize));
        if (header.chunkSize != sizeF - 8) { Error(12); }
        
        //format
        fin.read((char*)&header.format, sizeof(header.format));
        if (header.format != 1163280727) { Error(13); }
        
        //~~~~~"AUDIO_SUBCHUNK1"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        //subchunk1Id
        fin.read((char*)&property.subchunk1Id, sizeof(property.subchunk1Id));
        if (property.subchunk1Id != 544501094) { Error(21);}
        
        //subchunk1Size
        fin.read((char*)&property.subchunk1Size, sizeof(property.subchunk1Size));
        if (property.subchunk1Size != 16)      { Error(22);}
        
        //audioFormat
        fin.read((char*)&property.audioFormat, sizeof(property.audioFormat));
        if (property.audioFormat != 1)         { Error(23);}
        
        //numChannels
        fin.read((char*)&property.numChannels, sizeof(property.numChannels));
        if (property.numChannels < 1)          { Error(24);}
        
        //sampleRate
        fin.read((char*)&property.sampleRate, sizeof(property.sampleRate));
        if (property.sampleRate < 20000  ||  property.sampleRate > 20000000) { Error(25);}
        
        //byteRate
        fin.read((char*)&property.byteRate, sizeof(property.byteRate));
        
        //blockAlign
        fin.read((char*)&property.blockAlign, sizeof(property.blockAlign));
        
        //bitsPerSample
        fin.read((char*)&property.bitsPerSample, sizeof(property.bitsPerSample));
        
        //Checking_Errors
        if (property.byteRate != property.sampleRate * property.numChannels * (property.bitsPerSample / 8)) { Error(26);}
        if (property.blockAlign != property.numChannels * (property.bitsPerSample / 8))                     { Error(27);}
        if (property.bitsPerSample < 0  ||  property.bitsPerSample % 8 != 0  || property.bitsPerSample > 64){ Error(28);}
        
        //~~~~~"AUDIO_SUBCHUNK2"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        //subchunk2Id
        fin.read((char*)&data.subchunk2Id, sizeof(data.subchunk2Id));
        if (data.subchunk2Id != 1635017060) { Error(31);}
        
        //subchunk2Size
        fin.read((char*)&data.subchunk2Size, sizeof(data.subchunk2Size));
        
        //numSamples
        data.numSamples = data.subchunk2Size / (property.bitsPerSample / 8);

        //music
        if (property.bitsPerSample == 8)
        {
            music_8 = new int8_t[data.numSamples];
            for (int i = 0; i < data.numSamples; i++)
                fin.read((char*)&music_8, property.bitsPerSample / 8);
        }
        else  
        {
            music_16 = new int16_t[data.numSamples];
            for (int i = 0; i < data.numSamples; i++)
            fin.read((char*)&music_16, property.bitsPerSample / 8);

        }
    }
}

void Wav::WriteWav()
{
    data.subchunk2Size *= scale;
    header.chunkSize = 36 + data.subchunk2Size;

    ofstream fout(finalF, ios::binary);

    fout.write((char*)&header, sizeof(header));
    fout.write((char*)&property, sizeof(property));

    fout.write((char*)&data.subchunk2Id, sizeof(data.subchunk2Id));
    fout.write((char*)&data.subchunk2Size, sizeof(data.subchunk2Size));
    
    if (property.bitsPerSample == 8)
        for (int i = 0; i < data.numSamples; i++)
            fout.write((char*)&newMusic_8[i], sizeof(newMusic_8[i]));
    else
        for (int i = 0; i < data.numSamples; i++)
            fout.write((char*)&newMusic_16[i], sizeof(newMusic_16[i]));
}

void Wav::InterpolationResize()
{
    data.numSamples *= scale;
    if (property.numChannels == 1 && property.bitsPerSample == 8) newMusic_8 = new int8_t[data.numSamples];
    else newMusic_16 = new int16_t[data.numSamples];

    float step = float(1) / scale;
    for (size_t i = 0; i < data.numSamples; i++)
    {
        float index = i * step;
        int prev = index;
        int next = index + 1;

        if (property.numChannels == 1)
        {
            if (property.bitsPerSample == 8)
                newMusic_8[i] = interpolate(prev, music_8[prev], next, music_8[next], index);
            else 
                newMusic_16[i] = interpolate(prev, music_16[prev], next, music_16[next], index);
        }
        else
        {
            // separate left
            int8_t prev_Left = (int8_t)(music_16[prev] >> 8);
            int8_t next_Left = (int8_t)(music_16[next] >> 8);
            // separate right
            int8_t prev_Right = (int8_t)(music_16[prev] & 0xff);
            int8_t next_Right = (int8_t)(music_16[next] & 0xff);
            // interpolate
            int8_t new_Left = interpolate(prev, prev_Left, next, next_Left, index);
            int8_t new_Right = interpolate(prev, prev_Right, next, next_Right, index);
            // combine left and right
            newMusic_16[i] = (int16_t)((new_Left << 8) + (new_Right & 0x00ff));
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
