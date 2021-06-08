#include "./buffer/buffermanager.h"
#include <iostream>
int main(){
    buffermanager bm;
    bm.GetFile("data.txt",0);
    Block * tmp = bm.GetBlock(bm.FileHead,NULL);
    bm.ShowInfo(tmp);
}