#ifndef __LED_H
#define __LED_H
#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"
#include "zmq.h"

void led_control(zmq_opt_t src, ipc_msg_type_t type, uint8_t *payload, uint16_t payload_len);
int led_create(void);
#endif
