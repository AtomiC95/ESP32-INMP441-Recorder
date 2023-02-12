#include "SDcard.h"

// const char *file_foo = MOUNT_POINT "/foo.txt";

SDcard::Status SDcard::mount()
{
    // error handling! keine magic numbers
    ret = gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1);
    if (ret != ESP_OK)
    {
        ESP_LOGE(SDcardTAG,"ESP_INTR_FLAG_LEVEL1 false");
        return Status::Error;
    }

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {};
    mount_config.format_if_mount_failed = true;
    mount_config.max_files = 5;
    mount_config.allocation_unit_size = 16 * 1024;

    host.max_freq_khz = 5000;
    ESP_LOGI(SDcardTAG,"Initializing SD card");

    spi_bus_config_t bus_cfg = {};
    bus_cfg.mosi_io_num = GPIO_NUM_23;
    bus_cfg.miso_io_num = GPIO_NUM_19;
    bus_cfg.sclk_io_num = GPIO_NUM_18;
    bus_cfg.quadwp_io_num = -1;
    bus_cfg.quadhd_io_num = -1;
    bus_cfg.max_transfer_sz = 4000;

    ret = spi_bus_initialize(spi_host_device_t(host.slot), &bus_cfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK)
    {
        ESP_LOGE(SDcardTAG,"Failed to initialize bus.");
        return Status::Error;
    }

    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = GPIO_NUM_5;
    slot_config.host_id = spi_host_device_t(host.slot);

    ESP_LOGI(SDcardTAG,"Mounting filesystem");
    ret = esp_vfs_fat_sdspi_mount(mount_point, &host, &slot_config, &mount_config, &card);
    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            ESP_LOGE(SDcardTAG,"Failed to mount filesystem.");
            return Status::Error;
        }
        else
        {
            ESP_LOGE(SDcardTAG,"Failed to initialize the card (%s). "
                          "Make sure SD card lines have pull-up resistors in place.",
                     esp_err_to_name(ret));
            return Status::Error;
        }
    }
    ESP_LOGI(SDcardTAG,"Filesystem mounted");
    sdmmc_card_print_info(stdout, card);
    return Status::Success;
}

SDcard::Status SDcard::save(std::array<int16_t, Microphone::BUFFERDEPTH>& outbuffer_i2s0)
{
    FILE *f = fopen(INMP441, "a");
    if (f == NULL)
    {
        ESP_LOGE(SDcardTAG,"Failed to open file for writing");
        return Status::Error;
    }
    for (int i = 0; i < outbuffer_i2s0.size(); i++)
    {
        fprintf(f, "%d\n", outbuffer_i2s0[i]);
    }
    fclose(f);
    ESP_LOGI(SDcardTAG,"File has been closed");
    return Status::Success;
}

SDcard::Status SDcard::unmount(){

    // error handling? 
        // deinitialize the bus after all devices are removed    // All done, unmount partition and disable SPI peripheral
    esp_vfs_fat_sdcard_unmount(mount_point, card);
    ESP_LOGI(SDcardTAG,"Card unmounted");

    // deinitialize the bus after all devices are removed
    spi_bus_free(spi_host_device_t(host.slot));
    return Status::Success;
}