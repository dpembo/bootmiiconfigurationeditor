
#include "file.h"

char* getFullFileName(char* fileName)
{
	return strdup(concat(strdup(app_path),fileName));
}