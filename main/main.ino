#include <Arduino.h>

#include "AudioFileSourcePROGMEM.h"
#include "AudioFileSource.h"
#include "AudioFileSourceSD.h"
#include "AudioGeneratorWAV.h"
#include "AudioOutputI2SNoDAC.h"
#include "AudioOutputI2S.h"

// VIOLA sample taken from https://ccrma.stanford.edu/~jos/pasp/Sound_Examples.html
#include "./src/viola.h"

#include "FS.h"
#include "SD.h"
#include "SPI.h"

#define BOUD    115200 
#define SCK     12
#define MISO    13
#define MOSI    11
#define CS      15



AudioGeneratorWAV *wav;
AudioFileSourceSD *source;
AudioOutputI2SNoDAC *out;

void setup()
{
    Serial.begin(115200);
    initSD();
    readTestFile();
    delay(1000);

    Serial.printf("WAV start\n");

    
    // audioLogger = &Serial;
    // File source = SD.open('./piano/A3.wav','r');
    source = new AudioFileSourceSD();
    source->open("/piano/A3.wav");

    out = new AudioOutputI2SNoDAC();
    out -> SetGain(4);
    out -> SetPinout(42, 41, 45);
    wav = new AudioGeneratorWAV();
    
    delay(500);
    wav->begin(source, out);
}

void loop()
{
  if (wav->isRunning()) {
    if (!wav->loop()) wav->stop();
  } else {
    Serial.printf("WAV done\n");
    delay(1000);
  }
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
