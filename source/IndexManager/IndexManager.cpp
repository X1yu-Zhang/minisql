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
    map<string,Index>::iterator iter;
    for(iter = IndexSet.begin(); iter != IndexSet.end(); iter++){
        Index temp_index=iter->second;
        File* temp_file=buffer->GetFile(temp_index.IndexName,1);//获取index文件指针
        Block* temp_Block=buffer->GetIndexBlock(temp_file,true);//获得用于写入的一个块,表示是第一次获得用于写入的块
        int KeySize=temp_index.KeySize;
        if(temp_index.KeyType=="int"){
            Node<int>* temp_node=temp_index.Int_Root;
            while(temp_node->isLeaf!=1){
                temp_node=temp_node->childs[0];
            }//找到第一个叶子结点
            while(temp_node!=NULL){
                int using_size=4+(KeySize+8)*temp_node->Info.size();
                temp_Block->SetUsingSize(using_size);
                temp_Block->write(0,(char*)(&using_size),4);
                for(int j=0;j<temp_node->Info.size();j++){//填一个块
                    temp_Block->write(4+(KeySize+8)*j,(char*)(&temp_node->Info[j].KeyValue),KeySize);
                    temp_Block->write(4+(KeySize+8)*j+KeySize,(char*)(&temp_node->Info[j].Block_Offset),4);
                    temp_Block->write(4+(KeySize+8)*j+KeySize+4,(char*)(&temp_node->Info[j].Offset_in_Block),4);
                }//依次填入KeyValue,Block_Offset,Offset_in_Block Index——Info大小为Keysize+8
                temp_Block=buffer->GetIndexBlock(temp_file,false);//不是第一次获得用于写入的块
                temp_node=temp_node->next;//到下一个node去，同时更换block
            }
        }else if(temp_index.KeyType=="float"){
            Node<float>* temp_node=temp_index.Float_Root;
            while(temp_node->isLeaf!=1){
                temp_node=temp_node->childs[0];
            }//找到第一个叶子结点
            while(temp_node!=NULL){
                int using_size=4+(KeySize+8)*temp_node->Info.size();
                temp_Block->SetUsingSize(using_size);
                temp_Block->write(0,(char*)(&using_size),4);
                for(int j=0;j<temp_node->Info.size();j++){//填一个块
                    temp_Block->write(4+(KeySize+8)*j,(char*)(&temp_node->Info[j].KeyValue),KeySize);
                    temp_Block->write(4+(KeySize+8)*j+KeySize,(char*)(&temp_node->Info[j].Block_Offset),4);
                    temp_Block->write(4+(KeySize+8)*j+KeySize+4,(char*)(&temp_node->Info[j].Offset_in_Block),4);
                }//依次填入KeyValue,Block_Offset,Offset_in_Block Index——Info大小为Keysize+8
                temp_Block=buffer->GetIndexBlock(temp_file,false);
                temp_node=temp_node->next;//到下一个node去，同时更换block
            }
        }else if(temp_index.KeyType=="string"){
            Node<string>* temp_node=temp_index.String_Root;
            while(temp_node->isLeaf!=1){
                temp_node=temp_node->childs[0];
            }//找到第一个叶子结点
            while(temp_node!=NULL){
                int using_size=4+(KeySize+8)*temp_node->Info.size();
                temp_Block->SetUsingSize(using_size);
                temp_Block->write(0,(char*)(&using_size),4);
                for(int j=0;j<temp_node->Info.size();j++){//填一个块
                    temp_Block->write(4+(KeySize+8)*j,temp_node->Info[j].KeyValue.data(),KeySize);//string转成char
                    temp_Block->write(4+(KeySize+8)*j+KeySize,(char*)(&temp_node->Info[j].Block_Offset),4);
                    temp_Block->write(4+(KeySize+8)*j+KeySize+4,(char*)(&temp_node->Info[j].Offset_in_Block),4);
                }//依次填入KeyValue,Block_Offset,Offset_in_Block Index——Info大小为Keysize+8
                temp_Block=buffer->GetIndexBlock(temp_file,false);
                temp_node=temp_node->next;//到下一个node去，同时更换block
            }
        }
    }
}

bool IndexManager:: Build_BplusTree_From_File( string KeyType,int KeySize,string IndexName){
    File *Index_File=buffer->GetFile(IndexName,1);//将文件连上链表并获得指针
    int degree=Block_Size/(sizeof(Node<int>*)+2*sizeof(int)+KeySize);
    Index* temp_index=new Index;
    temp_index->IndexName=IndexName;
    temp_index->KeySize=KeySize;
    temp_index->KeyType=KeyType;
    temp_index->Int_Root=NULL;
    temp_index->Float_Root=NULL;
    temp_index->String_Root=NULL;
    temp_index->degree=Block_Size/(sizeof(Node<int>*)+2*sizeof(int)+KeySize);
    IndexSet[IndexName]=*temp_index;

    if(KeyType=="int"){
        temp_index->Int_Root=new Node<int>;
        temp_index->Int_Root->degree=temp_index->degree;
        temp_index->Int_Root->isLeaf=1;
        temp_index->Int_Root->parent=NULL;
        temp_index->Int_Root->pre=NULL;
        temp_index->Int_Root->next=NULL;
        Block* temp_block=Index_File->head;//第一个block
        while(temp_block){
            string content=temp_block->GetContent();//返回4096长的string数组
            int using_size=*(int*)(content.substr(0,4).data());//取前四个字节为using_size
            for(int i=4;i<using_size;i+=KeySize+8){
                int keyvalue=*(int*)(content.substr(i,KeySize).data());
                int Block_offset=*(int*)(content.substr(i+KeySize,4).data());
                int offset_in_block=*(int*)(content.substr(i+KeySize+4,4).data());
                Node_Insert<int>(temp_index->Int_Root,keyvalue,Block_offset,offset_in_block);
            }
            temp_block=buffer->GetNextBlock( Index_File , temp_block);
        }
    }else if(KeyType=="float"){
        temp_index->Float_Root=new Node<float>;
        temp_index->Float_Root->degree=temp_index->degree;
        temp_index->Float_Root->isLeaf=1;
        temp_index->Float_Root->parent=NULL;
        temp_index->Float_Root->pre=NULL;
        temp_index->Float_Root->next=NULL;
        Block* temp_block=Index_File->head;//第一个block
        while(temp_block){
            string content=temp_block->GetContent();//返回4096长的string数组
            int using_size=*(int*)(content.substr(0,4).data());//取前四个字节为using_size
            for(int i=4;i<using_size;i+=KeySize+8){
                float keyvalue=*(float*)(content.substr(i,KeySize).data());
                int Block_offset=*(int*)(content.substr(i+KeySize,4).data());
                int offset_in_block=*(int*)(content.substr(i+KeySize+4,4).data());
                Node_Insert<float>(temp_index->Float_Root,keyvalue,Block_offset,offset_in_block);
            }//读完一个block里所有的数据
            temp_block=buffer->GetNextBlock( Index_File , temp_block);
        }
    }else if(KeyType=="string"){
        temp_index->String_Root=new Node<string>;
        temp_index->String_Root->degree=temp_index->degree;
        temp_index->String_Root->isLeaf=1;
        temp_index->String_Root->parent=NULL;
        temp_index->String_Root->pre=NULL;
        temp_index->String_Root->next=NULL;
        Block* temp_block=Index_File->head;//第一个block
        while(temp_block){
            string content=temp_block->GetContent();//返回4096长的string数组
            int using_size=*(int*)(content.substr(0,4).data());//取前四个字节为using_size
            for(int i=4;i<using_size;i+=KeySize+8){
                string keyvalue=content.substr(i,KeySize);
                int Block_offset=*(int*)(content.substr(i+KeySize,4).data());
                int offset_in_block=*(int*)(content.substr(i+KeySize+4,4).data());
                Node_Insert<string>(temp_index->String_Root,keyvalue,Block_offset,offset_in_block);
            }
            temp_block=buffer->GetNextBlock( Index_File , temp_block);
        }
    }else{
        return false;
    }
    return true;
}


bool IndexManager::Create_Index(string IndexName,int KeySize,string KeyType){//done
    if (IndexSet.find(IndexName) != IndexSet.end())return false;//存在这个Index
    File* index_file=buffer->GetFile(IndexName,1);
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
        temp_index->Int_Root->parent=NULL;
        temp_index->Int_Root->pre=NULL;
        temp_index->Int_Root->next=NULL;
    }else if(KeyType=="float"){
        temp_index->Float_Root=new Node<float>;
        temp_index->Float_Root->degree=Block_Size/(sizeof(Node<int>*)+2*sizeof(int)+KeySize);
        temp_index->Float_Root->isLeaf=1;
        temp_index->Float_Root->parent=NULL;
        temp_index->Float_Root->pre=NULL;
        temp_index->Float_Root->next=NULL;
    }else if(KeyType=="string"){
        temp_index->String_Root=new Node<string>;
        temp_index->String_Root->degree=Block_Size/(sizeof(Node<int>*)+2*sizeof(int)+KeySize);
        temp_index->String_Root->isLeaf=1;
        temp_index->String_Root->parent=NULL;
        temp_index->String_Root->pre=NULL;
        temp_index->String_Root->next=NULL;
    }
    temp_index->degree=Block_Size/(sizeof(Node<int>*)+2*sizeof(int)+KeySize);
    IndexSet[IndexName]=*temp_index;
    return true;
}

bool IndexManager::Insert_Into_Index(string IndexName,string KeyValue,string KeyType,int block_offset,int offset_in_block){
    if (IndexSet.find(IndexName) == IndexSet.end())return false;//不存在这个Index
    if(KeyType=="int"){
        Node_Insert<int>(IndexSet[IndexName].Int_Root,*(int*)(&KeyValue),block_offset,offset_in_block);
        return true;
    }else if(KeyType=="float"){
        Node_Insert<float>(IndexSet[IndexName].Float_Root,*(float*)(&KeyValue),block_offset,offset_in_block);
        return true;
    }else if(KeyType=="string"){
        Node_Insert(IndexSet[IndexName].String_Root,KeyValue,block_offset,offset_in_block);
        return true;
    }
    return false;
}

vector<Search_Info> IndexManager::Search_In_Index(string IndexName, Where query) {
    vector<Search_Info> result;
    if (IndexSet.find(IndexName) != IndexSet.end()){
        if(IndexSet[IndexName].KeyType=="int"){
            return Node_Search(IndexSet[IndexName].Int_Root,query);
        }else if(IndexSet[IndexName].KeyType=="float"){
            return Node_Search(IndexSet[IndexName].Float_Root,query);
        }else if(IndexSet[IndexName].KeyType=="string"){
            return Node_Search(IndexSet[IndexName].String_Root,query);
        }
    }
    return result;
}


bool IndexManager::Delete_From_Index(string IndexName,string KeyValue) {
    if (IndexSet.find(IndexName) != IndexSet.end())return false;;
    if(IndexSet[IndexName].KeyType=="int"){
        Node_Delete(IndexSet[IndexName].Int_Root,*(int*)(&KeyValue));
        return true;
    }else if(IndexSet[IndexName].KeyType=="float"){
        Node_Delete(IndexSet[IndexName].Float_Root,*(float*)(&KeyValue));
        return true;
    }else if(IndexSet[IndexName].KeyType=="string"){
        Node_Delete(IndexSet[IndexName].String_Root,KeyValue);
        return true;
    }
}

bool IndexManager::Drop_Index(string IndexName){
    if (IndexSet.find(IndexName) != IndexSet.end())return false;;
    IndexSet.erase(IndexName);//删除key为IndexName的元素
    buffer->DeleteFileFromList(IndexName );
    return true;
}





