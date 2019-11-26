#include "csp_mutex.h"

namespace csp {
    namespace utils {

        /************************************************
         *    class mutex                               *
         ************************************************/
        /**
         * @brief constructor
         */
        mutex::mutex()
        {
            init();
        }

        /**
         * @brief destructor
         */
        mutex::~mutex()
        {
            destroy();
        }

        /**
         * @brief init pthread mutex
         */
        void mutex::init()
        {
            pthread_mutex_init(&_mutex_lock, NULL);
        }


        /**
         * @brief pthread mutex lock
         */
        void mutex::lock()
        {
            pthread_mutex_lock(&_mutex_lock);
        }


        /**
         * @brief pthread mutex unlock
         */
        void mutex::unlock()
        {
            pthread_mutex_unlock(&_mutex_lock);
        }


        /**
         * @brief try get mutex
         * @return 0: get locked succeed
         *         other: get lock failed
         */
        int mutex::trylock()
        {
            pthread_mutex_trylock(&_mutex_lock);
        }


        /**
         * @brief pthread mutex destroy
         */
        void mutex::destroy()
        {
            pthread_mutex_destroy(&_mutex_lock);
        }


        /**
         * @brief get pthread mutex ptr
         */
        pthread_mutex_t* mutex::getMutexPtr()
        {
            return &_mutex_lock;
        }


        /************************************************
         *    class mutex_guard                         *
         ************************************************/
        mutex_guard::mutex_guard(mutex& mutex_lock_guard):_mutex_lock_gurad(mutex_lock_guard)
        {
            _mutex_lock_gurad.lock();
        }

        mutex_guard::~mutex_guard()
        {
            _mutex_lock_gurad.unlock();
        }
    }
}
