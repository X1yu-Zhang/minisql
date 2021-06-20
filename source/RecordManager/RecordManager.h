#ifndef _RECORDMANAGER_H_
#define _RECORDMANAGER_H_
#include "../BufferManager/buffermanager.h"
#include "../tuple.h"
#include "../table.h"
class RecordManager {
private:
    BufferManager& bm;
    Tuple & SelectRecord( Table &t, vector < string >& AttributeName ,vector<Where>& where );
    Tuple & SelectRecordWithIndex( );
    int DeleteRecord( vector <Where> & where );
    int InsertRecord( Data );
};
#endif