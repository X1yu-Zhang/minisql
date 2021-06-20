#include "CatalogManager.h"
CatalogManager :: CatalogManager(){
    fstream f1("./data/catalog/tablenameset.db",ios::in);
    fstream f2("./data/catalog/indexnameset.db",ios::in);
    string tmp;
    while( f1 >> tmp ){
        Table t(tmp);
        fstream file("./data/catalog/"+tmp+".db",ios::in);
        file >> t;
        TABLESET.insert(make_pair(tmp,t));
        file.close();
    }
    while( f2 >> tmp ){
        string name;
        f2 >> name;
        INDEXSET.insert(make_pair(tmp,name));
    }
    f1.close();
    f2.close();
}
CatalogManager :: ~CatalogManager(){
    fstream f1("./data/catalog/tablenameset.db",ios::in);
    fstream f2("./data/catalog/indexnameset.db",ios::in);
    map<string, Table > :: iterator STItor;
    map<string, string > :: iterator SSItor;
    for( STItor = TABLESET.begin() ; STItor != TABLESET.end() ; STItor ++ ){
        string tmp = STItor->first;
        f1 << tmp << endl;
        fstream file("./data/catalog/"+tmp+".db",ios::out);
        file << STItor->second;
        TABLESET.erase(tmp);
        file.close();
    }
    for( SSItor = INDEXSET.begin() ; SSItor != INDEXSET.end() ; SSItor ++ ){
        f2 << SSItor->first << " " << SSItor->second << endl;
    }
    f1.close();
    f2.close();
}


Table & CatalogManager :: GetTable( string table_name ){
    return TABLESET[table_name];
}

bool CatalogManager :: CreateTable( Table & t ){
    TABLESET.insert( make_pair( t.getTitle() , t ) );
    INDEXSET.insert( make_pair( t.getTitle()+"_primary", t.getTitle() ));
}

bool CatalogManager :: DeleteTable( string name ){
    Table &t = TABLESET[name];
    int n = t.attr_.num;
    remove( ("./data/catalog"+name+".db").c_str() );
    remove( ("./data/record/"+name+".db").c_str() );
    for(int i = 0 ; i < n ; i ++ ){
        if( t.attr_.has_index[i] )
        remove( ("./data/index/"+t.attr_.index_name[i]+".db").c_str() );
    }
    TABLESET.erase( name );
    return true;
}

bool CatalogManager :: DeleteIndex( string name ){
    if ( !FindIndex( name ) ) return false;
    Table & t = TABLESET[INDEXSET[name]];
    for( int i = 0 ; i < t.attr_.num ; i ++ ){
        if( t.attr_.index_name[i] == name ){
            t.attr_.has_index[i] = false;
            t.attr_.index_name[i] = "";
            remove( ("./data/record/"+t.attr_.index_name[i]+".db").c_str() );
            break;
        }
    }
    INDEXSET.erase( name );
}
bool CatalogManager :: CreateIndex( string name, string table_name ){
    INDEXSET.insert( make_pair( name , table_name ));
}
bool CatalogManager :: FindIndex( string name ){
    map<string, string > :: iterator SSItor = INDEXSET.find(name);
    return SSItor != INDEXSET.end(); 
}
bool CatalogManager :: FindTable( string name ){
    map<string, Table > :: iterator STItor = TABLESET.find(name);
    return STItor != TABLESET.end();
}