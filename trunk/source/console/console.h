

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
int drawInfo(int startX,int startY,int rows,int cols,char* msg,char* choiceMessage);
void systemError(char *msg);


