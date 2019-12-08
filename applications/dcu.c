#include "dcu.h"
#include <rthw.h>
#include "led.h"
#include "zmq.h"
#include "web.h"
#include "key.h"
#include "collect.h"
#include "cloud.h"

/**
* Name:        dcu_msg
* Brief:       dcu线程间通信线程
* Input:       
*   @dcu_msg:  线程参数   
* Output:      none
*/
static void dcu_msg(void *parameter)
{
    ipc_msg_t msg = IPC_MSG_INITIALIZER;
    while (1)
    {
        if (zmq_subscribe(&msg, ZMQ_OPT_DCU, RT_WAITING_FOREVER) == RT_EOK)
        {
            switch (msg.msg_type)
            {
            case MSG_TYPE_RESET:
            {
                led_control(ZMQ_OPT_DCU, MSG_TYPE_RESET, RT_NULL, 0);
                rt_thread_mdelay(2000);
                rt_hw_cpu_reset();
            }
            break;

            default:
            break;
            }
            rt_free(msg.data);
            msg.data = NULL;
        }
    }
}

/**
* Name:        dcu_thread_create
* Brief:       创建dcu线程
* Input:       none
* Output:      none
*/
static void dcu_thread_create(void)
{
    rt_thread_t tid;
    tid = rt_thread_create("dcu_msg", dcu_msg, RT_NULL, 1024, 19, 100);
    RT_ASSERT(tid);
    rt_thread_startup(tid);
}

/**
* Name:        dcu_create
* Brief:       初始化各个硬件及线程
* Input:       none
* Output:      RT_EOK:成功
*/
int dcu_create(void)
{
    led_create();
    key_create();
    collect_create();
    dcu_thread_create();
    webnet_create();
    cloud_create();
    return RT_EOK;
}
