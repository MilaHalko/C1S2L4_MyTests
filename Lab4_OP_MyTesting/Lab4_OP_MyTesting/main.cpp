#include "Libs.hpp"
#include "classWav.hpp"

int main() {
    
    Wav audio;
    audio.ReadWav();
    
    float scale;
    cout << "Scale is "; cin >> scale;
    
    audio.setScale(scale);

    cout << "Want reverse the audio? (y / n)\n";
    char ch; cin >> ch;
    if (ch == 'y') audio.reverse = true;

    audio.InterpolationResize();
    audio.WriteWav();

    system("pause");
    return 0;
}
