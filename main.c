#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"
#include "pico/time.h"
#include "ws2812.h"

#define SOUND_SENSOR_PIN 26      // Pin del sensor de sonido
#define WS2812_PIN 0             // Pin al que está conectada la matriz de LEDs WS2812B
#define NUM_LEDS 64              // Número total de LEDs en la matriz
#define INTERVAL_MS 500           // Intervalo de tiempo en milisegundos
#define BRIGHTNESS_THRESHOLD 500  // Umbral de brillo para activar LEDs

uint8_t leds[NUM_LEDS * 3];  // Array para almacenar los colores de los LEDs

void timer_callback(struct repeating_timer *t) {
    // Leer el valor analógico del sensor de sonido
    uint16_t sound_value = adc_read();

    // Calcular el color en función del nivel de sonido
    uint8_t brightness = sound_value / 16;  // Ajusta según sea necesario

    // Activar LEDs solo si el nivel de sonido supera el umbral
    if (brightness > BRIGHTNESS_THRESHOLD) {
        for (int i = 0; i < NUM_LEDS; ++i) {
            leds[i * 3] = brightness;    // Componente rojo
            leds[i * 3 + 1] = 0;          // Componente verde
            leds[i * 3 + 2] = 255 - brightness;  // Componente azul
        }
        ws2812_send_blocking(leds, NUM_LEDS);
    }
}

int main() {
    stdio_init_all();

    // Configurar el pin del sensor de sonido como entrada
    gpio_init(SOUND_SENSOR_PIN);
    gpio_set_dir(SOUND_SENSOR_PIN, GPIO_IN);

    // Configurar el pin de salida para la matriz de LEDs WS2812B
    gpio_init(WS2812_PIN);
    gpio_set_dir(WS2812_PIN, GPIO_OUT);

    adc_init();
    adc_select_input(0);

    // Configurar un timer repetitivo para generar interrupciones cada 500 ms
    struct repeating_timer timer;
    add_repeating_timer_ms(INTERVAL_MS, timer_callback, NULL, &timer);

    while (1) {
        tight_loop_contents();
    }

    return 0;
}
