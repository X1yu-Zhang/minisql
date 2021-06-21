#include "./RecordManager/RecordManager.h"
#include "./CatalogManager/CatalogManager.h"
#include "./BufferManager/BufferManager.h"
#include "Block.h"
#include "File.h"
#include "table.h"
#include "tuple.h"
#define SIZE 10000
Attribute ConstructAttr(){
    Attribute A;
    A.num = 4;
    A.type[0] = -1;
    A.type[1] = 20;
    A.type[2] = -1;
    A.type[3] = 0;
    A.has_index[0] = 1;
    A.index_name[0] = "t1_Primary";
    A.primary_key = 0;
    A.unique[1] = 1;
    A.name[0] = "id";
    A.name[1] = "name";
    A.name[2] = "age";
    A.name[3] = "salary";
    return A;
}
vector< Tuple > GetData(Table &t ){
    fstream file( "data.txt" ,ios::in);
    vector < Tuple > ret;
    int cnt = 0;
    while( cnt < SIZE ){
        Tuple tuple; 
        for(int i = 0 ; i < t.attr_.num ; i++ ){
            Data tmp;
            tmp.type = t.attr_.type[i];
            if( t.attr_.type[i] == -1 ){
                file >> tmp.datai;
            }else if( t.attr_.type[i] == 0 ){
                file >> tmp.dataf;
            }else{
                file >> tmp.datas;
                tmp.datas.resize( tmp.type );
            }
            tuple.addData(tmp);
        }
        ret.push_back(tuple);
        cnt ++ ;
    }
    return ret;   
}
int main(){
    CatalogManager cm;
    BufferManager bm;
    RecordManager rm(bm);
    vector <Where> condition;
    vector <string> Attrname;
    Table t1( "t1"  ,ConstructAttr() );
    cm.CreateTable(t1);
    Table & t = cm.GetTable( "t1" );
    for(int i = 0 ; i < t.attr_.num ; i ++ ){
        Attrname.push_back(t.attr_.name[i]);
    }
    vector <Tuple> tuples = GetData(t);
    for( int i = 0 ; i < tuples.size() ; i ++ ){
        rm.InsertRecord( t , tuples[i]);
    }
    cout << t.Unique["id"].size() << endl;
    vector <Tuple > res = rm.SelectRecord( t );
    rm.ShowTuple(res , t , Attrname);
    cout << t.Unique["id"].size() << endl;
    return 0;
} 