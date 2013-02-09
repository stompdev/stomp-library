#include "stomp.h"
#include "stomp-network.h"

#include "stomp-tools.h"
#include "stomp-strings.h"
#include "stomp-frame.h"

#include "contiki.h"
#include "contiki-net.h"

#include "uip-debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char stomp_version_default[4] = {0x31, 0x2e, 0x31,};

const char stomp_content_type_default[11] = {0x74, 0x65, 0x78, 0x74, 0x2f, 0x70, 0x6c, 0x61, 0x69, 0x6e,};

static void
__send(struct stomp_frame *frame) {
    char *buf = NULL;
    int len = 0;

    len = stomp_frame_length(frame);
    buf = stomp_frame_export(frame);
    stomp_frame_delete_frame(frame);

    stomp_network_send(buf, len);
    DELETE(buf);
}

void
stomp_connect(char* host, char* login, char* passcode) {
    struct stomp_header *headers = NULL;

    headers = stomp_frame_new_header(stomp_header_accept_version, stomp_version_default);
    if (passcode != NULL) {
        headers = stomp_frame_add_header(stomp_header_passcode, passcode, headers);
    }
    if (login != NULL) {
        headers = stomp_frame_add_header(stomp_header_login, login, headers);
    }
    if (host != NULL) {
        headers = stomp_frame_add_header(stomp_header_host, host, headers);
    } else {
#if STOMP_DEBUG > 2
        PRINTA("stomp_connect: no host for CONNECT. Abort.\n");
#endif
        stomp_frame_delete_header(headers);
        return;
    }

    __send(stomp_frame_new_frame(stomp_command_connect, headers, NULL));
}

void
stomp_subscribe(char *id, char *destination, char *ack) {
    struct stomp_header *headers = NULL;

    if (ack != NULL) {
        headers = stomp_frame_new_header(stomp_header_ack, ack);
    } else {
#if STOMP_DEBUG > 2
        PRINTA("stomp_subscribe: no ack for SUBSCRIBE. Set to 'auto'.\n");
#endif
        headers = stomp_frame_new_header(stomp_header_ack, stomp_header_auto);
    }
    if (destination != NULL) {
        headers = stomp_frame_add_header(stomp_header_destination, destination, headers);
    } else {
#if STOMP_DEBUG > 2
        PRINTA("stomp_subscribe: no destination for SUBSCRIBE. Abort.\n");
#endif
        stomp_frame_delete_header(headers);
        return;
    }
    if (id != NULL) {
        headers = stomp_frame_add_header(stomp_header_id, id, headers);
    } else {
#if STOMP_DEBUG > 2
        PRINTA("stomp_subscribe: no id for SUBSCRIBE. Abort.\n");
#endif
        stomp_frame_delete_header(headers);
        return;
    }

    __send(stomp_frame_new_frame(stomp_command_subscribe, headers, NULL));
}

void
stomp_unsubscribe(char *id) {
    struct stomp_header *headers = NULL;

    if (id != NULL) {
        headers = stomp_frame_new_header(stomp_header_id, id);
    } else {
#if STOMP_DEBUG > 2
        PRINTA("stomp_unsubscribe: no id for UNSUBSCRIBE. Abort.\n");
#endif
        stomp_frame_delete_header(headers);
        return;
    }

    __send(stomp_frame_new_frame(stomp_command_unsubscribe, headers, NULL));
}

void
stomp_send(char *destination, char *type, char *length, char *receipt, char *tx, char *message) {
    struct stomp_header *headers = NULL;

    if (tx != NULL) {
        headers = stomp_frame_new_header(stomp_header_transaction, tx);
    }
    if (receipt != NULL) {
        headers = stomp_frame_add_header(stomp_header_receipt, receipt, headers);
    }
    if (length != NULL) {
        headers = stomp_frame_add_header(stomp_header_content_length, length, headers);
    } else {
        char *_length = NEW_ARRAY(char, 4);
#if STOMP_DEBUG > 2
        PRINTA("stomp_send: no content-length for SEND. Set to computed value.\n");
#endif
        if (message != NULL) {
            sprintf((char*) _length, "%u", (unsigned int) strlen((char*) message));
        } else {
            sprintf((char*) _length, "0");
        }
        headers = stomp_frame_add_header(stomp_header_content_length, _length, headers);

        DELETE(_length);
    }
    if (type != NULL) {
        headers = stomp_frame_add_header(stomp_header_content_type, type, headers);
    } else {
#if STOMP_DEBUG > 2
        PRINTA("stomp_send: no content-type for SEND. Set to 'text/plain'.\n");
#endif
        headers = stomp_frame_add_header(stomp_header_content_type, stomp_content_type_default, headers);
    }
    if (destination != NULL) {
        headers = stomp_frame_add_header(stomp_header_destination, destination, headers);
    } else {
#if STOMP_DEBUG > 2
        PRINTA("stomp_send: no destination for SEND. Abort.\n");
#endif
        stomp_frame_delete_header(headers);
        return;
    }

    __send(stomp_frame_new_frame(stomp_command_send, headers, message));
}

void
stomp_ack(char *subscription, char *message_id, char *tx) {
    struct stomp_header *headers = NULL;

    if (subscription != NULL) {
        headers = stomp_frame_new_header(stomp_header_subscription, subscription);
    } else {
#if STOMP_DEBUG > 2
        PRINTA("stomp_ack: no subscription for ACK. Abort.\n");
#endif
        stomp_frame_delete_header(headers);
        return;
    }
    if (message_id != NULL) {
        headers = stomp_frame_new_header(stomp_header_message_id, message_id);
    } else {
#if STOMP_DEBUG > 2
        PRINTA("stomp_ack: no message_id for ACK. Abort.\n");
#endif
        stomp_frame_delete_header(headers);
        return;
    }
    if (tx != NULL) {
        headers = stomp_frame_new_header(stomp_header_transaction, tx);
    } else {
#if STOMP_DEBUG > 2
        PRINTA("stomp_ack: no tx for ACK. Abort.\n");
#endif
        stomp_frame_delete_header(headers);
        return;
    }

    __send(stomp_frame_new_frame(stomp_command_ack, headers, NULL));
}

void
stomp_nack(char *subscription, char *message_id, char *tx) {
    struct stomp_header *headers = NULL;

    if (subscription != NULL) {
        headers = stomp_frame_new_header(stomp_header_subscription, subscription);
    } else {
#if STOMP_DEBUG > 2
        PRINTA("stomp_ack: no subscription for NACK. Abort.\n");
#endif
        stomp_frame_delete_header(headers);
        return;
    }
    if (message_id != NULL) {
        headers = stomp_frame_new_header(stomp_header_message_id, message_id);
    } else {
#if STOMP_DEBUG > 2
        PRINTA("stomp_ack: no message_id for NACK. Abort.\n");
#endif
        stomp_frame_delete_header(headers);
        return;
    }
    if (tx != NULL) {
        headers = stomp_frame_new_header(stomp_header_transaction, tx);
    } else {
#if STOMP_DEBUG > 2
        PRINTA("stomp_ack: no tx for NACK. Abort.\n");
#endif
        stomp_frame_delete_header(headers);
        return;
    }

    __send(stomp_frame_new_frame(stomp_command_nack, headers, NULL));
}

void
stomp_begin(char *tx) {
    struct stomp_header *headers = NULL;

    if (tx != NULL) {
        headers = stomp_frame_new_header(stomp_header_transaction, tx);
    } else {
#if STOMP_DEBUG > 2
        PRINTA("stomp_begin: no tx for BEGIN. Abort.\n");
#endif
        stomp_frame_delete_header(headers);
        return;
    }

    __send(stomp_frame_new_frame(stomp_command_begin, headers, NULL));
}

void
stomp_commit(char *tx) {
    struct stomp_header *headers = NULL;

    if (tx != NULL) {
        headers = stomp_frame_new_header(stomp_header_transaction, tx);
    } else {
#if STOMP_DEBUG > 2
        PRINTA("stomp_commit: no tx for COMMIT. Abort.\n");
#endif
        stomp_frame_delete_header(headers);
        return;
    }

    __send(stomp_frame_new_frame(stomp_command_commit, headers, NULL));
}

void
stomp_abort(char *tx) {
    struct stomp_header *headers = NULL;

    if (tx != NULL) {
        headers = stomp_frame_new_header(stomp_header_transaction, tx);
    } else {
#if STOMP_DEBUG > 2
        PRINTA("stomp_abort: no tx for ABORT. Abort.\n");
#endif
        stomp_frame_delete_header(headers);
        return;
    }

    __send(stomp_frame_new_frame(stomp_command_abort, headers, NULL));
}

void
stomp_disconnect(char *receipt) {
    struct stomp_header *headers = NULL;

    if (receipt != NULL) {
        headers = stomp_frame_new_header(stomp_header_receipt, receipt);
    }

    __send(stomp_frame_new_frame(stomp_command_disconnect, headers, NULL));
}

void
stomp_network_sent(char *buf, int len) {
#if STOMP_DEBUG > 2
    PRINTA("Sent: {buf=\"%s\", len=%d}.\n", buf, len);
#endif
    stomp_sent(buf, len);
}

void
stomp_network_received(char *buf, int len) {
    struct stomp_frame *frame = NULL;
    struct stomp_header *header = NULL;

#if STOMP_DEBUG > 2
    PRINTA("Received: {buf=\"%s\", len=%d}.\n", buf, len);
#endif

    frame = stomp_frame_import(buf, len);
    if (frame != NULL && frame->command != NULL) {
        if (frame->command[0] == stomp_command_message[0]) {
            char *destination = NULL, *message_id = NULL, *subscription = NULL,
                    *content_type = NULL, *content_length = NULL;

            header = frame->headers;
            while (header != NULL && header->name != NULL) {
                if (header->name[0] == stomp_header_destination[0]) {
                    destination = header->value;
                } else if (header->name[0] == stomp_header_message_id[0]) {
                    message_id = header->value;
                } else if (header->name[0] == stomp_header_subscription[0]) {
                    subscription = header->value;
                } else if (header->name[0] == stomp_header_content_type[0]) {
                    if (strlen(header->name) > 8) {
                        if (header->name[8] == stomp_header_content_type[8]) {
                            content_type = header->value;
                        } else if (header->name[8] == stomp_header_content_length[8]) {
                            content_length = header->value;
                        }
                    }
                }
                header = header->next;
            }
            stomp_message(destination, message_id, subscription, content_type,
                    content_length, frame->payload);

        } else if (frame->command[0] == stomp_command_error[0]) {
            char *receipt_id = NULL, *content_type = NULL, *content_length = NULL;

            header = frame->headers;
            while (header != NULL && header->name != NULL) {
                if (header->name[0] == stomp_header_receipt_id[0]) {
                    receipt_id = header->value;
                } else if (header->name[0] == stomp_header_content_type[0]) {
                    if (strlen(header->name) > 8) {
                        if (header->name[8] == stomp_header_content_type[8]) {
                            content_type = header->value;
                        } else if (header->name[8] == stomp_header_content_length[8]) {
                            content_length = header->value;
                        }
                    }
                }
                header = header->next;
            }
            stomp_error(receipt_id, content_type, content_length, frame->payload);

        } else if (frame->command[0] == stomp_command_receipt[0]) {
            char *receipt_id = NULL;

            header = frame->headers;
            while (header != NULL && header->name != NULL) {
                if (header->name[0] == stomp_header_receipt_id[0]) {
                    receipt_id = header->value;
                }
                header = header->next;
            }
            stomp_receipt(receipt_id);

        } else if (frame->command[0] == stomp_command_connected[0]) {
            char *version = NULL, *server = NULL, *session = NULL, *host_id = NULL,
                    *heart_beat = NULL, *user_id = NULL;

            header = frame->headers;
            while (header != NULL && header->name != NULL) {
                if (header->name[0] == stomp_header_version[0]) {
                    version = header->value;
                } else if (header->name[0] == stomp_header_server[0]) {
                    if (strlen(header->name) > 2) {
                        if (header->name[2] == stomp_header_server[2]) {
                            server = header->value;
                        } else if (header->name[2] == stomp_header_session[2]) {
                            session = header->value;
                        }
                    }
                } else if (header->name[0] == stomp_header_host_id[0]) {
                    if (strlen(header->name) > 1) {
                        if (header->name[1] == stomp_header_host_id[1]) {
                            host_id = header->value;
                        } else if (header->name[1] == stomp_header_heart_beat[1]) {
                            heart_beat = header->value;
                        }
                    }
                } else if (header->name[0] == stomp_header_user_id[0]) {
                    user_id = header->value;
                }
                header = header->next;
            }
            stomp_connected(version, server, host_id, session, heart_beat, user_id);

        } else {
            stomp_received(buf, len);
        }
    } else {
        stomp_received(buf, len);
    }
    stomp_frame_delete_frame(frame);
}

void (*__stomp_sent)(char*, int) = NULL;

void
stomp_sent(char *buf, int len) {
    if (__stomp_sent != NULL) {
        __stomp_sent(buf, len);
    }
}

void (*__stomp_received)(char*, int) = NULL;

void
stomp_received(char *buf, int len) {
    if (__stomp_received != NULL) {
        __stomp_received(buf, len);
    }
}

void (*__stomp_connected)(char*, char*, char*, char*, char*, char*) = NULL;

void
stomp_connected(char *version, char *server, char *host_id, char *session, char *heart_beat, char *user_id) {
    if (__stomp_connected != NULL) {
        __stomp_connected(version, server, host_id, session, heart_beat, user_id);
    }
}

void (*__stomp_message)(char*, char*, char*, char*, char*, char*) = NULL;

void
stomp_message(char *destination, char *message_id, char *subscription, char *content_type, char *content_length, char *message) {
    if (__stomp_message != NULL) {
        __stomp_message(destination, message_id, subscription, content_type, content_length, message);
    }
}

void (*__stomp_error)(char*, char*, char*, char*) = NULL;

void
stomp_error(char *receipt_id, char *content_type, char *content_length, char *message) {
    if (__stomp_error != NULL) {
        __stomp_error(receipt_id, content_type, content_length, message);
    }
}

void (*__stomp_receipt)(char*) = NULL;

void
stomp_receipt(char *receipt_id) {
    if (__stomp_receipt != NULL) {
        __stomp_receipt(receipt_id);
    }
}
