#include "SSD1306.h"
#include <stdint.h>

void ssd1306_init(SSD1306* ssd1306, I2C_HandleTypeDef* hi2c1, uint16_t device_addr){
	ssd1306->hi2c1 = hi2c1;
	ssd1306->device_addr = device_addr;

	uint8_t init_commands[] = {
	        0xAE,       // Display OFF
	        0x20, 0x00, // Memory mode = horizontal
	        0x21, 0x00, 0x7F, // Column address range (0-127)
	        0x22, 0x00, 0x07, // Page address range (0-7)
	        0x40,       // Display start line
	        0xA0,       // Segment remap
	        0xA8, 0x3F, // Multiplex ratio (64-1)
	        0xC0,       // COM scan direction
	        0xDA, 0x12, // COM pins hardware configuration
	        0x81, 0x7F, // Contrast control
	        0xA4,       // Display follows RAM content
	        0xA6,       // Normal display (not inverted)
	        0xD5, 0x80, // Display clock divide ratio/oscillator frequency
	        0x8D, 0x14, // Enable charge pump
	        0xAF        // Display ON
	    };

	    for (int i = 0; i < sizeof(init_commands); i++) {
	    	ssd1306_send_cmd(ssd1306, init_commands[i]);
	    }
}
void ssd1306_send_cmd(SSD1306* ssd1306 ,uint8_t cmd) {
    HAL_I2C_Mem_Write(ssd1306->hi2c1, ssd1306->device_addr, 0x00, 1, &cmd, 1, -1);
}
// Send data to SSD1306
void ssd1306_send_data(SSD1306* ssd1306, uint8_t data) {
    // uint8_t buffer[2] = {0x40, data}; // Control byte (0x40 for data) + data
    HAL_I2C_Mem_Write(ssd1306->hi2c1, ssd1306->device_addr, 0x40, 1, &data, 1, -1);
}
void ssd1306_draw_pixel(SSD1306* ssd1306, uint8_t x, uint8_t y){
	// if (x >= 128 || y >= 64) return -1;
	    // Set column address
	ssd1306_send_cmd(ssd1306, 0x21);
	ssd1306_send_cmd(ssd1306, x);
	ssd1306_send_cmd(ssd1306, x);

	    // Set page address
	ssd1306_send_cmd(ssd1306, 0x22);
	ssd1306_send_cmd(ssd1306, y / 8);
	ssd1306_send_cmd(ssd1306, y / 8);

	    // Send the pixel data
	uint8_t pixel_bit = 1 << (y % 8);
	ssd1306_send_data(ssd1306, pixel_bit);

}
// Clear entire display RAM (fill with zeros)
void ssd1306_clear_ram(SSD1306* ssd1306) {
    // Set entire display area
	ssd1306_send_cmd(ssd1306, 0x21); // Column address command
	ssd1306_send_cmd(ssd1306, 0x00); // Start column
	ssd1306_send_cmd(ssd1306, 0x7F); // End column (127)

	ssd1306_send_cmd(ssd1306, 0x22); // Page address command
	ssd1306_send_cmd(ssd1306, 0x00); // Start page
	ssd1306_send_cmd(ssd1306, 0x07); // End page (7)

    // Fill entire display with zeros
    for (int page = 0; page < 8; page++) {
        for (int col = 0; col < 128; col++) {
            ssd1306_send_data(ssd1306, 0x00);
        }
    }
}
