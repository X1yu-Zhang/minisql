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
	//���ڶ�������ѯʱ��and�����ϲ�
	Table unionTable(Table& table1, Table& table2, string target_attr, Where where);
	//���ڶ�������ѯʱ��or�����ϲ�
	Table joinTable(Table& table1, Table& table2, string target_attr, Where where);

private:
	RecordManager & record;
	CatalogManager & catalog;
};

//���ڶ�vector��sortʱ����
bool sortcmp(const Tuple& tuple1, const Tuple& tuple2);
//���ڶ�vector�Ժϲ�ʱ������
bool calcmp(const Tuple& tuple1, const Tuple& tuple2);
bool isSatisfied(Tuple& tuple, int target_attr, Where where);

#endif