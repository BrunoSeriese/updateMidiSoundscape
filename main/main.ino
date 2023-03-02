#include <Arduino.h>

#include "AudioFileSourcePROGMEM.h"
#include "AudioFileSource.h"
#include "AudioGeneratorWAV.h"
#include "AudioOutputI2SNoDAC.h"
#include "AudioOutputI2S.h"

// VIOLA sample taken from https://ccrma.stanford.edu/~jos/pasp/Sound_Examples.html
#include "./src/viola.h"

AudioGeneratorWAV *wav;
AudioFileSourcePROGMEM *file;
AudioOutputI2SNoDAC *out;

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.printf("WAV start\n");

  audioLogger = &Serial;
  file = new AudioFileSourcePROGMEM( viola, sizeof(viola) );
  out = new AudioOutputI2SNoDAC();
  out -> SetGain(4);
  out -> SetPinout(42, 41, 45);
  wav = new AudioGeneratorWAV();
  wav->begin(file, out);
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
