#ifndef _RECORDMANAGER_H_
#define _RECORDMANAGER_H_
#include "buffermanager.h"
#include "IndexManager.h"
#include <iomanip>
#include "tuple.h"
#include "table.h"
class RecordManager {
private:
    BufferManager& bm;
    IndexManager & im;
public:
    RecordManager ( BufferManager &bm_ , IndexManager &im_):bm(bm_),im(im_){};
    int DeleteRecord( Table & t , vector<string> target_attr, vector<Where> where );
    int DeleteRecord ( Table & t );
    void DeleteUnique( Table & t , Tuple & tuple );
    void DropTable( string Table_name ); 
    int InsertRecord( Table &t, Tuple& tuple ); 
    bool RecordConditionFit(Tuple &t , vector < int > &AttributeName ,vector<Where>& where);
    vector <Tuple> SelectRecord( Table &t, vector < string > &AttributeName ,vector<Where>& where );
    vector <Tuple> SelectRecord( Table &t );
    vector <Tuple> SelectWithIndex(Table &t , File * file , string Index_name , Where where);
    bool CheckUnique( Table & t , Tuple & tuple, bool flag = false );
    void CreateIndex( Table &t , string AttrName , string index_name  );
    void ShowTuple( vector <Tuple>& tuples , Table & t , vector <string >AttrName);
};
#endif