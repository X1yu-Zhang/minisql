#include "./RecordManager/RecordManager.h"
#include "./CatalogManager/CatalogManager.h"
#include "./BufferManager/BufferManager.h"
#include "Block.h"
#include "File.h"
#include "table.h"
#include "tuple.h"
#define SIZE 1000
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

int main(){
    BufferManager bm;
    CatalogManager cm;
    RecordManager rm(bm);
    vector <Where> condition;
    vector <string> Attrname;
    Table &t = cm.GetTable("t1");
    cout << t.Unique["id"].size() << endl;
    vector <Tuple > res = rm.SelectRecord( t );
    for(int i = 0 ; i < t.attr_.num ; i ++ ){
        Attrname.push_back(t.attr_.name[i]);
    }
    rm.ShowTuple(res , t , Attrname);
    cm.DropTable( t.getTitle() );
    return 0;
} 