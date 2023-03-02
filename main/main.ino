#include <Arduino.h>

#include "AudioFileSourceSD.h"
#include "AudioGeneratorWAV.h"
#include "AudioOutputI2SNoDAC.h"
#include "AudioOutputMixer.h"

#include "FS.h"
#include "SD.h"
#include "SPI.h"

#define BOUD    115200 
#define SCK     12
#define MISO    13
#define MOSI    11
#define CS      15

AudioGeneratorWAV *wav[3];
AudioFileSourceSD *source[3];
AudioOutputI2SNoDAC *out;
AudioOutputMixer *mixer;
AudioOutputMixerStub *stub[3];

const String soundPaths[3] = {
    "/677070__xkeril__hell-drone-with-vibrator.wav",
    "/677071__xkeril__airy-sad-drone-memories.wav",
    "/676957__bernhoftbret__drone-41.wav"
};

void setup()
{
    Serial.begin(115200);
    initSD();
    readTestFile();
    delay(1000);

    Serial.printf("WAV start\n");

    // audioLogger = &Serial;
    source[0] = new AudioFileSourceSD(soundPaths[0].c_str());
    source[1] = new AudioFileSourceSD(soundPaths[1].c_str());
    // source[2] = new AudioFileSourceSD(soundPaths[2].c_str());

    out = new AudioOutputI2SNoDAC();
    // out -> SetGain(4);
    out -> SetBitsPerSample(16);
    out -> SetRate(32000);
    out -> SetPinout(42, 41, 45);
    mixer = new AudioOutputMixer(32, out);

    stub[0] = mixer->NewInput();
    stub[0]->SetGain(0.6);
    stub[1] = mixer->NewInput();
    stub[1]->SetGain(0.2);
    // stub[2] = mixer->NewInput();
    // stub[2]->SetGain(0.1);
    wav[0] = new AudioGeneratorWAV();
    wav[0]->begin(source[0], stub[0]);
    wav[1] = new AudioGeneratorWAV();
    wav[1]->begin(source[1], stub[1]);
    // wav[2] = new AudioGeneratorWAV();
    // wav[2]->begin(source[2], stub[2]);
}

void loop()
{
    if (wav[0]->isRunning()) {
        if (!wav[0]->loop()) { wav[0]->stop(); stub[0]->stop(); Serial.printf("stopping 1\n"); }
    }
    if (wav[1]->isRunning()) {
        if (!wav[1]->loop()) { wav[1]->stop(); stub[1]->stop(); Serial.printf("stopping 2\n"); }
    }
    // if (wav[2]->isRunning()) {
    //     if (!wav[2]->loop()) { wav[2]->stop(); stub[2]->stop(); Serial.printf("stopping 3\n"); }
    // }
}



void initSD() {
    SPI.begin(SCK,MISO,MOSI,CS);
    if (!SD.begin(CS)) {
        Serial.println("Failed to mount card");
        return;
    } else {
        Serial.println("sd mounted!!!");
    }
}


void readTestFile() {
    File file = SD.open("/test.txt", FILE_READ);
    if (!file) {
        Serial.println("Opening file to write failed");
        return;
    } else {
        Serial.println(file.readString());
    }
    file.close();
}
