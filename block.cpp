#include "block.h"
#include "record.h"

using namespace std;

Block::numOfBlocks = 0;

int DataBlock::maxNumOfRecords(int bsize){
    size_t recordSize = Record::getSize();
    return (bsize - 3 * sizeof(int)) / recordSize;
} 

DataBlock::DataBlock(int blockSize){
    this->numOfRecords = 0;
    this->numOfBlocks ++;
};
