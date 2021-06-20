#ifndef _FILE_H_
#define _FILE_H_
#include <string>
#include <fstream>
using namespace std;
#include "Block.h"
struct RecordFreeListNode{
    struct RecordFreeListNode * next;
    int BlockNum;
    int offset;
};
typedef struct RecordFreeListNode * RecordFreeList;
typedef struct RecordFreeListNode FreeListNode;
class Block;
class File{
public:
    string filename;
    int type;
    bool pin;
    RecordFreeList freelist;
    Block * head;
    File * next;
    File * pre;
    File(string _filename,int _type):filename(_filename),type(_type)\
    ,pin(false),head(NULL),next(NULL),pre(NULL),freelist(NULL){};
    bool ReadFreeList();
    void WriteFreeList();
    void ShowFreeList();
    void AppendFreeList( int offsetNum , int offset );
};

#endif