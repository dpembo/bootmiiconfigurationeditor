#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <string.h>
#include <unistd.h>
#include <ogc/lwp_watchdog.h>

// Slot light patch - from Bool's wiilight)
static vu32 *_wiilight_reg = (u32*)0xCD0000C0;

//____________________________________________________________________________
/**
 * This function calculates the number of frames we render each second.
 * It must be called right after GRRLIB_Render.
 * @return The number of frames per second.
 */
u8 CalculateFrameRate()
{

    static u8 frameCount = 0;
    static u32 lastTime;
    static u8 FPS = 0;
    u32 currentTime = ticks_to_millisecs(gettime());

    frameCount++;
    if(currentTime - lastTime > 1000) {
        lastTime = currentTime;
        FPS = frameCount;
        frameCount = 0;
    }
    return FPS;
}


//____________________________________________________________________________

void WiiLight(int enable)
{
        // Toggle wiilight (thanks Bool for wiilight source)
    u32 val = (*_wiilight_reg&~0x20);
    if(enable) val |= 0x20;
    *_wiilight_reg=val;
}




