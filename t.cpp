#include <iostream>
using namespace std;
int main(){
    char  b[] = "1111";
    int * a = (int *)b;
    printf("%x",*a);
}