
#ifndef STOMP_TOOLS_H
#define STOMP_TOOLS_H

#include <stdlib.h>

/* Malloc and memset new memory block */
void*
stomp_tools_new(int size);

/* Create new string and copy substr */
void
stomp_tools_substr(const char *src, char *dest, int offset, int length);

/* Create new string and copy substr ended with delim */
int
stomp_tools_substr_to(const char *src, char *dest, int offset, char delim, int length);

/* C Macros used to alloc and free memory */
#define NEW(T) (T*)stomp_tools_new(sizeof(T))
#define NEW_ARRAY(T, LEN) (T*)stomp_tools_new(sizeof(T) * (LEN))

#define DELETE(PTR) if (PTR != NULL) { free(PTR); PTR = NULL; }

#endif /* STOMP_TOOLS_H */
