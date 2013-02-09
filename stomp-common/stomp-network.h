#ifndef STOMP_NETWORK_H
#define	STOMP_NETWORK_H

#include "contiki.h"
#include "contiki-net.h"

#define STOMP_FLAG_DISCONNECT 1
#define STOMP_FLAG_ABORT 2

#define STOMP_NETWORK_BUF_SIZE 512

struct ultra_simple_stomp {
#if WITH_UDP > 0
    struct uip_udp_conn *conn;
#else
    struct uip_conn *conn;
#endif

    uip_ipaddr_t *addr;
    int port;
    char flags;

    char buf[STOMP_NETWORK_BUF_SIZE];
    int off;
    int len;
    int sentlen;
};

extern struct ultra_simple_stomp state;

PROCESS_NAME(stomp_network_process);

void
stomp_network_connect(uip_ipaddr_t *ipaddr, int port);

#define STOMP_NETWORK_CONNECT(ipaddr, port) \
        stomp_network_connect(ipaddr, port); \
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_CONTINUE);

void
stomp_network_connected();

#if WITH_UDP > 0
void
stomp_net_timedout();

void
stomp_net_abort();

void
stomp_net_aborted();

void
stomp_net_close();

void
stomp_net_closed();
#endif

void
stomp_network_send(char *buf, int len);

void
stomp_network_sent(char *buf, int len);

void
stomp_network_received(char *buf, int len);

#endif	/* STOMP_NETWORK_H */