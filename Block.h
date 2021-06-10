#ifndef _BLOCK_H_
#define _BLOCK_H_

#define BLOCK_SIZE 4096
#define BLOCK_NUMBER 100
#include <ctime>
#include <fstream>
#include <string>
using namespace std;
#include "File.h"
class Block
{
private:
    string filename;
    File * file;
    bool dirty;
    bool pin;
    bool end;
    int offsetNum;
    int UsingSize;
    clock_t time;
    Block * next;
    Block * pre;
    char *data;
public:
    Block();
    ~Block();
    void init();
    friend class buffermanager;
    void SetPin();
    void ClearPin();
    void SetDirty();
    void ClearDirty();
    void SetClock();
    void WriteBack();
    void ReadIn();
    void write(int offset, const char * data,int length);
    void SetUsingSize(int size);
};
#endif