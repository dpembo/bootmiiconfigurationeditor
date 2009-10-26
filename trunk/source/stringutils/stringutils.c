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
