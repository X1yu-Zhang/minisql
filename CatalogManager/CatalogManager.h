#ifndef _CATALOGMANAGER_H_
#define _CATALOGMANAGER_H_
#include <string>
#include "tuple.h"
using namespace std;
class CatalogManager
{
private:
    /* data */
public:
    CatalogManager(/* args */);
    ~CatalogManager();
    bool hasTable( string table_name );
    bool hasAttribute( string table_name , string attr_name );
    Attribute getAttribute( string table_name );
    void showTable( string table_name );
};
#endif