//
// Created by 11314 on 2021/6/18.
//
#include <iostream>
#include "IndexManager.h"
#include "string"
#include<sstream>
using namespace std;
#define Block_Size 4096
IndexManager::IndexManager(){

}

IndexManager::~IndexManager(){  //done Block的调用也许会有问题 index的存储可能会有问题
    for(int i=0;i<IndexSet.size();i++){
        File* temp_file=buffer->GetFile(IndexSet[i].IndexName,1);//获取index文件指针
        Block* temp_Block=buffer->GetIndexBlock(temp_file);//获得用于写入的一个块
        int KeySize=IndexSet[i].KeySize;
        if(IndexSet[i].KeyType=="int"){
            Node<int>* temp_node=IndexSet[i].Int_Root;
            while(temp_node->isLeaf!=1){
                temp_node=temp_node->childs[0];
            }//找到第一个叶子结点
            while(temp_node!=NULL){
                for(int j=0;j<temp_node->Info.size();j++){//填一个块
                    temp_Block->write((KeySize+8)*j,(char*)(&temp_node->Info[j].KeyValue),KeySize);
                    temp_Block->write((KeySize+8)*j+KeySize,(char*)(&temp_node->Info[j].Block_Offset),4);
                    temp_Block->write((KeySize+8)*j+KeySize+4,(char*)(&temp_node->Info[j].Offset_in_Block),4);
                }//依次填入KeyValue,Block_Offset,Offset_in_Block Index——Info大小为Keysize+8
                temp_Block=buffer->GetIndexBlock(temp_file);
                temp_node=temp_node->next;//到下一个node去，同时更换block
            }
        }else if(IndexSet[i].KeyType=="float"){
            Node<float>* temp_node=IndexSet[i].Float_Root;
            while(temp_node->isLeaf!=1){
                temp_node=temp_node->childs[0];
            }//找到第一个叶子结点
            while(temp_node!=NULL){
                for(int j=0;j<temp_node->Info.size();j++){//填一个块
                    temp_Block->write((KeySize+8)*j,(char*)(&temp_node->Info[j].KeyValue),KeySize);
                    temp_Block->write((KeySize+8)*j+KeySize,(char*)(&temp_node->Info[j].Block_Offset),4);
                    temp_Block->write((KeySize+8)*j+KeySize+4,(char*)(&temp_node->Info[j].Offset_in_Block),4);
                }//依次填入KeyValue,Block_Offset,Offset_in_Block Index——Info大小为Keysize+8
                temp_Block=buffer->GetIndexBlock(temp_file);
                temp_node=temp_node->next;//到下一个node去，同时更换block
            }
        }else if(IndexSet[i].KeyType=="string"){
            Node<string>* temp_node=IndexSet[i].String_Root;
            while(temp_node->isLeaf!=1){
                temp_node=temp_node->childs[0];
            }//找到第一个叶子结点
            while(temp_node!=NULL){
                for(int j=0;j<temp_node->Info.size();j++){//填一个块
                    temp_Block->write((KeySize+8)*j,temp_node->Info[j].KeyValue.data(),KeySize);//string转成char
                    temp_Block->write((KeySize+8)*j+KeySize,(char*)(&temp_node->Info[j].Block_Offset),4);
                    temp_Block->write((KeySize+8)*j+KeySize+4,(char*)(&temp_node->Info[j].Offset_in_Block),4);
                }//依次填入KeyValue,Block_Offset,Offset_in_Block Index——Info大小为Keysize+8
                temp_Block=buffer->GetIndexBlock(temp_file);
                temp_node=temp_node->next;//到下一个node去，同时更换block
            }
        }
    }
}

bool IndexManager:: Build_BplusTree_From_File(File * Index_File, string KeyType,int KeySize,string IndexName){
    int degree=Block_Size/(sizeof(Node<int>*)+2*sizeof(int)+KeySize);
    Index* temp_index=new Index;
    temp_index->IndexName=IndexName;
    temp_index->KeySize=KeySize;
    temp_index->KeyType=KeyType;
    temp_index->Int_Root=NULL;
    temp_index->Float_Root=NULL;
    temp_index->String_Root=NULL;
    temp_index->degree=Block_Size/(sizeof(Node<int>*)+2*sizeof(int)+KeySize);
    IndexSet.push_back(*temp_index);

    if(KeyType=="int"){
        Block* temp_block=Index_File->head;//第一个block
        while(temp_block){
            string content=temp_block->GetContent();//返回4096长的char数组，但string长度不一定为4096
            for(int i=0;i<content.size();i+=KeySize+8){
                int keyvalue=*(int*)(content.substr(i,KeySize).data());
                int Block_offset=*(int*)(content.substr(i+KeySize,4).data());
                int offset_in_block=*(int*)(content.substr(i+KeySize+4,4).data());
                temp_index->Int_Root->Node_Insert(keyvalue,Block_offset,offset_in_block);
            }
            temp_block=buffer->GetNextBlock( Index_File , temp_block);
        }
    }else if(KeyType=="float"){
        Block* temp_block=Index_File->head;//第一个block
        while(temp_block){
            string content=temp_block->GetContent();//返回4096长的char数组，但string长度不一定为4096
            for(int i=0;i<content.size();i+=KeySize+8){
                float keyvalue=*(float*)(content.substr(i,KeySize).data());
                int Block_offset=*(int*)(content.substr(i+KeySize,4).data());
                int offset_in_block=*(int*)(content.substr(i+KeySize+4,4).data());
                temp_index->Float_Root->Node_Insert(keyvalue,Block_offset,offset_in_block);
            }//读完一个block里所有的数据
            temp_block=buffer->GetNextBlock( Index_File , temp_block);
        }
    }else if(KeyType=="string"){
        Block* temp_block=Index_File->head;//第一个block
        while(temp_block){
            string content=temp_block->GetContent();//返回4096长的char数组，但string长度不一定为4096
            for(int i=0;i<content.size();i+=KeySize+8){
                string keyvalue=content.substr(i,KeySize);
                int Block_offset=*(int*)(content.substr(i+KeySize,4).data());
                int offset_in_block=*(int*)(content.substr(i+KeySize+4,4).data());
                temp_index->String_Root->Node_Insert(keyvalue,Block_offset,offset_in_block);
            }
            temp_block=buffer->GetNextBlock( Index_File , temp_block);
        }
    }else{
        return false;
    }
    return true;
}


bool IndexManager::Create_Index(string IndexName,int KeySize,string KeyType){//done
    for(int i=0;i<IndexSet.size();i++){
        if(IndexSet[i].IndexName==IndexName)return false;//若该IndexName已经存在，则创建失败
    }
    Index *temp_index=new Index;
    temp_index->IndexName=IndexName;
    temp_index->KeySize=KeySize;
    temp_index->KeyType=KeyType;
    temp_index->Int_Root=NULL;
    temp_index->Float_Root=NULL;
    temp_index->String_Root=NULL;
    if(KeyType=="int"){
        temp_index->Int_Root=new Node<int>;
        temp_index->Int_Root->degree=Block_Size/(sizeof(Node<int>*)+2*sizeof(int)+KeySize);
        temp_index->Int_Root->isLeaf=1;
        temp_index->Int_Root->parent=NULL;//可用于判断是否是根节点
    }else if(KeyType=="float"){
        temp_index->Float_Root=new Node<float>;
        temp_index->Float_Root->degree=Block_Size/(sizeof(Node<int>*)+2*sizeof(int)+KeySize);
        temp_index->Float_Root->isLeaf=1;
        temp_index->Float_Root->parent=NULL;//可用于判断是否是根节点
    }else if(KeyType=="string"){
        temp_index->String_Root=new Node<string>;
        temp_index->String_Root->degree=Block_Size/(sizeof(Node<int>*)+2*sizeof(int)+KeySize);
        temp_index->String_Root->isLeaf=1;
        temp_index->String_Root->parent=NULL;//可用于判断是否是根节点
    }
    temp_index->degree=Block_Size/(sizeof(Node<int>*)+2*sizeof(int)+KeySize);
    IndexSet.push_back(*temp_index);
    return true;
}

bool IndexManager::Insert_Into_Index(string IndexName,string KeyValue,string KeyType,int block_offset,int offset_in_block){
    for(int i=0;i<IndexSet.size();i++){
        if(IndexName==IndexSet[i].IndexName){
            if(KeyType=="int"){
                IndexSet[i].Int_Root=IndexSet[i].Int_Root->Node_Insert(*(int*)(&KeyValue),block_offset,offset_in_block);
                return true;
            }else if(KeyType=="float"){
                IndexSet[i].Float_Root=IndexSet[i].Float_Root->Node_Insert(*(float*)(&KeyValue),block_offset,offset_in_block);
                return true;
            }else if(KeyType=="string"){
                IndexSet[i].String_Root=IndexSet[i].String_Root->Node_Insert(KeyValue,block_offset,offset_in_block);
                return true;
            }
        }
    }
    return false;
}

Search_Info* IndexManager::Search_in_Index(string IndexName,string KeyValue){
    for(int i=0;i<IndexSet.size();i++){
        if(IndexName==IndexSet[i].IndexName){
            if(IndexSet[i].KeyType=="int"){
                return IndexSet[i].Int_Root->Node_Search(*(int*)(&KeyValue));
            }else if(IndexSet[i].KeyType=="float"){
                return IndexSet[i].Float_Root->Node_Search(*(float*)(&KeyValue));
            }else if(IndexSet[i].KeyType=="string"){
                return IndexSet[i].String_Root->Node_Search(KeyValue);
            }
        }
    }
    return NULL;
}

bool IndexManager::Delete_From_Index(string IndexName,string KeyValue) {
    for(int i=0;i<IndexSet.size();i++){
        if(IndexName==IndexSet[i].IndexName){
            if(IndexSet[i].KeyType=="int"){
                IndexSet[i].Int_Root=IndexSet[i].Int_Root->Node_Delete(*(int*)(&KeyValue));
                return true;
            }else if(IndexSet[i].KeyType=="float"){
                IndexSet[i].Float_Root=IndexSet[i].Float_Root->Node_Delete(*(float*)(&KeyValue));
                return true;
            }else if(IndexSet[i].KeyType=="string"){
                IndexSet[i].String_Root=IndexSet[i].String_Root->Node_Delete(KeyValue);
                return true;
            }
        }
    }
    return false;
}

bool IndexManager::Drop_Index(string IndexName){
    for(int i=0;i<IndexSet.size();i++){
        if(IndexName==IndexSet[i].IndexName){
            IndexSet.erase(IndexSet.begin()+i);//删除该元素，但我不确定这里是否要进行内存操作
            return true;
        }
    }
    return false;
}



