#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>

//Magic macro to check enum size
//#define ctassert(n,e) extern unsigned char n[(e)?0:-1]
#define ctassert(COND,MSG) typedef char static_assertion_##MSG[(COND)?1:-1]
#define _UNUSED   __attribute__ ((unused))
#define NO_INLINE __attribute__ ((noinline))

#define TEMPSTRINGLENGTH 400 //This is the max dialog size (80 characters * 5 lines)
                             //We could reduce this to ~240 on the 128x64 screens
                             //But only after all sprintf are replaced with snprintf
                             //Maybe move this to target_defs.h
extern char tempstring[TEMPSTRINGLENGTH];


typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

extern volatile u8 priority_ready;
void medium_priority_cb();

#include "target.h"
#include "std.h"

/* Misc */
void Delay(u32 count);
u32 Crc(const void *buffer, u32 size);
const char *utf8_to_u32(const char *str, u32 *ch);
int exact_atoi(const char *str); //Like atoi but will not decode a number followed by non-number
size_t strlcpy(char* dst, const char* src, size_t bufsize);
void tempstring_cpy(const char* src);
int fexists(const char *file);
u32 rand32_r(u32 *seed, u8 update); //LFSR based PRNG
u32 rand32(); //LFSR based PRNG
extern volatile u8 priority_ready;
void medium_priority_cb();
void debug_timing(u32 type, int startend); //This is only defined if TIMING_DEBUG is defined
void DEBUGLOG_Putc(char c);

#endif /* _COMMON_H_ */
