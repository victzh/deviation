#ifndef _DEVOF4_TARGET_H_
#define _DEVOF4_TARGET_H_

#define SPIFLASH_SECTOR_OFFSET 0

#define TXID 0xF4
#define VECTOR_TABLE_LOCATION 0x3000 //0x3000

#ifdef BUILDTYPE_DEV
   #define DEBUG_WINDOW_SIZE 0 //200
#else
   #define DEBUG_WINDOW_SIZE 0
#endif


#include "ports.h"

#define INP_HAS_CALIBRATION 4

/* Compute voltage from y = 2.1592x + 0.2493 */
#define VOLTAGE_NUMERATOR 324
#define VOLTAGE_OFFSET    382

#endif //_DEVOF4_TARGET_H_

