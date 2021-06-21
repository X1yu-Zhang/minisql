//
// Created by 11314 on 2021/6/18.
//

#ifndef FOCUS_INDEXMANAGER_BPLUSTREE_H
#define FOCUS_INDEXMANAGER_BPLUSTREE_H
#include <vector>
#include "../tuple.h"
using namespace std;

template <class KeyType>
class Index_Info{
public:
    KeyType KeyValue;
    int Block_Offset;
    int Offset_in_Block;
};
class Search_Info{
public:
    int Block_Offset;
    int Offset_in_Block;
};
template<class KeyType>
class Node{
public:
    int degree;
    int isLeaf;//0代表不是叶子结点，1代表是叶子结点
    Node* parent;
    Node* pre;
    Node* next;
    vector<Node<KeyType>*> childs;
    vector<Index_Info<KeyType>> Info;

    Node* Node_Insert(KeyType KeyValue,int Block_Offset,int Offset_in_Block);
    vector<Search_Info> Node_Search(Where query);//找不到就return NULL
    Node* Node_Delete(KeyType KeyValue);
};

#endif //FOCUS_INDEXMANAGER_BPLUSTREE_H
