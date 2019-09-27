##### ARM GCC Toolchain Development #####
FOLDER  = ./Output-GCC
LD_SCRIPT = ../../../../APS/project/opl1000/link.ld
PATCH_LIB = ../../../../APS/targets/opl1000/Output-GCC/opl1000_app_m3.a
# Output file names
OUTPUT = opl1000_app_m3

# Compiler & Linker
CROSS_COMPILE = arm-none-eabi-
CC      = $(CROSS_COMPILE)gcc
OBJDUMP = $(CROSS_COMPILE)objdump
OBJCOPY = $(CROSS_COMPILE)objcopy

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

#############   source    ##############
M_SRC = ./main_patch.c

#############   build   ##############
all:
	@mkdir $(FOLDER)
	@$(CC) -o $(FOLDER)/$(OUTPUT).elf $(LFLAGS) $(CFLAGS) $(INCDIR) $(M_SRC) $(PATCH_LIB)
	$(OBJCOPY) -O binary $(FOLDER)/$(OUTPUT).elf $(FOLDER)/$(OUTPUT).bin

#############   clean   ##############  
clean:
	@rm -rf $(FOLDER)