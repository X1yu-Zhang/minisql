//
// Created by 11314 on 2021/6/18.
//

#ifndef FOCUS_INDEXMANAGER_INDEXMANAGER_H
#define FOCUS_INDEXMANAGER_INDEXMANAGER_H
#include<vector>
#include <map>
#include"File.h"
#include "BPlusTree.h"
using namespace std;

class Index{
public:
    string IndexName;
    string keyType;//"int" "float" "char"
    int KeySize;//键值长度,以字节为单位
    BplusTree Root;
};
class Index_Info{
    int KeyValue;
    int Block_Offset;
    int Offset_in_Block;
};

class IndexManager{
private:
    map <string , Index> IndexSet;//保存所有已经从文件中建好树的Index
public:
    bool Build_BplusTree_From_File(File * Index_File, string KeyType,int KeySize);
    //需要给出文件指针，Index会加入到IndexSet中，用于查找、删除等使用 成功返回True 否则返回False
    //每次minisql运行前都应该把所有的Index建树
    bool Create_Index(string IndexName,int KeySize,string KeyType);//创建一个新的Index
    bool Insert_Into_Index(string IndexName,string KeyValue,string KeyType,int block_offset,int offset_in_block);
    Index_Info Search_in_Index(string IndexName,string KeyValue);//返回键值的所在Block的块号和在块中的偏移量
    bool Delete_From_Index(string IndexName,string KeyValue);//删除Index的某个键值
    bool Drop_Index(string IndexName);//删除整个Index (包括IndexSet中的对应元素 和 Index文件)
};
#endif //FOCUS_INDEXMANAGER_INDEXMANAGER_H
