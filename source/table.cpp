#include "table.h"
Table::Table(string title, Attribute attr) {
    this->title_ = title;
    this->attr_ = attr;
    this->index_.num = 0;
}

//table�Ĺ��캯����������
Table::Table(const Table& table_in) {
    this->attr_ = table_in.attr_;
    this->index_ = table_in.index_;
    this->title_ = table_in.title_;
    for (int index = 0; index < tuple_.size(); index++)
        this->tuple_.push_back(table_in.tuple_[index]);
}

//����Ԫ��
 int Table::addTuple(Tuple tuple_in){
     if(tuple_in.getSize()!=attr_.num)
    {
         cout<<"Illegal Tuple Insert: The size of column is unequal."<<endl;
         return 0;
     }
     for(int index=0;index<attr_.num;index++){
        //�������tuple��typeΪint����floatʱ���������Ӧ��attr_.type��ͬ�����Ϊstring�Ļ�������Ҫ������attr_.type
        if(tuple_in.getData()[index].type>attr_.type[index]||(tuple_in.getData()[index].type<=0&&tuple_in.getData()[index].type!=attr_.type[index]))
         {
             cout<<"Illegal Tuple Insert: The types of attributes are unequal."<<endl;
             return 0;
        }
     }
     tuple_.push_back(tuple_in);
     return 1;
 }

//��������
int Table::setIndex(short index, string index_name) {
    short tmpIndex;
    for (tmpIndex = 0; tmpIndex < index_.num; tmpIndex++) {
        if (index == index_.location[tmpIndex]|| index_name == index_.indexname[tmpIndex])  //����Ԫ���Ѿ�������ʱ������
        {
            cout << "Illegal Set Index: The index has been in the table." << endl;
            return 0;
        }
    }
    index_.location[index_.num] = index;  //��������λ�ú��������֣���������������һ
    index_.indexname[index_.num] = index_name;
    index_.num++;
    return 1;
}

int Table::dropIndex(string index_name) {
    int tmpIndex;
    for (tmpIndex = 0; tmpIndex < index_.num; tmpIndex++) {
        if (index_name == index_.indexname[tmpIndex])  //����Ԫ���Ѿ�������ʱ������
            break;
    }
    if (tmpIndex == index_.num)
    {
        cout << "Illegal Drop Index: No such a index in the table." << endl;
        return 0;
    }

    //�����һ��index����ɾ����
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
    t.ConstructMap();
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

void Table :: ConstructMap(){
    for(int i = 0 ; i < this->attr_.num ; i ++ ){
        AttrName2Index.insert( make_pair( this->attr_.name[i] , i ) );
    }
}

vector<int >& Table :: ConvertIntoIndex( const vector<string> AttrName ){
    vector<int>ret(AttrName.size());
    for(int i = 0;i < AttrName.size() ; i++ ){
        ret[i] = AttrName2Index[AttrName[i]];
    }
    return ret;
}