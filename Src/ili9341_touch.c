/* vim: set ai et ts=4 sw=4: */

#include "ili9341_touch.h"

#include "stm32f7xx_hal.h"

#define READ_X 0xD0
#define READ_Y 0x90

static void ILI9341_TouchSelect(ILI9341_Touch_HandleTypeDef* ili9341_touch) {
    HAL_GPIO_WritePin(ili9341_touch->cs_port, ili9341_touch->cs_pin, GPIO_PIN_RESET);
}

void ILI9341_TouchDeselect(ILI9341_Touch_HandleTypeDef* ili9341_touch) {
    HAL_GPIO_WritePin(ili9341_touch->cs_port, ili9341_touch->cs_pin, GPIO_PIN_SET);
}

ILI9341_Touch_HandleTypeDef ILI9341_Touch_Init(
    SPI_HandleTypeDef* spi_handle,
    GPIO_TypeDef* cs_port,
    uint16_t cs_pin,
    GPIO_TypeDef* irq_port,
    uint16_t irq_pin,
    uint8_t rotation,
    uint16_t width,
    uint16_t height
) {
    ILI9341_Touch_HandleTypeDef ili9341_touch_instance = {
        .spi_handle = spi_handle,
        .cs_port = cs_port,
        .cs_pin = cs_pin,
        .irq_port = irq_port,
        .irq_pin = irq_pin,
        .rotation = rotation,
        .width = width,
        .height = height
    };

    ILI9341_TouchDeselect(&ili9341_touch_instance);

    return ili9341_touch_instance;
}

bool ILI9341_TouchPressed(ILI9341_Touch_HandleTypeDef* ili9341_touch) {
    return HAL_GPIO_ReadPin(ili9341_touch->irq_port, ili9341_touch->irq_pin) == GPIO_PIN_RESET;
}

bool ILI9341_TouchGetCoordinates(ILI9341_Touch_HandleTypeDef* ili9341_touch, uint16_t* x, uint16_t* y) {
    static const uint8_t cmd_read_x[] = {READ_X};
    static const uint8_t cmd_read_y[] = {READ_Y};
    static const uint8_t zeroes_tx[] = {0x00, 0x00};

    ILI9341_TouchSelect(ili9341_touch);

    uint32_t avg_x = 0;
    uint32_t avg_y = 0;
    uint8_t nsamples = 0;
    for (uint8_t i = 0; i < 16; i++) {
        if (!ILI9341_TouchPressed(ili9341_touch)) break;

        nsamples++;

        HAL_SPI_Transmit(ili9341_touch->spi_handle, (uint8_t*)cmd_read_y, sizeof(cmd_read_y), HAL_MAX_DELAY);
        uint8_t y_raw[2];
        HAL_SPI_TransmitReceive(ili9341_touch->spi_handle, (uint8_t*)zeroes_tx, y_raw, sizeof(y_raw), HAL_MAX_DELAY);

        HAL_SPI_Transmit(ili9341_touch->spi_handle, (uint8_t*)cmd_read_x, sizeof(cmd_read_x), HAL_MAX_DELAY);
        uint8_t x_raw[2];
        HAL_SPI_TransmitReceive(ili9341_touch->spi_handle, (uint8_t*)zeroes_tx, x_raw, sizeof(x_raw), HAL_MAX_DELAY);

        avg_x += (((uint16_t)x_raw[0]) << 8) | ((uint16_t)x_raw[1]);
        avg_y += (((uint16_t)y_raw[0]) << 8) | ((uint16_t)y_raw[1]);
    }

    ILI9341_TouchDeselect(ili9341_touch);

    if (nsamples < 16) return false;

    uint32_t raw_x = (avg_x / 16);
    if (raw_x < ILI9341_TOUCH_MIN_RAW_X) raw_x = ILI9341_TOUCH_MIN_RAW_X;
    if (raw_x > ILI9341_TOUCH_MAX_RAW_X) raw_x = ILI9341_TOUCH_MAX_RAW_X;

    uint32_t raw_y = (avg_y / 16);
    if (raw_y < ILI9341_TOUCH_MIN_RAW_Y) raw_y = ILI9341_TOUCH_MIN_RAW_Y;
    if (raw_y > ILI9341_TOUCH_MAX_RAW_Y) raw_y = ILI9341_TOUCH_MAX_RAW_Y;

    // Uncomment this line to calibrate touchscreen:
    // UART_Printf("raw_x = %d, raw_y = %d\r\n", x, y);

    *x = (raw_x - ILI9341_TOUCH_MIN_RAW_X) * ili9341_touch->width / (ILI9341_TOUCH_MAX_RAW_X - ILI9341_TOUCH_MIN_RAW_X);
    *y =
        (raw_y - ILI9341_TOUCH_MIN_RAW_Y) * ili9341_touch->height / (ILI9341_TOUCH_MAX_RAW_Y - ILI9341_TOUCH_MIN_RAW_Y);

    return true;
}
