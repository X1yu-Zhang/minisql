#ifndef _CATALOGMANAGER_H_
#define _CATALOGMANAGER_H_
#include <string>
#include <fstream>
#include <map>
#include "../table.h"
#include "../tuple.h"
using namespace std;
class CatalogManager
{
private:
    map<string , Table > TABLESET;
    map<string , string> INDEXSET;
public:
    CatalogManager(/* args */);
    ~CatalogManager();
    Table& GetTable( string table_name );
    bool CreateTable( Table & t );
    bool DeleteTable( string name );
    bool DeleteIndex( string name );
    bool FindIndex( string name );
    bool FindTable( string name );
};
#endif