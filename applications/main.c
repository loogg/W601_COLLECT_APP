#include <rtthread.h>
#include <rtdevice.h>
#include <fal.h>
#include <easyflash.h>
#include <wifi_config.h>
#include <dfs_fs.h>
#include <drv_lcd.h>
#include <netdev_ipaddr.h>
#include <netdev.h>

#include "zmq.h"
#include "dcu.h"

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define FS_PARTITION_NAME ("filesystem")

void wlan_ready_handler(int event, struct rt_wlan_buff *buff, void *parameter)
{
    struct rt_wlan_device *wlan = (struct rt_wlan_device *)rt_device_find(RT_WLAN_DEVICE_STA_NAME);
    
    lcd_show_string(32, 64, 24, "%s", inet_ntoa(wlan->netdev->ip_addr));
    rt_wlan_config_autoreconnect(RT_TRUE);
}

/* 断开连接回调函数 */
void wlan_station_disconnect_handler(int event, struct rt_wlan_buff *buff, void *parameter)
{
    lcd_show_string(32, 64, 24, "                ");
}

/**
* Name:        dcu_zmq_init
* Brief:       初始化线程间通信队列
* Input:       none     
* Output:      none
*/
static void dcu_zmq_init(void)
{
    RT_ASSERT(zmq_bind(ZMQ_OPT_KEY) == RT_EOK);
    RT_ASSERT(zmq_bind(ZMQ_OPT_LED) == RT_EOK);
    RT_ASSERT(zmq_bind(ZMQ_OPT_DCU) == RT_EOK);
    RT_ASSERT(zmq_bind(ZMQ_OPT_WEB) == RT_EOK);
    RT_ASSERT(zmq_create() == RT_EOK);
}

int main(void)
{
    LOG_I("Start init!");
    dcu_zmq_init();
    /* 初始化分区表 */
    fal_init();
    /* 初始化 easyflash */
    easyflash_init();
    
    lcd_clear(WHITE);
    lcd_set_color(WHITE, BLACK);
    lcd_show_string(32, 2, 32, "GET IP");

    /* 配置 wifi 工作模式 */
    rt_wlan_set_mode(RT_WLAN_DEVICE_STA_NAME, RT_WLAN_STATION);
    /* 注册 wlan ready 回调函数 */
    rt_wlan_register_event_handler(RT_WLAN_EVT_READY, wlan_ready_handler, RT_NULL);
    /* 注册 wlan 断开回调函数 */
    rt_wlan_register_event_handler(RT_WLAN_EVT_STA_DISCONNECTED, wlan_station_disconnect_handler, RT_NULL);
    
    /* 初始化自动连接配置 */
    wlan_autoconnect_init();
    /* 使能 wlan 自动连接 */
    rt_wlan_config_autoreconnect(RT_TRUE);

    /* 在 filesystem 分区上创建一个 Block 设备 */
    rt_device_t flash_dev;
    flash_dev = fal_blk_device_create(FS_PARTITION_NAME);
    if (flash_dev == NULL)
    {
        LOG_E("Can't create a Block device on '%s' partition.", FS_PARTITION_NAME);
    }

    /* 挂载 FAT32 文件系统 */
    if (dfs_mount(FS_PARTITION_NAME, "/", "elm", 0, 0) == 0)
    {
        LOG_I("Filesystem initialized!");
    }
    else
    {
        /* 创建 FAT32 文件系统 */
        dfs_mkfs("elm", FS_PARTITION_NAME);
        /* 再次挂载 FAT32 文件系统 */
        if (dfs_mount(FS_PARTITION_NAME, "/", "elm", 0, 0) != 0)
        {
            LOG_E("Failed to initialize filesystem!");
        }
    }
    
    dcu_create();
    return 0;
}
