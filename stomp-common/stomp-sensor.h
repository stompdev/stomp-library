#include "contiki.h"

#ifndef STOMP_SENSOR_H
#define	STOMP_SENSOR_H

#define STOMP_SENSOR_NO_UPDATE                          0b00
#define STOMP_SENSOR_UPDATE_PERIODICALLY                0b01
#define STOMP_SENSOR_UPDATE_ON_CHANGE                   0b10
#define STOMP_SENSOR_UPDATE_PERIODICALLY_ON_CHANGE      0b11

#define STOMP_SENSOR_PERIODIC_15s       0b100000
#define STOMP_SENSOR_PERIODIC_30s       0b010000
#define STOMP_SENSOR_PERIODIC_1m        0b001000
#define STOMP_SENSOR_PERIODIC_2m        0b000100
#define STOMP_SENSOR_PERIODIC_5m        0b000010
#define STOMP_SENSOR_PERIODIC_10m       0b000001
#define STOMP_SENSOR_PERIODIC_NONE      0b000000

struct stomp_sensor {
    char *name;
    int update;
    int periodic;
    int last;
    int value;
};

#define STOMP_SENSOR(PROCESS_NAME, FREQ, NEXT_VALUE, INIT, MIN, MAX, DELTA, STEP, PERIODIC, UPDATE) \
static struct stomp_sensor PROCESS_NAME##_data; \
PROCESS(PROCESS_NAME, "STOMP sensor " #PROCESS_NAME); \
PROCESS_THREAD(PROCESS_NAME, ev, data) { \
    static struct etimer et; \
    PROCESS_BEGIN(); \
    PROCESS_NAME##_data.value = INIT; \
    PROCESS_NAME##_data.name = #PROCESS_NAME; \
    PROCESS_NAME##_data.periodic = PERIODIC; \
    PROCESS_NAME##_data.update = UPDATE; \
    etimer_set(&et, CLOCK_CONF_SECOND * FREQ); \
    while (1) { \
        PROCESS_WAIT_EVENT(); \
        PROCESS_NAME##_data.last = PROCESS_NAME##_data.value; \
        PROCESS_NAME##_data.value = NEXT_VALUE(MIN, MAX, DELTA, STEP, PROCESS_NAME##_data.value); \
        etimer_restart(&et); \
    } \
    PROCESS_END(); \
}

extern struct stomp_sensor * stomp_sensor_processes[];

#if ! CC_NO_VA_ARGS
#define STOMP_SENSOR_PROCESSES(...) \
struct stomp_sensor * stomp_sensor_processes[] = {__VA_ARGS__, NULL}
#else
#error "C compiler must support __VA_ARGS__ macro"
#endif

int stomp_sensor_const(int min, int max, int delta, int step, int value);

int stomp_sensor_random(int min, int max, int delta, int step, int value);

int stomp_sensor_random_delta(int min, int max, int delta, int step, int value);

#endif	/* STOMP_SENSOR_H */

