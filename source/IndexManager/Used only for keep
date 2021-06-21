//
// Created by 11314 on 2021/6/18.
//

#include "BPlusTree.h"

using namespace std;

template<class KeyType>
vector<Search_Info> Node<KeyType>::Node_Search(Where query){//找不到就return nullptr
    KeyType KeyValue;
    if(query.data.type==-1){//int类型
        KeyValue=query.data.datai;
    }else if(query.data.type==0){//float
        KeyValue=query.data.dataf;
    }else{
        KeyValue=query.data.datas;
    }
    vector<Search_Info> result;
    Node<KeyType> * temp_node=this;
    while(temp_node->isLeaf!=1){//还没找到叶子结点
        for(int i=0;i<temp_node->Info.size();i++){
            if(i==0 && KeyValue<temp_node->Info[i].KeyValue){//第一个指针
                temp_node=childs[i];
            }else if(i==temp_node->Info.size()-1){//最后一个
                temp_node=childs[i+1];
            }else if(KeyValue>=temp_node->Info[i].KeyValue && KeyValue<temp_node->Info[i+1].KeyValue){
                temp_node=childs[i+1];
            }
        }
    }
    Node<KeyType> * head=this;
    while(head->isLeaf!=1){
        head=head->childs[0];
    }
    Node<KeyType> * tail=this;
    while(head->isLeaf!=1){
        head=head->childs[childs.size()-1];
    }
    //head是最头上的node,tail是尾巴上的node

    if(query.relation_character==LESS){
        int flag=0;//标志搜索是否结束
        while(head!=NULL){
            if(flag==1)break;
            for(int i=0;i<head->Info.size();i++){
                if(head->Info[i].KeyValue>=KeyValue){
                    flag=1;
                    break;
                }
                Search_Info temp_result=*(new Search_Info);
                temp_result.Block_Offset=head->Info[i].Block_Offset;
                temp_result.Offset_in_Block=head->Info[i].Offset_in_Block;
                result.push_back(temp_result);//小于则插入到result
            }
            head=head->next;
        }
    }else if(query.relation_character==LESS_OR_EQUAL){
        int flag=0;//标志搜索是否结束
        while(head!=NULL){
            if(flag==1)break;
            for(int i=0;i<head->Info.size();i++){
                if(head->Info[i].KeyValue>KeyValue){
                    flag=1;
                    break;
                }
                Search_Info temp_result=*(new Search_Info);
                temp_result.Block_Offset=head->Info[i].Block_Offset;
                temp_result.Offset_in_Block=head->Info[i].Offset_in_Block;
                result.push_back(temp_result);//不大于则插入到result
            }
            head=head->next;
        }
    }else if(query.relation_character==EQUAL){//done
        int pos=0;
        for(;pos<temp_node->Info.size();pos++){
            if(temp_node->Info[pos].KeyValue==KeyValue){//找到了
                Search_Info temp_result=*(new Search_Info);
                temp_result.Block_Offset=temp_node->Info[pos].Block_Offset;
                temp_result.Offset_in_Block=temp_node->Info[pos].Offset_in_Block;
                result.push_back(temp_result);
            }
        }
    }else if(query.relation_character==GREATER_OR_EQUAL){
        int flag=0;//标志搜索是否结束
        while(tail!=NULL){
            if(flag==1)break;
            for(int i=tail->Info.size()-1;i>=0;i--){//从后往前遍历
                if(tail->Info[i].KeyValue<KeyValue){
                    flag=1;
                    break;
                }
                Search_Info temp_result=*(new Search_Info);
                temp_result.Block_Offset=tail->Info[i].Block_Offset;
                temp_result.Offset_in_Block=tail->Info[i].Offset_in_Block;
                result.push_back(temp_result);//不小于则插入到result
            }
            tail=tail->pre;
        }
    }else if(query.relation_character==GREATER){
        int flag=0;//标志搜索是否结束
        while(tail!=NULL){
            if(flag==1)break;
            for(int i=tail->Info.size()-1;i>=0;i--){//从后往前遍历
                if(tail->Info[i].KeyValue<=KeyValue){
                    flag=1;
                    break;
                }
                Search_Info temp_result=*(new Search_Info);
                temp_result.Block_Offset=tail->Info[i].Block_Offset;
                temp_result.Offset_in_Block=tail->Info[i].Offset_in_Block;
                result.push_back(temp_result);//大于则插入到result
            }
            tail=tail->pre;
        }
    }else if(query.relation_character==NOT_EQUAL){
        while(head!=NULL){
            for(int i=0;i<head->Info.size();i++){
                if(head->Info[i].KeyValue!=KeyValue){
                    Search_Info temp_result=*(new Search_Info);
                    temp_result.Block_Offset=head->Info[i].Block_Offset;
                    temp_result.Offset_in_Block=head->Info[i].Offset_in_Block;
                    result.push_back(temp_result);//不等于则插入到result
                }
            }
            head=head->next;
        }
    }
    return result;
}


template<class KeyType>
Node<KeyType>* Node<KeyType>:: Node_Insert(KeyType KeyValue,int Block_Offset,int Offset_in_Block){
    if(isLeaf==1){//对于叶子结点

    }else if(isLeaf==0){//对于非叶子结点

    }

}



template<class KeyType>
Node<KeyType>* Node<KeyType>::Node_Delete(KeyType KeyValue){
    if(isLeaf==1){//对于叶子结点

    }else if(isLeaf==0){//对于非叶子结点

    }
}