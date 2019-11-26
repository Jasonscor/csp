#include "csp_cond.h"

namespace csp {
    namespace utils {
        cond::cond()
        {
            init();
        }

        cond::~cond()
        {
            destroy();
        }


        /**
         * @brief init pthread condition
         */
        void cond::init()
        {
            pthread_cond_init(&_cond, NULL);
            pthread_mutex_init(&_mutex, NULL);
        }


        /**
         * @brief wait pthread condition signal
         */
        void cond::wait()
        {
            pthread_cond_wait(&_cond, &_mutex);
        }

        /**
         * @brief wait for the specified time or recv condition signal
         */
        void cond::time_wait(struct timespec wait_time)
        {
            pthread_cond_timedwait(&_cond, &_mutex, &wait_time);
        }

        /**
         * @brief sending pthread condition signal
         */
        void cond::notify()
        {
            pthread_cond_signal(&_cond);
        }


        /**
         * @brief broadcast sending pthread condition signal
         */
        void cond::notify_all()
        {
            pthread_cond_broadcast(&_cond);
        }


        /**
         * @brief cond lock thread
         */
        void cond::lock()
        {
            pthread_mutex_lock(&_mutex);
        }


        /**
         * @brief cond unlock thread
         */
        void cond::unlock()
        {
            pthread_mutex_unlock(&_mutex);
        }


        /**
         * @brief destroy pthread condition
         */
        void cond::destroy()
        {
            pthread_cond_destroy(&_cond);
            pthread_mutex_destroy(&_mutex);
        }


        /**
         * @brief get pthread cond ptr
         */
        pthread_cond_t* cond::getCondPtr()
        {
            return &_cond;
        }


        /**
         * @brief get pthread cond mutex ptr
         */
        pthread_mutex_t cond::getCondMutex()
        {
            return _mutex;
        }
    }
}
