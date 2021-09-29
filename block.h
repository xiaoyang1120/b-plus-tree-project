#ifndef BLOCK_H
#define BLOCK_H

#include "record.h"

class Block
{
protected:
    int blockId;
public:
    static int numOfBlocks;
};

class DataBlock: Block
{
public:
    int numOfRecords;
    std::list<Record> recordList;
public:
    int maxNumOfRecords();
    //constructor
    DataBlock(int blockSize);

};

#endif