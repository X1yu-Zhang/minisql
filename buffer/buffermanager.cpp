#include "buffermanager.h"
Block :: Block():filename(""),file(NULL),dirty(false),\
pin(false),end(false),offsetNum(-1),UsingSize(0),\
time(clock()),next(NULL),pre(NULL),data(NULL){};
Block :: ~Block(){
    delete data;
}
void Block :: init(){
    filename = "";
    file = NULL;
    dirty = false;
    pin = false;
    end = false;
    offsetNum = -1;
    clock_t time = clock();
    next = pre = 0;
    if( data != NULL ) delete data;
    data = NULL;
}
void Block :: SetPin(){
    pin = true;
}
void Block :: ClearPin(){
    pin = false;
}
void Block :: SetDirty(){
    dirty = true;
}
void Block :: ClearDirty(){
    dirty = false;
}
void Block :: SetClock(){
    time = clock();
}
void Block :: ReadIn(){
    string filename = this->filename;
    string filetype = ( this->file->type?"index" : "record");
    string fpath = "./"+filetype+"/"+filename;
	fstream file(fpath, ios::in | ios::out);
    int offset = this->offsetNum * BLOCK_SIZE;		
	file.seekp(offset, ios::beg);		
    if( this->data != NULL ) delete this->data;			
	this->data = new char[BLOCK_SIZE]();	
    string temp = data;
    this->UsingSize = temp.length();	
	file.read(this->data, BLOCK_SIZE);		
	file.close();
}
void Block :: WriteBack(){
    string filename = this->filename;
    string filetype = ( this->file->type?"record":"index" );
    string fpath = "../"+filetype+"/"+filename;
	fstream file(fpath, ios::in | ios::out);
    int offset = this->offsetNum * BLOCK_SIZE;		
	file.seekp(offset, ios::beg);			
    string temp = this->data;
	file.write( this->data, this->UsingSize );
	file.close();
}
void Block :: SetUsingSize( int size ){
    UsingSize = size;
}
File * buffermanager :: GetFile( string table_name, int type,bool pin ){
    File * ret;
    if( FileHead != NULL ){
        for ( File * tmp = FileHead; tmp ; tmp = tmp->next ){
            if( tmp->filename == table_name && tmp->type == type ){
                ret = tmp;
                break;
            }
        }
    }else{
        FileHead = new File;
        FileHead->filename = table_name;
        FileHead->type = type;
        FileHead->pin = pin;
        FileHead->head = NULL;
        FileHead->next = FileHead->pre = NULL;
        ret = FileHead;
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
    Block * ret;
    if ( total_block == 0 ){
        ret = &block_pool[total_block++];
    }else if ( total_block < BLOCK_NUMBER ){
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