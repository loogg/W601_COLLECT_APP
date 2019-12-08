#include "web.h"
#include <webnet.h>
#include <wn_module.h>

void cgi_sysreset_handler(struct webnet_session *session);
void cgi_devdata_handler(struct webnet_session *session);

extern const struct webnet_module_upload_entry upload_bin_upload;

/**
* Name:        webnet_create
* Brief:       初始化webnet
* Input:       none    
* Output:      RT_EOK:成功
*/
int webnet_create(void)
{
    webnet_cgi_register("sysreset", cgi_sysreset_handler);
    webnet_cgi_register("dev_data", cgi_devdata_handler);
    
    webnet_upload_add(&upload_bin_upload);

    /* start WebNet */
    webnet_init();
    return RT_EOK;
}
