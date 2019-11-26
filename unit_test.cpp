#include "unit_test.h"

int _num = 0;
pthread_mutex_t _mutex = PTHREAD_MUTEX_INITIALIZER; /* init pthread mutex */
pthread_cond_t _cond = PTHREAD_COND_INITIALIZER;    /* init pthread cond */

int j = 1;
int g_count = 0;
csp_utils_mutex g_mutex;
csp_utils_cond  g_cond;

/*线程1的服务程序*/
void* pthread_func_1(void* args)
{
    for (int i = 0; i < 5; i++)
    {
        /* 获取互斥锁 */
        pthread_mutex_lock(&_mutex);

        /* 注意，这里以防线程的抢占，以造成一个线程在另一个线程sleep时多次访问互斥资源，所以sleep要在得到互斥锁后调用. */
        sleep (1);

        /* 临界资源 */
        _num++;
        std::cout << "Thread_1 add one to num : " << _num << std::endl;

        /* 释放互斥锁. */
        pthread_mutex_unlock(&_mutex);
    }

    pthread_exit(NULL);

    return NULL;
}

/*线程2的服务程序*/
void* pthread_func_2(void* args)
{
    for (int i = 0; i < 5; i++)
    {
        /* 获取互斥锁. */
        pthread_mutex_lock(&_mutex);

        /* 注意，这里以防线程的抢占，以造成一个线程在另一个线程sleep时多次访问互斥资源，所以sleep要在得到互斥锁后调用. */
        sleep(1);

        /* 临界资源. */
        _num++;
        std::cout << "Thread_2 add one to num : " << _num << std::endl;

        /* 释放互斥锁. */
        pthread_mutex_unlock(&_mutex);
    }

    pthread_exit (NULL);

    return NULL;
}


#if 1
void* pthread_func_cond_1(void* args)
{
    for(; j <= 6; j++)
    {
        pthread_mutex_lock(&_mutex);/*锁住互斥量*/
        printf("[thread1]: locked g_mutex \n");

        if(j%3 == 0)
        {
            std::cout << "[thread1]: j = " << j << std::endl;
            printf("[thread1]: g_cond signal begin \n");
            pthread_cond_signal(&_cond); /*条件改变，发送信号，通知pthread_func_cond_2*/
            printf("[thread1]: g_cond signal end \n");
            printf("\n");
            sleep(1);
        }

        std::cout << "[thread1]: j = " << j << std::endl;
        pthread_mutex_unlock(&_mutex);/*解锁互斥量*/
        printf("[thread1]: unlocked g_mutex \n");
        printf("\n");
        sleep(1);
    }
}

void* pthread_func_cond_2(void* args)
{
    for(; j <= 6; j++)
    {
        pthread_mutex_lock(&_mutex);
        printf("[thread2]: locked g_mutex \n");

        if(j%3 != 0)
        {
            printf("[thread2]: g_cond wait begin \n");
            // wait until mutex unlocked and recv condition signal
            pthread_cond_wait(&_cond, &_mutex);     /*解锁mutex，并等待cond改变*/
            std::cout << "[thread2]: j = " << j << std::endl;
            printf("[thread2]: g_cond wait end \n");
            printf("\n");
            sleep(1);
        }

        std::cout << "[thread2]: j = " << j << std::endl;
        pthread_mutex_unlock(&_mutex);
        printf("[thread2]: unlocked g_mutex \n");
        printf("\n");
        sleep(1);
    }
}
#endif


void* thread_func_cond_01(void* args)
{
    while(true)
    {
        g_cond.lock();
        std::cout << "[thread 1 : locked] " << std::endl;
        ++g_count;
        if(g_count%10 != 0)
        {
            std::cout << "[thread 1 : g_count = " << g_count << std::endl;
            g_cond.unlock();
            std::cout << "[thread 1 : unlocked] " << std::endl;
            std::cout << std::endl;
        }
        else
        {
            g_cond.notify();
            std::cout << "[thread 1 : send cond signal] " << std::endl;
            g_cond.unlock();
            std::cout << "[thread 1 : unlocked] " << std::endl;
            std::cout << std::endl;
        }

        sleep(1);
    }
}


void* thread_func_cond_02(void* args)
{
    while(true)
    {
        g_cond.lock();
        std::cout << "[thread 2 : locked] " << std::endl;
        std::cout << "[thread 2 : cond wait begin] " << std::endl;
        std::cout << std::endl;
        g_cond.wait();

        std::cout << "[thread 2 : cond wait end] " << std::endl;
        std::cout << "[thread 2 : g_count = " << g_count << std::endl;

        g_cond.unlock();
        std::cout << "[thread 2 : unlocked] " << std::endl;
        std::cout << std::endl;

        sleep(1);
    }
}



UNIT_TEST::UNIT_TEST()
{

}


UNIT_TEST::~UNIT_TEST()
{

}



void UNIT_TEST::execute()
{
    std::cout << "********************CSP Unit Test******************** " << std::endl;
    int sel = -1;
    while(true)
    {
        std::cout << "### 0. exit ###" << std::endl;
        std::cout << "### 1. test thread ###" << std::endl;
        std::cout << "### 2. test mutex ###" << std::endl;
        std::cout << "### 3. test shm read ###" << std::endl;
        std::cout << "### 4. test shm write ###" << std::endl;
        std::cout << "### 5. test cond ###" << std::endl;
        std::cout << "### 6. test threadpool ###" << std::endl;
        std::cout << "select case : ";
        std::cin  >> sel;

        switch (sel)
        {
        case 0:
            _exit(0);
            break;
        case 1:
            test_thread();
            break;
        case 2:
            test_mutex();
            break;
        case 3:
            test_read_shm_001();
            break;
        case 4:
            test_write_shm("hello, shared memory");
            break;
        case 5:
            test_cond_01();
            break;
        case 6:
            test_threadpool();
            break;
        default:
            break;
        }
    }
}


void UNIT_TEST::test_thread()
{
    m_thrd1.registThreadFunc(pthread_func_1);
    m_thrd2.registThreadFunc(pthread_func_2);

    #if 0
    m_thrd1.create();
    m_thrd2.create();

    m_thrd1.join();
    m_thrd2.join();
    #endif

    #if 1
    /* 互斥初始化 */
    pthread_mutex_init(&_mutex, NULL);

    m_thrd1.run();
    m_thrd2.run();
    #endif

}


// test mutex
void UNIT_TEST::test_mutex()
{
    int ret = 0;

    /*线程的标识符*/
    pthread_t pt_1 = 0;
    pthread_t pt_2 = 0;

    /* 互斥初始化 */
    pthread_mutex_init(&_mutex, NULL);

    /* 分别创建线程1、2 */
    ret = pthread_create(&pt_1,  //线程标识符指针
                          NULL,  //默认属性
                          pthread_func_1, //运行函数
                          NULL); //无参数
    if (ret == 0)
    {
       std::cout << "pthread_1_create!" << std::endl;
    }

    ret = pthread_create(&pt_2, //线程标识符指针
                          NULL,  //默认属性
                          pthread_func_2, //运行函数
                          NULL); //无参数
    if (ret == 0)
    {
       std::cout << "pthread_2_create!" << std::endl;
    }

    /*等待线程1、2的结束*/
    pthread_join(pt_1, NULL);
    pthread_join(pt_2, NULL);
}


void UNIT_TEST::test_read_shm_001()
{
    std::cout << "Reader 001 start! " << std::endl;
    char* str = NULL;
    csp_utils_shm shm_item;
    shm_item.get_shm(1234);
    shm_item.attach_shm();
    str = (char*)shm_item.read_shm();
    std::cout << "read from shared memory : " << str << std::endl;
    //shm_item.detach_shm();
    //shm_item.del_shm();
}


void UNIT_TEST::test_write_shm(char* str)
{
    csp_utils_shm shm_item;
    shm_item.get_shm(1234);
    shm_item.attach_shm();
    shm_item.write_shm(str);
    std::cout << "write to shared memory : " << str << std::endl;
    //shm_item.detach_shm();
}


void UNIT_TEST::test_cond()
{
    pthread_mutex_init(&_mutex, NULL);
    pthread_cond_init(&_cond, NULL);

    m_thrd1.registThreadFunc(pthread_func_cond_1);
    m_thrd2.registThreadFunc(pthread_func_cond_2);

    m_thrd1.create();
    m_thrd2.create();

    m_thrd1.join();
    m_thrd2.join();
}


void UNIT_TEST::test_cond_01()
{
    m_thrd1.create(thread_func_cond_01);
    m_thrd2.create(thread_func_cond_02);

    m_thrd1.join();
    m_thrd2.join();
}


void UNIT_TEST::test_threadpool()
{
    csp_utils_threadpool* pool = csp_utils_threadpool::createThreadPool(5);
    msg_t msg[10];
    mytask my_task[10];
    char buf[32] = {0};
    int task_num = 10;

    // producer
    for(int i = 0; i < task_num; i++)
    {
        msg[i].task_id = i + 1;
        sprintf(buf, "task_%d", i+1);
        msg[i].task_name = buf;
        my_task[i].setData(&msg[i]);
        pool->addTask(&my_task[i]);
        //csp::utils::sleep_ms(100);
        sleep(1);
    }

    while (true)
    {
        if(pool->getTaskSize() == 0)
        {
            std::cout << "Now exit from main!" << std::endl;
            break;
        }

        csp::utils::sleep_s(1);
    }

    delete pool;
    pool = NULL;

#if 0
    while(true)
    {
        std::cout << "There are still " << tp.getTaskSize() << " tasks need to handle" << std::endl;
        std::cout << std::endl;

        if(tp.getTaskSize() == 0)
        {
            #if 1
            if(tp.stopAll() == -1)
            {
                std::cout << "ThreadPool clear, Exit! " << std::endl;
                break;
            }
            #endif
        }

        //sleep(1);
        csp::utils::sleep_s(1);
        std::cout << "1 sec later..." << std::endl;
    }
#endif

}


void UNIT_TEST::test_singleton()
{
    //auto obj = csp::utils::singleton<csp_threadpool>::getInstance();
    auto obj = csp::utils::singleton<msg_t>::getInstance();
}






