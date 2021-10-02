#include "b_plus_tree.h"
#include "memory_pool.h"

#include <iostream>
#include <list>
#include <fstream>
using namespace std;

int main()
{
    int select = 0;
    cout<<"Select Block Size:"<<endl;
    do{
        cout<<"1. 100B"<<endl;
        cout<<"2. 500B"<<endl;
        cin>>select;
    }
    while(select!=1 && select!=2);

    //read data

    ifstream infile("data.tsv");
    string a, b, c;
    while (infile >> a >> b >> c)
    {
        // process tconst, avgRating and numVotes
        // cout<<a<<endl;
        // create record
	// create a data block if there no data block or current data block is full
	// insert record into data block
    }


    cout<<"Experiment 1"<<endl;
    cout<<"Number of blocks: "<<endl;
    cout<<"Size of database: "<<endl;
    cout<<endl;

    //create b+ tree
    //insert data into b+ tree

    cout<<"Experiment 2"<<endl;
    cout<<"N = "<<endl;
    cout<<"Number of blocks: "<<endl;
    cout<<"Maximum of keys in a block: "<<endl;
    cout<<"Maximum of pointers in a block: "<<endl;
    cout<<"Height of B+ tree: "<<endl;
    cout<<"First child node: "<<endl;
    cout<<"tconst: "<<endl;
    cout<<"Average Rating: "<<endl;
    cout<<"Number of Votes: "<<endl;
    cout<<endl;

    //retrieve movies with votes = 500
    cout<<"Experiment 3"<<endl;
    cout<<"Number of index nodes accessed: "<<endl;
    cout<<"Content of index nodes: "<<endl;
    cout<<"Number of data blocks accessed: "<<endl;
    cout<<"Content of data blocks: "<<endl;
    cout<<"Average rating: "<<endl;
    cout<<endl;

    //retrieve movies with votes between 30,000
    //and 40,000
    cout<<"Experiment 4"<<endl;
    cout<<"Number of index nodes accessed: "<<endl;
    cout<<"Content of index nodes: "<<endl;
    cout<<"Number of data blocks accessed: "<<endl;
    cout<<"Content of data blocks: "<<endl;
    cout<<"Average rating: "<<endl;
    cout<<endl;

    //delete movies with votes = 1000
    cout<<"Experiment 5"<<endl;
    cout<<"Number of index nodes deleted or merged: "<<endl;
    cout<<"Number of index nodes updated: "<<endl;
    cout<<"Height of B+ tree"<<endl;
    cout<<"Content of root node: "<<endl;
    cout<<"tconst: "<<endl;
    cout<<"Average Rating: "<<endl;
    cout<<"Number of Votes: "<<endl;
    cout<<"Content of first child node: "<<endl;
    cout<<"tconst: "<<endl;
    cout<<"Average Rating: "<<endl;
    cout<<"Number of Votes: "<<endl;


    return 0;
}
