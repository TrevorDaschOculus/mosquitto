#ifndef DUMMYTHREAD_H
#define DUMMYTHREAD_H

#define mosquitto_thread__create(A, B, C)
#define mosquitto_thread__join(A)
#define mosquitto_thread__cancel(A)
#define mosquitto_thread__testcancel()

#define mosquitto_mutex__init(A)
#define mosquitto_mutex__destroy(A)
#define mosquitto_mutex__lock(A)
#define mosquitto_mutex__unlock(A)

#endif
