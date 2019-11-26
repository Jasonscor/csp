#ifndef UNIT_TEST_H
#define UNIT_TEST_H

#include "define.h"
#include "cmn_hdr.h"
#include "csp_thread.h"
#include "csp_mutex.h"
#include "csp_shm.h"
#include "csp_cond.h"
#include "csp_threadpool.h"
#include "csp_singleton.h"
#include "csp_common.h"

/* 全局变量 */

/* 互斥量 */

typedef struct _msg_t
{
    int         task_id;
    std::string task_name;
}msg_t;


class mytask : public csp_utils_task
{
public:
    mytask() = default;
    ~mytask() = default;

    void run()
    {
        msg_t* msg = (msg_t*)_data;
        std::cout << "working thread[" << pthread_self() << "] << consume : task_id  " \
                  << msg->task_id << " task_name  " << msg->task_name << std::endl;
        std::cout << std::endl;
        sleep(10);
    }
};

extern int _num;
extern pthread_mutex_t _mutex;
extern void* pthread_func_1(void* args);
extern void* pthread_func_2(void* args);
extern void* pthread_func_cond_1(void* args);
extern void* pthread_func_cond_2(void* args);
extern void* thread_func_cond_01(void* args);
extern void* thread_func_cond_02(void* args);


class UNIT_TEST
{
public:
    UNIT_TEST();
    ~UNIT_TEST();

public:
    void execute();

public:
    void test_thread();

public:
    void test_mutex();

public:
    void test_read_shm_001();
    void test_write_shm(char* str);

public:
    void test_cond();
    void test_cond_01();

public:
    void test_threadpool();

public:
    void test_singleton();

private:
    csp_utils_thread m_thrd1;
    csp_utils_thread m_thrd2;

};



#endif // UNIT_TEST_H
