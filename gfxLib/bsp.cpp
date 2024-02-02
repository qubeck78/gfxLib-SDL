
#include "bsp.h"

#include <climits>
#include <cstdlib>
#include <cstdio>

#include "gfFont.h"
#include "osAlloc.h"

//https://wiki.libsdl.org/SDL2/FrontPage
//https://gist.github.com/thales17/fb2e4cff60890a51d9dddd4c6e832ad2

tgfTextOverlay       con;

static BSP_T bspObj;

BSP_T *bsp = &bspObj;

unsigned int random_state = 3242323459;

int main( void );


int bspInitSDL()
{

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

int threadFunction( void *data )
{
    return main();
}


//windows main
int main( int argv, char** args )
{

    bspInitSDL();

    //start user code
    bsp->threadID = SDL_CreateThread( threadFunction, "gfxLibMainThread", NULL );

    //refresh window, get events
    do
    {

        //simulate 60fps refresh
        SDL_Delay( 1000 / 60 );

        bsp->bspMain();


    }while( !bsp->terminateApp );

    return 0;
}



int bspInit()
{

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
    
    bsp->videoMuxMode = videoMode;

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



//BSP class

BSP_T::BSP_T()
{
    videoMuxMode            = _VIDEOMODE_320_TEXT40_OVER_GFX;
    dmaDisplayPointerStart  = NULL;

    window          = NULL;
    renderer        = NULL;
    texture         = NULL;
    threadID        = NULL;
    terminateApp    = false;

    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) 
    {
        printf( "Error: SDL_Init failed '%s'\n", SDL_GetError() );
        return;
    }

    window = SDL_CreateWindow( "GfxLib sim", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_ALLOW_HIGHDPI );
  
    if( window == NULL ) 
    {
        printf( "Error: Could not create window '%s'\n", SDL_GetError() );
        return;
    }

   
    // create renderer
    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
    
    if( renderer == NULL ) 
    {
        printf( "Error: Unable to create renderer '%s'\n", SDL_GetError() );
        return;
    }

    SDL_RenderSetLogicalSize( renderer, 640, 480 );

    texture = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, 640, 480 );
    if( texture == NULL ) 
    {
        printf( "Error: Unable to create texture '%s'\n", SDL_GetError() );
        return;
    }

}


BSP_T::~BSP_T()
{

    if( texture != NULL )
    {
        SDL_DestroyTexture( texture );
    }

    if( renderer != NULL )
    {
        SDL_DestroyRenderer( renderer );
    }

    if( window != NULL )
    {
        SDL_DestroyWindow( window );
    }

}



ulong BSP_T::bspMain()
{
    ulong        rv;
    SDL_Event    event;
    int          texturePitch;
    void        *texturePixels;
    
    ushort      *frameBuffer;
    ushort      *textureBuffer;
    ushort       pixel;

    ulong        x;
    ulong        y;

    rv              = 0;

    texturePitch    = 0;
    texturePixels   = NULL;

    if( dmaDisplayPointerStart != NULL )
    {

        switch( videoMuxMode )
        {
            case _VIDEOMODE_640_TEXT40_OVER_GFX:

                SDL_LockTexture( texture, NULL, &texturePixels, &texturePitch );
                
                memcpy( texturePixels, (const void*)bsp->dmaDisplayPointerStart, texturePitch * 480 );

                SDL_UnlockTexture( texture );

                break;

            case _VIDEOMODE_320_TEXT40_OVER_GFX:

                SDL_LockTexture( texture, NULL, &texturePixels, &texturePitch );
                

                frameBuffer     = (ushort*)bsp->dmaDisplayPointerStart;
                textureBuffer   = (ushort*)texturePixels;


                for( y = 0; y < 240; y++ )
                {
                    for( x = 0; x < 320; x++ )
                    {
                        pixel = *frameBuffer++;

                        textureBuffer[ ( x * 2 ) + ( y * 2 ) * 640 ]                = pixel;
                        textureBuffer[ ( x * 2 ) + 1 + ( y * 2 ) * 640 ]            = pixel;
                        textureBuffer[ ( x * 2 ) + ( ( y * 2 ) + 1 ) * 640 ]        = pixel;
                        textureBuffer[ ( x * 2 ) + 1 + ( ( y * 2 ) + 1 ) * 640 ]    = pixel;

                    }

                }

                SDL_UnlockTexture( texture );

                break;
        }


        SDL_RenderClear( renderer );
        SDL_RenderCopy( renderer, texture, NULL, NULL );
        SDL_RenderPresent( renderer );

    }



    if( SDL_PollEvent( &event ) ) 
    {
    
        if( event.type == SDL_QUIT ) 
        {
            terminateApp = true;
        }
    }


    return rv;
}