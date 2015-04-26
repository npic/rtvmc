#include <stdio.h>
#include "error.h"

void log_error(const char * message)
{
    if (message != NULL)
    {
        fprintf(stderr, "%s", message);
    }
    return;
}

