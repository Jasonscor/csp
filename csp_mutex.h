/**
* Copyright (C) - All Rights Reserved.
*
* File Name: csp_thread.h
* Summary  : Define the crowded sourcing platform mutex utils.
*            based on posix mutex
*
* Version  : 1.0
* Author   : weixun
* Date     : 2019.10.27
**/

#ifndef CSP_MUTEX_H
#define CSP_MUTEX_H

#include "cmn_hdr.h"
#include "define.h"

namespace csp{
    namespace utils{
        /************************************************
         *    Class mutex                               *
         ************************************************/
        class mutex : public csp_copyable
        {
        public:
            /**
             * @brief constructor default
             */
            mutex();

            /**
             * @brief destructor default
             */
            ~mutex();

        /************************************************
         *    External InterFace                        *
         ************************************************/
        public:
            /**
             * @brief init thread mutex lock
             */
            void init();

            /**
             * @brief mutex lock thread
             */
            void lock();

            /**
             * @brief mutex unlock thread
             */
            void unlock();

            /**
             * @brief mutex try lock thread
             */
            int trylock();

            /**
             * @brief init thread mutex lock
             */
            void destroy();

        /************************************************
         *    Internal InterFace                        *
         ************************************************/
        private:
            /**
             * @brief get pthread mutex ptr
             */
            pthread_mutex_t* getMutexPtr();

        private:
            pthread_mutex_t _mutex_lock = PTHREAD_MUTEX_INITIALIZER;
        };


        /************************************************
         *    Class mutex_guard                         *
         ************************************************/
        class mutex_guard : public csp_noncopyable
        {
        public:
            mutex_guard(mutex& mutex_lock_guard);
            ~mutex_guard();

        private:
            mutex& _mutex_lock_gurad;

        };
    }
}

typedef csp::utils::mutex       csp_utils_mutex;
typedef csp::utils::mutex_guard csp_utils_mutex_guard;

#endif // CSP_MUTEX_H
