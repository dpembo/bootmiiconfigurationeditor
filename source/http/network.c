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

#include <errno.h>
#include <malloc.h>
#include <math.h>
#include <network.h>
#include <ogcsys.h>
#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dir.h>
#include <time.h>
#include <wiiuse/wpad.h>
#include <unistd.h>
#include "../console/console.h"
#include "../wiimote/wiimote.h"
#include "network.h"

char myIP[16];

bool networkInit(char *ip)
{
		printf(" Initialising Network ..... ");

        s32 res;
        while ((res = net_init()) == -EAGAIN)
		{
                //printf(".");
				usleep(100 * 1000); //100ms
				//sleep(5000);
		}

        if (if_config(ip, NULL, NULL, true) < 0)
        {
                //WindowPrompt(TX.error, TX.noLocalIP, &okButton, 0);
				//int choice = drawInfo(15,11,11,50,"Cannot get local IP address.","Press '1' to Continue or '2' to Exit");
				//if(choice>1)exit(0);

                usleep(1000 * 1000 * 1); //1 sec
                return false;
        }
		printf("Done (IP Address is [%s])\n",ip);
		usleep(1000 * 1000 * 1); //1 sec
        return true;
}
