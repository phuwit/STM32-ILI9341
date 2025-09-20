/* vim: set ai et ts=4 sw=4: */
#include "ili9341.h"

#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_gpio.h"
#include "stm32f7xx_hal_spi.h"

static void ILI9341_Select() {
    HAL_GPIO_WritePin(ILI9341_CS_GPIO_Port, ILI9341_CS_Pin, GPIO_PIN_RESET);
}

void ILI9341_Deselect() {
    HAL_GPIO_WritePin(ILI9341_CS_GPIO_Port, ILI9341_CS_Pin, GPIO_PIN_SET);
}

static void ILI9341_Reset() {
    HAL_GPIO_WritePin(ILI9341_RES_GPIO_Port, ILI9341_RES_Pin, GPIO_PIN_RESET);
    HAL_Delay(5);
    HAL_GPIO_WritePin(ILI9341_RES_GPIO_Port, ILI9341_RES_Pin, GPIO_PIN_SET);
}

static void ILI9341_WriteCommand(uint8_t cmd) {
    HAL_GPIO_WritePin(ILI9341_DC_GPIO_Port, ILI9341_DC_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&ILI9341_SPI_PORT, &cmd, sizeof(cmd), HAL_MAX_DELAY);
}

static void ILI9341_WriteData(uint8_t* buff, size_t buff_size) {
    HAL_GPIO_WritePin(ILI9341_DC_GPIO_Port, ILI9341_DC_Pin, GPIO_PIN_SET);

    // split data in small chunks because HAL can't send more then 64K at once
    while (buff_size > 0) {
        uint16_t chunk_size = buff_size > 32768 ? 32768 : buff_size;
        HAL_SPI_Transmit(&ILI9341_SPI_PORT, buff, chunk_size, HAL_MAX_DELAY);
        buff += chunk_size;
        buff_size -= chunk_size;
    }
}

static void ILI9341_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    // column address set
    ILI9341_WriteCommand(0x2A);  // CASET
    {
        uint8_t data[] = {(x0 >> 8) & 0xFF, x0 & 0xFF, (x1 >> 8) & 0xFF, x1 & 0xFF};
        ILI9341_WriteData(data, sizeof(data));
    }

    // row address set
    ILI9341_WriteCommand(0x2B);  // RASET
    {
        uint8_t data[] = {(y0 >> 8) & 0xFF, y0 & 0xFF, (y1 >> 8) & 0xFF, y1 & 0xFF};
        ILI9341_WriteData(data, sizeof(data));
    }

    // write to RAM
    ILI9341_WriteCommand(0x2C);  // RAMWR
}

void ILI9341_Init() {
    ILI9341_Select();
    ILI9341_Reset();

    // command list is based on https://github.com/martnak/STM32-ILI9341

    // SOFTWARE RESET
    ILI9341_WriteCommand(0x01);
    HAL_Delay(1000);

    // POWER CONTROL A
    ILI9341_WriteCommand(0xCB);
    {
        uint8_t data[] = {0x39, 0x2C, 0x00, 0x34, 0x02};
        ILI9341_WriteData(data, sizeof(data));
    }

    // POWER CONTROL B
    ILI9341_WriteCommand(0xCF);
    {
        uint8_t data[] = {0x00, 0xC1, 0x30};
        ILI9341_WriteData(data, sizeof(data));
    }

    // DRIVER TIMING CONTROL A
    ILI9341_WriteCommand(0xE8);
    {
        uint8_t data[] = {0x85, 0x00, 0x78};
        ILI9341_WriteData(data, sizeof(data));
    }

    // DRIVER TIMING CONTROL B
    ILI9341_WriteCommand(0xEA);
    {
        uint8_t data[] = {0x00, 0x00};
        ILI9341_WriteData(data, sizeof(data));
    }

    // POWER ON SEQUENCE CONTROL
    ILI9341_WriteCommand(0xED);
    {
        uint8_t data[] = {0x64, 0x03, 0x12, 0x81};
        ILI9341_WriteData(data, sizeof(data));
    }

    // PUMP RATIO CONTROL
    ILI9341_WriteCommand(0xF7);
    {
        uint8_t data[] = {0x20};
        ILI9341_WriteData(data, sizeof(data));
    }

    // POWER CONTROL,VRH[5:0]
    ILI9341_WriteCommand(0xC0);
    {
        uint8_t data[] = {0x23};
        ILI9341_WriteData(data, sizeof(data));
    }

    // POWER CONTROL,SAP[2:0];BT[3:0]
    ILI9341_WriteCommand(0xC1);
    {
        uint8_t data[] = {0x10};
        ILI9341_WriteData(data, sizeof(data));
    }

    // VCM CONTROL
    ILI9341_WriteCommand(0xC5);
    {
        uint8_t data[] = {0x3E, 0x28};
        ILI9341_WriteData(data, sizeof(data));
    }

    // VCM CONTROL 2
    ILI9341_WriteCommand(0xC7);
    {
        uint8_t data[] = {0x86};
        ILI9341_WriteData(data, sizeof(data));
    }

    // MEMORY ACCESS CONTROL
    ILI9341_WriteCommand(0x36);
    {
        uint8_t data[] = {0x48};
        ILI9341_WriteData(data, sizeof(data));
    }

    // PIXEL FORMAT
    ILI9341_WriteCommand(0x3A);
    {
        uint8_t data[] = {0x55};
        ILI9341_WriteData(data, sizeof(data));
    }

    // FRAME RATIO CONTROL, STANDARD RGB COLOR
    ILI9341_WriteCommand(0xB1);
    {
        uint8_t data[] = {0x00, 0x18};
        ILI9341_WriteData(data, sizeof(data));
    }

    // DISPLAY FUNCTION CONTROL
    ILI9341_WriteCommand(0xB6);
    {
        uint8_t data[] = {0x08, 0x82, 0x27};
        ILI9341_WriteData(data, sizeof(data));
    }

    // 3GAMMA FUNCTION DISABLE
    ILI9341_WriteCommand(0xF2);
    {
        uint8_t data[] = {0x00};
        ILI9341_WriteData(data, sizeof(data));
    }

    // GAMMA CURVE SELECTED
    ILI9341_WriteCommand(0x26);
    {
        uint8_t data[] = {0x01};
        ILI9341_WriteData(data, sizeof(data));
    }

    // POSITIVE GAMMA CORRECTION
    ILI9341_WriteCommand(0xE0);
    {
        uint8_t data[] = {0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1,
                          0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00};
        ILI9341_WriteData(data, sizeof(data));
    }

    // NEGATIVE GAMMA CORRECTION
    ILI9341_WriteCommand(0xE1);
    {
        uint8_t data[] = {0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1,
                          0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F};
        ILI9341_WriteData(data, sizeof(data));
    }

    // EXIT SLEEP
    ILI9341_WriteCommand(0x11);
    HAL_Delay(120);

    // TURN ON DISPLAY
    ILI9341_WriteCommand(0x29);

    // MADCTL
    ILI9341_WriteCommand(0x36);
    {
        uint8_t data[] = {ILI9341_ROTATION};
        ILI9341_WriteData(data, sizeof(data));
    }

    ILI9341_Deselect();
}

static void ILI9341_DrawPixelFast(int16_t x, int16_t y, uint16_t color) {
    if (x < 0 || y < 0 || x >= ILI9341_WIDTH || y >= ILI9341_HEIGHT)
        return;

    ILI9341_SetAddressWindow(x, y, x + 1, y + 1);
    uint8_t data[] = {color >> 8, color & 0xFF};
    ILI9341_WriteData(data, sizeof(data));
}

void ILI9341_DrawPixel(int16_t x, int16_t y, uint16_t color) {
    ILI9341_Select();
    ILI9341_DrawPixelFast(x, y, color);
    ILI9341_Deselect();
}

static void ILI9341_WriteChar(uint16_t x, uint16_t y, char ch, ILI9341_FontDef font, uint16_t color, uint16_t bgcolor) {
    if (ch < 32 || ch > 127)
        return;

    uint32_t index = (ch - 32) * font.intsPerGlyph;
    uint32_t mask = 0x80000000;

    ILI9341_SetAddressWindow(x, y, x + font.width - 1, y + font.height - 1);

    for (uint32_t i = 0; i < font.width * font.height; i++) {
        if (font.data[index] & mask) {
            uint8_t data[] = {color >> 8, color & 0xFF};
            ILI9341_WriteData(data, sizeof(data));
        } else {
            uint8_t data[] = {bgcolor >> 8, bgcolor & 0xFF};
            ILI9341_WriteData(data, sizeof(data));
        }
        mask >>= 1;
        if (mask == 0) {
            index++;
            mask = 0x80000000
        }
    }
}

void ILI9341_WriteString(uint16_t x, uint16_t y, const char* str, ILI9341_FontDef font, uint16_t color, uint16_t bgcolor) {
    if (y + font.height - 1 > ILI9341_HEIGHT)
        return;

    ILI9341_Select();

    while (*str && x + font.width - 1 < ILI9341_WIDTH) {
        ILI9341_WriteChar(x, y, *str, font, color, bgcolor);
        x += font.width;
        str++;
    }

    ILI9341_Deselect();
}

static void ILI9341_WriteCharTransparent(uint16_t x, uint16_t y, char ch, ILI9341_FontDef font, uint16_t color) {
    if (ch < 32 || ch > 127)
        return;

    uint32_t index = (ch - 32) * font.intsPerGlyph;
    uint32_t mask = 0x80000000;

    for (uint32_t row = 0; row < font.height; row++) {
        for (uint32_t col = 0; col < font.width; col++) {
            if (font.data[index] & mask) {
                ILI9341_DrawPixelFast(x + col, y + row, color);
            }
            mask >>= 1;
            if (mask == 0) {
                index++;
                mask = 0x80000000
            }
        }
    }
}

void ILI9341_WriteStringTransparent(uint16_t x, uint16_t y, const char* str, ILI9341_FontDef font, uint16_t color) {
    if (y + font.height - 1 > ILI9341_HEIGHT)
        return;

    ILI9341_Select();

    while (*str && x + font.width - 1 < ILI9341_WIDTH) {
        ILI9341_WriteCharTransparent(x, y, *str, font, color);
        x += font.width;
        str++;
    }

    ILI9341_Deselect();
}

static void ILI9341_FillRectangleFast(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if (w < 0) {
        w = -w;
        x -= w - 1;
    }

    if (h < 0) {
        h = -h;
        y -= h - 1;
    }

    if (x >= ILI9341_WIDTH || y >= ILI9341_HEIGHT || w == 0 || h == 0) return;

    if (x < 0) {
        w += x;
        x = 0;
    }

    if (y < 0) {
        h += y;
        y = 0;
    }

    if ((x + w - 1) >= ILI9341_WIDTH) w = ILI9341_WIDTH - x;
    if ((y + h - 1) >= ILI9341_HEIGHT) h = ILI9341_HEIGHT - y;

    ILI9341_SetAddressWindow(x, y, x + w - 1, y + h - 1);

    uint8_t data[] = {color >> 8, color & 0xFF};
    HAL_GPIO_WritePin(ILI9341_DC_GPIO_Port, ILI9341_DC_Pin, GPIO_PIN_SET);
    for (uint16_t row = 0; row < h; row++) {
        for (uint16_t col = 0; col < w; col++) {
            HAL_SPI_Transmit(&ILI9341_SPI_PORT, data, sizeof(data), HAL_MAX_DELAY);
        }
    }
}

void ILI9341_FillRectangle(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    ILI9341_Select();
    ILI9341_FillRectangleFast(x, y, w, h, color);
    ILI9341_Deselect();
}

void ILI9341_FillScreen(uint16_t color) {
    ILI9341_Select();
    ILI9341_FillRectangleFast(0, 0, ILI9341_WIDTH, ILI9341_HEIGHT, color);
    ILI9341_Deselect();
}

void ILI9341_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data) {
    if (x >= ILI9341_WIDTH || y >= ILI9341_HEIGHT || (x + w - 1) >= ILI9341_WIDTH || (y + h - 1) >= ILI9341_HEIGHT) return;

    ILI9341_Select();
    ILI9341_SetAddressWindow(x, y, x + w - 1, y + h - 1);
    ILI9341_WriteData((uint8_t*)data, sizeof(uint16_t) * w * h);
    ILI9341_Deselect();
}

void ILI9341_InvertColors(bool invert) {
    ILI9341_Select();
    ILI9341_WriteCommand(invert ? 0x21 /* INVON */ : 0x20 /* INVOFF */);
    ILI9341_Deselect();
}

static void ILI9341_DrawLineFast(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
    if (x1 == x2) {
        ILI9341_FillRectangleFast(x1, y1, 1, y2 - y1 + 1, color);
        return;
    } else if (y1 == y2) {
        ILI9341_FillRectangleFast(x1, y1, x2 - x1 + 1, 1, color);
        return;
    }

    int16_t dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int16_t dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int16_t err = dx + dy, e2; /* error value e_xy */

    while (true) {
        ILI9341_DrawPixelFast(x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { /* e_xy+e_x > 0 */
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx) { /* e_xy+e_y < 0 */
            err += dx;
            y1 += sy;
        }
    }
}

void ILI9341_DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
    ILI9341_Select();
    ILI9341_DrawLineFast(x1, y1, x2, y2, color);
    ILI9341_Deselect();
}

void ILI9341_DrawLineThick(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, uint16_t thickness, bool cap) {
    if (thickness == 0) return;
    if (thickness == 1) {
        ILI9341_DrawLine(x1, y1, x2, y2, color);
        return;
    }

    if (x1 > x2) {
        int16_t tmp = x1;
        x1 = x2;
        x2 = tmp;
    }

    if (y1 > y2) {
        int16_t tmp = y1;
        y1 = y2;
        y2 = tmp;
    }

    if (x1 >= ILI9341_WIDTH || y1 >= ILI9341_HEIGHT || x2 < 0 || y2 < 0)
        return;

    // calculate line vector
    int16_t dx = x2 - x1;
    int16_t dy = y2 - y1;
    float length = sqrtf(dx * dx + dy * dy);
    if (length == 0) return;  // zero-length line

    // unit vector
    float ux = dx / length;
    float uy = dy / length;

    // perpendicular vector
    float px = -uy;
    float py = ux;

    // half thickness
    float half_thickness = thickness / 2.0f;

    // calculate corner points
    int16_t corners_x[4] = {
        (int16_t)(x1 + px * half_thickness),
        (int16_t)(x1 - px * half_thickness),
        (int16_t)(x2 - px * half_thickness),
        (int16_t)(x2 + px * half_thickness)};
    int16_t corners_y[4] = {
        (int16_t)(y1 + py * half_thickness),
        (int16_t)(y1 - py * half_thickness),
        (int16_t)(y2 - py * half_thickness),
        (int16_t)(y2 + py * half_thickness)};

    ILI9341_FillPolygon(corners_x, corners_y, 4, color);

    if (cap) {
        ILI9341_FillCircle(x1, y1, half_thickness, color);
        ILI9341_FillCircle(x2, y2, half_thickness, color);
    }
}

void ILI9341_DrawRectangle(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    ILI9341_DrawRectangleThick(x, y, w, h, color, 1);
}

void ILI9341_DrawRectangleThick(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, uint16_t thickness) {
    if (thickness == 0) return;

    ILI9341_Select();

    ILI9341_FillRectangleFast(x, y, w, thickness, color);                                              // top
    ILI9341_FillRectangleFast(x, y + h - thickness, w, thickness, color);                              // bottom
    ILI9341_FillRectangleFast(x, y + thickness, thickness, h - 2 * thickness, color);                  // left
    ILI9341_FillRectangleFast(x + w - thickness, y + thickness, thickness, h - 2 * thickness, color);  // right

    ILI9341_Deselect();
}

void ILI9341_DrawCircle(int16_t xc, int16_t yc, uint16_t r, uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    ILI9341_Select();

    ILI9341_DrawPixelFast(xc, yc + r, color);
    ILI9341_DrawPixelFast(xc, yc - r, color);
    ILI9341_DrawPixelFast(xc + r, yc, color);
    ILI9341_DrawPixelFast(xc - r, yc, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        ILI9341_DrawPixelFast(xc + x, yc + y, color);
        ILI9341_DrawPixelFast(xc - x, yc + y, color);
        ILI9341_DrawPixelFast(xc + x, yc - y, color);
        ILI9341_DrawPixelFast(xc - x, yc - y, color);
        ILI9341_DrawPixelFast(xc + y, yc + x, color);
        ILI9341_DrawPixelFast(xc - y, yc + x, color);
        ILI9341_DrawPixelFast(xc + y, yc - x, color);
        ILI9341_DrawPixelFast(xc - y, yc - x, color);
    }

    ILI9341_Deselect();
}

void ILI9341_DrawCircleThick(int16_t xc, int16_t yc, uint16_t r, uint16_t color, uint16_t thickness) {
    if (thickness == 0 || thickness > r) return;

    int16_t ri = r - thickness;
    int16_t xo = r;
    int16_t xi = ri;
    int16_t y = 0;
    int16_t erro = 1 - xo;
    int16_t erri = 1 - xi;

    ILI9341_Select();

    while (xo >= y) {
        ILI9341_DrawLineFast(xc + xi, xc + xo, yc + y, yc + y, color);
        ILI9341_DrawLineFast(xc + y, xc + y, yc + xi, yc + xo, color);
        ILI9341_DrawLineFast(xc - xo, xc - xi, yc + y, yc + y, color);
        ILI9341_DrawLineFast(xc - y, xc - y, yc + xi, yc + xo, color);
        ILI9341_DrawLineFast(xc - xo, xc - xi, yc - y, yc - y, color);
        ILI9341_DrawLineFast(xc - y, xc - y, yc - xo, yc - xi, color);
        ILI9341_DrawLineFast(xc + xi, xc + xo, yc - y, yc - y, color);
        ILI9341_DrawLineFast(xc + y, xc + y, yc - xo, yc - xi, color);

        y++;

        if (erro < 0) {
            erro += 2 * y + 1;
        } else {
            xo--;
            erro += 2 * (y - xo + 1);
        }

        if (y > ri) {
            xi = y;
        } else {
            if (erri < 0) {
                erri += 2 * y + 1;
            } else {
                xi--;
                erri += 2 * (y - xi + 1);
            }
        }
    }

    ILI9341_Deselect();
}

void ILI9341_FillCircle(int16_t xc, int16_t yc, uint16_t r, uint16_t color) {
    int16_t x = 0;

    ILI9341_Select();

    for (int16_t y = 0; y <= r; y++) {
        while (x * x + y * y <= r * r) {
            x++;
        }
        x--;
        ILI9341_DrawLineFast(xc - x, yc + y, xc + x, yc + y, color);
        ILI9341_DrawLineFast(xc - x, yc - y, xc + x, yc - y, color);
    }

    ILI9341_Deselect();
}

void ILI9341_DrawPolygon(int16_t* x, int16_t* y, uint16_t n, uint16_t color) {
    if (n < 2) return;

    ILI9341_Select();

    for (uint16_t i = 0; i < n - 1; i++) {
        ILI9341_DrawLineFast(x[i], y[i], x[i + 1], y[i + 1], color);
    }
    ILI9341_DrawLineFast(x[n - 1], y[n - 1], x[0], y[0], color);

    ILI9341_Deselect();
}

void ILI9341_DrawPolygonThick(int16_t* x, int16_t* y, uint16_t n, uint16_t color, uint16_t thickness, bool cap) {
    if (n < 2 || thickness == 0) return;

    for (uint16_t i = 0; i < n - 1; i++) {
        ILI9341_DrawLineThick(x[i], y[i], x[i + 1], y[i + 1], color, thickness, cap);
    }
    ILI9341_DrawLineThick(x[n - 1], y[n - 1], x[0], y[0], color, thickness, cap);
}

void ILI9341_FillPolygon(int16_t* x, int16_t* y, uint16_t n, uint16_t color) {
    if (n < 3) return;

    // find max and min Y
    int16_t minY = y[0], maxY = y[0];
    for (uint16_t i = 1; i < n; i++) {
        if (y[i] < minY) minY = y[i];
        if (y[i] > maxY) maxY = y[i];
    }

    if (minY >= ILI9341_HEIGHT) return;
    if (maxY < 0) return;
    if (minY < 0) minY = 0;
    if (maxY >= ILI9341_HEIGHT) maxY = ILI9341_HEIGHT - 1;

    int16_t nodeX[32];  // max 32 intersections

    ILI9341_Select();

    // scanline algorithm
    for (uint16_t j = minY; j <= maxY; j++) {
        uint16_t nodes = 0;
        uint16_t k = n - 1;

        // find intersections
        for (uint16_t i = 0; i < n; i++) {
            if ((y[i] < j && y[k] >= j) || (y[k] < j && y[i] >= j)) {
                int16_t dy = y[k] - y[i];
                if (dy != 0 && nodes < 32) {
                    nodeX[nodes++] = x[i] + ((int32_t)(j - y[i]) * (x[k] - x[i])) / dy;
                }
            }
            k = i;
        }

        // insertion sort nodeX array
        for (uint16_t i = 1; i < nodes; i++) {
            int16_t key = nodeX[i];
            int16_t j_sort = i - 1;
            while (j_sort >= 0 && nodeX[j_sort] > key) {
                nodeX[j_sort + 1] = nodeX[j_sort];
                j_sort--;
            }
            nodeX[j_sort + 1] = key;
        }

        // fill the pixels between node pairs
        for (uint16_t i = 0; i < nodes - 1; i += 2) {
            int16_t x1 = nodeX[i];
            int16_t x2 = nodeX[i + 1];

            if (x1 >= ILI9341_WIDTH) break;
            if (x2 >= ILI9341_WIDTH) x2 = ILI9341_WIDTH - 1;
            if (x1 < ILI9341_WIDTH && x2 > 0 && x2 >= x1) {
                ILI9341_FillRectangleFast(x1, j, x2 - x1 + 1, 1, color);
            }
        }
    }

    ILI9341_Deselect();
}
