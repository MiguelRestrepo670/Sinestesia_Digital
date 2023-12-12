/**
 * @file main.c
 * @brief Programa principal para controlar LEDs RGB WS2812 basado en lecturas del sensor de sonido.
 */

#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/pio.h"
#include "pico/multicore.h"
#include "ws2812.h"

#define SOUND_SENSOR_PIN 26      ///< Pin GPIO conectado al sensor de sonido
#define INTERVAL_MS 2000         ///< Intervalo de tiempo en milisegundos para lecturas del ADC
#define STRING_LEN 64            ///< Longitud de la cadena de LEDs RGB WS2812
const int PIN_MATRIZ = 0;       ///< Pin PIO para controlar la cadena de LEDs RGB WS2812

uint32_t pixelsb[STRING_LEN];   ///< Valores de píxeles azules para LEDs WS2812
uint32_t erroresb[STRING_LEN];   ///< Valores de error para píxeles azules

uint32_t pixelsr[STRING_LEN];   ///< Valores de píxeles rojos para LEDs WS2812
uint32_t erroresr[STRING_LEN];   ///< Valores de error para píxeles rojos

uint32_t pixelsg[STRING_LEN];   ///< Valores de píxeles verdes para LEDs WS2812
uint32_t erroresg[STRING_LEN];   ///< Valores de error para píxeles verdes

bool iniciar_programa = false;    ///< Bandera para controlar si el programa LED está en ejecución o detenido

/**
 * @brief Establece el píxel especificado en la cadena de LEDs WS2812.
 *
 * Esta función envía el valor del píxel a la biblioteca WS2812 para su visualización.
 *
 * @param pixel_grb El valor de color combinado de 24 bits para el píxel en formato GRB.
 */
static inline void put_pixel(uint32_t pixel_grb) {
    ws2812_put_pixel(pixel_grb);
}

/**
 * @brief Función de devolución de llamada del temporizador para lecturas periódicas del ADC y actualizaciones de LED.
 *
 * Esta función lee el valor analógico del sensor de sonido, lo imprime en la consola y actualiza los valores
 * de los LEDs RGB en función de la lectura del sensor si el programa está en ejecución.
 *
 * @param t Puntero a la estructura del temporizador periódico.
 * @return Siempre devuelve true para mantener el temporizador en repetición.
 */
bool timer_callback(struct repeating_timer *t) {
    if (iniciar_programa) {
        uint16_t sound_value = adc_read();
        printf("Valor Analógico: %d\n", sound_value);

        if (sound_value > 0) {
            for (int i = 0; i < STRING_LEN; ++i) {
                pixelsr[i] = sound_value;
                pixelsg[i] = 0;
                pixelsb[i] = 255 - sound_value;
            }
        }
    }

    return true;
}

/**
 * @brief Función principal para controlar LEDs RGB WS2812 basados en lecturas del sensor de sonido.
 *
 * Esta función inicializa el sistema, configura un temporizador periódico para las lecturas del ADC,
 * inicializa la biblioteca WS2812, y lanza un núcleo secundario para el control de los LEDs.
 * El bucle principal procesa los comandos del usuario para iniciar o detener el programa LED.
 *
 * @return Siempre devuelve 0.
 */
int main() {
    stdio_init_all();

    // Inicializar GPIO del sensor de sonido
    gpio_init(SOUND_SENSOR_PIN);
    gpio_set_dir(SOUND_SENSOR_PIN, GPIO_IN);

    // Inicializar ADC
    adc_init();
    adc_select_input(0);

    // Configurar temporizador periódico para lecturas del ADC
    struct repeating_timer timer;
    add_repeating_timer_ms(INTERVAL_MS, timer_callback, NULL, &timer);

    // Inicializar PIO para el control de WS2812
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_init(pio, sm, offset, PIN_MATRIZ, 12);

    // Lanzar un núcleo secundario para el control de LEDs WS2812
    multicore_launch_core1(ws2812b_core);

    // Bucle principal de procesamiento de comandos
    while (1) {
        char command[10];
        scanf("%s", command);

        if (strcmp(command, "iniciar") == 0) {
            iniciar_programa = true;
        } else if (strcmp(command, "parar") == 0) {
            iniciar_programa = false;
        }
    }

    return 0;
}