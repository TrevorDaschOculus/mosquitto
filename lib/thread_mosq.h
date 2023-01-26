#ifndef THREAD_MOSQ_H
#define THREAD_MOSQ_H

#if defined(WITH_THREADING)

#  ifdef WIN32
#    include <winsock2.h>
#    define mosquitto_mutex_t CRITICAL_SECTION
#    define mosquitto_thread_t HANDLE
#    define mosquitto_thread_result_t DWORD
#    define get_mosquitto_thread_result(arg) (arg != NULL)

#    define mosquitto_thread__create(thread, start_routine, arg) (!thread || !(*thread = CreateThread(NULL, 0, start_routine, arg, 0, NULL)))
#    define mosquitto_thread__join(thread) WaitForSingleObject(thread, INFINITE)
#    define mosquitto_thread__cancel(thread)
#    define mosquitto_thread__testcancel()

#    define mosquitto_thread__self() GetCurrentThread()
#    define mosquitto_thread__equal(a, b) CompareObjectHandles(a, b)

#    define mosquitto_thread__set_name(thread, name) SetThreadDescription(thread, L##name);

#    define mosquitto_mutex__init(lock) (void)InitializeCriticalSectionAndSpinCount(lock, 0x400)
#    define mosquitto_mutex__destroy(lock) DeleteCriticalSection(lock)
#    define mosquitto_mutex__lock(lock) EnterCriticalSection(lock)
#    define mosquitto_mutex__unlock(lock) LeaveCriticalSection(lock)

#  else
#    if defined(__linux__) || defined(__NetBSD__)
#      include <pthread.h>
#    elif defined(__FreeBSD__) || defined(__OpenBSD__)
#      include <pthread_np.h>
#    endif
#    define mosquitto_mutex_t pthread_mutex_t
#    define mosquitto_thread_t pthread_t
#    define mosquitto_thread_result_t void*
#    define get_mosquitto_thread_result(arg) arg

#    define mosquitto_thread__create(thread, start_routine, arg) pthread_create(thread, NULL, start_routine, arg)
#    define mosquitto_thread__join(thread) pthread_join(thread, NULL)
/* Android Bionic libpthread implementation doesn't have pthread_cancel */
#    ifndef ANDROID
#      define HAVE_THREAD_CANCEL
#      define mosquitto_thread__cancel(thread) pthread_cancel(thread)
#      define mosquitto_thread__testcancel() pthread_testcancel()
#    else
#      define mosquitto_thread__cancel(thread) 
#      define mosquitto_thread__testcancel()
#    endif

#    define mosquitto_thread__self() pthread_self()
#    define mosquitto_thread__equal(a, b) pthread_equal(a, b)

#    define mosquitto_mutex__init(lock) (void)pthread_mutex_init(lock, NULL)
#    define mosquitto_mutex__destroy(lock) (void)pthread_mutex_destroy(lock)
#    define mosquitto_mutex__lock(lock) (void)pthread_mutex_lock(lock)
#    define mosquitto_mutex__unlock(lock) (void)pthread_mutex_unlock(lock)

#    if defined(__linux__)
#      define mosquitto_thread__set_name(thread, name) pthread_setname_np(thread, name)
#    elif defined(__NetBSD__)
#      define mosquitto_thread__set_name(thread, name) pthread_setname_np(thread, "%s", name)
#    elif defined(__FreeBSD__) || defined(__OpenBSD__)
#      define mosquitto_thread__set_name(thread, name) pthread_set_name_np(thread, name)
#    else
#      define mosquitto_thread__set_name(thread, name)
#    endif

#  endif
#else
// dummy methods
#  define mosquitto_thread__create(thread, start_routine, arg)
#  define mosquitto_thread__join(thread)
#  define mosquitto_thread__cancel(thread)
#  define mosquitto_thread__testcancel()
#  define mosquitto_thread__set_name(thread, name)

#  define mosquitto_mutex__init(lock)
#  define mosquitto_mutex__destroy(lock)
#  define mosquitto_mutex__lock(lock)
#  define mosquitto_mutex__unlock(lock)

#endif

#endif
