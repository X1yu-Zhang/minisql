#include <iostream>

using namespace std;


int main(){
    for(int i = 0 ; i < 1000 ; i ++ ){
        cout << "insert into t1 values ( " << i << " , '" << i << "'," << i << ", " << i <<");" << endl; 
    }
    cout <<"quit;" << endl;
}