
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "i2c.h"
#include "lcd_i2c.h"
#include "uart.h"
#include <stdio.h>

void lcd_sent();

void Sound_play(float frequency,float duration);