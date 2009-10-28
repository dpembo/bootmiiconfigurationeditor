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

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

/**
* Gets the last index of a character in a string
**/
int lastIndexOf(char* string, char searchFor)
{
	char *ptr_my;
	ptr_my=strrchr(string, searchFor);


		if (ptr_my == NULL)
			return -1;
			//printf("Character is not found.\n");
		//else
			//printf("Character found at %d\n", ptr_my - string+1);


	int i = -1;
	i = (ptr_my - string+1);
	return i;
}
//_________________________________________________________________________

/**
 * Gets the length of a string
**/
int stringlength(char* string)
{
	return strlen(string);
}
//_________________________________________________________________________

/**
 * Performs a left substring
**/
char* left(char* input, int length)
{
	char result[length+1];
	//printf("\nInput is : [%s]",input);
	//printf("\nLength is: [%i]",length);
	strlcpy (result, input, length+1);
	//printf("\nRESULT IS: [%s]\n",result);
	return strdup(result);
}


//_________________________________________________________________________

/**
 * Concatenates 2 strings
**/

char * concat(char* str1,char* str2)
{
	char str[200];
	strcpy (str,strdup(str1));
	strcat  (str,strdup(str2));
	//puts(str);
	return strdup(str);

}
