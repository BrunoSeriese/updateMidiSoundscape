#include "./src/soundscaper.h"



sc::SoundScaper scaper = sc::SoundScaper();
const String soundPaths[3] = {
    "/677070__xkeril__hell-drone-with-vibrator.wav",
    "/677071__xkeril__airy-sad-drone-memories.wav",
    "/676957__bernhoftbret__drone-41.wav"
};

void setup() {
    Serial.begin(115200);

    scaper.start();
    scaper.addSound(soundPaths[0]);
    scaper.addSound(soundPaths[1]);
}

void loop() {
    scaper.update();
}