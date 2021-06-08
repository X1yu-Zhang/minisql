#include <iostream>
#include <fstream>
using namespace std;
int main(){
    fstream file("./record/data.txt",ios::in | ios :: out );
    cout << file.is_open();
    return 0;
}