/*
 *Project 3: My Malloc - my_malloc.c
 ***************************************************
 * Author: Matthew Viego
 * 
 *  The program is a custom dynamic memory management system that utilizes custom malloc, free and dump heap methods.
 *  I use a linked list to determine how much space is being user/how much space is freed. The malloc function will
 *  allocate space using a best fit algorithm and free will properly coalasce already freed memory
 *
 *******************************************************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "mymalloc.h"

/*
*Struct Block
*************
* Block is a linked list node.
* occ - whether the memory location is being used
* size - size of Block + size of memory that was allocated
***********************************************************/

struct Block{
	int occ;
	int size;
	struct Block *next;
	struct Block *prev; 
};

static struct Block *head = NULL;
static struct Block *tail = NULL;

/*
*my_malloc(int size)
********************
* size - size to allocate
*
* The function starts out by checking if a head node has been created in the linked list. If not, a head will be made. If not the function
* will first use a best fit algorithm to check if there is an open node. If not, a new node will be created and sbrk will be called to allocate
* space at the top of the heap
**************************************************************************************************************************************************/

void *my_malloc(int size){
	struct Block* current;
	if (head == NULL){
		head = (struct Block *)sbrk(sizeof(struct Block) + size);
		head->occ = 1;
		head->size = size + sizeof(struct Block); 		// account for size of argument and the size of the Block
		head->next = NULL;
		head->prev = NULL;
		tail = head;
		return (void *)head + sizeof(struct Block);		// returns pointer to portion of memory that is right after the struct size
	}

	else{
		current = best_fit(size);	//finds best fit if possible
		if (current != NULL){
			current->size = size + sizeof(struct Block);
			current->occ = 1;
			return (void *)current + sizeof(struct Block);
		}
		else{
                        current = (struct Block*)sbrk(sizeof(struct Block) + size);	// allocates room for a new node at the end of the linked list in the heap 
                        current->occ = 1;
                        current->size = size + sizeof(struct Block);
                        current->prev = tail;
                 	current->next = NULL;
                     	tail->next=current;
                        tail = current;
                        return (void *)tail + sizeof(struct Block);
                }			
		
		
	}
}

/*best_fit(int size)
 **********************
 * This function will accept as a paramter the size of the memory to be allocated (not including the size of the node).
 * The funtion will start out by finding the smallest possible continuous space of memory in the heap that is large enough
 * to account for the size of what we need to allocate. If there is a space to use, we check if it is large enough to make
 * two nodes out of it. Then we return a struct pointer to the location of the node where size will be allocated
 ****************************************************************************************************************************/

struct Block* best_fit(int size){
	struct Block* current = head;
	struct Block* bestFit = NULL;
	void* split = NULL;
	struct Block* fuck = NULL;		// Sorry I got frustrated and had to do it
	int minimum = 0x7fffffff;		// Maximum size of an int
	int extraSpace;
	while (current != NULL){
		if ((current->occ == 0) && (current->size >= size)){	// checks if the node is unoccupied and the size is large enough
			if (current->size < minimum){
				minimum = current->size;		// continues to find the next smallest possible space
				bestFit = current;
			}
		}
		current = current->next;
	}
	if (bestFit != NULL){
		extraSpace = bestFit->size - (size + sizeof(struct Block));	// finds the amount fo space after allocating the memory for size and the size of a node
		if (extraSpace > 24){			// ensures the extra space is greater than the size of a node
				current = bestFit + 1;
				split = (void *) current;	// we use a void* so we can move to the appropriate memory location
				fuck = split + size;
				printf("HELLO");
				printf("%d", (char *)bestFit->next - (char *)head);
                                fuck->prev = bestFit;
				fuck->next = bestFit->next;
				bestFit->next = fuck;
				fuck->occ = 0;
				fuck->size = extraSpace;
				if (bestFit == tail){	// accounts for if we are placing in the tail
					tail = fuck;
				}
				else{
					fuck->next->prev = fuck;
				}			
		}
	}
	return bestFit;

	
}

/*
 * my_free(void *data)
 ****************************
 * Time to free some Nodes!
 *
 * First the function will create a pointer to the node that we are trying to deallocate. After checking that the node we are freeing is neither the 
 * head or the tail, we check to see if there are any open spaces surrounding the node and coalesce accordingly. If we are freeing from the head or tail
 * of the linked list then a couple extra precautions must happen to preserve the linked list
 *********************************************************************************************************************************************************/

void my_free(void *data){
	struct Block* freeBlock = (struct Block*)(data - sizeof(struct Block));
	if ((freeBlock->occ == 0)||(data == NULL)){return;}
	if ((freeBlock != head) && (freeBlock != tail)){
		if ((freeBlock->prev->occ == 0)&&(freeBlock->next->occ == 0)){						//if the node is surrounded by free space
			freeBlock->prev->size = freeBlock->prev->size + freeBlock->size + freeBlock->next->size;
			freeBlock->prev->next=freeBlock->next->next;
			freeBlock->next->next->prev = freeBlock->prev;
		}
		else if (freeBlock->prev->occ == 0){							// if the node has a free space to the left
			freeBlock->prev->size = freeBlock->prev->size + freeBlock->size;
			freeBlock->prev->next = freeBlock->next;
			freeBlock->next->prev = freeBlock->prev;
		}
		else if (freeBlock->next->occ == 0){							// if the node has a free space to the right
			freeBlock->size = freeBlock->size + freeBlock->next->size;
			freeBlock->next->next->prev = freeBlock;
			freeBlock->next = freeBlock->next->next;
			freeBlock->occ = 0;
		}	
		else{											// if the node has no free space to the left or right
			freeBlock->occ = 0;	
		}
	}
	else{						// the node to be freed is either the head, tail or both
		if (head == tail){			// if node is the head and tail
			sbrk(0 - head->size);		// shrink heap by the size of the head and delete remainder of linked list
			head = NULL;
			tail = head;
		}
		else{
			if (freeBlock == head){					// the node is the head
				if (freeBlock->next->occ == 0){			// if the node to the right of the head is freed
					if (freeBlock->next == tail){		// if the node to the right of the head is freed and the tail
						sbrk(0 - head->size - tail->size);	// shrink heap by the size of the head and the tail
						head = NULL;
						tail = head;
					
					}
					head->size = (head->size + head->next->size);	// freed space to the right of the head is not the tail so combine sizes of head and head->next
					head->occ = 0;					
					head->next->next->prev = head;
					head->next = head->next->next;
				}	
				else{
					head->occ = 0;
				}
			}
			if (freeBlock == tail){					// the node is the tail
				if (tail->prev->occ == 0){			// if the node to the left of the tail is free
					if (tail->prev==head){			// if the node to the left of the tail is free and the head
						sbrk(0 - tail->size - head->size);	//shrink heap by  the size of the head and tail
						head = NULL;
						tail = head;
					}
					else{					// freed node to the left of the tail is not the head
						tail = freeBlock->prev->prev;	// change position of the tail
						sbrk(0 - freeBlock->size - freeBlock->prev->size);	// shrink heap by tail and tail->prev
						tail->next = NULL;
					}
				}
				else{
					tail = freeBlock->prev;
					sbrk(0 - freeBlock->size);
					tail->next = NULL;
				}
			}

		}
	}
}

void dump_heap(){
	struct Block *cur;
	printf("brk: %p\n", sbrk(0));
  	printf("head->");
  	for(cur = head; cur != NULL; cur = cur->next) {
    		printf("[%d:%d:%d]->", cur->occ, (char*)cur - (char*)head, cur->size);
    		fflush(stdout);
    		assert((char*)cur >= (char*)head && (char*)cur + cur->size <= (char*)sbrk(0)); // check that block is within bounds of the heap
    		if(cur->next != NULL) {
      			assert(cur->next->prev == cur); // if not last block, check that forward/backward links are consistent
      			assert((char*)cur + cur->size == (char*)cur->next); // check that the block size is correctly set
    		}
  	}
  	printf("NULL\n");

}
