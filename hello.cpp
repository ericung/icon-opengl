#include "loadfuncpp.h"

extern "C" int hello (value argv[]){
    argv[0] = "Hello World";
    return SUCCEEDED;
}
