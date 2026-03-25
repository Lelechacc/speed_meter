#include "CST816.h"
#include "i2c.h"

#define CST816_I2C_ADDR 0x2A
#define CST816_REG_XPOS_H 0x03

#define LCD_W 240
#define LCD_H 320

static uint8_t buf[4];
static bool is_pressed;
 
void CTP_INT_IRQHandler(void)
{
    is_pressed = true;
}

void CST816_Read(uint16_t *x, uint16_t *y)
{
    HAL_I2C_Mem_Read(&hi2c2, CST816_I2C_ADDR, CST816_REG_XPOS_H, I2C_MEMADD_SIZE_8BIT, buf, 4, 1000);
    *x = (uint16_t)(LCD_W - (((buf[0] & 0x0F) << 8) | buf[1])); 
    *y = (uint16_t)((buf[2] & 0x0F) << 8) | buf[3];
}

bool CST816_IsTouched(void)
{
    if (is_pressed)
    {
        is_pressed = false;
        return true;
    }
    return false;
}
