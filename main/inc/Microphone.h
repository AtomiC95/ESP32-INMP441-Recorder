#pragma once

#include <cstdint>
#include <array>
#include "Constants.h" // is this needed?

class Microphone // Rename to Microphone?
{

private:
    // check which one's are needed to be public..? -> make private
    static constexpr uint32_t I2S_SAMPLE_RATE = 8000; // nachschauen: constexpr
    static constexpr uint8_t I2S_WS_I2S0 = 25;
    static constexpr uint8_t I2S_SD_I2S0 = 33;
    static constexpr uint8_t I2S_SCK_I2S0 = 26;
public:
    enum class Status
    {
        Success,
        Error,
    };
    static constexpr const char *MicrophoneTAG = "Microphone";
    static constexpr uint16_t BUFFERDEPTH = 512;
    static constexpr uint16_t counter = (I2S_SAMPLE_RATE/BUFFERDEPTH) + 1;

    // outbuffers are filled in one call of Read
    // a buffer array is written to according to the following pattern
    // [LSB, sample1-left, sample2-left, MSB, LSB, sample1-right, sample2-right, MSB]
    Status init();
    Status read(std::array<int16_t, Microphone::BUFFERDEPTH> &data);

    // --> Alle funktionen klein schreiben
};