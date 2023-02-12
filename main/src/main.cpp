#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "SDcard.h"
#include "Microphone.h"
#include <cstdio>
#include <string.h>

void gatherData(Microphone &microphone, SDcard &filesystem, std::array<int16_t, Microphone::BUFFERDEPTH> &data, uint8_t seconds)
{
  // read data for seconds and save them to the sd card
  for (int i = 0; i < seconds * Microphone::counter; i++)
  {
    microphone.read(data);
    filesystem.save(data);
  }

  // log finished
  vTaskDelay(1 * pdSECOND); // needed?
  filesystem.unmount();
  // log unmound
}

void mainTask()
{
  Microphone microphone;
  SDcard filesystem;
  auto microphoneResult = microphone.init();
  auto fileSystemResult = filesystem.mount();
  std::array<int16_t, Microphone::BUFFERDEPTH> data;

  if (microphoneResult != Microphone::Status::Success)
  {
    ESP_LOGE(Microphone::MicrophoneTAG, "Microphone could not be initialized");
  }

  if (fileSystemResult != SDcard::Status::Success)
  {
    ESP_LOGE(SDcard::SDcardTAG, "Filesytem could not be initialized");
  }
  ESP_LOGI(SDcard::SDcardTAG, "Filesytem initialized");

  gatherData(microphone, filesystem, data, 1);

  while (true)
  {
    vTaskDelay(1 * pdSECOND); // wirklich?
  }
}

extern "C" void app_main()
{
  xTaskCreate((TaskFunction_t)&mainTask, "mainTask", 25 * 1024, NULL, 8, NULL);
  vTaskDelete(NULL);
}