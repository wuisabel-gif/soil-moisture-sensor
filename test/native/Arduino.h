#pragma once

#include <stdint.h>

#define INPUT 0

void pinMode(uint8_t pin, uint8_t mode);
int analogRead(uint8_t pin);
void delay(unsigned long milliseconds);
