##### ARM GCC Toolchain Development #####
FOLDER  = ./Output-GCC
# Output file names
OUTPUT = opl1000_app_m3
ALI_OUTPUT = opl1000_ali_lib
OUTPUT_TRANSPARNET = opl1000_app_m3_transparent

LD_SCRIPT = ../../../../APS/project/opl1000/link.ld
PATCH_LIB = ../../../../APS/targets/opl1000/Output-GCC/opl1000_app_m3.a
PATCH_LIB += $(FOLDER)/$(ALI_OUTPUT).a


# Compiler & Linker
CROSS_COMPILE = arm-none-eabi-
CC      = $(CROSS_COMPILE)gcc
OBJDUMP = $(CROSS_COMPILE)objdump
OBJCOPY = $(CROSS_COMPILE)objcopy
AR      = $(CROSS_COMPILE)ar

#############  CFLAGS   ##############

# Options for specific architecture
DEFS  = -DGCC
DEFS += -DARMCM3
DEFS += -DOPL1000_APS
DEFS += -D__noSIMULATOR__
DEFS += -D__BLE__
DEFS += -D__LE_HOST_USE_CMD__
DEFS += -D__LWIP_TASK__
DEFS += -D__WPA_SUPPLICANT__
DEFS += -D__noHW_CRYPTO_ENGINE__
DEFS += -D__WIFI_MAC_TASK__
DEFS += -D__NL1000_An__
DEFS += -D__PMP_ENABLE__
DEFS += -D__PMP_REGION__
DEFS += -D__HEAP_REGION__
DEFS += -DnoLWIP_DYNAMIC_DEBUG_ENABLE
DEFS += -D__AT_CMD_TASK__
DEFS += -D__noRTL_SIMULATION__
DEFS += -D_ARMABI_PURE=
DEFS += -D__WIFI_AUTO_CONNECT__ 
DEFS += -DALI_BLE_WIFI_PROVISION

DEFS_TRANSPARENT = -D__BLEWIFI_TRANSPARENT__

CFLAGS  = $(DEFS)
CFLAGS += -mthumb
CFLAGS += -mcpu=cortex-m3
CFLAGS += -mlittle-endian
CFLAGS += -std=c99
CFLAGS += -Os
CFLAGS += -x none
CFLAGS += -include sys_common.h
CFLAGS += -nostartfiles
CFLAGS += -no-pie
CFLAGS += -fdata-sections
CFLAGS += -ffunction-sections
CFLAGS += -specs=nosys.specs
CFLAGS += -Xlinker "--just-symbols=../../../../APS/targets/opl1000/object/symdefs_gcc.o"
CFLAGS += -Wno-implicit-function-declaration
CFLAGS += -Wno-int-conversion

LFLAGS  = -T $(LD_SCRIPT)
LFLAGS += -Wl,--gc-sections
LFLAGS += -Wl,--entry=__Patch_EntryPoint
LFLAGS += -Wl,-Map=$(FOLDER)/$(OUTPUT).map

#############   include    ##############
INCDIR  = -I ../../../../APS/apps/le_app/cmd_app
INCDIR += -I ../../../../APS/apps/le_app/mtc_app
INCDIR += -I ../../../../APS/apps/le_app/pts_app
INCDIR += -I ../../../../APS/apps/wifi_app
INCDIR += -I ../../../../APS/driver/chip/opl1000/hal_auxadc
INCDIR += -I ../../../../APS/driver/chip/opl1000/hal_dbg_uart
INCDIR += -I ../../../../APS/driver/chip/opl1000/hal_dma
INCDIR += -I ../../../../APS/driver/chip/opl1000/hal_i2c
INCDIR += -I ../../../../APS/driver/chip/opl1000/hal_patch
INCDIR += -I ../../../../APS/driver/chip/opl1000/hal_pwm
INCDIR += -I ../../../../APS/driver/chip/opl1000/hal_spi
INCDIR += -I ../../../../APS/driver/chip/opl1000/hal_system
INCDIR += -I ../../../../APS/driver/chip/opl1000/hal_tick
INCDIR += -I ../../../../APS/driver/chip/opl1000/hal_tmr
INCDIR += -I ../../../../APS/driver/chip/opl1000/hal_uart
INCDIR += -I ../../../../APS/driver/chip/opl1000/hal_vic
INCDIR += -I ../../../../APS/driver/chip/opl1000/hal_wdt
INCDIR += -I ../../../../APS/driver/chip/opl1000/securityipdriver
INCDIR += -I ../../../../APS/driver/CMSIS/Device/opl1000/Include
INCDIR += -I ../../../../APS/driver/CMSIS/Include
INCDIR += -I ../../../../APS/FreeRtos/Source/include
INCDIR += -I ../../../../APS/FreeRtos/Source/portable/GCC/ARM_CM3
INCDIR += -I ../../../../APS/middleware/netlink/at
INCDIR += -I ../../../../APS/middleware/netlink/ble_controller_layer/inc
INCDIR += -I ../../../../APS/middleware/netlink/cli
INCDIR += -I ../../../../APS/middleware/netlink/common/sys_api
INCDIR += -I ../../../../APS/middleware/netlink/common/sys_api/rom_if
INCDIR += -I ../../../../APS/middleware/netlink/common/sys_ctrl
INCDIR += -I ../../../../APS/middleware/netlink/controller_task
INCDIR += -I ../../../../APS/middleware/netlink/data_flow
INCDIR += -I ../../../../APS/middleware/netlink/diag_task
INCDIR += -I ../../../../APS/middleware/netlink/le_stack
INCDIR += -I ../../../../APS/middleware/netlink/msg
INCDIR += -I ../../../../APS/middleware/netlink/mw_fim
INCDIR += -I ../../../../APS/middleware/netlink/mw_ota
INCDIR += -I ../../../../APS/middleware/netlink/ps_task
INCDIR += -I ../../../../APS/middleware/netlink/wifi_controller_layer
INCDIR += -I ../../../../APS/middleware/netlink/wifi_controller_layer/rom_if
INCDIR += -I ../../../../APS/middleware/netlink/wifi_mac
INCDIR += -I ../../../../APS/middleware/netlink/wifi_mac/common
INCDIR += -I ../../../../APS/middleware/netlink/wifi_mac/crypto
INCDIR += -I ../../../../APS/middleware/netlink/wifi_mac/utils
INCDIR += -I ../../../../APS/middleware/third_party/fatfs/src
INCDIR += -I ../../../../APS/middleware/third_party/httpclient
INCDIR += -I ../../../../APS/middleware/third_party/lwip-2.0.3
INCDIR += -I ../../../../APS/middleware/third_party/lwip-2.0.3/lwip/src/include
INCDIR += -I ../../../../APS/middleware/third_party/lwip-2.0.3/lwip/src/include/lwip
INCDIR += -I ../../../../APS/middleware/third_party/lwip-2.0.3/ports/freertos/include
INCDIR += -I ../../../../APS/middleware/third_party/lwip-2.0.3/ports/freertos/netif
INCDIR += -I ../../../../APS/middleware/third_party/lwip-2.0.3/ports/rom_if
INCDIR += -I ../../../../APS/middleware/third_party/mbedtls/configs
INCDIR += -I ../../../../APS/middleware/third_party/mbedtls/include/mbedtls
INCDIR += -I ../../../../APS/middleware/third_party/mbedtls/port/include
INCDIR += -I ../../../../APS/middleware/third_party/tinycrypt/include
INCDIR += -I ../../../../APS/middleware/third_party/tinycrypt/include/tinycrypt
INCDIR += -I ../../../../APS/middleware/third_party/wpa_supplicant-0.7.3/src/ap
INCDIR += -I ../../../../APS/middleware/third_party/wpa_supplicant-0.7.3/src/common
INCDIR += -I ../../../../APS/middleware/third_party/wpa_supplicant-0.7.3/src/crypto
INCDIR += -I ../../../../APS/middleware/third_party/wpa_supplicant-0.7.3/src/drivers
INCDIR += -I ../../../../APS/middleware/third_party/wpa_supplicant-0.7.3/src/eapol_auth
INCDIR += -I ../../../../APS/middleware/third_party/wpa_supplicant-0.7.3/src/eapol_supp
INCDIR += -I ../../../../APS/middleware/third_party/wpa_supplicant-0.7.3/src/eap_common
INCDIR += -I ../../../../APS/middleware/third_party/wpa_supplicant-0.7.3/src/eap_peer
INCDIR += -I ../../../../APS/middleware/third_party/wpa_supplicant-0.7.3/src/eap_server
INCDIR += -I ../../../../APS/middleware/third_party/wpa_supplicant-0.7.3/src/l2_packet
INCDIR += -I ../../../../APS/middleware/third_party/wpa_supplicant-0.7.3/src/radius
INCDIR += -I ../../../../APS/middleware/third_party/wpa_supplicant-0.7.3/src/rsn_supp
INCDIR += -I ../../../../APS/middleware/third_party/wpa_supplicant-0.7.3/src/tls
INCDIR += -I ../../../../APS/middleware/third_party/wpa_supplicant-0.7.3/src/utils
INCDIR += -I ../../../../APS/middleware/third_party/wpa_supplicant-0.7.3/src/wps
INCDIR += -I ../../../../APS/middleware/third_party/wpa_supplicant-0.7.3/wpa_supplicant
INCDIR += -I ../../../../APS/middleware/third_party/wpa_supplicant-0.7.3/wpa_supplicant/dbus
INCDIR += -I ../../../../APS/project/common
INCDIR += -I ../../../../APS/project/opl1000/boot_sequence
INCDIR += -I ../../../../APS/project/opl1000/include
INCDIR += -I ../../../../APS/project/opl1000/startup
INCDIR += -I ../../../../APS_PATCH/apps/le_app/cmd_app
INCDIR += -I ../../../../APS_PATCH/apps/le_app/mtc_app
INCDIR += -I ../../../../APS_PATCH/apps/le_app/pts_app
INCDIR += -I ../../../../APS_PATCH/apps/wifi_app
INCDIR += -I ../../../../APS_PATCH/driver/chip/opl1000/hal_auxadc
INCDIR += -I ../../../../APS_PATCH/driver/chip/opl1000/hal_dbg_uart
INCDIR += -I ../../../../APS_PATCH/driver/chip/opl1000/hal_dma
INCDIR += -I ../../../../APS_PATCH/driver/chip/opl1000/hal_i2c
INCDIR += -I ../../../../APS_PATCH/driver/chip/opl1000/hal_patch
INCDIR += -I ../../../../APS_PATCH/driver/chip/opl1000/hal_pwm
INCDIR += -I ../../../../APS_PATCH/driver/chip/opl1000/hal_spi
INCDIR += -I ../../../../APS_PATCH/driver/chip/opl1000/hal_system
INCDIR += -I ../../../../APS_PATCH/driver/chip/opl1000/hal_tick
INCDIR += -I ../../../../APS_PATCH/driver/chip/opl1000/hal_tmr
INCDIR += -I ../../../../APS_PATCH/driver/chip/opl1000/hal_uart
INCDIR += -I ../../../../APS_PATCH/driver/chip/opl1000/hal_vic
INCDIR += -I ../../../../APS_PATCH/driver/chip/opl1000/hal_wdt
INCDIR += -I ../../../../APS_PATCH/driver/chip/opl1000/securityipdriver
INCDIR += -I ../../../../APS_PATCH/driver/CMSIS/Device/opl1000/Include
INCDIR += -I ../../../../APS_PATCH/driver/CMSIS/Include
INCDIR += -I ../../../../APS_PATCH/FreeRtos/Source/include
INCDIR += -I ../../../../APS_PATCH/FreeRtos/Source/portable/Keil/ARM_CM3
INCDIR += -I ../../../../APS_PATCH/middleware/netlink/at
INCDIR += -I ../../../../APS_PATCH/middleware/netlink/ble_controller_layer/inc
INCDIR += -I ../../../../APS_PATCH/middleware/netlink/cli
INCDIR += -I ../../../../APS_PATCH/middleware/netlink/common/sys_api
INCDIR += -I ../../../../APS_PATCH/middleware/netlink/common/sys_api/rom_if
INCDIR += -I ../../../../APS_PATCH/middleware/netlink/common/sys_ctrl
INCDIR += -I ../../../../APS_PATCH/middleware/netlink/controller_task
INCDIR += -I ../../../../APS_PATCH/middleware/netlink/data_flow
INCDIR += -I ../../../../APS_PATCH/middleware/netlink/diag_task
INCDIR += -I ../../../../APS_PATCH/middleware/netlink/le_stack
INCDIR += -I ../../../../APS_PATCH/middleware/netlink/le_stack/patch
INCDIR += -I ../../../../APS_PATCH/middleware/netlink/msg
INCDIR += -I ../../../../APS_PATCH/middleware/netlink/mw_fim
INCDIR += -I ../../../../APS_PATCH/middleware/netlink/mw_ota
INCDIR += -I ../../../../APS_PATCH/middleware/netlink/ps_task
INCDIR += -I ../../../../APS_PATCH/middleware/netlink/wifi_controller_layer
INCDIR += -I ../../../../APS_PATCH/middleware/netlink/wifi_controller_layer/rom_if
INCDIR += -I ../../../../APS_PATCH/middleware/netlink/wifi_mac
INCDIR += -I ../../../../APS_PATCH/middleware/netlink/wifi_mac/common
INCDIR += -I ../../../../APS_PATCH/middleware/netlink/wifi_mac/crypto
INCDIR += -I ../../../../APS_PATCH/middleware/netlink/wifi_mac/utils
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/fatfs/src
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/httpclient
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/lwip-2.0.3
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/lwip-2.0.3/lwip/src/include
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/lwip-2.0.3/ports/freertos/include
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/lwip-2.0.3/ports/freertos/netif
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/lwip-2.0.3/ports/rom_if
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/mbedtls/configs
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/mbedtls/include/mbedtls
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/mbedtls/port/include
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/tinycrypt/include/tinycrypt
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/src/ap
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/src/common
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/src/crypto
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/src/drivers
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/src/eapol_auth
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/src/eapol_supp
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/src/eap_common
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/src/eap_peer
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/src/eap_server
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/src/l2_packet
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/src/radius
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/src/rsn_supp
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/src/tls
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/src/utils
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/src/wps
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/wpa_supplicant
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/wpa_supplicant/dbus
INCDIR += -I ../../../../APS_PATCH/project/common
INCDIR += -I ../../../../APS_PATCH/project/opl1000/boot_sequence
INCDIR += -I ../../../../APS_PATCH/project/opl1000/include
INCDIR += -I ../../../../APS_PATCH/project/opl1000/startup
INCDIR += -I ../../../../APS_PATCH/project/opl1000/lib
INCDIR += -I ../../../../APS_PATCH/apps/le_app/include
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_bind/impl/os
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/infra
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/kv/include
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/kv/include/aos
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_sign
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/mqtt
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_model
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/wrappers
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_model/examples
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_bind/impl/awss_reset
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_bind
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/coap
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/coap/server
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/wifi_provision/frameworks/statics
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_model/client
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_model/server
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/coap/CoAPPacket
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_bind/impl
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/wifi_provision
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/wifi_provision/frameworks
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/wifi_provision/frameworks/aplist
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/wifi_provision/frameworks/utils
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/wifi_provision/frameworks/ieee80211
INCDIR += -I ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/wifi_provision/zero_config
INCDIR += -I ./src
INCDIR += -I ./src/iot_data
INCDIR += -I ./src/components
#INCDIR += -I ./src/breeze
INCDIR += -I ./src/breeze/api
INCDIR += -I ./src/breeze/include
INCDIR += -I ./src/breeze/hal/include

#############   ali_source_lib    ##############
ALI_SRC  = ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/coap/CoAPPacket/CoAPDeserialize_common.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/coap/CoAPPacket/CoAPMessage_common.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/coap/CoAPPacket/CoAPSerialize_common.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/coap/server/CoAPExport.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/coap/server/CoAPMessage.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/coap/server/CoAPNetwork.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/coap/server/CoAPObserve.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/coap/server/CoAPPlatform.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/coap/server/CoAPResource.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/coap/server/CoAPServer.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_bind/impl/awss_bind.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_bind/impl/awss_bind_statis.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_bind/impl/awss_cmp_coap.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_bind/impl/awss_cmp_mqtt.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_bind/impl/awss_event.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_bind/impl/awss_info.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_bind/impl/awss_notify.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_bind/impl/awss_packet.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_bind/impl/awss_report.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_bind/impl/awss_timer.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_bind/impl/passwd.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_bind/impl/awss_reset/awss_dev_reset.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_bind/impl/os/os_misc.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_sign/dev_sign_mqtt.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/infra/infra_cjson.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/infra/infra_compat.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/infra/infra_defs.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/infra/infra_json_parser.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/infra/infra_md5.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/infra/infra_net.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/infra/infra_report.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/infra/infra_sha1.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/infra/infra_sha256.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/infra/infra_string.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/infra/infra_timer.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/mqtt/mqtt_api.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/mqtt/impl/iotx_mqtt_client.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/mqtt/impl/MQTTConnectClient.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/mqtt/impl/MQTTDeserializePublish.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/mqtt/impl/MQTTPacket.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/mqtt/impl/MQTTSerializePublish.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/mqtt/impl/MQTTSubscribeClient.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/mqtt/impl/MQTTUnsubscribeClient.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/wrappers/os/freertos/HAL_OS_Freertos.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/wrappers/os/freertos/HAL_TCP_FreeRTOS.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/wrappers/os/freertos/HAL_UDP_FreeRTOS.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_model/dm_api.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_model/dm_ipc.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_model/dm_log_report.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_model/dm_manager.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_model/dm_message.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_model/dm_message_cache.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_model/dm_msg_process.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_model/dm_opt.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_model/dm_utils.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_model/impl_linkkit.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_model/iotx_cm.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_model/iotx_cm_coap.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_model/iotx_cm_mqtt.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_model/client/dm_client.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_model/client/dm_client_adapter.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/dev_model/examples/cJSON.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/kv/aos_freertos.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/kv/kvmgr.o
ALI_SRC += ../../../../APS_PATCH/middleware/third_party/aliyun_3.0.1/src/wifi_provision/frameworks/statics/awss_statis.o

#############   source    ##############
M_SRC  = ./src/main_patch.c
M_SRC += ./src/app_at_cmd.c
M_SRC += ./src/breeze/core/core.c
M_SRC += ./src/blewifi_ctrl.c
M_SRC += ./src/blewifi_server_app.c
M_SRC += ./src/blewifi_app.c
M_SRC += ./src/blewifi_ctrl_http_ota.c
M_SRC += ./src/blewifi_server_app_gatt.c
M_SRC += ./src/blewifi_ble_api.c
M_SRC += ./src/blewifi_data.c
M_SRC += ./src/blewifi_wifi_api.c
M_SRC += ./src/blewifi_common.c
M_SRC += ./src/blewifi_http_ota.c
M_SRC += ./src/mw_fim_default_group11_project.c
M_SRC += ./src/mw_fim_default_group12_project.c
M_SRC += ./src/iot_data/iot_data.c
M_SRC += ./src/iot_data/iot_rb_data.c
M_SRC += ./src/iot_data/ali_linkkitsdk_impl.c
M_SRC += ./src/breeze/core/auth.c
M_SRC += ./src/breeze/core/extcmd.c
M_SRC += ./src/breeze/core/sha256.c
M_SRC += ./src/breeze/core/transport.c
M_SRC += ./src/breeze/core/utils.c
M_SRC += ./src/breeze/core/ali_hal_impl.c
M_SRC += ./src/breeze/hal/ble/aes.c
M_SRC += ./src/breeze/hal/ble/aes_mbed.c
M_SRC += ./src/breeze/hal/ble/breeze_hal_os.c
M_SRC += ./src/breeze/hal/ble/breeze_hal_sec.c

#############   build   ##############
all: lib
	@$(CC) -o $(FOLDER)/$(OUTPUT).elf $(LFLAGS) $(CFLAGS) $(INCDIR) $(M_SRC) -lm $(PATCH_LIB)
	$(OBJCOPY) -O binary $(FOLDER)/$(OUTPUT).elf $(FOLDER)/$(OUTPUT).bin
	@$(CC) -o $(FOLDER)/$(OUTPUT_TRANSPARNET).elf $(LFLAGS) $(DEFS_TRANSPARENT) $(CFLAGS) $(INCDIR) $(M_SRC) -lm $(PATCH_LIB)
	$(OBJCOPY) -O binary $(FOLDER)/$(OUTPUT_TRANSPARNET).elf $(FOLDER)/$(OUTPUT_TRANSPARNET).bin

    lib: $(ALI_SRC)
	@mkdir $(FOLDER)
	$(AR) -r $(FOLDER)/$(ALI_OUTPUT).a $(ALI_SRC)

    %.o: %.c
	@$(CC) -c $(CFLAGS) $(INCDIR) $^ -o $@
    
#############   clean   ##############  
clean:
	@rm -f $(ALI_SRC)
	@rm -rf $(FOLDER)