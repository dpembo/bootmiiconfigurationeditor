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

#ifndef _HTTP_H_
#define _HTTP_H_

#include <errno.h>
#include <network.h>
#include <ogcsys.h>
#include <string.h>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include "dns.h"

/**
 * A simple structure to keep track of the size of a malloc()ated block of memory
 */
struct block
{
        u32 size;
        unsigned char *data;
        char error[128];
};

extern struct block emptyblock;

struct block downloadfile(const char *url,bool showDots);
bool getFileFromServer(char* url, char* imgPath);

#endif /* _HTTP_H_ */
