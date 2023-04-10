#include "./src/soundscaper.h"



sc::SoundScaper scaper = sc::SoundScaper();
const String soundPaths[3] = {
    "/677070__xkeril__hell-drone-with-vibrator.wav",
    "/677071__xkeril__airy-sad-drone-memories.wav",
    "/677138__trevornet__dropwave.wav"
};

// #define SLIDERPIN 1


const int sliderPins = 7;
int slider [sliderPins] = {};


void setup() {
    Serial.begin(115200);

    // pinMode(SLIDERPIN, INPUT);

    initSlider(sliderPins);

    scaper.start();
   
    scaper.addSound(soundPaths[1]);
    scaper.addSound(soundPaths[2]);
    // scaper.addSound(soundPaths[0]);
   
    // scaper.clearSounds();
    // scaper.changeSoundVolume(0, 0.6);
    scaper.changeSoundVolume(0, 0);
    scaper.changeSoundVolume(1, 0.6);
    // scaper.changeSoundVolume(2, 1);
}


int lastInput = 0;
double newVolume = 0;
void loop() {
    scaper.update();
    
    checkSliders();
    

    scaper.changeSoundVolume(0, newVolume);
    
}


void initSlider(int sliders){
    for (int i = 0; i< sliders; i++){
        pinMode(i+1, INPUT);
        slider[i] = i+1;
    }
}

void checkSliders(){
    for (int i = 0; i <sliderPins; i++){
        lastInput = (lastInput*1.9 + analogRead(slider[i])*0.1)/2;
        newVolume = mapd(lastInput, 0, 8200, 0.0, 2.0);
    }
}


double mapd(double x, double in_min, double in_max, double out_min, double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}