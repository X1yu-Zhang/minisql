#include "File.h"
File :: ~File(){
    Block * next;
    for(Block * tmp = head ; tmp ; tmp = next){
        next = tmp->next;
        delete tmp;
    }
}