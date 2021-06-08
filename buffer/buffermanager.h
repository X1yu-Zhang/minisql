#ifndef _BUFFERMANAGER_H_
#define _BUFFERMANAGER_H_
#define BLOCK_SIZE 4096
#define BLOCK_NUMBER 100
#include <string>
#include <fstream>
#include <iostream>
#include <ctime>
using namespace std;
class Block;
class File{
    string filename;
    int type;
    bool pin;
    Block * head;
    File * next;
    File * pre;
    friend class buffermanager;
    friend class Block;
};
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
    void SetUsingSize(int size);
};
class buffermanager
{
private:
    Block block_pool [BLOCK_NUMBER];
    int total_block;
public:
    File * FileHead;
    buffermanager():FileHead(NULL),total_block(0){};
    ~buffermanager();
    File * GetFile( string table_name, int type, bool pin = false );
    Block * GetBlock( File * file, Block * position );
    Block * GetNextBlock( File * file , Block * position);
    Block * GetBlockByNum( File * file , int offsetNum );
    Block * GetReplaceBlock();
    Block * GetBlockHead( File * file );
    void DeleteFile( File * file );
    void ShowInfo(Block * tmp);
    void WriteBackAll();
};

#endif