#include "Block.h"
Block :: Block():filename(""),file(NULL),dirty(false),\
pin(false),offsetNum(-1),UsingSize(0),\
time(0),next(NULL),pre(NULL),end(false),data(new char[BLOCK_SIZE]()){};
Block :: ~Block(){
    if ( this->dirty && this->offsetNum != -1 ) WriteBack();
    delete data;
}
void Block :: clear(){
	memset(data, 0, BLOCK_SIZE);
	dirty = false;
	file = NULL;
	next = NULL;
	offsetNum = -1;
	pre = NULL;
	time = 0;
	UsingSize = 0;
}
void Block :: SetPin(){
    pin = true;
}
void Block :: ClearPin(){
    pin = false;
}
bool Block :: IsEnd(){
    return end;
}
void Block :: SetDirty(){
    dirty = true;
}
void Block :: ClearDirty(){
    dirty = false;
}
void Block :: SetClock(){
    time ++ ;
}
void Block :: ReadIn(){
    string filename = this->filename;
    string filetype = ( this->file->type?"index" : "record");
    string fpath = "./data/"+filetype+"/"+filename+".db";
	fstream file(fpath, ios::in | ios::out | ios :: binary);
    if ( !file.is_open() ){
        this->end = true;
        return ;
    }
    int offset = this->offsetNum * BLOCK_SIZE;	
	file.seekp(offset, ios::beg);		
    if( this->data == NULL )		
	    this->data = new char[BLOCK_SIZE]();
	file.read( this->data, BLOCK_SIZE );
    if( file.eof() ) this->end = true;
    UsingSize = file.gcount();
    file.close();
}
int Block:: GetUsingSize(){
    return UsingSize;
}
void Block :: WriteBack(){
    string filename = this->filename;
    string filetype = ( this->file->type?"index" : "record" );
    string fpath = "./data/"+filetype+"/"+filename+".db";
	fstream file(fpath, ios::in | ios::out | ios :: binary);
    int offset = this->offsetNum * BLOCK_SIZE;		
	file.seekp(offset, ios::beg);	
	file.write( this->data, this->UsingSize );
	file.close();
}
void Block :: SetUsingSize( int size ){
    UsingSize = size;
}
void Block :: write(int offset, const char * data, int length){
    memcpy( this->data+offset , data, length);
    SetDirty();
}
int Block :: GetBlockOffsetNum(){
    return offsetNum;
}
char * Block :: GetContent(){
    return this->data;
}
void Block :: SetNext( Block * next ){
    this->next = next;
}
void Block :: SetOffsetNum( int num ){
    this->offsetNum = num;
}
void Block :: SetEnd( bool end ){
    this->end = end;
} 