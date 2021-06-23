#include "../include/RecordManager.h"

Data ConverseIntoData( char * data, int type ){ 
    Data ret;
    ret.type = type;
    if( type == -1 ){
        ret.datai = *((int*) data);
    }else if ( type == 0 ){
        ret.dataf = *((float*)data);
    }else{
        string tmp(data, type);
        ret.datas = tmp;
    }
    return ret;
}
Tuple ConverseIntoTuple( char * data , Attribute * attr ){
    Tuple ret;
    for(int i = 0 ; i < attr->num ; i ++ ){
        ret.addData( ConverseIntoData( data , attr->type[i] ) );
        data += attr->type[i] < 1 ? 4 : attr->type[i];
    }
    return ret;
}
string Data2String( Data d ){
    if ( d.type < 0){
        string ret( (char*) (&d.datai) , 4);
        return ret;
    }else if( d.type == 0 ){
        string ret( (char*) (&d.dataf) , 4);
        return ret;
    }else {
        string ret = d.datas;
        ret.resize( d.type );
        return ret;
    }
    return "";
}

vector<Tuple> RecordManager :: SelectWithIndex( Table &t ,File * file , string Index_name , Where where){
    vector <Tuple> ret;
//    Index_Where<KeyType> condi;
//    if(where.data.type==-1){
//        condi.relation_character=where.relation_character;
//        condi.data=where.data.datai;
//    }else if(where.data.type==0){
//        condi.relation_character=where.relation_character;
//        condi.data=where.data.dataf;
//    }else{
//        condi.relation_character=where.relation_character;
//        condi.data=where.data.datas;
//    }
    vector <Search_Info> Fetch = im.Search_In_Index( Index_name, where);
    if( Fetch.size() == 0 ) return ret;
    int size = t.GetLength();
    for( int i = 0 ; i < Fetch.size() ; i ++ ){
        Tuple tp;
        Block * tmp = bm.GetBlockByNum(file , Fetch[i].Block_Offset);
        char * data = tmp->GetContent();
        data += Fetch[i].Offset_in_Block;
        if( *data == 0 ){
            for( int i = 0; i < t.attr_.num ; i ++  ){
                tp.addData( ConverseIntoData( data ,t.attr_.type[i] ) );
                data += ( t.attr_.type[i] < 1 ) ?  4 : t.attr_.type[i];
            }
            ret.push_back(tp);
        }
    }
    return ret;
}
bool  RecordManager :: RecordConditionFit(Tuple &t , vector < int > &AttributeName ,vector<Where> &where){
    vector < Data > D = t.getData();
    bool ret = true;
    for(int i = 0 ; i < where.size() ; i ++ ){
        bool res;
        switch (where[i].relation_character){
            case LESS:
                if( where[i].data.type == -1 )
                    res = D[AttributeName[i]].datai < where[i].data.datai;
                else if ( where[i].data.type == 0 )
                    res = D[AttributeName[i]].dataf < where[i].data.dataf;
                else res = D[AttributeName[i]].datas < where[i].data.datas;
            break;
            case LESS_OR_EQUAL:
                if( where[i].data.type == -1 )
                    res = D[AttributeName[i]].datai <= where[i].data.datai;
                else if ( where[i].data.type == 0 )
                    res = D[AttributeName[i]].dataf <= where[i].data.dataf;
                else res = D[AttributeName[i]].datas <= where[i].data.datas;
            break;
            case EQUAL:
                if( where[i].data.type == -1 )
                    res = D[AttributeName[i]].datai == where[i].data.datai;
                else if ( where[i].data.type == 0 )
                    res = D[AttributeName[i]].dataf == where[i].data.dataf;
                else res = D[AttributeName[i]].datas == where[i].data.datas;
            break;
            case GREATER_OR_EQUAL:
                if( where[i].data.type == -1 )
                    res = D[AttributeName[i]].datai >= where[i].data.datai;
                else if ( where[i].data.type == 0 )
                    res = D[AttributeName[i]].dataf >= where[i].data.dataf;
                else res = D[AttributeName[i]].datas >= where[i].data.datas;
            break;
            case GREATER:
                if( where[i].data.type == -1 )
                    res = D[AttributeName[i]].datai > where[i].data.datai;
                else if ( where[i].data.type == 0 )
                    res = D[AttributeName[i]].dataf > where[i].data.dataf;
                else res = D[AttributeName[i]].datas > where[i].data.datas;
            break;
            case NOT_EQUAL:
                if( where[i].data.type == -1 )
                    res = D[AttributeName[i]].datai != where[i].data.datai;
                else if ( where[i].data.type == 0 )
                    res = D[AttributeName[i]].dataf != where[i].data.dataf;
                else res = D[AttributeName[i]].datas != where[i].data.datas;
            break;
        }
        ret = ret & res;
    } 
    return ret;
}
vector<Tuple> RecordManager :: SelectRecord( Table &t, vector < string > &AttributeName ,vector<Where> &where ){
    vector <Tuple> ret;
    Attribute attr = t.attr_;
    vector < int > AttrIndex = t.ConvertIntoIndex( AttributeName );
    File * file = bm.GetFile( t.getTitle(), 0 );
    bool no_index = true;
    for( int i = 0;i < AttributeName.size() ; i ++ ){
        int index = t.AttrName2Index[AttributeName[i]];
        if( attr.has_index[index] ){
            ret = SelectWithIndex( t , file ,attr.index_name[index], where[i] );
            AttributeName.erase(AttributeName.begin()+i);
            where.erase(where.begin()+i);
            no_index = false;
            break;
        }
    }
    if ( no_index ){
        for( Block * tmp = bm.GetBlockHead(file) ; !tmp->IsEnd() ; tmp = bm.GetNextBlock(file , tmp ) ){
            char * data = tmp->GetContent();
            for(int i = 0 ; i + t.GetLength() < BLOCK_SIZE ; i += t.GetLength()+1 ){
                if( *(data+i) == 1 ) continue;
                Tuple tempTuple = ConverseIntoTuple( data + i + 1 , & t.attr_ );
                if ( RecordConditionFit( tempTuple , AttrIndex , where ) ){
                    ret.push_back(tempTuple);
                }
            }
        } 
    }else if ( !ret.size() ){
        for( int i = 0 ; i < ret.size() ; i ++ ){
            if( !RecordConditionFit( ret[i] , AttrIndex , where ) ){
                ret.erase(ret.begin() + i);
            }
        }
    }
    return ret;
}
vector<Tuple> RecordManager :: SelectRecord( Table &t ){
    vector <Tuple> ret;
    Attribute attr = t.attr_;
    File * file = bm.GetFile( t.getTitle(), 0 );
    int flag = 1;
    for( Block * tmp = bm.GetBlockHead(file) ; flag ; tmp = bm.GetNextBlock(file , tmp ) ){
        char * data = tmp->GetContent();
        for(int i = 0 ; i + t.GetLength() < tmp->GetUsingSize() ; i += t.GetLength()+1 ){
            if( *(data+i) == 1 ) continue;
            Tuple tempTuple = ConverseIntoTuple( data + i + 1 , & t.attr_ );
            ret.push_back(tempTuple);
        }
        if( tmp->IsEnd() ) flag = 0;
    } 
    return ret;
}
int RecordManager :: DeleteRecord(  Table & t , vector<string> target_attr, vector<Where> where ){
    string Index_name ;
    vector <int > AttrIndex = t.ConvertIntoIndex( target_attr );
    int SearchIndex = -1;
    for(int i = 0 ; i < AttrIndex.size() ; i ++ ){
        if ( t.attr_.has_index[AttrIndex[i]] ) {
            Index_name = t.attr_.index_name[AttrIndex[i]];
            SearchIndex = i;
            break;
        }
    }
    File * file = bm.GetFile( t.getTitle() , 0 ); 
    int cnt = 0;
    if( SearchIndex != -1 ){
        vector <Search_Info> Fetch;
//        Index_Where<KeyType> condi;
//        if( where[SearchIndex].data.type == -1 ){
//            condi.relation_character = where[SearchIndex].relation_character;
//            condi.data = where[SearchIndex].data.datai;
//        }else if(where[SearchIndex].data.type == 0 ){
//            condi.relation_character = where[SearchIndex].relation_character;
//            condi.data = where[SearchIndex].data.dataf;
//        }else{
//            condi.relation_character = where[SearchIndex].relation_character;
//            condi.data = where[SearchIndex].data.dataf;
//        }
        Fetch = im.Search_In_Index( Index_name, where[SearchIndex] );
        if ( Fetch.size() ){
            for( int i = 0 ;i < Fetch.size() ; i++ ){
                Block * tmp = bm.GetBlockByNum( file , Fetch[i].Block_Offset );
                Tuple tempTuple = ConverseIntoTuple(tmp->GetContent() + 1 + i, &t.attr_ );
                if ( RecordConditionFit( tempTuple , AttrIndex , where ) ){
                    for( int i = 0 ; i < t.attr_.num; i ++ ){
                        if( t.attr_.has_index[i] ){
                            string tmp = Data2String( tempTuple.getData()[i] );
                            im.Delete_From_Index( t.attr_.index_name[i], tmp );
                        }
                    }
                    char DeletedFlag = 1;
                    tmp->write( Fetch[i].Offset_in_Block , &DeletedFlag , 1 );
                    RecordFreeList node = new FreeListNode;
                    file->AppendFreeList( Fetch[i].Block_Offset , Fetch[i].Offset_in_Block );
                    cnt ++ ;
                }
            }
        }
    }else{
        for( Block * tmp = bm.GetBlockHead(file) ; !tmp->IsEnd() ; tmp = bm.GetNextBlock(file , tmp ) ){
            for(int i = 0 ; i + t.GetLength() < BLOCK_SIZE ; i += t.GetLength()+1 ){
                Tuple tempTuple = ConverseIntoTuple( tmp->GetContent() + 1 , & t.attr_ );
                if ( RecordConditionFit( tempTuple , AttrIndex , where ) ){
                    char DeletedFlag = 1;
                    tmp->write( i , &DeletedFlag , 1 );
                    RecordFreeList node = new FreeListNode;
                    file->AppendFreeList( tmp->GetBlockOffsetNum() , i );
                    cnt ++ ;
                }
            }
        } 
    }
    return cnt;
}
int RecordManager :: DeleteRecord ( Table & t ){
    string Index_name ;
    File * file = bm.GetFile( t.getTitle() , 0 ); 
    int cnt = 0;
    for( Block * tmp = bm.GetBlockHead(file) ; 1 ; tmp = bm.GetNextBlock(file , tmp ) ){
        for(int i = 0 ; i + t.GetLength() < tmp->GetUsingSize() ; i += t.GetLength()+1 ){
            Tuple tempTuple = ConverseIntoTuple( tmp->GetContent() + 1 + i , & t.attr_ );
            char DeletedFlag = 1;
            tmp->write( i , &DeletedFlag , 1 );
            file->AppendFreeList( tmp->GetBlockOffsetNum() , i );
            DeleteUnique( t , tempTuple );
            cnt ++ ;
        }
        if( tmp->IsEnd() ) break;
    } 
    return cnt;
}
void RecordManager :: DropTable( string TableName){
    File * file = bm.GetFile(TableName , 0);
    bm.DeleteFileFromList( TableName );
}
void RecordManager :: DeleteUnique( Table & t , Tuple & tuple ){
    CheckUnique( t , tuple , true ); 
}
bool RecordManager :: CheckUnique( Table & t , Tuple & tuple, bool flag ){
    bool ret = true;
    for( int i = 0 ; i < t.attr_.num ; i ++ ){
        if( t.attr_.unique[i] || i == t.attr_.primary_key ){
            unordered_set < string > & ValueSet = t.Unique[t.attr_.name[i]] ;
            string data = Data2String( tuple.getData()[i] );
            unordered_set < string >  ::iterator SETITOR = ValueSet.find( data );
            if( flag ){
                if( SETITOR != ValueSet.end() ){
                    ValueSet.erase( data );
                }
            }else{
                if( SETITOR == ValueSet.end() ){
                    ValueSet.insert( data );
                }else {
                    ret = false;
                    break;
                }
            }
        }
    }
    return ret;
}
int RecordManager :: InsertRecord( Table &t , Tuple& tuple ){
    if( !CheckUnique( t , tuple ) )  return 0;
    File * file = bm.GetFile( t.getTitle(),0 );
    string data("\0",1);
    for( int i = 0 ; i < tuple.getData().size(); i++ ){
        data += Data2String(tuple.getData()[i]);
    }
    int OffsetNum , offset ;
    if( file->freelist == NULL ){
        Block * tmp = bm.GetBlockHead( file );
        while( !tmp->IsEnd() ) tmp = bm.GetNextBlock( file , tmp );
        if( tmp->GetUsingSize() + t.GetLength() + 1 > BLOCK_SIZE){
            tmp->SetUsingSize(BLOCK_SIZE);
            Block * nxt = bm.GetEmptyBlock( file );
            tmp->SetNext( nxt );
            nxt->SetOffsetNum( tmp->GetBlockOffsetNum() + 1 );
            tmp->SetEnd( false );
            nxt->SetEnd( true );
            tmp = nxt;
        }
        offset = tmp->GetUsingSize();
        OffsetNum = tmp->GetBlockOffsetNum();
        tmp->write( offset , data.c_str() , data.size() );
        tmp->SetUsingSize( offset + data.size() );
    }else{
        RecordFreeList now = file->freelist;
        file->freelist = file->freelist->next;
        now->next = NULL ;
        OffsetNum = now->BlockNum;
        offset = now->offset;
        delete now;
        Block * tmp = bm.GetBlockByNum(file , OffsetNum);
        tmp->write( offset , data.c_str() , data.size() ); 
    }
    for( int i = 0 ; i < t.attr_.num; i ++ ){
        if( t.attr_.has_index[i] ){
            string tmp = Data2String( tuple.getData()[i] );
            string KeyType;
            if( t.attr_.type[i] == -1) KeyType = "int";
            else if ( t.attr_.type[i] == 0 ) KeyType = "float";
            else KeyType = "string"; 
            im.Insert_Into_Index( t.attr_.index_name[i] ,tmp,  KeyType , OffsetNum , offset );
        }
    }
    return 1;
}
void RecordManager :: CreateIndex( Table &t , string AttrName , string index_name  ){
    int index = t.AttrName2Index[AttrName];
    int KeySize = t.attr_.type[index] < 1 ? 4 : t.attr_.type[index];
    string KeyType;
    if( t.attr_.type[index] == -1) KeyType = "int";
    else if ( t.attr_.type[index] == 0 ) KeyType = "float";
    else KeyType = "string"; 
    im.Create_Index( index_name ,KeySize , KeyType);
    File * file = bm.GetFile( t.getTitle() , 0);
    int offset = 1;
    for(int i = 0 ; i < index ; i ++ ){
        offset += t.attr_.type[i] < 1 ? 4 : t.attr_.type[i];
    } 
    int RecordSize = t.GetLength();
    for( Block * tmp = bm.GetBlockHead(file) ;tmp ; tmp = bm.GetNextBlock( file , tmp)){
        char* data = tmp->GetContent();
        for(int i = 0 ; i + RecordSize < BLOCK_SIZE ; i += RecordSize + 1){
            if( *(data+i) == 1 ) continue;
            string KeyValue( data + i + 1 , KeySize);
            im.Insert_Into_Index( index_name , KeyValue , KeyType , tmp->GetBlockOffsetNum() , i);
        }
    }
}

ostream & operator << ( ostream & out , Data tmp ){
    if( tmp.type == -1 ) out << tmp.datai;
    else if( tmp.type == 0 ) out << tmp.dataf;
    else out << tmp.datas;
    return out;
}
void RecordManager :: ShowTuple( vector <Tuple> &tuples , Table & t , vector <string >AttrName ){
    int all_length = 0;
    if( AttrName.size() == 0 ){
        cout << "There is " << 0 << " record shown as above" << endl;
        return ;
    }
    vector < int > index = t.ConvertIntoIndex( AttrName );
    for(int i = 0 ; i < index.size() ; i ++ ){
        int length = t.attr_.type[index[i]] < 1 ? 10 : t.attr_.type[index[i]] ;
        all_length += length;
        cout << "|" << left << setw(length) << t.attr_.name[index[i]];
    }
    cout << "|" << endl;
    for(int i = 0 ; i < all_length+t.attr_.num ; i++ )cout << "-" ;
    cout << endl;
    for(int i = 0; i < tuples.size() ; i++ ){
        for(int j = 0 ; j < index.size() ; j ++ ){
            int length = t.attr_.type[index[j]] < 1 ? 10 : t.attr_.type[index[j]];
            if( t.attr_.type[index[j]] == -1 )
            cout << "|" << left << setw(length) << tuples[i].getData()[index[j]].datai; 
            else if( t.attr_.type[index[j]] == 0 )
            cout << "|" << left << setw(length) << tuples[i].getData()[index[j]].dataf;
            else 
            cout << "|" << left << setw(length) << tuples[i].getData()[index[j]].datas.c_str();
        }
        cout << "|" << endl;
        for(int j = 0 ; j < all_length+t.attr_.num ; j++ )cout << "-" ;
        cout << endl;
    }
    cout << endl;
    cout << "There are " << tuples.size() << " records shown as above" << endl;
}
