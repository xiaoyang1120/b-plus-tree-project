#include "b_plus_tree.h"
#include "memory_pool.h"
#include "memory_pool.cpp"
#include "b_plus_tree.cpp"
// #include "tree_node.cpp"

#include <iostream>
#include <list>
#include <fstream>
#include <cstddef>
using namespace std;

int main()
{
    BPlusTree bptree;
     // int numbers[12] = {1, 4, 7, 10, 17, 21, 31, 25, 19, 20, 28, 42};
     bptree.insert(1);
     bptree.insert(4);
     bptree.insert(7);
     bptree.insert(10);
     bptree.insert(17);
     bptree.insert(21);
     bptree.insert(31);
     bptree.insert(25);
     bptree.insert(19);
     bptree.insert(20);
     bptree.insert(28);
     bptree.insert(42);
     cout << "root: " << bptree.getRoot()->getKey(0) << endl;
    bptree.display(bptree.getRoot());
    int numU = 0;
    int numD = 0;
    //bptree.remove(4, numD, numU);
    bptree.remove(1, numD, numU);
    //bptree.remove(7, numD, numU);
    cout<<"num updates: "<<numU<<endl;
    cout<<"num delete/merge: "<<numD<<endl;
    cout << "root: " << bptree.getRoot()->getKey(0) << endl;
    bptree.display(bptree.getRoot());
    bptree.getFirstLeaf();

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
