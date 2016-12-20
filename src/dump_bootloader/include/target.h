#ifndef _TARGET_H_
#define _TARGET_H_
//#define printf if(0) printf

//Load target-specific include
#include "target_defs.h"

/* Backlight Functions */
void BACKLIGHT_Init();
void BACKLIGHT_Brightness(unsigned brightness);

/* Power functions */
void PWR_Init(void);
unsigned  PWR_ReadVoltage(void);
int  PWR_CheckPowerSwitch();
void PWR_Shutdown();
void PWR_Sleep();

void _usleep(u32 usec);
void _msleep(u32 msec);
#define usleep _usleep

/* Clock functions */
#define LOW_PRIORITY_MSEC 100
#define MEDIUM_PRIORITY_MSEC   5
enum MsecCallback {
    MEDIUM_PRIORITY,
    LOW_PRIORITY,
    LAST_PRIORITY,
};

void CLOCK_Init(void);
u32 CLOCK_getms(void);
void CLOCK_StartTimer(unsigned us, u16 (*cb)(void));
void CLOCK_StopTimer();
void CLOCK_SetMsecCallback(int cb, u32 msec);
void CLOCK_StartWatchdog();
void CLOCK_ResetWatchdog();

/* UART & Debug */
void UART_Initialize();
void UART_Stop();
u8 UART_Send(u8 *data, u16 len);
void UART_SetDataRate(u32 bps);

/* SPI Flash */
void SPIFlash_Init();
u32  SPIFlash_ReadID();
void SPIFlash_EraseSector(u32 sectorAddress);
void SPIFlash_BulkErase();
void SPIFlash_WriteBytes(u32 writeAddress, u32 length, const u8 * buffer);
void SPIFlash_WriteByte(u32 writeAddress, const unsigned byte);
void SPIFlash_ReadBytes(u32 readAddress, u32 length, u8 * buffer);
int  SPIFlash_ReadBytesStopCR(u32 readAddress, u32 length, u8 * buffer);
void SPI_FlashBlockWriteEnable(unsigned enable);

void printstr(char * ptr, int len);

#endif /* _TARGET_H_ */
