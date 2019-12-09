#include "zmq.h"
#include "stdio.h"

//#define DBG_ENABLE
#define DBG_COLOR
#define DBG_LEVEL    DBG_LOG
#define DBG_SECTION_NAME     "zmq"
#include <rtdbg.h>

typedef struct 
{
    zmq_opt_t opt;
    rt_mq_t mq;
    rt_list_t list;
}zmq_mq_t;


static rt_list_t zmq_mq_list;

static int zmq_init(void)
{
    rt_list_init(&zmq_mq_list);
    return RT_EOK;
}
INIT_ENV_EXPORT(zmq_init);

static zmq_mq_t *zmq_get_by_opt(zmq_opt_t opt)
{
    rt_list_t *node;
    rt_list_for_each(node, &zmq_mq_list)
    {
        zmq_mq_t *zmq_mq = rt_list_entry(node, zmq_mq_t, list);
        if(zmq_mq->opt == opt)
            return zmq_mq;
    }
    return RT_NULL;
}

int zmq_bind(zmq_opt_t opt)
{
    if(zmq_get_by_opt(opt))
        return -RT_ERROR;
    zmq_mq_t *zmq_mq = (zmq_mq_t *)rt_malloc(sizeof(zmq_mq_t));
    if(!zmq_mq)
    {
        LOG_E("No mem for zmq_bind");
        return -RT_ERROR;
    }

    zmq_mq->opt = opt;
    zmq_mq->mq = RT_NULL;
    rt_list_init(&(zmq_mq->list));

    rt_list_insert_before(&zmq_mq_list, &(zmq_mq->list));
    return RT_EOK;
}

int zmq_create(void)
{
    rt_err_t result = RT_EOK;
    rt_list_t *node;
    char name[RT_NAME_MAX];
    int index = 0;
    rt_list_for_each(node, &zmq_mq_list)
    {
        zmq_mq_t *zmq_mq = rt_list_entry(node, zmq_mq_t, list);
        if(zmq_mq->mq == RT_NULL)
        {
            rt_memset(name, 0, sizeof(name));
            snprintf(name, sizeof(name), "%s%d", "zmq_mq", index++);
            zmq_mq->mq = rt_mq_create(name, sizeof(ipc_msg_t), 10, RT_IPC_FLAG_FIFO);
            if(zmq_mq->mq == RT_NULL)
            {
                result = -RT_ENOMEM;
                goto _exit;
            }
        }
    }

_exit:
    if(result != RT_EOK)
    {
        while(!rt_list_isempty(&zmq_mq_list))
        {
            zmq_mq_t *zmq_mq = rt_list_entry(zmq_mq_list.next, zmq_mq_t, list);
            rt_list_remove(&(zmq_mq->list));
            if(zmq_mq->mq)
            {
                rt_mq_delete(zmq_mq->mq);
                zmq_mq->mq = RT_NULL;
            }
            rt_free(zmq_mq);
        }
    }
    return result;
}

void zmq_load_msg(ipc_msg_t *msg, zmq_opt_t src, ipc_msg_type_t type, uint8_t *buf, uint16_t len)
{
    msg->msg_src = src;
    msg->msg_type = type;
    msg->data = buf;
    msg->len = len;
}

int zmq_publish(ipc_msg_t *msg, zmq_opt_t dest)
{
    zmq_mq_t *zmq_mq = zmq_get_by_opt(dest);
    if(!zmq_mq)
        return -RT_ERROR;

    ipc_msg_t send_msg = IPC_MSG_INITIALIZER;
    send_msg.msg_src = msg->msg_src;
    send_msg.msg_type = msg->msg_type;
    send_msg.len = msg->len;
    if (msg->data && msg->len)
    {
        send_msg.data = (uint8_t *)rt_malloc(msg->len);
        if (!send_msg.data)
            return -RT_ENOMEM;
        
        rt_memcpy(send_msg.data, msg->data, msg->len);
    }
    
    if(rt_mq_send(zmq_mq->mq, (void *)&send_msg, sizeof(ipc_msg_t)) != RT_EOK)
    {
        if (send_msg.data)
        {
            rt_free(send_msg.data);
            send_msg.data = RT_NULL;
        }
        return -RT_ERROR;
    }
    return RT_EOK;
}

int zmq_subscribe(ipc_msg_t *msg, zmq_opt_t opt, rt_int32_t timeout)
{
    zmq_mq_t *zmq_mq = zmq_get_by_opt(opt);
    if(!zmq_mq)
        return -RT_ERROR;
    
    return rt_mq_recv(zmq_mq->mq, (void *)msg, sizeof(ipc_msg_t), timeout);
}

