#include "record.h"
#include <vector>
#include <iostream>
#include <cstring>

using namespace std;

Record::Record() {}

Record::Record(char id[10], float r, int n) {
    this->isValid = true;
    memcpy(this->tconst, id, 10);
    free(id);
    this->averageRating = r;
    this->numVotes = n;
}

size_t Record::getSize(){
    return sizeof(bool) + sizeof(char[10]) + sizeof(float) + sizeof(int);
}

void Record::setDelete(){
    this->isValid = false;
}