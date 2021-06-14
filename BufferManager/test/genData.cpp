#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
int main(){
    fstream file("../data/record/all_int.db" , ios::in | ios :: out | ios :: binary | ios::trunc);
    cout << file.is_open() << endl;
    file.clear();
    for(int i = 0 ; i < 1025 ; i ++ ){
        char * t = (char *) & i;
        for( int j = 0 ; j < 4 ;j++ ){
            file << t[j];
        }
    }
}