#include "csp_thread.h"

namespace csp {
    namespace utils {

        /**
         * @brief constructor
         */
        thread::thread():_isrunning(false)
        {

        }


        /**
         * @brief constructor
         * @param THREAD_FUNC_T threadfunc
         *        void* args
         *        std::string name
         */
        thread::thread(THREAD_FUNC_T threadfunc, void* args, std::string name):\
                        _threadfunc(threadfunc), _args(args), _name(name), _isrunning(false)
        {

        }

        /**
         * @brief destructor
         */
        thread::~thread()
        {

        }


        /**
         * @brief create posix thread
         * @param null
         * @return true: succeed
         *         false: failed
         */
        bool thread::create()
        {
            bool rc = false;

            if(_isrunning)
            {
                // log_output [thread is running, can't create again]
                std::cout << "[csp::utils::thread::create] failed! " << std::endl;
                return rc;
            }

            if(0 == pthread_create(&_tid, NULL, _threadfunc, NULL))
            {
                _isrunning = true;
                rc = true;
            }

            return rc;
        }


        /**
         * @brief create posix thread
         * @param THREAD_FUNC_T threadfunc
         * @return true: succeed
         *         false: failed
         */
        bool thread::create(THREAD_FUNC_T threadfunc)
        {
            bool rc = false;
            _threadfunc = threadfunc;

            if(_isrunning)
            {
                // log_output [thread is running, can't create again]
                std::cout << "[csp::utils::thread::create] failed! " << std::endl;
                return rc;
            }

            if(0 == pthread_create(&_tid, NULL, _threadfunc, NULL))
            {
                _isrunning = true;
                rc = true;
            }

            return rc;
        }


        /**
         * @brief create posix thread
         * @param thread_task threadtask
         * @return true: succeed
         *         false: failed
         */
        bool thread::create(thread_task threadtask)
        {
            bool rc = false;
            _threadfunc = threadtask._thread_func;
            _args = threadtask._thread_args;

            if(_isrunning)
            {
                // log_output [thread is running, can't create again]
                std::cout << "[csp::utils::thread::create] failed! " << std::endl;
                return rc;
            }

            if(0 == pthread_create(&_tid, NULL, _threadfunc, _args))
            {
                _isrunning = true;
                rc = true;
            }

            return rc;
        }

        /**
         * @brief block parent thread until current thread finished
         * @param null
         * @return true: succeed
         *         false: failed
         */
        bool thread::join()
        {
            bool rc = false;
            if(!_isrunning)
            {
                /* log_output [thread not create, can't join] */
                std::cout << "[csp::utils::thread::join]->[pthread_create] failed! " << std::endl;
                return rc;
            }

            if(0 != pthread_join(_tid, NULL))
            {
                /* when pthread_join return 0, means current thread is done */
                std::cout << "[csp::utils::thread::join]->[pthread_join] failed! " << std::endl;
                _isrunning = false;
                rc = true;
            }

            return rc;
        }


        /**
         * @brief register thread function
         * @param THREAD_FUNC_T threadfunc
         */
        void thread::registThreadFunc(THREAD_FUNC_T threadfunc)
        {
            this->_threadfunc = threadfunc;
        }


        /**
         * @brief get the status of the thread running
         * @param null
         * @return true: thread is running
         *         false: thread is not running
         */
        bool thread::running()
        {
            return _isrunning;
        }


        /**
         * @brief get thread id
         * @param null
         * @return pthread_t
         */
        pthread_t thread::get_tid()
        {
            return _tid;
        }


        void thread::run()
        {
            if(!create())
            {
                /* log_output [thread not create] */
                std::cout << "[csp::utils::thread::run]->[create] failed! " << std::endl;
            }

            if(join())
            {
                /* log_output [thread join failed] */
                std::cout << "[csp::utils::thread::run]->[join] failed! " << std::endl;
            }
        }


        /**
         * @brief get thread status
         * @param null
         * @return thread running status
         */
        bool thread::get_status()
        {
            return _isrunning;
        }


        /**
         * @brief get thread status
         * @param bool isrunning
         * @return null
         */
        void thread::set_status(bool isrunning)
        {
            _isrunning = isrunning;
        }


        /**
         * @brief exit thread
         * @param null
         * @return null
         */
        void thread::exit()
        {
            pthread_exit(NULL);
        }


        void sleep_s(int32_t s)
        {
            sleep(s);
        }


        void sleep_ms(int32_t ms)
        {
            usleep(ms);
        }
    }
}


