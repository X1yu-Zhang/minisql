#include "BufferManager.h"
File * BufferManager :: GetFile( string table_name, int type ){
    File * ret = NULL;
    File * tail = NULL;
    if( FileHead != NULL ){
        for ( File * tmp = FileHead; tmp ; tmp = tmp->next ){
            if( tmp->next == NULL )tail = tmp;
            if( tmp->filename == table_name && tmp->type == type ){
                ret = tmp;
                break;
            }
        }
    }
    if( !ret ){
        if( FileHead == NULL )
            FileHead = ret = new File(table_name, type);
        else{
            ret = new File(table_name , type);
            ret->pre = tail;
            tail->next = ret;
        }
        ret->ReadFreeList();
    }
    return ret;
}
BufferManager :: ~BufferManager(){
    File * next;
    for(File * tmp = FileHead ; tmp ; tmp = next){
        next = tmp->next;
        CloseFile(tmp);
    }
}

Block * BufferManager :: GetReplaceBlock(){
    clock_t max_clock = clock()*1.0 / 1000;
    Block * ret = NULL;
    for(int i = 0 ; i < BLOCK_NUMBER ; i++ ){
        if( block_pool[i].time < max_clock && !block_pool[i].pin && block_pool[i].offsetNum != -1 ){
            max_clock = block_pool[i].time;
            ret = & block_pool[i];
        }
    }
    return ret;
}
Block * BufferManager :: GetBlock( File * file, Block * position ){
    string filename = file->filename;
    Block * ret = GetEmptyBlock();
    if( position && !position->next ){
        ret->pre = position;
        position->next = ret;
        ret->offsetNum = position->offsetNum+1;
    }else if( position && position->next ){
        ret->pre = position;
        ret->next = position->next;
        position->next->pre = ret;
        position->next = ret;
        ret->offsetNum = position->offsetNum + 1;
    }else{
        ret->offsetNum = 0;
        if( file->head ){
            file->head->pre = ret;
            ret->next = file->head;
        }
        file->head = ret;
    }
    ret->SetClock();
    ret->file = file;
    ret->filename = filename;
    ret->ReadIn();
    return ret;
}
void BufferManager :: WriteBackAll(){
    for( File * tmp = FileHead;tmp; tmp = tmp->next){
        Block * next;
        for(Block * btmp = tmp->head; btmp ; btmp = next){
            next = btmp->next;
            if( next->dirty )
                btmp->WriteBack();
            btmp->clear();
        }
    }
}
Block* BufferManager :: GetBlockHead( File * file ){
    if( file->head && file->head->offsetNum == 0 ){
        return file->head;
    }else{
        return GetBlock( file, NULL );
    }
}
Block * BufferManager :: GetNextBlock( File * file, Block * position ){
    Block * ret;
    if( position->next ){
        if( position->offsetNum == position->next->offsetNum - 1){
            ret = position->next;
        }else{
            ret = GetBlock(file,position);
        }
    }else{
        if( position->end ) ret = NULL;
        else{
            ret = GetBlock(file,position);
        }
    }
    return ret;
}

Block * BufferManager :: GetBlockByNum( File * file , int offsetNum ){
    Block * ret;
    if( offsetNum == 0 )
        ret = file->head;
    else{
        ret = GetBlockHead(file);
        for(int i = 0; i < offsetNum ; i ++ ){
            ret = GetNextBlock(file , ret);
        }
    }
    return ret;
}
void converse( char t ){
    if( t > 9) t += 'A' - 10;
    else t += '0';
    cout << t;
}
void BufferManager :: ShowInfo( Block * tmp ){
    for(Block * btmp = tmp ; btmp ; btmp = btmp -> next){
        cout << "Block Num      : " << btmp->offsetNum << endl;
        cout << "Block UsingSize: " << btmp->UsingSize << endl;
        for(int i = 0;i < btmp->UsingSize  ; i++ ){
            unsigned char t = btmp->data[i];
            converse(t/16);
            converse(t%16);
            cout << " " ;
        }
        cout << endl;
    }
}
Block * BufferManager :: GetEmptyBlock(){
    Block * ret;
    if ( total_block < BLOCK_NUMBER ){
        for(int i = 0 ;i < BLOCK_NUMBER ; i ++ ){
            if( block_pool[i].offsetNum == -1 ){
                ret = &block_pool[i];
                total_block++;
                break;
            }
        }
    }else {
        int to_be_replaced;
        Block* tmp = GetReplaceBlock();
        if( tmp->next ) tmp->next->pre = tmp->pre;
        if( tmp->pre ) tmp->pre->next = tmp->next;
        if( FileHead->head == tmp ) FileHead->head = tmp->next;
        if( tmp->dirty )
            tmp->WriteBack();
        tmp->clear();
        ret = tmp;
    }
    return ret;
}
void BufferManager :: CloseFile( File * file ){
    file->WriteFreeList();
	Block * tmp = file->head ,* next;
    for( ; tmp ; tmp = next){
        next = tmp->next;
        if( tmp->dirty )
            tmp->WriteBack();
        total_block -- ;
        tmp->clear();
    }
	file->head = NULL;
	file->filename = "";
	file->next = NULL;
	file->pre = NULL;
	delete file;
}
void BufferManager :: DeleteFileFromList( string  filename ){
    if( FileHead == NULL )return ;
    for ( File * tmp = FileHead; tmp ; tmp = tmp->next ){
        if( tmp->filename == filename ){
            if( tmp->next ){
                tmp->next->pre = tmp->pre;
            }
            if( tmp->pre ){
                tmp->pre->next = tmp->next;
            }else{
                FileHead = tmp->next;
            }
            CloseFile(tmp);
            return ;
        }
    }
}

bool BufferManager :: DeleteRecord( Block * b, int offset ){
    char tmp = 1;
    if ( b->data[offset] == 1 ){
        return false;
    }
    b->write(offset , &tmp , 1);
    File * file = b->file;
    RecordFreeList now = new FreeListNode;
    now->next = NULL;
    now->BlockNum = b->offsetNum;
    now->offset = offset;
    if ( file->freelist == NULL )
        file->freelist = now;
    else{
        now->next = file->freelist;
        file->freelist = now;
    }
    return true;
}