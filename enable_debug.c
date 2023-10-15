#include "structures.h"

void determine_debug()
{
    const char* envVar = getenv("DEBUG");

    if ((NULL != envVar) && (0 == strcmp(envVar, "1")))
    {
        debug = 1;
    }

}
