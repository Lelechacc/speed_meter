#ifndef POWER_H
#define POWER_H

#include <stdbool.h>

void Power_Init();

bool Power_IsCharging();

float Power_GetBatteryVoltage();

#endif