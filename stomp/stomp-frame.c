#include "stomp-frame.h"
#include "stomp-tools.h"

#include "uip-debug.h"

#include <stdio.h>
#include <string.h>

void
stomp_frame_delete_header(struct stomp_header *header) {
    struct stomp_header *ptr = NULL;

    while (header != NULL) {
        ptr = header;
        header = header->next;
        DELETE(ptr);
    }
}

void
stomp_frame_delete_frame(struct stomp_frame *frame) {
    if (frame != NULL) {
        stomp_frame_delete_header(frame->headers);
        DELETE(frame);
    }
}

struct stomp_header*
stomp_frame_new_header(const char *name, const char *value) {
    struct stomp_header *header = NEW(struct stomp_header);

    memcpy(header->name, name, strlen((char*) name));
    memcpy(header->value, value, strlen((char*) value));

    return header;
}

struct stomp_header*
stomp_frame_add_header(const char *name, const char *value, struct stomp_header *headers) {
    struct stomp_header *header = stomp_frame_new_header(name, value);
    header->next = headers;
    return header;
}

struct stomp_frame*
stomp_frame_new_frame(const char *command, struct stomp_header *headers, const char *payload) {
    struct stomp_frame *frame = NEW(struct stomp_frame);

    frame->headers = headers;

    if (command != NULL) {
        memcpy(frame->command, command, strlen((char*) command) + 1);
    }
    if (payload != NULL) {
        memcpy(frame->payload, payload, strlen((char*) payload) + 1);
    }

    return frame;
}

struct stomp_frame*
stomp_frame_import(const char *stream, int length) {
    int offset = 0;
    struct stomp_frame *frame = NULL;
    struct stomp_header *header = NULL;

    frame = NEW(struct stomp_frame);

    offset = stomp_tools_substr_to(stream, frame->command, offset, STOMP_NEW_LINE, length);

    while (stream + offset != NULL && *(stream + offset) != STOMP_NULL && *(stream + offset) != STOMP_NEW_LINE) {
        header = NEW(struct stomp_header);

        offset = stomp_tools_substr_to(stream, header->name, offset, STOMP_COLON, length);
        offset = stomp_tools_substr_to(stream, header->value, offset, STOMP_NEW_LINE, length);

        header->next = frame->headers;
        frame->headers = header;
    }

    offset = offset + 1;

    offset = stomp_tools_substr_to(stream, frame->payload, offset, STOMP_NULL, length);

    return frame;
}

char*
stomp_frame_export(struct stomp_frame *frame) {
    int lenght = 0, offset = 0, len = 0;
    struct stomp_header *header = NULL;
    char *stream = NULL;

    if (frame == NULL) {
        return NULL;
    }

    lenght = stomp_frame_length(frame);
    stream = NEW_ARRAY(char, lenght);
    memset(stream, 0, lenght);

    /* COMMAND */
    len = strlen((char*) frame->command);
    strncpy((char*) stream + offset, (const char*) frame->command, len);
    offset = offset + len;

    stream[offset] = STOMP_NEW_LINE;
    offset = offset + 1;

    /* HEADERS */
    header = frame->headers;
    while (header != NULL) {
        /* NAME */
        len = strlen((char*) header->name);
        strncpy((char*) stream + offset, (const char*) header->name, len);
        offset = offset + len;

        stream[offset] = STOMP_COLON;
        offset = offset + 1;

        /* VALUE */
        len = strlen((char*) header->value);
        strncpy((char*) stream + offset, (const char*) header->value, len);
        offset = offset + len;

        stream[offset] = STOMP_NEW_LINE;
        offset = offset + 1;

        header = header->next;
    }

    stream[offset] = STOMP_NEW_LINE;
    offset = offset + 1;

    /* PAYLOAD */
    if (frame->payload != NULL) {
        len = strlen((char*) frame->payload);
        strncpy((char*) stream + offset, (const char*) frame->payload, len);
        offset = offset + len;
    }

    /* END of FRAME */
    stream[offset] = 0x00;
    offset = offset + 1;

#if STOMP_DEBUG > 0
    if (offset != lenght) {
        PRINTA("Stomp frame offset not equals with size: %d != %d\n", offset, lenght);
    }
#endif

    return stream;
}

int
stomp_frame_length(struct stomp_frame *frame) {
    int sum = 0;
    struct stomp_header *header;

    if (frame == NULL) {
        return sum;
    }

    /* +1 bo na '\n' */
    sum = sum + strlen((char*) frame->command) + 1;

    header = frame->headers;
    while (header != NULL) {
        /* +2 na ':' i '\n' */
        sum = sum + strlen((char*) header->name) + strlen((char*) header->value) + 2;
        header = header->next;
    }

    if (frame->payload != NULL) {
        /* Zalozenie: payload nie zawiera znaku 0x00. */
        sum = sum + strlen((char*) frame->payload);
    }

    /* +2 na '\n' po naglowku i '\0' na koniec ramki */
    sum = sum + 2;

    return sum;
}