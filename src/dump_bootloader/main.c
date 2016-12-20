/*
 This project is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Deviation is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Deviation.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libopencm3/stm32/usart.h>
#include "common.h"

#define SPI_BOOTLOADER 0

volatile u8 priority_ready;

void dump_bootloader(int uart);

int main() {
    PWR_Init();
    CLOCK_Init();
    priority_ready = 0;
    UART_Initialize();
    char a[] = "Hello!\n\r";
    for (size_t i = 0; i < sizeof(a); ++i) {
        usart_send_blocking(USART1, a[i]);
    }

//    UART_Send("Hello\r\n", 7);

    printf("hello\n");
    if(PWR_CheckPowerSwitch()) PWR_Shutdown();

    BACKLIGHT_Init();
    BACKLIGHT_Brightness(5);

#if SPI_BOOTLOADER
    Initialize_ButtonMatrix();
    SPIFlash_Init(); //This must come before LCD_Init() for 7e
    SPI_FlashBlockWriteEnable(1); //Enable writing to all banks of SPIFlash
    LCD_Init();
    LCD_Clear(0x0000);
    BACKLIGHT_Init();
    BACKLIGHT_Brightness(5);
    LCD_SetFont(0);
    LCD_SetFontColor(0xffff);
    dump_bootloader(0);
#else
    dump_bootloader(1);
#endif
}

void medium_priority_cb()
{
}

u32 SOUND_Callback()
{
    return 0x0fffffff;
}

#if 0
void BACKLIGHT_Init()
{
}

void BACKLIGHT_Brightness(unsigned brightness)
{
    (void) brightness;
}
#endif