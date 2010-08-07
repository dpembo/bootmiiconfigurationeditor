// ==========================================================
/*
BootMii Configuration Editor
Copyright, (C)2009 - David Pemberton

http://www.pembo.co.uk
bootmiied@dpembo.ukfsn.org

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
// ==========================================================

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




