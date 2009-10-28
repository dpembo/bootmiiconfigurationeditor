/*
 * Simple unzip example
 * This uses Minizip (by Gilles Vollant), a lightweight zlib extension
 * The entire contents of test.zip are extracted into /test/
 * The needed folder structure is created as it goes
 * Tantric, November 2008
 *
 */

#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <fat.h>
#include <zlib.h>

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

#include "unzip/unzip.h"
#include "unzip/miniunz.h"

bool unzipArchive(char * zipfilepath, char * unzipfolderpath)
{
	unzFile uf = unzOpen(zipfilepath);
	if (uf==NULL)
	{
		printf("Cannot open %s, aborting\n",zipfilepath);
		return false;
	}
	printf("%s opened\n",zipfilepath);

	if(chdir(unzipfolderpath)) // can't access dir
	{
		makedir(unzipfolderpath); // attempt to make dir
		if(chdir(unzipfolderpath)) // still can't access dir
		{
			printf("Error changing into %s, aborting\n", unzipfolderpath);
			return false;
		}
	}

	extractZip(uf,0,1,0);

	unzCloseCurrentFile(uf);
	return true;
}

int main(int argc, char **argv)
{
	VIDEO_Init();
	WPAD_Init();
	rmode = VIDEO_GetPreferredMode(NULL);
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(xfb);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE)
		VIDEO_WaitVSync();
	fatInit (8, false);

	printf("\n\n\n");

	char zipfile[512] = "fat:/test.zip";
	char unzipdir[512] = "fat:/test";
	unzipArchive(zipfile, unzipdir);

	printf("Done! Press HOME to return to the HBC.\n");
	while(1)
	{
		WPAD_ScanPads();
		u32 pressed = WPAD_ButtonsDown(0);

		if (pressed & WPAD_BUTTON_HOME)
			exit(0);
	}
}
