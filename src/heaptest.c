/* Inadequate program for testing heap.c */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "heap.h"

#define HEAP_SIZE (512)
#define MAX_BLOCKS 1024

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
     int size = (rand()%(HEAP_SIZE/8));
     
     (size+=(4-(size&3)));
          
     if (rand()%5 == 0 && !blocks[num])
       {
        blocks[num] = heap_block_alloc(test, size);
        if (blocks[num] != NULL)
          {
           for (c=0; c!=size; c++)
             blocks[num][c] = num%256;
           printf("allocated block %d @ %08x, size %d (described %d)\n", num, (unsigned int)blocks[num], size, heap_block_size(test, blocks[num]));
           for (c=0; c!=size; c++)
              if ( (blocks[num][c]&0xff) != num%256)
        	 abort();
          }
	else
	  {
	   printf("alloc failed for block %d, size %d\n", num, size);
	  }
       }
     num = rand()%MAX_BLOCKS;
     if (rand()%5 == 0 && blocks[num])
       {
        size = heap_block_size(test, blocks[num]);
        printf("checking block %d (fill byte %02x) size %d\n", num, num%256, size);
        for (c=0; c!=size; c++)
           if ( (blocks[num][c]&0xff) != num%256)
	     {
	      printf("aborting on byte %d = %d\n", c, blocks[num][c]&0xff);
              abort();
	     }
        heap_block_free(test, blocks[num]);
        blocks[num] = NULL;
        printf("freed block %d\n", num);
       }
     //printf("Heap free %d bytes, largest %d\n", heap_describe(test, &num), num); 
    }
}
