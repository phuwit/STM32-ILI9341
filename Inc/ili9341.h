/* vim: set ai et ts=4 sw=4: */
#ifndef __ILI9341_H__
#define __ILI9341_H__

#include <stdint.h>
#include "ili9341_fonts.h"
#include "math.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"
#include "stm32f7xx_hal.h"

// optionally enable dma by uncommenting the line below
// #define ILI9341_ENABLE_DMA

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

/**
 * @brief ILI9341 handle structure
 */
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

/**
 * @brief Deselect the ILI9341 display, call before using other SPI peripherals on the same bus
 * @param ili9341 Pointer to ILI9341 handle structure
 */
void ILI9341_Deselect(ILI9341_HandleTypeDef* ili9341);

/**
 * @brief Initialize the ILI9341 display
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param spi_handle Pointer to the SPI handle
 * @param cs_port GPIO port for Chip Select pin
 * @param cs_pin GPIO pin for Chip Select
 * @param dc_port GPIO port for Data/Command pin
 * @param dc_pin GPIO pin for Data/Command
 * @param rst_port GPIO port for Reset pin
 * @param rst_pin GPIO pin for Reset
 * @param rotation Initial display rotation, one of ILI9341_ROTATION_* values
 * @param width Display width in pixels
 * @param height Display height in pixels
 * @return Initialized ILI9341_HandleTypeDef structure
 */
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

/**
 * @brief Set display orientation
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param rotation New display rotation, one of ILI9341_ROTATION_* values
 */
void ILI9341_SetOrientation(ILI9341_HandleTypeDef* ili9341, uint8_t rotation, uint8_t scrollBit);

/**
 * @brief Draw a single pixel at specified coordinates
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param x X coordinate of the pixel
 * @param y Y coordinate of the pixel
 * @param color 16-bit color of the pixel in RGB565 format
 */
void ILI9341_DrawPixel(ILI9341_HandleTypeDef* ili9341, int16_t x, int16_t y, uint16_t color);

/**
 * @brief Fill a rectangle with specified color
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param x X coordinate of the top-left corner of the rectangle
 * @param y Y coordinate of the top-left corner of the rectangle
 * @param w Width of the rectangle in pixels
 * @param h Height of the rectangle in pixels
 * @param color 16-bit fill color in RGB565 format
 */
void ILI9341_FillRectangle(ILI9341_HandleTypeDef* ili9341, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

/**
 * @brief Fill the entire screen with specified color
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param color 16-bit fill color in RGB565 format
 */
void ILI9341_FillScreen(ILI9341_HandleTypeDef* ili9341, uint16_t color);

/**
 * @brief Write a string to the display with specified font and colors
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param x X coordinate of the top-left corner of the string
 * @param y Y coordinate of the top-left corner of the string
 * @param str Null-terminated string to write
 * @param font Font definition to use for rendering the string
 * @param color 16-bit text color in RGB565 format
 * @param bgcolor 16-bit background color in RGB565 format
 * @param tracking Additional space in pixels between characters, can be negative
 */
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

/**
 * @brief Write a scaled string to the display with specified font and colors
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param x X coordinate of the top-left corner of the string
 * @param y Y coordinate of the top-left corner of the string
 * @param str Null-terminated string to write
 * @param font Font definition to use for rendering the string
 * @param color 16-bit text color in RGB565 format
 * @param bgcolor 16-bit background color in RGB565 format
 * @param scale Scaling factor for the font, must be >= 1
 * @param tracking Additional space in pixels between characters, can be negative
 * @note This function does not support clipping, any characters that would go beyond the display width are not drawn.
 */
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

/**
 * @brief Write a string to the display with specified font and color, background is not drawn (transparent)
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param x X coordinate of the top-left corner of the string
 * @param y Y coordinate of the top-left corner of the string
 * @param str Null-terminated string to write
 * @param font Font definition to use for rendering the string
 * @param color 16-bit text color in RGB565 format
 * @param tracking Additional space in pixels between characters, can be negative
 */
void ILI9341_WriteStringTransparent(
    ILI9341_HandleTypeDef* ili9341,
    uint16_t x,
    uint16_t y,
    const char* str,
    ILI9341_FontDef font,
    uint16_t color,
    int16_t tracking
);

/**
 * @brief Write a scaled string to the display with specified font and color, background is not drawn (transparent)
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param x X coordinate of the top-left corner of the string
 * @param y Y coordinate of the top-left corner of the string
 * @param str Null-terminated string to write
 * @param font Font definition to use for rendering the string
 * @param color 16-bit text color in RGB565 format
 * @param scale Scaling factor for the font, must be >= 1
 * @param tracking Additional space in pixels between characters, can be negative
 * @note This function does not support clipping, any characters that would go beyond the display width are not drawn.
 */
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

/**
 * @brief Draw an image (bitmap) at specified coordinates
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param x X coordinate of the top-left corner of the image
 * @param y Y coordinate of the top-left corner of the image
 * @param w Width of the image in pixels
 * @param h Height of the image in pixels
 * @param data Pointer to the image pixel data in RGB565 format with the 2 bytes swapped, must contain at least w*h
 * elements
 */
void ILI9341_DrawImage(
    ILI9341_HandleTypeDef* ili9341,
    uint16_t x,
    uint16_t y,
    uint16_t w,
    uint16_t h,
    const uint16_t* data
);

/**
 * @brief Draw an image (bitmap) at specified coordinates, clip out of bounds pixels
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param x X coordinate of the top-left corner of the image
 * @param y Y coordinate of the top-left corner of the image
 * @param w Width of the image in pixels
 * @param h Height of the image in pixels
 * @param data Pointer to the image pixel data in RGB565 format with the 2 bytes swapped, must contain at least w*h
 * elements
 */
void ILI9341_DrawImageWithClip(
    ILI9341_HandleTypeDef* ili9341,
    int16_t x,
    int16_t y,
    uint16_t w,
    uint16_t h,
    const uint16_t* data
);

/**
 * @brief Invert the display colors
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param invert true to invert colors, false for normal colors
 */
void ILI9341_InvertColors(ILI9341_HandleTypeDef* ili9341, bool invert);

/**
 * @brief Draw a thin line between two points
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param x1 X coordinate of the start point
 * @param y1 Y coordinate of the start point
 * @param x2 X coordinate of the end point
 * @param y2 Y coordinate of the end point
 * @param color 16-bit line color in RGB565 format
 */
void ILI9341_DrawLine(ILI9341_HandleTypeDef* ili9341, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

/**
 * @brief Draw a thick line between two points
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param x1 X coordinate of the start point
 * @param y1 Y coordinate of the start point
 * @param x2 X coordinate of the end point
 * @param y2 Y coordinate of the end point
 * @param color 16-bit line color in RGB565 format
 * @param thickness Line thickness in pixels, must be >= 1
 * @param cap true to draw rounded line caps, false for no caps
 */
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

/**
 * @brief Draw a thin rectangle outline
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param x X coordinate of the top-left corner of the rectangle
 * @param y Y coordinate of the top-left corner of the rectangle
 * @param w Width of the rectangle in pixels
 * @param h Height of the rectangle in pixels
 * @param color 16-bit rectangle color in RGB565 format
 */
void ILI9341_DrawRectangle(ILI9341_HandleTypeDef* ili9341, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

/**
 * @brief Draw a thick rectangle outline
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param x X coordinate of the top-left corner of the rectangle
 * @param y Y coordinate of the top-left corner of the rectangle
 * @param w Width of the rectangle in pixels
 * @param h Height of the rectangle in pixels
 * @param color 16-bit rectangle color in RGB565 format
 * @param thickness Line thickness in pixels, must be >= 1
 */
void ILI9341_DrawRectangleThick(
    ILI9341_HandleTypeDef* ili9341,
    int16_t x,
    int16_t y,
    int16_t w,
    int16_t h,
    uint16_t color,
    uint16_t thickness
);

/**
 * @brief Draw a thin circle outline
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param x X coordinate of the center of the circle
 * @param y Y coordinate of the center of the circle
 * @param r Radius of the circle
 * @param color 16-bit circle color in RGB565 format
 */
void ILI9341_DrawCircle(ILI9341_HandleTypeDef* ili9341, int16_t x, int16_t y, uint16_t r, uint16_t color);

/**
 * @brief Draw a thick circle outline
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param x0 X coordinate of the center of the circle
 * @param y0 Y coordinate of the center of the circle
 * @param r Radius of the circle
 * @param color 16-bit circle color in RGB565 format
 * @param thickness Circle line thickness in pixels, must be >= 1 and <= r
 */
void ILI9341_DrawCircleThick(
    ILI9341_HandleTypeDef* ili9341,
    int16_t x0,
    int16_t y0,
    uint16_t r,
    uint16_t color,
    uint16_t thickness
);

/**
 * @brief Fill a circle
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param x X coordinate of the center of the circle
 * @param y Y coordinate of the center of the circle
 * @param r Radius of the circle
 * @param color 16-bit circle color in RGB565 format
 */
void ILI9341_FillCircle(ILI9341_HandleTypeDef* ili9341, int16_t x, int16_t y, uint16_t r, uint16_t color);

/**
 * @brief Draw a polygon outline
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param x Array of X coordinates of the polygon vertices
 * @param y Array of Y coordinates of the polygon vertices
 * @param n Number of vertices in the polygon
 * @param color 16-bit polygon color in RGB565 format
 * @note The polygon is automatically closed by connecting the last vertex to the first.
 */
void ILI9341_DrawPolygon(ILI9341_HandleTypeDef* ili9341, int16_t* x, int16_t* y, uint16_t n, uint16_t color);

/**
 * @brief Draw a thick polygon outline
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param x Array of X coordinates of the polygon vertices
 * @param y Array of Y coordinates of the polygon vertices
 * @param n Number of vertices in the polygon
 * @param color 16-bit polygon color in RGB565 format
 * @param thickness Line thickness in pixels, must be >= 1
 * @param cap true to draw rounded line caps, false for no caps
 * @note The polygon is automatically closed by connecting the last vertex to the first.
 */
void ILI9341_DrawPolygonThick(
    ILI9341_HandleTypeDef* ili9341,
    int16_t* x,
    int16_t* y,
    uint16_t n,
    uint16_t color,
    uint16_t thickness,
    bool cap
);

/**
 * @brief Fill a polygon
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param x Array of X coordinates of the polygon vertices
 * @param y Array of Y coordinates of the polygon vertices
 * @param n Number of vertices in the polygon
 * @param color 16-bit polygon color in RGB565 format
 * @note The algorithm used is scanline algorithm, with support for concave and self-intersecting polygons. Max
 * intersections for scanline is 32.
 */
void ILI9341_FillPolygon(ILI9341_HandleTypeDef* ili9341, int16_t* x, int16_t* y, uint16_t n, uint16_t color);

void ILI9341_DefineVerticalScrollArea(ILI9341_HandleTypeDef* ili9341, uint16_t topFixedLines, uint16_t bottomFixedLines);

void ILI9341_DoVerticalScroll(ILI9341_HandleTypeDef* ili9341, uint16_t lines);

void ILI9341_SetModeNormal(ILI9341_HandleTypeDef* ili9341);

#endif  // __ILI9341_H__
