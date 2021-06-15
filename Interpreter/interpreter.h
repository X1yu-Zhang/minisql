#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "../API/API.h"
#include "../exception.h"
using namespace std;
#include <sstream>std::stringstream ss;

using namespace std;
class Interpreter {
public:
	Interpreter();
	void getQuery();
	void EXEC();
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

private:
	string query;
	int getType(int pos, int& end_pos);//������������
	int getBits(int num);//���������ĳ���
	int getBits(float num);//����С������
	string getRelation(int pos, int& end_pos);//�����߼���ϵ
	string getWord(int pos, int& end_pos);//���ص���
	string getLower(string s, int p);//��Сд������ʶ��

};

string trim(string s);

//ʵ��string��int��float������ת��
template <class T>
T stringToNum(const string& str)
{
	istringstream iss(str);
	T num;
	iss >> num;
	return num;
}

#endif 
