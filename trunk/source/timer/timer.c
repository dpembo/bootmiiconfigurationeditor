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

#include "timer.h"

unsigned long tb_diff_msec (tb_t * end, tb_t * start)
{
  unsigned long upper, lower;
  upper = end->u - start->u;
  if (start->l > end->l)
    upper--;
  lower = end->l - start->l;
  return ((upper * ((unsigned long) 0x80000000 / (TB_CLOCK / 2000))) +
          (lower / (TB_CLOCK / 1000)));
}
