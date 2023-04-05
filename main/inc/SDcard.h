#pragma once
#include <sys/stat.h>
#include <sys/unistd.h>

#include <array>
#include <string>

#include "Microphone.h"
#include "SDcard.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"

#define mount_point "/SDcard"
class SDcard {
   public:
    enum class Status {
        Success,
        Error,
    };

    const std::string BASE_FILENAME = "/SDcard/inmp441.txt";
    static constexpr const char *SDcardTAG = "SDcard";
    static constexpr uint8_t MAX_FILES = 5;
    static constexpr uint32_t ALLOCATION_UNIT_SIZE = 16 * 1024;
    static constexpr int8_t NOT_USED = -1;
    static constexpr uint32_t MAX_BUS_FREQUENZ = 5000;
    static constexpr int32_t MAX_TRANSFER_IN_BYTES = 4000;
    struct stat st;
    uint8_t files_created = 0;
    FILE *f = nullptr;
    sdmmc_card_t *card = nullptr;
    esp_err_t ret;
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    std::string filename = "/SDcard/d.txt";

    SDcard::Status mount();
    SDcard::Status unmount();
    // SDcard::Status files_checker();
    SDcard::Status save(std::array<int16_t, Microphone::BUFFERDEPTH> &outbuffer_i2s0);  // outbuffer als const reference, sample rate -> stdint typen nehmen (unitX_t)
   private:
};

// #pragma once
// #include <sys/stat.h>
// #include <sys/unistd.h>

// #include <array>
// #include <string>

// #include "Microphone.h"
// #include "SDcard.h"
// #include "esp_vfs_fat.h"
// #include "sdmmc_cmd.h"

// #define mount_point "/SDcard"
// class SDcard {
//    public:
//     enum class Status {
//         Success,
//         Error,
//     };

//     const std::string BASE_FILENAME = "/SDcard/INMP441.txt";
//     static constexpr const char *SDcardTAG = "SDcard";
//     static constexpr uint8_t MAX_FILES = 5;
//     static constexpr uint32_t ALLOCATION_UNIT_SIZE = 16 * 1024;
//     static constexpr int8_t NOT_USED = -1;
//     static constexpr uint32_t MAX_BUS_FREQUENZ = 5000;
//     static constexpr int32_t MAX_TRANSFER_IN_BYTES = 4000;
//     struct stat st;
//     uint8_t files_created = 0;
//     FILE *f = nullptr;
//     sdmmc_card_t *card = nullptr;
//     esp_err_t ret;
//     sdmmc_host_t host = SDSPI_HOST_DEFAULT();
//     std::string filename;

//     SDcard::Status mount();
//     SDcard::Status unmount();
//     SDcard::Status files_checker();
//     int file_exists(const char *path);
//     SDcard::Status save(std::array<int16_t, Microphone::BUFFERDEPTH> &outbuffer_i2s0);  // outbuffer als const reference, sample rate -> stdint typen nehmen (unitX_t)
//    private:
// };