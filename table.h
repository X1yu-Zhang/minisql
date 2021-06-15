#ifndef TABLE_H
#define TABLE_H
#include <string>
#include <vector>
#include "tuple.h"
using namespace std;


class Table {
private:
    string title_;  //表名
    vector<Tuple> tuple_;  //存放所有的元组
    Index index_;  //表的索引信息
public:
    Attribute attr_;  //属性
    //构造函数
    Table() {};
    Table(string title, Attribute attr);
    Table(const Table& table_in);

    int addTuple(Tuple tuple_in); // 插入元祖，判断tuple_in 的格式是否符合table中属性格式
    int setIndex(short index, string index_name);  //插入索引，输入要建立索引的Attribute的编号，以及索引的名字，成功返回1失败返回0
    int dropIndex(string index_name);  //删除索引，输入建立的索引的名字，成功返回1失败返回0
    string getTitle();
    Attribute getAttr();
    vector <Tuple>& getTuple();//返回元祖
    Index getIndex();
    short gethasKey();//返回主码所在的属性位置
    void showTable(); //打印表属性和元祖
    void showTable(int limit);//显示table的部分数据
};

#endif