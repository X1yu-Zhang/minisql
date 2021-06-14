#ifndef _BUFFERMANAGER_H_
#define _BUFFERMANAGER_H_
#include <string>
#include <iostream>
#include "../File.h"
#include "../Block.h"
using namespace std;
class buffermanager
{
private:
    Block block_pool [BLOCK_NUMBER];
    int total_block;
public:
    File * FileHead;
    buffermanager():FileHead(NULL),total_block(0){};
    ~buffermanager();
    File * GetFile( string table_name, int type );
    Block * GetBlock( File * file, Block * position );
    Block * GetNextBlock( File * file , Block * position);
    Block * GetBlockByNum( File * file , int offsetNum );
    Block * GetEmptyBlock();
    Block * GetReplaceBlock();
    Block * GetBlockHead( File * file );
    void DeleteFileFromList( string filename );
    void CloseFile( File * file );
    void ShowInfo(Block * tmp);
    void WriteBackAll();
};

#endif