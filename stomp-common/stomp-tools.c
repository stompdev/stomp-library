#include "stomp-tools.h"

#include <string.h>

void*
stomp_tools_new(int size) {
    void *ptr = malloc(size);
    memset(ptr, 0, size);
    return ptr;
}

void
stomp_tools_substr(const char *src, char *dest, int offset, int length) {
    memcpy(dest, src + offset, length);
}

int
stomp_tools_substr_to(const char *src, char *dest, int offset, char delim, int length) {
    int len = 0;

    if (src != NULL && (src + offset) != NULL
            && (*(src + offset) != 0x00 || *(src + offset) != delim)) {

        while (offset + len < length && (src + offset + len) != NULL
                && *(src + offset + len) != 0x00 && *(src + offset + len) != delim) {
            len = len + 1;
        }
    }

    stomp_tools_substr(src, dest, offset, len);

    return offset + len + 1;
}