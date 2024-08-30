/*

MULTIPLEXOR -----> CD74HC4067

            PINOUT      
GND     ---------->     CABLE A GND
VCC     ---------->     3V3 DEL ESP32
EN      ---------->     CABLE A GND -> ES NEGADO    
S0-S3   ---------->     PIN DE SALIDA
SIG     ---------->     ADC_1


    TABLA DE VERDAD:
S0  S1  S2  S3          !E(NEGADO)   SELECTEDCHANNEL
1   0   1   0 ---------->   0               5
0   1   1   0 ---------->   0               6
1   1   1   0 ---------->   0               7
0   0   0   1 ---------->   0               8
1   0   0   1 ---------->   0               9
0   1   0   1 ---------->   0               10
1   1   0   1 ---------->   0               11
0   0   1   1 ---------->   0               12

*/

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "driver/ledc.h"
#include "esp_err.h"

// Include del driver del ADC:
#include "driver/adc.h"

constexpr gpio_num_t S3_PIN    = GPIO_NUM_26;
constexpr gpio_num_t S2_PIN    = GPIO_NUM_25;
constexpr gpio_num_t S1_PIN    = GPIO_NUM_32;
constexpr gpio_num_t S0_PIN    = GPIO_NUM_33;

extern "C" void app_main();

void app_main() 
{
    // Setup Configuración:
    // Vamos a configurar los pines de salida del Multiplexor
    // S0_PIN, S1_PIN, S2_PIN y S3_PIN:
    // config input pin
    gpio_config_t config_input;
    config_input.mode           = GPIO_MODE_OUTPUT;
    config_input.pin_bit_mask   = ((1ULL << S0_PIN) | (1ULL << S1_PIN)| (1ULL << S2_PIN)| (1ULL << S3_PIN));    // ULL = uint64_t entero sin signo de 64 bits
    config_input.intr_type      = GPIO_INTR_DISABLE;
    config_input.pull_down_en   = GPIO_PULLDOWN_DISABLE;
    config_input.pull_up_en     = GPIO_PULLUP_DISABLE;
    gpio_config(&config_input);

    // Configuramos el ADC:
    adc1_config_width(ADC_WIDTH_BIT_12); 
    adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_DB_11);
    // Si queremos usar más de un sensor debemos usar más de un canal:
    //adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_DB_11);


    // El raw del acd tiene una resolucion de 12 bits
    uint16_t adcValue   = 0;
    
    //uint8_t cntAdcValue = 0;
    // Loop logica:
    while (1)
    {
        /*
            S3_PIN    = GPIO_NUM_34;
            S2_PIN    = GPIO_NUM_35;
            S1_PIN    = GPIO_NUM_32;
            S0_PIN    = GPIO_NUM_33;
        */

        // Seleccionamos el pin a leer poniendo en la salida de los pines 34,35,32,33
        // La tabla de la vedad del chip Multiplexor:

        // 1   0   1   0 ---------->   C_5
        gpio_set_level(S0_PIN,1);
        gpio_set_level(S1_PIN,0);
        gpio_set_level(S2_PIN,1);
        gpio_set_level(S3_PIN,0);

        // Tomamos el valor del adc de ese canal:
        adcValue = adc1_get_raw(ADC1_CHANNEL_0);

        // Mostramos el valor del ADC
        printf("El valor del Canal 5:%d\n", adcValue);
        vTaskDelay(pdMS_TO_TICKS(10));

        // 0   1   1   0 ---------->   C_6
        gpio_set_level(S0_PIN,0);
        gpio_set_level(S1_PIN,1);
        gpio_set_level(S2_PIN,1);
        gpio_set_level(S3_PIN,0);

        // Tomamos el valor del adc de ese canal:
        adcValue = adc1_get_raw(ADC1_CHANNEL_0);

        printf("El valor del Canal 6:%d\n", adcValue);
        vTaskDelay(pdMS_TO_TICKS(100));


        // 1   1   1   0 ---------->   C_7
        gpio_set_level(S0_PIN,1);
        gpio_set_level(S1_PIN,1);
        gpio_set_level(S2_PIN,1);
        gpio_set_level(S3_PIN,0);

        // Tomamos el valor del adc de ese canal:
        adcValue = adc1_get_raw(ADC1_CHANNEL_0);

        printf("El valor del Canal 7:%d\n", adcValue);
        vTaskDelay(pdMS_TO_TICKS(100));

        // 0   0   0   1 ---------->   C_8
        gpio_set_level(S0_PIN,0);
        gpio_set_level(S1_PIN,0);
        gpio_set_level(S2_PIN,0);
        gpio_set_level(S3_PIN,1);

        // Tomamos el valor del adc de ese canal:
        adcValue = adc1_get_raw(ADC1_CHANNEL_0);

        printf("El valor del Canal 8:%d\n", adcValue);
        vTaskDelay(pdMS_TO_TICKS(100));


        // 1   0   0   1 ---------->   C_9
        gpio_set_level(S0_PIN,1);
        gpio_set_level(S1_PIN,0);
        gpio_set_level(S2_PIN,0);
        gpio_set_level(S3_PIN,1);

        // Tomamos el valor del adc de ese canal:
        adcValue = adc1_get_raw(ADC1_CHANNEL_0);

        printf("El valor del Canal 9:%d\n", adcValue);
        vTaskDelay(pdMS_TO_TICKS(100));

        // 0   1   0   1 ---------->   C_10
        gpio_set_level(S0_PIN,0);
        gpio_set_level(S1_PIN,1);
        gpio_set_level(S2_PIN,0);
        gpio_set_level(S3_PIN,1);

        // Tomamos el valor del adc de ese canal:
        adcValue = adc1_get_raw(ADC1_CHANNEL_0);

        printf("El valor del Canal 10:%d\n", adcValue);
        vTaskDelay(pdMS_TO_TICKS(100));

        // 1   1   0   1 ---------->   C_11
        gpio_set_level(S0_PIN,1);
        gpio_set_level(S1_PIN,1);
        gpio_set_level(S2_PIN,0);
        gpio_set_level(S3_PIN,1);

        // Tomamos el valor del adc de ese canal:
        adcValue = adc1_get_raw(ADC1_CHANNEL_0);

        printf("El valor del Canal 11:%d\n", adcValue);
        vTaskDelay(pdMS_TO_TICKS(100));

        // 0   0   1   1 ---------->   C_12
        gpio_set_level(S0_PIN,0);
        gpio_set_level(S1_PIN,0);
        gpio_set_level(S2_PIN,1);
        gpio_set_level(S3_PIN,1);

        // Tomamos el valor del adc de ese canal:
        adcValue = adc1_get_raw(ADC1_CHANNEL_0);

        printf("El valor del Canal 12:%d\n", adcValue);
        vTaskDelay(pdMS_TO_TICKS(100));



        /* code */
        /*
        vTaskDelay(pdMS_TO_TICKS(10));
        adcValue = adc1_get_raw(ADC1_CHANNEL_0);
        
        if(cntAdcValue > 10)
        {
            printf("Valor del ADC: %d\n", adcValue);
            cntAdcValue = 0;
        }
        cntAdcValue++;
        */
       
    }
    
}


