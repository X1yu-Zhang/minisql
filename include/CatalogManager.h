#ifndef _CATALOGMANAGER_H_
#define _CATALOGMANAGER_H_
#include <string>
#include <fstream>
#include <map>
#include "table.h"
#include "tuple.h"
#include "IndexManager.h"
using namespace std;
class CatalogManager
{
private:
    map<string , Table > TABLESET;
    map<string , string> INDEXSET;// index_name to table_namef
    IndexManager &im;
public:
    CatalogManager(IndexManager &i);
    ~CatalogManager();
    Table& GetTable( string table_name );
    bool CreateTable( Table & t );
    bool DropTable( string name );
    bool CreateIndex( string name, string table_name, string attr_name );
    bool DeleteIndex( string name );
    bool HasIndex( string name );
    bool HasTable( string name );
    bool HasAttribute(string table_name, string attr_name);
    Attribute getAttribute( string table_name );

};
#endif