#ifndef _INFRA_CONFIG_H_
#define _INFRA_CONFIG_H_

#ifndef GCC
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
#endif

#define PLATFORM_HAS_STDINT
#define PLATFORM_HAS_DYNMEM
#define PLATFORM_HAS_OS
#define INFRA_STRING
#define INFRA_NET
#define INFRA_LIST
#define INFRA_TIMER
#define INFRA_JSON_PARSER
#define INFRA_CJSON
#define INFRA_MD5
#define INFRA_SHA1
#define INFRA_SHA256
#define INFRA_REPORT
#define INFRA_COMPAT
#define INFRA_CLASSIC
#define DEV_SIGN
#define MQTT_COMM_ENABLED
#define MQTT_DEFAULT_IMPL
#define MQTT_DIRECT
#define DEVICE_MODEL_CLASSIC
#define LOG_REPORT_TO_CLOUD
#define DEVICE_MODEL_ENABLED
#define HAL_KV
#define HAL_CRYPTO
#define HAL_UDP
#define COAP_PACKET
#define COAP_SERVER
#define DEV_BIND_ENABLED
#define WIFI_PROVISION_ENABLED
#define AWSS_SUPPORT_DEV_BIND_STATIS
#define DM_MESSAGE_CACHE_DISABLED
#define AWSS_DISABLE_REGISTRAR
#define AWSS_SUPPORT_STATIS

// modified for ali_lib by Jeff, 20190525
// redefine the HAL_Printf function
#if 1   // the modified
#define HAL_Printf  printf
#else   // the original
#endif

#endif

