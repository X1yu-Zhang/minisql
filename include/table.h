#ifndef TABLE_H
#define TABLE_H
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <unordered_set>
#include "tuple.h"
using namespace std;


class Table {
private:
    string title_;  //????
    int length;
    void ConstructMap();
public:
    Attribute attr_;  //????
    map<string , int> AttrName2Index;
    map<string , unordered_set<string> > Unique;
    
    Table():length(0){};
    Table(string title, Attribute attr);
    Table(const Table& table_in);
    Table( string name ):title_(name),length(0){};
    int GetLength();
    void SetTitle( string name );
    string getTitle();
    short gethasKey();//?????????????????????
    bool HasAttribute( string name );
    friend istream& operator>>( istream & in , Table & t); 
    friend ostream& operator<<( ostream & out , const Table & t);
    vector<int >ConvertIntoIndex( const vector<string> AttrName );
    void ShowTableInfo();
    void ReadUnique();
    void WriteUnique();
    void ShowUnique( string name , int type );
};

#endif