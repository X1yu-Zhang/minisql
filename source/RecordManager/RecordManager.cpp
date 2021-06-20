#include "RecordManager.h"

Tuple SelectWithIndex( Table & t , string AttributeName, string Index_name , Where where); 
void RecordConditionFit(Tuple &t , vector < string > AttributeName ,vector<Where> where);
Tuple SelectRecord( Table &t, vector < string > AttributeName ,vector<Where> where ){
    Tuple ret;
    Attribute attr = t.getAttr();
    for( int i = 0;i < AttributeName.size() ; i ++ ){
        int index = t.AttrName2Index[AttributeName[i]];
        if( attr.has_index[index] ){
            ret = SelectWithIndex( t , AttributeName[i], attr.index_name[index], where[i] );
            AttributeName.erase(AttributeName.begin()+i);
            where.erase(where.begin()+i);
            break;
        }
    }
    RecordConditionFit( ret , AttributeName, where );
}

void DeleteRecord( string Table_name , vector<string> target_attr, vector<Where> where );

void DropTable( string Table_name ); 

void InsertRecord( string table_name, Tuple& tuple );