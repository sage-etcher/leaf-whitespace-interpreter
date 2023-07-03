#include <stdio.h>
#include <malloc.h>

int main (void)
{
char *ptr;

ptr = malloc (10 * sizeof (char));

printf ("test\n");

return 0;
}
