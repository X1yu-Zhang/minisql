#include "buffermanager.h"
#include <iostream>
int main(){
    buffermanager bm;
    bm.GetFile("data2.txt",0);
    Block * tmp = bm.GetBlock(bm.FileHead,NULL);
    int t = 11;
    char * temp = (char*)&t;
    tmp->write(40, temp , 4);
    bm.ShowInfo(tmp);
}
/*
10 : 0A00 0000 "0A 00 00 00" 0A 00 00 00
*/