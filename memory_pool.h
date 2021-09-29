#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <vector>
#include <unordered_map>
#include <tuple>

using namespace std;

struct Address
{
  void *blockAddress;
  short int index;
};

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

    void *pool;
    void *block;
public:
    // constructor
    MemoryPool(size_t totalPoolSize, size_t blockSize);

    bool allocateBlock();

    Address allocate(size_t size);

    bool deallocate(Address address, size_t size);

    void *loadRecord(Address address, size_t size);
    
    Address saveRecord(void *itemAddress, size_t size);

    size_t getTotalPoolSize() {return totalPoolSize;}

    size_t getBlockSize() {return blockSize;}

    size_t getTotalBlockSizeUsed() {return totalBlockSizeUsed;}

    size_t getTotalRecordSizeUsed() {return totalRecordSizeUsed;}

    int getNumAllocated() {return numAllocated;}

    int getBlockSizeUsed() {return blockSizeUsed;}

    ~MemoryPool();
};

#endif