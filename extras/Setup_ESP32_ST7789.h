// TFT_eSPI sample setup for a common 2.8-inch ST7789 SPI TFT.
// Copy into TFT_eSPI/User_Setups/ and select it in User_Setup_Select.h.
// Confirm the display controller and pinout before use.

#define ST7789_DRIVER
#define TFT_WIDTH  240
#define TFT_HEIGHT 320

#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS    5
#define TFT_DC    2
#define TFT_RST  15

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF
#define SMOOTH_FONT

#define SPI_FREQUENCY       27000000
#define SPI_READ_FREQUENCY  16000000
