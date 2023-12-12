<<<<<<< HEAD
=======

>>>>>>> de1c3e4307715df8e5b98171030afa6e5944b009
#ifndef WS2812_H
#define WS2812_H

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"

extern bool iniciar_programa;
extern uint32_t pixelsb[];
extern uint32_t erroresb[];

extern uint32_t pixelsr[];
extern uint32_t erroresr[];

extern uint32_t pixelsg[];
extern uint32_t erroresg[];

extern int STRING_LEN;

void ws2812_init(PIO pio, int sm, uint offset, int pin_matriz, int longitud_bits);
void ws2812_put_pixel(uint32_t pixel_grb);
uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b);
void ws2812b_core();

#endif // WS2812_H
