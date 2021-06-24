#include "../include/interpreter.h"
#include <typeinfo>

void Interpreter::getQuery() 
{
    string tmp;
    query = "";
    do 
    {
        cout << ">>> ";
        getline(cin, tmp);
        query += tmp;
        query += ' ';
    } while (tmp[tmp.length() - 1] != ';');
    query[query.length() - 2] = '\0';
    Normalize();
}

void Interpreter::Normalize() 
{
    for (int pos = 0; pos < query.length(); pos++) 
    {
        if (query[pos] == '*' || query[pos] == '=' || query[pos] == ',' || query[pos] == '(' || query[pos] == ')' || query[pos] == '<' || query[pos] == '>') 
        {
            if (query[pos - 1] != ' ')
                query.insert(pos++, " ");
            if (query[pos + 1] != ' ')
                query.insert(++pos, " ");
        }
    }
    query.insert(query.length() - 2, " ");

    string::iterator it;
    int flag = 0;
    for (it = query.begin(); it < query.end(); it++) {
        if (flag == 0 && (*it == ' ' || *it == '\t')) {
            flag = 1;
            continue;
        }
        if (flag == 1 && (*it == ' ' || *it == '\t')) {
            query.erase(it);
            if (it != query.begin())
                it--;
            continue;
        }
        if (*it != ' ' && *it != '\t') {
            flag = 0;
            continue;
        }
    }
   
    query = trim(query);
    query = getLower(query, 0);
}

int Interpreter::getBits(int num) {
    int bit = 0;
    if (num == 0)
        return 1;
    if (num < 0) {
        bit++;
        num = -num;
    }
    while (num != 0) {
        num /= 10;
        bit++;
    }
    return bit;
}

int Interpreter::getBits(float num) {
    int bit = 0;
    if ((int)num == 0)
        return 4;
    if (num < 0) {
        bit++;
        num = -num;
    }
    int integer_part = num;
    while (integer_part != 0) {
        bit++;
        integer_part /= 10;
    }
    return bit + 3;
}

int Interpreter::getType(int pos, int& end_pos) {
    string type = getWord(pos, end_pos);
    if (type == "int")
        return -1;
    else if (type == "float")
        return 0;
    else if (type == "char") {
        end_pos += 3;
        string length = getWord(end_pos, end_pos);
        end_pos += 2;
        return atoi(length.c_str()) + 1;
    }else{
        throw input_format_error();
    }
}

string trim(string s)
{
    int i = 0; while (s[i] == ' ') i++;
    int j = s.length() - 1; while (s[j] == ' ') j--;
    return s.substr(i, j - i + 1);
}

string Interpreter::getLower(string s,int p)
{
    for (int index = p;; index++) {
        if (s[index] == ' ' || s[index] == '\0')
            break;
        else if (s[index] >= 'A' && s[index] <= 'Z')
            s[index] += 32;
    }
    return s;
}

string Interpreter::getRelation(int pos, int& end_pos) {
    string PartWord = "";
    for (int pos1 = pos; pos1 < (int)query.length(); pos1++) 
    {
        if (query[pos1] == ' ')
            continue;
        if (query[pos1] != '<' && query[pos1] != '>' && query[pos1] != '=' && query[pos1] != '!')
        {
            PartWord = query.substr(pos, pos1 - pos - 1);
            end_pos = pos1 - 1;
            return PartWord;
        }
    }
    return PartWord;
}

string Interpreter::getWord(int pos, int& end_pos) {
    string PartWord = "";
    for (int pos1 = pos; pos1 < (int)query.length(); pos1++) {
        if (query[pos1] == ' ' || query[pos1] == '\0')
        {
            PartWord = query.substr(pos, pos1 - pos);
            end_pos = pos1;
            return PartWord;
        }
    }
    return PartWord;
}
void Interpreter::EXEC() {
    if (query.substr(0, 6) == "select") {
        EXEC_SELECT();
    }
    else if (query.substr(0, 4) == "drop") {
        query = getLower(query, 5);
        if (query.substr(5, 5) == "table")
            EXEC_DROP_TABLE();
        else if (query.substr(5, 5) == "index")
            EXEC_DROP_INDEX();
    }
    else if (query.substr(0, 6) == "insert") {
        EXEC_INSERT();
    }
    else if (query.substr(0, 6) == "create") {
        query = getLower(query, 7);
        if (query.substr(7, 5) == "table") {
            EXEC_CREATE_TABLE();
        }
        else if (query.substr(7, 5) == "index") {
            EXEC_CREATE_INDEX();
        }
    }
    else if (query.substr(0, 6) == "delete") {
        EXEC_DELETE();
    }
    else if (query.substr(0, 8) == "describe" || query.substr(0, 4) == "desc") {
        EXEC_SHOW();
    }
    else if (query.substr(0, 4) == "exit" || query.substr(0,4) == "quit" && query[5] == '\0') {
        EXEC_EXIT();
    }
    else if (query.substr(0, 8) == "execfile") {
        EXEC_FILE();
    }
    else if (query.substr(0, 4) == "help" && query[5] == '\0') {
        EXEC_HELP();
    }
}

//create index
void Interpreter::EXEC_CREATE_INDEX() {
    string index_name;
    string table_name;
    string attr_name;
    int check_index;
    index_name = getWord(13, check_index);
    check_index++;
    if (getLower(query, check_index).substr(check_index, 2) != "on"){
        throw input_format_error();
    }
    table_name = getWord(check_index + 3, check_index);
    if (query[check_index + 1] != '('){
        throw input_format_error();
    }
    attr_name = getWord(check_index + 3, check_index);
    if (query[check_index + 1] != ')' || query[check_index + 3] != '\0'){
        throw input_format_error();
    }
    api.createIndex(table_name, index_name, attr_name);

    cout << ">>> SUCCESS" << endl;
}
// drop index
void Interpreter::EXEC_DROP_INDEX() {
    
    string table_name;
    string index_name;
    int check_index;
    index_name = getWord(11, check_index);
    check_index++;
    if (query[check_index + 2] != '\0'){
        throw input_format_error();
    }
    api.dropIndex( index_name );
    cout << ">>> SUCCESS" << endl;
}

// exit
void Interpreter::EXEC_EXIT() {
    cout << ">>>See you next time! ";
    api.~API();
    IM.~IndexManager();
    CM.~CatalogManager();
    RM.~RecordManager();
    BM.~BufferManager();
    exit(0);
}
void Interpreter::EXEC_FILE() {
    int check_index = 0;
    int start_index = 0;
    string tmp_query;
    string file_path = getWord(9, check_index);
    if (query[check_index + 1] != '\0'){
        throw input_format_error();
    }
    string::iterator it;
    fstream fs(file_path);
    stringstream ss;
    ss << fs.rdbuf();
    tmp_query = ss.str();
    check_index = 0;
    do {
        try{
            while (tmp_query[check_index] != '\n')
                check_index++;
            query = tmp_query.substr(start_index, check_index - start_index);
            check_index++;
            start_index = check_index;
            Normalize();
            EXEC();
        }
        catch (table_exist error) {
            std::cout << ">>> Error: Table has existed!" << std::endl;
        }
        catch (table_not_exist error) {
            std::cout << ">>> Error: Table not exist!" << std::endl;
        }
        catch (attribute_not_exist error) {
            std::cout << ">>> Error: Attribute not exist!" << std::endl;
        }
        catch (index_exist error) {
            std::cout << ">>> Error: Index has existed!" << std::endl;
        }
        catch (index_not_exist error) {
            std::cout << ">>> Error: Index not existed!" << std::endl;
        }
        catch (tuple_type_conflict error) {
            std::cout << ">>> Error: Tuple type conflict!" << std::endl;
        }
        catch (primary_key_conflict error) {
            std::cout << ">>> Error: Primary key conflict!" << std::endl;
        }
        catch (data_type_conflict error) {
            std::cout << ">>> Error: data type conflict!" << std::endl;
        }
        catch (index_full error) {
            std::cout << ">>> Error: Index full!" << std::endl;
        }
        catch (unique_conflict error) {
            std::cout << ">>> Error: unique conflict!" << std::endl;
        }
        catch ( input_format_error error ) {
            std::cout << ">>> Error: Input format error!" << std::endl;
        }
    } while (tmp_query[check_index] != '\0');
}

void Interpreter::EXEC_SHOW() {
    
    string table_name;
    int check_index;
    getWord(0, check_index);
    table_name = getWord(check_index + 1, check_index);
    if (query[check_index + 1] != '\0'){
        throw input_format_error();
    }
  //  CM.showTable(table_name);
}
void Interpreter::EXEC_DELETE() {

    vector<string> attr_name;
    vector<string> target_name;
    vector<Where> where_delete;
    string tmp_target_name;
    Where tmp_where;
    string tmp_value;
    int check_index;
    string table_name;
    string relation;
    if (getLower(query, 7).substr(7, 4) != "from") {
        throw input_format_error();
    }
    table_name = getWord(12, check_index);

    if (query[check_index + 1] == '\0') {
        api.deleteRecord(table_name, attr_name, where_delete);
        cout << ">>> SUCCESS" << endl;
        return;
    }

    if (getLower(query, check_index + 1).substr(check_index + 1, 5) != "where") {
        throw input_format_error();
    }

    check_index += 7;
    Attribute tmp_attr = CM.getAttribute(table_name);

    while (1) {
        tmp_target_name = getWord(check_index, check_index);
        if (!CM.HasAttribute(table_name, tmp_target_name))
        {
            throw attribute_not_exist();
        }
        target_name.push_back(tmp_target_name);
        relation = getRelation(check_index + 1, check_index);
        if (relation == "<")
            tmp_where.relation_character = LESS;
        else if (relation == "< =")
            tmp_where.relation_character = LESS_OR_EQUAL;
        else if (relation == "=")
            tmp_where.relation_character = EQUAL;
        else if (relation == "> =")
            tmp_where.relation_character = GREATER_OR_EQUAL;
        else if (relation == ">")
            tmp_where.relation_character = GREATER;
        else if (relation == "! =")
            tmp_where.relation_character = NOT_EQUAL;
        else
        {
            throw input_format_error();
        }
        tmp_value = getWord(check_index + 1, check_index);
        for (int i = 0; i < tmp_attr.num; i++)
        {
            if (tmp_target_name == tmp_attr.name[i]) {
                tmp_where.data.type = tmp_attr.type[i];
                switch (tmp_where.data.type) {
                case -1:
                    if (typeid(tmp_value) == typeid(int)) {
                        tmp_where.data.datai = stringToNum<int>(tmp_value);
                    }
                    else {
                        throw data_type_conflict();
                    }
                    break;
                case 0:
                    if (typeid(tmp_value) == typeid(float)) {
                        tmp_where.data.dataf = stringToNum<float>(tmp_value);
                    }
                    else {
                        throw data_type_conflict();
                    }
                    break;
                default:
                    if (!(tmp_value[0] != '\'' && tmp_value[tmp_value.length() - 1] != '\'') && !(tmp_value[0] != '"' && tmp_value[tmp_value.length() - 1] != '"')) {
                        throw input_format_error();
                    }
                    tmp_where.data.datas = tmp_value.substr(1, tmp_value.length() - 2);
                    break;
                }
            }

            where_delete.push_back(tmp_where);
            if (query[check_index + 1] == '\0')
                break;
            else if (getLower(query, check_index + 1).substr(check_index + 1, 3) == "and")
            {
                getWord(check_index + 1, check_index);
                check_index++;
            }
            else 
            {
                throw input_format_error();
            }
        }
        api.deleteRecord(table_name, attr_name, where_delete);
        cout << ">>> SUCCESS" << endl;
    }
}

//Insert
    void Interpreter::EXEC_INSERT() {


        string table_name;
        int check_index;
        Tuple tuple_insert;
        Attribute attr_exist;
        if (getLower(query, 7).substr(7, 4) != "into")
        {
            throw input_format_error();
        }
        table_name = getWord(12, check_index);
        if (getLower(query, check_index + 1).substr(check_index + 1, 6) != "values")
        {
            throw input_format_error();
        }
        check_index += 8;
        if (query[check_index] != '(')
        {
            throw input_format_error();
        }
        if (!CM.HasTable(table_name))
        {
            throw table_not_exist();
        }

        attr_exist = CM.getAttribute(table_name);
        check_index--;
        int num_of_insert = 0;

        while (query[check_index + 1] != '\0' && query[check_index + 1] != ')') {
            if (num_of_insert >= attr_exist.num) {
                throw input_format_error();
            }
            check_index += 3;
            string value_insert = getWord(check_index, check_index);
            Data insert_data;
            insert_data.type = attr_exist.type[num_of_insert];
            switch (attr_exist.type[num_of_insert]) {
            case -1:
                try {
                    insert_data.datai = stringToNum<int>(value_insert);
                }
                catch (...) {
                    throw data_type_conflict();//ת��ʧ��
                }
                break;
            case 0:
                try {
                    insert_data.dataf = stringToNum<float>(value_insert);
                }
                catch (...) {
                    throw data_type_conflict();//ת��ʧ��
                }
                break;
            default:
                if (!(value_insert[0] == '\'' && value_insert[value_insert.length() - 1] == '\'')
                    && !(value_insert[0] == '"' && value_insert[value_insert.length() - 1] == '"'))
                {
                    throw input_format_error();
                }
                if (value_insert.length() - 1 > attr_exist.type[num_of_insert])
                {
                    throw input_format_error();
                }
                insert_data.datas = value_insert.substr(1, value_insert.length() - 2);
                break;
            }
            tuple_insert.addData(insert_data);
            num_of_insert++;
        }
        if (query[check_index + 1] == '\0')
        {
            throw input_format_error();
        }
        if (num_of_insert != attr_exist.num)
        {
            throw input_format_error();
        }
    api.insertRecord(table_name, tuple_insert);
    cout << ">>> SUCCESS" << endl;
}

void Interpreter::EXEC_HELP()
{
    cout << "Supported commands:" << endl;
    cout << "  select * from tableName [where condition];" << endl;
    cout << "  select (attr1, attr2, ... , attrN) from tableName [where condition];" << endl;
    cout << "" << endl;
    cout << "  create table tableName (attr1 type1, ... , attrN typeN, primary key(attr));" << endl;
    cout << "" << endl;
    cout << "  create index indexName on tableName (attrName);" << endl;
    cout << "" << endl;
    cout << "  insert into tableName values(val1, val2, ... , valN);" << endl;
    cout << "" << endl;
    cout << "  delete * from tableName [where condition];" << endl;
    cout << "  delete (attr1, attr2, ... , attrN) from tableName [where condition];" << endl;
    cout << "" << endl;
    cout << "  drop table tableName;" << endl;
    cout << "" << endl;
    cout << "  drop index indexName;" << endl;
    cout << "" << endl;
    cout << "  '\\clear' to clear the screen." << endl;
    cout << "  '\\exit' or '\\quit' to exit this program." << endl;
    cout << "  '\\status' to show all info that last command created." << endl;
    cout << "  '\\flush' to flush all info into disk." << endl;
    cout << "" << endl;
    cout << "supported where expression:" << endl;
    cout << "  eg. where attr>val          >,<,>=,<=,<>" << endl;
    cout << "  eg. where attr1<>attr2" << endl;
    cout << "  eg. where attr1<val1 and ( attr2>=val2 or attr3<>val3 )" << endl;
    cout << "  eg. where attr between val1, val2" << endl;
    cout << "     note: between clause turns into :   attr>=val1 and attr<=val2" << endl;
    cout << "" << endl;
    cout << " NOTE:" << endl;
    cout << "   UNDEFINED values is allowed in tables, but compasion with UNDEFINED is NOT supported." << endl;
    cout << "   So if you insert an all-undefined record, you will not be able to access it." << endl;
}

void Interpreter::EXEC_DROP_TABLE(){
    
    string table_name;
    int check_index;
    table_name = getWord(11, check_index);
    api.dropTable(table_name);
    cout << ">>> SUCCESS" << endl;
}

void Interpreter::EXEC_CREATE_TABLE() {
    string table_name;
    int check_index;
    Attribute attr_;
    table_name = getWord(13, check_index);
    int i = 0, temp; 
    int primary = -1;
    while (query[check_index + 1] != '\0' && query[check_index + 1] != ')') {
        check_index += 3;
        attr_.name[i] = getWord(check_index, check_index);
        check_index += 1;
        attr_.type[i] = getType(check_index, check_index);
        i++;
        if (query.substr(check_index+3, 11) == "primary key")
        {
            check_index += 17;
            string temp = getWord(check_index, check_index);
            for (int j = 0; j < i ; j++)
                if (attr_.name[j] == temp )
                {
                    primary = j;
                    attr_.index_name[j] = table_name + "_Primary";
                    attr_.has_index[j] = 1;
                    attr_.unique[j] = 1;
                    break;
                }
            check_index += 2;
        }
    }
    attr_.num = i;
    attr_.primary_key = primary;
    api.createTable( table_name, attr_ );
}
void Interpreter::EXEC_SELECT()
{
    string table_name;
    vector<string> attr_name;
    vector<string> target_name;
    vector<Where> where_select;
    string tmp_target_name;
    string tmp_value;
    Where tmp_where;
    string relation;
    char op = 0;
    int check_index=0;
    int flag = 0;

    if (getWord(7, check_index) == "*")
    {
        flag = 1;
        attr_name.push_back("*");
        check_index ++;
    }
    else {
        check_index = 7;
        while (1) {
            attr_name.push_back(getWord(check_index, check_index));
            if (query[++check_index] != ',')
                break;
            else
                check_index += 2;
        }
    }

    if (getLower(query, check_index).substr(check_index, 4) != "from") {
        throw input_format_error();
    }
    check_index += 5;
    table_name = getWord(check_index, check_index);
    if (!CM.HasTable(table_name))
    {
        throw table_not_exist();
    }

    Attribute tmp_attr = CM.getAttribute(table_name);

    check_index ++;
    if (query[check_index] == '\0')
        api.selectRecord( table_name, attr_name,target_name, where_select);
    else 
    {
        if (getLower(query, check_index).substr(check_index, 5) != "where")
        {
            throw input_format_error();
        }
        int the_first = 1;
        check_index += 6;
        while (1) 
        {
            tmp_target_name = getWord(check_index, check_index);
            if( tmp_target_name.size() == 0 ) break;
            if( !the_first && tmp_target_name != "and" ) throw input_format_error(); 
            if (!CM.HasAttribute(table_name, tmp_target_name))
            {
                throw attribute_not_exist();
            }
            target_name.push_back(tmp_target_name);
            relation = getRelation(check_index + 1, check_index);
            if (relation == "<")
                tmp_where.relation_character = LESS;
            else if (relation == "< =")
                tmp_where.relation_character = LESS_OR_EQUAL;
            else if (relation == "=")
                tmp_where.relation_character = EQUAL;
            else if (relation == "> =")
                tmp_where.relation_character = GREATER_OR_EQUAL;
            else if (relation == ">")
                tmp_where.relation_character = GREATER;
            else if (relation == "! =")
                tmp_where.relation_character = NOT_EQUAL;
            else
            {
                throw input_format_error();
            }
            tmp_value = getWord(check_index + 1, check_index);
            int flag = 1;
            for (int i = 0; i < tmp_attr.num && flag ; i++)
            {
                if (tmp_target_name == tmp_attr.name[i]) {
                    tmp_where.data.type = tmp_attr.type[i];
                    switch (tmp_where.data.type) {
                    case -1:
                        tmp_where.data.datai = stoi(tmp_value);
                        flag = 0;
                        break;
                    case 0:
                        tmp_where.data.dataf = stof(tmp_value);
                        flag = 0;
                        break;
                    default:
                        if (!(tmp_value[0] != '\'' && tmp_value[tmp_value.length() - 1] != '\'') && !(tmp_value[0] != '"' && tmp_value[tmp_value.length() - 1] != '"')) {
                            throw input_format_error();
                        }
                        tmp_where.data.datas = tmp_value.substr(1, tmp_value.length() - 2);
                        flag = 0;
                        break;
                    }
                }
            }
            where_select.push_back(tmp_where);
        }
        api.selectRecord(table_name, attr_name, target_name, where_select);
    }
}


