
#include "ws2812.h"
#include "pico/stdlib.h"

/**
 * @brief Longitud de la cadena de LEDs RGB.
 *
 * Esta variable define la longitud de la cadena de LEDs RGB y se utiliza
 * en diferentes partes del programa.
 */
int STRING_LEN = 64;

static int longitud_bit;

/**
 * @brief Inicializa el controlador WS2812.
 *
 * Esta función configura el controlador WS2812 con los parámetros dados.
 *
 * @param pio         Puntero al controlador PIO.
 * @param sm          Número del estado de la máquina de PIO.
 * @param offset      Desplazamiento del programa de la máquina de estado.
 * @param pin_matriz  Número del pin al que está conectada la matriz RGB.
 * @param _longitud_bit  Profundidad de bits del color.
 */
void ws2812_init(PIO pio, int sm, uint offset, int pin_matriz, int _longitud_bit) {
    longitud_bit = _longitud_bit;
    ws2812_program_init(pio, sm, offset, pin_matriz, 800000, false);
}

/**
 * @brief Envía un píxel al controlador WS2812.
 *
 * Esta función coloca un píxel en la cadena de LEDs RGB utilizando el
 * controlador WS2812.
 *
 * @param pixel_grb   Valor del píxel en formato GRB.
 */
void ws2812_put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

/**
 * @brief Convierte valores RGB de 8 bits a un entero de 32 bits.
 *
 * Esta función convierte valores RGB de 8 bits para formar un entero de
 * 32 bits en el formato esperado por el controlador WS2812.
 *
 * @param r   Componente rojo.
 * @param g   Componente verde.
 * @param b   Componente azul.
 *
 * @return    Valor de 32 bits que representa el color.
 */
uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(r) << 8) |
           ((uint32_t)(g) << 16) |
           (uint32_t)(b);
}

/**
 * @brief Función principal de control de la cadena de LEDs RGB.
 *
 * Esta función se ejecuta en un bucle infinito y controla el comportamiento
 * de la cadena de LEDs RGB en función de la variable de estado `iniciar_programa`.
 */
/**
 * @brief Función principal de control de la cadena de LEDs RGB.
 *
 * Esta función se ejecuta en un bucle infinito y controla el comportamiento
 * de la cadena de LEDs RGB en función de la variable de estado `iniciar_programa`.
 *
 * Si el programa está en ejecución, actualiza los valores de los píxeles en
 * la cadena de LEDs RGB con base en los valores actuales y los errores acumulados.
 * Luego, ajusta los errores acumulados para el siguiente ciclo.
 *
 * Si el programa no está en ejecución, apaga todos los LEDs en la cadena.
 * Se espera un tiempo pruedente corto para la escritura de cada LED.
 */
void ws2812b_core() {
    int valorr, valorg, valorb;
    int shift = longitud_bit - 8;

    while (1) {
        if (iniciar_programa) {
            for (int i = 0; i < STRING_LEN; i++) {
                // Calcular valores de píxeles ajustados y enviar a la cadena de LEDs RGB
                valorb = (pixelsb[i] + erroresb[i]) >> shift;
                valorr = (pixelsr[i] + erroresr[i]) >> shift;
                valorg = (pixelsg[i] + erroresg[i]) >> shift;

                ws2812_put_pixel(urgb_u32(valorr, valorg, valorb));

                // Ajustar errores acumulados para el siguiente ciclo
                erroresb[i] = (pixelsb[i] + erroresb[i]) - (valorb << shift);
                erroresr[i] = (pixelsr[i] + erroresr[i]) - (valorr << shift);
                erroresg[i] = (pixelsg[i] + erroresg[i]) - (valorg << shift);
            }
            sleep_us(400);
        } else {
            // Apagar todos los LEDs cuando el programa no está en ejecución
            for (int i = 0; i < STRING_LEN; ++i) {
                ws2812_put_pixel(urgb_u32(0, 0, 0));
            }
            sleep_us(400);
        }
    }
}

