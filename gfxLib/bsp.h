#ifndef _BSP_H
#define _BSP_H


//Memory regions

//7.5MB
#define _SYSTEM_MEMORY_SIZE 	( 7864320 - 16)
#define _SYSTEM_MEMORY_BASE		0x20000000

//512 KB
#define _FASTRAM_MEMORY_SIZE	524288
#define _FASTRAM_MEMORY_BASE	0x30000000

//31.5 MB
#define _SDRAM_MEMORY_SIZE		0x01F80000
#define _SDRAM_MEMORY_BASE		0x40000000



#define _VIDEOMODE_TEXT40_ONLY              0x00
#define _VIDEOMODE_TEXT80_ONLY              0x04

#define _VIDEOMODE_320_TEXT40_OVER_GFX      0x02
#define _VIDEOMODE_320_TEXT80_OVER_GFX      0x06

#define _VIDEOMODE_640_TEXT40_OVER_GFX      0x12
#define _VIDEOMODE_640_TEXT80_OVER_GFX      0x16

#include "gfTypes.h"
#include <SDL2/SDL.h>

int     bspInit( void );
int     randomNumber( void );
ulong   getTicks( void );
void    delayMs( unsigned long delay );

int     setVideoMode( ulong videoMode );
void    reboot( void );



float inline ffMul( float a, float b )
{
    return a * b;
}

float inline ffAdd( float a, float b )
{
    return a + b;
}

float inline ffSub( float a, float b )
{
    return a - b;
}

float inline ffDiv( float a, float b )
{
    return a / b;
}

#endif
