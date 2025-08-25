#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

#define BTN_PIN_R 28
#define BTN_PIN_G 26
#define LED_PIN_R 4
#define LED_PIN_G 6

volatile int flag_r = 0;
volatile int flag_g = 0;

void btn_callback(uint gpio, uint32_t events) {
  if (events==0x04 && gpio == BTN_PIN_R)flag_r = 1;
  else if (events==0x08 && gpio == BTN_PIN_G) flag_g = 1;
}

int main() {
  stdio_init_all();

  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);
  
  gpio_init(BTN_PIN_G);
  gpio_set_dir(BTN_PIN_G, GPIO_IN);
  gpio_pull_up(BTN_PIN_G);

  gpio_init(LED_PIN_R);
  gpio_set_dir(LED_PIN_R, GPIO_OUT);

  gpio_init(LED_PIN_G);
  gpio_set_dir(LED_PIN_G, GPIO_OUT);

  gpio_set_irq_enabled_with_callback(
    BTN_PIN_R, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);
  gpio_set_irq_enabled(
    BTN_PIN_G, GPIO_IRQ_EDGE_RISE, true);

  int led_r = 0;
  int led_g = 0;

  while (true) {
    if (flag_r) {
      led_r = !led_r;
      gpio_put(LED_PIN_R, led_r);
      flag_r = 0;
    }
    if (flag_g) {
      led_g = !led_g;
      gpio_put(LED_PIN_G, led_g);
      flag_g = 0;
    }
  }
}