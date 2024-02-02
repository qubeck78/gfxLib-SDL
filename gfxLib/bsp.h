#ifndef _BSP_H
#define _BSP_H


//Memory regions

//7.5MB
#define _SYSTEM_MEMORY_SIZE 	( 7864320 - 16)
#define _SYSTEM_MEMORY_BASE		0x0

//512 KB
#define _FASTRAM_MEMORY_SIZE	524288
#define _FASTRAM_MEMORY_BASE	0x0

//31.5 MB
#define _SDRAM_MEMORY_SIZE		0x01F80000
#define _SDRAM_MEMORY_BASE		0x0



#define _VIDEOMODE_TEXT40_ONLY              0x00
#define _VIDEOMODE_TEXT80_ONLY              0x04

#define _VIDEOMODE_320_TEXT40_OVER_GFX      0x02
#define _VIDEOMODE_320_TEXT80_OVER_GFX      0x06

#define _VIDEOMODE_640_TEXT40_OVER_GFX      0x12
#define _VIDEOMODE_640_TEXT80_OVER_GFX      0x16

#include "gfTypes.h"
#include <SDL2/SDL.h>



class BSP_T
{
    private:

        SDL_Window      *window;
        SDL_Renderer    *renderer;
        SDL_Texture     *texture;

 
    public:

    //
        volatile unsigned long  videoMuxMode;
        volatile void*          dmaDisplayPointerStart;

    //b0 - vsync (positive)
    //volatile unsigned long videoVSync;
    
    //pointer of gfx data to display (within DMA address space, address divided by 4)
    //volatile unsigned long  dmaDisplayPointerStart;
        
    //wr b7, b6, b5, b4 - LEDS, b0 - spi0SSel
    //volatile unsigned long gpoPort;
    
    //wr b0 - tickTimerReset
    //volatile unsigned long tickTimerConfig;
    
    //rd - tickTimerValue
    //volatile unsigned long tickTimerValue;
    
    //counts frames (up), write resets timer
    //volatile unsigned long  frameTimer;
    
        BSP_T();
        ~BSP_T();

        ulong bspMain();
    
};


extern BSP_T*   bsp;

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
