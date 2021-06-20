#ifndef TUPLE_H
#define TUPLE_H
#include <vector>
#include <iostream>
#include <sstream>
using namespace std;

typedef enum {
    LESS,
    LESS_OR_EQUAL,
    EQUAL,
    GREATER_OR_EQUAL,
    GREATER,
    NOT_EQUAL
} WHERE;


struct Data {
    int type; // -1 for int , 0 for float , 1 - 255 for the length of string
    int datai;
    float dataf;
    std::string datas;
};

//Where???????��????
struct Where {
    Data data; //????
    WHERE relation_character;   //???
};

struct Index {
    int num;  //index??????
    short location[10];  //??i??index???????location[i]??attribute??
    string indexname[10];  //???index??????
};

struct Attribute {
    int num;  //???table????????
    string name[32];  //???????????????
    string index_name[32];
    unsigned short type[32];  //????????????????-1??int,0:float,1~255:string?????+1
    bool unique[32];  //?��???????????unique?????true
    short primary_key;  //?��???????????,-1?????????????????????????��??
    bool has_index[32]; //?��???????????
};

class Tuple {
private:
    vector<Data> data_;  //?��?????????????????
public:
    Tuple();
    Tuple(const Tuple& tuple_in);  //???????
    void addData(Data data_in);  //???????
    vector<Data> getData() const;  //????????
    int getSize();  //????????????????
    void showTuple();  //???????��?????????
};

//???????????
template <typename T>
int getDataLength(T data) {
    stringstream stream;
    stream << data;
    return stream.str().length();
}

//????a,b??????��?
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

//???????????????
template <typename T>
void copyString(char* p, int& offset, T data) {
    stringstream sStream;
    sStream << data;
    string s1 = sStream.str();
    for (int i = 0; i < s1.length(); i++, offset++)
        p[offset] = s1[i];
}
#endif
