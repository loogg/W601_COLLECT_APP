#include <webnet.h>
#include <wn_module.h>

#include "zmq.h"
#include "json_create.h"

#define cgi_head()                       \
    const char *mimetype;                \
    mimetype = mime_get_type(".html");   \
    session->request->result_code = 200; \
    webnet_session_set_header(session, mimetype, 200, "Ok", -1);

void cgi_sysreset_handler(struct webnet_session *session)
{
    cgi_head();
    webnet_session_printf(session, "{\"code\":0}");

    ipc_msg_t send_msg = IPC_MSG_INITIALIZER;
    zmq_load_msg(&send_msg, ZMQ_OPT_WEB, MSG_TYPE_RESET, RT_NULL, 0);
    zmq_publish(&send_msg, ZMQ_OPT_DCU);
}

void cgi_devdata_handler(struct webnet_session *session)
{
    cgi_head();
    char *body = RT_NULL;
    if (session->request->query_counter)
    {
        body = json_create_collect_items();
        if(body != RT_NULL)
        {
            webnet_session_printf(session, body);
            rt_free(body);
        }
        else
        {
            webnet_session_printf(session, "{\"code\":-1}");
        }
    }
}

void cgi_mem_handler(struct webnet_session *session)
{
    cgi_head();
    char *body = RT_NULL;
    body = json_create_mem();
    webnet_session_printf(session, body);
    rt_free(body);
}
