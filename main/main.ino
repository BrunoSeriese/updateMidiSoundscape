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
int sliderVal[sliderPins];                  //Analog Read sliders


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
    }
}

void checkSliders(){
    for (int i = 0; i <sliderPins; i++){
        sliderVal[i] = (sliderVal[i]*1.9 + analogRead(slider[i])*0.1)/2;
        double newVolume = mapd(sliderVal[i], 0, 8200, 0.0, 2.0);
        scaper.changeSoundVolume(i, newVolume);
    }
}


double mapd(double x, double in_min, double in_max, double out_min, double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}