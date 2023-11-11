


### goodisplay purchase page
* [https://www.buy-lcd.com/products/-391](https://www.buy-lcd.com/products/-391)

| property | detail |
| --- | --- |
| PART NUMBER | GDMN0129PGWP30 |
| DIAGONAL SIZE | 1.29" |
| DISPLAY MODE | Passive Matrix |
| DISPLAY COLOR | White |
| DRIVE Duty | 1/64 Duty |
| INTERFACE | 8-bit 68XX/80XX Parallel, 4-wire SPI, I2C |
| Driver IC | SSD1315 |
| OUTLINE DIMENSION(mm) | Display: 32.42x21.36x4 |
| Connector board: | 39.8x27.8 |
| ACTIVE AREA(mm) | 29.42x14.7 |
| PIXEL PITCH(mm) | 0.23x0.23 |
| PIXEL SIZE(mm) | 0.21x0.21 |
| BRIGHTNESS(Typ) | 110 cd/m2 |
| VIEW ANGLE | Free |
| OPERATING TEMP. | -40℃~85℃ |
| POWER SUPPLY | 3.3V |
| MAIN FUNCTION | 1. Provide communication interface and  power supply for OLED   2. It is convenient for users to quickly master the use of OLED display. |
| ACCESSIBILITY | 1. OLED display power consumption measurement   2. OLED display working state detection |


### notes 
* `SSD1315` -- the driiver
* still use it like `SSD1306` with 128 x 64
* https://github.com/olikraus/u8g2/wiki/u8g2setupcpp
* constructor:
  - U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI(rotation, cs, dc [, reset]) [full framebuffer, size = 1024 bytes]
    * `U8G2_R0` -- *No rotation, landscape*
    * `U8G2_R1` -- *90 degree clockwise rotation*
    * `U8G2_R2` -- *180 degree clockwise rotation*
    * `U8G2_R3` -- *270 degree clockwise rotation*
    * `U8G2_MIRROR` -- *No rotation, landscape, display content is mirrored (v2.6.x)*