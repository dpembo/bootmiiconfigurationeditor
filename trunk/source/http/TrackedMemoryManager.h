#pragma once

#define MEMORY_TEST
#define MEMORY_TRACKER_SIZE 2048
#define MEMORY_TRACKER_POINTER 0
#define MEMORY_TRACKER_POINTER_SIZE 1
#ifdef MEMORY_TEST
        #define CFMalloc(x) TrackedMalloc(x)
        #define CFMemAlign(x,y) TrackedMemAlign(x,y)
        #define CFCalloc(x,y) TrackedCalloc(x,y)
        #define CFRealloc(x,y) TrackedRealloc(x,y)
        #define CFFree(x) TrackedFree(x)
#else
        #define CFMalloc(x) malloc(x)
        #define CFMemAlign(x,y) memalign(x,y)
        #define CFCalloc(x) calloc(x)
        #define CFRealloc(x) realloc(x,y)
        #define CFFree(x) free(x)
#endif

int TrackingBuffer[MEMORY_TRACKER_SIZE][2];

void InitialiseTracker();

void * TrackedMalloc(int size);

void * TrackedMemAlign(int alignment, int size);

void * TrackedCalloc(int count, int size);

void * TrackedRealloc(void * ptr, int size);

void TrackedFree(void * ptr);

int ReportNumberOfAllocations();
int ReportSizeOfAllocations();

void ClearAllocatedMemory();
