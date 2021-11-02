#include "main.hpp"

void serial_task(void *pvParameter)
{
    while (1)
    {
        static int saw = 0;
        saw++;

        int num = esp_random() % 100;
        if (saw == 100)
        {
            saw = 0;
        }

        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, "num", num);
        cJSON_AddNumberToObject(root, "saw", saw);

        if (num <= 40)
        {
            cJSON_AddStringToObject(root, "info", "Low");
        }
        else if (num >= 80)
        {
            cJSON_AddStringToObject(root, "info", "Hi");
        }
        else
        {
            cJSON_AddStringToObject(root, "info", "Ok");
        }

        xSemaphoreTake(mutex, portMAX_DELAY);
        std::cout << cJSON_PrintUnformatted(root) << std::endl;
        xSemaphoreGive(mutex);
        cJSON_Delete(root);
        vTaskDelay(2000 / portTICK_RATE_MS);
    }
}

void app_main()
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Disable buffer
    setvbuf(stdin, nullptr, _IONBF, 0);
    setvbuf(stdout, nullptr, _IONBF, 0);

    mutex = xSemaphoreCreateMutex();

    // Enable cin uart
    esp_vfs_dev_uart_use_driver(0);
    ESP_ERROR_CHECK(uart_driver_install((uart_port_t)CONFIG_ESP_CONSOLE_UART_NUM,
                                        256, 0, 0, nullptr, 0));

    xTaskCreate(&serial_task, "serial_task", 2048, nullptr, 10, nullptr);
}