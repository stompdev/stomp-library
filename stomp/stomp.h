#include "contiki.h"
#include "contiki-net.h"

#ifndef STOMP_H
#define STOMP_H

extern const char stomp_version_default[4];
extern const char stomp_content_type_default[11];

void
stomp_connect(char *host, char* login, char* pass);

#define STOMP_CONNECT(host, login, pass) \
        stomp_connect(host, login, pass); \
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_CONTINUE);

void
stomp_subscribe(char *id, char *destination, char *ack);

#define STOMP_SUBSCRIBE(id, destination, ack) \
        stomp_subscribe(id, destination, ack); \
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_CONTINUE);

void
stomp_unsubscribe(char *id);

#define STOMP_UNSUBSCRIBE(id) \
        stomp_unsubscribe(id); \
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_CONTINUE);

void
stomp_send(char *destination, char *type, char *length, char *receipt, char *tx, char *message);

#define STOMP_SEND(destination, type, length, receipt, tx, message) \
        stomp_send(destination, type, length, receipt, tx, message); \
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_CONTINUE);

void
stomp_ack(char *subscription, char *message_id, char *tx);

#define STOMP_ACK(subscription, message_id, tx) \
        stomp_ack(subscription, message_id, tx); \
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_CONTINUE);

void
stomp_nack(char *subscription, char *message_id, char *tx);

#define STOMP_NACK(subscription, message_id, tx) \
        stomp_nack(subscription, message_id, tx); \
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_CONTINUE);

void
stomp_begin(char *tx);

#define STOMP_BEGIN(tx) \
        stomp_begin(tx); \
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_CONTINUE);

void
stomp_commit(char *tx);

#define STOMP_COMMIT(tx) \
        stomp_commit(tx); \
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_CONTINUE);

void
stomp_abort(char *tx);

#define STOMP_ABORT(tx) \
        stomp_abort(tx); \
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_CONTINUE);

void
stomp_disconnect(char *receipt);

#define STOMP_DISCONNECT(receipt) \
        stomp_disconnect(receipt); \
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_CONTINUE);

extern void (*__stomp_sent)(char*, int);
void
stomp_sent(char *buf, int len);
#define STOMP_REGISTER_STOMP_SENT(func) __stomp_sent = func

extern void (*__stomp_received)(char*, int);
void
stomp_received(char *buf, int len);
#define STOMP_REGISTER_STOMP_RECEIVED(func) __stomp_received = func

extern void (*__stomp_connected)(char*, char*, char*, char*, char*, char*);
void
stomp_connected(char *version, char *server, char *host_id, char *session, char *heart_beat, char *user_id);
#define STOMP_REGISTER_STOMP_CONNECTED(func) __stomp_connected = func

extern void (*__stomp_message)(char*, char*, char*, char*, char*, char*);
void
stomp_message(char *destination, char *message_id, char *subscription, char *content_type, char *content_length, char *message);
#define STOMP_REGISTER_STOMP_MESSAGE(func) __stomp_message = func

extern void (*__stomp_error)(char*, char*, char*, char*);
void
stomp_error(char *receipt_id, char *content_type, char *content_length, char *message);
#define STOMP_REGISTER_STOMP_ERROR(func) __stomp_error = func

extern void (*__stomp_receipt)(char*);
void
stomp_receipt(char *receipt_id);
#define STOMP_REGISTER_STOMP_RECEIPT(func) __stomp_receipt = func

#endif /* STOMP_H */
