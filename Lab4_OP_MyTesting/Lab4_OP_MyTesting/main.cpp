#include "Libs.hpp"
#include "classWav.hpp"

int main() {
    
    Wav audio;
    audio.ReadWav();
    
    int scale;
    cout << "Scale is "; cin >> scale;
    cout << endl;
    
    audio.setScale(scale);

    system("pause");
    return 0;
}
