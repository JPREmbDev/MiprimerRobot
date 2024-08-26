#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "driver/ledc.h"
#include "esp_err.h"

constexpr gpio_num_t LED_PIN = GPIO_NUM_2;
constexpr gpio_num_t INPUT_PIN = GPIO_NUM_32; // push button

extern "C" void app_main();

void app_main() 
{
// Setup Aquí configuraremos los puertos:
    // Primero configuramos el timer, con la estructura:
    ledc_timer_config_t ledctimer;
    ledctimer.duty_resolution  = LEDC_TIMER_10_BIT;    // Resolucion de 1024 - 1
    ledctimer.freq_hz          = 4000;                 // Frecuencia 4kHz
    ledctimer.speed_mode       = LEDC_LOW_SPEED_MODE;  // Envío de los cambios en el timer será en el modo lento
    ledctimer.timer_num        = LEDC_TIMER_0;
    ledctimer.clk_cfg          = LEDC_AUTO_CLK;
    // Le pasamos la configuracion y si hubiera algun error nos lo diría ESP_ERROR_CHECK
    ESP_ERROR_CHECK(ledc_timer_config(&ledctimer));

    // Ahora debemos de configurar el CANAL, será el que lleve la config del TIMER_0 al PIN
    ledc_channel_config_t ledcchannel;
    ledcchannel.channel        = LEDC_CHANNEL_0;
    ledcchannel.duty           = 0;/*!< LEDC channel duty, the range of duty setting is [0, (2**duty_resolution)] */
    //2* LEDC_TIMER_10_BIT(2^1 - 1 = 1023) -> En nuestro caso iremos variando el tiempo que está en algo y que está en bajo
    // Por lo que empezamos en 0 e iremos aumentando hasta el max 1023
    ledcchannel.gpio_num       = LED_PIN;
    ledcchannel.speed_mode     = LEDC_LOW_SPEED_MODE;   // Lo configuramos como el TIMER
    ledcchannel.timer_sel      = LEDC_TIMER_0;
    ledcchannel.intr_type      = LEDC_INTR_DISABLE;     // Por ahora no queremos interrupciones.
    ledcchannel.hpoint         = 0;
    // Le pasamos la configuracion y si hubiera algun error nos lo diría ESP_ERROR_CHECK
    ESP_ERROR_CHECK(ledc_channel_config(&ledcchannel));

    // config input pin
    gpio_config_t config_input;
    config_input.mode = GPIO_MODE_INPUT;
    config_input.pin_bit_mask = (1ULL << INPUT_PIN);
    config_input.intr_type = GPIO_INTR_DISABLE;
    config_input.pull_down_en = GPIO_PULLDOWN_ENABLE;
    config_input.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&config_input);

    int led_state_count = 0;
    bool led_state_direction = true;

    // Loop
    while (1) 
    {
        if (gpio_get_level(INPUT_PIN))
        {
            vTaskDelay(pdMS_TO_TICKS(300));
            printf("Botón presionado.\n");
            
            if (led_state_direction)
            {
                led_state_count += 1024 / 4;
                printf("Aumentamos el brillo del LED.\n");
                printf("led_state_count: %d\n", led_state_count);
            }
            else
            {
                led_state_count -= 1024 / 4;
                printf("Drecementamos el brillo del LED.\n");
                printf("led_state_count: %d\n", led_state_count);
            }

            if (led_state_direction)
            {
                printf("El brillo va aumentando.\n");
                // Se compara si led_state_count es menor a (1 << ledc_timer.duty_resolution) - 1
                // ledc_timer.duty_resolution ->> 10 BITS
                // Si es menor  led_state_direction = True
                // Si es mayor led_state_direction = Falso
                led_state_direction = led_state_count < (1 << ledctimer.duty_resolution) - 1;
            }
            else
            {
                printf("El brillo va disminuyendo.\n");
                led_state_direction = led_state_count <= 0;
            }

            // ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, led_state_count));
            // ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, led_state_count);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}