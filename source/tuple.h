#ifndef TUPLE_H
#define TUPLE_H
#include <vector>
#include <iostream>
using namespace std;

//用于where的判断 分别为小于，小于等于，等于，大于等于，大于，不等于
typedef enum {
    LESS,
    LESS_OR_EQUAL,
    EQUAL,
    GREATER_OR_EQUAL,
    GREATER,
    NOT_EQUAL
} WHERE;

//一个struct存放它的一条信息的类型和值
//用一个strunt嵌套一个union实现多种类型的转换
//type的类型：-1：int,0:float,1-255:string(数值为字符串的长度+1)
struct Data {
    int type;
    int datai;
    float dataf;
    std::string datas;
};

//Where存放一组判断语句
struct Where {
    Data data; //数据
    WHERE relation_character;   //关系
};

struct Index {
    int num;  //index的总数
    short location[10];  //第i个index建立在第location[i]个attribute上
    string indexname[10];  //每个index的名字
};

struct Attribute {
    int num;  //存放table的属性数
    string name[32];  //存放每个属性的名字
    string index_name[32];
    short type[32];  //存放每个属性的类型，-1：int,0:float,1~255:string的长度+1
    bool unique[32];  //判断每个属性是否unique，是为true
    short primary_key;  //判断是否存在主键,-1为不存在，其他则为主键的所在位置
    bool has_index[32]; //判断是否存在索引
};

class Tuple {
private:
    vector<Data> data_;  //存储元组里的每个数据的信息
public:
    Tuple();
    Tuple(const Tuple& tuple_in);  //拷贝元组
    void addData(Data data_in);  //新增元组
    vector<Data> getData() const;  //返回数据
    int getSize();  //返回元组的数据数量
    bool isDeleted();
    void setDeleted();
    void showTuple();  //显示元组中的所有数据
};

//返回数据长度
template <typename T>
int getDataLength(T data) {
    stringstream stream;
    stream << data;
    return stream.str().length();
}

//返回a,b关系的判断
template <typename T>
bool isSatisfied(T a, T b, WHERE relation) {
    switch (relation) {
    case LESS: {
        if (a < b)
            return true;
        else
            return false;
    }; break;
    case LESS_OR_EQUAL: {
        if (a <= b)
            return true;
        else
            return false;
    }; break;
    case EQUAL: {
        if (a == b)
            return true;
        else
            return false;
    }; break;
    case GREATER_OR_EQUAL: {
        if (a >= b)
            return true;
        else
            return false;
    }; break;
    case GREATER: {
        if (a > b)
            return true;
        else
            return false;
    }; break;
    case NOT_EQUAL: {
        if (a != b)
            return true;
        else
            return false;
    }; break;
    }
}

//将数据转化为字符串
template <typename T>
void copyString(char* p, int& offset, T data) {
    stringstream stream;
    stream << data;
    string s1 = stream.str();
    for (int i = 0; i < s1.length(); i++, offset++)
        p[offset] = s1[i];
}
#endif
