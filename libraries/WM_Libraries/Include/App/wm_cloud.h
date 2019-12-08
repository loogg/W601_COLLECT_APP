/**
 * @file    wm_cloud.h
 *
 * @brief   Cloud Interface API
 *
 * @author  winnermicro
 *
 * Copyright (c) 2015 Winner Microelectronics Co., Ltd.
 */
#ifndef __WM_CLOUD_H__
#define __WM_CLOUD_H__

#include "wm_config.h"
#if TLS_CONFIG_CLOUD
#include "wm_sockets.h"
#if TLS_CONFIG_HTTP_CLIENT_SECURE
#include "matrixsslApi.h"
#endif

#define SERVER_STATE_OFFLINE 0
#define SERVER_STATE_ONLINE 1

#define SOCKET_TCP 1
#define SOCKET_CLINET 2
#define SOCKET_SSL  4

/**
 * �������Զ���socket����Ϣ��Ŀǰsock_type��֧��Tcp server��
 * ��֧��udp��tcp client�Լ�ssl��tcp����
 */
typedef struct _SOCKET{
	int socket_num;  			/**< socket�ţ�socket�����ɹ���ϵͳ���룬
										�����߳�ʼ��ʱ������д */
	struct sockaddr * sock_addr;/**< socket��ַ��udpʱ����ʾ���ص�ַ��
										tcpʱ����ʾ�Զ˵�ַ */
	u8 sock_type; 				/**< socket���Ͷ��壬Ŀǰ��������bit��Ч��
										����鿴SOCKET_xxx���� */
	u8 connected;  				/**< socket���ӱ�־����tcp client��Ч��
										Ϊ1��ʾ���ӳɹ� */
	void * user_param;			/**< �������Զ�����Ϣ�����������Ƿ�Ϊ������
										�Զ���socket */
#if TLS_CONFIG_HTTP_CLIENT_SECURE
	ssl_t * ssl; 				/**< ssl������Ϣ����SOCKET_SSLʱ��Ч */
#endif
}SOCKET;

/** �������Զ���socket�������ݵĸ�ʽ */
typedef struct CReadData{
	SOCKET *socket;  			/**< socket�����Ϣ */
	char * read_data;			/**< ���յ����� */
	u32     data_len;  			/**< �������ݳ��� */
	struct sockaddr_in sin_recv;/**< �Զ�socket��ַ����udpʱ��Ч */
}CloudReadData;

#if TLS_CONFIG_CLOUD_KII
/** �ƶ˻ص����ݸ�ʽ */
typedef struct _Cloud_Callback_data
{
	char* bucketName;/**< bucket���� */
	char* objectId;  /**< ����id */
	char* jsonObject;/**< �������ݣ�ͨ����json��ʽ��ʾ */
} CloudCallbackData, *PCloudCallbackData;
/** �����ϴ��ƶ˻ص���errcodeΪ0��ʾ�ϴ��ɹ���dataΪ�ϴ����ݣ�
����ϴ��ɹ������Դ�data->objectId�õ��ϴ��Ƶ����ݶ���id */
typedef void (*upload_callback)(PCloudCallbackData data, int errcode);
/** �ϴ��ƶ˵����ݸ�ʽ */
typedef struct _CloudData
{
	char* bucketName;    /**< bucket���� */
	char* jsonObject;    /**< �������ݣ�ͨ����json��ʽ��ʾ */
	upload_callback cb;  /**< �ϴ�����ص� */
} CloudData,*PCloudData;

/** ��ʼ���Ʋ������� */
typedef struct _Cloud_Init_arg
{
	char* site;       /**< ��������ַ��Ŀǰ�ɴ���"CN" "US" "JP" "SG" */
	char* appId;      /**< Ӧ��ID */
	char* appKey;     /**< Ӧ��Key */
	char* deviceType; /**< �豸���� */
	char* venderId;   /**< �豸ID��ͨ��Ϊ�豸��MAC��ַ */
	char* modelName;  /**< ģ�����ƣ��Զ����ַ�����OTA���͵�bucketName��ʽΪ */
	char* password;   /**< �豸���� */
	char* recvDataBucket;    /**< �豸���͵�bucketName */
} CloudInitArg, *PCloudInitArg;
#endif

#if TLS_CONFIG_CLOUD_JD
/** �Ʋ�������ö�� */
enum cloud_opt_type
{
	UPLOAD_REQ,    	/**< �ϴ��������� */
	UPLOAD_RESP,    /**< �ϴ�������Ӧ */
	CONTROL_REQ,    /**< �ƶ˿����������� */
	CONTROL_RESP,   /**< ����������Ӧ */
	SNAPSHOT_REQ,   /**< �ƶ�ץȡ���գ�״̬������ */
	SNAPSHOT_RESP   /**< ���գ�״̬����Ӧ */
};

/**
 *  �ƶ�ͨ�����ݽṹ����Ҫע��һ������:
 *	1)��Ϊtls_cloud_upload_data��������:
 *	 �ϴ��������󡢿���������Ӧ�����գ�״̬����Ӧ�ӿڣ�
 *	 �����ϴ�������������˽ӿڷ��غ�data->optΪUPLOAD_RESP��
 *	 ��ʵ����ͬ����Ӧ���ϴ������ʱdata->result��Ϊ�ϴ����ݺ��ƶ˷��ؽ����
 *	 CloudData��Ҫ�������Լ������ڴ棬�Լ��ͷš�
 *	2)��Ϊcloud_callback�ص�����:
 *	�ϴ�������Ӧ���ƶ˿��������´�ƶ�ץȡ���գ�״̬������ʱ��
 *	cloud_callback�ص��ӿڱ����á�
 *	�����ƶ˿������������ƶ�ץȡ������������ڴ˻ص��ӿڷ��غ�
 *	data->optΪCONTROL_RESP��SNAPSHOT_RESP����ʵ����ͬ����Ӧ�˴����
 *	��ʱdata->result��Ϊ����������Ӧ�����data->values�а����˿�����Ӧ�����
 *	ͬ����ӦSNAPSHOT_RESPʱ��names��values���ֵ��ڴ��Լ�ͬ����ӦCONTROL_RESPʱ��
 *	data->control_resp���ֵ��ڴ��ɿ��������룬����ʵ�����ͷš�
 */
typedef struct _CloudData
{
	enum cloud_opt_type opt;/**< ��������ö�� */
	int result;    		/**< �ƶ���Ӧ�ϴ����ݻ��豸��Ӧ�ƶ˿��ƽ�� */
	char* control_resp; /**< �ƶ˿�������ʧ��ԭ�� */
	char** names;  		/**< �ϴ����������ƶ˿�������
							 �ƶ˿�������������Ӧ���ֶ��� */
	char** values;    	/**< �ϴ����������ƶ˿������󡢿�����Ӧ��ֵ */
	int cnt;    		/**< �ֶθ��� */
	void * arg;    		/**< �ƶ˷��͹����Ĳ�������Ҫԭ�ⲻ���ش����ƶ� */
} CloudData,*PCloudData, CloudCallbackData, *PCloudCallbackData;
/** ��ʼ���Ʋ������壬Ӧ�ô���product_uuid */
typedef struct _Cloud_Init_arg
{
	char *product_uuid;    /**< ��ƷUUID */
} CloudInitArg, *PCloudInitArg;
#endif
#if TLS_CONFIG_CLOUD_GAGENT

/** �ƶ�ͨ�����ݽṹ */
typedef struct _CloudData
{
    int32 totalcap;    /**< ȫ�ֻ������Ĵ�С 4k+128byte */
    int32 remcap;      /**< ���ݰ�ͷ�����೤�� 128byte */
    uint8 *allbuf;     /**< ������ʼ��ַ */
    int32 bufcap;      /**< ��������Ĵ�С 4K */

    uint32 type;       /**< ��Ϣ������ */
    uint8 *phead;      /**< ���ݰ�ͷ��ʼ��ַ */
    uint8 *ppayload;   /**< ҵ���߼�ͷ��ʼ��ַ */
    uint8 *pend;       /**< ���ݽ�����ַ */
} CloudData,*PCloudData, CloudCallbackData, *PCloudCallbackData;

/** ��ʼ���Ʋ������� */
typedef struct _Cloud_Init_arg
{
	char* product_key;    	/**< ��Ʒ��ʶ�� */
	char* wifi_soft_ver;    /**< ����汾�� */
	char* wifi_hard_ver;    /**< Ӳ���汾�� */
} CloudInitArg, *PCloudInitArg;
#endif
#if TLS_CONFIG_CLOUD_360
/**
 *          �����ϴ��ƶ˻ص���errcodeΪ0��ʾ�ϴ��ɹ���user_paramΪ
 *          ����tls_cloud_upload_dataʱ�����CloudData�е�user_param
 */
typedef void (*upload_callback)(void* user_param, int errcode);

/** �ƶ˻ص����ݸ�ʽ */
typedef struct _Cloud_Callback_data
{
    const uint8_t* data;    /**< ���� */
    size_t datalen;    		/**< ���ݳ��� */
}CloudCallbackData, *PCloudCallbackData;

/** �ϴ��ƶ˵����ݸ�ʽ */
typedef struct _CloudData
{
    void* user_param;    	/**< �û����������ϴ��ص��д���˲��� */
    const uint8_t* data;    /**< ���� */
    size_t datalen;    		/**< ���ݳ��� */
    upload_callback cb;    	/**< �ϴ�����ص� */
} CloudData,*PCloudData;

/** ��ʼ���Ʋ������� */
typedef struct _Cloud_Init_arg
{
	int product_id;    				/**< ��ƷID */
	unsigned char product_aes[16];  /**< ��ƷAES��Կ */
	char *device_name;    			/**< �豸���� */
} CloudInitArg, *PCloudInitArg;
#endif

#if TLS_CONFIG_CLOUD_QQ

/**
 *          �����ϴ��ƶ˻ص���errcodeΪ0��ʾ�ϴ��ɹ���cookieΪ����
 *          ����ţ����ڵ���tls_cloud_upload_dataʱ�õ���CloudData��
 *          ��pCookie��ͬ
 *
 */
typedef void (*upload_callback)(int32_t err_code, uint32_t cookie);

/** �ƶ˻ص����ݸ�ʽ */
typedef struct  _Cloud_Callback_data {
    uint32_t property_id;    	   /**< ����ID */
    uint8_t *property_value;       /**< ����value������Ϊ�ֽ����� */
    uint32_t property_value_len;   /**< ����value�ĳ��� */
    uint32_t seq;    	/**< ������� */
    int32_t  ret_code;  /**< �յ�cc datapoint���ظ�ACKʱ���ô�ֵ��H5ҳ��ʹ�� */
} CloudCallbackData, *PCloudCallbackData;

/** �ϴ��ƶ˵����ݸ�ʽ */
typedef struct _CloudData
{
	CloudCallbackData *datapoints;    	/**< ���ݽṹ */
	uint32_t datapoints_count;    		/**< ���� */
	upload_callback cb;    				/**< �ϴ�����ص� */
	uint32_t *pCookie;    				/**< ��������� */
}CloudData,*PCloudData;

/** ��ʼ���Ʋ������� */
typedef struct _Cloud_Init_arg{
    /* Ӳ����Ϣ */
    char*    device_name;    /**< �豸����,'\0'��β�ַ������Ȳ�����32�ֽڣ�
    							  nettypeΪ1ʱ��������վע���devicenameһ�� */
    char*    sn;    		/**< �豸���к�,'\0'��β�ַ������ȱ�����16�ֽ� */
    char*    license;    	/**< �豸LICENSE */
    int32_t  product_id;    /**< ÿһ�����̵�ÿһ�����͵��豸��Ӧ��һ��id */
    int32_t  product_version;    /**< ��Ʒ�İ汾��Ϣ������OTA��Ĭ����0 */
    /** ��Կ��Ϣ */
    uint8_t* client_pub_key;    /**< client��Կ��ÿ̨�豸���ɺ���¼���豸 */
    int32_t  client_pub_key_len;
    uint8_t* auth_key;    		/**< authkey��ÿ̨�豸���ɺ���¼���豸 */
    int32_t  auth_key_len;
} CloudInitArg, *PCloudInitArg;
#endif
#if TLS_CONFIG_CLOUD_ABLECLOUD

/** ABLECLOUD�ƶ�ͨ�����ݽṹ */
typedef struct _CloudData
{
    u8 msg_id;    /**< ��ϢID����Ӧ��Ϣͬ�ƶ��·�����Ϣ��ID�������ϴ����� */
    u8 msg_code;  /**< ��ϢCode����Ӧ��Ϣͬ�ƶ��·�����Ϣcode��
    				   �����ϴ���Ҫ����200 */
    u8 *data;    /**< ���� */
    uint32 datalen;    /**< ���ݳ��� */
} CloudData,*PCloudData, CloudCallbackData, *PCloudCallbackData;

/** ��ʼ���Ʋ������� */
typedef struct _Cloud_Init_arg
{
	u32 major_domain;    	/**< ����ID */
	u16 sub_domain;    		/**< ����ID */
	u8 private_key[112];    /**< ��Ʒ˽Կ */
	u32 product_version;    /**< ��Ʒ�İ汾��Ϣ������OTA */
} CloudInitArg, *PCloudInitArg;
#endif
#if TLS_CONFIG_CLOUD_ELIAN
enum
{
	DATA_TYPE_BINARY = 0,    /**< ������ */
	DATA_TYPE_TEXT = 1,      /**< �ı���ʽ */
	DATA_TYPE_NV = 2,        /**< �Կո���������Ӳ����ĸ�ʽ��
								  �磺power 34�� */
	DATA_TYPE_EL_CMD = 3,    /**< Ϊ4�ֽ���������Id+4�ֽ�������ֵ��
								  �磺140 (��Դ״̬�ϱ���14Ϊ������
								  0ҲΪ��������ʾ״̬���� */
	DATA_TYPE_INFRA = 6,     /**< �����ʽ */
	DATA_TYPE_STATUS_REQ = 7,/**< �豸״̬���� */
	DATA_TYPE_EL_CMD2 = 8    /**< Ϊ4�ֽ���������Id+�ַ�����ʽ��ֵ��
								  �磺140 (��Դ״̬�ϱ���14Ϊ������
								  0Ϊ�ַ�������ʾ״̬)�� */
};

/** �ƶ�ͨ�����ݽṹ */
typedef struct _CloudData
{
    const uint8_t* data;    /**< ���� */
    size_t datalen;    		/**< ���ݳ��� */
    u8 datatype;
} CloudData,*PCloudData, CloudCallbackData, *PCloudCallbackData;

/** ��ʼ���Ʋ������� */
typedef struct _Cloud_Init_arg
{
	char*                 sn;    /**< �豸���к�,'\0'��β�ַ�������
									  ������13�ֽ� */
} CloudInitArg, *PCloudInitArg;
#endif

/** �豸���ƶ�ע���õ��Ĺ����豸����Ϣ */
typedef struct _Cloud_Dev_info{
	u8* device_id;    	 /**< �豸ID */
	u8* access_token;    /**< �豸token�����豸IDһ�𹹳��豸�����Ƶ�ƾ֤ */
}CloudDeviceInfo, *PCloudDeviceInfo;


/**
 * @brief          �����ƵĻص�
 *
 * @param[in]      data    
 *
 * @retval         0     success
 * @retval         other failed
 *
 * @note           None
 */
typedef int (*cloud_callback)(PCloudCallbackData data);

/**
 * @brief          ������״̬�Ļص���statusΪ1��ʾ�豸���ߣ�statusΪ0��ʾ
                   �豸������
 *
 * @param[in]      status    
 *
 * @retval         0 ��ʾ�豸������
 * @retval         1 ��ʾ�豸����
 *
 * @note           None
 */
typedef void (*cloud_online_status_callback)(int status);

/**
 * @defgroup APP_APIs APP APIs
 * @brief APP APIs
 */

/**
 * @addtogroup APP_APIs
 * @{
 */

/**
 * @defgroup cloud_APIs cloud APIs
 * @brief cloud APIs
 */

/**
 * @addtogroup cloud_APIs
 * @{
 */

/**
 * @brief          ��ʼ���ƣ�user task��ʼ��ʱ���ã�
 				   �Ծ����ƣ�argΪproduct_uuid��
 				   ��������arg��鿴CloudInitArg�ṹ����
 *
 * @param[in]      arg
 *
 * @retval         0     success
 * @retval         other failed
 *
 * @note           None
 */
int tls_cloud_init(PCloudInitArg arg);

/**
 * @brief          ע���ƣ��豸����ʱ���ã�������Դ��blockΪ0ʱ���˺�����
                   �����أ�ע�����ں�ִ̨�У�blockΪ1ʱ����ע����ɺ�˺���
                   ���ء�
 *
 * @param[in]      block   �Ƿ�������Ϊ1��ʾ��ֱ����ע����ɷ��أ�
 						   Ϊ0��ʾ���������أ���ע���ں�ִ̨�����
 *
 * @return         None
 *
 * @note           None
 */
void tls_cloud_finish(u8 block);

/**
 * @brief          �豸�����ϴ����ݣ������Կ����������Ӧ���ϴ�״̬�Լ���
                   �����յ�
 *
 * @param[in]      data
 *
 * @retval         0     success
 * @retval         other failed
 *
 * @note           None
 */
int tls_cloud_upload_data(PCloudData data);

/**
 * @brief          �����ƵĻص��ӿڣ����豸�����ƵĿ������������Ϣ�ȵ�
                   Ψһ�ӿ�
 *
 * @param[in]      callback
 *
 * @retval         0     success
 * @retval         other failed
 *
 * @note           None
 */
int tls_cloud_set_callback(cloud_callback callback);

/**
 * @brief          ������״̬�Ļص��ӿڣ��豸���ߣ�����ʱ�ص��˽ӿ�
 *
 * @param[in]      callback
 *
 * @return         None
 *
 * @note           None
 */
void tls_cloud_set_online_status_callback(cloud_online_status_callback callback);

/**
 * @brief          ����豸���ƶ�ע�����豸�����Ϣ��
 				   ����鿴CloudDeviceInfo�ṹ����
 *
 * @param[in]      info
 *
 * @retval         0     success
 * @retval         other failed
 *
 * @note           None
 */
int tls_cloud_get_device_info(PCloudDeviceInfo info);

/*
* ����extern��ͷ�ĺ�������Ҫ������ʵ�֣�����ʵ�ֲ鿴wm_lsd_device_found.c����Ϊ����д��һ���������豸���ֵļ�ʵ�֣�
* �����߿��Ը�����Ҫ��д�Լ����豸����ʵ�֣������Ǿ�����ͨ�ſ��Ƶȡ�
*/

/**
 * @brief          ��ÿ����Զ����socket��Ϣ��countΪ���֧�ֵ�socket����
                   ���ݲ�ͬ���ƴ˲���������ͬ
 *
 * @param[in]      sockets
 * @param[in]      count
 *
 * @retval         0     success
 * @retval         other failed
 *
 * @note           None
 */
extern int tls_cloud_get_custom_sockets(SOCKET** sockets, int count);

/**
 * @brief          �������Զ���socket�������ݵĴ��������˺�����socket��
                   �յ�����ʱ�����ã�
                   �����߿ɸ���SOCKET�е�user_param�ж��Ƿ�Ϊ�Զ���socket
                   ��Ȼ������Ӧ�Ĵ���
 *
 * @param[in]      data
 *
 * @retval         0     success
 * @retval         other failed
 *
 * @note           None
 */
extern int tls_cloud_socket_data_handler(CloudReadData* data);


/**
 * @brief          ͨ��socket�������ݣ�����to��tolenֻ��udpʱ��Ч���ұ���
                   ��tcpʱto��������NULL
 *
 * @param[in]      *sock
 * @param[in]      *data
 * @param[in]      size
 * @param[in]      flags
 * @param[in]      *to
 * @param[in]      tolen
 *
 * @retval         0     success
 * @retval         other failed
 *
 * @note           None
 */
int tls_cloud_socket_sendto(SOCKET *sock, const void *data, size_t size, int flags,
       const struct sockaddr *to, socklen_t tolen);

/**
 * @}
 */

/**
 * @}
 */

#endif //TLS_CONFIG_CLOUD
#endif

