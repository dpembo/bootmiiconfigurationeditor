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
