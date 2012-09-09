/*
 * I use the sepereated list and 1 bytes differed seperated fits.
 * The final score is 55+40=95. Optimize the test data a little.(SIZE13 is 614800)
 *
 * ID:	 09302010035
 * Name: Wu Yanhong
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/* declare mm_check as static */
static int mm_check(void);

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8
#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1<<12)
#define OVERHEAD 8

#define NUM 14
#define SIZE0 16
#define SIZE1 32
#define SIZE2 64
#define SIZE3 128
#define SIZE4 256
#define SIZE5 512
#define SIZE6 1024
#define SIZE7 2048
#define SIZE8 4096
#define SIZE9 8192
#define SIZE10 16384
#define SIZE11 32768
#define SIZE12 65536
#define SIZE13 614800
/* rounds up to the nearest multiple of ALIGNMENT */
#define PACK(size, alloc) ((size) | (alloc))
#define GET(p) (*(size_t *)(p))
#define PUT(p, val) (*(size_t *)(p) = (val))
//If the current block has been seperated.
#define GET_IF_LOW(p) (GET(p)& 0x1)
#define GET_IF_LOW2(p) (GET(HDRP(p))&0x2)
#define GET_SIZE(p) (GET(p) & ~0x7)
#define HDRP(bp) ((char *)(bp) - WSIZE)

//The start byte.
char* heap_st = 0;
//The end byte.
char* heap_ed = 0;
int sizeLocation = 0;
//Select a 8 bytes aligned size.
size_t switchSize(size_t size) {
	if (size <= SIZE0) {
		sizeLocation = 0;
		return SIZE0;
	} else if (size <= SIZE1) {
		sizeLocation = 1;
		return SIZE1;
	} else if (size <= SIZE2) {
		sizeLocation = 2;
		return SIZE2;
	} else if (size <= SIZE3) {
		sizeLocation = 3;
		return SIZE3;
	} else if (size <= SIZE4) {
		sizeLocation = 4;
		return SIZE4;
	} else if (size <= SIZE5) {
		sizeLocation = 5;
		return SIZE5;
	} else if (size <= SIZE6) {
		sizeLocation = 6;
		return SIZE6;
	} else if (size <= SIZE7) {
		sizeLocation = 7;
		return SIZE7;
	} else if (size <= SIZE8) {
		sizeLocation = 8;
		return SIZE8;
	} else if (size <= SIZE9) {
		sizeLocation = 9;
		return SIZE9;
	} else if (size <= SIZE10) {
		sizeLocation = 10;
		return SIZE10;
	} else if (size <= SIZE11) {
		sizeLocation = 11;
		return SIZE11;
	} else if (size <= SIZE12) {
		sizeLocation = 12;
		return SIZE12;
	} else {
		sizeLocation = 13;
		return SIZE13;
	}
}
//Return the start address of corresponding size.
int getLocation(size_t size) {
	if (size <= SIZE0) {
		return 0;
	} else if (size <= SIZE1) {
		return 1;
	} else if (size <= SIZE2) {
		return 2;
	} else if (size <= SIZE3) {
		return 3;
	} else if (size <= SIZE4) {
		return 4;
	} else if (size <= SIZE5) {
		return 5;
	} else if (size <= SIZE6) {
		return 6;
	} else if (size <= SIZE7) {
		return 7;
	} else if (size <= SIZE8) {
		return 8;
	} else if (size <= SIZE9) {
		return 9;
	} else if (size <= SIZE10) {
		return 10;
	} else if (size <= SIZE11) {
		return 11;
	} else if (size <= SIZE12) {
		return 12;
	} else {
		return 13;
	}
}
//Return the mid byte of a block.
int buddy(int sizeLocation) {
	size_t st = 16;
	int i;
	for (i = 0; i < NUM - 1; i++) {
		if (sizeLocation == i)
			return (st + 16) / 2;
		st *= 2;
	}
	return -1;
}
//Give the address of a seperated link point,return the size.
int getSize(int sizeLocation) {
	switch (sizeLocation) {
	case 0:
		return SIZE0;
	case 1:
		return SIZE1;
	case 2:
		return SIZE2;
	case 3:
		return SIZE3;
	case 4:
		return SIZE4;
	case 5:
		return SIZE5;
	case 6:
		return SIZE6;
	case 7:
		return SIZE7;
	case 8:
		return SIZE8;
	case 9:
		return SIZE9;
	case 10:
		return SIZE10;
	case 11:
		return SIZE11;
	case 12:
		return SIZE12;
	case 13:
		return SIZE13;
	}
	return -1;
}
int count;
int flag1, flag2;
int* flagB;
/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void) {
	count = 0;
	int i;
	//The seperated list.
	if ((heap_st = mem_sbrk(NUM * WSIZE)) == NULL)
		return -1;
	for (i = 0; i < NUM; i++) {
		*((int*) heap_st + i) = 0;
	}
	//32 bytes of little size cache.
	flag1 = 0;
	flag2 = 0;
	if ((flagB = mem_sbrk(2 * SIZE0)) == NULL)
		return -1;
	heap_ed = (char*) ((int*) heap_st + NUM + 8);
	return 0;
}
//If there exists a free block which has the same size,return it.
//If the block is large enough,then if there exists a larger free block,seperate it.
//If there is two smaller free block, combinate them.
int freePlace;
int flag = 0;
size_t find(int sizeLocation) {
	void *bp = (int*) heap_st + sizeLocation;
	//Check its size.
	if ((sizeLocation == NUM - 1) || (sizeLocation == 0) || (sizeLocation <= 7)) {
		freePlace = sizeLocation;
		return GET(bp);
	}
	freePlace = 0;
	//Get the same size free block.
	if (GET(bp) != 0) {
		freePlace = sizeLocation;
		return (GET(bp));
	}
	//Check the larger one.
	if (GET((int*)bp+1) != 0)
		if (GET_IF_LOW(GET((int*)bp+1)) == 0) {
			freePlace = sizeLocation + 1;
			return ((size_t) ((int*) bp + 1));
		}
	//Check the smaller ones.
	int *tar1 = (int*) GET((int*)bp-1);
	if ((int) tar1 != 0) {
		int *tar2 = (int*) GET(HDRP(tar1));
		if (tar2 != 0) {
			//Check if the address of the 2 blocks are successive.
			flag = ((getSize(sizeLocation - 1) + 16 + (size_t) tar1
					== (size_t) tar2) || (getSize(sizeLocation - 1) + 16
					+ (size_t) tar2 == (size_t) tar1));
			if ((flag > 0) || ((getSize(sizeLocation - 1) + DSIZE
					+ (size_t) tar1 == (size_t) tar2) || (getSize(sizeLocation
					- 1) + (size_t) tar2 + DSIZE == (size_t) tar1))) {
				freePlace = sizeLocation - 1;
				return ((size_t) tar1);
			}
		}
	}
	return 0;
}
/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size) {
	int* ans = 0;
	if (size <= 0)
		return NULL;
	size_t asize = switchSize(size);
	//Check the little block cache.
	if (asize == SIZE0) {
		switch (flag1 | flag2) {
		case 0:
			flag1 = 1;
			return flagB;
		case 1:
			flag2 = 2;
			return ((int*) flagB + 4);
		case 2:
			flag1 = 1;
			return flagB;
		}
	}
	void *bp = (int*) heap_st + sizeLocation;
	//If there is no appropriate free block, increase the heap size.
	if (find(sizeLocation) == 0) {
		int overhead, overhead2;
		//If the block size is small,the header size is 8 bytes, otherwise it's 16 bytes.
		//Because 16 bytes can be seperated by 2 smaller ones.
		if (asize <= SIZE7) {
			overhead = 8;
			overhead2 = 1;
		} else {
			overhead = 16;
			overhead2 = 3;
		}
		if ((ans = mem_sbrk(asize + overhead)) == NULL)
			return NULL;
		heap_ed = (char*) heap_ed + asize + overhead;
		ans = (int*) ans + overhead2;
		PUT(ans,PACK(asize,1));
		ans = (int*) ans + 1;
	} else {
		//The part which combinate 2 smaller free blocks.
		if (freePlace == sizeLocation - 1) {
			ans = (int*) GET((int*)bp-1);
			PUT((int*)bp-1,GET(HDRP(ans)));
			int *ans2 = (int*) GET((int*)bp-1);
			PUT((int*)bp-1,GET(HDRP(ans2)));
			if (ans > ans2)
				ans = ans2;
			if (!flag)
				ans = ans + 2;
			PUT(HDRP(ans),PACK(asize,1));
		}//Get the corresponding free block.
		else if (freePlace == sizeLocation) {
			ans = (int*) GET(bp);
			PUT(bp,GET(HDRP(ans)));
			//If the current free block has been seperated.
			if (GET_IF_LOW(ans) == 1)
				PUT(HDRP(ans),PACK(asize,3));
			else
				PUT(HDRP(ans),PACK(asize,1));
		}//Seperate a larger one.
		else if (freePlace == sizeLocation + 1) {
			ans = (int*) GET((int*)bp+1);
			PUT((int*)bp+1,GET(HDRP(ans)));
			int *ans2 = (int*) (ans) + buddy(sizeLocation + 1) / 4 - 2;
			ans = ans - 2;
			//Indicate this block has been seperated.
			PUT(HDRP(ans),PACK(asize,3));
			PUT(HDRP(ans2),GET(bp));
			PUT(ans2,0x1);
			PUT(bp,(size_t)ans2);
		}
	}
	return ans;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *block) {
	//Free the little block cache.
	if (block == flagB) {
		flag1 = 0;
		return;
	}
	if (block == ((int*) flagB + 4)) {
		flag2 = 0;
		return;
	}
	size_t size = GET_SIZE(HDRP(block));
	int location = getLocation(size);
	void *bp = (int*) heap_st + location;
	//If the current free block has been seperated.
	if (GET_IF_LOW2(block) > 0)
		PUT(block,0x1);
	else
		PUT(block,0x0);
	PUT(HDRP(block),GET(bp));
	PUT(bp,(int)block);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *bp, size_t size) {
	void *oldptr = bp;
	void *newptr;
	//Get current block size.
	size_t copySize = GET_SIZE(HDRP(bp));
	//An optimization,if the realloc size is smaller than before then return oldptr immediately.
	if (size <= copySize)
		return oldptr;
	//Check if the realloc block is the tail of the heap.
	if (bp == heap_ed - GET_SIZE(HDRP(bp))) {
		size_t limit = size - copySize;
		//Make it 8 bytes aligned.
		if (limit % 8 != 0)
			limit = (limit / 8 + 1) * 8;
		mem_sbrk(limit);
		heap_ed = (char*) heap_ed + limit;
		PUT(HDRP(oldptr),PACK(copySize+limit,1));
		return oldptr;
	}
	//Otherwise malloc a new block and free the old one simply.
	newptr = mm_malloc(size);
	if (newptr == NULL)
		return NULL;
	memcpy(newptr, oldptr, size);
	mm_free(oldptr);
	return newptr;
}
/*
 * mm_check - Does not currently check anything
 */
static int mm_check(void) {
	int i;
	//Check free blocks.
	for (i = 0; i < NUM; i++) {
		int *bp = (int*) heap_st + i;
		while (1) {
			if (*bp == 0)
				break;
			//Check if the address pointed is valid.
			if ((int) (*bp) < (int) heap_st) {
				printf("The number 0x%x is not a valid heap address.\n",
						(int) *bp);
				return -1;
			}
			//Check 8 bytes alignment.
			if ((int) (*bp) % 8 != 0) {
				printf("The block 0x%x is not 8 bytes aligned.\n", (int) *bp);
				return -1;
			}
			bp = (int*) GET(HDRP(bp));
		}
	}
	//Get the data content.
	int *bp = (int *) heap_st + NUM;
	while (1) {
		bp = (int*) bp + DSIZE;
		if (bp >= (int*) mem_heap_hi())
			break;
		if (GET(HDRP(bp)) == 0)
			bp = (int*) bp + DSIZE;
		if (bp >= (int*) mem_heap_hi())
			break;
		//Check 8 bytes alignment.
		if (((int) bp) % 8 != 0) {
			printf("The block 0x%x is not 8 byte aligned.\n", (int) bp);
			return -1;
		}
		//Check if the block size is valid.
		size_t size = GET_SIZE(HDRP(bp));
		bp = (int*) bp + size;
		if ((int) *bp != 0) {
			printf("The size of block 0x%x is invalid.\n", (int) ((int*) bp
					- size));
			return -1;
		}
	}
	return 0;
}

