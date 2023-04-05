#include <string.h>

#include <cstdio>

#include "Microphone.h"
#include "SDcard.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void gatherData(Microphone &microphone, SDcard &filesystem, std::array<int16_t, Microphone::BUFFERDEPTH> &data, uint16_t seconds) {
    //filesystem.files_checker();
    // read data for seconds and save them to the sd card
    for (int i = 0; i < seconds; i++) {
        microphone.read(data);
        ESP_LOGI(SDcard::SDcardTAG, "saving data");
        SDcard::Status status = filesystem.save(data);
        if (status == SDcard::Status::Error) {
            filesystem.unmount();
            i = seconds;
        }
    }

    vTaskDelay(100);
    filesystem.unmount();
}

void mainTask() {
    Microphone microphone;
    SDcard filesystem;
    auto microphoneResult = microphone.init();
    auto fileSystemResult = filesystem.mount();
    std::array<int16_t, Microphone::BUFFERDEPTH> data;

    if (microphoneResult != Microphone::Status::Success) {
        ESP_LOGE(Microphone::MicrophoneTAG, "Microphone could not be initialized");
    }

    if (fileSystemResult != SDcard::Status::Success) {
        ESP_LOGE(SDcard::SDcardTAG, "Filesytem could not be initialized");
    }

    ESP_LOGI(SDcard::SDcardTAG, "Filesytem initialized");

    gatherData(microphone, filesystem, data, 610);

    while (true) {
        vTaskDelay(1);
    }
}

extern "C" void app_main() {
    xTaskCreate((TaskFunction_t)&mainTask, "mainTask", 25 * 1024, NULL, 8, NULL);
    vTaskDelete(NULL);
}