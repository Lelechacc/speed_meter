#ifndef CST816_H
#define CST816_H

#include "main.h"
#include <stdbool.h>

void CST816_Init(void);

void CST816_Read(uint16_t* x, uint16_t* y);

bool CST816_IsTouched(void);

#endif
