#include <stdint.h>
#include "stm32f1xx_hal.h"

typedef struct {
	I2C_HandleTypeDef* hi2c1;
	uint16_t device_addr;
} SSD1306;
void ssd1306_init(SSD1306* ssd1306, I2C_HandleTypeDef* hi2c1, uint16_t device_addr);
void ssd1306_send_data(SSD1306* ssd1306, uint8_t data);
void ssd1306_send_cmd(SSD1306* ssd1306, uint8_t cmd);
void ssd1306_draw_pixel(SSD1306* ssd1306, uint8_t x, uint8_t y);
void ssd1306_clear_ram(SSD1306* ssd1306);
