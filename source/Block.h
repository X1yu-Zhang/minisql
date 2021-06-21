#ifndef _BLOCK_H_
#define _BLOCK_H_

#define BLOCK_SIZE 4096
#define BLOCK_NUMBER 100
#include <cstring>
#include <iostream>
#include <ctime>
#include <fstream>
#include<vector>
#include <string>
using namespace std;
#include "File.h"
class File;
class Block
{
private:
    string filename;
    File * file;
    bool dirty;
    bool end;
    bool pin;
    int offsetNum;
    int UsingSize;
    int time;
    Block * next;
    Block * pre;
    char *data;
    friend File;
public:
    Block();
    ~Block();
    void clear();
    friend class BufferManager;
    void SetPin();
    void ClearPin();
    void SetDirty();
    void ClearDirty();
    void SetClock();
    void WriteBack();
    void ReadIn();
    void write( int offset, const char * data,int length );
    void SetUsingSize(int size);
    int GetUsingSize(); 
    void SetNext( Block * next );
    int GetBlockOffsetNum();
    void SetOffsetNum( int num );
    void SetEnd( bool end );
    char * GetContent();
    bool IsEnd( );
};
#endif