#include <iostream>
#include <fstream>
using namespace std;
int main(){
    fstream file("../data/record/data2.txt" , ios::in | ios :: out | ios :: binary);
    cout << file.is_open() << endl;
    char temp[5];
    for(int i = 0 ; i < 10 ; i ++ ){
        file.read(temp , 4);
        cout << *(int *)temp <<endl;
    }
}
/*
0100 0000
*/