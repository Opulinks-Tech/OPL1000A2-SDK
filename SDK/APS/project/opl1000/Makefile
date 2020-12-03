##### ARM GCC Toolchain Development #####

PROJECT = ../../..
FOLDER  = ../../targets/opl1000/Output-GCC

# Build with "SDK_LITE" option or not
LITE = 0

# Output file names
OUTPUT = opl1000_app_m3
ifeq ("$(LITE)", "1")
    OUTPUT = opl1000_app_m3_lite
endif

# Compiler & Linker
CROSS_COMPILE = arm-none-eabi-
CC      = $(CROSS_COMPILE)gcc
OBJDUMP = $(CROSS_COMPILE)objdump
OBJCOPY = $(CROSS_COMPILE)objcopy
AR      = $(CROSS_COMPILE)ar

############# include SRC ############
ifneq ("$(wildcard ./src.mk)", "")
    SRC_EXIST = 1
    include ./src.mk
else
    SRC_EXIST = 0
endif

#############  CFLAGS   ##############

# Options for specific architecture
DEFS  = -DGCC
ifeq ("$(LITE)", "1")
    DEFS += -DSDK_LITE
endif 
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
CFLAGS += -Xlinker "--just-symbols=../../../APS/targets/opl1000/object/symdefs_gcc.o"
CFLAGS += -Wno-implicit-function-declaration
CFLAGS += -Wno-int-conversion

LFLAGS  = -T link.ld
LFLAGS += -Wl,--gc-sections
LFLAGS += -Wl,--entry=__Patch_EntryPoint
LFLAGS += -Wl,-Map=$(FOLDER)/$(OUTPUT).map

#############   include    ##############
INCDIR  = -I ../../apps/le_app/cmd_app
INCDIR += -I ../../apps/le_app/mtc_app
INCDIR += -I ../../apps/le_app/pts_app
INCDIR += -I ../../apps/wifi_app
INCDIR += -I ../../driver/chip/opl1000/hal_auxadc
INCDIR += -I ../../driver/chip/opl1000/hal_dbg_uart
INCDIR += -I ../../driver/chip/opl1000/hal_dma
INCDIR += -I ../../driver/chip/opl1000/hal_i2c
INCDIR += -I ../../driver/chip/opl1000/hal_patch
INCDIR += -I ../../driver/chip/opl1000/hal_pwm
INCDIR += -I ../../driver/chip/opl1000/hal_spi
INCDIR += -I ../../driver/chip/opl1000/hal_system
INCDIR += -I ../../driver/chip/opl1000/hal_tick
INCDIR += -I ../../driver/chip/opl1000/hal_tmr
INCDIR += -I ../../driver/chip/opl1000/hal_uart
INCDIR += -I ../../driver/chip/opl1000/hal_vic
INCDIR += -I ../../driver/chip/opl1000/hal_wdt
INCDIR += -I ../../driver/chip/opl1000/securityipdriver
INCDIR += -I ../../driver/CMSIS/Device/opl1000/Include
INCDIR += -I ../../driver/CMSIS/Include
INCDIR += -I ../../FreeRtos/Source/include
INCDIR += -I ../../FreeRtos/Source/portable/GCC/ARM_CM3
INCDIR += -I ../../middleware/netlink/at
INCDIR += -I ../../middleware/netlink/ble_controller_layer/inc
INCDIR += -I ../../middleware/netlink/cli
INCDIR += -I ../../middleware/netlink/common/sys_api
INCDIR += -I ../../middleware/netlink/common/sys_api/rom_if
INCDIR += -I ../../middleware/netlink/common/sys_ctrl
INCDIR += -I ../../middleware/netlink/controller_task
INCDIR += -I ../../middleware/netlink/data_flow
INCDIR += -I ../../middleware/netlink/diag_task
INCDIR += -I ../../middleware/netlink/le_stack
INCDIR += -I ../../middleware/netlink/msg
INCDIR += -I ../../middleware/netlink/mw_fim
INCDIR += -I ../../middleware/netlink/mw_ota
INCDIR += -I ../../middleware/netlink/ps_task
INCDIR += -I ../../middleware/netlink/wifi_controller_layer
INCDIR += -I ../../middleware/netlink/wifi_controller_layer/rom_if
INCDIR += -I ../../middleware/netlink/wifi_mac
INCDIR += -I ../../middleware/netlink/wifi_mac/common
INCDIR += -I ../../middleware/netlink/wifi_mac/crypto
INCDIR += -I ../../middleware/netlink/wifi_mac/utils
INCDIR += -I ../../middleware/third_party/fatfs/src
INCDIR += -I ../../middleware/third_party/httpclient
INCDIR += -I ../../middleware/third_party/lwip-2.0.3
INCDIR += -I ../../middleware/third_party/lwip-2.0.3/lwip/src/include
INCDIR += -I ../../middleware/third_party/lwip-2.0.3/ports/freertos/include
INCDIR += -I ../../middleware/third_party/lwip-2.0.3/ports/freertos/netif
INCDIR += -I ../../middleware/third_party/lwip-2.0.3/ports/rom_if
INCDIR += -I ../../middleware/third_party/mbedtls/configs
INCDIR += -I ../../middleware/third_party/mbedtls/include/mbedtls
INCDIR += -I ../../middleware/third_party/mbedtls/port/include
INCDIR += -I ../../middleware/third_party/tinycrypt/include
INCDIR += -I ../../middleware/third_party/tinycrypt/include/tinycrypt
INCDIR += -I ../../middleware/third_party/wpa_supplicant-0.7.3/src/ap
INCDIR += -I ../../middleware/third_party/wpa_supplicant-0.7.3/src/common
INCDIR += -I ../../middleware/third_party/wpa_supplicant-0.7.3/src/crypto
INCDIR += -I ../../middleware/third_party/wpa_supplicant-0.7.3/src/drivers
INCDIR += -I ../../middleware/third_party/wpa_supplicant-0.7.3/src/eapol_auth
INCDIR += -I ../../middleware/third_party/wpa_supplicant-0.7.3/src/eapol_supp
INCDIR += -I ../../middleware/third_party/wpa_supplicant-0.7.3/src/eap_common
INCDIR += -I ../../middleware/third_party/wpa_supplicant-0.7.3/src/eap_peer
INCDIR += -I ../../middleware/third_party/wpa_supplicant-0.7.3/src/eap_server
INCDIR += -I ../../middleware/third_party/wpa_supplicant-0.7.3/src/l2_packet
INCDIR += -I ../../middleware/third_party/wpa_supplicant-0.7.3/src/radius
INCDIR += -I ../../middleware/third_party/wpa_supplicant-0.7.3/src/rsn_supp
INCDIR += -I ../../middleware/third_party/wpa_supplicant-0.7.3/src/tls
INCDIR += -I ../../middleware/third_party/wpa_supplicant-0.7.3/src/utils
INCDIR += -I ../../middleware/third_party/wpa_supplicant-0.7.3/src/wps
INCDIR += -I ../../middleware/third_party/wpa_supplicant-0.7.3/wpa_supplicant
INCDIR += -I ../../middleware/third_party/wpa_supplicant-0.7.3/wpa_supplicant/dbus
INCDIR += -I ../common
INCDIR += -I ./boot_sequence
INCDIR += -I ./include
INCDIR += -I ./startup
INCDIR += -I ../../../APS_PATCH/apps/le_app/cmd_app
INCDIR += -I ../../../APS_PATCH/apps/le_app/mtc_app
INCDIR += -I ../../../APS_PATCH/apps/le_app/pts_app
INCDIR += -I ../../../APS_PATCH/apps/wifi_app
INCDIR += -I ../../../APS_PATCH/driver/chip/opl1000/hal_auxadc
INCDIR += -I ../../../APS_PATCH/driver/chip/opl1000/hal_dbg_uart
INCDIR += -I ../../../APS_PATCH/driver/chip/opl1000/hal_dma
INCDIR += -I ../../../APS_PATCH/driver/chip/opl1000/hal_i2c
INCDIR += -I ../../../APS_PATCH/driver/chip/opl1000/hal_patch
INCDIR += -I ../../../APS_PATCH/driver/chip/opl1000/hal_pwm
INCDIR += -I ../../../APS_PATCH/driver/chip/opl1000/hal_spi
INCDIR += -I ../../../APS_PATCH/driver/chip/opl1000/hal_system
INCDIR += -I ../../../APS_PATCH/driver/chip/opl1000/hal_tick
INCDIR += -I ../../../APS_PATCH/driver/chip/opl1000/hal_tmr
INCDIR += -I ../../../APS_PATCH/driver/chip/opl1000/hal_uart
INCDIR += -I ../../../APS_PATCH/driver/chip/opl1000/hal_vic
INCDIR += -I ../../../APS_PATCH/driver/chip/opl1000/hal_wdt
INCDIR += -I ../../../APS_PATCH/driver/chip/opl1000/securityipdriver
INCDIR += -I ../../../APS_PATCH/driver/CMSIS/Device/opl1000/Include
INCDIR += -I ../../../APS_PATCH/driver/CMSIS/Include
INCDIR += -I ../../../APS_PATCH/FreeRtos/Source/include
INCDIR += -I ../../../APS_PATCH/FreeRtos/Source/portable/Keil/ARM_CM3
INCDIR += -I ../../../APS_PATCH/middleware/netlink/at
INCDIR += -I ../../../APS_PATCH/middleware/netlink/ble_controller_layer/inc
INCDIR += -I ../../../APS_PATCH/middleware/netlink/cli
INCDIR += -I ../../../APS_PATCH/middleware/netlink/common/sys_api
INCDIR += -I ../../../APS_PATCH/middleware/netlink/common/sys_api/rom_if
INCDIR += -I ../../../APS_PATCH/middleware/netlink/common/sys_ctrl
INCDIR += -I ../../../APS_PATCH/middleware/netlink/controller_task
INCDIR += -I ../../../APS_PATCH/middleware/netlink/data_flow
INCDIR += -I ../../../APS_PATCH/middleware/netlink/diag_task
INCDIR += -I ../../../APS_PATCH/middleware/netlink/le_stack
INCDIR += -I ../../../APS_PATCH/middleware/netlink/le_stack/patch
INCDIR += -I ../../../APS_PATCH/middleware/netlink/msg
INCDIR += -I ../../../APS_PATCH/middleware/netlink/mw_fim
INCDIR += -I ../../../APS_PATCH/middleware/netlink/mw_ota
INCDIR += -I ../../../APS_PATCH/middleware/netlink/ps_task
INCDIR += -I ../../../APS_PATCH/middleware/netlink/wifi_controller_layer
INCDIR += -I ../../../APS_PATCH/middleware/netlink/wifi_controller_layer/rom_if
INCDIR += -I ../../../APS_PATCH/middleware/netlink/wifi_mac
INCDIR += -I ../../../APS_PATCH/middleware/netlink/wifi_mac/common
INCDIR += -I ../../../APS_PATCH/middleware/netlink/wifi_mac/crypto
INCDIR += -I ../../../APS_PATCH/middleware/netlink/wifi_mac/utils
INCDIR += -I ../../../APS_PATCH/middleware/third_party/fatfs/src
INCDIR += -I ../../../APS_PATCH/middleware/third_party/httpclient
INCDIR += -I ../../../APS_PATCH/middleware/third_party/lwip-2.0.3
INCDIR += -I ../../../APS_PATCH/middleware/third_party/lwip-2.0.3/lwip/src/include
INCDIR += -I ../../../APS_PATCH/middleware/third_party/lwip-2.0.3/ports/freertos/include
INCDIR += -I ../../../APS_PATCH/middleware/third_party/lwip-2.0.3/ports/freertos/netif
INCDIR += -I ../../../APS_PATCH/middleware/third_party/lwip-2.0.3/ports/rom_if
INCDIR += -I ../../../APS_PATCH/middleware/third_party/mbedtls/configs
INCDIR += -I ../../../APS_PATCH/middleware/third_party/mbedtls/include/mbedtls
INCDIR += -I ../../../APS_PATCH/middleware/third_party/mbedtls/port/include
INCDIR += -I ../../../APS_PATCH/middleware/third_party/tinycrypt/include/tinycrypt
INCDIR += -I ../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/src/ap
INCDIR += -I ../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/src/common
INCDIR += -I ../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/src/crypto
INCDIR += -I ../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/src/drivers
INCDIR += -I ../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/src/eapol_auth
INCDIR += -I ../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/src/eapol_supp
INCDIR += -I ../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/src/eap_common
INCDIR += -I ../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/src/eap_peer
INCDIR += -I ../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/src/eap_server
INCDIR += -I ../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/src/l2_packet
INCDIR += -I ../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/src/radius
INCDIR += -I ../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/src/rsn_supp
INCDIR += -I ../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/src/tls
INCDIR += -I ../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/src/utils
INCDIR += -I ../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/src/wps
INCDIR += -I ../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/wpa_supplicant
INCDIR += -I ../../../APS_PATCH/middleware/third_party/wpa_supplicant-0.7.3/wpa_supplicant/dbus
INCDIR += -I ../../../APS_PATCH/project/common
INCDIR += -I ../../../APS_PATCH/project/opl1000/boot_sequence
INCDIR += -I ../../../APS_PATCH/project/opl1000/include
INCDIR += -I ../../../APS_PATCH/project/opl1000/startup
INCDIR += -I ../../../APS_PATCH/project/opl1000/lib
INCDIR += -I ../../../APS_PATCH/FreeRtos/Source


#############   source    ##############
M_SRC = ../../../APS_PATCH/project/opl1000/startup/main_patch.c

#############   build   ##############
ifeq ("$(SRC_EXIST)","1")
    all: lib
		@$(CC) -o $(FOLDER)/$(OUTPUT).elf $(LFLAGS) $(CFLAGS) $(INCDIR) $(M_SRC) $(FOLDER)/$(OUTPUT).a
		$(OBJCOPY) -O binary $(FOLDER)/$(OUTPUT).elf $(FOLDER)/$(OUTPUT).bin

    lib: cleanobj prebuild $(SRC)
		@$(AR) -r $(FOLDER)/$(OUTPUT).a $(SRC)

    %.o: %.c
		@$(CC) -c $(CFLAGS) $(INCDIR) $^ -o $@
else
    all:
		@$(CC) -o $(FOLDER)/$(OUTPUT).elf $(LFLAGS) $(CFLAGS) $(INCDIR) $(M_SRC) $(FOLDER)/$(OUTPUT).a
		$(OBJCOPY) -O binary $(FOLDER)/$(OUTPUT).elf $(FOLDER)/$(OUTPUT).bin
endif

prebuild:
	../../../scripts/SubWCRev.exe $(PROJECT) $(PROJECT)/APS/project/common/svn_rev.txt $(PROJECT)/APS/project/common/svn_rev.h

#############   clean   ##############  
clean:
ifeq ("$(SRC_EXIST)","1")
	@rm -f $(SRC)
	@rm -f ../../targets/opl1000/Output-GCC/*
else
	@rm -f ../../targets/opl1000/Output-GCC/*.bin
	@rm -f ../../targets/opl1000/Output-GCC/*.elf
endif

cleanobj:
ifeq ("$(SRC_EXIST)","1")
	@rm -f $(SRC)
endif
