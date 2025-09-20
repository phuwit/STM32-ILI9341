# STM32-ILI9341

STM32 HAL-based library for ILI9341 TFT modules with touchscreen.

This library is a fork of [afiskon/stm32-ili9341][u0] which is based on [afiskon/stm32-st7735][u1] and
[afiskon/stm32-ssd1306][u2]. Initialization sequence for ILI9341 was borrowed
from [martnak/STM32-ILI9341][u3].

See also:

- https://github.com/afiskon/stm32-i2c-lcd-1602
- https://github.com/afiskon/stm32-ssd1306
- https://github.com/afiskon/stm32-ssd1351
- https://github.com/afiskon/stm32-st7735

[u0]: https://github.com/afiskon/stm32-ili9341
[u1]: https://github.com/afiskon/stm32-st7735
[u2]: https://github.com/afiskon/stm32-ssd1306
[u3]: https://github.com/martnak/STM32-ILI9341

## Additional features

This fork add 4 functions to the original library from [afiskon/stm32-ili9341][u0], they're the following:

1. `ILI9341_DrawLine`: draw a line from x1, y1 to x2, y2 using Bresenham's line algorithm
2. `ILI9341_DrawRectangle`: draw a hollow rectangle at x, y with w, h widht and height
3. `ILI9341_DrawCircle`: draw a hollow circle using Bresenham’s circle algorithm
4. `ILI9341_FillCircle`: draw a filled circle using generic x^2 + y^x <= r^2 algorithm

## Configuration

To specify hardware SPI handler and GPIO pins, edit the definitions in [Inc/ili9341.h](./Inc/ili9341.h) and [Inc/ili9341_touch.h](./Inc/ili9341_touch.h). The SPI speed can be up to 50 Mbps for the LCD, and 1.3 Mbps for the touch screen. All GPIOs used should be set to very high (maximum) output frequency.
