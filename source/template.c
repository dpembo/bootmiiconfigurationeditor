// ==========================================================
/*
BootMii Configuration Editor
Copyright, (C)2010 - David Pemberton

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
#include <ogc/pad.h>

#include <fat.h>
#include <dirent.h>
#include <sys/dir.h>
#include <zlib.h>


#include "console/console.h"
#include "template.h"
#include "wiimote/wiimote.h"
#include "http/network.h"
#include "http/http.h"
#include "i18n/i18n.h"
#include "langvars.h"
#include "skinvars.h"
#include "version.h"
#include "stringutils/stringutils.h"

#include "unzip/unzip.h"
#include "unzip/miniunz.h"
#include "file/file.h"


//_____________________________________________________________________________

//Booleans - True or False
#define FALSE 0
#define TRUE 1

//_____________________________________________________________________________

#define LEFT 0
#define RIGHT 1

//_____________________________________________________________________________

//Console Colors
#define BLACK 0
#define DARK_RED 1
#define DARK_GREEN 2
#define DARK_YELLOW 3
#define DARK_BLUE 4
#define DARK_MAGENTA 5
#define DARK_CYAN 6
#define GRAY 7
#define DARK_GRAY 10
#define RED 11
#define GREEN 12
#define YELLOW 13
#define BLUE 14
#define MAGENTA 15
#define CYAN 16
#define WHITE 17

//_____________________________________________________________________________

#define MAINMENU 0
#define SETTINGS 1

//_____________________________________________________________________________

int LOGOX = 0;
int LOGOY = 450;
int MAIN_WINY = 9;

//_____________________________________________________________________________

/*
VERSION NUMBERS

Take off 48 to get real version number, e.g.
50,48 = v2.0
50,49 = v2.1
50,53 = v2.5
50,57 = v2.9
51,48 = v3.0

*/

#define VERSION 50
#define REV 53

//_____________________________________________________________________________

/*
 * WII POWER SETTINGS
 */
#define SYS_RESTART          0 /*!< Reboot the gamecube, force, if necessary, to boot the IPL menu. Cold reset is issued */
#define SYS_HOTRESET         1 /*!< Restart the application. Kind of softreset */
#define SYS_SHUTDOWN         2 /*!< Shutdown the thread system, card management system etc. Leave current thread running and return to caller */
#define SYS_RETURNTOMENU     3 /*!< Directly load the Wii Channels menu, without actually cold-resetting the system */
#define SYS_POWEROFF         4 /*!< Powers off the Wii, automatically choosing Standby or Idle mode depending on the user's configuration */
#define SYS_POWEROFF_STANDBY 5 /*!< Powers off the Wii to standby (red LED, WC24 off) mode. */
#define SYS_POWEROFF_IDLE    6 /*!< Powers off the Wii to idle (yellow LED, WC24 on) mode. */

//_____________________________________________________________________________

/*
DIRECTORY EXISTS BOOLEANS
*/
bool bootMiiExists = false;
bool altBootMiiExists = false;
bool bootMiiIniExists = false;
bool altBootMiiIniExists = false;
//_____________________________________________________________________________

/*
LANGAUGE STRINGS
*/
char *languages_code[10];
char *languages_desc[10];
int numberLangs = 0;
char* selected_language;
char* mm1;
char* mm2;
char* mm3;
char* mm4;
char* mm5;
char* mm6;
char* up1;
char* up2;
char* error;
char* err1_initFatErr;
char* err2_rootFS;
char* err3_noWrite;
char* err4_noExist;

char *setTitle;
char *setLang;
char *setSkin;
char *setUpdate;
char *setSave;
char *setCancel;
char *enabled;
char *disabled;



//_____________________________________________________________________________
/*
 * SKIN INTS
 */
char *skins_code[10];
char *skins_desc[10];
char *selected_skin;
int numberSkins = 0;
int skin_console_bgcolor=14;
int skin_console_textcolor=17;
int skin_main_bgcolor=7;
int skin_main_textcolor=0;
int skin_main_textcolor2=10;
int skin_main_titlecolor=11;
int skin_main_titlelinecolor=11;
int skin_main_selectedIndicatorcolor=11;
int skin_main_selectedOptioncolor=15;
int skin_main_shadow=0;
int skin_dialog_bgcolor=2;
int skin_dialog_textcolor=17;
int skin_dialog_textcolor2=17;
int skin_dialog_titlecolor=17;
int skin_dialog_titlelinecolor=17;
int skin_dialog_selectedIndicatorcolor=17;
int skin_dialog_selectedOptioncolor=17;
int skin_dialog_shadow=0;
int skin_main_decl = 7;
int skin_dialog_decl = 2;

//_____________________________________________________________________________

/**
* Holds the full path to the DOL file (from argv/argc)
*/
char* app_path;

//_____________________________________________________________________________

//static void *xfb = NULL;
static u32 *xfb;

static GXRModeObj *rmode = NULL;
static int debug = FALSE;
static char* video = "PAL50";
static char* autoboot = "SYSMENU";
static char* bootdelay = "5";
static char* videoOptions[]     = {"NTSC","PAL50","PAL60","PROGRESSIVE"};
static char* autobootOptions[]  = {"OFF","SYSMENU","HBC"};
static char* delayOptions[]  = {"0","1","2","3","4","5","6","7","8","9","10"};
//static char* switchOptions[]  = {"Disabled","Enabled","No change","Switch Directory"};
//static int updateOptions[] = {0,1};

char* videoOptionsDesc[4];
char* autobootOptionsDesc[3];
char* updateOptionsDesc[2];
char* swapOptionsDesc[4];

static int selVideoOption = 0;
static int selAutoBootOption = 0;
static int selDelayOption = 0;
static int selSwitchOption = 0;
static int selSwitchOptionStart = 0;
int selSkinOption = 0;
int selLangOption = 0;

static char fileLine[200];

static char myIP[16];


int mode = MAINMENU;
int onItem = 1;
int prevOnItem = 1;
int onSettingsItem = 1;
bool startup = true;

int major_version=0;
int minor_version=0;
char* updateDescription;
char* updateCheckURL;
char* downloadURL;
bool checkNewVersion = true;

char* bootDevice;


int networkActive=0;


extern char picdata[];
extern int  piclength;

//JPEGIMG title;

int screenHeight;

int rumbletime = 0;

s8 HWButton = -1;


//Filename to hold the ini file we're using!
char* fileName;

//_______________________________________________________________________________
/**
 * Callback for the reset button on the Wii.
 */
void WiiResetPressed()
{
	HWButton = SYS_RETURNTOMENU;
}

//_______________________________________________________________________________
/**
 * Callback for the power button on the Wii.
 */
void WiiPowerPressed()
{
	HWButton = SYS_POWEROFF_STANDBY;
}

//_______________________________________________________________________________
/**
 * Callback for the power button on the Wiimote.
 * @param[in] chan The Wiimote that pressed the button
 */
void WiimotePowerPressed(s32 chan)
{
	HWButton = SYS_POWEROFF_STANDBY;
}


//_______________________________________________________________________________
/*
* Can find the bootmii dir
*/
bool bootmiiDirExists()
{
	if (chdir("sd:/bootmii"))
	{
		//printf ("bootmii dir does not exist");
		return false;
	}
	else
	{
		//printf ("bootmii dir exists");
		return true;
	}
}

//_______________________________________________________________________________
/*
* Can find the bootmii.ini file in the normal dir
*/
bool bootMiiIniExistsInDir()
{
	if(!bootMiiExists)return false;
	else
	{
		FILE *f = fopen (strdup("sd:/bootmii/bootmii.ini"), "rb");
		if(f==NULL)return false;
	}
	return true;
}

//_______________________________________________________________________________
/*
* Can find the al bootmii dir
*/
bool altBootmiiDirExists()
{
	if (chdir("sd:/~bootmii"))
	{
		//printf ("bootmii dir does not exist");
		return false;
	}
	else
	{
		//printf ("bootmii dir exists");
		return true;
	}
}
//_______________________________________________________________________________
/*
* Can find the bootmii.ini file in the alt dir
*/
bool altBootMiiIniExistsInDir()
{
	if(!altBootMiiExists)return false;
	else
	{
		FILE *f = fopen (strdup("sd:/~bootmii/bootmii.ini"), "rb");
		if(f==NULL)return false;
	}
	return true;
}

//_______________________________________________________________________________
/*
* Can open the SD Card
*/
bool can_open_root_fs()
{
	DIR_ITER *root = diropen("sd:/");
	if (root)
	{
		dirclose(root);
		return true;
	}
	return false;
}

//_______________________________________________________________________________
/*
* Used for system exit condition
*/
void errorApplication(char *msg)
{

	fatUnmount(0);
	systemError(msg);
}

//_______________________________________________________________________________
/*
* Initialises FAT
*/
void initialiseFat()
{
	//printf(" Initialising FAT ......... ");
	if (!fatInitDefault())
	{
		drawError(15,11,11,50,err1_initFatErr);
		//errorApplication("Unable to initialise FAT subsystem, exiting.\n");
	}

	if (!can_open_root_fs())
	{
		//errorApplication("Unable to open SD card, exiting.\n");
		drawError(15,11,11,50,err2_rootFS);
	}
	//printf("Done\n");

}



//_______________________________________________________________________________
/*
* Splits a string into tokens via an equal and newline
* character and returns the second token
*/
char* getLineValue(char* line)
{
char * pch;

	if(debug==TRUE)
	{
		moveCursor(3,2);
		setColor(CYAN,FALSE);
		printf ("Splitting string \"%s\" into tokens:\n",line);
	}
	pch = strtok (line,"=\n");
	//printf ("%s\n",pch);
	pch = strtok (NULL, "=\n");
	if(debug==TRUE)printf ("%s\n",pch);
	return strdup(pch);

}

//_______________________________________________________________________________
/*
* Actually populates the sel variables with the position in the
* array of the selected item
*/
void populateOptionsFromFileData()
{
	//Now set sel values to correct ones...
	int i=0;
	for(i=0;i<4;i++)
	{
			if(strcmp(video,videoOptions[i])==0)selVideoOption=i;
	}

	for(i=0;i<3;i++)
	{
			if(strcmp(autoboot,autobootOptions[i])==0)selAutoBootOption=i;
	}

	for(i=0;i<11;i++)
	{
			if(strcmp(bootdelay,delayOptions[i])==0)selDelayOption=i;
	}
}


//_______________________________________________________________________________
/*
* writes bootmii init file and populates the
* values into variables
*/
void writeFile()
{
	initialiseFat();

	FILE *f = fopen (strdup(fileName), "wb");
	if (f == NULL)
	{
		drawError(15,11,11,50,err3_noWrite);
	}
	else
	{
		//int x;
		//for (x = 0; x < line_number; x++)
		//{

		//sprintf(temp_string, "%i %i", x_pos[x], y_pos[x]);

		//}

		char video_line[200];
		char auto_line[200];
		char delay_line[200];

		sprintf(video_line, "VIDEO=%s\n", videoOptions[selVideoOption]);

		if(selAutoBootOption==0)
		{
			sprintf(auto_line, "#AUTOBOOT=%s\n", autobootOptions[selAutoBootOption]);
		}
		else
		{
			sprintf(auto_line, "AUTOBOOT=%s\n", autobootOptions[selAutoBootOption]);
		}

		sprintf(delay_line, "BOOTDELAY=%s\n", delayOptions[selDelayOption]);
		fputs ("# BootMii config file\n",f);
		fputs ("# Created by bootmii config editor\n",f);
		fputs ("\n",f);
		fputs ("# Video mode, possible settings:\n",f);
		fputs ("# NTSC, PAL50, PAL60, PROGRESSIVE\n",f);
		fputs (video_line ,f);
		fputs ("\n",f);
		fputs ("# Autoboot, possible settings:\n",f);
		fputs ("# SYSMENU, HBC\n",f);
		fputs (auto_line ,f);
		fputs (delay_line ,f);

		fclose(f);

	}

}
//_______________________________________________________________________________
/*
S
*/
void swapIcons(bool beforeSwap)
{
  DIR *pdir;
  FILE *file;
  
  
  //app_path
  char* onIconPath;
  char* offIconPath;
  char* switchIconPath;
  char* normalIconPath;


  onIconPath = concat(app_path,"bceactive.png");
  offIconPath = concat(app_path,"bcedisabled.png");
  switchIconPath = concat(app_path,"bceswitch.png");
  normalIconPath = concat(app_path,"icon.png");
  
  pdir=opendir(app_path);
  if (!pdir) 
  {
    return;
  }
  closedir(pdir);
  
  file = fopen(onIconPath, "rb");
  if(!file)
  {
	rename(normalIconPath,onIconPath);
  }
  fclose(file);
  
  file = fopen(offIconPath, "rb");
  if(!file)
  {
	rename(normalIconPath,offIconPath);
  }
  fclose(file);
  
  file = fopen(switchIconPath, "rb");
  if(!file)
  {
	rename(normalIconPath,switchIconPath);
  }
  fclose(file);
  
  if( bootMiiExists == true && altBootMiiExists == true )
  {
	rename(switchIconPath,normalIconPath);
  }
  
  if(beforeSwap)
  {
	  if( bootMiiExists == true && altBootMiiExists == false )
	  { 
		//Will be swapped by now so alt = boot and boot = alt!
		rename(onIconPath,normalIconPath);
	  }
	  if( bootMiiExists == false && altBootMiiExists == true )
	  {

		rename(offIconPath,normalIconPath);
	  }
  }
  else
  {
	  if( bootMiiExists == true && altBootMiiExists == false )
	  { 
		//Will be swapped by now so alt = boot and boot = alt!
		rename(offIconPath,normalIconPath);
	  }
	  if( bootMiiExists == false && altBootMiiExists == true )
	  {

		rename(onIconPath,normalIconPath);
	  }
  }
}

//_______________________________________________________________________________
/*
* Renames the directories
*/
void renameDirectories()
{
  if( bootMiiExists && altBootMiiExists)
  {
    rename("sd:/bootmii","sd:/~bce");
    rename("sd:/~bootmii","sd:/bootmii");
    rename("sd:/~bce","sd:/~bootmii");
  }
  if( bootMiiExists == true && altBootMiiExists == false ) rename("sd:/bootmii","sd:/~bootmii");
  if( bootMiiExists == false && altBootMiiExists == true ) rename("sd:/~bootmii","sd:/bootmii");
  
  swapIcons(false);
}

//_______________________________________________________________________________
/*
* Reads the bootmii init file and populates the
* values into variables
*/
void readFile()
{
	selSwitchOption = 0;
	if(bootMiiExists && altBootMiiExists)
	{
		selSwitchOptionStart = 2;
		selSwitchOption = 2;
		if(bootMiiIniExists && altBootMiiIniExists)
		{

			int choice = drawThreeChoice(13,MAIN_WINY+2,11,54,strdup("Multiple BootMii Directories Found"),strdup("(A) Swap [/bootmii/] and [~/bootmii/] then Exit"), strdup("(1) Edit: [/bootmii/boomii.ini]"),strdup("(2) Edit: [/~bootmii/boomii.ini]"));
			if(choice==1)
			{
				fileName = strdup("sd:/bootmii/bootmii.ini");
			}
			else if(choice == 2)
			{
				fileName = strdup("sd:/~bootmii/bootmii.ini");
			}
			else
			{
				//Choice must be A - just swap!
				renameDirectories();
				exit(0);
			}
		}
		else if(bootMiiIniExists)
		{
			int choice = drawTwoChoiceA1(13,MAIN_WINY+2,11,54,strdup("Multiple BootMii Directories Found"),strdup("(A) Swap [/bootmii/] and [~/bootmii/] then Exit"), strdup("(1) Edit: [/bootmii/boomii.ini]"));
			if(choice==1)fileName = strdup("sd:/bootmii/bootmii.ini");
			else if (choice==2)
			{
				renameDirectories();
				exit(0);
			}
		}
		else if(altBootMiiIniExists)
		{
			int choice = drawTwoChoiceA1(13,MAIN_WINY+2,11,54,strdup("Multiple BootMii Directories Found"),strdup("(A) Swap [/bootmii/] and [~/bootmii/] then Exit"), strdup("(1) Edit: [/~bootmii/boomii.ini]"));
			if(choice==1)fileName = strdup("sd:/~bootmii/bootmii.ini");
			else if (choice==2)
			{
				renameDirectories();
				exit(0);
			}		
		}
		else
		{
			int choice = drawTwoChoiceA1(13,MAIN_WINY+2,11,54,strdup("No BootMii.ini found!"),strdup("(A) Swap [/bootmii/] and [~/bootmii/] then Exit"), strdup("(1) Exit"));
			if(choice==1)
			{
				exit(0);			
			}
			else if (choice==2)
			{
				renameDirectories();
				exit(0);			
			}
		}
		
	}
	else if(bootMiiExists)
	{
		selSwitchOption = 1;
		selSwitchOptionStart = 1;
		fileName = strdup("sd:/bootmii/bootmii.ini");
	}
	else if(altBootMiiExists)
	{
		selSwitchOption = 0;
		selSwitchOptionStart = 0;
		fileName = strdup("sd:/~bootmii/bootmii.ini");
	}
	else
	{
		//Unable to find a directory!
		drawError(15,11,11,50,strdup(err4_noExist));
	}
	
	char* value;
	FILE *f = fopen (strdup(fileName), "rb");
	if(f==NULL)
	{
		drawError(15,11,11,50,err4_noExist);
	}
	else
	{
		int line_number = 0;
		while (fgets (fileLine, 200, f))
		{

			//int len = strlen(fileLine);
			//setColor(WHITE,TRUE);

			value = "\0";
			if(strstr(fileLine,"VIDEO=")!=NULL)
			{
				video = getLineValue(fileLine);
				//setColor(RED,TRUE);
			}

			if(strstr(fileLine,"AUTOBOOT=")!=NULL)
			{
				autoboot = getLineValue(fileLine);
				//setColor(RED,TRUE);
			}

			if(strstr(fileLine,"BOOTDELAY=")!=NULL)
			{
				bootdelay = getLineValue(fileLine);
				//setColor(RED,TRUE);
			}


			//if(DEBUG)
			//{
			//moveCursor(12+line_number,2);
			//printf("Line %i: %s \n",line_number, fileLine);
			//moveCursor(12+line_number,44);
			//setColor(GREEN,TRUE);
			//printf("[%s]",filevalue);
			//}
			line_number++;
		}
		fclose(f);



		populateOptionsFromFileData();

	}
}


//_______________________________________________________________________________
/*
* Draws the header text that appears
* at the top of the screen
*/
void drawHeader()
{
	//drawString(2,7,GREEN,GRAY,  "BootMii Configuration Editor (v1.0)");
	//drawString(2,8,GREEN,GRAY, "(C)2010 Pembo - http://www.pembo.co.uk");
	//drawString(2,9,GREEN,GRAY,  "_________________________________________________");
}

//_______________________________________________________________________________
/*
* Draws the menu on the screen
*/
void drawMenu(int onPos,int prevPos)
{

	//Start of column 5 then add whatever position were on to it
	//MAIN_WINY==9

	clearArea(29,MAIN_WINY+1,8,46,skin_main_bgcolor);
	//clearArea(25,MAIN_WINY+1,8,50,GREEN);
	//clearArea(25,10+onPos,2,52,GREEN);
	//clearArea(25,10+prevPos,2,52,GREEN);

	int startCol = 6;
	int endCol = startCol+25;

	if(strcmp("es",selected_language)==0)
	{
		endCol = endCol + 5;
	}

	if(strcmp("pt",selected_language)==0)
	{
		endCol = endCol + 5;
	}


	drawString(startCol,MAIN_WINY+2,skin_main_textcolor,skin_main_bgcolor,  strdup(mm1));
	drawString(endCol,MAIN_WINY+2,skin_main_textcolor,skin_main_bgcolor,":");
	drawString(startCol,MAIN_WINY+3,skin_main_textcolor,skin_main_bgcolor,  strdup(mm2));
	drawString(endCol,MAIN_WINY+3,skin_main_textcolor,skin_main_bgcolor,":");
	if(selAutoBootOption!=0)
	{
		drawString(startCol,MAIN_WINY+4,skin_main_textcolor,skin_main_bgcolor,  strdup(mm3));
	}
	else
	{
		//Disabled so draw as dark_gray
		drawString(startCol,MAIN_WINY+4,DARK_GRAY,skin_main_bgcolor,  strdup(mm3));
	}
	drawString(endCol,MAIN_WINY+4,skin_main_textcolor,skin_main_bgcolor,":");
	
	drawString(startCol,MAIN_WINY+5,skin_main_textcolor,skin_main_bgcolor,  strdup(mm4));
	drawString(endCol,MAIN_WINY+5,skin_main_textcolor,skin_main_bgcolor,":");

	//Now write out the selected values

	int col1=skin_main_textcolor;
	int col2=skin_main_textcolor;
	int col3=skin_main_textcolor;
	int col4=skin_main_textcolor;
	int col5=skin_main_textcolor;
	int col6=skin_main_textcolor;

	if(onPos ==1)col1=skin_main_selectedOptioncolor;
	if(onPos ==2)col2=skin_main_selectedOptioncolor;
	if(onPos ==3)col3=skin_main_selectedOptioncolor;
	if(onPos ==4)col4=skin_main_selectedOptioncolor;
	if(onPos ==5)col5=skin_main_selectedIndicatorcolor;
	if(onPos ==6)col6=skin_main_selectedIndicatorcolor;

	if(onPos>4)onPos++;

	int indicatorPos = endCol + 2;
	int optionsPos = endCol + 5;
	drawString(indicatorPos,MAIN_WINY+1+onPos,skin_main_selectedIndicatorcolor,skin_main_bgcolor,"->");
	drawString(optionsPos,MAIN_WINY+2,col1,skin_main_bgcolor, strdup(videoOptionsDesc[selVideoOption]));
	drawString(optionsPos,MAIN_WINY+3,col2,skin_main_bgcolor, strdup(autobootOptionsDesc[selAutoBootOption]));


	

	if(selAutoBootOption!=0)
	{
		drawString(optionsPos,MAIN_WINY+4,col3,skin_main_bgcolor, strdup(delayOptions[selDelayOption]));
	}
	else
	{
		//Disabled so draw as dark_gray
		drawString(optionsPos,MAIN_WINY+4,DARK_GRAY,skin_main_bgcolor, strdup(delayOptions[selDelayOption]));
	}
	

	drawString(optionsPos,MAIN_WINY+5,col4,skin_main_bgcolor, strdup(swapOptionsDesc[selSwitchOption]));
	drawString(optionsPos,MAIN_WINY+7,col5,skin_main_bgcolor,strdup(mm5));
	drawString(optionsPos,MAIN_WINY+8,col6,skin_main_bgcolor,strdup(mm6));
}


//_______________________________________________________________________________
/*
* Draws the entire screen (including a clear).
*/
void drawScreen(int onItem,int prevOnItem)
{

	//drawHeader();
	drawMenu(onItem,prevOnItem);
}


//_______________________________________________________________________________
/*
* Clears screen and draws bottom right logo
*/
void startupScreen()
{
	clearConsole(BLACK);
}

//_______________________________________________________________________________
/*
* Clears the text area of the screen and draws the main window
*/
void initScreen()
{

	//Old code for the logo screen.
	/*if(screenHeight < 528)
	{
		clearArea(0,0,25,80,skin_console_bgcolor);
	}
	else
	{
		clearArea(0,0,29,80,skin_console_bgcolor);
	}*/
	clearConsole(skin_console_bgcolor);

	drawWindow (4,MAIN_WINY,15,72,skin_main_bgcolor,skin_main_titlecolor,skin_main_titlelinecolor,skin_main_bgcolor,skin_main_shadow,skin_main_decl,"BootMii Configuration Editor (v2.5)");
	drawString(36,MAIN_WINY + 12,skin_main_textcolor,skin_main_bgcolor, concat("Editing: ",strdup(fileName)));
	drawString(36,MAIN_WINY + 13,skin_main_textcolor2,skin_main_bgcolor, "(C)2010 Pembo - http://www.pembo.co.uk");

}

//_______________________________________________________________________________
/*
* Draws the settings options
*/
void drawSettingsOptions(int onSettingsPos)
{
	int startCol = 20;
	int settingsY = MAIN_WINY+2;
	int endCol = startCol+13;

	int indicatorPos=endCol+1;
	//clearArea(41,14,5,20,RED);
	int drawPos = onSettingsPos;
	if(onSettingsPos>=4)drawPos++;
	drawString(indicatorPos+1,settingsY+2+drawPos,skin_dialog_selectedIndicatorcolor,skin_dialog_bgcolor,"->");

	int col1=skin_dialog_textcolor;
	int col2=skin_dialog_textcolor;
	int col3=skin_dialog_textcolor;

	int startpos = endCol+5;

	if(onSettingsPos ==1)col1=skin_dialog_selectedOptioncolor;
	if(onSettingsPos ==2)col2=skin_dialog_selectedOptioncolor;
	if(onSettingsPos ==3)col3=skin_dialog_selectedOptioncolor;


	//char *msg = "";
	//fsprintf(msg, "[%i]%s-%s", selLangOption,strdup(languages_code[selLangOption]),strdup(languages_desc[selLangOption]));

	drawString(startpos,settingsY+3,col1,skin_dialog_bgcolor, strdup(languages_desc[selLangOption]));
	//drawString(startpos,settingsY+3,col1,skin_dialog_bgcolor, msg);
	drawString(startpos,settingsY+4,col2,skin_dialog_bgcolor, strdup(skins_desc[selSkinOption]));

	int onUpdateOpt = 0;
	if(checkNewVersion==true)onUpdateOpt = 1;
	else onUpdateOpt = 0;
	drawString(startpos,settingsY+5,col3,skin_dialog_bgcolor, strdup(updateOptionsDesc[onUpdateOpt]));


}


//_______________________________________________________________________________
/*
* Draws the settings dialog
*/
void draWSettingsDialog(int onSettingsItem)
{
	//onSettingsItem = 1;
	int settingsY = MAIN_WINY+2;
	drawWindow(15,settingsY,11,50,skin_dialog_bgcolor,skin_dialog_titlecolor,skin_dialog_titlelinecolor,skin_dialog_bgcolor,skin_dialog_shadow,skin_dialog_decl,strdup(setTitle));


	int startCol = 20;
	int endCol = startCol+13;

	int col1=skin_dialog_textcolor;
	int col2=skin_dialog_textcolor;
	int col3=skin_dialog_textcolor;
	int col4=skin_dialog_textcolor;

	if(onSettingsItem ==1)col1=skin_dialog_selectedOptioncolor;
	if(onSettingsItem ==2)col2=skin_dialog_selectedOptioncolor;
	if(onSettingsItem ==3)col2=skin_dialog_selectedOptioncolor;
	if(onSettingsItem ==4)col3=skin_dialog_selectedIndicatorcolor;
	if(onSettingsItem ==5)col4=skin_dialog_selectedIndicatorcolor;

	drawString(startCol,settingsY+3,skin_dialog_textcolor,skin_dialog_bgcolor,strdup(setLang));
	drawString(endCol,settingsY+3,skin_dialog_textcolor,skin_dialog_bgcolor,":");
	drawString(startCol,settingsY+4,skin_dialog_textcolor,skin_dialog_bgcolor,strdup(setSkin));
	drawString(endCol,settingsY+4,skin_dialog_textcolor,skin_dialog_bgcolor,":");

	drawString(startCol,settingsY+5,skin_dialog_textcolor,skin_dialog_bgcolor,strdup(setUpdate));
	drawString(endCol,settingsY+5,skin_dialog_textcolor,skin_dialog_bgcolor,":");


	drawString(endCol+5,settingsY+7,col3,skin_dialog_bgcolor,strdup(setSave));
	drawString(endCol+5,settingsY+8,col4,skin_dialog_bgcolor,strdup(setCancel));

	drawSettingsOptions(onSettingsItem);

}

//_______________________________________________________________________________
/*
* Changes the settings option
*/
void changeSettingsOption(int onsettingItem, int direction)
{

	if(onsettingItem==1)
	{
		if(direction != LEFT)
		{
			selLangOption++;
		}
		else
		{
			selLangOption--;
		}

		if(selLangOption<0)selLangOption=numberLangs-1;
		if(selLangOption>numberLangs-1)selLangOption=0;

		parseLanguageFile(strdup(languages_code[selLangOption]));

	}

	if(onsettingItem==2)
	{
		if(direction != LEFT)
		{
			selSkinOption++;
		}
		else
		{
			selSkinOption--;
		}

		if(selSkinOption<0)selSkinOption=numberSkins-1;
		if(selSkinOption>numberSkins-1)selSkinOption=0;



		parseSkinsFile(strdup(skins_code[selSkinOption]));
		initScreen();
		drawScreen(onItem,prevOnItem);
		draWSettingsDialog(onsettingItem);

	}

	if(onsettingItem==3)
	{
		checkNewVersion = !checkNewVersion;

		//if(selSkinOption<0)selSkinOption=numberSkins-1;
		//if(selSkinOption>numberSkins-1)selSkinOption=0;
		//parseSkinsFile(strdup(skins_code[selSkinOption]));
		//initScreen();
		//drawScreen(onItem,prevOnItem);
		draWSettingsDialog(onsettingItem);

	}


}
//_______________________________________________________________________________
/*
* Process change option using right/left on the DPAD
* cycles through the available options
*/
void changeOption(int onItem, int direction)
{

	if(onItem==1)
	{
		if(direction != LEFT)
		{
			selVideoOption++;
		}
		else
		{
			selVideoOption--;
		}

		if(selVideoOption<0)selVideoOption=3;
		if(selVideoOption>3)selVideoOption=0;
	}

	if(onItem==2)
	{

		if(direction != LEFT)
		{
			selAutoBootOption++;
		}
		else
		{
			selAutoBootOption--;
		}

		if(selAutoBootOption<0)selAutoBootOption=2;
		if(selAutoBootOption>2)selAutoBootOption=0;

	}

	if(onItem==3)
	{
		if(direction != LEFT)
		{
			selDelayOption++;
		}
		else
		{
			selDelayOption--;
		}

		if(selDelayOption<0)selDelayOption=10;
		if(selDelayOption>10)selDelayOption=0;
	}
	
	if(onItem==4)
	{
		if(direction != LEFT)
		{
			selSwitchOption++;
		}
		else
		{
			selSwitchOption--;
		}


		if(bootMiiExists && altBootMiiExists && selSwitchOption <2)selSwitchOption=3;
		if(bootMiiExists && altBootMiiExists && selSwitchOption >3)selSwitchOption=2;
		
		if(bootMiiExists && !altBootMiiExists && selSwitchOption <0 ) selSwitchOption = 1;
		if(bootMiiExists && !altBootMiiExists && selSwitchOption >1 ) selSwitchOption = 0;
		
		if(!bootMiiExists && altBootMiiExists && selSwitchOption <0 ) selSwitchOption = 1;
		if(!bootMiiExists && altBootMiiExists && selSwitchOption >1 ) selSwitchOption = 0;
		
	
	}

}



//_______________________________________________________________________________
/*
* Initialise items required for a WII Application
*/
void initialiseWiiApp()
{
	//printf("\n\n");
	// Initialise the video system
	VIDEO_Init();

	// This function initialises the attached controllers

	//Init wii pad
	WPAD_Init();

	//Init Gamecube pad
	PAD_Init();

	// Obtain the preferred video mode from the system
	// This will correspond to the settings in the Wii menu
	rmode = VIDEO_GetPreferredMode(NULL);

	// Allocate memory for the display in the uncached region
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

	// Initialise the console, required for printf
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);



	// Set up the video registers with the chosen mode
	VIDEO_Configure(rmode);

	// Tell the video hardware where our display memory is
	VIDEO_SetNextFramebuffer(xfb);

	// Make the display visible
	VIDEO_SetBlack(FALSE);

	// Flush the video register changes to the hardware
	VIDEO_Flush();

	// Wait for Video setup to complete
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();



	//printf("WIDTH  [%i]\n",rmode->fbWidth);
	//printf("HEIGHT [%i]\n",rmode->xfbHeight);

	screenHeight = rmode->xfbHeight;

	if(screenHeight < 528)
	{
		LOGOY = LOGOY - 58;
		MAIN_WINY = MAIN_WINY - 3;
	}

}

//_______________________________________________________________________________
/*
* Initialise the App path
*/
void initialiseAppPath(int argc, char* argv)
{
	//printf("ARGC is %i",argc);
	//printf("start app path");
	//sleep(5);
	
	if(argc>0 && argv!=NULL && (stringlength(argv)>0))
	{
		int lindexof =lastIndexOf(strdup(argv),'/');
		//printf("\nLast Index of: %i\n",lindexof);
		app_path = strdup(left(strdup(argv),lindexof));
		//printf("PATH: %s\n",app_path);
	}
	else
	{
		setColor(RED,BLACK);
		printf(" Unable to determine working directory - assuming [sd:/apps/Bootmii_cfg_ed/]");
		app_path = "sd:/apps/Bootmii_cfg_ed/";
		setColor(WHITE,BLACK);
	}

	//printf("Done app path");
	//sleep(5);


}

//_______________________________________________________________________________
/*
* Check the relevant directories exist on the SD card
*/
void checkDirectories()
{
  printf(" sd:/bootmii/ exists? ..... ");
  bootMiiExists = bootmiiDirExists();
  bootMiiIniExists = bootMiiIniExistsInDir();
  if(bootMiiExists){
	printf("Yes\n");
  }
  else
  {
	printf("No\n");
  }
  
  printf(" sd:/~bootmii/ exists? .... ");
  altBootMiiExists = altBootmiiDirExists();
  altBootMiiIniExists = altBootMiiIniExistsInDir();
  if(altBootMiiExists){
	printf("Yes\n");
  }
  else
  {
	printf("No\n");
  }
}


bool updateFileFromUrl(char* url , char* location)
{
	printf("\n");
	printf(" Downloading Update ....... ");
	setColor(YELLOW,BLACK);
	//sleep(2);
	bool doneUpdate = false;
	doneUpdate = getFileFromServer(strdup(url), strdup(location));
	setColor(WHITE,BLACK);

	if(doneUpdate==true)printf("Done\n");
	else printf("Failed\n");
	//sleep(5);
	return doneUpdate;
}

//_______________________________________________________________________________
/*
* Unzips a file!
*/
bool unzipArchive(char * zipfilepath, char * unzipfolderpath)
{
	//setColor(YELLOW,BLACK);
	unzFile uf = unzOpen(zipfilepath);
	//setColor(WHITE,BLACK);
	if (uf==NULL)
	{
		printf(" Cannot open %s, aborting\n",zipfilepath);
		return false;
	}
	//printf(" %s opened\n",zipfilepath);

	if(chdir(unzipfolderpath)) // can't access dir
	{
		makedir(unzipfolderpath); // attempt to make dir
		if(chdir(unzipfolderpath)) // still can't access dir
		{
			printf(" Error changing into %s, aborting\n", unzipfolderpath);
			return false;
		}
	}

	int uzipResult = 0;
	uzipResult = extractZip(uf,0,1,0);

	if(uzipResult==0)
	{
		printf("\bDone\n");
	}
	else
	{
		printf("\bFailed\n");
	}

	unzCloseCurrentFile(uf);
	return true;
}

//_______________________________________________________________________________
/*
* Checks for a new version
*/
void checkForUpdatedVersion()
{
			if(networkActive==0)return;
			if(checkNewVersion==false)return;

			printf(" Checking for new version.. ");

			setColor(WHITE,BLACK);
			char* vercheckUrl;
			vercheckUrl = concat(updateCheckURL,"bce_update.ver");
			struct block result = downloadfile(vercheckUrl,false);
			sleep(1);
			//printf("\n\nBLOCK SIZE [%d]\n",result.size);
			//printf(" ERROR [%s]\n", result.error);
			//printf("DATA-------------------------------\n");
			//printf("%s",result.data);
			//printf("---------------------------END-DATA\n");

			printf("Done (Latest Version is v%i.%i)\n\n",(result.data[0]-48),(result.data[1]-48));
			//printf("Downloading update");
				//downloadUpdate("");
			//printf(" Done\n");

			//printf("VERSION [%i]\n",result.data[0]);
			//printf("REV     [%i]\n",result.data[1]);
			if ( (result.data[0] > VERSION) ||  ((result.data[0] = VERSION ) && (result.data[1] > REV)) )
			{
				//printf("\n\nNEW VERSION AVAILABLE!\n");
				setColor(GREEN,BLACK);
				printf(" There is a new version of BootMii Configuration Editor available\n");
				char ver[2];
				sprintf(ver, "%i%i",(result.data[0]-48),(result.data[1]-48));
				//char longver[4];
				//sprintf(longver, "v%i.%i",(result.data[0]-48),(result.data[1]-48));
				printf(" v%i.%i is available to download.\n",(result.data[0]-48),(result.data[1]-48));
				setColor(WHITE,BLACK);
				printf(" Download Update Info...... ");
				//printf(" Checking for new version.. ");
				char* whatsnewUrlDir;
				char* whatsnewUrlFull;
				char versionDir[3];
				sprintf(versionDir,"%i%i/",(result.data[0]-48),(result.data[1]-48));
				whatsnewUrlDir = concat(updateCheckURL,versionDir);
				whatsnewUrlFull = concat(whatsnewUrlDir,"whatsnew.txt");

				struct block whatsnewresult = downloadfile(whatsnewUrlFull,false);
				printf("Done\n");

				char changes[240];

				strcpy (changes,(char*)whatsnewresult.data);

				char* changesStr;
				changesStr= left(strdup(changes),whatsnewresult.size - 1);

				setColor(GRAY,BLACK);
				printf("\n Changes\n ------------------------------------------------------------------------------\n%s\n ------------------------------------------------------------------------------\n",changesStr);

				setColor(RED,BLACK);
				printf(" Press '1' to Update or '2' to continue without updating\n");
				setColor(WHITE,BLACK);
				int choice = 2;
				choice = getChoiceFrom1or2();
				if(choice == 1)
				{

					bool ok=true;

					char* zipfile = getFullFileName("update.zip");

					char* fullZipUrl = concat(whatsnewUrlDir,"update.zip");
					ok = updateFileFromUrl(strdup(fullZipUrl),strdup(zipfile));

					if(ok==true)
					{
						//Now unzip
						char* unzipdir;
						unzipdir = strdup(app_path);
						ok = unzipArchive(strdup(zipfile), strdup(unzipdir));
					}

					if(ok==false)
					{
					setColor(RED,BLACK);
					printf(" Update Failed - Aborting");
					sleep(5);
					return;
					}

					setColor(GREEN,BLACK);
					printf("\n Update Complete.  Press 'HOME' to Exit, then please restart\n");
					printf(" BootMii Configuration Editor from your launcher.");
					pressHomeToExit();

				}
				else
				{
					setColor(GREEN,BLACK);
					printf(" Update Skipped");
					setColor(WHITE,BLACK);
				}


			}
			else
			{
				//printf(" You are running the latest version");

			}
			sleep(2);
}


//_______________________________________________________________________________
/*
* Initialise the App parts (load settings files/etc)
*/
void initialiseCoreApplication(int argc, char* argv)
{
	//printf("in init app\n");
	//sleep(2);
	//display_jpeg(title, LOGOX, LOGOY);

	//printf("Initialising Please wait....");
	printf("\n\n Initialising BootMii Configuration Editor (v2.5) - Please wait\n\n");
	//sleep(5);


	initialiseAppPath(argc,strdup(argv));

	//printf("about to initialise fat");

	initialiseFat();
	sleep(1);


	printf(" Loading Settings File .... ");
	load_settingsFile();
	printf("Done\n");

	printf(" Loading Skins File ....... ");
	load_xmlSkinsFile();
	printf("Done\n");

	printf(" Loading Language File .... ");
	load_xmlLanguageFile();
	printf("Done\n");


	networkActive = 0;
	if(checkNewVersion==true)
	{
		bool result = false;
		result = networkInit(myIP);
		if(result==true)networkActive=1;
	}
	checkDirectories();
	checkForUpdatedVersion();
	swapIcons(true);
	printf(" Loading bootmii.ini....... ");
	clearConsole(skin_console_bgcolor);
	readFile();
	//printf("Done\n");
}

//_______________________________________________________________________________
/*
* Move to next option
*/
void changeOptionRight()
{
	if(mode==MAINMENU && onItem<5)
	{
		changeOption(onItem,RIGHT);
		drawScreen(onItem,prevOnItem);
	}
	if(mode==SETTINGS && onSettingsItem<4)
	{
		changeSettingsOption(onSettingsItem,RIGHT);
		draWSettingsDialog(onSettingsItem);

	}
}

//_______________________________________________________________________________
/*
* Move to next option
*/
void changeOptionLeft()
{
	if(mode==MAINMENU && onItem<5)
	{
		changeOption(onItem,LEFT);
		drawScreen(onItem,prevOnItem);
	}
	if(mode==SETTINGS && onSettingsItem<4)
	{
		changeSettingsOption(onSettingsItem,LEFT);
		draWSettingsDialog(onSettingsItem);
	}
}

//_______________________________________________________________________________
/*
* Move to next option
*/
void changeOptionUp()
{
	if(mode==MAINMENU)
	{
		prevOnItem = onItem;
		onItem--;
		if(onItem<1)onItem=1;

		if(selAutoBootOption==0 && onItem==3)onItem--;

		drawScreen(onItem,prevOnItem);
	}
	if(mode==SETTINGS)
	{
		onSettingsItem--;
		if(onSettingsItem<1)onSettingsItem=1;
		draWSettingsDialog(onSettingsItem);
	}
}

//_______________________________________________________________________________
/*
* Move to next option
*/
void changeOptionDown()
{
	if(mode==MAINMENU)
	{
		prevOnItem = onItem;
		onItem++;
		if(onItem>6)onItem=6;
		if(selAutoBootOption==0 && onItem==3)onItem++;
		drawScreen(onItem,prevOnItem);
	}

	if(mode==SETTINGS)
	{
		onSettingsItem++;
		if(onSettingsItem>5)onSettingsItem=5;
		draWSettingsDialog(onSettingsItem);
	}
}







//_______________________________________________________________________________
/*
* Main application
*/
//int main(int argc, char **argv)
int main(int argc, char *argv[])
{
	//sleep(5);

	//printf(" initialise wii app\n");
	//sleep(1);
	initialiseWiiApp();
	//sleep(5);

	//printf(" startup screen\n");
	//sleep(1);
	startupScreen();
	//sleep(5);

	//printf("init app\n ");
	//sleep(2);
	initialiseCoreApplication(argc,strdup(argv[0]));
	//sleep(5);


	SYS_SetResetCallback(WiiResetPressed);
	SYS_SetPowerCallback(WiiPowerPressed);
	WPAD_SetPowerButtonCallback(WiimotePowerPressed);


	


	initScreen();

	int origLang = -1;
	int origSkin = -1;
	bool origUpdate = true;

	drawScreen(onItem,prevOnItem);

	//int mode=MAINMENU;

	while(1) {

		// Call WPAD_ScanPads each loop, this reads the latest controller states
		WPAD_ScanPads();
		PAD_ScanPads();

		checkStopRumble();


		if(HWButton != -1)
		{
			break;
		}



		// WPAD_ButtonsDown tells us which buttons were pressed in this loop
		// this is a "one shot" state which will not fire again until the button has been released
		u32 pressed = WPAD_ButtonsDown(0);
		u16 gcPressed = PAD_ButtonsDown(0);





		// show settings menu if wii home or GC start pressed
		if ( (pressed & WPAD_BUTTON_HOME)||(gcPressed & PAD_BUTTON_START) )
		{
			startRumble();
			if(mode==MAINMENU)
			{

				origLang = selLangOption + 0;
				origSkin = selSkinOption + 0;
				origUpdate = checkNewVersion + 0;
				onSettingsItem = 1;
				mode = SETTINGS;
				draWSettingsDialog(onSettingsItem);
			}
			else if(mode==SETTINGS)
			{
				if(origLang>=0)
				{
					if(origLang != selLangOption)
					{
						selLangOption = origLang + 0;
						parseLanguageFile(strdup(languages_code[selLangOption]));
					}

					if(origSkin != selSkinOption)
					{
						selSkinOption = origSkin + 0;
						parseSkinsFile(strdup(skins_code[selSkinOption]));
					}

					checkNewVersion = origUpdate;
				}


				mode=MAINMENU;
				initScreen();
				drawScreen(onItem,prevOnItem);

			}
		//exit(0);

		}



		if ( (pressed & WPAD_BUTTON_DOWN) || (PAD_StickY(0) < -18) || (gcPressed & PAD_BUTTON_DOWN))
		{
			changeOptionDown(mode,onItem,prevOnItem,onSettingsItem);
		}

		if ( (pressed & WPAD_BUTTON_UP) || (PAD_StickY(0) > 18) || (gcPressed & PAD_BUTTON_UP))
		{
			changeOptionUp(mode,onItem,prevOnItem,onSettingsItem);
		}

		if ( (pressed & WPAD_BUTTON_LEFT) || (PAD_StickX(0) < -18) || (gcPressed & PAD_BUTTON_LEFT) || (gcPressed & PAD_TRIGGER_L))
		{
			changeOptionLeft(mode,onItem,prevOnItem,onSettingsItem);

		}

		if ( (pressed & WPAD_BUTTON_RIGHT) || (PAD_StickX(0) > 18) || (gcPressed & PAD_BUTTON_RIGHT) || (gcPressed & PAD_TRIGGER_R))
		{
			changeOptionRight(mode,onItem,prevOnItem,onSettingsItem);
		}

		if (pressed & WPAD_BUTTON_PLUS )
		{
			changeOptionRight(mode,onItem,prevOnItem,onSettingsItem);
		}

		if (pressed & WPAD_BUTTON_MINUS)
		{
			changeOptionLeft(mode,onItem,prevOnItem,onSettingsItem);
		}

		if (pressed & WPAD_BUTTON_B)
		{
			if(onItem<4)
			{
				changeOptionLeft(mode,onItem,prevOnItem,onSettingsItem);
			}
		}


		if (pressed & WPAD_BUTTON_1)
		{
			//struct block file;
			//file = downloadfile("http://77.75.108.7/BCE/version.xml");
			//printf("SIZE [%i]",file.size);
			//printf("DATA\n%s",file.data);
			//printf("ERROR [%s]",file.error);
			//clearConsole(skin_console_bgcolor);
			//downloadFromURL();




			/*
			* GET FILE FROM URL
			*/





			//press1ToContinue();

			//clearConsole(skin_console_bgcolor);
			//initScreen();


		}


		if ((pressed & WPAD_BUTTON_A) || (gcPressed & PAD_BUTTON_A))
		{
			if(mode==MAINMENU)
			{
				if(onItem<5)
				{
					changeOptionRight(mode,onItem,prevOnItem,onSettingsItem);
				}

				if(onItem==5)
				{
					//if(bootmiiDirExists())
					//{
						writeFile();
						drawWindow(15,MAIN_WINY+2,11,50,skin_dialog_bgcolor,skin_dialog_titlecolor,skin_dialog_titlelinecolor,skin_dialog_bgcolor,skin_dialog_shadow,skin_dialog_decl,up1);
						centerString(MAIN_WINY+6,skin_dialog_textcolor,skin_dialog_bgcolor,up2);
						if(selSwitchOptionStart!=selSwitchOption)
						{
							centerString(MAIN_WINY+7,skin_dialog_textcolor,skin_dialog_bgcolor,"Now Switching Directories...");
							renameDirectories();
						}
						
						exit(0);
					//}
					//else
					//{
					//	drawWindow(15,MAIN_WINY+2,11,50,DARK_RED,WHITE,WHITE,DARK_RED,BLACK,DARK_RED,error);
					//	centerString(MAIN_WINY+6,WHITE,DARK_RED,err4_noExist);
					//	exit(0);
					//}
					//fatUnmount(0);


				}


				if(onItem==6)
				{
					//fatUnmount(0);
					exit(0);
				}

			}
			else if(mode==SETTINGS)
			{
				if(onSettingsItem==4)
				{
					//printf("About to update settings - press 1 to continue");
					//press1ToContinue();
					update_settings();

					//printf("About to load lang file - press 1 to continue");
					//press1ToContinue();

					//load_xmlLanguageFile();
					//load_xmlSkinsFile();
					parseLanguageFile(strdup(languages_code[selLangOption]));
					//parseSkinsFile();
					parseSkinsFile(strdup(skins_code[selSkinOption]));

					mode=MAINMENU;
					initScreen();
					drawScreen(onItem,prevOnItem);
				}

				if(onSettingsItem==5)
				{
					//load_xmlLanguageFile();
					selLangOption = origLang + 0;
					selSkinOption = origSkin + 0;
					checkNewVersion = origUpdate + 0;
					parseLanguageFile(strdup(languages_code[selLangOption]));
					parseSkinsFile(strdup(skins_code[selSkinOption]));
					mode=MAINMENU;
					initScreen();
					drawScreen(onItem,prevOnItem);
				}
			}

		}


		// Wait for the next frame
		VIDEO_WaitVSync();
	}


	if(HWButton == SYS_POWEROFF_STANDBY)
	{
		SYS_ResetSystem(SYS_POWEROFF,0,0);
	}

	if(HWButton == SYS_RETURNTOMENU)
	{
		SYS_ResetSystem(SYS_RESTART,0,0);

	}


	return 0;
}
