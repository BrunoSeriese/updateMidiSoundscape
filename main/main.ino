#include "./src/soundscaper.h"

sc::SoundScaper scaper = sc::SoundScaper();
const String soundPaths[7] = {
    "/s1.wav",
    "/s2.wav",
    "/s3.wav",
    "/s4.wav",
    "/s5.wav",
    "/s6.wav",
    "/s7.wav"
};

const int sliderPins = 7;                   //Max sliderpins
int slider [sliderPins] = {1,2,3,4,5,6,7};  //used sliderpins
int sliderVal[sliderPins] = {0,0,0,0,0,0,0};                  //Analog Read sliders


void setup() { 
    Serial.begin(115200);
    initSlider(sliderPins);
}


void loop() {
    scaper.update();
    checkSliders();  
}


void initSlider(int sliders){
    scaper.start();

    for (int i = 0; i< sliders; i++){
        pinMode(slider[i], INPUT);
        scaper.addSound(soundPaths[i]);
        scaper.changeSoundVolume(i, 32);
    }
}

int sliderIndex=0;
void checkSliders(){
    sliderVal[sliderIndex] = (sliderVal[sliderIndex]*1.9
     + analogRead(slider[sliderIndex])*0.1)/2;
    int newVolume = map(sliderVal[sliderIndex], 0, 8200, 0, 32);
    scaper.changeSoundVolume(sliderIndex, newVolume);
    
    sliderIndex ++;
    if (sliderIndex > sliderPins-1) sliderIndex = 0;
}


double mapd(double x, double in_min, double in_max, double out_min, double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}