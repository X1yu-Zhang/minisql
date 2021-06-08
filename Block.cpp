#include "Block.h"
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