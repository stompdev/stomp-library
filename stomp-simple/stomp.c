#include "stomp.h"
#include "stomp-network.h"

#include "stomp-tools.h"
#include "stomp-strings.h"

#include "contiki.h"
#include "contiki-net.h"

#include "uip-debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char stomp_version_default[4] = {0x31, 0x2e, 0x31,};

const char stomp_content_type_default[11] = {0x74, 0x65, 0x78, 0x74, 0x2f, 0x70, 0x6c, 0x61, 0x69, 0x6e,};

void
stomp_connect(char *host, char* login, char* pass) {
    int off = 0, total_len = 0, host_len = 0, login_len = 0, pass_len = 0;
    char *buf = NULL;

    total_len = STOMP_COMMAND_CONNECT_LEN + 1;
    if (host != NULL) {
        host_len = strlen(host);
        total_len += STOMP_HEADER_HOST_LEN + 1 + host_len + 1;
    } else {
#if STOMP_DEBUG > 2
        PRINTA("CONNECT: No host. Abort.\n");
#endif
        return;
    }
    if (login != NULL) {
        login_len = strlen(login);
        total_len += STOMP_HEADER_LOGIN_LEN + 1 + login_len + 1;
    }
#if STOMP_DEBUG > 2
    else {
        PRINTA("CONNECT: No login.\n");
    }
#endif
    if (pass != NULL) {
        pass_len = strlen(pass);
        total_len += STOMP_HEADER_PASSCODE_LEN + 1 + pass_len + 1;
    }
#if STOMP_DEBUG > 2
    else {
        PRINTA("CONNECT: No pass.\n");
    }
#endif
    total_len += STOMP_HEADER_ACCEPT_VERSION_LEN + 1 + 3 + 1 + 1;

    buf = NEW_ARRAY(char, total_len + 1);

    memcpy(buf + off, stomp_command_connect, STOMP_COMMAND_CONNECT_LEN);
    off += STOMP_COMMAND_CONNECT_LEN;
    *(buf + off) = STOMP_NEW_LINE;
    off += 1;

    memcpy(buf + off, stomp_header_host, STOMP_HEADER_HOST_LEN);
    off += STOMP_HEADER_HOST_LEN;
    *(buf + off) = STOMP_COLON;
    off += 1;
    memcpy(buf + off, host, host_len);
    off += host_len;
    *(buf + off) = STOMP_NEW_LINE;
    off += 1;

    if (login != NULL) {
        memcpy(buf + off, stomp_header_login, STOMP_HEADER_LOGIN_LEN);
        off += STOMP_HEADER_LOGIN_LEN;
        *(buf + off) = STOMP_COLON;
        off += 1;
        memcpy(buf + off, login, login_len);
        off += login_len;
        *(buf + off) = STOMP_NEW_LINE;
        off += 1;
    }

    if (pass != NULL) {
        memcpy(buf + off, stomp_header_passcode, STOMP_HEADER_PASSCODE_LEN);
        off += STOMP_HEADER_PASSCODE_LEN;
        *(buf + off) = STOMP_COLON;
        off += 1;
        memcpy(buf + off, pass, pass_len);
        off += pass_len;
        *(buf + off) = STOMP_NEW_LINE;
        off += 1;
    }

    memcpy(buf + off, stomp_header_accept_version, STOMP_HEADER_ACCEPT_VERSION_LEN);
    off += STOMP_HEADER_ACCEPT_VERSION_LEN;
    *(buf + off) = STOMP_COLON;
    off += 1;
    memcpy(buf + off, stomp_version_default, 3);
    off += 3;
    *(buf + off) = STOMP_NEW_LINE;
    off += 1;

    *(buf + off) = STOMP_NEW_LINE;
    off += 1;

#if STOMP_DEBUG > 0
    if (off != total_len) {
        PRINTA("CONNECT: Length doesn't match: off(%d) != total_len(%d).\n", off, total_len);
    }
#endif
#if STOMP_DEBUG > 2
    PRINTA("CONNECT: {host=\"%s\", login=\"%s\", pass=\"%s\"}.\n", host, login, pass);
#endif
    stomp_network_send(buf, total_len + 1);
    DELETE(buf);
}

void
stomp_subscribe(char *id, char *destination, char *ack) {
    int off = 0, total_len = 0, id_len = 0, destination_len = 0, ack_len = 0;
    char *buf = NULL;

    total_len = STOMP_COMMAND_SUBSCRIBE_LEN + 1;
    if (id != NULL) {
        id_len = strlen(id);
        total_len += STOMP_HEADER_ID_LEN + 1 + id_len + 1;
    } else {
#if STOMP_DEBUG > 2
        PRINTA("SUBSCRIBE: No id. Abort.\n");
#endif
        return;
    }
    if (destination != NULL) {
        destination_len = strlen(destination);
        total_len += STOMP_HEADER_DESTINATION_LEN + 1 + destination_len + 1;
    } else {
#if STOMP_DEBUG > 2
        PRINTA("SUBSCRIBE: No destination. Abort.\n");
#endif
        return;

    }
    if (ack != NULL) {
        ack_len = strlen(ack);
        total_len += STOMP_HEADER_ACK_LEN + 1 + ack_len + 1;
    } else {
#if STOMP_DEBUG > 2
        PRINTA("SUBSCRIBE: No ack. Set to 'auto'.\n");
#endif
        ack = (char*) stomp_header_auto;
        ack_len = 4;
        total_len += STOMP_HEADER_ACK_LEN + 1 + ack_len + 1;
    }
    total_len += 1;

    buf = NEW_ARRAY(char, total_len + 1);

    memcpy(buf + off, stomp_command_subscribe, STOMP_COMMAND_SUBSCRIBE_LEN);
    off += STOMP_COMMAND_SUBSCRIBE_LEN;
    *(buf + off) = STOMP_NEW_LINE;
    off += 1;

    if (id != NULL) {
        memcpy(buf + off, stomp_header_id, STOMP_HEADER_ID_LEN);
        off += STOMP_HEADER_ID_LEN;
        *(buf + off) = STOMP_COLON;
        off += 1;
        memcpy(buf + off, id, id_len);
        off += id_len;
        *(buf + off) = STOMP_NEW_LINE;
        off += 1;
    }

    if (destination != NULL) {
        memcpy(buf + off, stomp_header_destination, STOMP_HEADER_DESTINATION_LEN);
        off += STOMP_HEADER_DESTINATION_LEN;
        *(buf + off) = STOMP_COLON;
        off += 1;
        memcpy(buf + off, destination, destination_len);
        off += destination_len;
        *(buf + off) = STOMP_NEW_LINE;
        off += 1;
    }

    if (ack != NULL) {
        memcpy(buf + off, stomp_header_ack, STOMP_HEADER_ACK_LEN);
        off += STOMP_HEADER_ACK_LEN;
        *(buf + off) = STOMP_COLON;
        off += 1;
        memcpy(buf + off, ack, ack_len);
        off += ack_len;
        *(buf + off) = STOMP_NEW_LINE;
        off += 1;
    }

    *(buf + off) = STOMP_NEW_LINE;
    off += 1;

#if STOMP_DEBUG > 0
    if (off != total_len) {
        PRINTA("SUBSCRIBE: Length doesn't match: off(%d) != total_len(%d).\n", off, total_len);
    }
#endif
#if STOMP_DEBUG > 2
    PRINTA("SUBSCRIBE: {id=\"%s\", destination=\"%s\", ack=\"%s\"}\n", id, destination, ack);
#endif
    stomp_network_send(buf, total_len + 1);
    DELETE(buf);
}

void
stomp_unsubscribe(char *id) {
    int off = 0, total_len = 0, id_len = 0;
    char *buf = NULL;

    total_len = STOMP_COMMAND_UNSUBSCRIBE_LEN + 1;
    if (id != NULL) {
        id_len = strlen(id);
        total_len += STOMP_HEADER_ID_LEN + 1 + id_len + 1;
    } else {
#if STOMP_DEBUG > 2
        PRINTA("UNSUBSCRIBE: No id. Abort.\n");
#endif
        return;
    }
    total_len += 1;

    buf = NEW_ARRAY(char, total_len + 1);

    memcpy(buf + off, stomp_command_unsubscribe, STOMP_COMMAND_UNSUBSCRIBE_LEN);
    off += STOMP_COMMAND_UNSUBSCRIBE_LEN;
    *(buf + off) = STOMP_NEW_LINE;
    off += 1;

    if (id != NULL) {
        memcpy(buf + off, stomp_header_id, STOMP_HEADER_ID_LEN);
        off += STOMP_HEADER_ID_LEN;
        *(buf + off) = STOMP_COLON;
        off += 1;
        memcpy(buf + off, id, id_len);
        off += id_len;
        *(buf + off) = STOMP_NEW_LINE;
        off += 1;
    }

    *(buf + off) = STOMP_NEW_LINE;
    off += 1;

#if STOMP_DBEUG > 0
    if (off != total_len) {
        PRINTA("UNSUBSCRIBE: Length doesn't match: off(%d) != total_len(%d).\n", off, total_len);
    }
#endif
#if STOMP_DEBUG > 2
    PRINTA("UNSUBSCRIBE: {id=\"%s\"}.\n", id);
#endif
    stomp_network_send(buf, total_len + 1);
    DELETE(buf);
}

void
stomp_send(char *destination, char *type, char *length, char *receipt, char *tx, char *message) {
    int off = 0, total_len = 0, destination_len = 0, type_len = 0, length_len = 0, receipt_len = 0, tx_len = 0, message_len = 0;
    char *buf = NULL;

    total_len = STOMP_COMMAND_SEND_LEN + 1;
    if (destination != NULL) {
        destination_len = strlen(destination);
        total_len += STOMP_HEADER_DESTINATION_LEN + 1 + destination_len + 1;
    } else {
#if STOMP_DEBUG > 2
        PRINTA("SEND: No destination. Abort.\n");
#endif
        return;
    }
    if (type != NULL) {
        type_len = strlen(type);
        total_len += STOMP_HEADER_CONTENT_TYPE_LEN + 1 + type_len + 1;
    } else {
#if STOMP_DEBUG > 2
        PRINTA("SEND: No content type. Set to default 'plain/text'.\n");
#endif
        type = (char*) stomp_content_type_default;
        type_len = 10;
        total_len += STOMP_HEADER_CONTENT_TYPE_LEN + 1 + type_len + 1;
    }
    if (length != NULL) {
        length_len = strlen(length);
        total_len += STOMP_HEADER_CONTENT_LENGTH_LEN + 1 + length_len + 1;
    }
#if STOMP_DEBUG > 2
    else {
        PRINTA("SEND: No length.\n");
    }
#endif
    if (receipt != NULL) {
        receipt_len = strlen(receipt);
        total_len += STOMP_HEADER_RECEIPT_LEN + 1 + receipt_len + 1;
    }
#if STOMP_DEBUG > 2
    else {
        PRINTA("SEND: No receipt.\n");
    }
#endif
    if (tx != NULL) {
        tx_len = strlen(tx);
        total_len += STOMP_HEADER_TRANSACTION_LEN + 1 + tx_len + 1;
    }
#if STOMP_DEBUG > 2
    else {
        PRINTA("SEND: No tx.\n");
    }
#endif
    if (message != NULL) {
        message_len = strlen(message);
        total_len += message_len;
    } else {
#if STOMP_DEBUG > 2
        PRINTA("SEND: No message. Abort.\n");
#endif
        return;
    }
    total_len += 1;

    buf = NEW_ARRAY(char, total_len + 1);

    memcpy(buf + off, stomp_command_send, STOMP_COMMAND_SEND_LEN);
    off += STOMP_COMMAND_SEND_LEN;
    *(buf + off) = STOMP_NEW_LINE;
    off += 1;

    if (destination != NULL) {
        memcpy(buf + off, stomp_header_destination, STOMP_HEADER_DESTINATION_LEN);
        off += STOMP_HEADER_DESTINATION_LEN;
        *(buf + off) = STOMP_COLON;
        off += 1;
        memcpy(buf + off, destination, destination_len);
        off += destination_len;
        *(buf + off) = STOMP_NEW_LINE;
        off += 1;
    }

    if (type != NULL) {
        memcpy(buf + off, stomp_header_content_type, STOMP_HEADER_CONTENT_TYPE_LEN);
        off += STOMP_HEADER_CONTENT_TYPE_LEN;
        *(buf + off) = STOMP_COLON;
        off += 1;
        memcpy(buf + off, type, type_len);
        off += type_len;
        *(buf + off) = STOMP_NEW_LINE;
        off += 1;
    }

    if (length != NULL) {
        memcpy(buf + off, stomp_header_content_length, STOMP_HEADER_CONTENT_LENGTH_LEN);
        off += STOMP_HEADER_CONTENT_LENGTH_LEN;
        *(buf + off) = STOMP_COLON;
        off += 1;
        memcpy(buf + off, length, length_len);
        off += length_len;
        *(buf + off) = STOMP_NEW_LINE;
        off += 1;
    }

    if (receipt != NULL) {
        memcpy(buf + off, stomp_header_receipt, STOMP_HEADER_RECEIPT_LEN);
        off += STOMP_HEADER_RECEIPT_LEN;
        *(buf + off) = STOMP_COLON;
        off += 1;
        memcpy(buf + off, receipt, receipt_len);
        off += receipt_len;
        *(buf + off) = STOMP_NEW_LINE;
        off += 1;
    }

    if (tx != NULL) {
        memcpy(buf + off, stomp_header_transaction, STOMP_HEADER_TRANSACTION_LEN);
        off += STOMP_HEADER_TRANSACTION_LEN;
        *(buf + off) = STOMP_COLON;
        off += 1;
        memcpy(buf + off, tx, tx_len);
        off += tx_len;
        *(buf + off) = STOMP_NEW_LINE;
        off += 1;
    }

    *(buf + off) = STOMP_NEW_LINE;
    off += 1;

    if (message != NULL) {
        memcpy(buf + off, message, message_len);
        off += message_len;
    }

#if STOMP_DEBUG > 0
    if (off != total_len) {
        PRINTA("SEND: Length doesn't match: off(%d) != total_len(%d).\n", off, total_len);
    }
#endif
#if STOMP_DEBUG > 2
    PRINTA("SEND: {destination=\"%s\", type=\"%s\", length=\"%s\", receipt=\"%s\", tx=\"%s\", message=\"%s\"}\n",
            destination, type, length, receipt, tx, message);
#endif
    stomp_network_send(buf, total_len + 1);
    DELETE(buf);
}

void
stomp_ack(char *subscription, char *message_id, char *tx) {
    int off = 0, total_len = 0, subscription_len = 0, message_id_len = 0, tx_len = 0;
    char *buf = NULL;

    total_len = STOMP_COMMAND_ACK_LEN + 1;
    if (subscription != NULL) {
        subscription_len = strlen(subscription);
        total_len += STOMP_HEADER_SUBSCRIPTION_LEN + 1 + subscription_len + 1;
    } else {
#if STOMP_DEBUG > 2
        PRINTA("ACK: No subscription. Abort.\n");
#endif
        return;
    }
    if (message_id != NULL) {
        message_id_len = strlen(message_id);
        total_len += STOMP_HEADER_MESSAGE_ID_LEN + 1 + message_id_len + 1;
    } else {
#if STOMP_DEBUG > 2
        PRINTA("ACK: No message-id. Abort.\n");
#endif
        return;
    }
    if (tx != NULL) {
        tx_len = strlen(tx);
        total_len += STOMP_HEADER_TRANSACTION_LEN + 1 + tx_len + 1;
    } else {
#if STOMP_DEBUG > 2
        PRINTA("ACK: No transaction. Abort.\n");
#endif
        return;
    }
    total_len += 1;

    buf = NEW_ARRAY(char, total_len + 1);

    memcpy(buf, stomp_command_ack, STOMP_COMMAND_ACK_LEN);
    off += STOMP_COMMAND_ACK_LEN;
    *(buf + off) = STOMP_NEW_LINE;
    off += 1;

    if (subscription != NULL) {
        memcpy(buf + off, stomp_header_subscription, STOMP_HEADER_SUBSCRIPTION_LEN);
        off += STOMP_HEADER_SUBSCRIPTION_LEN;
        *(buf + off) = STOMP_COLON;
        off += 1;
        memcpy(buf + off, subscription, subscription_len);
        off += subscription_len;
        *(buf + off) = STOMP_NEW_LINE;
        off += 1;
    }
    if (message_id != NULL) {
        memcpy(buf + off, stomp_header_message_id, STOMP_HEADER_MESSAGE_ID_LEN);
        off += STOMP_HEADER_MESSAGE_ID_LEN;
        *(buf + off) = STOMP_COLON;
        off += 1;
        memcpy(buf + off, message_id, message_id_len);
        off += message_id_len;
        *(buf + off) = STOMP_NEW_LINE;
        off += 1;
    }
    if (tx != NULL) {
        memcpy(buf + off, stomp_header_transaction, STOMP_HEADER_TRANSACTION_LEN);
        off += STOMP_HEADER_TRANSACTION_LEN;
        *(buf + off) = STOMP_COLON;
        off += 1;
        memcpy(buf + off, tx, tx_len);
        off += tx_len;
        *(buf + off) = STOMP_NEW_LINE;
        off += 1;
    }

    *(buf + off) = STOMP_NEW_LINE;
    off += 1;

#if STOMP_DEBUG > 0
    if (off != total_len) {
        PRINTA("ACK: Length doesn't match: off(%d) != total_len(%d).\n", off, total_len);
    }
#endif
#if STOMP_DEBUG > 2
    PRINTA("ACK: {subscription=\"%s\", message-id=\"%s\", tx=\"%s\"}.\n", subscription, message_id, tx);
#endif
    stomp_network_send(buf, total_len + 1);
    DELETE(buf);
}

void
stomp_nack(char *subscription, char *message_id, char *tx) {
    int off = 0, total_len = 0, subscription_len = 0, message_id_len = 0, tx_len = 0;
    char *buf = NULL;

    total_len = STOMP_COMMAND_NACK_LEN + 1;
    if (subscription != NULL) {
        subscription_len = strlen(subscription);
        total_len += STOMP_HEADER_SUBSCRIPTION_LEN + 1 + subscription_len + 1;
    } else {
#if STOMP_DEBUG > 2
        PRINTA("NACK: No subscription. Abort.\n");
#endif
        return;
    }
    if (message_id != NULL) {
        message_id_len = strlen(message_id);
        total_len += STOMP_HEADER_MESSAGE_ID_LEN + 1 + message_id_len + 1;
    } else {
#if STOMP_DEBUG > 2
        PRINTA("NACK: No message-id. Abort.\n");
#endif
        return;
    }
    if (tx != NULL) {
        tx_len = strlen(tx);
        total_len += STOMP_HEADER_TRANSACTION_LEN + 1 + tx_len + 1;
    } else {
#if STOMP_DEBUG > 2
        PRINTA("NACK: No transaction. Abort.\n");
#endif
        return;
    }
    total_len += 1;

    buf = NEW_ARRAY(char, total_len + 1);

    memcpy(buf, stomp_command_nack, STOMP_COMMAND_NACK_LEN);
    off += STOMP_COMMAND_NACK_LEN;
    *(buf + off) = STOMP_NEW_LINE;
    off += 1;

    if (subscription != NULL) {
        memcpy(buf + off, stomp_header_subscription, STOMP_HEADER_SUBSCRIPTION_LEN);
        off += STOMP_HEADER_SUBSCRIPTION_LEN;
        *(buf + off) = STOMP_COLON;
        off += 1;
        memcpy(buf + off, subscription, subscription_len);
        off += subscription_len;
        *(buf + off) = STOMP_NEW_LINE;
        off += 1;
    }
    if (message_id != NULL) {
        memcpy(buf + off, stomp_header_message_id, STOMP_HEADER_MESSAGE_ID_LEN);
        off += STOMP_HEADER_MESSAGE_ID_LEN;
        *(buf + off) = STOMP_COLON;
        off += 1;
        memcpy(buf + off, message_id, message_id_len);
        off += message_id_len;
        *(buf + off) = STOMP_NEW_LINE;
        off += 1;
    }
    if (tx != NULL) {
        memcpy(buf + off, stomp_header_transaction, STOMP_HEADER_TRANSACTION_LEN);
        off += STOMP_HEADER_TRANSACTION_LEN;
        *(buf + off) = STOMP_COLON;
        off += 1;
        memcpy(buf + off, tx, tx_len);
        off += tx_len;
        *(buf + off) = STOMP_NEW_LINE;
        off += 1;
    }

    *(buf + off) = STOMP_NEW_LINE;
    off += 1;

#if STOMP_DEBUG > 0
    if (off != total_len) {
        PRINTA("NACK: Length doesn't match: off(%d) != total_len(%d).\n", off, total_len);
    }
#endif
#if STOMP_DEBUG > 2
    PRINTA("NACK: {subscription=\"%s\", message-id=\"%s\", tx=\"%s\"}\n", subscription, message_id, tx);
#endif
    stomp_network_send(buf, total_len + 1);
    DELETE(buf);
}

void
stomp_begin(char *tx) {
    int off = 0, total_len = 0, tx_len = 0;
    char *buf = NULL;

    total_len = STOMP_COMMAND_BEGIN_LEN + 1;
    if (tx != NULL) {
        tx_len = strlen(tx);
        total_len += STOMP_HEADER_TRANSACTION_LEN + 1 + tx_len + 1;
    } else {
#if STOMP_DEBUG > 2
        PRINTA("BEGIN: No tx. Abort.\n");
#endif
        return;
    }
    total_len += 1;

    buf = NEW_ARRAY(char, total_len + 1);

    memcpy(buf, stomp_command_begin, STOMP_COMMAND_BEGIN_LEN);
    off += STOMP_COMMAND_BEGIN_LEN;
    *(buf + off) = STOMP_NEW_LINE;
    off += 1;

    if (tx != NULL) {
        memcpy(buf + off, stomp_header_transaction, STOMP_HEADER_TRANSACTION_LEN);
        off += STOMP_HEADER_TRANSACTION_LEN;
        *(buf + off) = STOMP_COLON;
        off += 1;
        memcpy(buf + off, tx, tx_len);
        off += tx_len;
        *(buf + off) = STOMP_NEW_LINE;
        off += 1;
    }

    *(buf + off) = STOMP_NEW_LINE;
    off += 1;

#if STOMP_DEBUG > 0
    if (off != total_len) {
        PRINTA("BEGIN: Length doesn't match: off(%d) != total_len(%d).\n", off, total_len);
    }
#endif
#if STOMP_DEBUG > 2
    PRINTA("BEGIN: {tx=\"%s\"}\n", tx);
#endif
    stomp_network_send(buf, total_len + 1);
    DELETE(buf);
}

void
stomp_commit(char *tx) {
    int off = 0, total_len = 0, tx_len = 0;
    char *buf = NULL;

    total_len = STOMP_COMMAND_COMMIT_LEN + 1;
    if (tx != NULL) {
        tx_len = strlen(tx);
        total_len += STOMP_HEADER_TRANSACTION_LEN + 1 + tx_len + 1;
    } else {
#if STOMP_DEBUG > 2
        PRINTA("COMMIT: No tx. Abort.\n");
#endif
        return;
    }
    total_len += 1;

    buf = NEW_ARRAY(char, total_len + 1);

    memcpy(buf, stomp_command_commit, STOMP_COMMAND_COMMIT_LEN);
    off += STOMP_COMMAND_COMMIT_LEN;
    *(buf + off) = STOMP_NEW_LINE;
    off += 1;

    if (tx != NULL) {
        memcpy(buf + off, stomp_header_transaction, STOMP_HEADER_TRANSACTION_LEN);
        off += STOMP_HEADER_TRANSACTION_LEN;
        *(buf + off) = STOMP_COLON;
        off += 1;
        memcpy(buf + off, tx, tx_len);
        off += tx_len;
        *(buf + off) = STOMP_NEW_LINE;
        off += 1;
    }

    *(buf + off) = STOMP_NEW_LINE;
    off += 1;

#if STOMP_DEBUG > 0
    if (off != total_len) {
        PRINTA("COMMIT: Length doesn't match: off(%d) != total_len(%d).\n", off, total_len);
    }
#endif
#if STOMP_DEBUG > 2
    PRINTA("COMMIT: {tx=\"%s\"}.\n", tx);
#endif
    stomp_network_send(buf, total_len + 1);
    DELETE(buf);
}

void
stomp_abort(char *tx) {
    int off = 0, total_len = 0, tx_len = 0;
    char *buf = NULL;

    total_len = STOMP_COMMAND_ABORT_LEN + 1;
    if (tx != NULL) {
        tx_len = strlen(tx);
        total_len += STOMP_HEADER_TRANSACTION_LEN + 1 + tx_len + 1;
    } else {
#if STOMP_DEBUG > 2
        PRINTA("ABORT: No tx. Abort.\n");
#endif
        return;
    }
    total_len += 1;

    buf = NEW_ARRAY(char, total_len + 1);

    memcpy(buf, stomp_command_abort, STOMP_COMMAND_ABORT_LEN);
    off += STOMP_COMMAND_ABORT_LEN;
    *(buf + off) = STOMP_NEW_LINE;
    off += 1;

    if (tx != NULL) {
        memcpy(buf + off, stomp_header_transaction, STOMP_HEADER_TRANSACTION_LEN);
        off += STOMP_HEADER_TRANSACTION_LEN;
        *(buf + off) = STOMP_COLON;
        off += 1;
        memcpy(buf + off, tx, tx_len);
        off += tx_len;
        *(buf + off) = STOMP_NEW_LINE;
        off += 1;
    }

    *(buf + off) = STOMP_NEW_LINE;
    off += 1;

#if STOMP_DEBUG > 0
    if (off != total_len) {
        PRINTA("ABORT: Length doesn't match: off(%d) != total_len(%d).\n", off, total_len);
    }
#endif
#if STOMP_DEBUG > 2
    PRINTA("ABORT: {tx=\"%s\"}.\n", tx);
#endif
    stomp_network_send(buf, total_len + 1);
    DELETE(buf);
}

void
stomp_disconnect(char *receipt) {
    int off = 0, total_len = 0, receipt_len = 0;
    char *buf = NULL;

    total_len = STOMP_COMMAND_DISCONNECT_LEN + 1;
    if (receipt != NULL) {
        receipt_len = strlen(receipt);
        total_len += STOMP_HEADER_RECEIPT_LEN + 1 + receipt_len + 1;
    } else {
#if STOMP_DEBUG > 2
        PRINTA("DISCONNECT: No receipt. Abort.\n");
#endif
        return;
    }
    total_len += 1;

    buf = NEW_ARRAY(char, total_len + 1);

    memcpy(buf, stomp_command_disconnect, STOMP_COMMAND_DISCONNECT_LEN);
    off += STOMP_COMMAND_DISCONNECT_LEN;
    *(buf + off) = STOMP_NEW_LINE;
    off += 1;

    if (receipt != NULL) {
        memcpy(buf + off, stomp_header_receipt, STOMP_HEADER_RECEIPT_LEN);
        off += STOMP_HEADER_RECEIPT_LEN;
        *(buf + off) = STOMP_COLON;
        off += 1;
        memcpy(buf + off, receipt, receipt_len);
        off += receipt_len;
        *(buf + off) = STOMP_NEW_LINE;
        off += 1;
    }

    *(buf + off) = STOMP_NEW_LINE;
    off += 1;

#if STOMP_DEBUG > 0
    if (off != total_len) {
        PRINTA("DISCONNECT: Length doesn't match: off(%d) != total_len(%d).\n", off, total_len);
    }
#endif
#if STOMP_DEBUG > 2
    PRINTA("DISCONNECT: {receipt=\"%s\"}\n", receipt);
#endif
    stomp_network_send(buf, total_len + 1);
    DELETE(buf);
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
#if STOMP_DEBUG > 2
    PRINTA("Received: {buf=\"%s\", len=%d}.\n", buf, len);
#endif
    int off = 0;
    char *destination = NULL, *message_id = NULL, *subscription = NULL, *content_type = NULL,
            *content_length = NULL, *message = NULL, *receipt_id = NULL, *server = NULL,
            *host_id = NULL, *session = NULL, *heart_beat = NULL, *user_id = NULL, *version = NULL;

    if (buf != NULL) {
        if (*buf == stomp_command_message[0]) {
            while (buf != NULL && *buf != STOMP_NEW_LINE)
                buf += 1;
            if (buf == NULL)
                return;
            buf += 1;
            while (buf != NULL && *buf != STOMP_NEW_LINE) {
                if (*buf == stomp_header_destination[0]) {
                    while (buf != NULL && *buf != STOMP_COLON)
                        buf += 1;
                    if (buf == NULL)
                        return;
                    buf += 1;
                    off = 0;
                    while (buf + off != NULL && *(buf + off) != STOMP_NEW_LINE)
                        off += 1;
                    if (buf + off == NULL)
                        return;
                    destination = NEW_ARRAY(char, off + 1);
                    memcpy(destination, buf, off);
                    buf += off + 1;
                } else if (*buf == stomp_header_message_id[0]) {
                    while (buf != NULL && *buf != STOMP_COLON)
                        buf += 1;
                    if (buf == NULL)
                        return;
                    buf += 1;
                    off = 0;
                    while (buf + off != NULL && *(buf + off) != STOMP_NEW_LINE)
                        off += 1;
                    if (buf + off == NULL)
                        return;
                    message_id = NEW_ARRAY(char, off + 1);
                    memcpy(message_id, buf, off);
                    buf += off + 1;
                } else if (*buf == stomp_header_subscription[0]) {
                    buf += 1;
                    if (buf == NULL)
                        return;
                    if (*buf == stomp_header_subscription[1]) {
                        while (buf != NULL && *buf != STOMP_COLON)
                            buf += 1;
                        if (buf == NULL)
                            return;
                        buf += 1;
                        off = 0;
                        while (buf + off != NULL && *(buf + off) != STOMP_NEW_LINE)
                            off += 1;
                        if (buf + off == NULL)
                            return;
                        subscription = NEW_ARRAY(char, off + 1);
                        memcpy(subscription, buf, off);
                        buf += off + 1;
                    } else {
                        return;
                    }
                } else if (*buf == stomp_header_content_type[0]) {
                    while (buf != NULL && *buf != stomp_header_content_type[7])
                        buf++;
                    if (buf == NULL)
                        return;
                    buf += 1;
                    if (*buf == stomp_header_content_type[8]) {
                        while (buf != NULL && *buf != STOMP_COLON)
                            buf += 1;
                        if (buf == NULL)
                            return;
                        buf += 1;
                        off = 0;
                        while (buf + off != NULL && *(buf + off) != STOMP_NEW_LINE)
                            off += 1;
                        if (buf + off == NULL)
                            return;
                        content_type = NEW_ARRAY(char, off + 1);
                        memcpy(content_type, buf, off);
                        buf += off + 1;
                    } else if (*buf == stomp_header_content_length[8]) {
                        while (buf != NULL && *buf != STOMP_COLON)
                            buf += 1;
                        if (buf == NULL)
                            return;
                        buf += 1;
                        off = 0;
                        while (buf + off != NULL && *(buf + off) != STOMP_NEW_LINE)
                            off += 1;
                        if (buf + off == NULL)
                            return;
                        content_length = NEW_ARRAY(char, off + 1);
                        memcpy(content_length, buf, off);
                        buf += off + 1;
                    } else {
                        return;
                    }
                } else {
                    return;
                }
            }
            buf += 1;
            off = 0;
            while (buf + off != NULL && *(buf + off) != STOMP_NULL)
                off += 1;
            if (buf + off == NULL)
                return;
            message = NEW_ARRAY(char, off + 1);
            memcpy(message, buf, off);
            stomp_message(destination, message_id, subscription, content_type, content_length, message);
            DELETE(destination);
            DELETE(message_id);
            DELETE(subscription);
            DELETE(content_type);
            DELETE(content_length);
            DELETE(message);
        } else if (*buf == stomp_command_error[0]) {
            stomp_error(NULL, NULL, NULL, buf);
        } else if (*buf == stomp_command_receipt[0]) {
            while (buf != NULL && *buf != STOMP_NEW_LINE)
                buf += 1;
            if (buf == NULL)
                return;
            buf += 1;
            while (buf != NULL && *buf != STOMP_NEW_LINE) {
                if (*buf == stomp_header_receipt_id[0]) {
                    while (buf != NULL && *buf != STOMP_COLON)
                        buf += 1;
                    if (buf == NULL)
                        return;
                    buf += 1;
                    off = 0;
                    while (buf + off != NULL && *(buf + off) != STOMP_NEW_LINE)
                        off += 1;
                    if (buf + off == NULL)
                        return;
                    receipt_id = NEW_ARRAY(char, off + 1);
                    memcpy(receipt_id, buf, off);
                    buf += off + 1;
                }
            }
            stomp_receipt(receipt_id);
            DELETE(receipt_id);
        } else if (*buf == stomp_command_connected[0]) {
            while (buf != NULL && *buf != STOMP_NEW_LINE)
                buf += 1;
            if (buf == NULL)
                return;
            buf += 1;
            while (buf != NULL && *buf != STOMP_NEW_LINE) {
                if (*buf == stomp_header_version[0]) {
                    while (buf != NULL && *buf != STOMP_COLON)
                        buf += 1;
                    if (buf == NULL)
                        return;
                    buf += 1;
                    off = 0;
                    while (buf + off != NULL && *(buf + off) != STOMP_NEW_LINE)
                        off += 1;
                    if (buf + off == NULL)
                        return;
                    version = NEW_ARRAY(char, off + 1);
                    memcpy(version, buf, off);
                    buf += off + 1;
                } else if (*buf == stomp_header_server[0]) {
                    buf += 1;
                    if (buf == NULL)
                        return;
                    buf += 1;
                    if (buf == NULL)
                        return;
                    if (*buf == stomp_header_server[2]) {
                        while (buf != NULL && *buf != STOMP_COLON)
                            buf += 1;
                        if (buf == NULL)
                            return;
                        buf += 1;
                        off = 0;
                        while (buf + off != NULL && *(buf + off) != STOMP_NEW_LINE)
                            off += 1;
                        if (buf + off == NULL)
                            return;
                        server = NEW_ARRAY(char, off + 1);
                        memcpy(server, buf, off);
                        buf += off + 1;
                    } else if (*buf == stomp_header_session[2]) {
                        while (buf != NULL && *buf != STOMP_COLON)
                            buf += 1;
                        if (buf == NULL)
                            return;
                        buf += 1;
                        off = 0;
                        while (buf + off != NULL && *(buf + off) != STOMP_NEW_LINE)
                            off += 1;
                        if (buf + off == NULL)
                            return;
                        session = NEW_ARRAY(char, off + 1);
                        memcpy(session, buf, off);
                        buf += off + 1;
                    } else {
                        return;
                    }
                } else if (*buf == stomp_header_host_id[0]) {
                    buf += 1;
                    if (buf == NULL)
                        return;
                    if (*buf == stomp_header_host_id[1]) {
                        while (buf != NULL && *buf != STOMP_COLON)
                            buf++;
                        if (buf == NULL)
                            return;
                        buf += 1;
                        off = 0;
                        while (buf + off != NULL && *(buf + off) != STOMP_NEW_LINE)
                            off += 1;
                        if (buf + off == NULL)
                            return;
                        host_id = NEW_ARRAY(char, off + 1);
                        memcpy(host_id, buf, off);
                        buf += off + 1;
                    } else if (*buf == stomp_header_heart_beat[1]) {
                        while (buf != NULL && *buf != STOMP_COLON)
                            buf += 1;
                        if (buf == NULL)
                            return;
                        buf += 1;
                        off = 0;
                        while (buf + off != NULL && *(buf + off) != STOMP_NEW_LINE)
                            off += 1;
                        if (buf + off == NULL)
                            return;
                        heart_beat = NEW_ARRAY(char, off + 1);
                        memcpy(heart_beat, buf, off);
                        buf += off + 1;
                    } else {
                        return;
                    }
                } else if (*buf == stomp_header_user_id[0]) {
                    while (buf != NULL && *buf != STOMP_COLON)
                        buf += 1;
                    if (buf == NULL)
                        return;
                    buf += 1;
                    off = 0;
                    while (buf + off != NULL && *(buf + off) != STOMP_NEW_LINE)
                        off += 1;
                    if (buf + off == NULL)
                        return;
                    user_id = NEW_ARRAY(char, off + 1);
                    memcpy(user_id, buf, off);
                    buf += off + 1;
                }
            }
            stomp_connected(version, server, host_id, session, heart_beat, user_id);
            DELETE(version);
            DELETE(server);
            DELETE(host_id);
            DELETE(session);
            DELETE(heart_beat);
            DELETE(user_id);
        } else {
            stomp_received(buf, len);
        }
    }
}

void (*__stomp_sent)(char*, int);

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