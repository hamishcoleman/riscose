/* Inadequate program for testing heap.c */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "heap.h"

#define HEAP_SIZE (2048<<10)
#define MAX_BLOCKS 2048

int
main(void)
{
  BYTE *blocks[MAX_BLOCKS];
  heap_t *test = malloc(HEAP_SIZE);
  
  bzero(blocks, MAX_BLOCKS*sizeof(BYTE*));
  heap_init(test, HEAP_SIZE);
  
  for (;;)
    {
     int num = rand()%MAX_BLOCKS, c;
     int size = rand()%(HEAP_SIZE/8);
     
     if (rand()%5 == 0 && !blocks[num])
       {
        blocks[num] = heap_block_alloc(test, size);
        if (blocks[num] != NULL)
          {
           for (c=0; c!=size; c++)
             blocks[num][c] = num%256;
           printf("allocated block %d %08x\n", num, (unsigned int)blocks[num]);
          }
       }
     num = rand()%MAX_BLOCKS;
     if (rand()%5 == 0 && blocks[num])
       {
        size = heap_block_size(test, blocks[num]);
        printf("checking %d (%02x)\n", num, num%256);
        for (c=0; c!=size; c++)
          {
           if ( (blocks[num][c]&0xff) != num%256)
             {
              abort();
             }
          }
        heap_block_free(test, blocks[num]);
        blocks[num] = NULL;
        printf("freed block %d\n", num);
       }
     printf("Heap free %d bytes, largest %d\n", heap_describe(test, &num), num); 
    }
}
