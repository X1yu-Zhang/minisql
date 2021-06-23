//
// Created by 11314 on 2021/6/18.
//

#ifndef FOCUS_INDEXMANAGER_INDEXMANAGER_H
#define FOCUS_INDEXMANAGER_INDEXMANAGER_H
#include <vector>
#include "File.h"
#include "BPlusTree.h"
#include "buffermanager.h"
#include "tuple.h"
#include <map>
using namespace std;

class Index{
public:
    string IndexName;
    string KeyType;//"int" "float" "string"
    int KeySize;//键值长度,以字节为单位
    int degree;//B+树的结点的长度
    Node<int>* Int_Root;
    Node<float>* Float_Root;
    Node<string>* String_Root;
};

class IndexManager{
    BufferManager &bm;
public:
    map<string,Index> IndexSet;//保存所有已经从文件中建好树的Index

    IndexManager( BufferManager & buffer ):bm(buffer){};
    ~IndexManager();//析构时把B+树叶子结点写回
    bool Build_BplusTree_From_File( string KeyType,int KeySize,string IndexName);
    //需要给出文件指针，Index会加入到IndexSet中，用于查找、删除等使用 成功返回True 否则返回False
    //每次minisql运行前都应该把所有的Index建树
    bool Create_Index(string IndexName,int KeySize,string KeyType);//创建一个新的Index(使用时应该先检查)

    bool Insert_Into_Index(string IndexName,string KeyValue,string KeyType,int block_offset,int offset_in_block);

    template<class KeyType>
    vector<Search_Info> Search_In_Index(string IndexName,Index_Where<KeyType> query);
    vector<Search_Info> Search_In_Index(string IndexName,where query);
    bool Delete_From_Index(string IndexName,string KeyValue);//删除Index的某个键值
    bool Drop_Index(string IndexName);//删除整个Index (包括IndexSet中的对应元素 和 Index文件)

};
#endif //FOCUS_INDEXMANAGER_INDEXMANAGER_H