#ifndef _API_H_
#define _API_H_
#include <string>
#include "exception.h"
#include "table.h"
#include "RecordManager.h"
#include "CatalogManager.h"
#include "IndexManager.h"
class API {
public:
	API(RecordManager &rm, CatalogManager &cm):record(rm),catalog(cm){};
	~API();
	void selectRecord(string table_name, vector<string> DispAttr ,vector<string> target_attr, vector<Where> where);
	void deleteRecord(string table_name, vector<string> target_attr, vector<Where> where);
	void insertRecord(string table_name, Tuple& tuple);
	void createTable( string table_name, Attribute& attribute );
	void dropTable( string table_name );
	void createIndex( string table_name, string index_name, string attr_name );
	void dropIndex( string index_name );

private:
	RecordManager &record;
	CatalogManager &catalog;
	// IndexManager & im;
};


#endif