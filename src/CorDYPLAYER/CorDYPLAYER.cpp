/**
 * @file CorMeteorology.cpp
 * @brief Displays real time weather information.
 *          in the electronic design implements the DYPLAYER module.
 * @version 1.0
 * @author ICI/INF Eugenio Muñoz Sobenes
 * @Contact: eugeniomunozsobenes@gmail.com
 * @date 2024-03-01
 */
#include "CorDYPLAYER/CorDYPLAYER.h"
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <HardwareSerial.h>
#include <memory>
HardwareSerial HwSerial(_HW_SERIAL);

CorDYPLAYER::CorDYPLAYER(void)
{
}
void CorDYPLAYER::begin(void)
{ return;
    player = DY::Player(&HwSerial);
    player.begin();
    player.setVolume(30); // 100% Volume
}
void CorDYPLAYER::sayWelcome(void)
{
}
void CorDYPLAYER::sayTheRollInclination(float angle)
{ return;
    int i = 0;
    char buf[6];
   
    voicesBuffer[i++] = _VOICE_ROLL_INCLINATOR; // Inclinación roll
    dtostrf(abs(angle), 2, 0, buf);
    voicesBuffer[i++] = buf;
    voicesBuffer[i++] = _VOICE_GRADES; // grados
    sendToDYPLAYER(i);
}
void CorDYPLAYER::sayThePitchInclination(float angle)
{ return;
    int i = 0;
    char buf[6];
    voicesBuffer[i++] = _VOICE_PITCH_INCLINATOR; // Inclinación pitch
    dtostrf(abs(angle), 2, 0, buf);
    voicesBuffer[i++] = buf;
    voicesBuffer[i++] = _VOICE_GRADES; // grados
    sendToDYPLAYER(i);
}
void CorDYPLAYER::sayTheCurrentTime(uint8_t hour, uint8_t minute)
{ return;
    int i = 0;
    char buf[6];
    dtostrf(hour, 2, 0, buf);
    voicesBuffer[i++] = buf;
    voicesBuffer[i++] = _VOICE_HOUR; // hora
    dtostrf(minute, 2, 0, buf);
    voicesBuffer[i++] = buf;
    voicesBuffer[i++] = _VOICE_MINUTES; // minutos
    sendToDYPLAYER(i);
}
void CorDYPLAYER::sayTheTemperature(float temperature)
{ return;
    int i = 0;
    char buf[6];
    voicesBuffer[i++] = _VOICE_TEMPERATURE; // temperature
    dtostrf(abs(temperature), 2, 0, buf);
    voicesBuffer[i++] = buf;
    voicesBuffer[i++] = _VOICE_GRADES; // grados
    sendToDYPLAYER(i);
}
void CorDYPLAYER::sayTheHumidity(float humedity)
{
}
void CorDYPLAYER::sayThePressure(float pressure)
{
}
void CorDYPLAYER::sendToDYPLAYER(int numberOfWords)
{ return;
    int size = 8;
    char *sounds[size];
    //std::unique_ptr<char[]> sounds(new char[size]);
    if (player.checkPlayState() != DY::PlayState::Playing)
    {
        for (int i = 0; i < numberOfWords; i++)
        {
            sounds[i] = (char *)malloc(sizeof(char) * (2 + 1)); // two chars +  'end' char
            strcpy(sounds[i], voicesBuffer[i].c_str());
            //sounds[i] = (char)voicesBuffer[i].c_str();
        }
        player.combinationPlay(sounds, numberOfWords);
        for (int i = 0; i < numberOfWords; i++)
        {
            free(sounds[i]);
        }
    }
    if (player.checkPlayState() == DY::PlayState::Stopped || player.checkPlayState() == DY::PlayState::Fail)
    {
        // free(sounds);
    }
}