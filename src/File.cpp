#include "File.h"

bool File :: ReadFreeList(){
    fstream file("./data/record/"+this->filename+"_FreeList.db", ios :: in );
    if( !file.is_open() ) return false;
    int n;
    file >> n;
    for(int i = 0 ; i < n ; i++ ){
        RecordFreeList tmp = new FreeListNode;
        tmp->next = NULL;
        file >> tmp->BlockNum >> tmp->offset;
        if( freelist == NULL ){
            freelist = tmp;
        }else{
            tmp->next = freelist;
            freelist = tmp;
        }
    } 
    return true;
}
void File :: WriteFreeList(){
    fstream file("./data/record/"+this->filename+"_FreeList.db", ios :: out );
    RecordFreeList now = freelist;
    int cnt = 0;
    while( now ){
        now = now->next;
        cnt++;
    }
    file << cnt << endl;
    RecordFreeList next;
    for( RecordFreeList now = freelist; now ; now = next){
        next = now->next;
        file << now->BlockNum << " " << now->offset << endl;
        delete now;
    }
}
void File :: ShowFreeList(){
    RecordFreeList now = freelist;
    while( now ){
        cout << now->BlockNum << " " << now->offset << endl;
        now = now->next;
    }
}
void File :: AppendFreeList( int OffsetNum , int Offset ){
    RecordFreeList node = new FreeListNode;
    node->BlockNum = OffsetNum;
    node->offset = Offset;
    node->next = NULL ;
    if ( freelist == NULL ){
        freelist = node;
    }else {
        node->next = freelist;
        freelist = node;
    }
}