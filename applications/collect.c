#include "collect.h"
#include <modbus.h>
#include <sal_socket.h>

collect_items_t collect_items[5] = {
    {"Ua", {0}, COLLECT_ITEM_TYPE_FLOAT, 0},
    {"Ub", {0}, COLLECT_ITEM_TYPE_FLOAT, 2},
    {"Uc", {0}, COLLECT_ITEM_TYPE_FLOAT, 4},
    {"DO1", {0}, COLLECT_ITEM_TYPE_SHORT, 6},
    {"DO2", {0}, COLLECT_ITEM_TYPE_SHORT, 7},
};

static void collect(void *parameter)
{
    modbus_t *ctx = RT_NULL;
    
    ctx = modbus_new_tcp("192.168.1.102", 601, AF_INET);
    modbus_set_slave(ctx, 1);
    modbus_set_response_timeout(ctx, 0, 1000000);
_mbtcp_start:
    if(modbus_connect(ctx) < 0)
        goto _mbtcp_restart;
    
    while (1)
    {
        for (int i = 0; i < sizeof(collect_items) / sizeof(collect_items[0]); i++)
        {
            int nb = 0;
            if(collect_items[i].type == COLLECT_ITEM_TYPE_FLOAT)
                nb = 2;
            else if(collect_items[i].type == COLLECT_ITEM_TYPE_SHORT)
                nb = 1;
            int rc = modbus_read_registers(ctx, collect_items[i].addr, nb, (uint16_t *)collect_items[i].value);
            if(rc < 0)
                goto _mbtcp_restart;
            
            rt_thread_mdelay(300);
        }
    }

_mbtcp_restart:
    //7-关闭modbus端口
    modbus_close(ctx);
    rt_thread_mdelay(2000);
    goto _mbtcp_start;
    
    //8-释放modbus资源
    modbus_free(ctx);
}

int collect_create(void)
{
    rt_thread_t tid;
    tid = rt_thread_create("collect", collect, RT_NULL, 2048, 12, 10);
    RT_ASSERT(tid != RT_NULL);
    rt_thread_startup(tid);
    return RT_EOK;
}
