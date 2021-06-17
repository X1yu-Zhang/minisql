#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
#define BLOCKSIZE 4096
int main(){
    fstream file("../../data/record/all_int.db" , ios::in | ios :: out | ios :: binary | ios::trunc);
    cout << file.is_open() << endl;
    file.clear();
    int flag = 1;
    for(int i = 0 ; i < 1024 ; i ++ ){
        char * t = (char *) & i;
        file << '\0';
        for( int j = 0 ; j < 4 ;j++ ){
            file << t[j];
        }
        if( (i+2) * 5 > BLOCKSIZE && flag ){
            flag = 0;
            int n = BLOCKSIZE - (i+1) * 5;
            while(n--) file << '\0';
        }
    }
}