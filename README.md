# Minisql

## 1.关于属性，元组和表的定义部分

```
//储存数据类型和对应数据
struct Data {
    int type;    -1：int,0:float,1~255:string的长度+1
    int datai;
    float dataf;
    std::string datas;
};
```

```
//Where存放一组判断语句
struct Where {
    Data data; //数据
    WHERE relation_character;   //关系
};
```

```
//索引
struct Index {
    int num;  //index的总数
    short location[10];  //第i个index建立在第location[i]个attribute上
    string indexname[10];  //每个index的名字
};
```

```
//属性
struct Attribute {
    int num;  //存放table的属性数
    string name[32];  //存放每个属性的名字
    short type[32];  //存放每个属性的类型，-1：int,0:float,1~255:string的长度+1
    bool unique[32];  //判断每个属性是否unique，是为true
    short primary_key;  //判断是否存在主键,-1为不存在，其他则为主键的所在位置
    bool has_index[32]; //判断是否存在索引
};
```

```
//元祖
class Tuple {
private:
    vector<Data> data_;  //存储元组里的每个数据的信息
    bool isDeleted_;
public:
    Tuple() : isDeleted_(false) {};
    Tuple(const Tuple& tuple_in);  //拷贝元组
    void addData(Data data_in);  //新增元组
    vector<Data> getData() const;  //返回数据
    int getSize();  //返回元组的数据数量
    bool isDeleted();
    void setDeleted();
    void showTuple();  //显示元组中的所有数据
};
```

```
class Table {
private:
    string title_;  //表名
    vector<Tuple> tuple_;  //存放所有的元组
    Index index_;  //表的索引信息
public:
    Attribute attr_;  //属性
    //构造函数
    Table() {};
    Table(string title, Attribute attr);
    Table(const Table& table_in);

    int addTuple(Tuple tuple_in); // 插入元祖，判断tuple_in 的格式是否符合table中属性格式
    int setIndex(short index, string index_name);  //插入索引，输入要建立索引的Attribute的编号，以及索引的名字，成功返回1失败返回0
    int dropIndex(string index_name);  //删除索引，输入建立的索引的名字，成功返回1失败返回0
    string getTitle();
    Attribute getAttr();
    vector <Tuple>& getTuple();//返回元祖
    Index getIndex();
    short gethasKey();//返回主码所在的属性位置
    void showTable(); //打印表属性和元祖
    void showTable(int limit);//显示table的部分数据
};
```

## 2.INTERPRETER

```
class Interpreter {
public:
	Interpreter();
	void getQuery();
	void EXEC();

private:
	string query;
	int getType(int pos, int& end_pos);//返回数据类型
	int getBits(int num);//返回整数的长度
	int getBits(float num);//返回小数长度
	string getRelation(int pos, int& end_pos);//返回逻辑关系
	string getWord(int pos, int& end_pos);//返回单词
	string getLower(string s, int p);//变小写（便于识别）
	void Normalize();
	void EXEC_SELECT();
	void EXEC_DROP_TABLE();
	void EXEC_DROP_INDEX();
	void EXEC_CREATE_TABLE();
	void EXEC_CREATE_INDEX();
	void EXEC_INSERT();
	void EXEC_DELETE();
	void EXEC_SHOW();
	void EXEC_EXIT();
	void EXEC_FILE();
	void EXEC_HELP();

};
```

interpreter部分负责程序与使用者之间的交互关系

## 3.API部分

```
class API {
public:
	API();
	~API();
	Table selectRecord(string table_name, vector<string> target_attr, vector<Where> where, char operation);
	int deleteRecord(string table_name, string target_attr, Where where);
	void insertRecord(string table_name, Tuple& tuple);
	bool createTable(string table_name, Attribute attribute, int primary, Index index);
	bool dropTable(string table_name);
	bool createIndex(string table_name, string index_name, string attr_name);
	bool dropIndex(string table_name, string index_name);
private:
	//用于多条件查询时的and条件合并
	Table unionTable(Table& table1, Table& table2, string target_attr, Where where);
	//用于多条件查询时的or条件合并
	Table joinTable(Table& table1, Table& table2, string target_attr, Where where);
	RecordManager record;
	CatalogManager catalog;
};
```

提供与其他模块的接口

```
Table selectRecord(string table_name, vector<string> target_attr, vector<Where> where, char operation);
```

输入：表名，where的限制条件（target_attr为要where中出现的属性，where中存放对应属性的限制要求，operation存放各where之间的逻辑运算关系（交集/并集））

输出：select语句对应的table

功能：根据where条件调用recordmanager的选择元组函数，在有operation要求的情况下通过逻辑运算进行筛选并构成对应的table



```
int deleteRecord(string table_name, string target_attr, Where where);
```

输入：表名，where的限制条件

输出：返回删除的结果，删除成功 -1 失败 -0

功能：根据where条件调用recordmanager的成员函数函数，删除元组

```
void insertRecord(string table_name, Tuple& tuple);
```

输入：表名，元组

输出：无

功能：向recordmanager中的table中调用成员函数插入元组

```
bool createTable(string table_name, Attribute attribute, int primary, Index index);
```

输入：表名，元组，主码对应的属性下标（没有为-1），索引

输出：创建结果

功能：向recordmanager和catalogmanager调用成员函数写入table



```
bool dropTable(string table_name);
```

输入：表名

输出：删除结果

功能：在recordmanager和catalogmanager中调用成员函数删除table



```
bool createIndex(string table_name, string index_name, string attr_name);
```

输入：表名，元组，索引

输出：创建结果

功能：向indexmanager、recordmanager、catalogmanager中调用对应成员函数建立table的索引



```
bool dropIndex(string table_name, string index_name);
```

输入：表名，索引名

输出：删除结果

功能：向indexmanager、catalogmanager中调用对应成员函数删除table的索引




## Buffer Manager 相关说明

Buffer Manager负责缓冲区的管理，主要功能有： 
1.	根据需要，读取指定的数据到系统缓冲区或将缓冲区中的数据写出到文件 
2.	实现缓冲区的替换算法，当缓冲区满时选择合适的页进行替换 
3.	记录缓冲区中各页的状态，如是否被修改过等 
4.	提供缓冲区页的pin功能，及锁定缓冲区的页，不允许替换出去 
为提高磁盘I/O操作的效率，缓冲区与文件系统交互的单位是块，块的大小应为文件系统与磁盘交互单位的整数倍，一般可定为4KB或8KB。

在实际实现过程中，将Block 定义为4KB，将所有数据存入char 的数组中，数组大小为4096。

### 结构说明

#### File

File 中存入了调用过的所有文件的头
```c++
class File{
public:
    string filename; 
    int type;// index or record
    Block * head;
    File * next;
    File * pre;
    File(string _filename,int _type):filename(_filename),type(_type)\
    ,pin(false),head(NULL),next(NULL),pre(NULL){};
};
```
其中，head 表示file 下的块

#### Block
```c++
class Block
{
private:
    string filename;
    File * file;
    bool dirty;
    bool pin;
    int offsetNum;
    int UsingSize;
    clock_t time;
    Block * next;
    Block * pre;
    char *data;
    friend File;
public:
    Block();
    ~Block();
    void clear(); // 清楚里面的内容
    friend class buffermanager;
    void SetPin();
    void ClearPin();
    void SetDirty();
    void ClearDirty();
    void SetClock();
    void WriteBack(); //写回
    void ReadIn();//通过offsetNum读入，每次读入4096个字节
    void write(int offset, const char * data,int length);//写入数据，通过offset定位
    void SetUsingSize(int size);//设置大小
    char * FetchRecord( int offset , int size );//获取一条记录
    char * GetContent();//获取所有记录
};
```

#### buffermanager

```c++
class buffermanager
{
private:
    Block block_pool [BLOCK_NUMBER];
    int total_block;
public:
    File * FileHead;
    buffermanager():FileHead(NULL),total_block(0){};
    ~buffermanager();
    File * GetFile( string tabame, int type );//读入文件，获得某个文件文件的头
    Block * GetBlock( File * file, Block * position) //获得一个block
    Block * GetNextBlock( File * file , Block * position);// 获得offsetNum + 1的块
    Block * GetBlockByNum( File * file , int offsetNum );//通过offsetNum获得块
    Block * GetEmptyBlock();//获得一个空的块
    Block * GetReplaceBlock();
    Block * GetBlockHead( File * file );//获得第一个块
    void DeleteFileFromList( string filename );//关掉文件
    void CloseFile( File * file ); // 关掉某个文件
    void ShowInfo(Block * tmp); // 调试用
    void WriteBackAll(); //写回所有数据
};
```

在开始一条指令时，需要通过`GetFile()` 获得固定的一个File 的指针，再通过`GetBlock()`获取一个块。

块需要读入相应offsetNum 的内容，例如，offsetNum = 1，读入第2 (offsetNum+1) 个4kb的内容

如果进行查询操作，可使用`GetNextBlock()`以此获得块，Block中end 标志是否为最后一个块

如果进行插入或者删除操作，可使用Block的Write 函数，插入可给一条 record 的char * 的数组，并且给予插入的大小，删除可给全空的char * , 之后需要调用`SetDirty()`,`SetUsingSize()`对块的属性进行修改。Buffer Manager 不考虑记录的连续性的问题，需要Record Manager 进行对记录的移动。

如果进行更改，也可调用write ，只不过只需要`SetDirty()`即可，不需要`SetUsingSize()`