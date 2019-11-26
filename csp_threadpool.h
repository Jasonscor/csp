/**
* Copyright (C) - All Rights Reserved.
*
* File Name: csp_threadpool.h
* Summary  : Define the crowded sourcing platform threadpool utils.
*            which based on posix thread pool
*
* Version  : 1.1
* Author   : weixun
* Date     : 2019.10.27
**/

#ifndef CSP_threadpool_H
#define CSP_threadpool_H

#include "cmn_hdr.h"
#include "define.h"
#include "csp_thread.h"
#include "csp_cond.h"

namespace csp {
    namespace utils {
        /************************************************
         *    class task                                *
         ************************************************/
        class task
        {
        public:
            task(void* data = NULL);
            ~task() = default;

        public:
            void setData(void* data);
            void* getData();
            virtual void run() = 0;     // must be override

        protected:
            void* _data;
        };


        /************************************************
         *    class threadpool                          *
         ************************************************/
        class threadpool : public noncopyable
        {
        public:
            typedef std::queue<task*>  task_queue;

        /************************************************
         *    External InterFace                        *
         ************************************************/
        public:
            /**
             * @brief create threadpool using singleton pattern
             */
            static threadpool* createThreadPool(int thread_num);

            /**
             * @brief destructor
             */
            ~threadpool();

            /**
             * @brief add task to task queue
             */
            void addTask(task* new_task);

            /**
             * @brief get task container size
             */
            int getTaskSize();

            /**
             * @brief get threadpool threads number
             */
            int getThreadNum();


        /************************************************
         *    Internal InterFace                        *
         ************************************************/
        private:
            /**
             * @brief constructor with default arguments
             */
            threadpool(int16_t thread_num = 5);

            /**
             * @brief create threads array
             */
            void createThreadArray();

            /**
             * @brief working thread get task from task queue
             */
            task* takeTask();

            /**
             * @brief clear threads array
             */
            void clearThreadArray();

            /**
             * @brief clear task queue
             */
            void clearTaskQueue();

        protected:
            static void* thread_func(void* arg);

        private:
            int16_t         _thread_num = 0;      // number of threads started in the threadpool
            pthread_t*      _thread_set = NULL;   // threads array
            bool            _isrunning = false;   // thread running flag
            task_queue      _queue;               // task queue
            csp_utils_cond  _cond;                // mutex & condition
        };
    }
}

typedef csp::utils::task                    csp_utils_task;
typedef csp::utils::threadpool              csp_utils_threadpool;
typedef csp::utils::threadpool::task_queue  csp_task_queue;

#endif // CSP_threadpool_H
