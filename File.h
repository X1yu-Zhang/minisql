#ifndef _FILE_H_
#define _FILE_H_
#include <string>
using namespace std;
#include "Block.h"
#include "buffermanager.h"
class Block;
class File{
public:
    string filename;
    int type;
    bool pin;
    Block * head;
    File * next;
    File * pre;
    File(string _filename,int _type):filename(_filename),type(_type)\
    ,pin(false),head(NULL),next(NULL),pre(NULL){};
};
#endif