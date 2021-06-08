#ifndef _ATTRIBUTE_H_
#define _ATTRIBUTE_H_
#include <string>
using namespace std;
class attribute{
public:
    string name;
    int type;
    int size;
    attribute(string a_name,int a_type,int a_size):name(a_name),type(a_type),size(a_size){};
};
#endif