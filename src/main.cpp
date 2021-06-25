#include <iostream>
#include "../include/interpreter.h"
#include "../include/CatalogManager.h"
#include "../include/API.h"
#include "../include/RecordManager.h"
#include "../include/buffermanager.h"
#include "../include/IndexManager.h"
#include "../include/tuple.h"
#include "../include/table.h"
#include <chrono>
int main()
{
    BufferManager bm;
    IndexManager  im( bm );
    CatalogManager cm(im);

    RecordManager rm( bm ,im );
    API api (rm ,cm ,im);
    Interpreter interpreter( api , cm , rm, im ,bm);
    chrono::steady_clock::time_point start, end;
    cout << "********************************************************" << endl;
    cout << "                       MINISQL" << endl;
    cout << "********************************************************" << endl;
    while (1)
    {
        try
        {
            cout << "Enter your SQL statement  " << endl;
            interpreter.getQuery();
            start = chrono::steady_clock::now();
            interpreter.EXEC();
            end = chrono::steady_clock::now();
            std::chrono::duration<double, std::micro> elapsed = end - start;
            cout << ">>> SUCCESS" << endl;
            cout << "It takes : "<< (double)elapsed.count()/ 1000000 << " seconds! "<< endl;
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


