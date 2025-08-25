#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <stdint.h>

#define BTN_PIN_R 28

volatile int btn_flag = 0;

static void btn_callback(uint gpio, uint32_t events) {
    if (gpio == BTN_PIN_R && (events & GPIO_IRQ_EDGE_FALL)) btn_flag = 1;
}

int main(void) {
    stdio_init_all();
    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_set_irq_enabled_with_callback(BTN_PIN_R,GPIO_IRQ_EDGE_FALL,true,&btn_callback);

    int capture_flag = 0;

    while (true) {
        if (btn_flag) {
            btn_flag = 0;
            printf("btn pressed \n");

            while (!gpio_get(BTN_PIN_R)) {
                sleep_ms(1);
            }
            sleep_ms(1);
            printf("btn released \n");

            capture_flag = 1;
        }

        if (capture_flag) capture_flag = 0;

        tight_loop_contents();
    }
}
