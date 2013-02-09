#include "stomp-network.h"
#include "stomp-strings.h"

#include "stomp-tools.h"

#include "contiki.h"
#include "contiki-net.h"

#include "uip-debug.h"

#include <string.h>

struct ultra_simple_stomp state;

uip_ipaddr_t __ipaddr;
int __port;

PROCESS(stomp_network_process, "STOMP network process");

static void
__connect(uip_ipaddr_t *ipaddr, int port) {
#if WITH_UDP > 0
    state.conn = udp_new(ipaddr, UIP_HTONS(port), &state);
#else
    state.conn = tcp_connect(ipaddr, UIP_HTONS(port), &state);
#endif

    if (state.conn == NULL) {
        return;
    }

#if WITH_UDP > 0
    udp_bind(state.conn, UIP_HTONS(port));
#endif

    state.addr = ipaddr;
    state.port = port;
    state.flags = 0;
    state.len = 0;
    memset(state.buf, 0, STOMP_NETWORK_BUF_SIZE);
    state.off = 0;
    state.sentlen = 0;
}

static void
__sent() {
    stomp_network_sent(state.buf, state.len);
    state.len = 0;
    memset(state.buf, 0, STOMP_NETWORK_BUF_SIZE);
    state.off = 0;
    state.sentlen = 0;
}

static void
__send() {
    if (state.len == 0) {
        return;
    }

#if WITH_UDP < 1
    if (state.len > uip_mss()) {
        state.sentlen = uip_mss();
    } else {
        state.sentlen = state.len;
    }
#endif

#if WITH_UDP > 0
    uip_udp_packet_sendto(state.conn, state.buf + state.off, state.len, state.addr, UIP_HTONS(state.port));
#else
    uip_send(state.buf + state.off, state.sentlen);
#endif
}

#if WITH_UDP < 1

static void
__acked() {
    state.len -= state.sentlen;
    if (state.len == 0) {
        __sent();
    } else {
        state.off += state.sentlen;
    }
    state.sentlen = 0;
}
#endif

PROCESS_THREAD(stomp_network_process, ev, data) {
#if WITH_UDP > 0
    static struct etimer et;
#endif

    PROCESS_BEGIN();

#if STOMP_DEBUG > 3
    PRINTA("Waiting for network settings\n");
#endif
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_CONTINUE);
    __connect(&__ipaddr, __port);

#if WITH_UDP > 0
    etimer_set(&et, CLOCK_CONF_SECOND * 15);
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);
    stomp_network_connected();
#endif

    while (1) {
        PROCESS_WAIT_EVENT();
#if WITH_UDP > 0
        if (ev == tcpip_event) {
            if (uip_newdata()) {
                if (uip_datalen() == 1 && *((char*) uip_appdata) == '\r') {
                    __sent();
                } else {
                    char *str = (char*) uip_appdata;
                    str[uip_datalen()] = '\0';
                    stomp_network_received(str, uip_datalen());
                }
            }
        } else if (state.len != 0) {
            __send();
        }
#else
        if (uip_connected()) {
            state.flags = 0;
            stomp_network_connected();
            __send();
            continue;
        }
        if (uip_closed()) {
            stomp_network_closed();
        }
        if (uip_aborted()) {
            stomp_network_aborted();
        }
        if (uip_timedout()) {
            stomp_network_timedout();
        }
        if (state.flags & STOMP_FLAG_DISCONNECT) {
            uip_close();
            continue;
        }
        if (state.flags & STOMP_FLAG_ABORT) {
            uip_abort();
            continue;
        }
        if (uip_acked()) {
            __acked();
        }
        if (uip_newdata()) {
            stomp_network_received((char*) uip_appdata, uip_datalen());
        }
        if (uip_rexmit() || uip_newdata() || uip_acked()) {
            __send();

        } else if (uip_poll()) {
            __send();
        }
#endif
    }
    PROCESS_END();
}

void
stomp_network_connect(uip_ipaddr_t *ipaddr, int port) {
    memcpy(&__ipaddr, ipaddr, sizeof (uip_ipaddr_t));
    __port = port;
    process_post(&stomp_network_process, PROCESS_EVENT_CONTINUE, NULL);
}

void
stomp_network_send(char *buf, int len) {
    memcpy(state.buf, buf, len);
    state.len = len;
#if WITH_UDP > 0
    process_post(&stomp_network_process, PROCESS_EVENT_CONTINUE, NULL);
#endif
}

#if WITH_UDP < 1

void
stomp_network_timedout() {
#if STOMP_DEBUG > 3
    PRINTA("Timedout.\n");
#endif
}

void
stomp_network_abort() {
    state.flags = STOMP_FLAG_ABORT;
}

void
stomp_network_aborted() {
#if STOMP_DEBUG > 3
    PRINTA("Aborted.\n");
#endif
}

void
stomp_network_close() {
    state.flags = STOMP_FLAG_DISCONNECT;
}

void
stomp_network_closed() {
#if STOMP_DEBUG > 3
    PRINTA("Closed.\n");
#endif
}
#endif