#include "cloud.h"
#include "collect.h"
#include <onenet.h>
#include <stdio.h>
#include <modbus.h>

static void cloud_publish(void *parameter)
{
    while (1)
    {
        for (int i = 0; i < sizeof(collect_items) / sizeof(collect_items[0]); i++)
        {
            char value[10] = "";
            if (collect_items[i].type == COLLECT_ITEM_TYPE_FLOAT)
            {
                snprintf(value, sizeof(value), "%.2f", modbus_get_float_abcd((uint16_t *)collect_items[i].value));
            }
            else if (collect_items[i].type == COLLECT_ITEM_TYPE_SHORT)
            {
                snprintf(value, sizeof(value), "%d", *(uint16_t *)collect_items[i].value);
            }

            onenet_mqtt_upload_string(collect_items[i].name, value);
            rt_thread_mdelay(200);
        }

        rt_thread_mdelay(20 * 1000);
    }
}

int cloud_create(void)
{
    onenet_mqtt_init();
    rt_thread_t tid;
    tid = rt_thread_create("cloud_publish", cloud_publish, RT_NULL, 2048, 11, 10);
    RT_ASSERT(tid != RT_NULL);
    rt_thread_startup(tid);
    return RT_EOK;
}
