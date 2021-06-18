#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "API.h"
#include "exception.h"
using namespace std;
#include <sstream>std::stringstream ss;

using namespace std;
class Interpreter {
public:
	Interpreter();
	void getQuery();
	void EXEC();

private:
	string query;
	int getType(int pos, int& end_pos);//返回数据类型
	int getBits(int num);//返回整数的长度
	int getBits(float num);//返回小数长度
	string getRelation(int pos, int& end_pos);//返回逻辑关系
	string getWord(int pos, int& end_pos);//返回单词
	string getLower(string s, int p);//变小写（便于识别）
	void Normalize();
	void EXEC_SELECT();
	void EXEC_DROP_TABLE();
	void EXEC_DROP_INDEX();
	void EXEC_CREATE_TABLE();
	void EXEC_CREATE_INDEX();
	void EXEC_INSERT();
	void EXEC_DELETE();
	void EXEC_SHOW();
	void EXEC_EXIT();
	void EXEC_FILE();
	void EXEC_HELP();
};

string trim(string s);

//实现string到int或float的类型转换
template <class T>
T stringToNum(const string& str)
{
	istringstream iss(str);
	T num;
	iss >> num;
	return num;
}

#endif 
