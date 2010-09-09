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
#include "../wiimote/wiimote.h"
#include "../skinvars.h"


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

//Booleans - True or False
#define FALSE 0
#define TRUE 1




//_______________________________________________________________________________
/*
* Gets the actual console color code from the MACRO
*
*/
int getColor(int color)
{
	if(color>=10) color = color -10;
	return color;
}

//_______________________________________________________________________________
/*
* Returns TRUE of FALSE if the color is bright version or not
*
*/
int getBright(int color)
{
	if(color>=10)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//_______________________________________________________________________________
/*
* Clears the console
*
*/
void clearConsole(int bgcolor)
{

	int setColor;
	setColor = getColor(bgcolor);

	if(getBright(bgcolor)==TRUE)
	{
		printf("\x1b[4%i;1m",setColor);
	}
	else
	{
		printf("\x1b[4%im",setColor);
	}

	printf("\x1b[2J");
}


//_______________________________________________________________________________
/*
* prints a backspace character
*/
void backspace()
{
	printf("\b");
}

//_______________________________________________________________________________
/*
* Moves the cursor to the provided
* row / column non the screen
*/
void moveCursor (int row, int column)
{
	printf("\x1b[%i;%iH",row,column);
}

///_______________________________________________________________________________
/*
* Sets the color of the console text
* to the provided value
* Valid colors are:
* BLACK 0
* DARK_RED 1
* DARK_GREEN 2
* DARK_YELLOW 3
* DARK_BLUE 4
* DARK_MAGENTA 5
* DARK_CYAN 6
* LIGHTGRAY 7
* DARKGRAY 10
* RED 11
* GREEN 12
* YELLOW 13
* BLUE 14
* MAGENTA 15
* CYAN 16
* WHITE 17
* See http://en.wikipedia.org/wiki/ANSI_escape_code
*/
void setColor(int fgcolor,int bgcolor)
{

	int actualFgColor = getColor(fgcolor);
	int actualBgColor = getColor(bgcolor);
	int fgBright = getBright(fgcolor);
	int bgBright = getBright(bgcolor);

	if(bgBright==TRUE)
	{
		printf("\x1b[4%i;1m",actualBgColor);
	}
	else
	{
		printf("\x1b[4%im",actualBgColor);
	}

	if(fgBright==TRUE)
	{
		printf("\x1b[3%i;1m",actualFgColor);
	}
	else
	{
		printf("\x1b[3%im",actualFgColor);
	}

}

//_______________________________________________________________________________
/*
* Used for system exit condition
*/
void systemError(char *msg)
{
	perror(msg);
	//sleep(5);
	exit(0);
}


//_______________________________________________________________________________
/*
* Draws a text string at a given position
* in the provided color and inteisty and then
* resets the color back to white
*/
void drawString (int x, int y, int color, int bgColor, char *msg){

moveCursor(y,x);
setColor(color,bgColor);
printf(msg);
setColor(WHITE,BLACK);
}


//_______________________________________________________________________________
/*
* centres a text string at a vertical position
* in the provided color and inteisty and then
* resets the color back to white
*/
void centerString (int y, int color, int bgColor, char *msg){

int stringLen = strlen(msg);
int centrePos = 40;
int titleLenDiv2 = stringLen / 2;

int startPos = (centrePos - titleLenDiv2);

moveCursor(y,startPos);
setColor(color,bgColor);
printf(msg);
setColor(WHITE,BLACK);
}

//_______________________________________________________________________________
/*
* Draws a console based window!
*/
void drawWindow (int startX,int startY,int rows,int cols, int bgColor,int title_textColor,int title_textlineColor,int title_bgColor, int shadowColor, int declColor, char* windowTitle)
{
	int i=0;
	int j=0;

	for(i=0;i<cols;i++)
	{
		drawString(startX+i,startY,title_bgColor,title_bgColor," ");
	}

	int titleLen = strlen(windowTitle);
	int centrePos = cols/2+startX;
	int titleLenDiv2 = titleLen / 2;

	int titlePos = (centrePos - titleLenDiv2);
	int titlePosMinusTwo = (titlePos -2);


	/** draw the lines **/
	i=0;
	for(i=startX;i<titlePosMinusTwo;i++)
	{
		drawString(i,startY,title_textlineColor,title_bgColor,"-");
	}
	drawString(titlePosMinusTwo,startY,title_textlineColor,title_bgColor,"|");

	drawString(titlePos,startY,title_textColor,title_bgColor,windowTitle);

	drawString(titlePos + titleLen + 1,startY,title_textlineColor,title_bgColor,"|");
	i=0;
	for(i=titlePos + titleLen + 2;i<cols+startX;i++)
	{
		drawString(i,startY,title_textlineColor,title_bgColor,"-");
	}


	for(i=0;i<cols;i++)
	{
	  for(j=1;j<rows;j++)
	  {
		drawString(startX+i,startY+j,bgColor,bgColor," ");
	  }
	}

	//bottom line
	for(i=0;i<cols;i++)
	{
		drawString(startX+i,startY+rows-1,declColor,bgColor,"-");
	}

	//left & right line
	for(j=1;j<rows-1;j++)
	{
		drawString(startX,startY+j,declColor,bgColor,"|");
		drawString(startX+cols-1,startY+j,declColor,bgColor,"|");
	}

	//Corners
	//top left
	//drawString(startX,startY+1,title_textlineColor,bgColor,"+");
	//top right
	//drawString(startX+cols-1,startY+1,title_textlineColor,bgColor,"+");
	//bottom left
	//drawString(startX,startY+rows-1,title_textlineColor,bgColor,"+");
	//bottom right
	//drawString(startX+cols-1,startY+rows-1,title_textlineColor,bgColor,"+");





	//finally the shadow
	for(i=1;i<=rows;i++)
	{
		drawString(startX+cols,startY+i,shadowColor,shadowColor," ");
	}

	for(i=1;i<=cols;i++)
	{
		drawString(startX+i,startY+rows,shadowColor,shadowColor," ");
	}

	moveCursor(startY + cols + 3,2);
}

//_______________________________________________________________________________
/*
* Shows an application error
* Good size if 15,11,11,50,DARK_RED,WHITE,DARK_RED
*/

int drawInfo(int startX,int startY,int rows,int cols,char* msg,char* choiceMessage)
{
	//errorApplication("Could not open file for reading");
	drawWindow(startX,startY,rows,cols,GRAY,BLACK,BLACK,GRAY,BLACK,GRAY,"Information!");
	//drawWindow(15,11,11,50,DARK_RED,WHITE,DARK_RED,"Error");

	centerString(15,BLACK,GRAY,msg);
	centerString(17,BLACK,GRAY,choiceMessage);
	return getChoiceFrom1or2();
}


//_______________________________________________________________________________
/*
* Shows an application error
* Good size if 15,11,11,50,DARK_RED,WHITE,DARK_RED
*/

void drawError(int startX,int startY,int rows,int cols,char* msg)
{
		//errorApplication("Could not open file for reading");
		drawWindow(startX,startY,rows,cols,DARK_RED,WHITE,WHITE,DARK_RED,BLACK,DARK_RED,"Error");
		//drawWindow(15,11,11,50,DARK_RED,WHITE,DARK_RED,"Error");

		centerString(15,WHITE,DARK_RED,msg);
		centerString(17,WHITE,DARK_RED,"Press HOME to Exit");
		pressHomeToExit();
		exit(-1);
}
//_______________________________________________________________________________
/*
* Shows an application error
* Good size if 15,11,11,50,DARK_RED,WHITE,DARK_RED
*/

int drawFourChoice(int startX,int startY,int rows,int cols,char* title,char* opt1,char* opt2,char* opt3,char* opt4)
{
		//errorApplication("Could not open file for reading");
		
		drawWindow(startX,startY,rows,cols,skin_dialog_bgcolor,skin_dialog_titlecolor,skin_dialog_titlelinecolor,skin_dialog_bgcolor,skin_dialog_shadow,skin_dialog_decl,title);
		//drawWindow(15,11,11,50,DARK_RED,WHITE,DARK_RED,"Error");

		drawString(startX+3, 12,skin_dialog_textcolor,skin_dialog_bgcolor,opt1);
		drawString(startX+3, 13,skin_dialog_textcolor,skin_dialog_bgcolor,opt2);
		drawString(startX+3, 14,skin_dialog_textcolor,skin_dialog_bgcolor,opt3);
		drawString(startX+3, 15,skin_dialog_textcolor,skin_dialog_bgcolor,opt4);
		centerString(17,skin_dialog_textcolor,skin_dialog_bgcolor,"Press (A), (B), (1) or (2) to select");
		int choice = getChoiceFromAB1or2();
		return choice;
}

int drawThreeChoiceAB1(int startX,int startY,int rows,int cols,char* title,char* opt1,char* opt2,char* opt3)
{
		//errorApplication("Could not open file for reading");
		
		drawWindow(startX,startY,rows,cols,skin_dialog_bgcolor,skin_dialog_titlecolor,skin_dialog_titlelinecolor,skin_dialog_bgcolor,skin_dialog_shadow,skin_dialog_decl,title);
		//drawWindow(15,11,11,50,DARK_RED,WHITE,DARK_RED,"Error");

		drawString(startX+3, 13,skin_dialog_textcolor,skin_dialog_bgcolor,opt1);
		drawString(startX+3, 14,skin_dialog_textcolor,skin_dialog_bgcolor,opt2);
		drawString(startX+3, 15,skin_dialog_textcolor,skin_dialog_bgcolor,opt3);
		centerString(17,skin_dialog_textcolor,skin_dialog_bgcolor,"Press (A), (B) or (1) to select");
		int choice = getChoiceFromABor1();
		return choice;
}

int drawThreeChoice(int startX,int startY,int rows,int cols,char* title,char* opt1,char* opt2,char* opt3)
{
		//errorApplication("Could not open file for reading");
		
		drawWindow(startX,startY,rows,cols,skin_dialog_bgcolor,skin_dialog_titlecolor,skin_dialog_titlelinecolor,skin_dialog_bgcolor,skin_dialog_shadow,skin_dialog_decl,title);
		//drawWindow(15,11,11,50,DARK_RED,WHITE,DARK_RED,"Error");

		drawString(startX+3, 13,skin_dialog_textcolor,skin_dialog_bgcolor,opt1);
		drawString(startX+3, 14,skin_dialog_textcolor,skin_dialog_bgcolor,opt2);
		drawString(startX+3, 15,skin_dialog_textcolor,skin_dialog_bgcolor,opt3);
		centerString(17,skin_dialog_textcolor,skin_dialog_bgcolor,"Press (A), (1) or (2) to select");
		int choice = getChoiceFromA1or2();
		return choice;
}

int drawTwoChoice(int startX,int startY,int rows,int cols,char* title,char* opt1,char* opt2)
{
		//errorApplication("Could not open file for reading");
		
		drawWindow(startX,startY,rows,cols,skin_main_bgcolor,skin_main_titlecolor,skin_main_titlelinecolor,skin_main_bgcolor,skin_main_shadow,skin_main_decl,title);
		//drawWindow(15,11,11,50,DARK_RED,WHITE,DARK_RED,"Error");

		drawString(startX+3, 14,skin_main_textcolor,skin_main_bgcolor,opt1);
		drawString(startX+3, 15,skin_main_textcolor,skin_main_bgcolor,opt2);
		centerString(17,skin_main_textcolor,skin_main_bgcolor,"Press 1 or 2 to select");
		int choice = getChoiceFrom1or2();
		return choice;
}

int drawTwoChoiceA1(int startX,int startY,int rows,int cols,char* title,char* opt1,char* opt2)
{
		//errorApplication("Could not open file for reading");
		
		drawWindow(startX,startY,rows,cols,skin_dialog_bgcolor,skin_dialog_titlecolor,skin_dialog_titlelinecolor,skin_dialog_bgcolor,skin_dialog_shadow,skin_dialog_decl,title);
		//drawWindow(15,11,11,50,DARK_RED,WHITE,DARK_RED,"Error");

		drawString(startX+3, 14,skin_dialog_textcolor,skin_dialog_bgcolor,opt1);
		drawString(startX+3, 15,skin_dialog_textcolor,skin_dialog_bgcolor,opt2);
		//centerString(17,skin_dialog_textcolor,skin_dialog_bgcolor,"Press 1 or 2 to select");
		centerString(17,skin_dialog_textcolor,skin_dialog_bgcolor,"Press (A) or (1) to select");
		int choice = getChoiceFromAor1();
		return choice;
}


//_______________________________________________________________________________
/*
* Clears a specific area of the screen (to prevent tearing)
*
*/
void clearArea(int startX,int startY, int rows, int cols, int bgcolor)
{

	int i,j;
	for(i=0;i<cols;i++)
	{
	  for(j=1;j<rows;j++)
	  {
		drawString(startX+i,startY+j,bgcolor,bgcolor," ");
	  }
	}

}
