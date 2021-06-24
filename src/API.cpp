#include "../include/API.h"
#include <algorithm>
#include <vector>
#include <iterator>


//���캯��


//��������
API::~API() {}

//������ı���where��������������ѡ���ӦԪ�飬������һ����
void API::selectRecord(string table_name, vector<string> DispAttr ,vector<string> target_attr, vector<Where> where )
{
    Table & t = catalog.GetTable(table_name);
    for(int i = 0 ; i < DispAttr.size() ; i++ ){
        if( i == 0 && DispAttr.size() == 1 && DispAttr[0] == "*" ){
            DispAttr.erase(DispAttr.begin());
            for(int j = 0 ; j < t.attr_.num ; j ++ ){
                DispAttr.push_back(t.attr_.name[j]);
            }
            break;
        }else if( !t.HasAttribute( DispAttr[i] ) ){
            throw attribute_not_exist();
        }
    }
    vector <Tuple> ret = record.SelectRecord( t , target_attr , where );
    record.ShowTuple(ret , t , DispAttr);
}
//ɾ�������������where���������µ�Ԫ��
void API::deleteRecord(string table_name, vector <string> target_attr, vector <Where> where)
{
    if( !catalog.HasTable( table_name ) ) throw table_not_exist();
    Table &t = catalog.GetTable( table_name );
    for( int i = 0 ; i < target_attr.size() ; i ++ ){
        if( ! t.HasAttribute ( target_attr[i]) ) throw attribute_not_exist();
    }
    record.DeleteRecord( t , target_attr , where);
    
}

//����в���tuple
void API::insertRecord(string table_name, Tuple& tuple)
{
    if( !catalog.HasTable( table_name ) ) throw table_not_exist();
    Table &t = catalog.GetTable( table_name );
    if( tuple.getData().size() != t.attr_.num ) throw input_format_error();
    for(int i = 0 ; i < t.attr_.num ; i++ ){
        if( t.attr_.type[i] != tuple.getData()[i].type ){
            throw data_type_conflict();
        }
    }
    record.InsertRecord( t , tuple);
}

//������
void API::createTable( string table_name, Attribute &attribute )
{
    if( catalog.HasTable( table_name ) ) throw table_not_exist();
    Table t( table_name , attribute );
    catalog.CreateTable( t );
    if( attribute.primary_key >= 0 ){
        int KeySize = attribute.type[attribute.primary_key] < 1 ? 4 : attribute.type[attribute.primary_key];
        string KeyType ;
        if( attribute.type[attribute.primary_key] == -1 ) KeyType = "int";
        else if( attribute.type[attribute.primary_key] == 0 ) KeyType = "float";
        else KeyType = "string";
        indexm.Create_Index(attribute.index_name[attribute.primary_key] , KeySize , KeyType );
    }
    t.ShowTableInfo();
}

//ɾ�����Ͷ�ӦԪ����Ϣ
void API::dropTable(string table_name)
{
    if( !catalog.HasTable( table_name ) ) throw table_not_exist();
    catalog.DropTable( table_name );
    record.DropTable( table_name );
}
//��
void API::createIndex(string table_name, string index_name, string attr_name)
{
    if (!catalog.HasTable( table_name ) ) throw table_exist();
    if ( catalog.HasIndex( index_name ) ) throw index_exist();
    Table &t = catalog.GetTable(table_name);
    if( !t.HasAttribute( attr_name ) ) throw attribute_not_exist();
    record.CreateIndex( t , attr_name , index_name);
    catalog.CreateIndex( index_name , table_name , attr_name );
}
void API::dropIndex( string index_name )
{
    if ( !catalog.HasIndex( index_name ) ) throw index_not_exist();
    catalog.DeleteIndex( index_name );
}
