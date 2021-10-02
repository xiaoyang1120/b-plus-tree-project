#ifndef RECORD_H
#define RECORD_H

#include <iostream>

class Record{
public:
    char tconst[10];     // Movie unique identidier, a string of at most 10 chars
    float averageRating; // Average rating of the movie.
    int numVotes;        // Number of votes of the movie.
    bool isValid;       // True if the record is present and not deleted
public:
    //constructor, should init the slot
    Record();
    Record(char id[10], float r, int n);

    //utils
    size_t getSize();
    void setDelete();
};

#endif