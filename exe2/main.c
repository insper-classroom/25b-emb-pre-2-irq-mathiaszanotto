#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

#define BTN_PIN 28
#define LED_PIN 4

volatile int flag = 0;

void btn_callback(uint gpio, uint32_t events) {
  if (events==0x4) {
    if (flag == 1) flag = 0;
    else flag = 1;
  }
}

int main() {
  stdio_init_all();

  gpio_init(BTN_PIN);
  gpio_set_dir(BTN_PIN, GPIO_IN);
  gpio_pull_up(BTN_PIN);

  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);

  int led = 0;

  while (true) {
    gpio_set_irq_enabled_with_callback(
      BTN_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);
      if (flag) {
        led = !led;
        gpio_put(LED_PIN, led);
        flag = !flag;
      }
  }
}
