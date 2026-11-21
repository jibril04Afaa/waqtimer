

#include <inttypes.h>
#include <stdio.h>

#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_rom_sys.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "pins.h"

static const char* TAG = "sysinfo";

static const char* chip_model_name(esp_chip_model_t model)
{
    switch (model) {
        case CHIP_ESP32:   
            return "ESP32";
        case CHIP_ESP32S2: 
            return "ESP32-S2";
        case CHIP_ESP32S3: 
            return "ESP32-S3";
        case CHIP_ESP32C3: 
            return "ESP32-C3";
        default:           
            return "unknown";
    }
}

/* Prints chip, flash, PSRAM, heap and CPU info once per second.
   Success on the N16R8 board means:
    - Flash: 16 MB
    - PSRAM: about 8 MB
    - CPU: 240 MHz
*/
static void print_static_info(void)
{
    esp_chip_info_t chip;
    esp_chip_info(&chip);

    uint32_t flash_bytes = 0;
    esp_err_t err = esp_flash_get_size(NULL, &flash_bytes);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_flash_get_size failed: %s", esp_err_to_name(err));
    }

    uint32_t cpu_mhz = esp_rom_get_cpu_ticks_per_us();

    ESP_LOGI(TAG, "---------- Waqtimer system info ----------");
    ESP_LOGI(TAG, "Chip      : %s, %d core(s), revision v%d.%d",
             chip_model_name(chip.model), chip.cores,
             chip.revision / 100, chip.revision % 100);
    ESP_LOGI(TAG, "Features  : %s%s%s",
             (chip.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi " : "",
             (chip.features & CHIP_FEATURE_BLE) ? "BLE " : "",
             (chip.features & CHIP_FEATURE_EMB_PSRAM) ? "embedded-PSRAM" : "");
    ESP_LOGI(TAG, "CPU       : %" PRIu32 " MHz (expect 240)", cpu_mhz);
    ESP_LOGI(TAG, "Flash     : %" PRIu32 " MB (expect 16)", flash_bytes / (1024 * 1024));
    ESP_LOGI(TAG, "PSRAM     : %u KB total (expect about 8192)",
             (unsigned)(heap_caps_get_total_size(MALLOC_CAP_SPIRAM) / 1024));
    ESP_LOGI(TAG, "IDF       : %s", esp_get_idf_version());

    /* Prove pins.h compiles and the numbers are what was wired. */
    ESP_LOGI(TAG, "LCD pins  : CS=%d MOSI=%d SCK=%d DC=%d RST=%d BL=%d",
             PIN_LCD_CS, PIN_LCD_MOSI, PIN_LCD_SCK,
             PIN_LCD_DC, PIN_LCD_RST, PIN_LCD_BL);
    ESP_LOGI(TAG, "Btn pins  : B1=%d B2=%d B3=%d B4=%d",
             PIN_BTN_1, PIN_BTN_2, PIN_BTN_3, PIN_BTN_4);
}

void app_main(void)
{
    print_static_info();

    uint32_t tick = 0;
    while (1) {
        ESP_LOGI(TAG,
                 "t=%" PRIu32 "s | internal free %u KB (min %u KB) | PSRAM free %u KB",
                 tick,
                 (unsigned)(heap_caps_get_free_size(MALLOC_CAP_INTERNAL) / 1024),
                 (unsigned)(heap_caps_get_minimum_free_size(MALLOC_CAP_INTERNAL) / 1024),
                 (unsigned)(heap_caps_get_free_size(MALLOC_CAP_SPIRAM) / 1024));
        tick++;
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}