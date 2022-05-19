#pragma once

#include "umHalf.h"

half ParseHalfFloat(uint16_t v);

uint16_t HalfFloatAdd(uint16_t h1, uint16_t h2);

uint16_t HalfFloatSub(uint16_t h1, uint16_t h2);

uint16_t HalfFloatMul(uint16_t h1, uint16_t h2);

uint16_t HalfFloatDiv(uint16_t h1, uint16_t h2);

int HalfFloatCompare(uint16_t h1, uint16_t h2);