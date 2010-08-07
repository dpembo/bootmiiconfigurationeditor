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

#include "malloc.h"
#include "TrackedMemoryManager.h"

void InitialiseTracker()
{
        int i;
        for (i=0;i<MEMORY_TRACKER_SIZE;i++)
        {
                TrackingBuffer[i][MEMORY_TRACKER_POINTER]=0;
                TrackingBuffer[i][MEMORY_TRACKER_POINTER_SIZE]=0;
        }
}

int FindFreeSlot()
{
        int i,ret=0;
        for (i=0;i<MEMORY_TRACKER_SIZE;i++)
        {
                if (TrackingBuffer[i][MEMORY_TRACKER_POINTER]==0)
                {
                        ret=i;
                        break;
                }
        }
        return ret;
}

int FindPointerSlot(void * ptr)
{
        int i,ret=0;
        for (i=0;i<MEMORY_TRACKER_SIZE;i++)
        {
                if ((void*)TrackingBuffer[i][MEMORY_TRACKER_POINTER]==ptr)
                {
                        ret=i;
                        break;
                }
        }
        return ret;
}

void * TrackedMalloc(int size)
{
        void * ret;
        ret=malloc(size);
        if (ret>0)
        {
                int newSlot=FindFreeSlot();
                TrackingBuffer[newSlot][MEMORY_TRACKER_POINTER]=(int)ret;
                TrackingBuffer[newSlot][MEMORY_TRACKER_POINTER_SIZE]=size;
        }
        return ret;
}

void * TrackedMemAlign(int alignment, int size)
{
        void * ret;
        ret=memalign(alignment,size);
        if (ret>0)
        {
                int newSlot=FindFreeSlot();
                TrackingBuffer[newSlot][MEMORY_TRACKER_POINTER]=(int)ret;
                TrackingBuffer[newSlot][MEMORY_TRACKER_POINTER_SIZE]=size;
        }
        return ret;
}

void * TrackedCalloc(int count, int size)
{
        void * ret;
        ret=calloc(count,size);
        if (ret>0)
        {
                int newSlot=FindFreeSlot();
                TrackingBuffer[newSlot][MEMORY_TRACKER_POINTER]=(int)ret;
                TrackingBuffer[newSlot][MEMORY_TRACKER_POINTER_SIZE]=size;
        }
        return ret;
}

void * TrackedRealloc(void * ptr, int size)
{
        void * ret;
        ret=realloc(ptr,size);
        if (ret>0)
        {
                int existingSlot=FindPointerSlot(ptr);
                TrackingBuffer[existingSlot][MEMORY_TRACKER_POINTER]=(int)ret;
                TrackingBuffer[existingSlot][MEMORY_TRACKER_POINTER_SIZE]=size;
        }
        return ret;
}

void TrackedFree(void * ptr)
{
        int existingSlot=FindPointerSlot(ptr);
        TrackingBuffer[existingSlot][MEMORY_TRACKER_POINTER]=0;
        TrackingBuffer[existingSlot][MEMORY_TRACKER_POINTER_SIZE]=0;
        free(ptr);
}


int ReportNumberOfAllocations()
{
        int i,ret=0;
        for (i=0;i<MEMORY_TRACKER_SIZE;i++)
        {
                if (TrackingBuffer[i][MEMORY_TRACKER_POINTER]!=0)
                {
                        ret++;
                }
        }
        return ret;
}

int ReportSizeOfAllocations()
{
        int i,ret=0;
        for (i=0;i<MEMORY_TRACKER_SIZE;i++)
        {
                if (TrackingBuffer[i][MEMORY_TRACKER_POINTER]!=0)
                {
                        ret+=TrackingBuffer[i][MEMORY_TRACKER_POINTER_SIZE];
                }
        }

        return ret;
}

/*
void ReportOnTracking(int fullReport)
{
        int i;
        if (fullReport==0)
        {
                int TotalAllocations=0;
                int TotalAllocated=0;
                for (i=0;i<MEMORY_TRACKER_SIZE;i++)
                {
                        if (TrackingBuffer[i][MEMORY_TRACKER_POINTER]!=0)
                        {
                                TotalAllocated+=TrackingBuffer[i][MEMORY_TRACKER_POINTER_SIZE];
                                TotalAllocations++;
                        }
                }
                fprintf(stdout,"%d bytes in %d blocks\n",TotalAllocated,TotalAllocations);
        }
        else if (fullReport==1)
        {
                for (i=0;i<MEMORY_TRACKER_SIZE;i++)
                {
                        if (TrackingBuffer[i][MEMORY_TRACKER_POINTER]!=0)
                        {
                                fprintf(stdout,"%d bytes of memory at %d \n",TrackingBuffer[i][MEMORY_TRACKER_POINTER_SIZE],TrackingBuffer[i][MEMORY_TRACKER_POINTER]);
                        }
                }
        }
}
*/

void ClearAllocatedMemory()
{
        int i=0;
        for (i=0;i<MEMORY_TRACKER_SIZE;i++)
        {
                if (TrackingBuffer[i][MEMORY_TRACKER_POINTER]!=0)
                {
                        TrackedFree((void *)TrackingBuffer[i][MEMORY_TRACKER_POINTER]);
                }
        }
}
