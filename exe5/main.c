#include <stdio.h>

#include "hardware/gpio.h"
#include "pico/stdlib.h"

#define BTN_PIN 28

volatile int flag = 0;

void btn_callback(uint gpio, uint32_t events) {
    if (events==0x04) flag = 1;
    else if (events==0x08) flag = 0;
}

int main() {
    stdio_init_all();

    gpio_init(BTN_PIN);
    gpio_set_dir(BTN_PIN, GPIO_IN);
    gpio_pull_up(BTN_PIN);

    while (true) {
        gpio_set_irq_enabled_with_callback(BTN_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);
        if (flag) {
            sleep_ms(800);
            gpio_set_irq_enabled_with_callback(BTN_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);
            if (flag) {
                while (flag) {
                    gpio_set_irq_enabled_with_callback(BTN_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);
                    if (!flag) {
                        break;
                    }
                    sleep_ms(1);
                }
                printf("Aperto longo!\n");
            } else {
                printf("Aperto curto!\n");
            }
        }
    }
}
