#ifndef _BufferManager_H_
#define _BufferManager_H_
#include <string>
#include <iostream>
#include "../File.h"
#include "../Block.h"
using namespace std;
class BufferManager
{
private:
    Block block_pool [BLOCK_NUMBER];
    int total_block;
public:
    File * FileHead;
    BufferManager():FileHead(NULL),total_block(0){};
    ~BufferManager();
    File * GetFile( string table_name, int type );

    Block * GetNextBlock( File * file , Block * position);
    Block * GetBlockByNum( File * file , int offsetNum );
    Block * GetEmptyBlock();
    Block * GetBlockHead( File * file );
    Block * GetBlock( File * file, Block * position );
    Block * GetIndexBlock( File * file );
    Block * GetReplaceBlock();
    bool DeleteRecord( Block * b, int offset );
    void DeleteFileFromList( string filename );
    void CloseFile( File * file );
    void ShowInfo(Block * tmp);
    void WriteBackAll();
};

#endif