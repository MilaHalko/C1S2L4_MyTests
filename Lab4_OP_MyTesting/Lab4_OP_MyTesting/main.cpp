#include "Libs.hpp"
#include "classWav.hpp"

int main() {
    
    Wav audio;
    audio.ReadWav();
    
    float scale;
    cout << "Scale is "; cin >> scale;
    
    audio.setScale(scale);

    audio.InterpolationResize();
    audio.WriteWav();

    system("pause");
    return 0;
}
