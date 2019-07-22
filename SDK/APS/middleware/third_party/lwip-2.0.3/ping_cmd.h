#ifndef __PING_TEST_H__
#define __PING_TEST_H__


#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

#define PING_IP_ADDR_V4      0
#define PING_IP_ADDR_V6      1

typedef struct _ping_result
{
    uint32_t min_time;
    uint32_t max_time;
    uint32_t avg_time;
    uint32_t total_num;
    uint32_t lost_num;
    uint32_t recv_num;
} ping_result_t;

/**
 * PING_USE_SOCKETS: Set to 1 to use sockets, otherwise the raw api is used
 */
#ifndef PING_USE_SOCKETS
#define PING_USE_SOCKETS    LWIP_SOCKET
#endif

typedef void (* ping_request_result_t)(ping_result_t *result);

//addr_type:PING_IP_ADDR_V4 or PING_IP_ADDR_V6, current only support PING_IP_ADDR_V4
void ping_request(uint32_t count, char *addr, uint8_t addr_type, uint32_t ping_size,
                  uint32_t recv_timeout, uint32_t ping_period, ping_request_result_t callback);
void ping_init(uint32_t count, char *addr, uint8_t addr_len, uint32_t ping_size, uint32_t recv_timeout, uint32_t ping_period);
void ping_stop(void);


#if !PING_USE_SOCKETS
void ping_send_now(void);
#endif /* !PING_USE_SOCKETS */


#ifdef __cplusplus
}
#endif

#endif /* __PING_H__ */

