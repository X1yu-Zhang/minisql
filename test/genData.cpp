#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
int main(){
    fstream file("./data/record/data.txt" , ios::in | ios :: out | ios :: binary);
    cout << file.is_open() << endl;
    for(int i = 0 ; i < 10 ; i ++ ){
        char * t = (char *) & i;
        cout << strlen(t)<<endl;
        for( int j = 0 ; j < 4 ;j++ ){
            file << t[j];
        }
    }
}