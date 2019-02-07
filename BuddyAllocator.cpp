/*
 File: my_allocator.cpp
 */
#include "BuddyAllocator.h"
#include <iostream>
using namespace std;

BuddyAllocator::BuddyAllocator (unsigned int _basic_block_size, unsigned int _total_memory_length)
{
    bbs = _basic_block_size;
    tms = _total_memory_length;
    memBlck = new char[tms];
    
    BlockHeader* headBlock = (BlockHeader*)memBlck;
    headBlock->blockSize = tms;
    headBlock->next = NULL;
    headBlock->free = true;
    
    int index = log2(tms/bbs)+1;

    for(int i = 0; i < index; i++)
    {
        LinkedList* llPtrs = new LinkedList;
        freeList.push_back(llPtrs);
    }
    freeList.at(0)->insert(headBlock);
}



BuddyAllocator::~BuddyAllocator()
/* Destructor that returns any allocated memory back to the operating system.
 There should not be any memory leakage (i.e., memory staying allocated).
 */
{
    freeList.clear();
    delete memBlck;
}

char* BuddyAllocator::alloc(uint _length)
/* Allocate _length number of bytes of free memory and returns the
 address of the allocated portion. Returns 0 when out of memory. */
/* This preliminary implementation simply hands the call over the
 the C standard library!
 Of course this needs to be replaced by your implementation.
 */
{
    if((_length + sizeof(BlockHeader) > tms)){
      return NULL;
    }
    uint adjSize = _length + sizeof(BlockHeader);
    uint rqrMemr;
    if(pow(2,ceil(log2(adjSize))) < bbs){
        rqrMemr = bbs;
    }
    else{
        rqrMemr = pow(2,ceil(log2(adjSize)));
    }

    uint index = log2(tms/rqrMemr);
    
    for(int i = index; i >= 0; i--){
        if (freeList.at(i)->getHead() != NULL) {
            if (freeList.at(i)->getHead()->blockSize == rqrMemr) {
                BlockHeader* blck1 = freeList.at(i)->getHead() ;
                freeList.at(i)->remove(blck1);
                char* allocFinal = (char*)blck1;
                return ((char*)(allocFinal + sizeof(BlockHeader)));
            }
            else{
                split((char*)freeList.at(i)->getHead());
                i += 2;
            }
        }
    }
    return NULL;
}

int BuddyAllocator::free(char* _a)
/* Frees the section of physical memory previously allocated
 using â€™my_mallocâ€™. Returns 0 if everything ok. */
{
    char* _b = _a - sizeof(BlockHeader);
    char* _c = getbuddy(_b);
    BlockHeader* index = (BlockHeader*)(_b);
    int i = log2(tms/index->blockSize);
    
    for(int x = i; x > 0; x--)
    {
        BlockHeader* temp1 = (BlockHeader*)(_b);
        BlockHeader* temp2 = (BlockHeader*)(_c);
        if(temp1->blockSize == temp2->blockSize)
        {
            if(temp1->free && temp2->free){
                _b = (merge(_b,_c));
            }
        }
    }
    return 0;
}


char* BuddyAllocator::split (char* block){
    BlockHeader* temp = (BlockHeader *)block;
    BlockHeader *temp2 = (BlockHeader *)(block + temp->blockSize/2);
    int indexList = log2(tms/temp->blockSize);
    freeList.at(indexList)->remove(temp);
    
    temp->blockSize /= 2;
    indexList += 1;
    temp2->blockSize = temp->blockSize;
    
    freeList.at(indexList)->insert(temp);
    freeList.at(indexList)->insert(temp2);
    
    return (char*)temp2;
}

char* BuddyAllocator::merge(char* block1, char* block2)
{
    if((arebuddies(block1,block2)))
    {
        BlockHeader* temp1 = (BlockHeader*)(block1);
        BlockHeader* temp2 = (BlockHeader*)(block2);
        if((temp1->free && temp2->free) && temp1->blockSize == temp2->blockSize)
        {
            int temp = log2(tms/(temp1->blockSize));
            freeList.at(temp)->remove(temp1);
            freeList.at(temp)->remove(temp2);
            if(block1 < block2)
            {
                temp1->blockSize *= 2;
                int insert = log2(tms/(temp1->blockSize));
                temp1->free = true;
                freeList.at(insert)->insert(temp1);
                return (char*)temp1;
                
            }
            else
            {
                temp2->blockSize *= 2;
                int insert = log2(tms/(temp1->blockSize));
                temp2->free = true;
                freeList.at(insert)->insert(temp2);
                return (char*)temp2;
            }
            
        }
    }
    else
    {
        return NULL;
    }
    
    return NULL;
}




void BuddyAllocator::debug ()
{
    
}

