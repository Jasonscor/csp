/**
* Copyright (C) - All Rights Reserved.
*
* File Name: csp_singleton.h
* Summary  : Define the crowded sourcing platform singleton template utils.
*
* Version  : 1.0
* Author   : weixun
* Date     : 2019.10.27
**/


#ifndef CSP_SINGLETON_H
#define CSP_SINGLETON_H

#include "cmn_hdr.h"

namespace csp {
    namespace utils {

        template <typename T>
        class singleton
        {
        public:
            static T& getInstance()
            {
                init();
                return *_instance;
            }

        private:
            static void init()
            {
                if(NULL == _instance)
                {
                    pthread_mutex_lock(&_mutex);
                    if(NULL == _instance)
                    {
                        _instance = new T;
                        ::atexit(destroy);
                    }
                    pthread_mutex_unlock(&_mutex);
                }
            }

            static void destroy()
            {
                delete _instance;
                _instance = NULL;
            }

        private:
            ~singleton();
            singleton();
            singleton(const singleton&) = delete;
            singleton& operator= (const singleton&) = delete;

        private:
            static T* volatile _instance;
            static pthread_mutex_t _mutex;
        };
    }

    template <typename T>
    T* volatile csp::utils::singleton<T>::_instance = NULL;

    template <typename T>
    pthread_mutex_t csp::utils::singleton<T>::_mutex = PTHREAD_MUTEX_INITIALIZER;

}



#endif // CSP_SINGLETON_H
