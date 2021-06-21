//
// Created by 11314 on 2021/6/18.
//

#include "BPlusTree.h"

using namespace std;

template<class KeyType>
vector<Search_Info> Node_Search(Node<KeyType>* Tree,Where query){//找不到就return nullptr
    KeyType KeyValue;
    if(query.data.type==-1){//int类型
        KeyValue=query.data.datai;
    }else if(query.data.type==0){//float
        KeyValue=query.data.dataf;
    }else{
        KeyValue=query.data.datas;
    }//确认keyValue
    vector<Search_Info> result;
    Node<KeyType> * temp_node=Tree;
    while(temp_node->isLeaf!=1){//还没找到叶子结点
        for(int i=0;i<temp_node->Info.size();i++){
            if(i==0 && KeyValue<temp_node->Info[i].KeyValue){//第一个指针
                temp_node=temp_node->childs[i];
            }else if(i==temp_node->Info.size()-1){//最后一个
                temp_node=temp_node->childs[i+1];
            }else if(KeyValue>=temp_node->Info[i].KeyValue && KeyValue<temp_node->Info[i+1].KeyValue){
                temp_node=temp_node->childs[i+1];
            }
        }
    }
    Node<KeyType> * head=Tree;
    while(head->isLeaf!=1){
        head=head->childs[0];
    }
    Node<KeyType> * tail=Tree;
    while(tail->isLeaf!=1){
        tail=tail->childs[tail->childs.size()-1];
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


//template<class KeyType>
//bool Node_Insert(Node<KeyType>* Tree, KeyType KeyValue,int Block_Offset,int Offset_in_Block){
//    if(isLeaf==1){//对于叶子结点
//
//    }else if(isLeaf==0){//对于非叶子结点
//
//    }
//
//}
template<class KeyType>
void UpdateFathers(Node<KeyType>* father,Node<KeyType>* son){//用于更新非叶子结点在i位置（从0开始）的键值 该函数不会在仅有一个根节点时使用到
    if(father->Info.size()==0){//仅用于根分裂时更新 新根节点的Info
        father->Info.push_back(son->Info[0]);
        return;
    }
    for(int i=0;i<father->Info.size();i++){
        if(father->childs[i+1]==son){
            Node<KeyType> * temp_node = son;
            while(temp_node->isLeaf!=1){
                temp_node=temp_node->childs[0];
            }//到叶子结点
            father->Info[i]=temp_node->Info[0];//更新
            break;
        }
    }
}

template<class KeyType>
Node<KeyType>* Node_Insert(Node<KeyType>* Tree, KeyType KeyValue,int Block_Offset,int Offset_in_Block){
    Node<KeyType> *temp_node=Tree;
    while (temp_node->isleaf!=1){//
        for(int i=0;i<temp_node->Info.size();i++){
            if(i==0 && KeyValue<temp_node->Info[i].KeyValue){//第一个指针
                temp_node=temp_node->childs[i];
            }else if(i==temp_node->Info.size()-1){//最后一个
                temp_node=temp_node->childs[i+1];
            }else if(KeyValue>=temp_node->Info[i].KeyValue && KeyValue<temp_node->Info[i+1].KeyValue){
                temp_node=temp_node->childs[i+1];
            }
        }
    }//find the leaf
    Index_Info<KeyType> * new_Index_info=new Index_Info<KeyType>;
    new_Index_info->KeyValue=KeyValue;
    new_Index_info->Block_Offset=Block_Offset;
    new_Index_info->Offset_in_Block=Offset_in_Block;

    if(temp_node->Info.size()<temp_node->degree){//叶子结点没满可直接插入
        int i=temp_node->Info.size();//新key值的坐标
        temp_node->Info.push_back(new_Index_info);
        for(;i>0;i--){
            if(temp_node->Info[i-1]==KeyValue){
                return Tree;
            }else if(temp_node->Info[i-1]<KeyValue)break;
            temp_node->Info[i]=temp_node->Info[i-1];
        }
        temp_node->Info[i]=new_Index_info;
        Node<KeyType>* father=temp_node->parent;
        while(temp_node!=NULL){
            UpdataFathers(father,temp_node);
            temp_node=father;
            father=father->parent;
        }//更新父结点key值
        return Tree;
    }else{//叶子结点容量到达上限
        Index_Info<KeyType> a[temp_node->degree+1];//degree+1个键值
        for(int k=0;k<temp_node->degree;k++)a[k]=temp_node->Info[k];//先读取degree个数据
        int i=temp_node->degree;
        for(;i>0;i--){
            if(a[i-1].KeyValue==KeyValue){
                return Tree;//有重复值则不插入
            }else if(a[i-1].KeyValue<KeyValue)break;
            a[i]=a[i-1];
        }
        a[i]=new_Index_info;

        Node<KeyType>* node1 = new Node<KeyType>;
        Node<KeyType>* node2 = new Node<KeyType>;
        Node<KeyType>* node3 = NULL;
        Node<KeyType>* node4 = NULL;

        node1->isleaf=1,node2->isLeaf=1;
        node1->degree=temp_node->degree,node2->degree=temp_node->degree;
        node1->pre=temp_node->pre,node1->next=node2;
        node2->pre=node1,node2->next=temp_node->next;
        node1->parent=temp_node->parent,node2->parent=temp_node->parent;;
        int half=(temp_node->degree+1)/2;
        for(int j=0;j<half;j++){
            node1->Info.push_back(a[j]);
        }
        for(int j=half;j<temp_node->degree+1;j++){
            node2->Info.push_back(a[j]);
        }
        //node1和node2的初始化

        while(temp_node!=NULL){
            Node<KeyType>* parent=temp_node->parent;
            if(parent==NULL){//根结点分裂
                Node<KeyType>* root = new Node<KeyType>;
                root->isLeaf = 0;
                root->degree = temp_node->degree;
                root->pre = NULL;
                root->next = NULL;
                root->parent = NULL;
                root->childs.push_back(node1);
                root->childs.push_back(node2);
                UpdateFathers(root,node2);
                node1->parent=root;
                node2->parent=root;
                Tree = root;
                break;
            }else if(parent->Info.size()<parent->degree){//父节点没满 更新父结点
                int i=0;
                for(;i<parent->childs.size();i++){
                    if(parent->childs[i]==temp_node)break;
                }//找到原来子节点在父结点中的位置
                int childs_size=parent->childs.size();
                parent->childs.push_back(parent->childs[childs_size-1]);
                parent->Info.push_back(parent->Info[childs_size-2]);
                for(int j=childs_size-1;j>=i+2;j--){
                        parent->childs[j]=parent[j-1];
                        parent->Info[j-1]=parent[j-2];
                }
                parent->childs[i]=node1;
                parent->childs[i+1]=node2;
                UpdateFathers(parent,node2);
                node1->parent=parent;
                node2->parent=parent;
                //parent->isleaf = 0;
                break;
            }else{//父结点不是根节点，满了，需要分裂
                node3 = new Node<KeyType>;
                node4 = new Node<KeyType>;
                node3->isleaf=0,node4->isLeaf=0;
                node3->degree=parent->degree,node4->degree=parent->degree;
                node3->pre=parent->pre,node3->next=node4;
                node4->pre=node3,node4->next=parent->next;
                node3->parent=parent->parent,node4->parent=parent->parent;//这行其实可以删去，因为之后一定会对node3,node4的parent进行赋值
                int i=0;
                for(;i<parent->degree+1;i++){
                    if(parent->childs[i]==temp_node)break;
                }//找到原来子节点在父结点中的位置
                Node<KeyType>* temp_childs[parent->degree+2];
                for(int j=0;j<i;j++){
                    temp_childs[j]=parent->childs[j];
                }
                temp_childs[i]=node1;
                temp_childs[i+1]=node2;
                for(int j=i+2;j<parent->degree+2;j++){
                    temp_childs[j]=parent->childs[j-1];
                }//新的childs 大小为degree+2
                int half=(parent->degree+2)/2;
                for(int j=0;j<half;j++){
                    node3->childs.push_back(temp_childs[j]);
                    if(j!=0){
                        node3->Info.push_back(new_Index_info);//实际上这里push一个任意结构体即可，因为马上会对它进行更新
                        UpdateFathers(node3,temp_childs[j]);
                    }
                }
                for(int j=half;j<parent->degree+2;j++){
                    node4->childs.push_back(temp_childs[j]);
                    if(j!=half){
                        node4->Info.push_back(new_Index_info);//实际上这里push一个任意结构体即可，因为马上会对它进行更新
                        UpdateFathers(node4,temp_childs[j]);
                    }
                }
                if(half>=i+2){
                    node1->parent=node3;
                    node2->parent=node3;
                }else if(half<=i){
                    node1->parent=node4;
                    node2->parent=node4;
                }else{
                    node1->parent=node3;
                    node2->parent=node4;
                }
                node1 = node3, node2 = node4;
                temp_node = parent;
            }
        }
        return Tree;
    }
}



template<class KeyType>
bool Node_Delete(Node<KeyType>* Tree,KeyType KeyValue){
    if(isLeaf==1){//对于叶子结点

    }else if(isLeaf==0){//对于非叶子结点

    }
}