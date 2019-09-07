#ifndef _WRAPPERS_DEFS_H_
#define _WRAPPERS_DEFS_H_

#include "infra_types.h"
#include "infra_defs.h"

#define PLATFORM_WAIT_INFINITE (~0)

typedef struct {
    void *(*malloc)(uint32_t size);
    void (*free)(void *ptr);
} ssl_hooks_t;

typedef enum {
    os_thread_priority_idle = -3,        /* priority: idle (lowest) */
    os_thread_priority_low = -2,         /* priority: low */
    os_thread_priority_belowNormal = -1, /* priority: below normal */
    os_thread_priority_normal = 0,       /* priority: normal (default) */
    os_thread_priority_aboveNormal = 1,  /* priority: above normal */
    os_thread_priority_high = 2,         /* priority: high */
    os_thread_priority_realtime = 3,     /* priority: realtime (highest) */
    os_thread_priority_error = 0x84,     /* system cannot determine priority or thread has illegal priority */
} hal_os_thread_priority_t;

typedef struct _hal_os_thread {
    hal_os_thread_priority_t priority;     /*initial thread priority */
    void                    *stack_addr;   /* thread stack address malloced by caller, use system stack by . */
    int                   stack_size;   /* stack size requirements in bytes; 0 is default stack size */
    int                      detach_state; /* 0: not detached state; otherwise: detached state. */
    char                    *name;         /* thread name. */
} hal_os_thread_param_t;

#define DTLS_ERROR_BASE                (1<<24)
#define DTLS_SUCCESS                   (0)
#define DTLS_INVALID_PARAM             (DTLS_ERROR_BASE | 1)
#define DTLS_INVALID_CA_CERTIFICATE    (DTLS_ERROR_BASE | 2)
#define DTLS_HANDSHAKE_IN_PROGRESS     (DTLS_ERROR_BASE | 3)
#define DTLS_HANDSHAKE_FAILED          (DTLS_ERROR_BASE | 4)
#define DTLS_FATAL_ALERT_MESSAGE       (DTLS_ERROR_BASE | 5)
#define DTLS_PEER_CLOSE_NOTIFY         (DTLS_ERROR_BASE | 6)
#define DTLS_SESSION_CREATE_FAILED     (DTLS_ERROR_BASE | 7)
#define DTLS_READ_DATA_FAILED          (DTLS_ERROR_BASE | 8)

typedef struct {
    void *(*malloc)(uint32_t size);
    void (*free)(void *ptr);
} dtls_hooks_t;

typedef struct {
    unsigned char             *p_ca_cert_pem;
    char                      *p_host;
    unsigned short             port;
} coap_dtls_options_t;

typedef void DTLSContext;


typedef struct
{
    uint32_t event;
    uint32_t length;
    uint8_t ucaMessage[];
} xAliNetlinkDataMessage_t;

typedef enum ali_netlink_msg_type
{
	ALI_NETLINK_AWSS_REPORT_TOKEN_TO_CLOUD=0,
	ALI_NETLINK_AWSS_DEV_BIND_NOTIFY,
	ALI_NETLINK_AWSS_DEV_BIND_NOTIFY_FUN,
	ALI_NETLINK_AWSS_DEV_INFO_NOTIFY,
	ALI_NETLINK_AWSS_SUCCESS_NOTIFY,
	ALI_NETLINK_AWSS_PROCESS_GET_DEVINFO,
    ALI_NETLINK_AWSS_REPORT_RESET_TO_CLOUD,

    ALI_NETLINK_DATA_TX_MSG_NUM
} ali_netlink_msg_type_e;

typedef void (*T_Ali_Netlink_EvtHandler_Fp)(uint32_t evt_type, void *data, int len);
typedef struct
{
    uint32_t ulEventId;
    T_Ali_Netlink_EvtHandler_Fp fpFunc;
} T_Ali_Netlink_EvtHandlerTbl;

//netlink customize
																


void hal_ali_netlink_task_init(void);
int hal_ali_netlink_Task_MsgSend(int msg_type, uint8_t *data, int data_len);

void Ali_Netlink_Awss_Report_Token_To_Cloud(uint32_t evt_type, void *data, int len);
void Ali_Netlink_Awss_Dev_Bind_Notify(uint32_t evt_type, void *data, int len);
void __Ali_Netlink_Awss_Dev_Bind_Notify(uint32_t evt_type, void *data, int len);
//void Iot_Awss_Dev_Info_Notify(uint32_t evt_type, void *data, int len);
extern int __awss_suc_notify(void);
void Ali_Netlink_AliSendAwssSucNotify(uint32_t evt_type, void *data, int len);

void Ali_Netlink_Awss_Process_Get_Devinfo(uint32_t evt_type, void *data, int len);
void Ali_Netlink_Awss_Report_Reset_To_Cloud(uint32_t evt_type, void *data, int len);
//Netlink Kevin modify for rebinding
void HAL_ResetAliBindflag(void);
#endif

