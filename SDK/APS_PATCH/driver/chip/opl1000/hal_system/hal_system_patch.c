/* *****************************************************************************
 *  Copyright 2017 - 2018, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2018
 *
 *******************************************************************************
 * @file hal_system_patch.c
 * 
 * @brief Patch for System API
 *  
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "hal_system_patch.h"
#include "hal_pin.h"
#include "hal_pin_config_patch.h"
#include "hal_dbg_uart_patch.h"
#include "hal_spi_patch.h"
#include "hal_i2c.h"
#include "hal_wdt.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define RF_BASE         (MSQ_PERIPH_BASE + 0x9000)

#define AOS             ((S_Aos_Reg_t *) AOS_BASE)
#define SYS_REG         ((S_Sys_Reg_t *) SYS_BASE)
#define RF              ((S_Rf_Reg_t *) RF_BASE)
#define OTP             ((S_Otp_Reg_t *) OTP_BASE)

#define RF_BASE             (MSQ_PERIPH_BASE + 0x9000)
#define P_RC_OSC_CAL_CTL    (volatile uint32_t *)(RF_BASE + 0xC0)
#define P_RC_OSC_CAL_OUTPUT (volatile uint32_t *)(RF_BASE + 0xC4)

#define SYS_SRAM_BYPASS_APS          (1<<2)
#define SYS_SRAM_BYPASS_SHARE        (1<<3)

#define SYS_SW_RESET_ALL             (1<<0)

#define SYS_SW_DBG_EN                (1<<2)

#define SYS_ECO_VER_SHIFT            20
#define SYS_ECO_VER_MASK             (0xFF<<SYS_ECO_VER_SHIFT)

#define AOS_STRAP_MODE_MAX           0xF
#define AOS_STRAP_MODE_MASK          0xF

#define AOS_RET_HW_SETTING           1
#define AOS_RET_FW_SETTING           0

#define AOS_SLP_MODE_EN              (1<<0)
#define AOS_SLP_MODE_TIMER           (0<<1)
#define AOS_SLP_MODE_DEEP            (1<<1)
#define AOS_SLP_MODE_MASK            (0x1<<1)
#define AOS_SLP_MODE_EARLY_WAKE_UP   (0x1<<2)

#define AOS_SLP_TIMER_MASK_L         0xFFFFFFFF
#define AOS_SLP_TIMER_MASK_H         0x1F
#define AOS_SLP_TIMER_MASK_MAX       ((uint64_t)(AOS_SLP_TIMER_MASK_H)<<32 | AOS_SLP_TIMER_MASK_L)


#define AOS_STRAP_MODE_OVERWRITE_EN  (1<<4)

#define AOS_RST_DELAY                30

#define AOS_APS_RST_RFM              (1<<0)
#define AOS_APS_RST_I2C              (1<<2)
#define AOS_APS_RST_PWM              (1<<3)
#define AOS_APS_RST_TMR_0            (1<<4)
#define AOS_APS_RST_TMR_1            (1<<5)
#define AOS_APS_RST_WDT              (1<<6)
#define AOS_APS_RST_VIC              (1<<7)
#define AOS_APS_RST_SYS_BUS          (1<<8)
#define AOS_APS_RST_SPI_0            (1<<9)
#define AOS_APS_RST_SPI_1            (1<<10)
#define AOS_APS_RST_SPI_2            (1<<11)
#define AOS_APS_RST_UART_0           (1<<12)
#define AOS_APS_RST_UART_1           (1<<13)
#define AOS_APS_RST_DBG_UART         (1<<14)
#define AOS_APS_RST_OTP              (1<<15)
#define AOS_APS_RST_DMA              (1<<16)
#define AOS_APS_RST_ROM              (1<<17)
#define AOS_APS_RST_RAM              (1<<18)
#define AOS_APS_RST_JTAG             (1<<19)
#define AOS_APS_RST_AHB_TO_APB       (1<<20)
#define AOS_APS_RST_BRIDGE_APS       (1<<21)
#define AOS_APS_RST_BRIDGE_MSQ       (1<<22)
#define AOS_APS_RST_CM3              (1<<23)
#define AOS_APS_RST_SCRT             (1<<24)
#define AOS_APS_RST_TAP              (1<<25)

#define AOS_MSQ_RST_ROM              (1<<0)
#define AOS_MSQ_RST_SDM_XTAL         (1<<1)
#define AOS_MSQ_RST_BRIDGE_MSQ       (1<<3)
#define AOS_MSQ_RST_BRIDGE_APS       (1<<4)
#define AOS_MSQ_RST_MAC              (1<<5)
#define AOS_MSQ_RST_PHY              (1<<6)
#define AOS_MSQ_RST_WDT              (1<<7)
#define AOS_MSQ_RST_VIC              (1<<8)
#define AOS_MSQ_RST_PHY_REG          (1<<9)
#define AOS_MSQ_RST_AOS              (1<<10)
#define AOS_MSQ_RST_CM0              (1<<11)
#define AOS_MSQ_RST_RFPHY            (1<<12)
#define AOS_MSQ_RST_SPI              (1<<13)
#define AOS_MSQ_RST_DBG_UART         (1<<14)
#define AOS_MSQ_RST_PROG_PU          (1<<15)

#define AOS_RET_RAM_MASK             0xFFFFF /* 20 bits*/
#define AOS_RET_RAM_TIMEOUT          0x500

#define AOS_MSQ_ROM_REMAP_ALL_EN     1

#define AOS_PWM_CLK_MASK             (0x1<<27)

#define MHZ                          1000000
#define CLK_RC                       XTAL
#define CLK_XTAL                     XTAL
#define CLK_RF                       (176*MHZ)

#define RC_CAL_PATTERN_LEN           12
#define RC_CAL_PATTERN_MSK           ( (1 << RC_CAL_PATTERN_LEN) - 1)

// AOS 0x134
#define AOS_APS_CLK_SRC_RC           0
#define AOS_APS_CLK_SRC_XTAL         1
#define AOS_APS_CLK_SRC_XTAL_X2      2
#define AOS_APS_CLK_SRC_176M_SWITCH  3
#define AOS_APS_CLK_SRC_MASK         0x3
#define AOS_APS_CLK_176M_SRC_XTAL_X4 ( ((uint32_t)0<<31) | (0<<29) )
#define AOS_APS_CLK_176M_SRC_DECI    ( ((uint32_t)0<<31) | (1<<29) )
#define AOS_APS_CLK_176M_SRC_1P2G    ( ((uint32_t)1<<31) | (0<<29) )
#define AOS_APS_CLK_176M_SRC_EXT     ( ((uint32_t)1<<31) | (1<<29) )
#define AOS_APS_CLK_176M_SRC_MASK    ( ((uint32_t)0x1<<31) | (0x1<<29) )
#define AOS_APS_CLK_EN_GLOBAL        (1<<4)
#define AOS_APS_CLK_EN_I2C_PCLK      (1<<5)
#define AOS_APS_CLK_EN_TMR_0_PCLK    (1<<6)
#define AOS_APS_CLK_EN_TMR_1_PCLK    (1<<7)
#define AOS_APS_CLK_EN_WDT_PCLK      (1<<8)
#define AOS_APS_CLK_EN_VIC_PCLK      (1<<9)
#define AOS_APS_CLK_EN_SPI_0_PCLK    (1<<10)
#define AOS_APS_CLK_EN_SPI_1_PCLK    (1<<11)
#define AOS_APS_CLK_EN_SPI_2_PCLK    (1<<12)
#define AOS_APS_CLK_EN_UART_0_PCLK   (1<<13)
#define AOS_APS_CLK_EN_UART_1_PCLK   (1<<14)
#define AOS_APS_CLK_EN_DBG_UART_PCLK (1<<15)
#define AOS_APS_CLK_EN_OTP_PCLK      (1<<16)
#define AOS_APS_CLK_EN_CM3_HCLK      (1<<17)
#define AOS_APS_CLK_EN_DMA_HCLK      (1<<18)
#define AOS_APS_CLK_EN_ROM_HCLK      (1<<19)
#define AOS_APS_CLK_EN_RAM_HCLK      (1<<20)
#define AOS_APS_CLK_EN_APB_HCLK      (1<<21)
#define AOS_APS_CLK_EN_APS_HCLK      (1<<22)
#define AOS_APS_CLK_EN_MSQ_HCLK      (1<<23)
#define AOS_APS_CLK_EN_SCRT_HCLK     (1<<24)
#define AOS_APS_CLK_EN_FCLK          (1<<25)
#define AOS_APS_CLK_EN_PWM_CLK       (1<<26)
#define AOS_APS_CLK_EN_JTAG_HCLK     (1<<28)
#define AOS_APS_CLK_EN_WDT_INTERNAL  (1<<30)
#define AOS_APS_CLK_DIV2             (1<<2)
#define AOS_APS_PCLK_DIV2            (1<<3)

// AOS 0x138
#define AOS_MSQ_SRC_RC               0
#define AOS_MSQ_SRC_XTAL             1
#define AOS_MSQ_SRC_XTAL_X2          2
#define AOS_MSQ_SRC_EXT              3
#define AOS_MSQ_CLK_SRC_MASK         0x3
#define AOS_MSQ_CLK_EN_GLOBAL        (1<<2)
#define AOS_MSQ_CLK_EN_ROM_HCLK      (1<<4)
#define AOS_MSQ_CLK_EN_RAM_HCLK      (1<<5)
#define AOS_MSQ_CLK_EN_MSQ_HCLK      (1<<7)
#define AOS_MSQ_CLK_EN_APS_HCLK      (1<<8)
#define AOS_MSQ_CLK_EN_MAC_HCLK      (1<<9)
#define AOS_MSQ_CLK_EN_PHY_HCLK      (1<<10)
#define AOS_MSQ_CLK_EN_PHY_REG_HCLK  (1<<11)
#define AOS_MSQ_CLK_EN_AOS_HCLK      (1<<12)
#define AOS_MSQ_CLK_EN_WDT_HCLK      (1<<13)
#define AOS_MSQ_CLK_EN_VIC_HCLK      (1<<14)
#define AOS_MSQ_CLK_EN_FCLK          (1<<15)
#define AOS_MSQ_CLK_EN_DCLK          (1<<16)
#define AOS_MSQ_CLK_EN_SCLK          (1<<17)
#define AOS_MSQ_CLK_EN_PU_HCLK       (1<<18)
#define AOS_MSQ_CLK_EN_CM0_HCLK      (1<<19)
#define AOS_MSQ_CLK_EN_WDT           (1<<20)
#define AOS_MSQ_CLK_EN_SPI           (1<<21)
#define AOS_MSQ_CLK_EN_UART          (1<<22)
#define AOS_MSQ_CLK_DIV2             (1<<3)

#define AOS_APS_CLK_DIV2_UNGATED     (1<<23)
#define AOS_APS_PCLK_DIV2_UNGATED    (1<<24)
#define AOS_MSQ_CLK_DIV2_UNGATED     (1<<25)

#define AOS_PHY_RF_TEST_SRC_S_22M    (0<<4)
#define AOS_PHY_RF_TEST_SRC_S_44M    (1<<4)
#define AOS_PHY_RF_TEST_SRC_Q_44M    (2<<4)
#define AOS_PHY_RF_TEST_SRC_RFMMD    (3<<4)
#define AOS_PHY_RF_TEST_SRC_MASK     (0x3<<4)
#define AOS_PHY_RF_CAP_SRC_S_044M    (0<<6)
#define AOS_PHY_RF_CAP_SRC_Q_176M    (1<<6)
#define AOS_PHY_RF_CAP_SRC_MASK      (0x3<<6)
#define AOS_PHY_CLK_S_EN             (1<<0)
#define AOS_PHY_CLK_Q_EN             (1<<1)
#define AOS_PHY_CLK_RFMMD_EN         (1<<2)
#define AOS_PHY_CLK_S_22M_TX_EN      (1<<8)
#define AOS_PHY_CLK_S_22M_RX_EN      (1<<9)
#define AOS_PHY_CLK_S_44M_TX_EN      (1<<10)
#define AOS_PHY_CLK_S_44M_RX_EN      (1<<11)
#define AOS_PHY_CLK_Q_176M_TX_EN     (1<<12)
#define AOS_PHY_CLK_Q_176M_RX_EN     (1<<13)
#define AOS_PHY_CLK_Q_088M_TX_EN     (1<<15)
#define AOS_PHY_CLK_Q_088M_RX_EN     (1<<16)
#define AOS_PHY_CLK_Q_044M_TX_EN     (1<<17)
#define AOS_PHY_CLK_Q_044M_RX_EN     (1<<18)
#define AOS_PHY_CLK_CAP_FIFO2SRAM_EN (1<<14)
#define AOS_PHY_CLK_CAP_DATA2FIFO_EN (1<<19)
#define AOS_PHY_CLK_RFMMD_INV        (1<<20)
#define AOS_PHY_CLK_Q_INV            (1<<21)
#define AOS_PHY_CLK_S_INV            (1<<22)
#define AOS_PHY_CLK_S_DIV2           (1<<3)

#define AOS_PHY_CLK_S_DIV2_UNGATED   (1<<23)

#define AOS_M3_MMFACTOR_MAX          0x10
#define AOS_M3_MMFACTOR_MASK         0x1F
#define AOS_M3_MMFACTOR_EN           (1<<5)

// AOS 0x28
#define AOS_PTAT_EN (1 << 17)

// 0x80
#define XTAL_32K_GM_POS   3
#define XTAL_32K_GM_MAX   16
#define XTAL_32K_GM_MASK  (0xF << XTAL_32K_GM_POS)
#define XTAL_32K_CAP_POS  11
#define XTAL_32K_CAP_MAX   16
#define XTAL_32K_CAP_MASK  (0xF << XTAL_32K_CAP_POS)

// RC calibration, in RF bank
// 0xC0
#define RC_CAL_RST (1 << 0)
#define RC_CAL_EN  (1 << 1)

// 0xC4
#define RC_CAL_XTAL_CNT_POS 0
#define RC_CAL_XTAL_CNT_MSK (0x3FFF << RC_CAL_XTAL_CNT_POS)
#define RC_CAL_RC_CNT_POS   16
#define RC_CAL_RC_CNT_MSK   (0x3FFF << RC_CAL_RC_CNT_POS)


#define WDT_TIMEOUT_SECS            10


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...
typedef struct
{
    volatile uint32_t RET_MUX;            // 0x000
    volatile uint32_t MODE_CTL;           // 0x004
    volatile uint32_t OSC_SEL;            // 0x008
    volatile uint32_t SLP_TIMER_CURR_L;   // 0x00C
    volatile uint32_t SLP_TIMER_CURR_H;   // 0x010
    volatile uint32_t SLP_TIMER_PRESET_L; // 0x014
    volatile uint32_t SLP_TIMER_PRESET_H; // 0x018
    volatile uint32_t PS_TIMER_PRESET;    // 0x01C
    volatile uint32_t RET_SF_VAL_CTL;     // 0x020
    volatile uint32_t PMU_SF_VAL_CTL;     // 0x024
    volatile uint32_t HPBG_CTL;           // 0x028
    volatile uint32_t LPBG_CTL;           // 0x02C
    volatile uint32_t BUCK_CTL;           // 0x030
    volatile uint32_t ON1_TIME;           // 0x034
    volatile uint32_t ON2_TIME;           // 0x038
    volatile uint32_t ON3_TIME;           // 0x03C
    volatile uint32_t ON4_TIME;           // 0x040
    volatile uint32_t ON5_TIME;           // 0x044
    volatile uint32_t ON6_TIME;           // 0x048
    volatile uint32_t ON7_TIME;           // 0x04C
    volatile uint32_t CPOR_N_ON_TIME;     // 0x050
    volatile uint32_t reserve_054;        // 0x054, reserved
    volatile uint32_t SPS_TIMER_PRESET;   // 0x058
    volatile uint32_t SON1_TIME;          // 0x05C
    volatile uint32_t SON2_TIME;          // 0x060
    volatile uint32_t SON3_TIME;          // 0x064
    volatile uint32_t SON4_TIME;          // 0x068
    volatile uint32_t SON5_TIME;          // 0x06C
    volatile uint32_t SON6_TIME;          // 0x070
    volatile uint32_t SON7_TIME;          // 0x074
    volatile uint32_t SCPOR_N_ON_TIME;    // 0x078
    volatile uint32_t PU_CTL;             // 0x07C
    volatile uint32_t OSC_CTL;            // 0x080
    volatile uint32_t PMS_SPARE;          // 0x084, HW reservd for debug
    volatile uint32_t ADC_CTL;            // 0x088
    volatile uint32_t LDO_CTL;            // 0x08C
    volatile uint32_t RG_PD_IE;           // 0x090
    volatile uint32_t RG_PD_PE;           // 0x094
    volatile uint32_t RG_PD_O_INV;        // 0x098
    volatile uint32_t RG_PD_DS;           // 0x09C
    volatile uint32_t RG_GPO;             // 0x0A0
    volatile uint32_t RG_PD_I_INV;        // 0x0A4
    volatile uint32_t RG_PDOV_MODE;       // 0x0A8
    volatile uint32_t RG_PD_DIR;          // 0x0AC
    volatile uint32_t RG_PD_OENP_INV;     // 0x0B0
    volatile uint32_t RG_PDOC_MODE;       // 0x0B4
    volatile uint32_t RG_GPI;             // 0x0B8
    volatile uint32_t reserve_0bc;        // 0x0BC, reserved
    volatile uint32_t RG_PDI_SRC_IO_A;    // 0x0C0
    volatile uint32_t RG_PDI_SRC_IO_B;    // 0x0C4
    volatile uint32_t RG_PDI_SRC_IO_C;    // 0x0C8
    volatile uint32_t RG_PDI_SRC_IO_D;    // 0x0CC
    volatile uint32_t RG_PTS_INMUX_A;     // 0x0D0
    volatile uint32_t RG_PTS_INMUX_B;     // 0x0D4
    volatile uint32_t RG_PTS_INMUX_C;     // 0x0D8
    volatile uint32_t RG_PTS_INMUX_D;     // 0x0DC
    volatile uint32_t RG_SRAM_IOS_EN;     // 0x0E0
    volatile uint32_t RG_SRAM_RET_OFF;    // 0x0E4
    volatile uint32_t RG_PHY_WR_SRAM;     // 0x0E8
    volatile uint32_t RG_PHY_RD_SRAM;     // 0x0EC
    volatile uint32_t CAL_CEN;            // 0x0F0
    volatile uint32_t CAL_STR;            // 0x0F4
    volatile uint32_t SDM_PT_SEL;         // 0x0F8
    volatile uint32_t SDM_CTL;            // 0x0FC
    volatile uint32_t R_STRAP_MODE_CTL;   // 0x100
    volatile uint32_t R_APS_SWRST;        // 0x104
    volatile uint32_t R_MSQ_SWRST;        // 0x108
    volatile uint32_t RG_SPARE;           // 0x10C
    volatile uint32_t RG_PTS_INMUX_E;     // 0x110
    volatile uint32_t RG_PTS_INMUX_F;     // 0x114
    volatile uint32_t RG_SRAM_RET_ACK;    // 0x118
    volatile uint32_t RG_MSQ_ROM_MAP;     // 0x11C
    volatile uint32_t RG_AOS_ID;          // 0x120
    volatile uint32_t RG_SPARE_1;         // 0x124
    volatile uint32_t RG_RSTS;            // 0x128
    volatile uint32_t RG_SPARE_2;         // 0x12C
    volatile uint32_t RG_SPARE_3;         // 0x130
    volatile uint32_t R_M3CLK_SEL;        // 0x134
    volatile uint32_t R_M0CLK_SEL;        // 0x138
    volatile uint32_t R_RFPHY_SEL;        // 0x13C
    volatile uint32_t R_SCRT_EN;          // 0x140
    volatile uint32_t reserve_144[21];    // 0x144 ~ 0x194, move to sys_reg
    volatile uint32_t R_CLK_MMFACTOR_CM3; // 0x198
} S_Aos_Reg_t;

typedef struct
{
    volatile uint32_t reserve_r0[3];         // 0x000 ~ 0x008, reserved
    volatile uint32_t R_SRAM_BYPASS;         // 0x00C
    volatile uint32_t R_SW_RESET_EN;         // 0x010
    volatile uint32_t R_SW_DBG_EN;           // 0x014
    volatile uint32_t R_BOOT_STATUS;         // 0x018
    volatile uint32_t R_CHIP_ID;             // 0x01C
    volatile uint32_t reserve_r1[12];        // 0x020 ~ 0x04C, reserved
    volatile uint32_t R_CM3_I_PATCH[128];    // 0x050 ~ 0x24C
    volatile uint32_t R_CM3_I_PATCH_ST;      // 0x250
    volatile uint32_t R_CM3_I_PATCH_EN[4];   // 0x254 ~ 0x260
    volatile uint32_t R_CM3_D_P_ADDR[4];     // 0x268 ~ 0x270
    volatile uint32_t R_CM3_D_PATCH_EN;      // 0x274
    volatile uint32_t R_CM3_D_PATCH_DATA[4]; // 0x278 ~ 0x284
    volatile uint32_t reserve_r2[6];         // 0x288 ~ 0x29C, reserved
    volatile uint32_t R_CM0_I_PATCH[128];    // 0x2A0 ~ 0x49C
    volatile uint32_t R_CM0_I_PATCH_ST;      // 0x4A0
    volatile uint32_t R_CM0_I_PATCH_EN[4];   // 0x4A4 ~ 0x4B0
    volatile uint32_t R_CM3_ORIG_ADD[4];     // 0x4B4 ~ 0x4C0
    volatile uint32_t R_CM3_TAG_ADD[4];      // 0x4C4 ~ 0x4D0
    volatile uint32_t R_CM3_RMP_MASK[4];     // 0x4D4 ~ 0x4E0
    volatile uint32_t R_CM0_ORIG_ADD[3];     // 0x4E4 ~ 0x4EC
    volatile uint32_t R_CM0_TAG_ADD[3];      // 0x4F0 ~ 0x4F8
    volatile uint32_t R_CM0_RMP_MASK[3];     // 0x4FC ~ 0x504
} S_Sys_Reg_t;

typedef struct
{
    volatile uint32_t reserve_000[2];     // 0x000 ~ 0x004, reserved
    volatile uint32_t RG_SPARE;           // 0x008
    volatile uint32_t reserve_00C[15];    // 0x00C ~ 0x044, reserved
    volatile uint32_t RG_CK_GATE_CTRL;    // 0x048
    volatile uint32_t reserve_04C[4];     // 0x04C ~ 0x058, reserved
    volatile uint32_t AUXADC_CTRL0;       // 0x05C
    volatile uint32_t reserve_060[1];     // 0x060 ~ 0x060, reserved
    volatile uint32_t RG_AUX_IN_SEL;      // 0x064
    volatile uint32_t reserve_068[10];    // 0x068 ~ 0x08C, reserved
    volatile uint32_t PU_VAL;             // 0x090
    volatile uint32_t reserve_094[14];    // 0x094 ~ 0x0C8, reserved
    volatile uint32_t AUX_ADC_CK_GEN_CTL; // 0x0CC
    volatile uint32_t RG_AUX_ADC_ECL_OUT; // 0x0D0
} S_Rf_Reg_t;

typedef struct
{
    uint32_t DATA[128]; // Offset: 0x000 ~ 0x1FC (R/ ) OTP data                                                                 */
} S_Otp_Reg_t;

/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */

/*
*************************************************************************
*                           Declarations of Private Functions
*************************************************************************
*/
void Hal_SysPinMuxAppInit_patch(void);
uint32_t Hal_Sys_RetRamTurnOff_patch(uint32_t u32RetRamIdxs);
void Hal_Sys_ApsClkChangeApply_patch(void);
uint32_t Hal_Sys_SwResetAll_patch(void);

uint32_t Hal_Sys_ApsClkTreeSetup_impl(E_ApsClkTreeSrc_t eClkTreeSrc, uint8_t u8ClkDivEn, uint8_t u8PclkDivEn );
uint32_t Hal_Sys_ApsClkTreeSetup_patch(E_ApsClkTreeSrc_t eClkTreeSrc, uint8_t u8ClkDivEn, uint8_t u8PclkDivEn );
/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */


/*
 *************************************************************************
 *                          Private Variables
 *************************************************************************
 */
extern uint8_t g_u8PclkDivEn_Curr;
extern E_ApsClkTreeSrc_t g_eClkTreeSrc_Curr;
/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */

void Hal_Sys_PatchInit(void)
{
    /* Clock relative */
    Hal_Sys_ApsClkChangeApply = Hal_Sys_ApsClkChangeApply_patch;
    Hal_Sys_ApsClkTreeSetup = Hal_Sys_ApsClkTreeSetup_patch;
    
    /* Pin-Mux relative*/
    Hal_SysPinMuxAppInit = Hal_SysPinMuxAppInit_patch;

    /* Ret RAM relative*/
    Hal_Sys_RetRamTurnOff = Hal_Sys_RetRamTurnOff_patch;
    
    /* SW reset relative */
    Hal_Sys_SwResetAll = Hal_Sys_SwResetAll_patch;
}

/**
 * @brief Get APS PCLK frequency
 * @return PCLK frequency
 */
uint32_t Hal_Sys_ApsPclkGet(void)
{
    if (AOS->R_M3CLK_SEL & AOS_APS_PCLK_DIV2)
        return SystemCoreClockGet() >> 1;
    return SystemCoreClockGet();
}


/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */
/*************************************************************************
* FUNCTION:
*  Hal_Sys_ApsClkTreeSetup
*
* DESCRIPTION:
*   1. APS(M3) clock-tree setup. It contain a source select and two divider in clock-tree
*       [SRC]---CLK DIV2---+---PCLK DIV2--->PCLK
*                          +--->Other CLKs
*   2. Related reg.: AOS 0x134 and 0x138
* CALLS
*
* PARAMETERS
*   1. eClkTreeSrc : Select the source of clock tree. Refer to E_ApsClkTreeSrc_t
*   2. u8ClkDivEn  : 1 for Enable/0 for Disable the CLK divider
*   3. u8PclkDivEn : 1 for Enable/0 for Disable the PCLK divider
* RETURNS
*   0: setting complete
*   1: error
* GLOBALS AFFECTED
*
*************************************************************************/
uint32_t Hal_Sys_ApsClkTreeSetup_patch(E_ApsClkTreeSrc_t eClkTreeSrc, uint8_t u8ClkDivEn, uint8_t u8PclkDivEn )
{
    volatile E_ApsClkTreeSrc_t eClkTreeSrc_prev = g_eClkTreeSrc_Curr;
    uint32_t u32Ret;
    
    u32Ret = Hal_Sys_ApsClkTreeSetup_impl(eClkTreeSrc, u8ClkDivEn, u8PclkDivEn);
    
    /* When clock switch from 1p2g to non-1p2g, set the clock MUX 176M source to XTAL_X4 */
    if ((eClkTreeSrc_prev > APS_CLKTREE_SRC_XTAL_X2) &&
        (g_eClkTreeSrc_Curr <= APS_CLKTREE_SRC_XTAL_X2) )
    {
        AOS->R_M3CLK_SEL = (AOS->R_M3CLK_SEL & ~AOS_APS_CLK_176M_SRC_MASK) | AOS_APS_CLK_176M_SRC_XTAL_X4;
    }
    return u32Ret;
}

/*************************************************************************
* FUNCTION:
*  Hal_SysPinMuxAppInit
*
* DESCRIPTION:
*   1. Pin-Mux initial for application stage
*   2. Related reg.: AOS 0x090 ~ 0x0DC
* CALLS
*
* PARAMETERS
*   None
* RETURNS
*   None
* GLOBALS AFFECTED
*
*************************************************************************/
void Hal_SysPinMuxAppInit_patch(void)
{
    Hal_Pin_ConfigSet(0, HAL_PIN_TYPE_PATCH_IO_0, HAL_PIN_DRIVING_PATCH_IO_0);
    Hal_Pin_ConfigSet(1, HAL_PIN_TYPE_PATCH_IO_1, HAL_PIN_DRIVING_PATCH_IO_1);
    Hal_Pin_ConfigSet(2, HAL_PIN_TYPE_PATCH_IO_2, HAL_PIN_DRIVING_PATCH_IO_2);
    Hal_Pin_ConfigSet(3, HAL_PIN_TYPE_PATCH_IO_3, HAL_PIN_DRIVING_PATCH_IO_3);
    Hal_Pin_ConfigSet(4, HAL_PIN_TYPE_PATCH_IO_4, HAL_PIN_DRIVING_PATCH_IO_4);
    Hal_Pin_ConfigSet(5, HAL_PIN_TYPE_PATCH_IO_5, HAL_PIN_DRIVING_PATCH_IO_5);
    Hal_Pin_ConfigSet(6, HAL_PIN_TYPE_PATCH_IO_6, HAL_PIN_DRIVING_PATCH_IO_6);
    Hal_Pin_ConfigSet(7, HAL_PIN_TYPE_PATCH_IO_7, HAL_PIN_DRIVING_PATCH_IO_7);
    Hal_Pin_ConfigSet(8, HAL_PIN_TYPE_PATCH_IO_8, HAL_PIN_DRIVING_PATCH_IO_8);
    Hal_Pin_ConfigSet(9, HAL_PIN_TYPE_PATCH_IO_9, HAL_PIN_DRIVING_PATCH_IO_9);
    Hal_Pin_ConfigSet(10, HAL_PIN_TYPE_PATCH_IO_10, HAL_PIN_DRIVING_PATCH_IO_10);
    Hal_Pin_ConfigSet(11, HAL_PIN_TYPE_PATCH_IO_11, HAL_PIN_DRIVING_PATCH_IO_11);
    Hal_Pin_ConfigSet(12, HAL_PIN_TYPE_PATCH_IO_12, HAL_PIN_DRIVING_PATCH_IO_12);
    Hal_Pin_ConfigSet(13, HAL_PIN_TYPE_PATCH_IO_13, HAL_PIN_DRIVING_PATCH_IO_13);
    Hal_Pin_ConfigSet(14, HAL_PIN_TYPE_PATCH_IO_14, HAL_PIN_DRIVING_PATCH_IO_14);
    Hal_Pin_ConfigSet(15, HAL_PIN_TYPE_PATCH_IO_15, HAL_PIN_DRIVING_PATCH_IO_15);
    Hal_Pin_ConfigSet(16, HAL_PIN_TYPE_PATCH_IO_16, HAL_PIN_DRIVING_PATCH_IO_16);
    Hal_Pin_ConfigSet(17, HAL_PIN_TYPE_PATCH_IO_17, HAL_PIN_DRIVING_PATCH_IO_17);
    Hal_Pin_ConfigSet(18, HAL_PIN_TYPE_PATCH_IO_18, HAL_PIN_DRIVING_PATCH_IO_18);
    Hal_Pin_ConfigSet(19, HAL_PIN_TYPE_PATCH_IO_19, HAL_PIN_DRIVING_PATCH_IO_19);
    Hal_Pin_ConfigSet(20, HAL_PIN_TYPE_PATCH_IO_20, HAL_PIN_DRIVING_PATCH_IO_20);
    Hal_Pin_ConfigSet(21, HAL_PIN_TYPE_PATCH_IO_21, HAL_PIN_DRIVING_PATCH_IO_21);
    Hal_Pin_ConfigSet(22, HAL_PIN_TYPE_PATCH_IO_22, HAL_PIN_DRIVING_PATCH_IO_22);
    Hal_Pin_ConfigSet(23, HAL_PIN_TYPE_PATCH_IO_23, HAL_PIN_DRIVING_PATCH_IO_23);
}

/*************************************************************************
* FUNCTION:
*  Hal_Sys_RetRamTurnOff
*
* DESCRIPTION:
*   1. Turn off the given RetRam blocks. Ref. to E_RetRamIdx_t
*   2. Related reg.: AOS 0x0E0 ,0x0E4 and 0x118
* CALLS
*
* PARAMETERS
*   1. u32RetRamIdxs : Multi index combined. index refer to E_RetRamIdx_t
* RETURNS
*   0: setting complete
*   1: error 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Sys_RetRamTurnOff_patch(uint32_t u32RetRamIdxs)
{
    // Check Indexes are all in legal region
    if( u32RetRamIdxs & (~AOS_RET_RAM_MASK) )
        return 1;

    // Isolated
    AOS->RG_SRAM_IOS_EN |= u32RetRamIdxs;

    // Power off
    AOS->RG_SRAM_RET_OFF |= u32RetRamIdxs;

    // Chech status
    while( ( Hal_Sys_RetRamStatusRead() & u32RetRamIdxs ) != 0);

    return 0;
}


/*************************************************************************
* FUNCTION:
*  Hal_Sys_ApsClkChangeApply
*
* DESCRIPTION:
*   1. Update all system clock relative
* CALLS
*
* PARAMETERS
*   None
* RETURNS
*   None
* GLOBALS AFFECTED
*
*************************************************************************/
void Hal_Sys_ApsClkChangeApply_patch(void)
{
    // FreeRTOS, update system tick.
    // FIXME: Need used some define...
    SysTick->LOAD =( SystemCoreClockGet()/1000 ) - 1;

    // Modules (ex. I2c, SPI, PWM...)
    // debug UART
    if (AOS->R_M3CLK_SEL & AOS_APS_CLK_EN_DBG_UART_PCLK)
        Hal_DbgUart_DividerUpdate();

    // SPI
    if (AOS->R_M3CLK_SEL & AOS_APS_CLK_EN_SPI_0_PCLK)
        Hal_Spi_DividerUpdate(SPI_IDX_0);
    if (AOS->R_M3CLK_SEL & AOS_APS_CLK_EN_SPI_1_PCLK)
        Hal_Spi_DividerUpdate(SPI_IDX_1);
    if (AOS->R_M3CLK_SEL & AOS_APS_CLK_EN_SPI_2_PCLK)
        Hal_Spi_DividerUpdate(SPI_IDX_2);
    
    // I2C
    if (AOS->R_M3CLK_SEL & AOS_APS_CLK_EN_I2C_PCLK)
        Hal_I2c_SpeedSet( Hal_I2c_SpeedGet() );
        
    // WDT
    if (AOS->R_M3CLK_SEL & AOS_APS_CLK_EN_WDT_PCLK)
        Hal_Wdt_Feed(WDT_TIMEOUT_SECS * Hal_Sys_ApsPclkGet());
}

/*************************************************************************
* FUNCTION:
*  Hal_Sys_SwResetAll
*
* DESCRIPTION:
*   1. Reset whole system
*   2. Related reg.: sys_reg 0x010
*   3. Before reset chip, it needs to switch APS clock to XTAL first
*      If APS clock is from 0b'11(XTAL_X4/1p2g/DECI/EXT), it will hang after reset because 
*      there is no clock to trigger glitch free clock mux.
* CALLS
*
* PARAMETERS
*   None
*
* RETURNS
*   0: setting complete
*   1: error
* GLOBALS AFFECTED
*
*************************************************************************/
uint32_t Hal_Sys_SwResetAll_patch(void)
{
    /* Switch to XTAL */
    AOS->R_M3CLK_SEL = (AOS->R_M3CLK_SEL & ~AOS_APS_CLK_SRC_MASK) | AOS_APS_CLK_SRC_XTAL;
    
    while (1)
    {   /* Wait switch clock finished */
        if ((AOS->R_M3CLK_SEL & AOS_APS_CLK_SRC_MASK) == AOS_APS_CLK_SRC_XTAL)
            break;
    }
    
    /* Trigger reset */
    SYS_REG->R_SW_RESET_EN |= SYS_SW_RESET_ALL;
    return 0;
}
