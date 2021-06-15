#ifndef _API_H_
#define _API_H_
#include <string>
#include "table.h"

class API {
public:
	API();
	~API();
	Table selectRecord(string table_name, vector<string> target_attr, vector<Where> where, char operation);
	int deleteRecord(string table_name, string target_attr, Where where);
	void insertRecord(string table_name, Tuple& tuple);
	bool createTable(string table_name, Attribute attribute, int primary, Index index);
	bool dropTable(string table_name);
	bool createIndex(string table_name, string index_name, string attr_name);
	bool dropIndex(string table_name, string index_name);
private:
	//用于多条件查询时的and条件合并
	Table unionTable(Table& table1, Table& table2, string target_attr, Where where);
	//用于多条件查询时的or条件合并
	Table joinTable(Table& table1, Table& table2, string target_attr, Where where);

private:
	RecordManager record;
	CatalogManager catalog;
};

//用于对vector的sort时排序
bool sortcmp(const Tuple& tuple1, const Tuple& tuple2);
//用于对vector对合并时对排序
bool calcmp(const Tuple& tuple1, const Tuple& tuple2);
bool isSatisfied(Tuple& tuple, int target_attr, Where where);

#endif