#include "./src/soundscaper.h"



sc::SoundScaper scaper = sc::SoundScaper();
const String soundPaths[3] = {
    "/677070__xkeril__hell-drone-with-vibrator.wav",
    "/677071__xkeril__airy-sad-drone-memories.wav",
    "/677138__trevornet__dropwave.wav"
};

void setup() {
    Serial.begin(115200);

    scaper.start();
   
    scaper.addSound(soundPaths[1]);
    scaper.addSound(soundPaths[0]);
    scaper.addSound(soundPaths[0]);
   
    // scaper.clearSounds();
    scaper.changeSoundVolume(1, 0.8);
    scaper.changeSoundVolume(2, 0.8);
}

void loop() {
    scaper.update();
    
    
}