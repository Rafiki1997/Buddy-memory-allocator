/*
 File: my_allocator.h
 
 Original Author: R.Bettati
 Department of Computer Science
 Texas A&M University
 Date  : 08/02/08
 
 Modified:
 
 */

#ifndef _BuddyAllocator_h_                   // include file only once
#define _BuddyAllocator_h_
#include <iostream>
#include <math.h>
#include <vector>
using namespace std;
typedef unsigned int uint;

/* declare types as you need */

class BlockHeader{
    // decide what goes here
    // hint: obviously block size will go here
public:
    int blockSize;
    bool free;
    BlockHeader* next;
    BlockHeader(int bSize, bool fuhree, BlockHeader* heed)
    {
        blockSize = bSize;
        free = fuhree;
        next = heed;
    }
    BlockHeader()
    {
        blockSize = 0;
        free = true;
        next = NULL;
    }
    
};

class LinkedList{
    // this is a special linked list that is made out of BlockHeader s.
private:
    
    BlockHeader* head;
    
public:
    LinkedList(BlockHeader* h = nullptr) : head(h) {
        
    }
    
    bool isEmpty()
    {
        if(head == nullptr){
            return true;
        }
        else{
            return false;
        }
    }
    
    void insert (BlockHeader* b){    // adds a block to the list
        
        if(head == nullptr)
        {
            head = b;
            b->next = NULL;
        }
        else
        {
            b->next = head;
            head = b;
        }
        
    }
    
    void remove (BlockHeader* b){  // removes a block from the list
        
        BlockHeader* Curr = head;
        BlockHeader* Prev = head;
        if( head == b && head->next == NULL){
            head = NULL;
            return;
        }
        if(head->next != NULL && head == b)
        {
            head = head->next;
            return;
        }
        
        while(Curr != NULL)
        {
            if (Curr == b)
            {
                Prev->next = Curr->next;
                Curr->next = nullptr;
                break;
            }
            else
            {
                Prev = Curr;
                Curr = Curr->next;
            }
        }
    }
    
    BlockHeader* getHead()
    {
        return head;
    }
    
};


class BuddyAllocator{
private:
    /* declare member variables as necessary */
    vector<LinkedList*> freeList;
    uint bbs,tms;
    char* memBlck;
    
    
private:
    /* private function you are required to implement
     this will allow you and us to do unit test */
    
    char* getbuddy (char * addr)
    // given a block address, this function returns the address of its buddy
    {
        BlockHeader* temp = (BlockHeader*)(addr);
        return (((addr- memBlck) ^ temp->blockSize) + memBlck);
    }
    
    bool isvalid (char *addr);
    // Is the memory starting at addr is a valid block
    // This is used to verify whether the a computed block address is actually correct
    
    
    bool arebuddies (char* block1, char* block2)
    // checks whether the two blocks are buddies are not
    {
        BlockHeader* temp1 = (BlockHeader*) block1;
        BlockHeader* temp2 = (BlockHeader*) block2;
        if(getbuddy(block1) == block2)
        {
            if(temp1->blockSize == temp2->blockSize)
            {
                if(temp1->free && temp2->free){
                    return true;
                }
                else{
                    return false;
                }
            }
            else{
                return false;
            }
        }
        else{
            return false;
        }
        
    }
    
    
    char* merge (char* block1, char* block2);
    // this function merges the two blocks returns the beginning address of the merged block
    // note that either block1 can be to the left of block2, or the other way around
    
    
    char* split (char* block);
    // splits the given block by putting a new header halfway through the block
    // also, the original header needs to be corrected
    
    
    
    
public:
    BuddyAllocator (unsigned int _basic_block_size, unsigned int _total_memory_length);
    /* This initializes the memory allocator and makes a portion of
     â€™_total_memory_lengthâ€™ bytes available. The allocator uses a â€™_basic_block_sizeâ€™ as
     its minimal unit of allocation. The function returns the amount of
     memory made available to the allocator. If an error occurred,
     it returns 0.
     */
    
    ~BuddyAllocator();
    /* Destructor that returns any allocated memory back to the operating system.
     There should not be any memory leakage (i.e., memory staying allocated).
     */
    
    char* alloc(uint _length);
    /* Allocate _length number of bytes of free memory and returns the
     address of the allocated portion. Returns 0 when out of memory. */
    
    int free(char* _a);
    /* Frees the section of physical memory previously allocated
     using â€™my_mallocâ€™. Returns 0 if everything ok. */
    
    void debug ();
    /* Mainly used for debugging purposes and running short test cases */
    /* This function should print how many free blocks of each size belong to the allocator
     at that point. The output format should be the following (assuming basic block size = 128 bytes):
     
     128: 5
     256: 0
     512: 3
     1024: 0
     ....
     ....
     which means that at point, the allocator has 5 128 byte blocks, 3 512 byte blocks and so on.*/
};

#endif

