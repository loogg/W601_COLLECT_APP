/*
 * @Author: Longwei Ma
 * @Date: 2019-08-23 19:15:06
 * @LastEditTime: 2019-10-07 20:38:30
 * @LastEditors: Please set LastEditors
 * @Description: 
 */
#ifndef __ZMQ_H
#define __ZMQ_H
#include <rtthread.h>

typedef enum
{
    ZMQ_OPT_NULL = 0x00,
    ZMQ_OPT_LED,
    ZMQ_OPT_KEY,
    ZMQ_OPT_DCU,
    ZMQ_OPT_WEB
}zmq_opt_t;

typedef enum 
{
    MSG_TYPE_NULL,
    MSG_TYPE_RESET
}ipc_msg_type_t;

#define IPC_MSG_INITIALIZER {ZMQ_OPT_NULL, MSG_TYPE_NULL, 0, RT_NULL}

typedef struct 
{
    zmq_opt_t msg_src;
    ipc_msg_type_t msg_type;
    uint16_t len;
    uint8_t *data;
}ipc_msg_t;

int zmq_bind(zmq_opt_t opt);
int zmq_create(void);
void zmq_load_msg(ipc_msg_t *msg, zmq_opt_t src, ipc_msg_type_t type, uint8_t *buf, uint16_t len);
int zmq_publish(ipc_msg_t *msg, zmq_opt_t dest);
int zmq_subscribe(ipc_msg_t *msg, zmq_opt_t opt, rt_int32_t timeout);
#endif
