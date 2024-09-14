/*

DRIVER MOTORES -----> TB6612FNG

            PINOUT      
GND     ---------->     CABLE A GND
VCC     ---------->     3V3 DEL ESP32
VM      ---------->     2,5 V y 13,5 V

Pines salida a motores:
AO_1    ---------->     PIN DE SALIDO A MOTOR A
AO_2    ---------->     PIN DE SALIDO A MOTOR A
BO_1    ---------->     PIN DE SALIDO A MOTOR B
BO_2    ---------->     PIN DE SALIDO A MOTOR B

Pines para el sentido de los motores (ENTRADA)
AIN_1    ---------->     PIN SENTIDO DEL MOTOR A
AIN_2    ---------->     PIN SENTIDO DEL MOTOR A
BIN_1    ---------->     PIN SENTIDO DEL MOTOR B
BIN_2    ---------->     PIN SENTIDO DEL MOTOR B

PWM MOTORES:
PWM_A   ----------->    PWM DEL MOTOR A
PWM_B   ----------->    PWM DEL MOTOR B


STBY    ----------->    (VCC)PIN DE STAND BY CONTROLADOR DE USO DEL DRIVER

            INPUT                               OUTPUT
    IN1     IN2     PWM     STBY            OUT1        OUT2        MODE
-------------------------------------------------------------
     L       H       H       H               L           H          CCW COUNTERCLOCKWISE (EN CONTRA AGUJAS RELOJ)
     L       H       L       H               L           L          SHORT BRAKE (FRENADO CORTO)
     H       L       H       H               H           L          CW CLOCKWISE (EN SENTIDO AGUJAS RELOJ)
     H       L       L       H               L           L          SHORT BRAKE (FRENADO CORTO)

El funcionamiento es que el STBY debemos tenerlo siempre en HIGH

*/



#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "driver/ledc.h"
#include "esp_err.h"

constexpr gpio_num_t MOTOR1_AIN2    = GPIO_NUM_33;
constexpr gpio_num_t MOTOR1_AIN1    = GPIO_NUM_25;
constexpr gpio_num_t MOTOR1_PWM     = GPIO_NUM_32;

extern "C" void app_main();

void app_main() 
{
// Setup Aquí configuraremos los puertos:
    // Primero configuramos el timer, con la estructura:
    ledc_timer_config_t ledctimer;
    ledctimer.duty_resolution  = LEDC_TIMER_10_BIT;    // Resolucion de 1024 - 1
    ledctimer.freq_hz          = 500;                 // Frecuencia 500Hz 
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
    ledcchannel.gpio_num       = MOTOR1_PWM;            // GPIO_32
    ledcchannel.speed_mode     = LEDC_LOW_SPEED_MODE;   // Lo configuramos como el TIMER
    ledcchannel.timer_sel      = LEDC_TIMER_0;
    ledcchannel.intr_type      = LEDC_INTR_DISABLE;     // Por ahora no queremos interrupciones.
    ledcchannel.hpoint         = 0;
    // Le pasamos la configuracion y si hubiera algun error nos lo diría ESP_ERROR_CHECK
    ESP_ERROR_CHECK(ledc_channel_config(&ledcchannel));


    // Para el puente H necesitamos configurar a parte del MOTOR1_PWM los pines:
    // MOTOR1_AIN1 Y MOTOR1_AIN2:
    // config input pin
    gpio_config_t config_input;
    config_input.mode           = GPIO_MODE_OUTPUT;
    config_input.pin_bit_mask   = ((1ULL << MOTOR1_AIN1) | (1ULL << MOTOR1_AIN2));    // ULL = uint64_t entero sin signo de 64 bits
    config_input.intr_type      = GPIO_INTR_DISABLE;
    config_input.pull_down_en   = GPIO_PULLDOWN_DISABLE;
    config_input.pull_up_en     = GPIO_PULLUP_DISABLE;
    gpio_config(&config_input);

    uint8_t aint_1 = 0;
    uint8_t aint_2 = 1;    

    // Debemos decirle la dirección:
    gpio_set_level(MOTOR1_AIN1, aint_1);
    gpio_set_level(MOTOR1_AIN2, aint_2);


    uint32_t cnt = 0;
    uint32_t dir = 1;
    char dirDelant [] = "hacia delante";
    char dirDetras [] = "hacia detras";
    char* direccion = dirDelant;
    printf("Empezamos a girar hacia delante!!!\n");
    // Loop
    while (1) 
    {
        vTaskDelay(pdMS_TO_TICKS(10));
        cnt += dir;
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, cnt);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        if (cnt > 600 && dir == 1) 
        {
            dir = -1;
            aint_1 = (aint_1==1)?0:1;
            aint_2 = (aint_2==1)?0:1;
            gpio_set_level(MOTOR1_AIN1, aint_1);
            gpio_set_level(MOTOR1_AIN2, aint_2);
            direccion = dirDetras;
            printf("Empezamos a girar hacia DETRAS!!!\n");
        }
        else if( cnt == 50 && dir == -1)
        {
            dir = 1;
            aint_1 = (aint_1==1)?0:1;
            aint_2 = (aint_2==1)?0:1;
            // Cambiamos la dirección
            gpio_set_level(MOTOR1_AIN1, aint_1);
            gpio_set_level(MOTOR1_AIN2, aint_2);
            printf("Empezamos a girar hacia DELANTE!!!\n");
            direccion = dirDelant;
        }
        printf("Valor de cnt: %lu\n", cnt);
        printf("La direccion es %s\n.", direccion);

    }
}