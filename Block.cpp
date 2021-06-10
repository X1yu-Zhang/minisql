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
    string fpath = "./data/"+filetype+"/"+filename;
    cout << fpath << endl;
	fstream file(fpath, ios::in | ios::out | ios :: binary);
    int offset = this->offsetNum * BLOCK_SIZE;	
	file.seekp(offset, ios::beg);		
    if( this->data == NULL )		
	    this->data = new char[BLOCK_SIZE + 1]();
    cout << file.is_open() << endl;
	file.read( this->data, BLOCK_SIZE );
    file.close();
}
void Block :: WriteBack(){
    string filename = this->filename;
    string filetype = ( this->file->type?"record":"index" );
    string fpath = "./data/"+filetype+"/"+filename;
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
void Block :: write(int offset, const char * data,int length){
    memcpy(this->data+offset , data , length);
    SetDirty();
}
char * Block :: FetchRecord( int index , int size ){
    char * ret = new char[size];
    int offset = index * size;
    memcpy(ret , data + offset , size);
    return ret;
}