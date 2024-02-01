
#include "bsp.h"

#include <climits>
#include <cstdlib>
#include <cstdio>

#include "gfFont.h"
#include "osAlloc.h"

//https://wiki.libsdl.org/SDL2/FrontPage
//https://gist.github.com/thales17/fb2e4cff60890a51d9dddd4c6e832ad2

tgfTextOverlay       con;

unsigned int random_state = 3242323459;

int bspInit()
{

    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) 
    {
        printf("SDL_Init failed: %s\n", SDL_GetError() );
        return 1;
    }


    osAllocInit();

    osAllocAddNode( 0, malloc( _SYSTEM_MEMORY_SIZE ), _SYSTEM_MEMORY_SIZE, OS_ALLOC_MEMF_CHIP );
    osAllocAddNode( 1, malloc( _SDRAM_MEMORY_SIZE ), _SDRAM_MEMORY_SIZE, OS_ALLOC_MEMF_FAST );

   
    //bsp->videoMuxMode       = 0x02; //text over gfx, 320x240
    
    //connect gfxlib con to hardware text overlay   

    con.type                = GF_TEXT_OVERLAY_TYPE_HARDWARE;
    con.flags               = 0;
    con.width               = 80;               //clear whole buffer
    con.height              = 30;
    con.cursX               = 0;
    con.cursY               = 0;
    con.textAttributes      = 0x0f;
    con.font                = NULL;
    con.textBuffer          = (uchar*) malloc( con.width * con.height * 2 ); //hw text mode buffer address

    toCls( &con );

    con.width               = 40;               //default 40x30 console

    con.textAttributes      = 0x8f;  

    return 0;
} 

int randomNumber()
{
    unsigned int r = random_state;

    r ^= r << 13;
    r ^= r >> 17;
    r ^= r << 5;

    random_state = r;

    return r;
} 

ulong getTicks()
{
   return SDL_GetTicks();
}

void delayMs( unsigned long delay )
{
   return SDL_Delay( delay );
}

int setVideoMode( ulong videoMode )
{
    
    //check textmode: 40 or 80 column and adjust console width
    if( videoMode & _VIDEOMODE_TEXT80_ONLY )
    {
        con.width = 80;
    }
    else
    {
        con.width = 40;     
    }

    return 0;
}

void reboot()
{
   SDL_Quit();
}
