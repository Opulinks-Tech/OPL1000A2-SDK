#if defined(GROUP_SOURCE)
    #pragma arm section rwdata="group_source", zidata="group_source"
#elif defined(GROUP_RTOS)
    #pragma arm section rwdata="group_rtos", zidata="group_rtos"
#elif defined(GROUP_DRIVERS)
    #pragma arm section rwdata="group_drivers", zidata="group_drivers"
#elif defined(GROUP_BOOT)
    #pragma arm section rwdata="group_boot", zidata="group_boot"
#elif defined(GROUP_TASKS)
    #pragma arm section rwdata="group_tasks", zidata="group_tasks"
#elif defined(GROUP_LIB)
    #pragma arm section rwdata="group_lib", zidata="group_lib"
#elif defined(GROUP_BLE_HOST)
    #pragma arm section rwdata="group_ble_host", zidata="group_ble_host"
#elif defined(GROUP_LWIP)
    #pragma arm section rwdata="group_lwip", zidata="group_lwip"
#elif defined(GROUP_SEC_DRIVER)
    #pragma arm section rwdata="group_sec_driver", zidata="group_sec_driver"
#elif defined(GROUP_WPAS)
    #pragma arm section rwdata="group_wpas", zidata="group_wpas"
#elif defined(GROUP_TINYCRYPT)
    #pragma arm section rwdata="group_tinycrypt", zidata="group_tinycrypt"
#elif defined(GROUP_MBEDTLS)
    #pragma arm section rwdata="group_mbedtls", zidata="group_mbedtls"
#elif defined(GROUP_WIFI)
    #pragma arm section rwdata="group_wifi", zidata="group_wifi"
#elif defined(GROUP_BLE_CTRL)
    #pragma arm section rwdata="group_ble_ctrl", zidata="group_ble_ctrl"
#elif defined(GROUP_WIFI_CTRL)
    #pragma arm section rwdata="group_wifi_ctrl", zidata="group_wifi_ctrl"
#elif defined(GROUP_AT)
    #pragma arm section rwdata="group_at", zidata="group_at"
#elif defined(GROUP_IPERF)
    #pragma arm section rwdata="group_iperf", zidata="group_iperf"
#elif defined(GROUP_PS)
    #pragma arm section rwdata="group_ps", zidata="group_ps"
#elif defined(GROUP_LE_PTS_APP)
    #pragma arm section rwdata="group_le_pts_app", zidata="group_le_pts_app"
#elif defined(GROUP_LE_MTC_APP)
    #pragma arm section rwdata="group_le_mtc_app", zidata="group_le_mtc_app"
#elif defined(GROUP_LE_CMD_APP)
    #pragma arm section rwdata="group_le_cmd_app", zidata="group_le_cmd_app"
#elif defined(GROUP_MW_FIM)
    #pragma arm section rwdata="group_mw_fim", zidata="group_mw_fim"
#elif defined(GROUP_MW_OTA)
    #pragma arm section rwdata="group_mw_ota", zidata="group_mw_ota"
#else
	#pragma arm section rwdata="misc", zidata="misc"
#endif
