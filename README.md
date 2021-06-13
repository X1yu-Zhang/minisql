# Minisql

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