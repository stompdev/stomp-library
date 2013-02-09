#include "stomp-sensor.h"

int stomp_sensor_const(int min, int max, int delta, int step, int value) {
    return value;
}

int stomp_sensor_random(int min, int max, int delta, int step, int value) {
    if (min < max) {
        return (rand() % ((max - min) / step)) * step + min;
    } else {
        return (rand() % ((min - max) / step)) * step + max;
    }
}

int stomp_sensor_random_delta(int min, int max, int delta, int step, int value) {
    if ((rand() % 50) > 25) {
        value = value + (rand() % (delta / step)) * step;
    } else {
        value = value - (rand() % (delta / step)) * step;
    }
    if (min < max) {
        if (value < min) {
            value = min;
        } else if (value > max) {
            value = max;
        }
    } else {
        if (value < max) {
            value = max;
        } else {
            value = min;
        }
    }
    return value;
}