#include "./src/soundscaper.h"



sc::SoundScaper scaper = sc::SoundScaper();
const String soundPaths[3] = {
    "/677070__xkeril__hell-drone-with-vibrator.wav",
    "/677071__xkeril__airy-sad-drone-memories.wav",
    "/677138__trevornet__dropwave.wav"
};

//Max sliderpins  
const int sliderPins = 8;
int slider[sliderPins];
//used sliderpins
int slider [sliderPins] = {1,2,3,4,5,6,7,8,};
//Analog Read sliders
int sliderVal[sliderPins];


void setup() {
    Serial.begin(115200);

    // pinMode(SLIDERPIN, INPUT);

    initSlider(sliderPins);

    scaper.start();
   
    scaper.addSound(soundPaths[1]);
    scaper.addSound(soundPaths[2]);

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
        pinMode(slider[i], INPUT);
    }
}

void checkSliders(){
    for (int i = 0; i <sliderPins; i++){
        sliderVal[i] = (lastInput*1.9 + analogRead(slider[i])*0.1)/2;
        newVolume = mapd(sliderVal[i], 0, 8200, 0.0, 2.0);
        scaper.changeSoundVolume(i, newVolume);
    }
}


double mapd(double x, double in_min, double in_max, double out_min, double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}