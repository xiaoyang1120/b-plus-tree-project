#include "b_plus_tree.h"
#include "memory_pool.h"
#include "tree_node.h"
#include "record.h"
#include "memory_pool.cpp"
#include "b_plus_tree.cpp"
#include "record.cpp"
#include "search.cpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <unordered_map>

#include <iostream>
#include <list>
#include <fstream>
#include <cstddef>
using namespace std;

int main()
{    
    int BLOCKSIZE = 500;
    MemoryPool disk(150000000, BLOCKSIZE);  // 150MB
    MemoryPool index(350000000, BLOCKSIZE); // 350MB

    
    // Creating the tree
    BPlusTree tree = BPlusTree(BLOCKSIZE, &disk, &index);
    cout << "max keys: " << tree.getMaxKeys() << endl;

    std::cout <<"Reading in data ... "<<endl;
    std::ifstream file("data/data.tsv"); // actual data
    // std::ifstream file("../data/testdata.tsv"); // testing data

    int recordNum = 0;

    // Insert data into database and populate list of addresses
    if (file.is_open())
    {
        std::string line;
        getline(file, line);

        while (getline(file, line))
        {
            if (recordNum % 50000 == 0) {
                cout << "Record " << recordNum << " Read" << endl;
            }
            //temporary struct Record
            stringstream linestream(line);
            string data;
            char* tconst = new char[10];
            float averageRating = 0;
            int numVotes = 0;

            //assigning temp.tconst value
            strcpy(tconst, line.substr(0, line.find("\t")).c_str());
            std::getline(linestream, data, '\t');

            //assigning temp.averageRating & temp.numVotes values
            linestream >> averageRating >> numVotes;

            Record* temp = new Record(tconst, averageRating, numVotes);

            //insert this record into the database
            Address tempAddress = disk.save(&temp, temp->getSize());

            //build the bplustree as we insert records
            tree.insert(temp->numVotes);

            //logging
            // cout << "Inserted record " << recordNum + 1 << " at block address: " << &tempAddress.blockAddress << " and offset " << &tempAddress.offset << endl;
            recordNum += 1;
        }
        file.close();
    }

    //tree.search(500, 500);

    cout << "Parameter n: " << tree.getMaxKeys() << endl;
    cout << "Number of nodes: " << tree.getNumOfNodes() << endl;
    cout << "Height: " << tree.getHeight() << endl;
    cout << "Content of root: " << endl;
    // for (int i = 0; i < tree.getRoot()->getNumOfKeys(); i++) {
    //     cout << tree.getRoot()->getKey(i) << " ";
    // }
    tree.displayNode(tree.getRoot());
    cout << endl;
    cout << "Content of first child node of root: " << endl;

    // for (int i = 0; i < tree.getRoot()->getPointer(0)->getNumOfKeys(); i++) {
    //     cout << tree.getRoot()->getPointer(0)->getKey(i) << " ";
    // }
    tree.displayNode(tree.getRoot()->getPointer(0));

    cout << endl;
    

    cout << "Block Size: " << BLOCKSIZE << endl;
    cout << "Record Number: " << recordNum << endl;
    cout << "Number of Blocks: " << disk.getNumAllocated() << endl;
    cout << "Total Disk Size Used: " << disk.getTotalBlockSizeUsed() << "B" << endl;
    
    int numUpd =0;
    int numDel =0;
    bool noMore=false;
    while(!noMore){
        tree.remove(500,numDel,numUpd,noMore);
    }
    cout << "Height: " << tree.getHeight() << endl;
    cout<<"Number of nodes updated: "<<numUpd<<endl;
    cout<<"Number of nodes deleted/merged: "<<numDel<<endl;


    cout << endl;
   cout << "Content of root: " << endl;
    // for (int i = 0; i < tree.getRoot()->getNumOfKeys(); i++) {
    //     cout << tree.getRoot()->getKey(i) << " ";
    // }
    tree.displayNode(tree.getRoot());
    cout << endl;
    cout << "Content of first child node of root: " << endl;

    // for (int i = 0; i < tree.getRoot()->getPointer(0)->getNumOfKeys(); i++) {
    //     cout << tree.getRoot()->getPointer(0)->getKey(i) << " ";
    // }
    tree.displayNode(tree.getRoot()->getPointer(0));

    cout<<endl;
    


   
    // int select = 0;
    // cout<<"Select Block Size:"<<endl;
    // do{
    //     cout<<"1. 100B"<<endl;
    //     cout<<"2. 500B"<<endl;
    //     cin>>select;
    // }
    // while(select!=1 && select!=2);

    //read data

    // ifstream infile("data.tsv");
    // string a, b, c;
    // while (infile >> a >> b >> c)
    // {
    //     // process tconst, avgRating and numVotes
    //     // cout<<a<<endl;
    //     // create record
	// // create a data block if there no data block or current data block is full
	// // insert record into data block
    // }


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
