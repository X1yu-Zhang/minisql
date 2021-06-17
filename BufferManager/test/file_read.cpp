#include <iostream>
#include <fstream>
using namespace std;
int main(){
    fstream file("../../data/record/all_int.db" , ios::in | ios :: out | ios :: binary);
    cout << file.is_open() << endl;
    char temp[5];
    for(int i = 0 ; i < 12 ; i ++ ){
        file.read(temp , 5);
        cout << (int)temp[0]<< " " << *(int *)(temp+1) << endl;
    }
}
/*
0100 0000
*/