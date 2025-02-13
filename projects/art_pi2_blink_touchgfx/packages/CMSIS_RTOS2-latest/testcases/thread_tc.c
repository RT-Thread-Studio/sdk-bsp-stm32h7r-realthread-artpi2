#include <cmsis_rtthread.h>
#include "utest.h"

#define THREAD_STACK_SIZE  2048
#define THREAD_TIMESLICE   10

static osThreadId_t __current_thread;

static osThreadId_t tid1 = RT_NULL;

void thread1_entry(void *param)
{
}

static void test_thread_create(void)
{
    osThreadAttr_t thread1_attr;
    const char th1_name[5] = "th1";

    rt_memset((void *)&thread1_attr, 0, sizeof(thread1_attr));
    thread1_attr.name = th1_name;
    thread1_attr.stack_size = THREAD_STACK_SIZE;
    thread1_attr.priority = osThreadGetPriority(__current_thread) + 1;
    tid1 = osThreadNew(thread1_entry, (void *)1, &thread1_attr);

    uassert_true(tid1 != RT_NULL);
    return;
}

static rt_err_t utest_tc_init(void){
    __current_thread = osThreadGetId();
    return RT_EOK;
}

static rt_err_t utest_tc_cleanup(void){
    return RT_EOK;
}

static void testcase(void){
    UTEST_UNIT_RUN(test_thread_create)
}


UTEST_TC_EXPORT(testcase, "testcases.cmsis_os.kernel.thread_tc", utest_tc_init, utest_tc_cleanup, 1000);
