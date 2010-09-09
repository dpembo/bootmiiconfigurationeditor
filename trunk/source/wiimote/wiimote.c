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

#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <string.h>
#include <unistd.h>
#include <wiiuse/wpad.h>

extern int rumbletime;
void pressHomeToExit()
{
		while(1)
		{
			WPAD_ScanPads();
			u32 pressed = WPAD_ButtonsDown(0);
			if ( pressed & WPAD_BUTTON_HOME ) exit(0);
		}
}

int getChoiceFromAor1()
{
		while(1)
		{
			WPAD_ScanPads();
			u32 pressed = WPAD_ButtonsDown(0);
			if ( pressed & WPAD_BUTTON_1 ) return 1;
			if ( pressed & WPAD_BUTTON_A ) return 2;
		}
}

int getChoiceFromABor1()
{
		while(1)
		{
			WPAD_ScanPads();
			u32 pressed = WPAD_ButtonsDown(0);
			if ( pressed & WPAD_BUTTON_1 ) return 1;
			if ( pressed & WPAD_BUTTON_A ) return 2;
			if ( pressed & WPAD_BUTTON_B ) return 3;
		}
}

int getChoiceFrom1or2()
{
		while(1)
		{
			WPAD_ScanPads();
			u32 pressed = WPAD_ButtonsDown(0);
			if ( pressed & WPAD_BUTTON_1 ) return 1;
			if ( pressed & WPAD_BUTTON_2 ) return 2;
		}
}

int getChoiceFromA1or2()
{
		while(1)
		{
			WPAD_ScanPads();
			u32 pressed = WPAD_ButtonsDown(0);
			if ( pressed & WPAD_BUTTON_1 ) return 1;
			if ( pressed & WPAD_BUTTON_2 ) return 2;
			if ( pressed & WPAD_BUTTON_A ) return 3;
		}
}

int getChoiceFromAB1or2()
{
		while(1)
		{
			WPAD_ScanPads();
			u32 pressed = WPAD_ButtonsDown(0);
			if ( pressed & WPAD_BUTTON_1 ) return 1;
			if ( pressed & WPAD_BUTTON_2 ) return 2;
			if ( pressed & WPAD_BUTTON_A ) return 3;
			if ( pressed & WPAD_BUTTON_B ) return 4;
		}
}

void press1ToContinue()
{
		int a=0;
		while(a<1)
		{
			WPAD_ScanPads();
			u32 pressed = WPAD_ButtonsDown(0);
			if ( pressed & WPAD_BUTTON_1 ) a++;
		}
}

//_______________________________________________________________________________
/*
* Starts the wiimote rumble
*/
void startRumble()
{
	rumbletime = 1;
	WPAD_Rumble(WPAD_CHAN_0, 1);
}

//_______________________________________________________________________________
/*
* Starts the wiimote rumble after 7 iterations (about 0.5 seconds)
*/
bool checkStopRumble()
{
		if(rumbletime == 7)
		{
			rumbletime = 0;
			WPAD_Rumble(WPAD_CHAN_0, 0);
			return true;
		}
		else
		{
			rumbletime++;
			return false;
		}
}
