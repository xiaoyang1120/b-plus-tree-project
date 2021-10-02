#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include "address.h"

#include <vector>
#include <unordered_map>
#include <tuple>

using namespace std;

class MemoryPool
{
private:
    size_t totalPoolSize;
    size_t blockSize;
    size_t totalBlockSizeUsed;
    size_t totalRecordSizeUsed;
    size_t blockSizeUsed;

    int numAllocated;
    int blocksAccessed;

    void *pool; // memory pool
    void *block; //block pointer
public:
    // constructor
    MemoryPool(size_t totalPoolSize, size_t blockSize);

    bool allocateBlock();

    Address allocate(size_t size);

    bool deallocate(Address address, size_t size);

    void *load(Address address, size_t size);
    
    Address save(void *itemAddress, size_t size);

    size_t getTotalPoolSize() {return totalPoolSize;}

    size_t getBlockSize() {return blockSize;}

    size_t getTotalBlockSizeUsed() {return totalBlockSizeUsed;}

    size_t getTotalRecordSizeUsed() {return totalRecordSizeUsed;}

    int getNumAllocated() {return numAllocated;}

    int getBlockSizeUsed() {return blockSizeUsed;}

    void setNumAllocated(int num) { this->numAllocated = num; }

    void setBlockAccessed(int num) { this->blocksAccessed = num; }

    ~MemoryPool();
};

#endif