#include <string.h>
#include <stdlib.h>
#include "ping_cmd.h"
#include "ping_cli.h"
#include "controller_wifi.h"


void ping_cli_callback(ping_result_t *ptResult)
{
    CtrlWifi_PsStateForce(STA_PS_NONE, 0);

    return;
}

int ping_cli_handler(int len, char *param[])
{
    uint32_t count;
    uint32_t pktsz;
    uint32_t recv_timeout;
    uint32_t ping_period;
    uint32_t awake_dur;

    if (len < 1) {
        return 1;
    } else {
        count = 3;
        pktsz = 64;
        recv_timeout = 1000; //ms
        ping_period = 1000;  //ms
    }

    if (len == 1 && (strcmp(param[0], "s") == NULL) ) {
        ping_stop();
        return 0;
    }

    if (len > 1) {
        count = strtol(param[1], NULL, 10);
    }

    if (len > 2) {
        pktsz = strtol(param[2], NULL, 10);
    }

    if (len > 3) {
        recv_timeout = strtol(param[3], NULL, 10);
    }

    if (len > 4) {
        ping_period = strtol(param[4], NULL, 10);
    }

    ping_request(count, param[0], PING_IP_ADDR_V4, pktsz, recv_timeout, ping_period, ping_cli_callback);

    if (count > 0)
        awake_dur = ((recv_timeout>ping_period)? recv_timeout:ping_period)*(count+1);
    else
        awake_dur = 0;
    CtrlWifi_PsStateForce(STA_PS_AWAKE_MODE, awake_dur);

    return 0;
}

