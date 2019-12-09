#include "json_create.h"
#include "collect.h"
#include <modbus.h>
#include <cJSON.h>
#include <stdio.h>

char *json_create_collect_items(void)
{
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();

    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToObject(root, "code", cJSON_CreateNumber(0));
    cJSON_AddItemToObject(root, "count", cJSON_CreateNumber(sizeof(collect_items)/sizeof(collect_items[0])));

    for (int i = 0; i < sizeof(collect_items) / sizeof(collect_items[0]); i++)
    {
        cJSON *item = cJSON_CreateObject();
        cJSON_AddItemToObject(item, "name", cJSON_CreateString(collect_items[i].name));
        char value[10] = "";
        if(collect_items[i].type == COLLECT_ITEM_TYPE_FLOAT)
        {
            snprintf(value, sizeof(value), "%.2f", modbus_get_float_abcd((uint16_t *)collect_items[i].value));
        }
        else if(collect_items[i].type == COLLECT_ITEM_TYPE_SHORT)
        {
            snprintf(value, sizeof(value), "%d", *(uint16_t *)collect_items[i].value);
        }
        cJSON_AddItemToObject(item, "value", cJSON_CreateString(value));
        cJSON_AddItemToArray(items, item);
    }

    char *json_data = cJSON_PrintUnformatted(root);
    if (root)
        cJSON_Delete(root);
    return json_data;
}
