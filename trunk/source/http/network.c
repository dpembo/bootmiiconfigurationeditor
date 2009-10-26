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
