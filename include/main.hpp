#pragma once
#include <iostream>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_system.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "driver/uart.h"
#include "esp_vfs_dev.h"

#include "cJSON.h"

static SemaphoreHandle_t mutex;

void serial_task(void *pvParameter);

extern "C"
{
    void app_main();
}