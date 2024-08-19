
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>

constexpr gpio_num_t LED_PIN = GPIO_NUM_2;
//constexpr gpio_num_t INPUT_PIN = GPIO_NUM_32; // push button

// Añadamimos el mangling 
extern "C" void app_main();

void app_main() 
{
    // Setup
    // config ouput pin
    gpio_config_t config_output;
    config_output.mode = GPIO_MODE_OUTPUT;
    config_output.pin_bit_mask = (1ULL << LED_PIN);
    config_output.intr_type = GPIO_INTR_DISABLE;
    config_output.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config_output.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&config_output);



    // Loop
    while(1)
    {

        printf("Led encendido\n");
        gpio_set_level(LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(500));
        printf("Led apagado\n");
        gpio_set_level(LED_PIN, 0);

        vTaskDelay(pdMS_TO_TICKS(500));

    }

}