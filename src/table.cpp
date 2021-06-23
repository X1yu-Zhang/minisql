#include "table.h"
Table::Table(string title, Attribute attr) {
    this->title_ = title;
    this->attr_ = attr;
    this->length = 0;
    for(int i = 0 ; i < attr.num; i ++ ){
        this->length += ( attr.type[i] < 1 ) * 4 + ( attr.type[i] >= 1 ) * attr.type[i];  
        AttrName2Index.insert(make_pair( attr_.name[i] , i ));  
        if( attr_.unique[i] || i == attr_.primary_key ){
            unordered_set<string> tmp;
            Unique.insert(make_pair( attr_.name[i], tmp));
        }
    }
}
//table�Ĺ��캯����������
Table::Table(const Table& table_in) {
    this->attr_ = table_in.attr_;
    this->title_ = table_in.title_;
    this->length = table_in.length;
    AttrName2Index = table_in.AttrName2Index;
    Unique = table_in.Unique;
}
void Table :: ReadUnique(){
    int cnt = 0;
    for(int i = 0 ; i < this->attr_.num ; i ++ ){
        if ( this->attr_.unique[i] || i == this->attr_.primary_key ){
            fstream file("./data/catalog/Unique/"+ this->getTitle() + "_"+ this->attr_.name[i]+".db", ios::in);
            while( file.is_open() ){
                unordered_set<string> & AttrUnique = Unique[this->attr_.name[i]];
                int length = this->attr_.type[i] < 1 ? 4 : this->attr_.type[i]; 
                char data[length+1];
                file.read( data , length );
                if( file.eof() ) break;
                string tmp( data , length );
                AttrUnique.insert( tmp );
            }
            file.close();
        }
    }
}
void Table :: WriteUnique(){
    map<string, unordered_set<string>> :: iterator ITOR ;
    for( ITOR = Unique.begin() ; ITOR != Unique.end(); ITOR ++ ){
        fstream file("./data/catalog/Unique/"+ this->getTitle() + "_"+ ITOR->first +".db",ios::out | ios :: binary);
        for( unordered_set<string> :: iterator SETITOR = ITOR->second.begin() ; SETITOR != ITOR->second.end(); SETITOR ++ ){
            file.write( SETITOR->data() , SETITOR->size() );
        }
    }
}
string Table::getTitle() {
    return title_;
}

short Table::gethasKey(){
    return attr_.primary_key;
}

int Table :: HasAttribute( string AttributeName ){
    for ( int i = 0 ; i < attr_.num ; i ++ ){
        if( attr_.name[i] == AttributeName ){
            return i;
        }
    }
    return -1;
}

void Table :: SetTitle( string name ){
    title_ = name;
}
int Table :: GetLength(){
    return this->length;
}
istream& operator>>( istream & in , Table & t){
    in >> t.attr_.num >> t.attr_.primary_key;
    t.length = 0;
    for(int i = 0; i < t.attr_.num; i ++ ){
        in >> t.attr_.name[i] >> t.attr_.type[i];
        in >> t.attr_.unique[i] >> t.attr_.has_index[i];
        if ( t.attr_.has_index[i] ) in >> t.attr_.index_name[i];
        t.length += ( t.attr_.type[i] < 1 ) * 4 + ( t.attr_.type[i] >= 1 ) * t.attr_.type[i];
        t.AttrName2Index.insert(make_pair( t.attr_.name[i] , i ));  
    }
    t.ConstructMap();
    return in;
}
ostream& operator<<( ostream & out , const Table & t){
    out << t.attr_.num << " "<<t.attr_.primary_key << endl;
    for(int i = 0; i < t.attr_.num; i ++ ){
        out << t.attr_.name[i] << " " <<t.attr_.type[i] << " ";
        out << t.attr_.unique[i] <<" " << t.attr_.has_index[i] <<" ";
        if ( t.attr_.has_index[i] ) out << t.attr_.index_name[i];
        out << endl;
    }
    return out;
}

void Table :: ConstructMap(){
    for(int i = 0 ; i < this->attr_.num ; i ++ ){
        AttrName2Index.insert( make_pair( this->attr_.name[i] , i ) );
    }
}

vector<int > Table :: ConvertIntoIndex( const vector<string> AttrName ){
    vector<int> ret(AttrName.size());
    for(int i = 0;i < AttrName.size() ; i++ ){
        ret[i] = AttrName2Index[AttrName[i]];
    }
    return ret;
}
void Table :: ShowTableInfo(){
    cout << "Title : " << this->title_ << endl;
    int n = attr_.num;
    cout << "AttrNum : " << n << " " << "Primary key Index" << " " << attr_.primary_key << endl;
    for(int i = 0 ;i < n ; i++ ){
        cout << attr_.name[i] << " "<<attr_.type[i] << " " <<attr_.has_index[i] << " " << endl;
    }
}