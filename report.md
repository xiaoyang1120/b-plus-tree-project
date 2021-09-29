Design of the storage component, including: how each data item is
stored as a field, how fields are packed into a record, and how records are
packed into a block. It is suggested to use some figures to illustrate the
designs and include the size information of fields and records.

We use fixed length and fixed format for the data items. 
tconst (char[10]): 10 bytes
averageRating (float): 4 bytes
numVotes (integer): 4 bytes
struct record

define 2 types of block storage,
class DataBlock: numOfRecords, array of RecordSlot{bool, Record(18B)}
@update: directly add the boolean flag in Record class.
when building a data block object, we calculate the max number of records it can store in advance. We allocate array-like slots beforehand.
@update: Record objects are maintained in a DataBlock object as a list.

TODO: 
c++ memory allocation;
define Record struct, RecordSlot class, DataBlock class(including maxNumOfRecords function)
@update:change to record class with a bool variable
Block class have a static int c=variable numOfBlocks
B+ tree(root node, non leaf node, leaf node)
BPtreeBlock/Node structure: calculation of N
    leaf node: numOfKeys, pointer array(store pointers to DataBlock), key array, pointer to next leaf
    non leaf node: numOfKeys, pointer array (size N+1), key array(size N)
    key: fixed size(4B); pointer(8B, depend on machine)
    tree {*root}. When only 1 level, root points to the only leaf node.
BPTree Insertion/deletion/search

block size 100B or 500B