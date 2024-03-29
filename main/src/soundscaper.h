#ifndef SOUNDSCAPER_H
#define SOUNDSCAPER_H

#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <driver/i2s.h>

#define debug true
#define maxSounds 7
#define maxFileStrams 7
int currentSounds = 0;
const int bufferLen = 256;

const i2s_port_t I2S_PORT = I2S_NUM_0;
#define BLCK_PIN 42
#define LRC_PIN 41
#define DIN_PIN 45
const int sampleRate = 32000;
const int16_t maxValue = 32000;
const int16_t minValue = -32000;

#define SCK     12
#define MISO    13
#define MOSI    11
#define CS      10


void initSD() {
    pinMode(14,OUTPUT);
    digitalWrite(14,HIGH);
    SPI.begin(SCK,MISO,MOSI,CS);
    if (!SD.begin(CS, SPI, 8000000U, "/sd", maxFileStrams, false)) {
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE) {
        Serial.println("No SD card attached");
        return;
    }
    Serial.println("Done SD card...");
    if (!SD.begin(CS)) {
        Serial.println("ERROR - SD card initialization failed!");
        return;    // init failed
    }
}

void i2s_install() {
  // Set up I2S Processor configuration
  const i2s_config_t i2s_config = {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = sampleRate,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 256,
    .use_apll = true
  };
 
  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
}

void i2s_setpin() {
  // Set I2S pin configuration
  const i2s_pin_config_t pin_config = {
    .bck_io_num = BLCK_PIN,
    .ws_io_num = LRC_PIN,
    .data_out_num = DIN_PIN,
    .data_in_num = -1
  };
 
  i2s_set_pin(I2S_PORT, &pin_config);
}


namespace sc { 
    
    
class Sound {
    public:
        String name;
        File source;
        int16_t buffer[bufferLen];
        int volume = 32;
        
        Sound() {
            initBuffer();
        }
        Sound(String filepath) {
            name = filepath;
            source = SD.open(name);
            if (!source && debug) {
                Serial.println("File not found: "+name);
            }
            initSource();
        }
        ~Sound() {
            source.close();
        }

        void update() {
            if (!source) return;
            if (source.position() < source.size()-1) {
                readIntoBuffer();
            } else {
                restartSource();
            } 
        }


    private:
        const int bufferSize = sizeof(buffer);
        void initBuffer() {
            for (int i = 0; i < bufferLen; i++)
            {
                buffer[i] = 0;
            }
        }

        void readIntoBuffer() {
            int numBytes = _min(sizeof(buffer), source.size() - source.position() - 1);
            source.readBytes((char*)buffer, numBytes);
        }

        void restartSource() {
            Serial.println("restarting source: "+name);
            source.close();
            source = SD.open(name);
            initSource();
        }

        void initSource() {
            initBuffer();
            skipHeader();
        }

        void skipHeader() {
            int16_t tempBuffer[40];
            source.readBytes((char*)tempBuffer,sizeof(tempBuffer));
        }
};


class SoundScaper {
    public:
        SoundScaper() {
            for (int i = 0; i < maxSounds; i++)
            {
                sounds[i] = new Sound();
            } 
        }
        ~SoundScaper() {
            for (int i = 0; i < currentSounds; i++)
            {
                free(sounds[i]);
            }
        }

        void addSound(String filepath) {
            if (createIndex == maxSounds){
                createIndex = 0;
                free(sounds[createIndex]);
                sounds[createIndex] = new Sound(filepath);
                createIndex++;
            } else {
                free(sounds[createIndex]);
                sounds[createIndex] = new Sound(filepath);
                createIndex++;
                if (currentSounds < maxSounds){
                    currentSounds++;
                }
                
            }
           
        }

        void clearSounds(){
         for (int i = 0; i < maxSounds; i++) {

            free(sounds[i]);
            sounds[i] = new Sound();
            currentSounds = 0;
            }
        }

        void changeSoundVolume(int index, int volume) {

        if (sounds[index]){
            sounds[index]->volume = volume;
        }
            
        }

        void start() {
            Serial.println("Setting up sd");
            initSD();
            Serial.println("Installing i2s driver");
            i2s_install();
            Serial.println("Setting i2s pins");
            i2s_setpin();
            Serial.println("Starting i2s");
            i2s_start(I2S_PORT);
            Serial.println("Ready");
        }

        void update() {
            // clear buffer
            for (int i = 0; i < bufferLen; i++)
            {
                buffer[i] = 0;
            }
            
            // update sounds
            for (int i = 0; i < currentSounds; i++)
            {
                sounds[i]->update();
            }

            // get samples from sounds buffers
            for (int i = 0; i < bufferLen; i++) {
                for (int j = 0; j < currentSounds; j++)
                {
                    sample += sounds[j]->buffer[i]*sounds[j]->volume;      // read sample
                }
                
                sample /=256;

                if (sample < minValue) {
                    Serial.println("clipping1");
                    sample = minValue;}
                if (sample > maxValue) {
                    Serial.println("clipping2");
                    sample = maxValue;}
                buffer[i] = sample;
            }

            // write to i2s buffer
            size_t i2s_bytes_write = 0; 
            i2s_write(I2S_PORT, buffer, bufferSize, &i2s_bytes_write, 10);
        }

        void end() {
            i2s_stop(I2S_PORT);
        }

    private:
        Sound* sounds[maxSounds];
        int createIndex = 0;
        int16_t buffer[bufferLen];
        const int bufferSize = sizeof(buffer);
        int32_t sample = 0;
};

    

}


#endif