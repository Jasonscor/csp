/**
* Copyright (C) - All Rights Reserved.
*
* File Name: csp_cond.h
* Summary  : Define the crowded sourcing platform condition utils.
*            based on posix condition
*
* Version  : 1.0
* Author   : weixun
* Date     : 2019.10.27
**/

#ifndef CSP_COND_H
#define CSP_COND_H

#include "cmn_hdr.h"
#include "define.h"
#include "csp_mutex.h"

namespace csp {
    namespace utils {
        /************************************************
         *    Class cond                                *
         ************************************************/
        class cond : public csp_copyable
        {
        public:
            /**
             * @brief constructor default
             */
            cond();

            /**
             * @brief destructor default
             */
            ~cond();

        /************************************************
         *    External InterFace                        *
         ************************************************/
        public:
            /**
             * @brief init pthread condition
             */
            void init();

            /**
             * @brief wait pthread condition signal
             */
            void wait();

            /**
             * @brief wait for the specified time or recv condition signal
             */
            void time_wait(struct timespec wait_time);

            /**
             * @brief sending pthread condition signal
             */
            void notify();

            /**
             * @brief broadcast sending pthread condition signal
             */
            void notify_all();

            /**
             * @brief cond lock thread
             */
            void lock();

            /**
             * @brief cond unlock thread
             */
            void unlock();

            /**
             * @brief destroy pthread condition
             */
            void destroy();

        /************************************************
         *    External InterFace                        *
         ************************************************/
        private:
            /**
             * @brief get pthread cond ptr
             */
            pthread_cond_t* getCondPtr();

            /**
             * @brief get pthread cond mutex ptr
             */
            pthread_mutex_t getCondMutex();

        private:
            pthread_mutex_t _mutex = PTHREAD_MUTEX_INITIALIZER;
            pthread_cond_t  _cond  = PTHREAD_COND_INITIALIZER;
        };
    }
}

typedef csp::utils::cond  csp_utils_cond;

#endif // CSP_COND_H
