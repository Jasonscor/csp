#include <QCoreApplication>
#include "unit_test.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    UNIT_TEST ut;

    //ut.execute();
    //ut.test_read_shm_001();

    //ut.test_thread();

    //ut.test_cond();

    //ut.test_cond_01();

    ut.test_threadpool();

    return a.exec();
}

