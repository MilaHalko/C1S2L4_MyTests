#include "Libs.hpp"
#include "classWav.hpp"

int main() {
    
    Wav audio;
    audio.ReadWav();
    
    return 0;
}


/*
 int32_t swap_int32_t(int32_t);
 void endError(string);
 void Reader();

 template <typename T>
 T interpolate(int32_t x0, T y0, int32_t x1, T y1, float x);
 template <typename T>
 void scaleTrack(T* audio);

public:
 TrackWave();
 void scaleFile();
 void showInfo();
 */
