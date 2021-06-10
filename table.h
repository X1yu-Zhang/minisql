#ifndef _TABLE_H_
#define _TABLE_H_
#include "attribute.h"
#include "File.h"
#include <string>
#include <vector>
using namespace std;
class table: public File{
private:
    vector<attribute> attribute_list;
    int record_size;
public:
    table(string name,vector<attribute> list);
};
#endif 