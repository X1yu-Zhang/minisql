#include "buffermanager.h"
File * buffermanager :: GetFile( string table_name, int type ){
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
    }
    return ret;
}
buffermanager :: ~buffermanager(){
    File * next;
    for(File * tmp = FileHead ; tmp ; tmp = next){
        next = tmp->next;
        delete tmp;
    }
}

Block * buffermanager :: GetReplaceBlock(){
    clock_t max_clock = clock();
    Block * ret = NULL;
    for(int i = 0 ; i < BLOCK_NUMBER ; i++ ){
        if( block_pool[i].time < max_clock && !block_pool[i].pin && block_pool[i].offsetNum != -1 ){
            max_clock = block_pool[i].time;
            ret = & block_pool[i];
        }
    }
    return ret;
}
Block * buffermanager :: GetBlock( File * file, Block * position ){
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
void buffermanager :: WriteBackAll(){
    for( File * tmp = FileHead;tmp; tmp = tmp->next){
        Block * next;
        for(Block * btmp = tmp->head; btmp ; btmp = next){
            next = btmp->next;
            btmp->WriteBack();
            btmp->init();
        }
    }
}
Block* buffermanager :: GetBlockHead( File * file ){
    if( file->head && file->head->offsetNum == 0 ){
        return file->head;
    }else{
        return GetBlock( file, NULL );
    }
}
Block * buffermanager :: GetNextBlock( File * file, Block * position ){
    Block * ret;
    if( position->next ){
        if( position->offsetNum == position->next->offsetNum - 1){
            ret = position->next;
        }else{
            ret = GetBlock(file,position);
        }
    }else{
        if( position->end ) position->end = false;
        ret = GetBlock( file , position );
    }
    return ret;
}

Block * buffermanager :: GetBlockByNum( File * file , int offsetNum ){
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
void buffermanager :: ShowInfo(Block * tmp){
    for(Block * btmp = tmp ; btmp ; btmp = btmp -> next){
        cout << btmp->data << endl;
    }
}
Block * buffermanager :: GetEmptyBlock(){
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
        tmp->WriteBack();
        tmp->init();
        ret = tmp;
    }
    return ret;
}
