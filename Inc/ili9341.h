/* vim: set ai et ts=4 sw=4: */
#ifndef __ILI9341_H__
#define __ILI9341_H__

#include "ili9341_fonts.h"
#include "math.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"
#include "stm32f7xx_hal.h"

#define ILI9341_MADCTL_MY 0x80
#define ILI9341_MADCTL_MX 0x40
#define ILI9341_MADCTL_MV 0x20
#define ILI9341_MADCTL_ML 0x10
#define ILI9341_MADCTL_RGB 0x00
#define ILI9341_MADCTL_BGR 0x08
#define ILI9341_MADCTL_MH 0x04

// default orientation
#define ILI9341_ROTATION_VERTICAL_1 0

// rotate right
#define ILI9341_ROTATION_HORIZONTAL_1 1

// rotate left
#define ILI9341_ROTATION_HORIZONTAL_2 2

// upside down
#define ILI9341_ROTATION_VERTICAL_2 3

// Color definitions
#define ILI9341_COLOR_BLACK 0x0000
#define ILI9341_COLOR_BLUE 0x001F
#define ILI9341_COLOR_RED 0xF800
#define ILI9341_COLOR_GREEN 0x07E0
#define ILI9341_COLOR_CYAN 0x07FF
#define ILI9341_COLOR_MAGENTA 0xF81F
#define ILI9341_COLOR_YELLOW 0xFFE0
#define ILI9341_COLOR_WHITE 0xFFFF

#define ILI9341_COLOR565(r, g, b) \
    ((((uint16_t)r & 0xF8) << 8) | (((uint16_t)g & 0xFC) << 3) | (((uint16_t)b & 0xF8) >> 3))

// Other constants
#define ILI9341_FILL_RECT_BUFFER_SIZE 512  // x 2 bytes per pixel = 1024 bytes

// Handle type definition
typedef struct {
    SPI_HandleTypeDef* spi_handle;
    GPIO_TypeDef* cs_port;
    uint16_t cs_pin;
    GPIO_TypeDef* dc_port;
    uint16_t dc_pin;
    GPIO_TypeDef* rst_port;
    uint16_t rst_pin;
    uint8_t rotation;
    uint16_t width;
    uint16_t height;
} ILI9341_HandleTypeDef;

// call before initializing any SPI devices
void ILI9341_Deselect(ILI9341_HandleTypeDef* ili9341);

ILI9341_HandleTypeDef ILI9341_Init(
    SPI_HandleTypeDef* spi_handle,
    GPIO_TypeDef* cs_port,
    uint16_t cs_pin,
    GPIO_TypeDef* dc_port,
    uint16_t dc_pin,
    GPIO_TypeDef* rst_port,
    uint16_t rst_pin,
    uint8_t rotation,
    uint16_t width,
    uint16_t height
);
void ILI9341_SetOrientation(ILI9341_HandleTypeDef* ili9341, uint8_t rotation);
void ILI9341_DrawPixel(ILI9341_HandleTypeDef* ili9341, int16_t x, int16_t y, uint16_t color);
void ILI9341_FillRectangle(ILI9341_HandleTypeDef* ili9341, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void ILI9341_FillScreen(ILI9341_HandleTypeDef* ili9341, uint16_t color);
void ILI9341_WriteString(
    ILI9341_HandleTypeDef* ili9341,
    uint16_t x,
    uint16_t y,
    const char* str,
    ILI9341_FontDef font,
    uint16_t color,
    uint16_t bgcolor,
    int16_t tracking
);
void ILI9341_WriteStringScaled(
    ILI9341_HandleTypeDef* ili9341,
    uint16_t x,
    uint16_t y,
    const char* str,
    ILI9341_FontDef font,
    uint16_t color,
    uint16_t bgcolor,
    uint16_t scale,
    int16_t tracking
);
void ILI9341_WriteStringTransparent(
    ILI9341_HandleTypeDef* ili9341,
    uint16_t x,
    uint16_t y,
    const char* str,
    ILI9341_FontDef font,
    uint16_t color,
    int16_t tracking
);
void ILI9341_WriteStringTransparentScaled(
    ILI9341_HandleTypeDef* ili9341,
    uint16_t x,
    uint16_t y,
    const char* str,
    ILI9341_FontDef font,
    uint16_t color,
    uint16_t scale,
    int16_t tracking
);
void ILI9341_DrawImage(
    ILI9341_HandleTypeDef* ili9341,
    uint16_t x,
    uint16_t y,
    uint16_t w,
    uint16_t h,
    const uint16_t* data
);
void ILI9341_InvertColors(ILI9341_HandleTypeDef* ili9341, bool invert);

void ILI9341_DrawLine(ILI9341_HandleTypeDef* ili9341, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void ILI9341_DrawLineThick(
    ILI9341_HandleTypeDef* ili9341,
    int16_t x1,
    int16_t y1,
    int16_t x2,
    int16_t y2,
    uint16_t color,
    uint16_t thickness,
    bool cap
);
void ILI9341_DrawRectangle(ILI9341_HandleTypeDef* ili9341, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void ILI9341_DrawRectangleThick(
    ILI9341_HandleTypeDef* ili9341,
    int16_t x,
    int16_t y,
    int16_t w,
    int16_t h,
    uint16_t color,
    uint16_t thickness
);
void ILI9341_DrawCircle(ILI9341_HandleTypeDef* ili9341, int16_t x, int16_t y, uint16_t r, uint16_t color);
void ILI9341_DrawCircleThick(
    ILI9341_HandleTypeDef* ili9341,
    int16_t x0,
    int16_t y0,
    uint16_t r,
    uint16_t color,
    uint16_t thickness
);
void ILI9341_FillCircle(ILI9341_HandleTypeDef* ili9341, int16_t x, int16_t y, uint16_t r, uint16_t color);
void ILI9341_DrawPolygon(ILI9341_HandleTypeDef* ili9341, int16_t* x, int16_t* y, uint16_t n, uint16_t color);
void ILI9341_DrawPolygonThick(
    ILI9341_HandleTypeDef* ili9341,
    int16_t* x,
    int16_t* y,
    uint16_t n,
    uint16_t color,
    uint16_t thickness,
    bool cap
);
void ILI9341_FillPolygon(ILI9341_HandleTypeDef* ili9341, int16_t* x, int16_t* y, uint16_t n, uint16_t color);

#endif  // __ILI9341_H__
