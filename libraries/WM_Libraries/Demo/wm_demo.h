#ifndef __WM_DEMO_H__
#define __WM_DEMO_H__

#define DEMO_ON		1
#define DEMO_OFF	0

//demo console
#define DEMO_CONSOLE				DEMO_OFF

//socket demo
#define DEMO_STD_SOCKET_CLIENT	    (DEMO_OFF && DEMO_CONSOLE)
#define DEMO_STD_SOCKET_SERVER	    (DEMO_OFF && DEMO_CONSOLE)

#define DEMO_UDP                    (DEMO_OFF && DEMO_CONSOLE)

//connect demo
#define DEMO_CONNECT_NET			(DEMO_OFF && DEMO_CONSOLE)

//apsta demo
#define DEMO_APSTA   		  		(DEMO_OFF && DEMO_CONSOLE)

//�����շ�����
#define DEMO_UARTx   		  		(DEMO_OFF && DEMO_CONSOLE)

//AP dmeo
#define DEMO_SOFT_AP   		  		(DEMO_OFF && DEMO_CONSOLE)

//WPS demo	
#define DEMO_WPS			  		(DEMO_OFF && DEMO_CONSOLE && TLS_CONFIG_WPS && TLS_IEEE8021X_EAPOL)

//gpio demo
#define DEMO_GPIO					(DEMO_OFF && DEMO_CONSOLE)

//flash demo
#define DEMO_FLASH					(DEMO_OFF && DEMO_CONSOLE)

//master spi demo
#define DEMO_MASTER_SPI			    (DEMO_OFF && DEMO_CONSOLE)

//�ӽ���demo
#define DEMO_ENCRYPT				(DEMO_OFF && DEMO_CONSOLE)

//rsa demo
#define DEMO_RSA				    (DEMO_OFF && DEMO_CONSOLE)

//rtc demo
#define DEMO_RTC				    (DEMO_OFF && DEMO_CONSOLE)

//timer demo
#define DEMO_TIMER				    (DEMO_OFF && DEMO_CONSOLE)

//i2c demo
#define DEMO_I2C					(DEMO_OFF && DEMO_CONSOLE)

//pwm demo
#define DEMO_PWM					(DEMO_OFF && DEMO_CONSOLE)

//pmu demo
#define DEMO_PMU					(DEMO_OFF && DEMO_CONSOLE)

//ntp demo
#define DEMO_NTP					(DEMO_OFF && DEMO_CONSOLE)

//i2s demo
#define DEMO_I2S                    (DEMO_OFF && DEMO_CONSOLE)

//slave spi demo
#define DEMO_SLAVE_SPI				(DEMO_OFF && DEMO_CONSOLE)

//ssl server demo
#define DEMO_SSL_SERVER				(DEMO_OFF && DEMO_CONSOLE)

//websocket demo
#define DEMO_WEBSOCKETS				(DEMO_OFF && DEMO_CONSOLE)

//http demo
#define DEMO_HTTP				    (DEMO_OFF && DEMO_CONSOLE)


////////////////////////////////////////////////////////////////

#define RemotePort	1000	//demo��Ϊclientʱ��Զ�̶˿�
#define LocalPort	1020	//demo��Ϊserverʱ�����ض˿�


// user prio 32 - 60
#define  DEMO_TASK_PRIO			                32
#define  DEMO_RAW_SOCKET_C_TASK_PRIO	        (DEMO_TASK_PRIO + 1)
#define  DEMO_RAW_SOCKET_S_TASK_PRIO	        (DEMO_RAW_SOCKET_C_TASK_PRIO + 1)
#define  DEMO_SOCKET_C_TASK_PRIO	            (DEMO_RAW_SOCKET_S_TASK_PRIO + 1)
#define  DEMO_SOCKET_S_TASK_PRIO	            (DEMO_SOCKET_C_TASK_PRIO + 1)
#define  DEMO_SOCKET_RECEIVE_TASK_PRIO	        (DEMO_SOCKET_S_TASK_PRIO + 1)
#define  DEMO_SOCKET_S_SEND_TASK_PRIO    	    (DEMO_SOCKET_RECEIVE_TASK_PRIO + 1)
#define  DEMO_SOCKET_S_RECEIVE_TASK_PRIO    	(DEMO_SOCKET_S_SEND_TASK_PRIO + 1)
#define  DEMO_UDP_TASK_PRIO                     (DEMO_SOCKET_S_RECEIVE_TASK_PRIO + 1)
#define  DEMO_UDP_RECEIVE_TASK_PRIO             (DEMO_UDP_TASK_PRIO + 1)
#define  DEMO_MCAST_TASK_PRIO	                (DEMO_UDP_RECEIVE_TASK_PRIO + 1)
#define  DEMO_SOCK_FWUP_TASK_PRIO	            (DEMO_MCAST_TASK_PRIO + 1)
#define  DEMO_SOCK_S_SEL_TASK_PRIO	            (DEMO_SOCK_FWUP_TASK_PRIO + 1)
#define  DEMO_ONESHOT_TASK_PRIO	                (DEMO_SOCK_S_SEL_TASK_PRIO + 1)
#define  DEMO_DMR_TASK_PRIO	                    (DEMO_ONESHOT_TASK_PRIO + 1)
#define  DEMO_CLOUD_TASK_PRIO                   (DEMO_DMR_TASK_PRIO + 1)
#define  DEMO_ALLJOYN_LED_TASK_PRIO             (DEMO_CLOUD_TASK_PRIO + 1)
#define  DEMO_KII_PUSH_RECV_MSG_TASK_PRIO	    (DEMO_ALLJOYN_LED_TASK_PRIO + 1)
#define  DEMO_KII_PUSH_PINGREQ_TASK_PRIO	    (DEMO_KII_PUSH_RECV_MSG_TASK_PRIO + 1)
#define  DEMO_UART_TASK_PRIO	                (DEMO_KII_PUSH_PINGREQ_TASK_PRIO + 1)
#define  DEMO_SSPI_TASK_PRIO	                (DEMO_UART_TASK_PRIO + 1)
#define  DEMO_SSL_SERVER_TASK_PRIO	            (DEMO_SSPI_TASK_PRIO + 1)
#define  DEMO_WEBSOCKETS_TASK_PRIO              (DEMO_SSL_SERVER_TASK_PRIO + 1)

#define DEMO_QUEUE_SIZE	32

#define DEMO_BUF_SIZE		TLS_UART_RX_BUF_SIZE


/*����demo�п����õ�����Ϣ*/
#define	DEMO_MSG_WJOIN_FAILD	            1
#define	DEMO_MSG_WJOIN_SUCCESS	            2
#define DEMO_MSG_SOCKET_RECEIVE_DATA		3
#define	DEMO_MSG_UART_RECEIVE_DATA			4
#define	DEMO_MSG_SOCKET_ERR					5
#define DEMO_MSG_SOCKET_CREATE				6
#define DEMO_MSG_SOCKET_TEST				7
#define DEMO_MSG_OPEN_UART					8
#define DEMO_MSG_UART_CMD_DATA				9

typedef struct demo_sys{
	char *rxbuf;		/*uart rx*/
	char *txbuf;		/*uart tx*/
	u16 wptr;
	u16 rptr;
	u8 overflag;		/*�����־*/
	u8 MsgNum;


	bool socket_ok;
	int socket_num;
	char *sock_rx;
	u16 sock_data_len;

	int recvlen;		//socket���յ����ݳ��ȣ����ڲ���
	int translen;		//socket���͵����ݳ��ȣ����ڲ���
	u8 is_raw; // 1:raw socket; 0:��׼socket
}ST_Demo_Sys;

void CreateDemoTask(void);
void DemoStdSockOneshotSendMac(void);
void DemoRawSockOneshotSendMac(void);


#endif
