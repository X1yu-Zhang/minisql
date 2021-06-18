#include "table.h"
Table::Table(string title, Attribute attr) {
    this->title_ = title;
    this->attr_ = attr;
    this->index_.num = 0;
}

//table的构造函数，拷贝用
Table::Table(const Table& table_in) {
    this->attr_ = table_in.attr_;
    this->index_ = table_in.index_;
    this->title_ = table_in.title_;
    for (int index = 0; index < tuple_.size(); index++)
        this->tuple_.push_back(table_in.tuple_[index]);
}

//插入元组
 int Table::addTuple(Tuple tuple_in){
     if(tuple_in.getSize()!=attr_.num)
    {
         cout<<"Illegal Tuple Insert: The size of column is unequal."<<endl;
         return 0;
     }
     for(int index=0;index<attr_.num;index++){
        //当输入的tuple的type为int或者float时，必须与对应的attr_.type相同。如果为string的话，则需要不大于attr_.type
        if(tuple_in.getData()[index].type>attr_.type[index]||(tuple_in.getData()[index].type<=0&&tuple_in.getData()[index].type!=attr_.type[index]))
         {
             cout<<"Illegal Tuple Insert: The types of attributes are unequal."<<endl;
             return 0;
        }
     }
     tuple_.push_back(tuple_in);
     return 1;
 }

//插入索引
int Table::setIndex(short index, string index_name) {
    short tmpIndex;
    for (tmpIndex = 0; tmpIndex < index_.num; tmpIndex++) {
        if (index == index_.location[tmpIndex]|| index_name == index_.indexname[tmpIndex])  //当该元素已经有索引时，报错
        {
            cout << "Illegal Set Index: The index has been in the table." << endl;
            return 0;
        }
    }
    index_.location[index_.num] = index;  //插入索引位置和索引名字，最后把索引个数加一
    index_.indexname[index_.num] = index_name;
    index_.num++;
    return 1;
}

int Table::dropIndex(string index_name) {
    int tmpIndex;
    for (tmpIndex = 0; tmpIndex < index_.num; tmpIndex++) {
        if (index_name == index_.indexname[tmpIndex])  //当该元素已经有索引时，跳出
            break;
    }
    if (tmpIndex == index_.num)
    {
        cout << "Illegal Drop Index: No such a index in the table." << endl;
        return 0;
    }

    //把最后一个index覆盖删除项
    index_.indexname[tmpIndex] = index_.indexname[index_.num - 1];
    index_.location[tmpIndex] = index_.location[index_.num - 1];
    index_.num--;
    return 1;
}


string Table::getTitle() {
    return title_;
}
Attribute Table::getAttr() {
    return attr_;
}
vector<Tuple>& Table::getTuple() {
    return tuple_;
}
Index Table::getIndex() {
    return index_;
}

short Table::gethasKey(){
    return attr_.primary_key;
}
void Table::showTable() {
    for (int index = 0; index < attr_.num; index++)
        cout << attr_.name[index] << '\t';
    cout << endl;
    for (int index = 0; index < tuple_.size(); index++)
        tuple_[index].showTuple();
}

void Table::showTable(int limit) {
    for (int index = 0; index < attr_.num; index++)
        cout << attr_.name[index] << '\t';
    cout << endl;
    for (int index = 0; index < limit && index < tuple_.size(); index++)
        tuple_[index].showTuple();
}
int Table :: HasAttribute( string AttributeName ){
    for ( int i = 0 ; i < attr_.num ; i ++ ){
        if( attr_.name[i] == AttributeName ){
            return i;
        }
    }
    return -1;
}

string Table :: GetIndexName( int t ){
    if( t == -1 ) return "";
    return this->attr_.index_name[t];
}

void Table :: SetTitle( string name ){
    title_ = name;
}
istream& operator>>( istream & in , Table & t){
    in >> t.attr_.num >> t.attr_.primary_key;
    for(int i = 0; i < t.attr_.num; i ++ ){
        in >> t.attr_.name[i] >> t.attr_.type[i];
        in >> t.attr_.unique[i] >> t.attr_.has_index[i];
        if ( t.attr_.has_index[i] ) in >> t.attr_.index_name[i];
    }
}

ostream& operator<<( ostream & out , const Table & t){
    out << t.attr_.num << t.attr_.primary_key << endl;
    for(int i = 0; i < t.attr_.num; i ++ ){
        out << t.attr_.name[i] << t.attr_.type[i];
        out << t.attr_.unique[i] << t.attr_.has_index[i];
        if ( t.attr_.has_index[i] ) out << t.attr_.index_name[i];
        out << endl;
    }
}