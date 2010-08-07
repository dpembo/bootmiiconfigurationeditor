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

void clearConsole(int bgcolor);
void clearArea(int startX,int startY, int rows, int cols, int bgcolor);
int getColor(int color);
int getBright(int color);
void moveCursor (int row, int column);
void setColor(int fgcolor,int bgcolor);
void centerString (int y, int color, int bgColor, char *msg);
void drawString (int x, int y, int color, int bgColor, char *msg);
void drawWindow (int startX,int startY,int rows,int cols, int bgColor,int title_textColor,int title_textlineColor,int title_bgColor, int shadowColor, int declColor, char* windowTitle);
void drawError(int startX,int startY,int rows,int cols,char* msg);
int drawTwoChoice(int startX,int startY,int rows,int cols,char* title,char* opt1,char* opt2);
int drawTwoChoiceA1(int startX,int startY,int rows,int cols,char* title,char* opt1,char* opt2);
int drawThreeChoice(int startX,int startY,int rows,int cols,char* title,char* opt1,char* opt2,char* opt3);
int drawInfo(int startX,int startY,int rows,int cols,char* msg,char* choiceMessage);
void systemError(char *msg);


