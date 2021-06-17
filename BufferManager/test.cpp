#include "buffermanager.h"
#include <iostream>
int main(){
    BufferManager bm;
    bm.GetFile("all_int",0);
    cout << "------------------------------------------"<< endl;
    bm.FileHead->ShowFreeList();
    Block * tmp = bm.GetBlock(bm.FileHead,NULL);
    cout << bm.DeleteRecord( tmp , 0 ) << endl;
    tmp = bm.GetNextBlock(bm.FileHead,tmp);
    cout << bm.DeleteRecord( tmp , 0 ) << endl;
    cout <<" temp == NULL : " << (tmp == NULL) << endl;
    cout << "------------------------------------------"<< endl;
    bm.FileHead->ShowFreeList();
}
/*
10 : A0 00 00 00  
*/