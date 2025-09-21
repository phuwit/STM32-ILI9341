/* vim: set ai et ts=4 sw=4: */
#ifndef __ILI9341_TOUCH_H__
#define __ILI9341_TOUCH_H__

#include "stdbool.h"
#include "stdint.h"
#include "stm32f7xx_hal.h"

// Raw touch values range
// to calibrate uncomment UART_Printf line in ili9341_touch.c
#define ILI9341_TOUCH_MIN_RAW_X 1500
#define ILI9341_TOUCH_MAX_RAW_X 31000
#define ILI9341_TOUCH_MIN_RAW_Y 3276
#define ILI9341_TOUCH_MAX_RAW_Y 30110

// Handle type definition
typedef struct {
    SPI_HandleTypeDef* spi_handle;
    GPIO_TypeDef* cs_port;
    uint16_t cs_pin;
    GPIO_TypeDef* irq_port;
    uint16_t irq_pin;
    uint8_t rotation;
    uint16_t width;
    uint16_t height;
} ILI9341_Touch_HandleTypeDef;

// call before initializing any SPI devices
void ILI9341_TouchDeselect(ILI9341_Touch_HandleTypeDef* ili9341_touch);

ILI9341_Touch_HandleTypeDef ILI9341_Touch_Init(
    SPI_HandleTypeDef* spi_handle,
    GPIO_TypeDef* cs_port,
    uint16_t cs_pin,
    GPIO_TypeDef* irq_port,
    uint16_t irq_pin,
    uint8_t rotation,
    uint16_t width,
    uint16_t height
);
bool ILI9341_TouchPressed(ILI9341_Touch_HandleTypeDef* ili9341_touch);
bool ILI9341_TouchGetCoordinates(ILI9341_Touch_HandleTypeDef* ili9341_touch, uint16_t* x, uint16_t* y);

#endif  // __ILI9341_TOUCH_H__
