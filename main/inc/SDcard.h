#pragma once
#include "SDcard.h"
#include "Microphone.h"
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include <array>

#define mount_point "/SDcard"
class SDcard
{
public:
    enum class Status
    {
        Success,
        Error,
    };

    static constexpr const char* SDcardTAG = "SDcard";
    struct stat st;
    uint8_t file_created;
    sdmmc_card_t *card = nullptr; // pointer? ist uninitializiert
    esp_err_t ret;
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    const char *INMP441 = mount_point "/INMP441.txt"; // std::string

    SDcard::Status mount();
    SDcard::Status unmount();
    SDcard::Status save(std::array<int16_t, Microphone::BUFFERDEPTH> &outbuffer_i2s0); // outbuffer als const reference, sample rate -> stdint typen nehmen (unitX_t)

private:
};