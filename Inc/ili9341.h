/* vim: set ai et ts=4 sw=4: */
#ifndef __ILI9341_H__
#define __ILI9341_H__

#include "fonts.h"
#include "math.h"
#include "stdbool.h"

#define ILI9341_MADCTL_MY 0x80
#define ILI9341_MADCTL_MX 0x40
#define ILI9341_MADCTL_MV 0x20
#define ILI9341_MADCTL_ML 0x10
#define ILI9341_MADCTL_RGB 0x00
#define ILI9341_MADCTL_BGR 0x08
#define ILI9341_MADCTL_MH 0x04

/*** Redefine if necessary ***/
#define ILI9341_SPI_HANDLE hspi1
extern SPI_HandleTypeDef ILI9341_SPI_HANDLE;

#define ILI9341_CS_GPIO_Port GPIOC
#define ILI9341_CS_Pin GPIO_PIN_8

#define ILI9341_DC_GPIO_Port GPIOC
#define ILI9341_DC_Pin GPIO_PIN_9

#define ILI9341_RES_GPIO_Port GPIOC
#define ILI9341_RES_Pin GPIO_PIN_10

// default orientation
// #define ILI9341_WIDTH 240
// #define ILI9341_HEIGHT 320
// #define ILI9341_ROTATION (ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR)

// rotate right
// #define ILI9341_WIDTH  320
// #define ILI9341_HEIGHT 240
// #define ILI9341_ROTATION (ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR)

// rotate left
#define ILI9341_WIDTH 320
#define ILI9341_HEIGHT 240
#define ILI9341_ROTATION (ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR)

// upside down
// #define ILI9341_WIDTH  240
// #define ILI9341_HEIGHT 320
// #define ILI9341_ROTATION (ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR)

/****************************/

// Color definitions
#define ILI9341_BLACK 0x0000
#define ILI9341_BLUE 0x001F
#define ILI9341_RED 0xF800
#define ILI9341_GREEN 0x07E0
#define ILI9341_CYAN 0x07FF
#define ILI9341_MAGENTA 0xF81F
#define ILI9341_YELLOW 0xFFE0
#define ILI9341_WHITE 0xFFFF

#define ILI9341_COLOR565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))

// call before initializing any SPI devices
void ILI9341_Deselect();

void ILI9341_Init(void);
void ILI9341_DrawPixel(int16_t x, int16_t y, uint16_t color);
void ILI9341_WriteString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor);
void ILI9341_FillRectangle(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void ILI9341_FillScreen(uint16_t color);
void ILI9341_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data);
void ILI9341_InvertColors(bool invert);

void ILI9341_DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void ILI9341_DrawLineThick(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, uint16_t thickness, bool cap);
void ILI9341_DrawRectangle(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void ILI9341_DrawRectangleThick(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, uint16_t thickness);
void ILI9341_DrawCircle(int16_t x, int16_t y, uint16_t r, uint16_t color);
void ILI9341_DrawCircleThick(int16_t x0, int16_t y0, uint16_t r, uint16_t color, uint16_t thickness);
void ILI9341_FillCircle(int16_t x, int16_t y, uint16_t r, uint16_t color);
void ILI9341_DrawPolygon(int16_t* x, int16_t* y, uint16_t n, uint16_t color);
void ILI9341_DrawPolygonThick(int16_t* x, int16_t* y, uint16_t n, uint16_t color, uint16_t thickness, bool cap);
void ILI9341_FillPolygon(int16_t* x, int16_t* y, uint16_t n, uint16_t color);

#endif  // __ILI9341_H__
