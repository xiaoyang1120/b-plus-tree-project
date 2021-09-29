#include "record.h"

using namespace std;

Record::Record(char id[10], float r, int n) {
    this->isValid = true;
    this->tconst = id;
    this->averageRating = r;
    this->numVotes = n;
}

size_t Record::getSize(){
    return sizeof(bool) + sizeof(char[10]) +sizeof(float) + sizeof(int);
}

void Record::setDelete(){
    this->isValid = false;
}