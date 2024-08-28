#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "driver/ledc.h"
#include "esp_err.h"

// Include del driver del ADC:
#include "driver/adc.h"

constexpr gpio_num_t Sensor1    = GPIO_NUM_36;

extern "C" void app_main();

void app_main() 
{




    // Configuramos el ADC:
    adc1_config_width(ADC_WIDTH_BIT_12); 
    adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_DB_11);
    // Si queremos usar más de un sensor debemos usar más de un canal:
    //adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_DB_11);


    // El raw del acd tiene una resolucion de 12 bits
    uint16_t adcValue   = 0;
    uint8_t cntAdcValue = 0;
    // Loop logica:
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(10));
        /* code */
        adcValue = adc1_get_raw(ADC1_CHANNEL_0);
        
        if(cntAdcValue > 10)
        {
            printf("Valor del ADC: %d\n", adcValue);
            cntAdcValue = 0;
        }
        cntAdcValue++;
    }
    
}