#ifndef __COLLECT_H
#define __COLLECT_H
#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"

#define COLLECT_ITEM_TYPE_FLOAT     0
#define COLLECT_ITEM_TYPE_SHORT     1

typedef struct{
    char *name;
    uint8_t value[4];
    uint8_t type;
    uint32_t addr;
}collect_items_t;
extern collect_items_t collect_items[5];


int collect_create(void);
#endif
