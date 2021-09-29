#include "memory_pool.h"
#include "types.h"

#include <iostream>
#include <vector>
#include <tuple>
#include <cstring>

using namespace std;

MemoryPool::MemoryPool(size_t totalPoolSize, size_t blockSize) {
    this->totalPoolSize = totalPoolSize;
    this->blockSize = blockSize;
    this->totalBlockSizeUsed = 0;
    this->totalRecordSizeUsed = 0;
    this->numAllocated = 0;

    this->pool = operator new(totalPoolSize);
    std::memset(pool, '\0', totalPoolSize);
    this->block = nullptr;
    this->blockSizeUsed = 0;

    this->blocksAccessed = 0;
}

bool MemoryPool::allocateBlock(){
    if (totalBlockSizeUsed + blockSize <= totalPoolSize) {
        totalBlockSizeUsed += blockSize;
        block = (char *)pool + numAllocated * blockSize;
        blockSizeUsed = 0;
        numAllocated += 1;
        return true;
    } else {
        return false;
    }
}

Address MemoryPool::allocate(size_t size) {
    if (size > blockSize) {
        throw invalid_argument("requested size exceeded block size");
    }

    if (numAllocated == 0 || (blockSizeUsed + size > blockSize))
    {
        bool isSuccessful = allocateBlock();
        if (!isSuccessful) {
            throw std::logic_error("Failed to allocate new block!");
        }
    }

    short int index = blockSizeUsed;

    blockSizeUsed += size;
    totalRecordSizeUsed += size;

    Address recordAddress = {block, index};

    return recordAddress;
}

void *MemoryPool::loadRecord(Address address, size_t size)
{
    void *mainMemoryAddress = operator new(size);
    memcpy(mainMemoryAddress, (char *)address.blockAddress + address.index, size);

    // Update blocks accessed
    blocksAccessed++;

    return mainMemoryAddress;
}

Address MemoryPool::saveRecord(void *itemAddress, size_t size) {
    Address diskAddress = allocate(size);
    memcpy((char *)diskAddress.blockAddress + diskAddress.index, itemAddress, size);
    
    blocksAccessed++;

    return diskAddress;
}

MemoryPool::~MemoryPool(){};
