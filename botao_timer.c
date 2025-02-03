#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"

#define LED_vermelho 13
#define LED_verde 11
#define LED_azul 12
#define botao_A 5

bool LED_on = true;
bool LED_off = false;
volatile bool LED_ativo = false;
volatile uint estado = 0;

int64_t turn_off_callback(alarm_id_t id, void *user_data);


int main()
{
    stdio_init_all();
    //Inicializando LED's
    gpio_init(LED_vermelho);
    gpio_set_dir(LED_vermelho, GPIO_OUT);
    gpio_init(LED_verde);
    gpio_set_dir(LED_verde, GPIO_OUT);
    gpio_init(LED_azul);
    gpio_set_dir(LED_azul, GPIO_OUT);
    //Inicializando botão A
    gpio_init(botao_A);
    gpio_set_dir(botao_A, GPIO_IN);
    gpio_pull_up(botao_A);



    while (true) {

        if(gpio_get(botao_A) == 0 && !LED_ativo){
        sleep_ms(50); //debounce simple
        
        gpio_put(LED_vermelho, LED_on);
        gpio_put(LED_verde, LED_on);
        gpio_put(LED_azul, LED_on);

        LED_ativo = true;
        estado = 0;

        add_alarm_in_ms(3000, turn_off_callback, NULL, false);

        }

    }
}

int64_t turn_off_callback(alarm_id_t id, void *user_data) {

    switch (estado)
    {
    case 0:
        gpio_put(LED_azul, LED_off);
        estado = 1;
        add_alarm_in_ms(3000, turn_off_callback, NULL, false);
        break;
    case 1:
        gpio_put(LED_vermelho, LED_off);
        add_alarm_in_ms(3000, turn_off_callback, NULL, false);
        estado = 2;
        break;
    case 2:
        gpio_put(LED_verde, LED_off);
        estado = 0;
        LED_ativo = false;
        break;
    default:
        break;
    }

    return 0;
}