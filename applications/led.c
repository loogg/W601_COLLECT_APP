#include "led.h"
#include <agile_led.h>
#include "zmq.h"

static agile_led_t *led_r = RT_NULL;
static agile_led_t *led_g = RT_NULL;
static agile_led_t *led_b = RT_NULL;

#define LED_RUN_LIGHT_MODE  "1000,1000"

/**
* Name:         sys_led_reset
* Brief:        复位信号灯动作
* Input:        none
* Output:       none
*/
static void sys_led_reset(void)
{
    agile_led_stop(led_r);
    agile_led_stop(led_g);
    agile_led_stop(led_b);

    agile_led_off(led_r);
    agile_led_off(led_g);
    agile_led_off(led_b);

    rt_thread_mdelay(100);

    for(int count =6;count>0;count --)
    {
        agile_led_toggle(led_r);
        rt_thread_mdelay(250);
    }
}

/**
* Name:         led_msg
* Brief:        led线程通信线程
* Input:       
*   @parameter: 线程参数    
* Output:       none
*/
static void led_msg(void *parameter)
{
    ipc_msg_t msg = IPC_MSG_INITIALIZER;
    uint8_t net_connected = 0;
    uint8_t reset_lock = 0;
    uint8_t wifi_cfg_ok = 0;
    while (1)
    {
        if (zmq_subscribe(&msg, ZMQ_OPT_LED, RT_WAITING_FOREVER) == RT_EOK)
        {
            switch (msg.msg_type)
            {
            case MSG_TYPE_RESET:
                if(reset_lock == 0)
                {
                    sys_led_reset();
                    reset_lock = 1;
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
* Name:           led_control
* Brief:          控制led
* Input:   
*   @src:         消息源    
*   @type:        消息类型
*   @payload:     数据载体
*   @payload_len: 数据长度
* Output:         none
*/
void led_control(zmq_opt_t src, ipc_msg_type_t type, uint8_t *payload, uint16_t payload_len)
{
    ipc_msg_t send_msg = IPC_MSG_INITIALIZER;
    zmq_load_msg(&send_msg, src, type, payload, payload_len);
    zmq_publish(&send_msg, ZMQ_OPT_LED);
}

/**
* Name:         led_create
* Brief:        初始化led
* Input:        none 
* Output:       RT_EOK:成功
*/
int led_create(void)
{
    led_r = agile_led_create(PIN_LED_R, PIN_LOW, RT_NULL, 0);
    led_g = agile_led_create(PIN_LED_G, PIN_LOW, LED_RUN_LIGHT_MODE, -1);
    led_b = agile_led_create(PIN_LED_B, PIN_LOW, RT_NULL, 0);

    agile_led_start(led_g);

    rt_thread_t tid = rt_thread_create("led_msg", led_msg, RT_NULL, 512, 20, 10);
    RT_ASSERT(tid != RT_NULL);
    rt_thread_startup(tid);

    return RT_EOK;
}
