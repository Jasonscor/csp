/**
* Copyright (C) - All Rights Reserved.
*
* File Name: csp_thread.h
* Summary  : Define the crowded sourcing platform thread utils.
*            based on posix thread(pthread)
*
* Version  : 1.1
* Author   : weixun
* Date     : 2019.10.27
* Std I/F  : pthread_create (create posix thread)
*            pthread_join   (block current posix thread)
*            pthread_exit   (exit posix thread)
**/

#ifndef CSP_THREAD_H
#define CSP_THREAD_H

#include "define.h"
#include "cmn_hdr.h"
#include "csp_mutex.h"

namespace csp {
    namespace utils {
        /************************************************
         *    class thread                              *
         ************************************************/
        class thread : public csp_noncopyable
        {
        public:
            typedef void*(*CALLBACK_T)(void*);
            typedef void*(*THREAD_FUNC_T)(void*);
            typedef void*(*THREAD_FUNC)(void);
            typedef void* THREAD_ARGS_T;

        public:
            /************************************************
             *    struct thread_task                        *
             ************************************************/
            typedef struct thread_task
            {
                THREAD_FUNC_T _thread_func;
                THREAD_ARGS_T _thread_args;
            }thread_task;

        public:
            /**
             * @brief constructor default
             */
            thread();

            /**
             * @brief destructor default
             */
            ~thread();

            /**
             * @brief    constructor create thread
             * @param    THREAD_FUNC_T thread function
             * @param    void*         args, default is NULL
             * @param    std::string   name, default is "default_thread"
             */
            thread(THREAD_FUNC_T threadfunc, void* args = NULL, std::string name = std::string("default_thread"));

        /************************************************
         *    External InterFace                        *
         ************************************************/
        public:
            /**
             * @brief create thread
             * @return true: succeed
             *         false: failed
             */
            bool create();


            /**
             * @brief create thread
             * @param THREAD_FUNC_T threadfunc
             * @return true: succeed
             *         false: failed
             */
            bool create(THREAD_FUNC_T threadfunc);


            /**
             * @brief create thread
             * @param thread_task threadtask
             * @return true: succeed
             *         false: failed
             */
            bool create(thread_task threadtask);


            /**
             * @brief block the calling thread until current thread released
             * @return true: succeed
             *         false: failed
             */
            bool join();


            /**
             * @brief current thread is running
             * @return true: is running
             *         false: is not running
             */
            bool running();


            /**
             * @brief csp_thread class register thread function
             * @param THREAD_FUNC_T
             */
            void registThreadFunc(THREAD_FUNC_T threadfunc);


            /**
             * @brief thread exit
             */
            void exit();


            /**
             * @brief sub class inherit csp_thread class, override run interface
             */
            virtual void run();


            /**
             * @brief get thread id
             */
            pthread_t get_tid();

        /************************************************
         *    Internal InterFace                        *
         ************************************************/
        private:
            /**
             * @brief get thread id
             */
            bool get_status();


            /**
             * @brief get thread id
             */
            void set_status(bool isrunning);

        private:
            THREAD_FUNC_T _threadfunc;
            THREAD_ARGS_T _args;
            pthread_t     _tid;
            bool          _isrunning;
            std::string   _name;
        };

        /**
         * @brief current thread sleep seconds
         * @param s seconds
         */
        void sleep_s(int32_t s);

        /**
         * @brief current thread sleep milliseconds
         * @param ms milliseconds
         */
        void sleep_ms(int32_t ms);
    }
}

typedef csp::utils::thread                  csp_utils_thread;
typedef csp::utils::thread::thread_task     csp_utils_thread_task;
typedef csp::utils::thread::THREAD_FUNC_T   csp_utils_thread_func;
typedef csp::utils::thread::THREAD_ARGS_T   csp_utils_thread_args;


#endif // CSP_THREAD_H
