#include <iostream>
#include "interpreter.h"
#include "CatalogManager.h"
#include "API.h"
#include "RecordManager.h"
#include "buffermanager.h"
#include "IndexManager.h"
#include "tuple.h"
#include "table.h"

int main()
{
    BufferManager bm;
    CatalogManager cm;
    IndexManager  im( bm );

    RecordManager rm( bm ,im );
    API api (rm ,cm );
    Interpreter interpreter( api , cm , rm );
    while (1)
    {
        try
        {
            cout << "********************************************************" << endl;
            cout << "                       MINISQL" << endl;
            cout << "********************************************************" << endl;
            cout << "Enter your SQL statement  " << endl;
            interpreter.getQuery();
            interpreter.EXEC();
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
        catch (exit_command error) {
            std::cout << ">>> Bye bye~" << std::endl;
            return(0);
        }
        catch ( input_format_error error ) {
            std::cout << ">>> Error: Input format error!" << std::endl;
        }
    }

}


