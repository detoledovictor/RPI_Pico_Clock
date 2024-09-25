/*
* Arquitetura e Aplicação de sistemas - exercicio 01 - Relógio por interrupção interna
* Aluno: Victor Hugo de Toledo Nunes
* Prof.: Gustavo Ferreira Palma 
*/

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"

volatile bool timer_disparado = false; // Flag que controla se o timer foi disparado ou não
bool led_status = false;
int time_ss = 0, time_mm = 0, time_hh = 0;
int hora = 0, minuto = 0;

const uint LED_PIN = 17; // Pino padrão do LED

void time_updt();

bool sys_tick_timer_updt(struct repeating_timer *t) { // Callback chamada na interrupção do timer
    time_ss++;
    led_status = !led_status;
    time_updt();
    return true;
}

int main() {
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    struct repeating_timer timer;

    while (true) {
        gpio_put(LED_PIN, false);
        printf("Para iniciar - Insira as horas (0-23):\n");
        while (scanf("%d", &hora) != 1 || hora < 0 || hora > 23) {
            printf("Erro ao definir a hora. Tente novamente.\n");
            while (getchar() != '\n'); // Limpa o buffer de entrada
        }

        printf("Para continuar - Insira os minutos (0-59):\n");
        while (scanf("%d", &minuto) != 1 || minuto > 59) {
            printf("Erro ao definir os minutos. Tente novamente.\n");
            while (getchar() != '\n'); // Limpa o buffer de entrada
        }

        time_hh = hora;
        time_mm = minuto;
        time_ss = 0;

        add_repeating_timer_ms(1000, sys_tick_timer_updt, NULL, &timer); // Adiciona o timer com repetição periódica
        
        while (hora != -1) { // Simula a execução contínua
            tight_loop_contents();
        }

        cancel_repeating_timer(&timer); // Desabilita o timer na repetição
    }

    return 0;
}

void time_updt() {
    printf("pico_clock: %02d:%02d:%02d\n", time_hh, time_mm, time_ss);
    
    if (time_ss >= 59) {
        time_mm++;
        time_ss = -1;
    }
    if (time_mm > 59) {
        time_hh++;
        time_mm = 0;
    }
    if (time_hh >= 24) {
        time_hh = 0;
    }

    gpio_put(LED_PIN, led_status);
}
