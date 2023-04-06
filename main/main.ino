#include "./src/soundscaper.h"



sc::SoundScaper scaper = sc::SoundScaper();
const String soundPaths[3] = {
    "/677070__xkeril__hell-drone-with-vibrator.wav",
    "/677071__xkeril__airy-sad-drone-memories.wav",
    "/677138__trevornet__dropwave.wav"
};

#define SLIDERPIN 1

void setup() {
    Serial.begin(115200);

    pinMode(SLIDERPIN, INPUT);

    scaper.start();
   
    scaper.addSound(soundPaths[1]);
    // scaper.addSound(soundPaths[0]);
    // scaper.addSound(soundPaths[0]);
   
    // scaper.clearSounds();
    // scaper.changeSoundVolume(0, 0.6);
    scaper.changeSoundVolume(0, 0);
    // scaper.changeSoundVolume(2, 1);
}


int lastInput = 0;
double newVolume = 0;
void loop() {
    scaper.update();
    

    lastInput = (lastInput*1.9 + analogRead(SLIDERPIN)*0.1)/2;
    newVolume = mapd(lastInput, 0, 8200, 0.0, 2.0);

    scaper.changeSoundVolume(0, newVolume);
    Serial.println(newVolume);
    
}





double mapd(double x, double in_min, double in_max, double out_min, double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}