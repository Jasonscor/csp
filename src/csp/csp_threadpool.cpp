#include "csp_threadpool.h"

namespace csp {
    namespace utils {

        /************************************************
         *    class task                                *
         ************************************************/
        task::task(void* data) : _data(data) {}
        void  task::setData(void* data) {_data = data;}
        void* task::getData() {return _data;}


        /************************************************
         *    class threadpool                         *
         ************************************************/
        /**
         * @brief constructor create threads queue
         * @param int16_t thread_num
         */
        threadpool::threadpool(int16_t thread_num) : _thread_num(thread_num)
        {
            std::cout << thread_num << " threads created! " << std::endl;

            _isrunning = true;

            /* create threads array */
            createThreadArray();
        }

        /**
         * @brief destructor default
         */
        threadpool::~threadpool()
        {
            clearThreadArray();
            clearTaskQueue();
            std::cout << "[threadpool::~threadpool] threadpool is closed!" << std::endl;
        }


        /**
         * @brief  create threadpool using singleton pattern
         * @param  thread number
         * @return threadpool instance
         */
        threadpool* threadpool::createThreadPool(int thread_num)
        {
            static threadpool* pthread_pool_instance = new threadpool(thread_num);
            return pthread_pool_instance;
        }


        /**
         * @brief create threads array
         */
        void threadpool::createThreadArray()
        {
            _thread_set = (pthread_t*)malloc(sizeof(pthread_t) * _thread_num);

            for(int i = 0; i < _thread_num; i++)
            {
                pthread_create(&_thread_set[i], NULL, thread_func, this);
                csp::utils::sleep_ms(10);
            }
        }


        /**
         * @brief threadpool thread function
         * @param void* arg
         * @return void*
         */
        void* threadpool::thread_func(void* arg)
        {
            threadpool* pool = (threadpool*)arg;
            while(pool->_isrunning)
            {
                task* take_task = pool->takeTask();

                /*if task is null, then end this thread */
                if(!take_task)
                {
                    break;
                }

                std::cout << "[threadpool::thread_func]->[take task]" << std::endl;

                /* execute derived class run function */
                take_task->run();
            }

            return NULL;
        }


        /**
         * @brief add task to task list then send notification to task lisst
         * @param task* new_task
         */
        void threadpool::addTask(task* new_task)
        {
            _cond.lock();
            _queue.push(new_task);
            std::cout << "one task is put into queue! Current queue size is " << _queue.size() << std::endl;
            _cond.unlock();
            _cond.notify();
        }


        /**
         * @brief get task from task queue
         * @return task* task taken from queue
         */
        task* threadpool::takeTask()
        {
            task* take_out_task = NULL;
            while(!take_out_task)
            {
                _cond.lock();

                /* thread pool is running ok, but task queue is empty */
                while(_queue.empty() && _isrunning)
                {
                    _cond.wait();
                }

                if(!_isrunning)
                {
                    _cond.wait();
                    break;
                }
                else if(_queue.empty())
                {
                    _cond.unlock();
                    continue;
                }

                take_out_task = _queue.front();
                _queue.pop();
                _cond.unlock();
            }

            return take_out_task;
        }


        /**
         * @brief clear task vector
         */
        void threadpool::clearTaskQueue()
        {
            std::queue<task*> empty;
            std::swap(empty, _queue);
        }


        /**
         * @brief clear threads array when close threadpool
         */
        void threadpool::clearThreadArray()
        {
            if(!_isrunning)
            {
                return;
            }

            /* wake up all waiting threads */
            _isrunning = false;
            _cond.notify_all();
            std::cout << "[threadpool::clearThreadArray] start closing thread!" << std::endl;

            /* waiting for all threads exit */
            for(int i = 0; i < _thread_num; i++)
            {
                pthread_join(_thread_set[i], NULL);
                std::cout << "close thread " << _thread_set[i] << std::endl;
            }

            /* delete thread resources */
            if(_thread_set)
            {
                delete[] _thread_set;
                _thread_set = NULL;
            }

            /* destroy condition resources */
            _cond.destroy();
        }


        /**
         * @brief get task size
         * @return  task list size
         */
        int threadpool::getTaskSize()
        {
            int size = 0;
            _cond.lock();
            size = _queue.size();
            _cond.unlock();
            std::cout << "current queue size is " << size << std::endl;
            return size;
        }


        /**
         * @brief get threadpool threads number
         */
        int threadpool::getThreadNum()
        {
            return _thread_num;
        }

    }
}
