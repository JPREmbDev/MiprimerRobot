
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>

constexpr gpio_num_t LED_PIN = GPIO_NUM_2;
constexpr gpio_num_t INPUT_PIN = GPIO_NUM_32; // push button

// Añadamimos el mangling 
extern "C" void app_main();

void app_main() 
{
    // Setup
    // Config ouput pin LED
    gpio_config_t config_output;
    config_output.mode = GPIO_MODE_OUTPUT;
    config_output.pin_bit_mask = (1ULL << LED_PIN);
    config_output.intr_type = GPIO_INTR_DISABLE;
    config_output.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config_output.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&config_output);

    // Config input pin PULSADOR
    /*
        Esta config funciona de tal manera que cuando pulsamos le llega un 0:
            config_input.pull_down_en = GPIO_PULLDOWN_DISABLE;
            config_input.pull_up_en = GPIO_PULLUP_ENABLE;
        
        Esta config funciona de tal manera que cuando pulsamos le llega un 1:
            config_input.pull_down_en = GPIO_PULLDOWN_ENABLE;
            config_input.pull_up_en = GPIO_PULLUP_DISABLE;
    */
    gpio_config_t config_input;
    config_input.mode = GPIO_MODE_INPUT;
    config_input.pin_bit_mask = (1ULL << INPUT_PIN);
    config_input.intr_type = GPIO_INTR_DISABLE;
    config_input.pull_down_en = GPIO_PULLDOWN_ENABLE;
    config_input.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&config_input);


    // Loop
    while(1)
    {
        int level = gpio_get_level(INPUT_PIN);
        if(level){
            printf("Encender Led!\n");
            gpio_set_level(LED_PIN, level);
        } else {
            printf("Apagar Led!\n");
            gpio_set_level(LED_PIN, level);
        }
        vTaskDelay(pdMS_TO_TICKS(200));


    }

}