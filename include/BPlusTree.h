//
// Created by 11314 on 2021/6/18.
//

#ifndef FOCUS_INDEXMANAGER_BPLUSTREE_H
#define FOCUS_INDEXMANAGER_BPLUSTREE_H
#include<vector>
#include"tuple.h"
#include "algorithm"
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
    Node* pre;
    Node* next;
    vector<Node<KeyType>*> childs;
    vector<Index_Info<KeyType>> Info;
//    Node<KeyType>* Node_Insert(Node<KeyType>* Tree,KeyType KeyValue,int Block_Offset,int Offset_in_Block);
//    vector<Search_Info> Node_Search(Node<KeyType>* Tree,Where query);//找不到就return NULL
//    Node<KeyType>* Node_Delete(Node<KeyType>*Node,KeyType KeyValue);
//    void UpdateFathers(Node<KeyType>* father,Node<KeyType>* son);
};

template<class KeyType>
void Node_print(Node<KeyType>* node){
    cout<<"Node_print"<<endl;
    for(int i=0;i<node->Info.size();i++){
        cout<<node->Info[i].KeyValue<<" ";
    }
    cout<<endl;
};

//template<class KeyType>
//Node<KeyType>* Node_Insert(Node<KeyType>* Tree,KeyType KeyValue,int Block_Offset,int Offset_in_Block);
//
//template<class KeyType>
//vector<Search_Info> Node_Search(Node<KeyType>* Tree,Where query);//找不到就return NULL
//
//template<class KeyType>
//Node<KeyType>* Node_Delete(Node<KeyType>*Node,KeyType KeyValue);
template<class KeyType>
bool find_child(Node<KeyType>* parent,Node<KeyType>* son) {
    for(int i=0;i<parent->childs.size();i++){
        if(parent->childs[i]==son)return true;
    }
    return false;
}

template<class KeyType>
vector<Search_Info> Node_Search(Node<KeyType>* Tree,Index_Where<KeyType> query){//找不到就return nullptr
    KeyType KeyValue;
//    if(query.data.type==-1){//int类型
//        KeyValue=query.data.datai;
//    }else if(query.data.type==0){//float
//        KeyValue=query.data.dataf;
//    }else{
//        KeyValue=query.data.datas;
//    }//确认keyValue
    KeyValue=query.data;
    vector<Search_Info> result;
    Node<KeyType> * temp_node=Tree;
    while(temp_node->isLeaf!=1){//还没找到叶子结点
        for(int i=0;i<temp_node->Info.size();i++){
            if(i==0 && KeyValue<temp_node->Info[i].KeyValue){//第一个指针
                temp_node=temp_node->childs[i];
                break;
            }else if(i==temp_node->Info.size()-1){//最后一个
                temp_node=temp_node->childs[i+1];
                break;
            }else if(KeyValue>=temp_node->Info[i].KeyValue && KeyValue<temp_node->Info[i+1].KeyValue){
                temp_node=temp_node->childs[i+1];
                break;
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
        vector<Node<KeyType>*> node;
        int front=0,rear=0,last=0;
        int isleaf = Tree -> isLeaf;
        node.push_back(Tree);
        while(front<=rear){
            if(isleaf==0){//非叶子结点则push
                for(int i=front;i<=rear;i++){//遍历一层
                    //输出一个结点中的键值
                    Node<KeyType> *temp=node[i];
                    for(int j=0;j<temp->childs.size();j++){
                        node.push_back(temp->childs[j]);
                        last++;
                    }
                }
                if(node[last]->isLeaf)isleaf=1;
                front=rear+1;
                rear=last;
            }else{
                for(int i=front;i<=rear;i++){//结果输入给result
                    Node<KeyType> *temp=node[i];
                    for(int j=0;j<temp->Info.size();j++){
                        if(temp->Info[j].KeyValue>=KeyValue){
                            return result;
                        }
                        Search_Info temp_result=*(new Search_Info);
                        temp_result.Block_Offset=temp->Info[j].Block_Offset;
                        temp_result.Offset_in_Block=temp->Info[j].Offset_in_Block;
                        result.push_back(temp_result);//小于则插入到result
                    }
                }
                return result;
            }
        }
//        int flag=0;//标志搜索是否结束
//        while(head!=NULL){
//            if(flag==1)break;
//            for(int i=0;i<head->Info.size();i++){
//                if(head->Info[i].KeyValue>=KeyValue){
//                    flag=1;
//                    break;
//                }
//                Search_Info temp_result=*(new Search_Info);
//                temp_result.Block_Offset=head->Info[i].Block_Offset;
//                temp_result.Offset_in_Block=head->Info[i].Offset_in_Block;
//                result.push_back(temp_result);//小于则插入到result
//            }
//            head=head->next;
//        }
    }else if(query.relation_character==LESS_OR_EQUAL){
        vector<Node<KeyType>*> node;
        int front=0,rear=0,last=0;
        int isleaf = Tree -> isLeaf;
        node.push_back(Tree);
        while(front<=rear){
            if(isleaf==0){//非叶子结点则push
                for(int i=front;i<=rear;i++){//遍历一层
                    //输出一个结点中的键值
                    Node<KeyType> *temp=node[i];
                    for(int j=0;j<temp->childs.size();j++){
                        node.push_back(temp->childs[j]);
                        last++;
                    }
                }
                if(node[last]->isLeaf)isleaf=1;
                front=rear+1;
                rear=last;
            }else{
                for(int i=front;i<=rear;i++){//结果输入给result
                    Node<KeyType> *temp=node[i];
                    for(int j=0;j<temp->Info.size();j++){
                        if(temp->Info[j].KeyValue>KeyValue){
                            return result;
                        }
                        Search_Info temp_result=*(new Search_Info);
                        temp_result.Block_Offset=temp->Info[j].Block_Offset;
                        temp_result.Offset_in_Block=temp->Info[j].Offset_in_Block;
                        result.push_back(temp_result);//小于则插入到result
                    }
                }
                return result;
            }
        }
//        int flag=0;//标志搜索是否结束
//        while(head!=NULL){
//            if(flag==1)break;
//            for(int i=0;i<head->Info.size();i++){
//                if(head->Info[i].KeyValue>KeyValue){
//                    flag=1;
//                    break;
//                }
//                Search_Info temp_result=*(new Search_Info);
//                temp_result.Block_Offset=head->Info[i].Block_Offset;
//                temp_result.Offset_in_Block=head->Info[i].Offset_in_Block;
//                result.push_back(temp_result);//不大于则插入到result
//            }
//            head=head->next;
//        }
    }else if(query.relation_character==EQUAL){
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
        vector<Node<KeyType>*> node;
        int front=0,rear=0,last=0;
        int isleaf = Tree -> isLeaf;
        node.push_back(Tree);
        while(front<=rear){
            if(isleaf==0){//非叶子结点则push
                for(int i=front;i<=rear;i++){//遍历一层
                    //输出一个结点中的键值
                    Node<KeyType> *temp=node[i];
                    for(int j=temp->childs.size()-1;j>=0;j--){
                        node.push_back(temp->childs[j]);
                        last++;
                    }
                }
                if(node[last]->isLeaf)isleaf=1;
                front=rear+1;
                rear=last;
            }else{
                for(int i=front;i<=rear;i++){//结果输入给result
                    Node<KeyType> *temp=node[i];
                    for(int j=temp->Info.size()-1;j>=0;j--){
                        if(temp->Info[j].KeyValue<KeyValue){
                            reverse(result.begin(), result.end());
                            return result;
                        }
                        Search_Info temp_result=*(new Search_Info);
                        temp_result.Block_Offset=temp->Info[j].Block_Offset;
                        temp_result.Offset_in_Block=temp->Info[j].Offset_in_Block;
                        result.push_back(temp_result);//小于则插入到result
                    }
                }
                reverse(result.begin(), result.end());
                return result;
            }
        }
//        int flag=0;//标志搜索是否结束
//        while(tail!=NULL){
//            if(flag==1)break;
//            for(int i=tail->Info.size()-1;i>=0;i--){//从后往前遍历
//                if(tail->Info[i].KeyValue<KeyValue){
//                    flag=1;
//                    break;
//                }
//                Search_Info temp_result=*(new Search_Info);
//                temp_result.Block_Offset=tail->Info[i].Block_Offset;
//                temp_result.Offset_in_Block=tail->Info[i].Offset_in_Block;
//                result.push_back(temp_result);//不小于则插入到result
//            }
//            tail=tail->pre;
//        }
//        reverse(result.begin(), result.end());
    }else if(query.relation_character==GREATER){
        vector<Node<KeyType>*> node;
        int front=0,rear=0,last=0;
        int isleaf = Tree -> isLeaf;
        node.push_back(Tree);
        while(front<=rear){
            if(isleaf==0){//非叶子结点则push
                for(int i=front;i<=rear;i++){//遍历一层
                    //输出一个结点中的键值
                    Node<KeyType> *temp=node[i];
                    for(int j=temp->childs.size()-1;j>=0;j--){
                        node.push_back(temp->childs[j]);
                        last++;
                    }
                }
                if(node[last]->isLeaf)isleaf=1;
                front=rear+1;
                rear=last;
            }else{
                for(int i=front;i<=rear;i++){//结果输入给result
                    Node<KeyType> *temp=node[i];
                    for(int j=temp->Info.size()-1;j>=0;j--){
                        if(temp->Info[j].KeyValue<=KeyValue){
                            reverse(result.begin(), result.end());
                            // for(int k=0;k<result.size();k++){
                            //     cout<<"result:"<<result[k].Block_Offset<<" "<<result[k].Offset_in_Block<<endl;
                            // }
                            return result;
                        }
                        Search_Info temp_result;
                        temp_result.Block_Offset=temp->Info[j].Block_Offset;
                        temp_result.Offset_in_Block=temp->Info[j].Offset_in_Block;
                        result.push_back(temp_result);//小于则插入到result
                    }
                }
                reverse(result.begin(), result.end());
                // for(int i=0;i<result.size();i++){
                //     cout<<"result:"<<result[i].Block_Offset<<" "<<result[i].Offset_in_Block<<endl;
                // }
                return result;
            }
        }
//        int flag=0;//标志搜索是否结束
//        while(tail!=NULL){
//            if(flag==1)break;
//            for(int i=tail->Info.size()-1;i>=0;i--){//从后往前遍历
//                if(tail->Info[i].KeyValue<=KeyValue){
//                    flag=1;
//                    break;
//                }
//                Search_Info temp_result=*(new Search_Info);
//                temp_result.Block_Offset=tail->Info[i].Block_Offset;
//                temp_result.Offset_in_Block=tail->Info[i].Offset_in_Block;
//                result.push_back(temp_result);//大于则插入到result
//            }
//            tail=tail->pre;
//        }
//        reverse(result.begin(), result.end());
    }else if(query.relation_character==NOT_EQUAL){
        vector<Node<KeyType>*> node;
        int front=0,rear=0,last=0;
        int isleaf = Tree -> isLeaf;
        node.push_back(Tree);
        while(front<=rear){
            if(isleaf==0){//非叶子结点则push
                for(int i=front;i<=rear;i++){//遍历一层
                    //输出一个结点中的键值
                    Node<KeyType> *temp=node[i];
                    for(int j=0;j<temp->childs.size();j++){
                        node.push_back(temp->childs[j]);
                        last++;
                    }
                }
                if(node[last]->isLeaf)isleaf=1;
                front=rear+1;
                rear=last;
            }else{
                for(int i=front;i<=rear;i++){//结果输入给result
                    Node<KeyType> *temp=node[i];
                    for(int j=0;j<temp->Info.size();j++){
                        if(temp->Info[j].KeyValue!=KeyValue){
                            Search_Info temp_result=*(new Search_Info);
                            temp_result.Block_Offset=temp->Info[j].Block_Offset;
                            temp_result.Offset_in_Block=temp->Info[j].Offset_in_Block;
                            result.push_back(temp_result);//小于则插入到result
                        }
                    }
                }
                return result;
            }
        }
//        while(head!=NULL){
//            for(int i=0;i<head->Info.size();i++){
//                if(head->Info[i].KeyValue!=KeyValue){
//                    Search_Info temp_result=*(new Search_Info);
//                    temp_result.Block_Offset=head->Info[i].Block_Offset;
//                    temp_result.Offset_in_Block=head->Info[i].Offset_in_Block;
//                    result.push_back(temp_result);//不等于则插入到result
//                }
//            }
//            head=head->next;
//        }
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
        Node<KeyType> * temp_node = son;
        while(temp_node->isLeaf!=1){
            temp_node=temp_node->childs[0];
        }//到叶子结点
        father->Info.push_back(temp_node->Info[0]);
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
    // cout << KeyValue <<" "<<Block_Offset<<" "<<Offset_in_Block<<endl; 
    Node<KeyType> *temp_node=Tree;
    vector<Node<KeyType>* > PARENTS;
    PARENTS.push_back(NULL);//第一个是NULL
    while (temp_node->isLeaf!=1){//S
        for(int i=0;i<temp_node->Info.size();i++){
            if(i==0 && KeyValue<temp_node->Info[i].KeyValue){//第一个指针
                PARENTS.push_back(temp_node);
                temp_node=temp_node->childs[i];
                break;
            }else if(i==temp_node->Info.size()-1){//最后一个
                PARENTS.push_back(temp_node);
                temp_node=temp_node->childs[i+1];
                break;
            }else if(KeyValue>=temp_node->Info[i].KeyValue && KeyValue<temp_node->Info[i+1].KeyValue){
                PARENTS.push_back(temp_node);
                temp_node=temp_node->childs[i+1];
                break;
            }
        }
    }//find the leaf
    Index_Info<KeyType> new_Index_info;
    new_Index_info.KeyValue=KeyValue;
    new_Index_info.Block_Offset=Block_Offset;
    new_Index_info.Offset_in_Block=Offset_in_Block;

    if(temp_node->Info.size()<temp_node->degree){//叶子结点没满可直接插入
        int i=temp_node->Info.size();//新key值的坐标
        temp_node->Info.push_back(new_Index_info);
        for(;i>0;i--){
            if(temp_node->Info[i-1].KeyValue==KeyValue){
                return Tree;
            }else if(temp_node->Info[i-1].KeyValue<KeyValue)break;
            temp_node->Info[i]=temp_node->Info[i-1];
        }
        temp_node->Info[i]=new_Index_info;
        Node<KeyType>*parent;
        parent=PARENTS.back();
        PARENTS.pop_back();
        while(parent){
            UpdateFathers(parent,temp_node);
            temp_node=parent;
            parent=PARENTS.back();
            PARENTS.pop_back();
        }
        // for(int i=0;i<Tree->Info.size();i++){
        //     cout << i <<":"<<Tree->Info[i].KeyValue <<" "<<Tree->Info[i].Block_Offset<<" "<<Tree->Info[i].Offset_in_Block<<endl; 
        // }
//更新父结点key值
        return Tree;
    }else{//叶子结点容量到达上限
        Index_Info<KeyType> a[temp_node->degree+1];//degree+1个键值
        for(int k=0;k<temp_node->degree;k++)a[k]=temp_node->Info[k];//先读取degree个数据
        int i=temp_node->degree;
        for(;i>0;i--){
            if(a[i-1].KeyValue==KeyValue){
                return Tree;//有重复值则不插入
            }else if(a[i-1].KeyValue<KeyValue)break;
            else a[i]=a[i-1];
        }
        a[i]=new_Index_info;

        Node<KeyType>* node1 = new Node<KeyType>;
        Node<KeyType>* node2 = new Node<KeyType>;
        Node<KeyType>* node3 = NULL;
        Node<KeyType>* node4 = NULL;

        node1->isLeaf=1,node2->isLeaf=1;
        node1->degree=temp_node->degree,node2->degree=temp_node->degree;
        node1->pre=temp_node->pre,node1->next=node2;
        node2->pre=node1,node2->next=temp_node->next;
        if(temp_node->pre)temp_node->pre->next=node1;
        if(temp_node->next)temp_node->next->pre=node2;
        int half=(temp_node->degree+1)/2;
        for(int j=0;j<half;j++){
            node1->Info.push_back(a[j]);
        }
        for(int j=half;j<temp_node->degree+1;j++){
            node2->Info.push_back(a[j]);
        }
        //node1和node2的初始化
        //Node_print(node1);
        //Node_print(node2);
        Node<KeyType>*parent;
        parent=PARENTS.back();
        PARENTS.pop_back();
        while(temp_node!=NULL){
            if(parent==NULL){//根结点分裂
                Node<KeyType>* root = new Node<KeyType>;
                root->isLeaf = 0;
                root->degree = temp_node->degree;
                root->pre = NULL;
                root->next = NULL;
                root->childs.push_back(node1);
                root->childs.push_back(node2);
                UpdateFathers(root,node2);
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
                    parent->childs[j]=parent->childs[j-1];
                    parent->Info[j-1]=parent->Info[j-2];
                }
                parent->childs[i]=node1;
                parent->childs[i+1]=node2;
                UpdateFathers(parent,node2);
                temp_node=parent;
                parent=PARENTS.back();
                PARENTS.pop_back();
                while(parent){
                    UpdateFathers(parent,temp_node);
                    temp_node=parent;
                    parent=PARENTS.back();
                    PARENTS.pop_back();
                }//循环向上更新
                break;
            }else{//父结点不是根节点，满了，需要分裂
                //parent已经pop了一次
                node3 = new Node<KeyType>;
                node4 = new Node<KeyType>;
                node3->isLeaf=0,node4->isLeaf=0;
                node3->degree=parent->degree,node4->degree=parent->degree;
                node3->pre=parent->pre,node3->next=node4;
                node4->pre=node3,node4->next=parent->next;
                if(parent->pre)parent->pre->next=node3;
                if(parent->next)parent->next->pre=node4;
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
                node1 = node3, node2 = node4;
                temp_node = parent;
                parent=PARENTS.back();
                PARENTS.pop_back();
            }
        }
        return Tree;
    }
}
//template<class KeyType>
//Node<KeyType>* Node_Delete(Node<KeyType>* Tree,KeyType KeyValue){
//
//}


template<class KeyType>
Node<KeyType>* Node_Delete(Node<KeyType>* Tree,KeyType KeyValue){
    Node<KeyType>* temp_node=Tree;
    vector<Node<KeyType>*> PARENTS;
    PARENTS.push_back(NULL);
    while (temp_node->isLeaf!=1){//
        for(int i=0;i<temp_node->Info.size();i++){
            if(i==0 && KeyValue<temp_node->Info[i].KeyValue){//第一个指针
                PARENTS.push_back(temp_node);
                temp_node=temp_node->childs[i];
                break;
            }else if(i==temp_node->Info.size()-1){//最后一个
                PARENTS.push_back(temp_node);
                temp_node=temp_node->childs[i+1];
                break;
            }else if(KeyValue>=temp_node->Info[i].KeyValue && KeyValue<temp_node->Info[i+1].KeyValue){
                PARENTS.push_back(temp_node);
                temp_node=temp_node->childs[i+1];
                break;
            }
        }
    }//find the leaf
    int flag=0;
    int pos=0;
    for(;pos<temp_node->Info.size();pos++){
        if(temp_node->Info[pos].KeyValue==KeyValue){
            flag=1;
            break;
        }
    }//pos是要被删除的位置
    if(flag==0)return Tree;//没找到则不做删除
    int degree=temp_node->degree;//阶数，为了方便写而已
    for(int j=pos;j<temp_node->Info.size()-1;j++){
        temp_node->Info[j]=temp_node->Info[j+1];
    }
    temp_node->Info.pop_back();//弹出最后一个元素
    //Node_print(temp_node);
    //cout<<temp_node->Info.size()<<endl;
    //for(int i=0;i<temp_node->Info.size();i++)cout<<temp_node->Info[i].KeyValue<<endl;
    //对temp_node完成删除
    Node<KeyType> *parent=PARENTS.back();
    //cout<<"parent"<<endl;
    //Node_print(parent);
    PARENTS.pop_back();
    // cout<<degree/2<<endl;
    if(temp_node->Info.size()>=(degree/2) || parent==NULL){//叶子结点数量够多或者是根节点 都不用修改叶子结点
        while(parent){
            //cout<<"check";
            //Node_print(parent);
            UpdateFathers(parent,temp_node);
            temp_node=parent;
            parent=PARENTS.back();
            PARENTS.pop_back();
        }
        // cout<<"Tree"<<endl;
        //Node_print(Tree);
        return temp_node;
    }else{//叶子结点数量不够多且不是根节点,需要修改
        //parent不是空
        Node<KeyType>* node1=NULL;
        Node<KeyType>* node2=NULL;
        if(temp_node->next!=NULL && find_child(parent,temp_node->next)==true){
            node1=temp_node;
            node2=temp_node->next;
        }else if(temp_node->pre!=NULL && find_child(parent,temp_node->pre)==true){
            node1=temp_node->pre;
            node2=temp_node;
        }//node1和node2是用于调整的两个结点 拥有同一个父结点 不需要再做一遍删除 注意父结点指针的分配
        //Node_print(node2);
        //cout<<node2->Info.size()<<endl;
        //cout<<node1->Info.size()+node2->Info.size()<<endl;

        if(node1->Info.size()+node2->Info.size()>degree){//叶子结点 不合并
            int half=(node1->Info.size()+node2->Info.size())/2;
            vector<Index_Info<KeyType>> temp_info;
            for(int j=0;j<node1->Info.size();j++){
                temp_info.push_back(node1->Info[j]);
            }
            for(int j=0;j<node2->Info.size();j++){
                temp_info.push_back(node2->Info[j]);
            }//temp_info储存node1 和 node2所有元素
            node1->Info.clear();
            node2->Info.clear();
            for(int j=0;j<half;j++){
                node1->Info.push_back(temp_info[j]);
            }
            for(int j=half;j<temp_info.size();j++){
                node2->Info.push_back(temp_info[j]);
            }//node1 和 node2 更新完毕
            if(temp_node->pre)temp_node->pre->next=node1;
            if(temp_node->next)temp_node->next->pre=node2;
            UpdateFathers(parent,node1);
            UpdateFathers(parent,node2);
            temp_node=parent;
            parent=PARENTS.back();
            PARENTS.pop_back();
            while(parent){
                UpdateFathers(parent,temp_node);
                temp_node=parent;
                parent=PARENTS.back();
                PARENTS.pop_back();
            }//更新父结点键值
            return Tree;
        }else{//需要合并 一定有非空父结点
            for(int j=0;j<node2->Info.size();j++){
                node1->Info.push_back(node2->Info[j]);
            }//删除node2
            //Node_print(node1);
            node1->next=node2->next;
            if(node2->next)node2->next->pre=node1;
            if(node1->pre)node1->pre->next=node1;
            Node<KeyType>* node3;
            Node<KeyType>* node4;
            while(parent){//temp_node在node1 node2的同一level
                if(PARENTS.back()==NULL){//父结点是根节点
                    //Node_print(node1);
                    if(parent->Info.size()==1){//仅有两个子节点 即为node1 node2
                        node1->pre==NULL;
                        node1->next==NULL;//第一次时是叶子结点
                        return node1;
                    }else{//不止两个子节点
                        for(pos=0;pos<parent->Info.size();pos++){
                            if(parent->childs[pos+1]==node2)break;
                        }
                        for(int j=pos;j<parent->Info.size()-1;j++){
                            parent->Info[j]=parent->Info[j+1];
                            parent->childs[j+1]=parent->childs[j+2];
                        }
                        UpdateFathers(parent,node1);
                        parent->childs.pop_back();
                        parent->Info.pop_back();
                        return parent;//因为parent是根节点
                    }
                }else{//父结点不是根节点 有不用重新分配 需要重新分配 合并三种情况
                    int flag=0;
                    int pos=1;
                    for(;pos<parent->childs.size();pos++){
                        if(parent->childs[pos]==node2){
                            break;
                        }
                    }//pos是要被删除的位
//                    cout<<"child:"<<endl;
//                    Node_print(parent->childs[pos-1]);
                    for(int j=pos;j<parent->childs.size()-1;j++){
                        parent->Info[j-1]=parent->Info[j];
                        parent->childs[j]=parent->childs[j+1];
                    }
                    UpdateFathers(parent,node1);
                    parent->Info.pop_back();//弹出最后一个元素
                    parent->childs.pop_back();
//                    cout<<"childs_size: "<<parent->childs.size()<<endl;
//                    cout<<"parent_check"<<endl;
//                    Node_print(parent);
                    //对parent完成删除
                    if(parent->childs.size()>=degree/2+1){//parent结点数量够多
                        temp_node=parent;
                        parent=PARENTS.back();
                        PARENTS.pop_back();
                        while(parent){
                            UpdateFathers(parent,temp_node);
                            temp_node=parent;
                            parent=PARENTS.back();
                            PARENTS.pop_back();
                        }
                        return Tree;
                    }else{//父结点要重新分配
                        node3=NULL;
                        node4=NULL;
                        if(parent->next!=NULL && find_child(PARENTS.back(),parent->next)==true){
                            node3=parent;
                            node4=parent->next;
                        }else if(parent->pre!=NULL && find_child(PARENTS.back(),parent->pre)==true){
                            node3=parent->pre;
                            node4=parent;
                        }//node3和node4是用于调整的两个结点 拥有同一个父结点 不需要再做一遍删除 注意父结点指针的分配
                        // cout<<"Parent_Node_check"<<endl;
//                        Node_print(node3);
//                        Node_print(node4);
                        if(node3->childs.size()+node4->childs.size()>=degree+2){//node3和node4不合并
                            int half=(node3->childs.size()+node4->childs.size())/2;
                            vector<Node<KeyType>*> temp_childs;
                            for(int j=0;j<node3->childs.size();j++){
                                temp_childs.push_back(node3->childs[j]);
                            }
                            for(int j=0;j<node4->childs.size();j++){
                                temp_childs.push_back(node4->childs[j]);
                            }//temp_childs储存node3 和 node4所有元素
                            node3->childs.clear();
                            node4->childs.clear();
                            node3->Info.clear();
                            node4->Info.clear();//先都清空
                            for(int j=0;j<half;j++){
                                node3->childs.push_back(temp_childs[j]);
                                if(j!=0){
                                    node3->Info.push_back(*new Index_Info<KeyType>);//随便push一个即可，因为马上会更新
                                    UpdateFathers(node3,temp_childs[j]);
                                }
                            }
                            for(int j=half;j<temp_childs.size();j++){
                                node4->childs.push_back(temp_childs[j]);
                                if(j!=half){
                                    node4->Info.push_back(*new Index_Info<KeyType>);//随便push一个即可，因为马上会更新
                                    UpdateFathers(node4,temp_childs[j]);
                                }
                            }//node3 和 node4 更新完毕

                            parent=PARENTS.back();
                            PARENTS.pop_back();
                            UpdateFathers(parent,node3);
                            UpdateFathers(parent,node4);

                            temp_node=parent;
                            parent=PARENTS.back();
                            PARENTS.pop_back();
                            while(parent){
                                UpdateFathers(parent,temp_node);
                                temp_node=parent;
                                parent=PARENTS.back();
                                PARENTS.pop_back();
                            }//更新父结点键值
                            return Tree;
                        }else{//node3 和 node4 要合并
                            for(int j=0;j<node4->childs.size();j++){
                                node3->childs.push_back(node4->childs[j]);
                                node3->Info.push_back(*new Index_Info<KeyType>);//随便push一个即可，因为马上会更新
                                UpdateFathers(node3,node4->childs[j]);
                            }//删除node4
                            if(temp_node->pre)temp_node->pre->next=node3;
                            if(temp_node->next)temp_node->next->pre=node3;
                            node3->next=node4->next;
                            node1=node3;
                            node2=node4;
                            temp_node=parent;
                            parent=PARENTS.back();
                            PARENTS.pop_back();
                        }
                    }
                }
            }
            return Tree;
        }
    }
}
#endif //FOCUS_INDEXMANAGER_BPLUSTREE_H
