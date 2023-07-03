#include "fileio.h"

/* libraries */
#include <stdio.h>
#include <stdlib.h>


/* functions */
char *fio_file_contents (char *file_name)
{
    char *buffer = NULL;
    long length;
    FILE *f = fopen (file_name, "rb");

    if (f == NULL)
    {
        return NULL;
    }

    fseek (f, 0, SEEK_END);
    length = ftell (f);
    fseek (f, 0, SEEK_SET);
    buffer = malloc (length + 1);
    if (buffer)
    {
        fread (buffer, 1, length, f);
    }
    fclose (f);
    buffer[length] = '\0';

    return buffer;
}

