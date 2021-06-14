#include "buffermanager.h"
#include <iostream>
int main(){
    buffermanager bm;
    bm.GetFile("all_int",0);
    Block * tmp = bm.GetBlock(bm.FileHead,NULL);
    cout << endl;
    bm.ShowInfo(tmp);
    tmp = bm.GetNextBlock(bm.FileHead,tmp);
    bm.ShowInfo(tmp);
}
/*
10 : A0 00 00 00  
*/