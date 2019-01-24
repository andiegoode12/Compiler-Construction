#include <stdlib.h>
#include <string.h>

char *strdup(const char *s1)
{
  char *s2;
  s2 = (char *)malloc(strlen(s1)+1);
  strcpy(s2, s1);
  return(s2);
}
