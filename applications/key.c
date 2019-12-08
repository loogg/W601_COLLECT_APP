#include "key.h"
#include <agile_button.h>
#include "zmq.h"

#define RESET_TIME 2000

static agile_btn_t *btn_key_up = RT_NULL;
static agile_btn_t *btn_key0 = RT_NULL;
static agile_btn_t *btn_key1 = RT_NULL;

/**
* Name:        btn_click_event_cb
* Brief:       按键处理回掉函数
* Input:       
*   @btn:      按键句柄     
* Output:      none
*/
static void btn_click_event_cb(agile_btn_t *btn)
{
    if(btn->hold_time >= RESET_TIME)
    {
        ipc_msg_t send_msg = IPC_MSG_INITIALIZER;
        zmq_load_msg(&send_msg, ZMQ_OPT_KEY, MSG_TYPE_RESET, RT_NULL, 0);
        zmq_publish(&send_msg, ZMQ_OPT_DCU);
    }
}

/**
* Name:        key_create
* Brief:       按键初始化
* Input:       none  
* Output:      RT_EOK:成功
*/
int key_create(void)
{
    btn_key_up = agile_btn_create(PIN_WK_UP, PIN_HIGH, PIN_MODE_INPUT);
    agile_btn_start(btn_key_up);

    agile_btn_set_event_cb(btn_key_up, BTN_CLICK_EVENT, btn_click_event_cb);
    return RT_EOK;
}
