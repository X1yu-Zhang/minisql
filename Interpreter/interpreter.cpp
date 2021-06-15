#include "interpreter.h"


//��query
void Interpreter::getQuery() 
{
    string tmp;
    //�õ�һ�е������ַ��������һ���ַ�Ϊ�ֺ�ʱ����
    do 
    {
        cout << ">>> ";
        getline(cin, tmp);
        query += tmp;
        query += ' ';
    } while (tmp[tmp.length() - 1] != ';');
    //�����һ����β��ʶ��
    query[query.length() - 2] = '\0';
    //����Normalize�����ַ����Ĺ淶��
    Normalize();
}

//��query��ʽ����
void Interpreter::Normalize() 
{
    //�����е�������ŵ�ǰ������һ���ո��Բ�ֶ���
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
    //�ڽ�β��һ���ո��Թ淶��
    query.insert(query.length() - 2, " ");
    //ɾ�������еĶ���ո�
    //�ȼ���һ���ո��ٳ��ֿո��ɾ��
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
   
    trim(query);
    //��query�ĵ�һ���ʱ�ΪСд�����ں����ж�
    query = getLower(query, 0);
}

//���������ֳ���
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

//��С�����ֳ���
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
    return bit + 3;//Ϊ�˱���С����ĺ�λ
}

//�õ�һ��λ�õ���������
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
    }
    throw 1;
}
//ȥ��s��ǰ��Ŀո�
string trim(string s)
{
    int i = 0; while (s[i] == ' ') i++;
    int j = s.length() - 1; while (s[j] == ' ') j--;
    return s.substr(i, j - i + 1);
}

//��s�аѴ�p��ʼ���ո�Ĵʱ�ΪСд
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
//�����߼����������¼����λ
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
//���ص��ʲ���¼����λ
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
//ִ��query
void Interpreter::EXEC() {
    try {
        //�����ַ����ĵ�һ���������������еĲ�������
        if (query.substr(0, 6) == "select") {
            EXEC_SELECT();
        }
        //����drop�����������������Ҫ���н�һ���Ľ���
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
        //createҲ���������
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
        //����describe�����ַ�ʽ������ʹ�û��߼�
        else if (query.substr(0, 8) == "describe" || query.substr(0, 4) == "desc") {
            EXEC_SHOW();
        }
        else if (query.substr(0, 4) == "exit" && query[5] == '\0') {
            EXEC_EXIT();
        }
        else if (query.substr(0, 8) == "execfile") {
            EXEC_FILE();
        }
        else if (query.substr(0, 4) == "help" && query[5] == '\0') {
            EXEC_HELP();
        }
        //�������ָ����ܶ�Ӧ�����׳������ʽ����
        else {
            throw input_format_error();
        }
    }

    catch (table_exist error) {
        cout << ">>> Error: Table has existed!" << endl;
    }
    catch (table_not_exist error) {
        cout << ">>> Error: Table not exist!" << endl;
    }
    catch (attribute_not_exist error) {
        cout << ">>> Error: Attribute not exist!" << endl;
    }
    catch (index_exist error) {
        cout << ">>> Error: Index has existed!" << endl;
    }
    catch (index_not_exist error) {
        cout << ">>> Error: Index not existed!" << endl;
    }
    catch (tuple_type_conflict error) {
        cout << ">>> Error: Tuple type conflict!" << endl;
    }
    catch (primary_key_conflict error) {
        cout << ">>> Error: Primary key conflict!" << endl;
    }
    catch (data_type_conflict error) {
        cout << ">>> Error: data type conflict!" << endl;
    }
    catch (exit_command error) {
        cout << ">>> See you next time" << endl;
        exit(0);
    }
    catch (...) {
        cout << ">>> Error: Input format error!" << endl;
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
    if (getLower(query, check_index).substr(check_index, 2) != "on")
        throw 1;//��ʽ����
    table_name = getWord(check_index + 3, check_index);
    if (!CM.hasTable(table_name))
        throw table_not_exist();//table not exist
    if (query[check_index + 1] != '(')
        throw 1;//��ʽ����
    attr_name = getWord(check_index + 3, check_index);
    if (query[check_index + 1] != ')' || query[check_index + 3] != '\0')
        throw 1;//��ʽ����
    _API.createIndex(table_name, index_name, attr_name);
    cout << ">>> SUCCESS" << endl;
}
//drop index
void Interpreter::EXEC_DROP_INDEX() {
    
    string table_name;
    string index_name;
    int check_index;
    index_name = getWord(11, check_index);
    check_index++;
    //����ʽ
    if (getLower(query, check_index).substr(check_index, 2) != "on")
        throw 1;//��ʽ����
    table_name = getWord(check_index + 3, check_index);
    if (query[check_index + 1] != '\0')
        throw 1;//��ʽ����
    _API.dropIndex(table_name, index_name);
    cout << ">>> SUCCESS" << endl;
}

// exit
void Interpreter::EXEC_EXIT() {
    throw exit_command();
}

//�ļ���ȡ
void Interpreter::EXEC_FILE() {
    int check_index = 0;
    int start_index = 0;
    string tmp_query;
    //��ȡ�ļ�·��
    string file_path = getWord(9, check_index);
    //�жϸ�ʽ��·�����������ַ���
    if (query[check_index + 1] != '\0')
        throw 1;//��ʽ����
    string::iterator it;
    fstream fs(file_path);
    stringstream ss;
    ss << fs.rdbuf();
    tmp_query = ss.str();
    check_index = 0;
    //����ִ��
    do {
        while (tmp_query[check_index] != '\n')
            check_index++;
        query = tmp_query.substr(start_index, check_index - start_index);
        check_index++;
        start_index = check_index;
        Normalize();
        EXEC();
    } while (tmp_query[check_index] != '\0');
}

//ֱ�Ӵ�ӡtable
void Interpreter::EXEC_SHOW() {
    string table_name;
    int check_index;
    //���ص�һ�����ʵĽ���λ��
    getWord(0, check_index);
    //�õ�����
    table_name = getWord(check_index + 1, check_index);
    if (query[check_index + 1] != '\0')
        throw 1;//��ʽ����
    CM.showTable(table_name);
}

//ɾ��
void Interpreter::EXEC_DELETE() {
    
    Where where_delete;
    int check_index;
    string table_name;
    string attr_name;
    string relation;
    if (getLower(query, 7).substr(7, 4) != "from")
        throw 1;
    table_name = getWord(12, check_index);
    if (!CM.hasTable(table_name))
        throw table_not_exist();

    //ɾ��������Ϣ
    if (query[check_index + 1] == '\0') {
        attr_name = "";
        _API.deleteRecord(table_name, attr_name, where_delete);
        cout << ">>> SUCCESS" << endl;
        return;
    }

    if (getLower(query, check_index + 1).substr(check_index + 1, 5) != "where")
        throw 1;//��ʽ����
    attr_name = getWord(check_index + 7, check_index);
    //�ж��Ƿ���ڸ�����
    if (!CM.hasAttribute(table_name, attr_name))
        throw attribute_not_exist();
    relation = getRelation(check_index + 1, check_index);
    if (relation == "<")
        where_delete.relation_character = LESS;
    else if (relation == "< =")
        where_delete.relation_character = LESS_OR_EQUAL;
    else if (relation == "=")
        where_delete.relation_character = EQUAL;
    else if (relation == "> =")
        where_delete.relation_character = GREATER_OR_EQUAL;
    else if (relation == ">")
        where_delete.relation_character = GREATER;
    else if (relation == "! =")
        where_delete.relation_character = NOT_EQUAL;
    else
        throw 1;//��ʽ����
    string value_delete = getWord(check_index + 1, check_index);

    Attribute tmp_attr = CM.getAttribute(table_name);
    for (int i = 0; i < tmp_attr.num; i++)
    {
        if (attr_name == tmp_attr.name[i]) {
            where_delete.data.type = tmp_attr.type[i];
            switch (where_delete.data.type) {
            case -1:
                try {
                    where_delete.data.datai = stringToNum<int>(value_delete);
                }
                catch (...) {
                    throw data_type_conflict();//ת��ʧ��
                }
                break;
            case 0:
                try {
                    where_delete.data.dataf = stringToNum<float>(value_delete);
                }
                catch (...) {
                    throw data_type_conflict();//ת��ʧ��
                }
                break;
            default://��������Ϊstringʱ
                try {
                    if (!(value_delete[0] == '\'' && value_delete[value_delete.length() - 1] == '\'') && !(value_delete[0] == '"' && value_delete[value_delete.length() - 1] == '"'))
                        throw 1;//��ʽ����
                    where_delete.data.datas = value_delete.substr(1, value_delete.length() - 2);
                }
                catch (...) {
                    throw data_type_conflict();//ת��ʧ��
                }
                break;
            }
            break;
        }
    }
    _API.deleteRecord(table_name, attr_name, where_delete);
    cout << ">>> SUCCESS" << endl;
}

//Insert
void Interpreter::EXEC_INSERT() {
    
    string table_name;
    int check_index;
    Tuple tuple_insert;
    Attribute attr_exist;
    if (getLower(query, 7).substr(7, 4) != "into")
        throw input_format_error();
    table_name = getWord(12, check_index);
    if (getLower(query, check_index + 1).substr(check_index + 1, 6) != "values")
        throw input_format_error();
    check_index += 8;
    if (query[check_index] != '(')
        throw input_format_error();
    if (!CM.hasTable(table_name))
        throw table_not_exist();

    attr_exist = CM.getAttribute(table_name);
    check_index--;
    int num_of_insert = 0;
    //�������ڵ�����Ԫ�ؽ��б���
    while (query[check_index + 1] != '\0' && query[check_index + 1] != ')') {
        if (num_of_insert >= attr_exist.num)
            throw 1;//����������ƥ��
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
        default://��������Ϊstringʱ
            try {
                if (!(value_insert[0] == '\'' && value_insert[value_insert.length() - 1] == '\'') 
                 && !(value_insert[0] == '"' && value_insert[value_insert.length() - 1] == '"'))
                    throw input_format_error();//��ʽ����ȷ
                if (value_insert.length() - 1 > attr_exist.type[num_of_insert])
                    throw input_format_error();//���ȳ�������
                insert_data.datas = value_insert.substr(1, value_insert.length() - 2);
            }
            catch (input_format_error error) {
                throw input_format_error();
            }
            catch (...) {
                throw data_type_conflict();//转换失败
            }
            break;
        }
        tuple_insert.addData(insert_data);
        num_of_insert++;
    }
    if (query[check_index + 1] == '\0')
        throw input_format_error();//��ʽ����
    if (num_of_insert != attr_exist.num)
        throw input_format_error();//�������������ȷ

    _API.insertRecord(table_name, tuple_insert);
    cout << ">>> SUCCESS" << endl;
}

//��ӡhelp����
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
    cout << "  insert into tableName values(attr1=val1, attr2=val2, ... , attrN=valN);" << endl;
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

    _API.dropTable(table_name);
    cout << ">>> SUCCESS" << endl;
}

void Interpreter::EXEC_CREATE_TABLE() {
    
    Index index;
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
        if (query.substr(check_index+3, 11) == "primary key")//�ж�����
        {
            check_index += 17;
            string temp = getWord(check_index, check_index);
            for (int j = 0; j < i ; j++)
                if (attr_.name[j] == temp)
                {
                    primary = j;
                    break;
                }
            check_index += 3;
        }
    }
    attr_.primary_key = primary;
    _API.createTable(table_name, attr_, primary, index);
}
void Interpreter::EXEC_SELECT() {
    
    string table_name;
    vector<string> attr_name;
    vector<string> target_name;
    vector<Where> where_select;
    string tmp_target_name;
    string tmp_value;
    Where tmp_where;
    string relation;
    Table output_table;
    char op = 0;
    int check_index;
    int flag = 0;
    //ѡ��Ԫ��
    
    //�ж��Ƿ�Ϊselect *
    if (getWord(7, check_index) == "*")
    {
        flag = 1;
        check_index++;
    }
    else {
        check_index = 7;
        //�����е��漰��������
        while (1) {
            attr_name.push_back(getWord(check_index, check_index));
            if (query[++check_index] != ',')
                break;
            else
                check_index += 2;
        }
    }

    if (getLower(query, check_index).substr(check_index, 4) != "from")
        throw input_format_error();//��ʽ����
    check_index += 5;
    table_name = getWord(check_index, check_index);
    if (!CM.hasTable(table_name))
        throw table_not_exist();
    Attribute tmp_attr = CM.getAttribute(table_name);
    if (!flag) {
        for (int index = 0; index < attr_name.size(); index++) {
            if (!CM.hasAttribute(table_name, attr_name[index]))
                throw attribute_not_exist();
        }
    }
    else {
        for (int index = 0; index < tmp_attr.num; index++) {
            attr_name.push_back(tmp_attr.name[index]);
        }
    }
    check_index++;
    if (query[check_index] == '\0')
        output_table = _API.selectRecord(table_name, target_name, where_select, op);
    else {
        if (getLower(query, check_index).substr(check_index, 5) != "where")
            throw input_format_error();//��ʽ����
        check_index += 6;
        while (1) {
            tmp_target_name = getWord(check_index, check_index);
            if (!CM.hasAttribute(table_name, tmp_target_name))
                throw attribute_not_exist();
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
                throw input_format_error();//��ʽ����
            tmp_value = getWord(check_index + 1, check_index);
            for (int i = 0; i < tmp_attr.num; i++)
            {
                if (tmp_target_name == tmp_attr.name[i]) {
                    tmp_where.data.type = tmp_attr.type[i];
                    switch (tmp_where.data.type) {
                    case -1:
                        try {
                            tmp_where.data.datai = stringToNum<int>(tmp_value);
                        }
                        catch (...) {
                            throw data_type_conflict();//ת��ʧ��
                        }
                        break;
                    case 0:
                        try {
                            tmp_where.data.dataf = stringToNum<float>(tmp_value);
                        }
                        catch (...) {
                            throw data_type_conflict();//ת��ʧ��
                        }
                        break;
                    default:
                        try {
                            if (!(tmp_value[0] != '\'' && tmp_value[tmp_value.length() - 1] != '\'') && !(tmp_value[0] != '"' && tmp_value[tmp_value.length() - 1] != '"'))
                                throw input_format_error();//��ʽ����ȷ
                            tmp_where.data.datas = tmp_value.substr(1, tmp_value.length() - 2);
                        }
                        catch (input_format_error error) {
                            throw input_format_error();
                        }
                        catch (...) {
                            throw data_type_conflict();//ת��ʧ��
                        }
                    }
                    break;
                }
            }

            where_select.push_back(tmp_where);
            //�ҹ�ϵ����
            if (query[check_index + 1] == '\0')
                break;
            else if (getLower(query, check_index + 1).substr(check_index + 1, 3) == "and")
                op = 1;
            else if (getLower(query, check_index + 1).substr(check_index + 1, 2) == "or")
                op = 0;
            else
                throw 1;
            getWord(check_index + 1, check_index);
            check_index++;
        }

        output_table = _API.selectRecord(table_name, target_name, where_select, op);
    }

    //���

    Attribute attr_record = output_table.attr_;
    int use[32] = { 0 };
    if (attr_name.size() == 0) {
        for (int i = 0; i < attr_record.num; i++)
            use[i] = i;
    }
    else {
        for (int i = 0; i < attr_name.size(); i++)
            for (int j = 0; j < attr_record.num; j++) {
                if (attr_record.name[j] == attr_name[i])
                {
                    use[i] = j;
                    break;
                }
            }
    }
    vector<Tuple> output_tuple = output_table.getTuple();
    int longest = -1;
    for (int index = 0; index < attr_name.size(); index++) {
        if ((int)attr_record.name[use[index]].length() > longest)
            longest = (int)attr_record.name[use[index]].length();
    }
    for (int index = 0; index < attr_name.size(); index++) {
        int type = attr_record.type[use[index]];
        if (type == -1) {
            for (int i = 0; i < output_tuple.size(); i++) {
                if (longest < getBits(output_tuple[i].getData()[use[index]].datai)) {
                    longest = getBits(output_tuple[i].getData()[use[index]].datai);
                }
            }
        }
        if (type == 0) {
            for (int i = 0; i < output_tuple.size(); i++) {
                if (longest < getBits(output_tuple[i].getData()[use[index]].dataf)) {
                    longest = getBits(output_tuple[i].getData()[use[index]].dataf);
                }
            }
        }
        if (type > 0) {
            for (int i = 0; i < output_tuple.size(); i++) {
                if (longest < output_tuple[i].getData()[use[index]].datas.length()) {
                    longest = (int)output_tuple[i].getData()[use[index]].datas.length();
                }
            }
        }
    }
    longest += 1;
    for (int index = 0; index < attr_name.size(); index++) {
        if (index != attr_name.size() - 1) {
            for (int i = 0; i < (longest - attr_record.name[use[index]].length()) / 2; i++)
                printf(" ");
            printf("%s", attr_record.name[use[index]].c_str());
            for (int i = 0; i < longest - (longest - attr_record.name[use[index]].length()) / 2 - attr_record.name[use[index]].length(); i++)
                printf(" ");
            printf("|");
        }
        else {
            for (int i = 0; i < (longest - attr_record.name[use[index]].length()) / 2; i++)
                printf(" ");
            printf("%s", attr_record.name[use[index]].c_str());
            for (int i = 0; i < longest - (longest - attr_record.name[use[index]].length()) / 2 - attr_record.name[use[index]].length(); i++)
                printf(" ");
            printf("\n");
        }
    }
    for (int index = 0; index < attr_name.size() * (longest + 1); index++) {
        cout << "-";
    }
    cout << endl;
    for (int index = 0; index < output_tuple.size(); index++) {
        for (int i = 0; i < attr_name.size(); i++)
        {
            switch (output_tuple[index].getData()[use[i]].type) {
            case -1:
                if (i != attr_name.size() - 1) {
                    int len = output_tuple[index].getData()[use[i]].datai;
                    len = getBits(len);
                    for (int i = 0; i < (longest - len) / 2; i++)
                        printf(" ");
                    printf("%d", output_tuple[index].getData()[use[i]].datai);
                    for (int i = 0; i < longest - (longest - len) / 2 - len; i++)
                        printf(" ");
                    printf("|");
                }
                else {
                    int len = output_tuple[index].getData()[use[i]].datai;
                    len = getBits(len);
                    for (int i = 0; i < (longest - len) / 2; i++)
                        printf(" ");
                    printf("%d", output_tuple[index].getData()[use[i]].datai);
                    for (int i = 0; i < longest - (longest - len) / 2 - len; i++)
                        printf(" ");
                    printf("\n");
                }
                break;
            case 0:
                if (i != attr_name.size() - 1) {
                    float num = output_tuple[index].getData()[use[i]].dataf;
                    int len = getBits(num);
                    for (int i = 0; i < (longest - len) / 2; i++)
                        printf(" ");
                    printf("%.2f", output_tuple[index].getData()[use[i]].dataf);
                    for (int i = 0; i < longest - (longest - len) / 2 - len; i++)
                        printf(" ");
                    printf("|");
                }
                else {
                    float num = output_tuple[index].getData()[use[i]].dataf;
                    int len = getBits(num);
                    for (int i = 0; i < (longest - len) / 2; i++)
                        printf(" ");
                    printf("%.2f", output_tuple[index].getData()[use[i]].dataf);
                    for (int i = 0; i < longest - (longest - len) / 2 - len; i++)
                        printf(" ");
                    printf("\n");
                }
                break;
            default:
                string tmp = output_tuple[index].getData()[use[i]].datas;
                if (i != attr_name.size() - 1) {
                    for (int i = 0; i < (longest - tmp.length()) / 2; i++)
                        printf(" ");
                    printf("%s", tmp.c_str());
                    for (int i = 0; i < longest - (longest - (int)tmp.length()) / 2 - (int)tmp.length(); i++)
                        printf(" ");
                    printf("|");
                }
                else {
                    string tmp = output_tuple[index].getData()[i].datas;
                    for (int i = 0; i < (longest - tmp.length()) / 2; i++)
                        printf(" ");
                    printf("%s", tmp.c_str());
                    for (int i = 0; i < longest - (longest - (int)tmp.length()) / 2 - (int)tmp.length(); i++)
                        printf(" ");
                    printf("\n");
                }
                break;
            }
        }
    }
}


