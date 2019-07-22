/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

/*
 * Security IP low level test functions
 *
 * This program is used to do low level test for security IP functions.
 *
 * See README and COPYING for more details.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "msg.h"


#define BIT_0   0x00000001U
#define BIT_1   0x00000002U
#define BIT_2   0x00000004U
#define BIT_3   0x00000008U
#define BIT_4   0x00000010U
#define BIT_5   0x00000020U
#define BIT_6   0x00000040U
#define BIT_7   0x00000080U
#define BIT_8   0x00000100U
#define BIT_9   0x00000200U
#define BIT_10  0x00000400U
#define BIT_11  0x00000800U
#define BIT_12  0x00001000U
#define BIT_13  0x00002000U
#define BIT_14  0x00004000U
#define BIT_15  0x00008000U
#define BIT_16  0x00010000U
#define BIT_17  0x00020000U
#define BIT_18  0x00040000U
#define BIT_19  0x00080000U
#define BIT_20  0x00100000U
#define BIT_21  0x00200000U
#define BIT_22  0x00400000U
#define BIT_23  0x00800000U
#define BIT_24  0x01000000U
#define BIT_25  0x02000000U
#define BIT_26  0x04000000U
#define BIT_27  0x08000000U
#define BIT_28  0x10000000U
#define BIT_29  0x20000000U
#define BIT_30  0x40000000U
#define BIT_31  0x80000000U

#define EIP130_MAILBOX_IN_BASE                  0x0000
#define EIP130_MAILBOX_OUT_BASE                 0x0000
#define SCRT0_BASE_ADDR 0x60000000
#define SCRT1_BASE_ADDR 0x80000000
#define EIP130_REGISTEROFFSET_MAILBOX_STAT      0x3F00
#define EIP130_REGISTEROFFSET_MAILBOX_CTRL      0x3F00
#define EIP130_REGISTEROFFSET_MAILBOX_RAWSTAT   0x3F04
#define EIP130_REGISTEROFFSET_MAILBOX_RESET     0x3F04
#define EIP130_REGISTEROFFSET_MAILBOX_LINKID    0x3F08
#define EIP130_REGISTEROFFSET_MAILBOX_OUTID     0x3F0C
#define EIP130_REGISTEROFFSET_MAILBOX_LOCKOUT   0x3F10

#define EIP130_REGISTEROFFSET_MODULE_STATUS     0x3FE0
#define EIP130_REGISTEROFFSET_EIP_OPTIONS2      0x3FF4
#define EIP130_REGISTEROFFSET_EIP_OPTIONS       0x3FF8
#define EIP130_REGISTEROFFSET_EIP_VERSION       0x3FFC

#define HAS_NOP_DELAY
#define ITP_SCRT_SRAM_BASE_ADDR 0x0041a000

#if 1
void delay(int times){
    int i;
    for(i=0; i<times; i++){
#ifdef HAS_NOP_DELAY
        __NOP();
#endif
    }
}
#endif
//Disable Non-ITP test codes
#if 0
void test_onrom_level1_scrt0_reg_access(void) //it pass in Firdous's simulation test
{
    unsigned int scrt0_base = 0x60000000;
    unsigned int scrt0_ctrl_addr = scrt0_base + EIP130_REGISTEROFFSET_MAILBOX_CTRL;
    unsigned int scrt0_stat_addr = scrt0_base + EIP130_REGISTEROFFSET_MAILBOX_STAT;
    unsigned int data;

    //write
    *(volatile unsigned int *)scrt0_ctrl_addr = 0x4444;

    //read
    data = *((volatile unsigned int *)scrt0_stat_addr);
}


void test_onrom_level1_scrt1_reg_access(void) //it pass in Firdous's simulation test
{
    unsigned int scrt1_base = 0x80000000;
    unsigned int scrt1_reg_scrt_ctrl_offset = 0x0;
    unsigned int scrt1_reg_scrt_ctrl_addr = scrt1_base + scrt1_reg_scrt_ctrl_offset;
    unsigned int data;

    //write
    *(volatile unsigned int *)scrt1_reg_scrt_ctrl_addr = 0x1;

    //read
    data = *((volatile unsigned int *)scrt1_reg_scrt_ctrl_addr);
}


void test_onrom_level1_scrt0_scrt1_cross_reg_access(void) //it pass in Firdous's simulation test
{
    unsigned int scrt0_base = 0x60000000;
    unsigned int scrt0_ctrl_addr = scrt0_base + EIP130_REGISTEROFFSET_MAILBOX_CTRL;
    unsigned int scrt0_stat_addr = scrt0_base + EIP130_REGISTEROFFSET_MAILBOX_STAT;
    unsigned int data;
    unsigned int scrt1_base = 0x80000000;
    unsigned int scrt1_reg_scrt_ctrl_offset = 0x0;
    unsigned int scrt1_reg_scrt_ctrl_addr = scrt1_base + scrt1_reg_scrt_ctrl_offset;
    unsigned int sram_addr = 0x10001000;

    //write
    *(volatile unsigned int *)scrt0_ctrl_addr = 0x4444;
    //write
    *(volatile unsigned int *)scrt1_reg_scrt_ctrl_addr = 0x1;

    //read
    data = *((volatile unsigned int *)scrt0_stat_addr);
    //read
    data = *((volatile unsigned int *)scrt1_reg_scrt_ctrl_addr);

}


void test_onrom_level1_sram_access(void) //it pass in Firdous's simulation test
{
    unsigned int sram_addr = 0x10001000;
    unsigned int data;

    //write
    *(volatile unsigned int *)sram_addr = 0xf9ab1357;

    //There's something wrong in bus, if we didn't do delay ticks here, the read operation fail
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    //read
    data = *((volatile unsigned int *)sram_addr);
}


void test_onrom_level2_scrt0_all_reg_access(void) //it pass in Firdous's simulation test
{
    unsigned int data;

    unsigned int scrt0_write_addr_1 = SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_CTRL;
    //unsigned int scrt0_write_addr_2 = SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_RESET; //Not sure how to use it, skip it temporaly

    unsigned int scrt0_read_addr_1 = SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_STAT;
    unsigned int scrt0_read_addr_2 = SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_RAWSTAT;
    unsigned int scrt0_read_addr_3 = SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_LINKID;
    unsigned int scrt0_read_addr_4 = SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_OUTID;
    unsigned int scrt0_read_addr_5 = SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_LOCKOUT;
    unsigned int scrt0_read_addr_6 = SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MODULE_STATUS;
    unsigned int scrt0_read_addr_7 = SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_EIP_OPTIONS2;
    unsigned int scrt0_read_addr_8 = SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_EIP_OPTIONS;
    unsigned int scrt0_read_addr_9 = SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_EIP_VERSION;


    /*
        write - EIP130_REGISTEROFFSET_MAILBOX_CTRL
        read - EIP130_REGISTEROFFSET_MAILBOX_STAT
      */
    //Mailbox 1
    *(volatile unsigned int *)scrt0_write_addr_1 = 0x4;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    data = *((volatile unsigned int *)scrt0_read_addr_1);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    //Mailbox 2
    *(volatile unsigned int *)scrt0_write_addr_1 = 0x40;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    data = *((volatile unsigned int *)scrt0_read_addr_1);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
#endif
    //Mailbox 3
    *(volatile unsigned int *)scrt0_write_addr_1 = 0x400;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    data = *((volatile unsigned int *)scrt0_read_addr_1);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    //Mailbox 4
    *(volatile unsigned int *)scrt0_write_addr_1 = 0x4000;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    data = *((volatile unsigned int *)scrt0_read_addr_1);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif

    /*
        write - EIP130_REGISTEROFFSET_MAILBOX_RESET
      */
    //Not sure how to use it, skip it temporaly
    //*(volatile unsigned int *)scrt0_write_addr_2 = 0x8; //Not sure how to use it, skip it temporaly


    /*
        read -
                 EIP130_REGISTEROFFSET_MAILBOX_RAWSTAT
                 EIP130_REGISTEROFFSET_MAILBOX_LINKID
                 EIP130_REGISTEROFFSET_MAILBOX_OUTID
                 EIP130_REGISTEROFFSET_MAILBOX_LOCKOUT
                 EIP130_REGISTEROFFSET_MODULE_STATUS
                 EIP130_REGISTEROFFSET_EIP_OPTIONS2
                 EIP130_REGISTEROFFSET_EIP_OPTIONS
                 EIP130_REGISTEROFFSET_EIP_VERSION
      */
    data = *((volatile unsigned int *)scrt0_read_addr_2);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    data = *((volatile unsigned int *)scrt0_read_addr_3);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    data = *((volatile unsigned int *)scrt0_read_addr_4);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    data = *((volatile unsigned int *)scrt0_read_addr_5);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    data = *((volatile unsigned int *)scrt0_read_addr_6);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    data = *((volatile unsigned int *)scrt0_read_addr_7);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    data = *((volatile unsigned int *)scrt0_read_addr_8);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    data = *((volatile unsigned int *)scrt0_read_addr_9);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
}


void test_onrom_level2_sram_block_write_read_compare(void) //it pass in Firdous's simulation test
{
    unsigned int sram_addr = 0x10006000;

    unsigned int w_1_addr = sram_addr + 0;
    unsigned int w_2_addr = sram_addr + 4;
    unsigned int w_3_addr = sram_addr + 8;
    unsigned int w_4_addr = sram_addr + 12;
    unsigned int w_5_addr = sram_addr + 16;
    unsigned int w_6_addr = sram_addr + 20;

    unsigned int w_1_data = 0x11223344;
    unsigned int w_2_data = 0x55667788;
    unsigned int w_3_data = 0x99aabbcc;
    unsigned int w_4_data = 0xddeef00f;
    unsigned int w_5_data = 0xf9ab1357;
    unsigned int w_6_data = 0x24689876;

    unsigned int r_1_data = 0;
    unsigned int r_2_data = 0;
    unsigned int r_3_data = 0;
    unsigned int r_4_data = 0;
    unsigned int r_5_data = 0;
    unsigned int r_6_data = 0;

    unsigned int val_pass = 0xf0f0f0f0;
    unsigned int val_fail = 0x48484848;
    //unsigned int result_addr = 0x10000000;
    unsigned int result_addr = 0x10002500;
    unsigned int result_data = 0;

    delay(300);

    //write
    *(volatile unsigned int *)w_1_addr = w_1_data;
    *(volatile unsigned int *)w_2_addr = w_2_data;
    *(volatile unsigned int *)w_3_addr = w_3_data;
    *(volatile unsigned int *)w_4_addr = w_4_data;
    *(volatile unsigned int *)w_5_addr = w_5_data;
    *(volatile unsigned int *)w_6_addr = w_6_data;

    delay(10);

    //read
    r_1_data = *((volatile unsigned int *)w_1_addr);
    r_2_data = *((volatile unsigned int *)w_2_addr);
    r_3_data = *((volatile unsigned int *)w_3_addr);
    r_4_data = *((volatile unsigned int *)w_4_addr);
    r_5_data = *((volatile unsigned int *)w_5_addr);
    r_6_data = *((volatile unsigned int *)w_6_addr);

    delay(10);

    //compare
    if ((r_1_data == w_1_data)
        &&(r_2_data == w_2_data)
        &&(r_3_data == w_3_data)
        &&(r_4_data == w_4_data)
        &&(r_5_data == w_5_data)
        &&(r_6_data == w_6_data))
    {
        *(volatile unsigned int *)result_addr = val_pass;
    } else {
        *(volatile unsigned int *)result_addr = val_fail;
    }

    delay(10);

    result_data = *((volatile unsigned int *)result_addr);

    *(volatile unsigned int *)0x10002100 = 0x10101010;

}


void test_onrom_level3_scrt0_aes_64bytes_with_delay(void) //it pass in Firdous's simulation test
                                                                        //There's bus problem  (James fix it)
                                                                        // Filename in Firdous' system: scrt_aes_cbc_nist_romcpu
{
    unsigned int data;
    unsigned int scrt0_write_addr_1 = 0x60003f00;//SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_CTRL;
    unsigned int scrt0_read_addr_1 = 0x60003f00;//SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_STAT;

    unsigned int sram_addr_base = 0x10000000;
    unsigned int sram_addr_write_1 = (sram_addr_base + 0x3000);

    unsigned int w_1_addr = (sram_addr_write_1 + 0);
    unsigned int w_2_addr = (sram_addr_write_1 + 4);
    unsigned int w_3_addr = (sram_addr_write_1 + 8);
    unsigned int w_4_addr = (sram_addr_write_1 + 12);
    unsigned int w_5_addr = (sram_addr_write_1 + 16);
    unsigned int w_6_addr = (sram_addr_write_1 + 20);
    unsigned int w_7_addr = (sram_addr_write_1 + 24);
    unsigned int w_8_addr = (sram_addr_write_1 + 28);
    unsigned int w_9_addr = (sram_addr_write_1 + 32);
    unsigned int w_10_addr = (sram_addr_write_1 + 36);
    unsigned int w_11_addr = (sram_addr_write_1 + 40);
    unsigned int w_12_addr = (sram_addr_write_1 + 44);
    unsigned int w_13_addr = (sram_addr_write_1 + 48);
    unsigned int w_14_addr = (sram_addr_write_1 + 52);
    unsigned int w_15_addr = (sram_addr_write_1 + 56);
    unsigned int w_16_addr = (sram_addr_write_1 + 60);

    unsigned int w_1_data = 0xe2bec16b;
    unsigned int w_2_data = 0x969f402e;
    unsigned int w_3_data = 0x117e3de9;
    unsigned int w_4_data = 0x2a179373;
    unsigned int w_5_data = 0x578a2dae;
    unsigned int w_6_data = 0x9cac031e;
    unsigned int w_7_data = 0xac6fb79e;
    unsigned int w_8_data = 0x518eaf45;
    unsigned int w_9_data = 0x461cc830;
    unsigned int w_10_data = 0x11e45ca3;
    unsigned int w_11_data = 0x19c1fbe5;
    unsigned int w_12_data = 0xef520a1a;
    unsigned int w_13_data = 0x45249ff6;
    unsigned int w_14_data = 0x179b4fdf;
    unsigned int w_15_data = 0x7b412bad;
    unsigned int w_16_data = 0x10376ce6;

    unsigned int c_1_data = 0xacab4976;
    unsigned int c_2_data = 0x46b21981;
    unsigned int c_3_data = 0x9b8ee9ce;
    unsigned int c_4_data = 0x7d19e912;
    unsigned int c_5_data = 0x9bcb8650;
    unsigned int c_6_data = 0xee197250;
    unsigned int c_7_data = 0x3a11db95;
    unsigned int c_8_data = 0xb2787691;
    unsigned int c_9_data = 0xb8d6be73;
    unsigned int c_10_data = 0x3b74c1e3;
    unsigned int c_11_data = 0x9ee61671;
    unsigned int c_12_data = 0x16952222;
    unsigned int c_13_data = 0xa1caf13f;
    unsigned int c_14_data = 0x09ac1f68;
    unsigned int c_15_data = 0x30ca0e12;
    unsigned int c_16_data = 0xa7e18675;

    unsigned int mask_b_1 = 0x00000001;
    unsigned int mask_b_2 = 0x00000001;

    unsigned int sram_addr_output_encrypted_data = (sram_addr_base + 0x200);

    unsigned int w_output_token_result_addr = 0x10000000;
    unsigned int r_output_token_data;
    unsigned int c_output_token_pass_data = 0xd82c;
    unsigned int w_output_token_pass_data = 0xf0f0f0f0;
    unsigned int w_output_token_fail_data = 0x48484848;

    unsigned int sram_addr_read_13 = (sram_addr_base + 0x240);

    unsigned int r_1_addr = (sram_addr_read_13 + 0);
    unsigned int r_2_addr = (sram_addr_read_13 + 4);
    unsigned int r_3_addr = (sram_addr_read_13 + 8);
    unsigned int r_4_addr = (sram_addr_read_13 + 12);
    unsigned int r_5_addr = (sram_addr_read_13 + 16);
    unsigned int r_6_addr = (sram_addr_read_13 + 20);
    unsigned int r_7_addr = (sram_addr_read_13 + 24);
    unsigned int r_8_addr = (sram_addr_read_13 + 28);
    unsigned int r_9_addr = (sram_addr_read_13 + 32);
    unsigned int r_10_addr = (sram_addr_read_13 + 36);
    unsigned int r_11_addr = (sram_addr_read_13 + 40);
    unsigned int r_12_addr = (sram_addr_read_13 + 44);
    unsigned int r_13_addr = (sram_addr_read_13 + 48);
    unsigned int r_14_addr = (sram_addr_read_13 + 52);
    unsigned int r_15_addr = (sram_addr_read_13 + 56);
    unsigned int r_16_addr = (sram_addr_read_13 + 60);

    unsigned int r_1_data = 0;
    unsigned int r_2_data = 0;
    unsigned int r_3_data = 0;
    unsigned int r_4_data = 0;
    unsigned int r_5_data = 0;
    unsigned int r_6_data = 0;
    unsigned int r_7_data = 0;
    unsigned int r_8_data = 0;
    unsigned int r_9_data = 0;
    unsigned int r_10_data = 0;
    unsigned int r_11_data = 0;
    unsigned int r_12_data = 0;
    unsigned int r_13_data = 0;
    unsigned int r_14_data = 0;
    unsigned int r_15_data = 0;
    unsigned int r_16_data = 0;

    unsigned int scrt0_read_addr_2 = (SCRT0_BASE_ADDR + 0x0008);

    unsigned int c2_1_data = 0xa1caf13f;
    unsigned int c2_2_data = 0x09ac1f68;
    unsigned int c2_3_data = 0x30ca0e12;
    unsigned int c2_4_data = 0xa7e18675;

    unsigned int w2_1_data = 0x0104d82c;
    unsigned int w2_2_data = 0x00000000;
    unsigned int w2_3_data = 0x00000040;
    unsigned int w2_4_data = 0x10000100;
    unsigned int w2_5_data = 0x00000000;
    unsigned int w2_6_data = 0x00000040;
    unsigned int w2_7_data = 0x10000200;
    unsigned int w2_8_data = 0x00000000;
    unsigned int w2_9_data = 0x00000044;
    unsigned int w2_10_data = 0x00000000;
    unsigned int w2_11_data = 0x00000000;
    unsigned int w2_12_data = 0x00018010;
    unsigned int w2_13_data = 0x00000000;
    unsigned int w2_14_data = 0x03020100;
    unsigned int w2_15_data = 0x07060504;
    unsigned int w2_16_data = 0x0b0a0908;
    unsigned int w2_17_data = 0x0f0e0d0c;
    unsigned int w2_18_data = 0x16157e2b;
    unsigned int w2_19_data = 0xa6d2ae28;
    unsigned int w2_20_data = 0x8815f7ab;
    unsigned int w2_21_data = 0x3c4fcf09;
    unsigned int w2_22_data = 0x00000000;
    unsigned int w2_23_data = 0x00000000;
    unsigned int w2_24_data = 0x00000000;
    unsigned int w2_25_data = 0x00000000;

    //Mailbox 1
    *(volatile unsigned int *)0x60003f00 = 0x4;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    //Mailbox 2
    *(volatile unsigned int *)0x60003f00 = 0x40;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    //Mailbox 3
    *(volatile unsigned int *)0x60003f00 = 0x400;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    //Mailbox 4
    *(volatile unsigned int *)0x60003f00 = 0x4000;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    //Mailbox 1 again
    *(volatile unsigned int *)0x60003f00 = 0x4;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    //Burst_write 64 bytes in SRAM @ 0x1000_0100, this is plain text data
    *(volatile unsigned int *)0x10003000 = 0xe2bec16b;
    *(volatile unsigned int *)0x10003004 = 0x969f402e;
    *(volatile unsigned int *)0x10003008 = 0x117e3de9;
    *(volatile unsigned int *)0x1000300c = 0x2a179373;
    *(volatile unsigned int *)0x10003010 = 0x578a2dae;
    *(volatile unsigned int *)0x10003014 = 0x9cac031e;
    *(volatile unsigned int *)0x10003018 = 0xac6fb79e;
    *(volatile unsigned int *)0x1000301c = 0x518eaf45;
    *(volatile unsigned int *)0x10003020 = 0x461cc830;
    *(volatile unsigned int *)0x10003024 = 0x11e45ca3;
    *(volatile unsigned int *)0x10003028 = 0x19c1fbe5;
    *(volatile unsigned int *)0x1000302c = 0xef520a1a;
    *(volatile unsigned int *)0x10003030 = 0x45249ff6;
    *(volatile unsigned int *)0x10003034 = 0x179b4fdf;
    *(volatile unsigned int *)0x10003038 = 0x7b412bad;
    *(volatile unsigned int *)0x1000303c = 0x10376ce6;

#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif

    //Polling -1
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
        __NOP();
    }

#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif

    //Burst write 100 bytes
    *(volatile unsigned int *)0x60000000 = 0x0104d82c;

#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    *(volatile unsigned int *)0x60000004 = 0x00000000;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    *(volatile unsigned int *)0x60000008 = 0x00000040;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    *(volatile unsigned int *)0x6000000c = 0x10003000;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    *(volatile unsigned int *)0x60000010 = 0x00000000;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    *(volatile unsigned int *)0x60000014 = 0x00000040;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    *(volatile unsigned int *)0x60000018 = 0x10004000;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    *(volatile unsigned int *)0x6000001c = 0x00000000;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    *(volatile unsigned int *)0x60000020 = 0x00000044;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    *(volatile unsigned int *)0x60000024 = 0x00000000;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    *(volatile unsigned int *)0x60000028 = 0x00000000;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    *(volatile unsigned int *)0x6000002c = 0x00018010;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    *(volatile unsigned int *)0x60000030 = 0x00000000;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    *(volatile unsigned int *)0x60000034 = 0x03020100;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    *(volatile unsigned int *)0x60000038 = 0x07060504;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    *(volatile unsigned int *)0x6000003c = 0x0b0a0908;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    *(volatile unsigned int *)0x60000040 = 0x0f0e0d0c;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    *(volatile unsigned int *)0x60000044 = 0x16157e2b;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    *(volatile unsigned int *)0x60000048 = 0xa6d2ae28;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    *(volatile unsigned int *)0x6000004c = 0x8815f7ab;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    *(volatile unsigned int *)0x60000050 = 0x3c4fcf09;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    *(volatile unsigned int *)0x60000054 = 0x00000000;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    *(volatile unsigned int *)0x60000058 = 0x00000000;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    *(volatile unsigned int *)0x6000005c = 0x00000000;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    *(volatile unsigned int *)0x60000060 = 0x00000000;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
        __NOP();
    }
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    //Check output token status
    r_output_token_data = *((volatile unsigned int *)0x60000000);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    if (r_output_token_data == c_output_token_pass_data) {
        *(volatile unsigned int *)0x10002500 = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)0x10002500 = w_output_token_fail_data;
    }
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    //Check block 64 bytes output encrypted data in SRAM
    r_1_data = *((volatile unsigned int *)0x10004000);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    r_2_data = *((volatile unsigned int *)0x10004004);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    r_3_data = *((volatile unsigned int *)0x10004008);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    r_4_data = *((volatile unsigned int *)0x1000400c);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    r_5_data = *((volatile unsigned int *)0x10004010);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    r_6_data = *((volatile unsigned int *)0x10004014);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    r_7_data = *((volatile unsigned int *)0x10004018);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    r_8_data = *((volatile unsigned int *)0x1000401c);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    r_9_data = *((volatile unsigned int *)0x10004020);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    r_10_data = *((volatile unsigned int *)0x10004024);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    r_11_data = *((volatile unsigned int *)0x10004028);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    r_12_data = *((volatile unsigned int *)0x1000402c);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    r_13_data = *((volatile unsigned int *)0x10004030);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    r_14_data = *((volatile unsigned int *)0x10004034);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    r_15_data = *((volatile unsigned int *)0x10004038);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    r_16_data = *((volatile unsigned int *)0x1000403c);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    if ((r_1_data == c_1_data)
        &&(r_1_data == c_2_data)
        &&(r_1_data == c_3_data)
        &&(r_1_data == c_4_data)
        &&(r_1_data == c_5_data)
        &&(r_1_data == c_6_data)
        &&(r_1_data == c_7_data)
        &&(r_1_data == c_8_data)
        &&(r_1_data == c_9_data)
        &&(r_1_data == c_10_data)
        &&(r_1_data == c_11_data)
        &&(r_1_data == c_12_data)
        &&(r_1_data == c_13_data)
        &&(r_1_data == c_14_data)
        &&(r_1_data == c_15_data)
        &&(r_1_data == c_16_data)
        ) {
        //compare pass
        *(volatile unsigned int *)0x10002500 = w_output_token_pass_data;
    } else {
        //compare fail
        *(volatile unsigned int *)0x10002500 = w_output_token_fail_data;
    }
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    //Check 13
    r_output_token_data = *((volatile unsigned int *)0x10004040);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    if (r_output_token_data == c_output_token_pass_data) {
        *(volatile unsigned int *)0x10002500 = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)0x10002500 = w_output_token_fail_data;
    }
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    //Check 16 bytes in offset 0x0008
    r_1_data = *((volatile unsigned int *)0x60000008);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    r_2_data = *((volatile unsigned int *)0x6000000c);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    r_3_data = *((volatile unsigned int *)0x60000010);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    r_4_data = *((volatile unsigned int *)0x60000014);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    if ( (r_1_data == c2_1_data)
         &&(r_1_data == c2_2_data)
         &&(r_1_data == c2_3_data)
         &&(r_1_data == c2_4_data)
         ){
        *(volatile unsigned int *)0x10002500 = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)0x10002500 = w_output_token_fail_data;
    }
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    //Write
    *(volatile unsigned int *)0x60003f00 = 0x2;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
}


void test_onrom_level4_scrt0_aes_64bytes_no_delay(void) //it pass in Firdous's simulation test
                                                                     //Note: Write 64 bytes data to do aes encryption
                                                                     //Note: No NOP in the version
                                                                     //Note: Name in Firdous' folder: scrt_aes_cbc_nist_romcpu_no_nop
{
    unsigned int data;
    unsigned int scrt0_write_addr_1 = 0x60003f00;//SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_CTRL;
    unsigned int scrt0_read_addr_1 = 0x60003f00;//SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_STAT;

    unsigned int sram_addr_base = 0x10000000;
    unsigned int sram_addr_write_1 = (sram_addr_base + 0x3000);

    unsigned int w_1_addr = (sram_addr_write_1 + 0);
    unsigned int w_2_addr = (sram_addr_write_1 + 4);
    unsigned int w_3_addr = (sram_addr_write_1 + 8);
    unsigned int w_4_addr = (sram_addr_write_1 + 12);
    unsigned int w_5_addr = (sram_addr_write_1 + 16);
    unsigned int w_6_addr = (sram_addr_write_1 + 20);
    unsigned int w_7_addr = (sram_addr_write_1 + 24);
    unsigned int w_8_addr = (sram_addr_write_1 + 28);
    unsigned int w_9_addr = (sram_addr_write_1 + 32);
    unsigned int w_10_addr = (sram_addr_write_1 + 36);
    unsigned int w_11_addr = (sram_addr_write_1 + 40);
    unsigned int w_12_addr = (sram_addr_write_1 + 44);
    unsigned int w_13_addr = (sram_addr_write_1 + 48);
    unsigned int w_14_addr = (sram_addr_write_1 + 52);
    unsigned int w_15_addr = (sram_addr_write_1 + 56);
    unsigned int w_16_addr = (sram_addr_write_1 + 60);

    unsigned int w_1_data = 0xe2bec16b;
    unsigned int w_2_data = 0x969f402e;
    unsigned int w_3_data = 0x117e3de9;
    unsigned int w_4_data = 0x2a179373;
    unsigned int w_5_data = 0x578a2dae;
    unsigned int w_6_data = 0x9cac031e;
    unsigned int w_7_data = 0xac6fb79e;
    unsigned int w_8_data = 0x518eaf45;
    unsigned int w_9_data = 0x461cc830;
    unsigned int w_10_data = 0x11e45ca3;
    unsigned int w_11_data = 0x19c1fbe5;
    unsigned int w_12_data = 0xef520a1a;
    unsigned int w_13_data = 0x45249ff6;
    unsigned int w_14_data = 0x179b4fdf;
    unsigned int w_15_data = 0x7b412bad;
    unsigned int w_16_data = 0x10376ce6;

    unsigned int c_1_data = 0xacab4976;
    unsigned int c_2_data = 0x46b21981;
    unsigned int c_3_data = 0x9b8ee9ce;
    unsigned int c_4_data = 0x7d19e912;
    unsigned int c_5_data = 0x9bcb8650;
    unsigned int c_6_data = 0xee197250;
    unsigned int c_7_data = 0x3a11db95;
    unsigned int c_8_data = 0xb2787691;
    unsigned int c_9_data = 0xb8d6be73;
    unsigned int c_10_data = 0x3b74c1e3;
    unsigned int c_11_data = 0x9ee61671;
    unsigned int c_12_data = 0x16952222;
    unsigned int c_13_data = 0xa1caf13f;
    unsigned int c_14_data = 0x09ac1f68;
    unsigned int c_15_data = 0x30ca0e12;
    unsigned int c_16_data = 0xa7e18675;

    unsigned int mask_b_1 = 0x00000001;
    unsigned int mask_b_2 = 0x00000001;

    unsigned int sram_addr_output_encrypted_data = (sram_addr_base + 0x200);

    unsigned int w_output_token_result_addr = 0x10000000;
    unsigned int r_output_token_data;
    unsigned int c_output_token_pass_data = 0xd82c;
    unsigned int w_output_token_pass_data = 0xf0f0f0f0;
    unsigned int w_output_token_fail_data = 0x48484848;

    unsigned int sram_addr_read_13 = (sram_addr_base + 0x240);

    unsigned int r_1_addr = (sram_addr_read_13 + 0);
    unsigned int r_2_addr = (sram_addr_read_13 + 4);
    unsigned int r_3_addr = (sram_addr_read_13 + 8);
    unsigned int r_4_addr = (sram_addr_read_13 + 12);
    unsigned int r_5_addr = (sram_addr_read_13 + 16);
    unsigned int r_6_addr = (sram_addr_read_13 + 20);
    unsigned int r_7_addr = (sram_addr_read_13 + 24);
    unsigned int r_8_addr = (sram_addr_read_13 + 28);
    unsigned int r_9_addr = (sram_addr_read_13 + 32);
    unsigned int r_10_addr = (sram_addr_read_13 + 36);
    unsigned int r_11_addr = (sram_addr_read_13 + 40);
    unsigned int r_12_addr = (sram_addr_read_13 + 44);
    unsigned int r_13_addr = (sram_addr_read_13 + 48);
    unsigned int r_14_addr = (sram_addr_read_13 + 52);
    unsigned int r_15_addr = (sram_addr_read_13 + 56);
    unsigned int r_16_addr = (sram_addr_read_13 + 60);

    unsigned int r_1_data = 0;
    unsigned int r_2_data = 0;
    unsigned int r_3_data = 0;
    unsigned int r_4_data = 0;
    unsigned int r_5_data = 0;
    unsigned int r_6_data = 0;
    unsigned int r_7_data = 0;
    unsigned int r_8_data = 0;
    unsigned int r_9_data = 0;
    unsigned int r_10_data = 0;
    unsigned int r_11_data = 0;
    unsigned int r_12_data = 0;
    unsigned int r_13_data = 0;
    unsigned int r_14_data = 0;
    unsigned int r_15_data = 0;
    unsigned int r_16_data = 0;

    unsigned int scrt0_read_addr_2 = (SCRT0_BASE_ADDR + 0x0008);

    unsigned int c2_1_data = 0xa1caf13f;
    unsigned int c2_2_data = 0x09ac1f68;
    unsigned int c2_3_data = 0x30ca0e12;
    unsigned int c2_4_data = 0xa7e18675;

    unsigned int w2_1_data = 0x0104d82c;
    unsigned int w2_2_data = 0x00000000;
    unsigned int w2_3_data = 0x00000040;
    unsigned int w2_4_data = 0x10000100;
    unsigned int w2_5_data = 0x00000000;
    unsigned int w2_6_data = 0x00000040;
    unsigned int w2_7_data = 0x10000200;
    unsigned int w2_8_data = 0x00000000;
    unsigned int w2_9_data = 0x00000044;
    unsigned int w2_10_data = 0x00000000;
    unsigned int w2_11_data = 0x00000000;
    unsigned int w2_12_data = 0x00018010;
    unsigned int w2_13_data = 0x00000000;
    unsigned int w2_14_data = 0x03020100;
    unsigned int w2_15_data = 0x07060504;
    unsigned int w2_16_data = 0x0b0a0908;
    unsigned int w2_17_data = 0x0f0e0d0c;
    unsigned int w2_18_data = 0x16157e2b;
    unsigned int w2_19_data = 0xa6d2ae28;
    unsigned int w2_20_data = 0x8815f7ab;
    unsigned int w2_21_data = 0x3c4fcf09;
    unsigned int w2_22_data = 0x00000000;
    unsigned int w2_23_data = 0x00000000;
    unsigned int w2_24_data = 0x00000000;
    unsigned int w2_25_data = 0x00000000;

    //Mailbox 1
    *(volatile unsigned int *)0x60003f00 = 0x4;

    //Mailbox 2
    *(volatile unsigned int *)0x60003f00 = 0x40;

    //Mailbox 3
    *(volatile unsigned int *)0x60003f00 = 0x400;

    //Mailbox 4
    *(volatile unsigned int *)0x60003f00 = 0x4000;

    //Mailbox 1 again
    *(volatile unsigned int *)0x60003f00 = 0x4;

    //Burst_write 64 bytes in SRAM @ 0x1000_3000, this is plain text data
    *(volatile unsigned int *)0x10003000 = 0xe2bec16b;
    *(volatile unsigned int *)0x10003004 = 0x969f402e;
    *(volatile unsigned int *)0x10003008 = 0x117e3de9;
    *(volatile unsigned int *)0x1000300c = 0x2a179373;
    *(volatile unsigned int *)0x10003010 = 0x578a2dae;
    *(volatile unsigned int *)0x10003014 = 0x9cac031e;
    *(volatile unsigned int *)0x10003018 = 0xac6fb79e;
    *(volatile unsigned int *)0x1000301c = 0x518eaf45;
    *(volatile unsigned int *)0x10003020 = 0x461cc830;
    *(volatile unsigned int *)0x10003024 = 0x11e45ca3;
    *(volatile unsigned int *)0x10003028 = 0x19c1fbe5;
    *(volatile unsigned int *)0x1000302c = 0xef520a1a;
    *(volatile unsigned int *)0x10003030 = 0x45249ff6;
    *(volatile unsigned int *)0x10003034 = 0x179b4fdf;
    *(volatile unsigned int *)0x10003038 = 0x7b412bad;
    *(volatile unsigned int *)0x1000303c = 0x10376ce6;

    //Polling -1
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Burst write 100 bytes
    *(volatile unsigned int *)0x60000000 = 0x0104d82c;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = 0x00000040;
    *(volatile unsigned int *)0x6000000c = 0x10003000;
    *(volatile unsigned int *)0x60000010 = 0x00000000;
    *(volatile unsigned int *)0x60000014 = 0x00000040;
    *(volatile unsigned int *)0x60000018 = 0x10004000;
    *(volatile unsigned int *)0x6000001c = 0x00000000;
    *(volatile unsigned int *)0x60000020 = 0x00000044;
    *(volatile unsigned int *)0x60000024 = 0x00000000;
    *(volatile unsigned int *)0x60000028 = 0x00000000;
    *(volatile unsigned int *)0x6000002c = 0x00018010;
    *(volatile unsigned int *)0x60000030 = 0x00000000;
    *(volatile unsigned int *)0x60000034 = 0x03020100;
    *(volatile unsigned int *)0x60000038 = 0x07060504;
    *(volatile unsigned int *)0x6000003c = 0x0b0a0908;
    *(volatile unsigned int *)0x60000040 = 0x0f0e0d0c;
    *(volatile unsigned int *)0x60000044 = 0x16157e2b;
    *(volatile unsigned int *)0x60000048 = 0xa6d2ae28;
    *(volatile unsigned int *)0x6000004c = 0x8815f7ab;
    *(volatile unsigned int *)0x60000050 = 0x3c4fcf09;
    *(volatile unsigned int *)0x60000054 = 0x00000000;
    *(volatile unsigned int *)0x60000058 = 0x00000000;
    *(volatile unsigned int *)0x6000005c = 0x00000000;
    *(volatile unsigned int *)0x60000060 = 0x00000000;

    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;

    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }


    //Check output token status
    r_output_token_data = *((volatile unsigned int *)0x60000000);

    if (r_output_token_data == c_output_token_pass_data) {
        *(volatile unsigned int *)0x10002500 = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)0x10002500 = w_output_token_fail_data;
    }

    //Check block 64 bytes output encrypted data in SRAM
    r_1_data = *((volatile unsigned int *)0x10004000);
    r_2_data = *((volatile unsigned int *)0x10004004);
    r_3_data = *((volatile unsigned int *)0x10004008);
    r_4_data = *((volatile unsigned int *)0x1000400c);
    r_5_data = *((volatile unsigned int *)0x10004010);
    r_6_data = *((volatile unsigned int *)0x10004014);
    r_7_data = *((volatile unsigned int *)0x10004018);
    r_8_data = *((volatile unsigned int *)0x1000401c);
    r_9_data = *((volatile unsigned int *)0x10004020);
    r_10_data = *((volatile unsigned int *)0x10004024);
    r_11_data = *((volatile unsigned int *)0x10004028);
    r_12_data = *((volatile unsigned int *)0x1000402c);
    r_13_data = *((volatile unsigned int *)0x10004030);
    r_14_data = *((volatile unsigned int *)0x10004034);
    r_15_data = *((volatile unsigned int *)0x10004038);
    r_16_data = *((volatile unsigned int *)0x1000403c);

    if ((r_1_data == c_1_data)
        &&(r_2_data == c_2_data)
        &&(r_3_data == c_3_data)
        &&(r_4_data == c_4_data)
        &&(r_5_data == c_5_data)
        &&(r_6_data == c_6_data)
        &&(r_7_data == c_7_data)
        &&(r_8_data == c_8_data)
        &&(r_9_data == c_9_data)
        &&(r_10_data == c_10_data)
        &&(r_11_data == c_11_data)
        &&(r_12_data == c_12_data)
        &&(r_13_data == c_13_data)
        &&(r_14_data == c_14_data)
        &&(r_15_data == c_15_data)
        &&(r_16_data == c_16_data)
        ) {
        //compare pass
        *(volatile unsigned int *)0x10002500 = w_output_token_pass_data;
    } else {
        //compare fail
        *(volatile unsigned int *)0x10002500 = w_output_token_fail_data;
    }

    //Check Token ID in output data's last word
    r_output_token_data = *((volatile unsigned int *)0x10004040);

    if (r_output_token_data == c_output_token_pass_data) {
        *(volatile unsigned int *)0x10002500 = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)0x10002500 = w_output_token_fail_data;
    }

    //Check 16 bytes in offset 0x0008
    r_1_data = *((volatile unsigned int *)0x60000008);
    r_2_data = *((volatile unsigned int *)0x6000000c);
    r_3_data = *((volatile unsigned int *)0x60000010);
    r_4_data = *((volatile unsigned int *)0x60000014);

    if ( (r_1_data == c2_1_data)
         &&(r_1_data == c2_2_data)
         &&(r_1_data == c2_3_data)
         &&(r_1_data == c2_4_data)
         ){
        *(volatile unsigned int *)0x10002500 = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)0x10002500 = w_output_token_fail_data;
    }

    //Write
    *(volatile unsigned int *)0x60003f00 = 0x2;

}


void test_onrom_level5_scrt0_aes_2k(void) //Testing...  We found there's something wrong when polling, it's a bus issue
                                                  //This test is pending...
                                                  //Firdous said it's hw or environment issue because when firdous try to write 2k bytes data in his hw test pattern, it can't work
                                                  //Note: write 2k bytes
{                                               //Filename in Firdous' system: aes_cbc_2K_bytes
    unsigned int sram_addr_base = 0x10000000;
    unsigned int sram_addr_output_encrypted_data = (sram_addr_base + 0x200);

    unsigned int r_output_token_data;
    unsigned int c_output_token_pass_data = 0xd82c;
    unsigned int w_output_token_pass_data = 0xf0f0f0f0;
    unsigned int w_output_token_fail_data = 0x48484848;

    unsigned int r_1_data = 0;
    unsigned int r_2_data = 0;
    unsigned int r_3_data = 0;
    unsigned int r_4_data = 0;

    unsigned int scrt0_read_addr_2 = (SCRT0_BASE_ADDR + 0x0008);

    unsigned int c2_1_data = 0xa1caf13f;
    unsigned int c2_2_data = 0x09ac1f68;
    unsigned int c2_3_data = 0x30ca0e12;
    unsigned int c2_4_data = 0xa7e18675;

    unsigned int data;

    unsigned int i;


    //Mailbox 1
    *(volatile unsigned int *)0x60003f00 = 0x4;

    //Mailbox 2
    *(volatile unsigned int *)0x60003f00 = 0x40;

    //Mailbox 3
    *(volatile unsigned int *)0x60003f00 = 0x400;

    //Mailbox 4
    *(volatile unsigned int *)0x60003f00 = 0x4000;

    //Mailbox 1 again
    *(volatile unsigned int *)0x60003f00 = 0x4;



    //Burst_write 2K bytes in SRAM
    for (i=0; i<125; i++) {
        //Each loop, 4 words (16 bytes)
        *((volatile unsigned int *)(0x10003000 + ( (i*16) + 0 ))) = 0x11111111;
        *((volatile unsigned int *)(0x10003000 + ( (i*16) + 4 ))) = 0x22222222;
        *((volatile unsigned int *)(0x10003000 + ( (i*16) + 8 ))) = 0x33333333;
        *((volatile unsigned int *)(0x10003000 + ( (i*16) + 12 ))) = 0x44444444;
    }


    //Polling -1
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }


    //Write Control Token
    //Firmware Reference Manual pdf: 5.2.1 Encryption Input Token
    *(volatile unsigned int *)0x60000000 = 0x0104d82c;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = 0x000007d0;
    *(volatile unsigned int *)0x6000000c = 0x10003000;
    *(volatile unsigned int *)0x60000010 = 0x00000000;
    *(volatile unsigned int *)0x60000014 = 0x000007d0;
    *(volatile unsigned int *)0x60000018 = 0x10030000;
    *(volatile unsigned int *)0x6000001c = 0x00000000;
    *(volatile unsigned int *)0x60000020 = 0x000007d4;
    *(volatile unsigned int *)0x60000024 = 0x00000000;
    *(volatile unsigned int *)0x60000028 = 0x00000000;
    *(volatile unsigned int *)0x6000002c = 0x00018010;
    *(volatile unsigned int *)0x60000030 = 0x00000000;
    *(volatile unsigned int *)0x60000034 = 0x03020100;
    *(volatile unsigned int *)0x60000038 = 0x07060504;
    *(volatile unsigned int *)0x6000003c = 0x0b0a0908;
    *(volatile unsigned int *)0x60000040 = 0x0f0e0d0c;
    *(volatile unsigned int *)0x60000044 = 0x16157e2b;
    *(volatile unsigned int *)0x60000048 = 0xa6d2ae28;
    *(volatile unsigned int *)0x6000004c = 0x8815f7ab;
    *(volatile unsigned int *)0x60000050 = 0x3c4fcf09;
    *(volatile unsigned int *)0x60000054 = 0x00000000;
    *(volatile unsigned int *)0x60000058 = 0x00000000;
    *(volatile unsigned int *)0x6000005c = 0x00000000;
    *(volatile unsigned int *)0x60000060 = 0x00000000;


    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;


    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }


    //Check output token status
    r_output_token_data = *((volatile unsigned int *)0x60000000);

    if (r_output_token_data == c_output_token_pass_data) {
        *(volatile unsigned int *)0x10002500 = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)0x10002500 = w_output_token_fail_data;
    }


    //Read output encrypted 2K bytes data in SRAM
    for (i=0; i++; i<512) {
        r_1_data = *((volatile unsigned int *)(0x10030000 + (i*4)));
    }


    //Check token id in output data's last word
    r_output_token_data = *((volatile unsigned int *)0x100307d0);
    if (( r_output_token_data & 0x00001111 ) == c_output_token_pass_data) {
        *(volatile unsigned int *)0x10002500 = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)0x10002500 = w_output_token_fail_data;
    }


    //Check 16 bytes in offset 0x0008
    r_1_data = *((volatile unsigned int *)0x60000008);
    r_2_data = *((volatile unsigned int *)0x6000000c);
    r_3_data = *((volatile unsigned int *)0x60000010);
    r_4_data = *((volatile unsigned int *)0x60000014);

    if ( (r_1_data == c2_1_data)
         &&(r_1_data == c2_2_data)
         &&(r_1_data == c2_3_data)
         &&(r_1_data == c2_4_data)
         ){
        *(volatile unsigned int *)0x10002500 = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)0x10002500 = w_output_token_fail_data;
    }


    //Write
    *(volatile unsigned int *)0x60003f00 = 0x2;
}


void test_onrom_level5_scrt0_aes_64b_1mailbox_3tokens(void) //AES 64K Bytes Encryption on mailbox 1 with 3 tokens
                                                                 // Filename in Firdous' system: aes_cbc_nist_1mbx_3data
                                                                 //Not finish the implementation yet
{
    unsigned int data;

    unsigned int scrt0_write_addr_1 = 0x60003f00; //SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_CTRL;
    unsigned int scrt0_read_addr_1  = 0x60003f00; //SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_STAT;

    unsigned int sram_addr_base             = 0x00400000;
    unsigned int sram_addr_write            = 0x00410000; //(sram_addr_base + 0x10000);
    unsigned int sram_addr_output_encrypted = 0x00411000; //(sram_addr_base + 0x11000);

    unsigned int result_addr                = 0x0040a800;
    unsigned int flag_addr                  = 0x0040a900;

    unsigned int r_output_token_data;
    unsigned int c_output_token_pass_data = 0xd82c;
    unsigned int w_output_token_pass_data = 0xf0f0f0f0;
    unsigned int w_output_token_fail_data = 0x48484848;

    unsigned int r_1_data = 0;
    unsigned int r_2_data = 0;
    unsigned int r_3_data = 0;
    unsigned int r_4_data = 0;
    unsigned int r_5_data = 0;
    unsigned int r_6_data = 0;
    unsigned int r_7_data = 0;
    unsigned int r_8_data = 0;
    unsigned int r_9_data = 0;
    unsigned int r_10_data = 0;
    unsigned int r_11_data = 0;
    unsigned int r_12_data = 0;
    unsigned int r_13_data = 0;
    unsigned int r_14_data = 0;
    unsigned int r_15_data = 0;
    unsigned int r_16_data = 0;

    unsigned int c_1_data = 0xacab4976;
    unsigned int c_2_data = 0x46b21981;
    unsigned int c_3_data = 0x9b8ee9ce;
    unsigned int c_4_data = 0x7d19e912;
    unsigned int c_5_data = 0x9bcb8650;
    unsigned int c_6_data = 0xee197250;
    unsigned int c_7_data = 0x3a11db95;
    unsigned int c_8_data = 0xb2787691;
    unsigned int c_9_data = 0xb8d6be73;
    unsigned int c_10_data = 0x3b74c1e3;
    unsigned int c_11_data = 0x9ee61671;
    unsigned int c_12_data = 0x16952222;
    unsigned int c_13_data = 0xa1caf13f;
    unsigned int c_14_data = 0x09ac1f68;
    unsigned int c_15_data = 0x30ca0e12;
    unsigned int c_16_data = 0xa7e18675;

    unsigned int c2_1_data = 0xa1caf13f;
    unsigned int c2_2_data = 0x09ac1f68;
    unsigned int c2_3_data = 0x30ca0e12;
    unsigned int c2_4_data = 0xa7e18675;

    unsigned int c3_1_data = 0x79e2b008;
    unsigned int c3_2_data = 0x81885988;
    unsigned int c3_3_data = 0xe6a920d9;
    unsigned int c3_4_data = 0xcd15564f;


    //Mailbox 1
    *(volatile unsigned int *)0x60003f00 = 0x4;

    /*
        ===================================
            Plain text 1
        ===================================
      */
    //Burst_write 64 bytes plain text data in SRAM
    *((volatile unsigned int *)(sram_addr_write + 0x0)) = 0xe2bec16b;
    *((volatile unsigned int *)(sram_addr_write + 0x4)) = 0x969f402e;
    *((volatile unsigned int *)(sram_addr_write + 0x8)) = 0x117e3de9;
    *((volatile unsigned int *)(sram_addr_write + 0xc)) = 0x2a179373;
    *((volatile unsigned int *)(sram_addr_write + 0x10)) = 0x578a2dae;
    *((volatile unsigned int *)(sram_addr_write + 0x14)) = 0x9cac031e;
    *((volatile unsigned int *)(sram_addr_write + 0x18)) = 0xac6fb79e;
    *((volatile unsigned int *)(sram_addr_write + 0x1c)) = 0x518eaf45;
    *((volatile unsigned int *)(sram_addr_write + 0x20)) = 0x461cc830;
    *((volatile unsigned int *)(sram_addr_write + 0x24)) = 0x11e45ca3;
    *((volatile unsigned int *)(sram_addr_write + 0x28)) = 0x19c1fbe5;
    *((volatile unsigned int *)(sram_addr_write + 0x2c)) = 0xef520a1a;
    *((volatile unsigned int *)(sram_addr_write + 0x30)) = 0x45249ff6;
    *((volatile unsigned int *)(sram_addr_write + 0x34)) = 0x179b4fdf;
    *((volatile unsigned int *)(sram_addr_write + 0x38)) = 0x7b412bad;
    *((volatile unsigned int *)(sram_addr_write + 0x3c)) = 0x10376ce6;

    //Check mailbox is empty
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Burst write 100 bytes (token1) in mailbox @ 0x6000_0000
    *(volatile unsigned int *)0x60000000 = 0x0104d82c;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = 0x00000040;
    *(volatile unsigned int *)0x6000000c = 0x00410000;
    *(volatile unsigned int *)0x60000010 = 0x00000000;
    *(volatile unsigned int *)0x60000014 = 0x00000040;
    *(volatile unsigned int *)0x60000018 = 0x00411000;
    *(volatile unsigned int *)0x6000001c = 0x00000000;
    *(volatile unsigned int *)0x60000020 = 0x00000044;
    *(volatile unsigned int *)0x60000024 = 0x00000000;
    *(volatile unsigned int *)0x60000028 = 0x00000000;
    *(volatile unsigned int *)0x6000002c = 0x00018010;
    *(volatile unsigned int *)0x60000030 = 0x00000000;
    *(volatile unsigned int *)0x60000034 = 0x03020100;
    *(volatile unsigned int *)0x60000038 = 0x07060504;
    *(volatile unsigned int *)0x6000003c = 0x0b0a0908;
    *(volatile unsigned int *)0x60000040 = 0x0f0e0d0c;
    *(volatile unsigned int *)0x60000044 = 0x16157e2b;
    *(volatile unsigned int *)0x60000048 = 0xa6d2ae28;
    *(volatile unsigned int *)0x6000004c = 0x8815f7ab;
    *(volatile unsigned int *)0x60000050 = 0x3c4fcf09;
    *(volatile unsigned int *)0x60000054 = 0x00000000;
    *(volatile unsigned int *)0x60000058 = 0x00000000;
    *(volatile unsigned int *)0x6000005c = 0x00000000;
    *(volatile unsigned int *)0x60000060 = 0x00000000;

    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;



    /*
        ===================================
            Plain text 2
        ===================================
      */
    //Burst_write 64 bytes plain text data in SRAM
    *((volatile unsigned int *)(sram_addr_write + 0x0)) = 0xe2bec16b;
    *((volatile unsigned int *)(sram_addr_write + 0x4)) = 0x969f402e;
    *((volatile unsigned int *)(sram_addr_write + 0x8)) = 0x117e3de9;
    *((volatile unsigned int *)(sram_addr_write + 0xc)) = 0x2a179373;
    *((volatile unsigned int *)(sram_addr_write + 0x10)) = 0x578a2dae;
    *((volatile unsigned int *)(sram_addr_write + 0x14)) = 0x9cac031e;
    *((volatile unsigned int *)(sram_addr_write + 0x18)) = 0xac6fb79e;
    *((volatile unsigned int *)(sram_addr_write + 0x1c)) = 0x518eaf45;
    *((volatile unsigned int *)(sram_addr_write + 0x20)) = 0x461cc830;
    *((volatile unsigned int *)(sram_addr_write + 0x24)) = 0x11e45ca3;
    *((volatile unsigned int *)(sram_addr_write + 0x28)) = 0x19c1fbe5;
    *((volatile unsigned int *)(sram_addr_write + 0x2c)) = 0xef520a1a;
    *((volatile unsigned int *)(sram_addr_write + 0x30)) = 0x45249ff6;
    *((volatile unsigned int *)(sram_addr_write + 0x34)) = 0x179b4fdf;
    *((volatile unsigned int *)(sram_addr_write + 0x38)) = 0x7b412bad;
    *((volatile unsigned int *)(sram_addr_write + 0x3c)) = 0x10376ce6;


    //Polling to check mailbox is empty
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Burst write 100 bytes (token2) in mailbox @ 0x6000_0000
    *(volatile unsigned int *)0x60000000 = 0x010455fe;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = 0x00000040;
    *(volatile unsigned int *)0x6000000c = 0x00410000;
    *(volatile unsigned int *)0x60000010 = 0x00000000;
    *(volatile unsigned int *)0x60000014 = 0x00000040;
    *(volatile unsigned int *)0x60000018 = 0x00411000;
    *(volatile unsigned int *)0x6000001c = 0x00000000;
    *(volatile unsigned int *)0x60000020 = 0x00000044;
    *(volatile unsigned int *)0x60000024 = 0x00000000;
    *(volatile unsigned int *)0x60000028 = 0x00000000;
    *(volatile unsigned int *)0x6000002c = 0x00028010;
    *(volatile unsigned int *)0x60000030 = 0x00000000;
    *(volatile unsigned int *)0x60000034 = 0x03020100;
    *(volatile unsigned int *)0x60000038 = 0x07060504;
    *(volatile unsigned int *)0x6000003c = 0x0b0a0908;
    *(volatile unsigned int *)0x60000040 = 0x0f0e0d0c;
    *(volatile unsigned int *)0x60000044 = 0xf7b0738e;
    *(volatile unsigned int *)0x60000048 = 0x52640eda;
    *(volatile unsigned int *)0x6000004c = 0x2bf310c8;
    *(volatile unsigned int *)0x60000050 = 0xe5799080;
    *(volatile unsigned int *)0x60000054 = 0xd2eaf862;
    *(volatile unsigned int *)0x60000058 = 0x7b6b2c52;
    *(volatile unsigned int *)0x6000005c = 0x00000000;
    *(volatile unsigned int *)0x60000060 = 0x00000000;

    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;

    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Check output token's Token ID
    r_output_token_data = *((volatile unsigned int *)0x60000000);

    if (r_output_token_data == c_output_token_pass_data) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Check block 64 bytes output encrypted data in SRAM
    r_1_data = *((volatile unsigned int *)0x10004000);
    r_2_data = *((volatile unsigned int *)0x10004004);
    r_3_data = *((volatile unsigned int *)0x10004008);
    r_4_data = *((volatile unsigned int *)0x1000400c);
    r_5_data = *((volatile unsigned int *)0x10004010);
    r_6_data = *((volatile unsigned int *)0x10004014);
    r_7_data = *((volatile unsigned int *)0x10004018);
    r_8_data = *((volatile unsigned int *)0x1000401c);
    r_9_data = *((volatile unsigned int *)0x10004020);
    r_10_data = *((volatile unsigned int *)0x10004024);
    r_11_data = *((volatile unsigned int *)0x10004028);
    r_12_data = *((volatile unsigned int *)0x1000402c);
    r_13_data = *((volatile unsigned int *)0x10004030);
    r_14_data = *((volatile unsigned int *)0x10004034);
    r_15_data = *((volatile unsigned int *)0x10004038);
    r_16_data = *((volatile unsigned int *)0x1000403c);

    if ((r_1_data == c_1_data)
        &&(r_1_data == c_2_data)
        &&(r_1_data == c_3_data)
        &&(r_1_data == c_4_data)
        &&(r_1_data == c_5_data)
        &&(r_1_data == c_6_data)
        &&(r_1_data == c_7_data)
        &&(r_1_data == c_8_data)
        &&(r_1_data == c_9_data)
        &&(r_1_data == c_10_data)
        &&(r_1_data == c_11_data)
        &&(r_1_data == c_12_data)
        &&(r_1_data == c_13_data)
        &&(r_1_data == c_14_data)
        &&(r_1_data == c_15_data)
        &&(r_1_data == c_16_data)
        ) {
        //compare pass
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        //compare fail
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Check output token's Token ID
    r_output_token_data = *((volatile unsigned int *)0x60000000);

    if (r_output_token_data == c_output_token_pass_data) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Check 16 bytes in offset 0x0008
    r_1_data = *((volatile unsigned int *)0x60000008);
    r_2_data = *((volatile unsigned int *)0x6000000c);
    r_3_data = *((volatile unsigned int *)0x60000010);
    r_4_data = *((volatile unsigned int *)0x60000014);

    if ( (r_1_data == c2_1_data)
         &&(r_1_data == c2_2_data)
         &&(r_1_data == c2_3_data)
         &&(r_1_data == c2_4_data)
         ){
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Write a word, job1 done and checked, clear mailbox
    *(volatile unsigned int *)0x60003f00 = 0x2;



    /*
        ===================================
            Plain text 3
        ===================================
      */
    //Burst_write 64 bytes plain text data in SRAM
    *((volatile unsigned int *)(sram_addr_write + 0x0)) = 0xe2bec16b;
    *((volatile unsigned int *)(sram_addr_write + 0x4)) = 0x969f402e;
    *((volatile unsigned int *)(sram_addr_write + 0x8)) = 0x117e3de9;
    *((volatile unsigned int *)(sram_addr_write + 0xc)) = 0x2a179373;
    *((volatile unsigned int *)(sram_addr_write + 0x10)) = 0x578a2dae;
    *((volatile unsigned int *)(sram_addr_write + 0x14)) = 0x9cac031e;
    *((volatile unsigned int *)(sram_addr_write + 0x18)) = 0xac6fb79e;
    *((volatile unsigned int *)(sram_addr_write + 0x1c)) = 0x518eaf45;
    *((volatile unsigned int *)(sram_addr_write + 0x20)) = 0x461cc830;
    *((volatile unsigned int *)(sram_addr_write + 0x24)) = 0x11e45ca3;
    *((volatile unsigned int *)(sram_addr_write + 0x28)) = 0x19c1fbe5;
    *((volatile unsigned int *)(sram_addr_write + 0x2c)) = 0xef520a1a;
    *((volatile unsigned int *)(sram_addr_write + 0x30)) = 0x45249ff6;
    *((volatile unsigned int *)(sram_addr_write + 0x34)) = 0x179b4fdf;
    *((volatile unsigned int *)(sram_addr_write + 0x38)) = 0x7b412bad;
    *((volatile unsigned int *)(sram_addr_write + 0x3c)) = 0x10376ce6;


    //Polling to check mailbox is empty
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Burst write 100 bytes (token2) in mailbox @ 0x6000_0000
    *(volatile unsigned int *)0x60000000 = 0x010455fe;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = 0x00000040;
    *(volatile unsigned int *)0x6000000c = 0x00410000;
    *(volatile unsigned int *)0x60000010 = 0x00000000;
    *(volatile unsigned int *)0x60000014 = 0x00000040;
    *(volatile unsigned int *)0x60000018 = 0x00411000;
    *(volatile unsigned int *)0x6000001c = 0x00000000;
    *(volatile unsigned int *)0x60000020 = 0x00000044;
    *(volatile unsigned int *)0x60000024 = 0x00000000;
    *(volatile unsigned int *)0x60000028 = 0x00000000;
    *(volatile unsigned int *)0x6000002c = 0x00028010;
    *(volatile unsigned int *)0x60000030 = 0x00000000;
    *(volatile unsigned int *)0x60000034 = 0x03020100;
    *(volatile unsigned int *)0x60000038 = 0x07060504;
    *(volatile unsigned int *)0x6000003c = 0x0b0a0908;
    *(volatile unsigned int *)0x60000040 = 0x0f0e0d0c;
    *(volatile unsigned int *)0x60000044 = 0xf7b0738e;
    *(volatile unsigned int *)0x60000048 = 0x52640eda;
    *(volatile unsigned int *)0x6000004c = 0x2bf310c8;
    *(volatile unsigned int *)0x60000050 = 0xe5799080;
    *(volatile unsigned int *)0x60000054 = 0xd2eaf862;
    *(volatile unsigned int *)0x60000058 = 0x7b6b2c52;
    *(volatile unsigned int *)0x6000005c = 0x00000000;
    *(volatile unsigned int *)0x60000060 = 0x00000000;

    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;

    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }


    //23)CheckWord (0x1003_1040, 0x0000_55FE, 0x0000_FFFF) (tail of encrypted output 2)
    //??



    //Check_block 16 bytes in mailbox @ 0x6000_0008  (mailbox after job2):
    r_1_data = *((volatile unsigned int *)0x60000008);
    r_2_data = *((volatile unsigned int *)0x6000000c);
    r_3_data = *((volatile unsigned int *)0x60000010);
    r_4_data = *((volatile unsigned int *)0x60000014);

    if ( (r_1_data == c3_1_data)
         &&(r_1_data == c3_2_data)
         &&(r_1_data == c3_3_data)
         &&(r_1_data == c3_4_data)
         ){
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Write a word, job1 done and checked, clear mailbox
    *(volatile unsigned int *)0x60003f00 = 0x2;

    //not finish yet




}


void test_onrom_level5_scrt0_aes_64bytes_with_new_sram_address_400000(void) //it pass in Firdous's simulation test
                                                                                                 //Firdous is waiting for James's new item, then he can re-test it
                                                                                                 //Run this test on SRAM
                                                                                                 //Need to combine to Vincent to bootloader code, he will generate the new image to me, I need to convert it to hex file to Firdous
{
    unsigned int data;
    unsigned int scrt0_write_addr_1 = 0x60003f00; //SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_CTRL;
    unsigned int scrt0_read_addr_1 = 0x60003f00; //SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_STAT;

    unsigned int sram_addr_base = 0x00400000;
    unsigned int sram_addr_write = 0x00410000; //(sram_addr_base + 0x10000);
    unsigned int sram_addr_output_encrypted = 0x00411000; //(sram_addr_base + 0x11000);

    unsigned int result_addr = 0x0040a800;
    unsigned int flag_addr   = 0x0040a900;

    unsigned int w_1_addr = (sram_addr_write + 0x0);
    unsigned int w_2_addr = (sram_addr_write + 0x4);
    unsigned int w_3_addr = (sram_addr_write + 0x8);
    unsigned int w_4_addr = (sram_addr_write + 0xc);
    unsigned int w_5_addr = (sram_addr_write + 0x10);
    unsigned int w_6_addr = (sram_addr_write + 0x14);
    unsigned int w_7_addr = (sram_addr_write + 0x18);
    unsigned int w_8_addr = (sram_addr_write + 0x1c);
    unsigned int w_9_addr = (sram_addr_write + 0x20);
    unsigned int w_10_addr = (sram_addr_write + 0x24);
    unsigned int w_11_addr = (sram_addr_write + 0x28);
    unsigned int w_12_addr = (sram_addr_write + 0x2c);
    unsigned int w_13_addr = (sram_addr_write + 0x30);
    unsigned int w_14_addr = (sram_addr_write + 0x34);
    unsigned int w_15_addr = (sram_addr_write + 0x38);
    unsigned int w_16_addr = (sram_addr_write + 0x3c);

    unsigned int w_1_data = 0xe2bec16b;
    unsigned int w_2_data = 0x969f402e;
    unsigned int w_3_data = 0x117e3de9;
    unsigned int w_4_data = 0x2a179373;
    unsigned int w_5_data = 0x578a2dae;
    unsigned int w_6_data = 0x9cac031e;
    unsigned int w_7_data = 0xac6fb79e;
    unsigned int w_8_data = 0x518eaf45;
    unsigned int w_9_data = 0x461cc830;
    unsigned int w_10_data = 0x11e45ca3;
    unsigned int w_11_data = 0x19c1fbe5;
    unsigned int w_12_data = 0xef520a1a;
    unsigned int w_13_data = 0x45249ff6;
    unsigned int w_14_data = 0x179b4fdf;
    unsigned int w_15_data = 0x7b412bad;
    unsigned int w_16_data = 0x10376ce6;

    unsigned int c_1_data = 0xacab4976;
    unsigned int c_2_data = 0x46b21981;
    unsigned int c_3_data = 0x9b8ee9ce;
    unsigned int c_4_data = 0x7d19e912;
    unsigned int c_5_data = 0x9bcb8650;
    unsigned int c_6_data = 0xee197250;
    unsigned int c_7_data = 0x3a11db95;
    unsigned int c_8_data = 0xb2787691;
    unsigned int c_9_data = 0xb8d6be73;
    unsigned int c_10_data = 0x3b74c1e3;
    unsigned int c_11_data = 0x9ee61671;
    unsigned int c_12_data = 0x16952222;
    unsigned int c_13_data = 0xa1caf13f;
    unsigned int c_14_data = 0x09ac1f68;
    unsigned int c_15_data = 0x30ca0e12;
    unsigned int c_16_data = 0xa7e18675;

    unsigned int mask_b_1 = 0x00000001;
    unsigned int mask_b_2 = 0x00000001;

    unsigned int sram_addr_output_encrypted_data = (sram_addr_base + 0x200);

    unsigned int w_output_token_result_addr = 0x10000000;
    unsigned int r_output_token_data;
    unsigned int c_output_token_pass_data = 0xd82c;
    unsigned int w_output_token_pass_data = 0xf0f0f0f0;
    unsigned int w_output_token_fail_data = 0x48484848;

    unsigned int sram_addr_read_13 = (sram_addr_base + 0x240);

    unsigned int r_1_addr = (sram_addr_output_encrypted + 0x0);
    unsigned int r_2_addr = (sram_addr_output_encrypted + 0x4);
    unsigned int r_3_addr = (sram_addr_output_encrypted + 0x8);
    unsigned int r_4_addr = (sram_addr_output_encrypted + 0xc);
    unsigned int r_5_addr = (sram_addr_output_encrypted + 0x10);
    unsigned int r_6_addr = (sram_addr_output_encrypted + 0x14);
    unsigned int r_7_addr = (sram_addr_output_encrypted + 0x18);
    unsigned int r_8_addr = (sram_addr_output_encrypted + 0x1c);
    unsigned int r_9_addr = (sram_addr_output_encrypted + 0x20);
    unsigned int r_10_addr = (sram_addr_output_encrypted + 0x24);
    unsigned int r_11_addr = (sram_addr_output_encrypted + 0x28);
    unsigned int r_12_addr = (sram_addr_output_encrypted + 0x2c);
    unsigned int r_13_addr = (sram_addr_output_encrypted + 0x30);
    unsigned int r_14_addr = (sram_addr_output_encrypted + 0x34);
    unsigned int r_15_addr = (sram_addr_output_encrypted + 0x38);
    unsigned int r_16_addr = (sram_addr_output_encrypted + 0x3c);

    unsigned int r_1_data = 0;
    unsigned int r_2_data = 0;
    unsigned int r_3_data = 0;
    unsigned int r_4_data = 0;
    unsigned int r_5_data = 0;
    unsigned int r_6_data = 0;
    unsigned int r_7_data = 0;
    unsigned int r_8_data = 0;
    unsigned int r_9_data = 0;
    unsigned int r_10_data = 0;
    unsigned int r_11_data = 0;
    unsigned int r_12_data = 0;
    unsigned int r_13_data = 0;
    unsigned int r_14_data = 0;
    unsigned int r_15_data = 0;
    unsigned int r_16_data = 0;

    unsigned int scrt0_read_addr_2 = (SCRT0_BASE_ADDR + 0x0008);

    unsigned int c2_1_data = 0xa1caf13f;
    unsigned int c2_2_data = 0x09ac1f68;
    unsigned int c2_3_data = 0x30ca0e12;
    unsigned int c2_4_data = 0xa7e18675;

    unsigned int w2_1_data = 0x0104d82c;
    unsigned int w2_2_data = 0x00000000;
    unsigned int w2_3_data = 0x00000040;
    unsigned int w2_4_data = 0x00410000;
    unsigned int w2_5_data = 0x00000000;
    unsigned int w2_6_data = 0x00000040;
    unsigned int w2_7_data = 0x00411000;
    unsigned int w2_8_data = 0x00000000;
    unsigned int w2_9_data = 0x00000044;
    unsigned int w2_10_data = 0x00000000;
    unsigned int w2_11_data = 0x00000000;
    unsigned int w2_12_data = 0x00018010;
    unsigned int w2_13_data = 0x00000000;
    unsigned int w2_14_data = 0x03020100;
    unsigned int w2_15_data = 0x07060504;
    unsigned int w2_16_data = 0x0b0a0908;
    unsigned int w2_17_data = 0x0f0e0d0c;
    unsigned int w2_18_data = 0x16157e2b;
    unsigned int w2_19_data = 0xa6d2ae28;
    unsigned int w2_20_data = 0x8815f7ab;
    unsigned int w2_21_data = 0x3c4fcf09;
    unsigned int w2_22_data = 0x00000000;
    unsigned int w2_23_data = 0x00000000;
    unsigned int w2_24_data = 0x00000000;
    unsigned int w2_25_data = 0x00000000;

    //Mailbox 1
    *(volatile unsigned int *)0x60003f00 = 0x4;

    //Mailbox 2
    *(volatile unsigned int *)0x60003f00 = 0x40;

    //Mailbox 3
    *(volatile unsigned int *)0x60003f00 = 0x400;

    //Mailbox 4
    *(volatile unsigned int *)0x60003f00 = 0x4000;

    //Mailbox 1 again
    *(volatile unsigned int *)0x60003f00 = 0x4;

    //Burst_write 64 bytes in SRAM, this is plain text data
    *(volatile unsigned int *)0x00410000 = w_1_data;
    *(volatile unsigned int *)0x00410004 = w_2_data;
    *(volatile unsigned int *)0x00410008 = w_3_data;
    *(volatile unsigned int *)0x0041000c = w_4_data;
    *(volatile unsigned int *)0x00410010 = w_5_data;
    *(volatile unsigned int *)0x00410014 = w_6_data;
    *(volatile unsigned int *)0x00410018 = w_7_data;
    *(volatile unsigned int *)0x0041001c = w_8_data;
    *(volatile unsigned int *)0x00410020 = w_9_data;
    *(volatile unsigned int *)0x00410024 = w_10_data;
    *(volatile unsigned int *)0x00410028 = w_11_data;
    *(volatile unsigned int *)0x0041002c = w_12_data;
    *(volatile unsigned int *)0x00410030 = w_13_data;
    *(volatile unsigned int *)0x00410034 = w_14_data;
    *(volatile unsigned int *)0x00410038 = w_15_data;
    *(volatile unsigned int *)0x0041003c = w_16_data;

    //Polling -1
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Burst write 100 bytes
    *(volatile unsigned int *)0x60000000 = w2_1_data;
    *(volatile unsigned int *)0x60000004 = w2_2_data;
    *(volatile unsigned int *)0x60000008 = w2_3_data;
    *(volatile unsigned int *)0x6000000c = w2_4_data;
    *(volatile unsigned int *)0x60000010 = w2_5_data;
    *(volatile unsigned int *)0x60000014 = w2_6_data;
    *(volatile unsigned int *)0x60000018 = w2_7_data;
    *(volatile unsigned int *)0x6000001c = w2_8_data;
    *(volatile unsigned int *)0x60000020 = w2_9_data;
    *(volatile unsigned int *)0x60000024 = w2_10_data;
    *(volatile unsigned int *)0x60000028 = w2_11_data;
    *(volatile unsigned int *)0x6000002c = w2_12_data;
    *(volatile unsigned int *)0x60000030 = w2_13_data;
    *(volatile unsigned int *)0x60000034 = w2_14_data;
    *(volatile unsigned int *)0x60000038 = w2_15_data;
    *(volatile unsigned int *)0x6000003c = w2_16_data;
    *(volatile unsigned int *)0x60000040 = w2_17_data;
    *(volatile unsigned int *)0x60000044 = w2_18_data;
    *(volatile unsigned int *)0x60000048 = w2_19_data;
    *(volatile unsigned int *)0x6000004c = w2_20_data;
    *(volatile unsigned int *)0x60000050 = w2_21_data;
    *(volatile unsigned int *)0x60000054 = w2_22_data;
    *(volatile unsigned int *)0x60000058 = w2_23_data;
    *(volatile unsigned int *)0x6000005c = w2_24_data;
    *(volatile unsigned int *)0x60000060 = w2_25_data;

    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;

    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }


    //Check output token status
    r_output_token_data = *((volatile unsigned int *)0x60000000);

    if (r_output_token_data == c_output_token_pass_data) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Check block 64 bytes output encrypted data in SRAM
    r_1_data = *((volatile unsigned int *)0x00411000);
    r_2_data = *((volatile unsigned int *)0x00411004);
    r_3_data = *((volatile unsigned int *)0x00411008);
    r_4_data = *((volatile unsigned int *)0x0041100c);
    r_5_data = *((volatile unsigned int *)0x00411010);
    r_6_data = *((volatile unsigned int *)0x00411014);
    r_7_data = *((volatile unsigned int *)0x00411018);
    r_8_data = *((volatile unsigned int *)0x0041101c);
    r_9_data = *((volatile unsigned int *)0x00411020);
    r_10_data = *((volatile unsigned int *)0x00411024);
    r_11_data = *((volatile unsigned int *)0x00411028);
    r_12_data = *((volatile unsigned int *)0x0041102c);
    r_13_data = *((volatile unsigned int *)0x00411030);
    r_14_data = *((volatile unsigned int *)0x00411034);
    r_15_data = *((volatile unsigned int *)0x00411038);
    r_16_data = *((volatile unsigned int *)0x0041103c);

    if ((r_1_data == c_1_data)
        &&(r_2_data == c_2_data)
        &&(r_3_data == c_3_data)
        &&(r_4_data == c_4_data)
        &&(r_5_data == c_5_data)
        &&(r_6_data == c_6_data)
        &&(r_7_data == c_7_data)
        &&(r_8_data == c_8_data)
        &&(r_9_data == c_9_data)
        &&(r_10_data == c_10_data)
        &&(r_11_data == c_11_data)
        &&(r_12_data == c_12_data)
        &&(r_13_data == c_13_data)
        &&(r_14_data == c_14_data)
        &&(r_15_data == c_15_data)
        &&(r_16_data == c_16_data)
        ) {
        //compare pass
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        //compare fail
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Check Token ID in output data's last word
    r_output_token_data = *((volatile unsigned int *)0x00411040);

    if (r_output_token_data == c_output_token_pass_data) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Check 16 bytes in offset 0x0008
    r_1_data = *((volatile unsigned int *)0x60000008);
    r_2_data = *((volatile unsigned int *)0x6000000c);
    r_3_data = *((volatile unsigned int *)0x60000010);
    r_4_data = *((volatile unsigned int *)0x60000014);

    if ( (r_1_data == c2_1_data)
         &&(r_1_data == c2_2_data)
         &&(r_1_data == c2_3_data)
         &&(r_1_data == c2_4_data)
         ){
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Write
    *(volatile unsigned int *)flag_addr = 0x2;

}


void test_onsram_scrt_sram_burst(void) //it pass in Firdous's simulation test
                                               //Run this test on SRAM
                                               //Need to combine to Vincent to bootloader code, he will generate the new image to me, I need to convert it to hex file to Firdous
{
    unsigned int sram_addr   = 0x00410000;
    unsigned int result_addr = 0x0040a800;
    unsigned int flag_addr   = 0x0040a900;

    unsigned int w_1_addr = 0x00410000;
    unsigned int w_2_addr = 0x00410004;
    unsigned int w_3_addr = 0x00410008;
    unsigned int w_4_addr = 0x0041000c;
    unsigned int w_5_addr = 0x00410010;
    unsigned int w_6_addr = 0x00410014;

    unsigned int w_1_data = 0x11223344;
    unsigned int w_2_data = 0x55667788;
    unsigned int w_3_data = 0x99aabbcc;
    unsigned int w_4_data = 0xddeef00f;
    unsigned int w_5_data = 0xf9ab1357;
    unsigned int w_6_data = 0x24689876;

    unsigned int r_1_data = 0;
    unsigned int r_2_data = 0;
    unsigned int r_3_data = 0;
    unsigned int r_4_data = 0;
    unsigned int r_5_data = 0;
    unsigned int r_6_data = 0;

    unsigned int val_pass = 0xf0f0f0f0;
    unsigned int val_fail = 0x48484848;
    unsigned int result_data = 0;

    //write
    *(volatile unsigned int *)w_1_addr = w_1_data;
    *(volatile unsigned int *)w_2_addr = w_2_data;
    *(volatile unsigned int *)w_3_addr = w_3_data;
    *(volatile unsigned int *)w_4_addr = w_4_data;
    *(volatile unsigned int *)w_5_addr = w_5_data;
    *(volatile unsigned int *)w_6_addr = w_6_data;

    //read
    r_1_data = *((volatile unsigned int *)w_1_addr);
    r_2_data = *((volatile unsigned int *)w_2_addr);
    r_3_data = *((volatile unsigned int *)w_3_addr);
    r_4_data = *((volatile unsigned int *)w_4_addr);
    r_5_data = *((volatile unsigned int *)w_5_addr);
    r_6_data = *((volatile unsigned int *)w_6_addr);

    //compare
    if ((r_1_data == w_1_data)
      &&(r_2_data == w_2_data)
      &&(r_3_data == w_3_data)
      &&(r_4_data == w_4_data)
      &&(r_5_data == w_5_data)
      &&(r_6_data == w_6_data))
    {
      *((volatile unsigned int *)result_addr) = val_pass;
    } else {
      *((volatile unsigned int *)result_addr) = val_fail;
    }

    result_data = *((volatile unsigned int *)result_addr);

    *(volatile unsigned int *)flag_addr = 0x10101010;

}


void test_onsram_scrt_sram_burst_oneword(void) //it pass in Firdous's simulation test
                                                           //Run this test on SRAM
                                                           //Need to combine to Vincent to bootloader code, he will generate the new image to me, I need to convert it to hex file to Firdous

{
    unsigned int sram_addr   = 0x00410000;

    unsigned int result_addr = 0x0040a800;
    unsigned int flag_addr   = 0x0040a900;

    unsigned int w_1_data = 0x11223344;
    unsigned int r_1_data = 0;

    unsigned int val_pass = 0xf0f0f0f0;
    unsigned int val_fail = 0x48484848;
    unsigned int result_data = 0;

    //write
    *(volatile unsigned int *)0x00410000 = 0x11223344;

    //read
    r_1_data = *((volatile unsigned int *)0x00410000);

    //compare
    if (r_1_data == w_1_data)
    {
      *((volatile unsigned int *)result_addr) = val_pass;
    } else {
      *((volatile unsigned int *)result_addr) = val_fail;
    }

    result_data = *((volatile unsigned int *)result_addr);

    *(volatile unsigned int *)flag_addr = 0x10101010;

}


void test_onsram_scrt_mailbox_link(void) //it pass in Firdous's simulation test

{
    //Mailbox 1
    *(volatile unsigned int *)0x60003f00 = 0x4;

    //Mailbox 2
    *(volatile unsigned int *)0x60003f00 = 0x40;

    //Mailbox 3
    *(volatile unsigned int *)0x60003f00 = 0x400;

    //Mailbox 4
    *(volatile unsigned int *)0x60003f00 = 0x4000;

    //Mailbox 1 again
    *(volatile unsigned int *)0x60003f00 = 0x4;


}


void test_onsram_scrt_do_aes_sixty_four_bytes(void) //it pass in Firdous's simulation test

{
    unsigned int data;

    unsigned int result_addr = 0x0040a800;
    unsigned int flag_addr   = 0x0040a900;
    unsigned int r_output_token_data;
    unsigned int c_output_token_pass_data = 0xd82c;
    unsigned int w_output_token_pass_data = 0xf0f0f0f0;
    unsigned int w_output_token_fail_data = 0x48484848;

    unsigned int c_1_data = 0xacab4976;
    unsigned int c_2_data = 0x46b21981;
    unsigned int c_3_data = 0x9b8ee9ce;
    unsigned int c_4_data = 0x7d19e912;
    unsigned int c_5_data = 0x9bcb8650;
    unsigned int c_6_data = 0xee197250;
    unsigned int c_7_data = 0x3a11db95;
    unsigned int c_8_data = 0xb2787691;
    unsigned int c_9_data = 0xb8d6be73;
    unsigned int c_10_data = 0x3b74c1e3;
    unsigned int c_11_data = 0x9ee61671;
    unsigned int c_12_data = 0x16952222;
    unsigned int c_13_data = 0xa1caf13f;
    unsigned int c_14_data = 0x09ac1f68;
    unsigned int c_15_data = 0x30ca0e12;
    unsigned int c_16_data = 0xa7e18675;

    unsigned int r_1_data = 0;
    unsigned int r_2_data = 0;
    unsigned int r_3_data = 0;
    unsigned int r_4_data = 0;
    unsigned int r_5_data = 0;
    unsigned int r_6_data = 0;
    unsigned int r_7_data = 0;
    unsigned int r_8_data = 0;
    unsigned int r_9_data = 0;
    unsigned int r_10_data = 0;
    unsigned int r_11_data = 0;
    unsigned int r_12_data = 0;
    unsigned int r_13_data = 0;
    unsigned int r_14_data = 0;
    unsigned int r_15_data = 0;
    unsigned int r_16_data = 0;

    unsigned int c2_1_data = 0xa1caf13f;
    unsigned int c2_2_data = 0x09ac1f68;
    unsigned int c2_3_data = 0x30ca0e12;
    unsigned int c2_4_data = 0xa7e18675;


    //Mailbox 1
    *(volatile unsigned int *)0x60003f00 = 0x4;

    //Mailbox 2
    *(volatile unsigned int *)0x60003f00 = 0x40;

    //Mailbox 3
    *(volatile unsigned int *)0x60003f00 = 0x400;

    //Mailbox 4
    *(volatile unsigned int *)0x60003f00 = 0x4000;

    //Mailbox 1 again
    *(volatile unsigned int *)0x60003f00 = 0x4;

    //Burst_write 64 bytes in SRAM @ 0x1000_3000, this is plain text data
    *(volatile unsigned int *)0x0040a000 = 0xe2bec16b;
    *(volatile unsigned int *)0x0040a004 = 0x969f402e;
    *(volatile unsigned int *)0x0040a008 = 0x117e3de9;
    *(volatile unsigned int *)0x0040a00c = 0x2a179373;
    *(volatile unsigned int *)0x0040a010 = 0x578a2dae;
    *(volatile unsigned int *)0x0040a014 = 0x9cac031e;
    *(volatile unsigned int *)0x0040a018 = 0xac6fb79e;
    *(volatile unsigned int *)0x0040a01c = 0x518eaf45;
    *(volatile unsigned int *)0x0040a020 = 0x461cc830;
    *(volatile unsigned int *)0x0040a024 = 0x11e45ca3;
    *(volatile unsigned int *)0x0040a028 = 0x19c1fbe5;
    *(volatile unsigned int *)0x0040a02c = 0xef520a1a;
    *(volatile unsigned int *)0x0040a030 = 0x45249ff6;
    *(volatile unsigned int *)0x0040a034 = 0x179b4fdf;
    *(volatile unsigned int *)0x0040a038 = 0x7b412bad;
    *(volatile unsigned int *)0x0040a03c = 0x10376ce6;

    //Polling -1
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Burst write 100 bytes
    *(volatile unsigned int *)0x60000000 = 0x0104d82c;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = 0x00000040;
    *(volatile unsigned int *)0x6000000c = 0x0040a000;
    *(volatile unsigned int *)0x60000010 = 0x00000000;
    *(volatile unsigned int *)0x60000014 = 0x00000040;
    *(volatile unsigned int *)0x60000018 = 0x0040b000;
    *(volatile unsigned int *)0x6000001c = 0x00000000;
    *(volatile unsigned int *)0x60000020 = 0x00000044;
    *(volatile unsigned int *)0x60000024 = 0x00000000;
    *(volatile unsigned int *)0x60000028 = 0x00000000;
    *(volatile unsigned int *)0x6000002c = 0x00018010;
    *(volatile unsigned int *)0x60000030 = 0x00000000;
    *(volatile unsigned int *)0x60000034 = 0x03020100;
    *(volatile unsigned int *)0x60000038 = 0x07060504;
    *(volatile unsigned int *)0x6000003c = 0x0b0a0908;
    *(volatile unsigned int *)0x60000040 = 0x0f0e0d0c;
    *(volatile unsigned int *)0x60000044 = 0x16157e2b;
    *(volatile unsigned int *)0x60000048 = 0xa6d2ae28;
    *(volatile unsigned int *)0x6000004c = 0x8815f7ab;
    *(volatile unsigned int *)0x60000050 = 0x3c4fcf09;
    *(volatile unsigned int *)0x60000054 = 0x00000000;
    *(volatile unsigned int *)0x60000058 = 0x00000000;
    *(volatile unsigned int *)0x6000005c = 0x00000000;
    *(volatile unsigned int *)0x60000060 = 0x00000000;

    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;

    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }


    //Check output token status
    r_output_token_data = *((volatile unsigned int *)0x60000000);

    if (r_output_token_data == c_output_token_pass_data) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Check block 64 bytes output encrypted data 1 in SRAM
    r_1_data = *((volatile unsigned int *)0x0040b000);
    r_2_data = *((volatile unsigned int *)0x0040b004);
    r_3_data = *((volatile unsigned int *)0x0040b008);
    r_4_data = *((volatile unsigned int *)0x0040b00c);
    r_5_data = *((volatile unsigned int *)0x0040b010);
    r_6_data = *((volatile unsigned int *)0x0040b014);
    r_7_data = *((volatile unsigned int *)0x0040b018);
    r_8_data = *((volatile unsigned int *)0x0040b01c);
    r_9_data = *((volatile unsigned int *)0x0040b020);
    r_10_data = *((volatile unsigned int *)0x0040b024);
    r_11_data = *((volatile unsigned int *)0x0040b028);
    r_12_data = *((volatile unsigned int *)0x0040b02c);
    r_13_data = *((volatile unsigned int *)0x0040b030);
    r_14_data = *((volatile unsigned int *)0x0040b034);
    r_15_data = *((volatile unsigned int *)0x0040b038);
    r_16_data = *((volatile unsigned int *)0x0040b03c);

    if ((r_1_data == c_1_data)
        &&(r_2_data == c_2_data)
        &&(r_3_data == c_3_data)
        &&(r_4_data == c_4_data)
        &&(r_5_data == c_5_data)
        &&(r_6_data == c_6_data)
        &&(r_7_data == c_7_data)
        &&(r_8_data == c_8_data)
        &&(r_9_data == c_9_data)
        &&(r_10_data == c_10_data)
        &&(r_11_data == c_11_data)
        &&(r_12_data == c_12_data)
        &&(r_13_data == c_13_data)
        &&(r_14_data == c_14_data)
        &&(r_15_data == c_15_data)
        &&(r_16_data == c_16_data)
        ) {
        //compare pass
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        //compare fail
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Check Token ID in output data's last word
    r_output_token_data = *((volatile unsigned int *)0x0040b040);

    if (r_output_token_data == c_output_token_pass_data) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Check 16 bytes in offset 0x0008
    r_1_data = *((volatile unsigned int *)0x60000008);
    r_2_data = *((volatile unsigned int *)0x6000000c);
    r_3_data = *((volatile unsigned int *)0x60000010);
    r_4_data = *((volatile unsigned int *)0x60000014);

    if ( (r_1_data == c2_1_data)
         &&(r_1_data == c2_2_data)
         &&(r_1_data == c2_3_data)
         &&(r_1_data == c2_4_data)
         ){
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Write
    *(volatile unsigned int *)0x60003f00 = 0x2;

}


void test_onsram_scrt_do_aes_2k_bytes(void) //it pass in Firdous's simulation test
{
    unsigned int c_output_token_pass_data = 0xd82c;
    unsigned int w_output_token_pass_data = 0xf0f0f0f0;
    unsigned int w_output_token_fail_data = 0x48484848;
    unsigned int result_addr = 0x0040c000;
    unsigned int w_data_word = 0x12345678;
    unsigned int o_data_word = 0;
    unsigned int r_1_data = 0;
    unsigned int r_2_data = 0;
    unsigned int r_3_data = 0;
    unsigned int r_4_data = 0;
    unsigned int c2_1_data = 0xa1caf13f;
    unsigned int c2_2_data = 0x09ac1f68;
    unsigned int c2_3_data = 0x30ca0e12;
    unsigned int c2_4_data = 0xa7e18675;
    unsigned int r_output_token_data;
    unsigned int data = 0;
    unsigned int i = 0;

    //Mailbox 1
    *(volatile unsigned int *)0x60003f00 = 0x4;

    //Mailbox 2
    *(volatile unsigned int *)0x60003f00 = 0x40;

    //Mailbox 3
    *(volatile unsigned int *)0x60003f00 = 0x400;

    //Mailbox 4
    *(volatile unsigned int *)0x60003f00 = 0x4000;

    //Mailbox 1 again
    *(volatile unsigned int *)0x60003f00 = 0x4;


    //Burst_write 2K bytes in SRAM
    for (i=0; i<500; i++) {
        *((volatile unsigned int *)(0x0040a000 + (i*4))) = w_data_word;
    }

    //Polling -1
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Write Control Token
    //Firmware Reference Manual pdf: 5.2.1 Encryption Input Token
    *(volatile unsigned int *)0x60000000 = 0x0104d82c;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = 0x000007d0;
    *(volatile unsigned int *)0x6000000c = 0x0040a000;
    *(volatile unsigned int *)0x60000010 = 0x00000000;
    *(volatile unsigned int *)0x60000014 = 0x000007d0;
    *(volatile unsigned int *)0x60000018 = 0x0040b000;
    *(volatile unsigned int *)0x6000001c = 0x00000000;
    *(volatile unsigned int *)0x60000020 = 0x000007d4;
    *(volatile unsigned int *)0x60000024 = 0x00000000;
    *(volatile unsigned int *)0x60000028 = 0x00000000;
    *(volatile unsigned int *)0x6000002c = 0x00018010;
    *(volatile unsigned int *)0x60000030 = 0x00000000;
    *(volatile unsigned int *)0x60000034 = 0x03020100;
    *(volatile unsigned int *)0x60000038 = 0x07060504;
    *(volatile unsigned int *)0x6000003c = 0x0b0a0908;
    *(volatile unsigned int *)0x60000040 = 0x0f0e0d0c;
    *(volatile unsigned int *)0x60000044 = 0x16157e2b;
    *(volatile unsigned int *)0x60000048 = 0xa6d2ae28;
    *(volatile unsigned int *)0x6000004c = 0x8815f7ab;
    *(volatile unsigned int *)0x60000050 = 0x3c4fcf09;
    *(volatile unsigned int *)0x60000054 = 0x00000000;
    *(volatile unsigned int *)0x60000058 = 0x00000000;
    *(volatile unsigned int *)0x6000005c = 0x00000000;
    *(volatile unsigned int *)0x60000060 = 0x00000000;


    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;


    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }


    //Check output token status
    r_output_token_data = *((volatile unsigned int *)0x60000000);

    if (r_output_token_data == c_output_token_pass_data) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Read output encrypted 2K bytes data in SRAM
    for (i=0; i<500; i++) {
        o_data_word = *((volatile unsigned int *)(0x0040b000 + (i*4)));
    }

    //Don't compare
    //ignore


    //Check Token ID in output data's last word
    r_output_token_data = *((volatile unsigned int *)0x0040b7d0);

    if (r_output_token_data == c_output_token_pass_data) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Check 16 bytes in offset 0x0008
    r_1_data = *((volatile unsigned int *)0x60000008);
    r_2_data = *((volatile unsigned int *)0x6000000c);
    r_3_data = *((volatile unsigned int *)0x60000010);
    r_4_data = *((volatile unsigned int *)0x60000014);

    if ( (r_1_data == c2_1_data)
        &&(r_1_data == c2_2_data)
        &&(r_1_data == c2_3_data)
        &&(r_1_data == c2_4_data)
        ){
       *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
       *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Write
    *(volatile unsigned int *)0x60003f00 = 0x2;

}


void test_onsram_scrt_do_aes_2k_bytes_src_dst_addr_the_same(void) //it pass in Firdous's simulation test
                                                                                     //Configure the output address the same with input address
{
    unsigned int c_output_token_pass_data = 0xd82c;
    unsigned int w_output_token_pass_data = 0xf0f0f0f0;
    unsigned int w_output_token_fail_data = 0x48484848;
    unsigned int result_addr = 0x0040c000;
    unsigned int w_data_word = 0x12345678;
    unsigned int o_data_word = 0;
    unsigned int r_1_data = 0;
    unsigned int r_2_data = 0;
    unsigned int r_3_data = 0;
    unsigned int r_4_data = 0;
    unsigned int c2_1_data = 0xa1caf13f;
    unsigned int c2_2_data = 0x09ac1f68;
    unsigned int c2_3_data = 0x30ca0e12;
    unsigned int c2_4_data = 0xa7e18675;
    unsigned int r_output_token_data;
    unsigned int data = 0;
    unsigned int i = 0;

    //Mailbox 1
    *(volatile unsigned int *)0x60003f00 = 0x4;

    //Mailbox 2
    *(volatile unsigned int *)0x60003f00 = 0x40;

    //Mailbox 3
    *(volatile unsigned int *)0x60003f00 = 0x400;

    //Mailbox 4
    *(volatile unsigned int *)0x60003f00 = 0x4000;

    //Mailbox 1 again
    *(volatile unsigned int *)0x60003f00 = 0x4;


    //Burst_write 2K bytes in SRAM
    for (i=0; i<500; i++) {
        *((volatile unsigned int *)(0x0040a000 + (i*4))) = w_data_word;
    }

    //Polling -1
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Write Control Token
    //Firmware Reference Manual pdf: 5.2.1 Encryption Input Token
    *(volatile unsigned int *)0x60000000 = 0x0104d82c;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = 0x000007d0;
    *(volatile unsigned int *)0x6000000c = 0x0040a000;
    *(volatile unsigned int *)0x60000010 = 0x00000000;
    *(volatile unsigned int *)0x60000014 = 0x000007d0;
    *(volatile unsigned int *)0x60000018 = 0x0040a000;//0x0040b000;
    *(volatile unsigned int *)0x6000001c = 0x00000000;
    *(volatile unsigned int *)0x60000020 = 0x000007d4;
    *(volatile unsigned int *)0x60000024 = 0x00000000;
    *(volatile unsigned int *)0x60000028 = 0x00000000;
    *(volatile unsigned int *)0x6000002c = 0x00018010;
    *(volatile unsigned int *)0x60000030 = 0x00000000;
    *(volatile unsigned int *)0x60000034 = 0x03020100;
    *(volatile unsigned int *)0x60000038 = 0x07060504;
    *(volatile unsigned int *)0x6000003c = 0x0b0a0908;
    *(volatile unsigned int *)0x60000040 = 0x0f0e0d0c;
    *(volatile unsigned int *)0x60000044 = 0x16157e2b;
    *(volatile unsigned int *)0x60000048 = 0xa6d2ae28;
    *(volatile unsigned int *)0x6000004c = 0x8815f7ab;
    *(volatile unsigned int *)0x60000050 = 0x3c4fcf09;
    *(volatile unsigned int *)0x60000054 = 0x00000000;
    *(volatile unsigned int *)0x60000058 = 0x00000000;
    *(volatile unsigned int *)0x6000005c = 0x00000000;
    *(volatile unsigned int *)0x60000060 = 0x00000000;


    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;


    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }


    //Check output token status
    r_output_token_data = *((volatile unsigned int *)0x60000000);

    if (r_output_token_data == c_output_token_pass_data) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Read output encrypted 2K bytes data in SRAM
    for (i=0; i<500; i++) {
        //o_data_word = *((volatile unsigned int *)(0x0040b000 + (i*4)));
        o_data_word = *((volatile unsigned int *)(0x0040a000 + (i*4)));
    }

    //Don't compare
    //ignore


    //Check Token ID in output data's last word
    //r_output_token_data = *((volatile unsigned int *)0x0040b7d0);
    r_output_token_data = *((volatile unsigned int *)0x0040a7d0);

    if (r_output_token_data == c_output_token_pass_data) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Check 16 bytes in offset 0x0008
    r_1_data = *((volatile unsigned int *)0x60000008);
    r_2_data = *((volatile unsigned int *)0x6000000c);
    r_3_data = *((volatile unsigned int *)0x60000010);
    r_4_data = *((volatile unsigned int *)0x60000014);

    if ( (r_1_data == c2_1_data)
        &&(r_1_data == c2_2_data)
        &&(r_1_data == c2_3_data)
        &&(r_1_data == c2_4_data)
        ){
       *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
       *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Write
    *(volatile unsigned int *)0x60003f00 = 0x2;

}


void test_onsram_scrt_do_aes_64b_1mailbox_3tokens(void) //it pass in Firdous's simulation test
                                                                        //AES 64K Bytes Encryption on mailbox 1 with 3 tokens
{
    unsigned int data;

    unsigned int result_addr                = 0x0040a800;
    unsigned int flag_addr                  = 0x0040a900;

    unsigned int r_output_token_data;
    unsigned int r_output_token_data_f;

    unsigned int w_output_token_pass_data = 0xf0f0f0f0;
    unsigned int w_output_token_fail_data = 0x48484848;

    unsigned int c_output_token_id_1 = 0xd82c;
    unsigned int c_output_token_id_2 = 0x55fe;
    unsigned int c_output_token_id_3 = 0x0a68;

    unsigned int r_1_data = 0;
    unsigned int r_2_data = 0;
    unsigned int r_3_data = 0;
    unsigned int r_4_data = 0;
    unsigned int r_5_data = 0;
    unsigned int r_6_data = 0;
    unsigned int r_7_data = 0;
    unsigned int r_8_data = 0;
    unsigned int r_9_data = 0;
    unsigned int r_10_data = 0;
    unsigned int r_11_data = 0;
    unsigned int r_12_data = 0;
    unsigned int r_13_data = 0;
    unsigned int r_14_data = 0;
    unsigned int r_15_data = 0;
    unsigned int r_16_data = 0;

    unsigned int c_1_data_t1 = 0xacab4976;
    unsigned int c_2_data_t1 = 0x46b21981;
    unsigned int c_3_data_t1 = 0x9b8ee9ce;
    unsigned int c_4_data_t1 = 0x7d19e912;
    unsigned int c_5_data_t1 = 0x9bcb8650;
    unsigned int c_6_data_t1 = 0xee197250;
    unsigned int c_7_data_t1 = 0x3a11db95;
    unsigned int c_8_data_t1 = 0xb2787691;
    unsigned int c_9_data_t1 = 0xb8d6be73;
    unsigned int c_10_data_t1 = 0x3b74c1e3;
    unsigned int c_11_data_t1 = 0x9ee61671;
    unsigned int c_12_data_t1 = 0x16952222;
    unsigned int c_13_data_t1 = 0xa1caf13f;
    unsigned int c_14_data_t1 = 0x09ac1f68;
    unsigned int c_15_data_t1 = 0x30ca0e12;
    unsigned int c_16_data_t1 = 0xa7e18675;

    unsigned int c_1_data_t2 = 0xB21D024F;
    unsigned int c_2_data_t2 = 0x3D63BC43;
    unsigned int c_3_data_t2 = 0x3A187871;
    unsigned int c_4_data_t2 = 0xE871A09F;
    unsigned int c_5_data_t2 = 0xA9ADD9B4;
    unsigned int c_6_data_t2 = 0xF4ED7DAD;
    unsigned int c_7_data_t2 = 0x7638E7E5;
    unsigned int c_8_data_t2 = 0x5A14693F;
    unsigned int c_9_data_t2 = 0x20241B57;
    unsigned int c_10_data_t2 = 0xE07AFB12;
    unsigned int c_11_data_t2 = 0xACBAA97F;
    unsigned int c_12_data_t2 = 0xE002F13D;
    unsigned int c_13_data_t2 = 0x79E2B008;
    unsigned int c_14_data_t2 = 0x81885988;
    unsigned int c_15_data_t2 = 0xE6A920D9;
    unsigned int c_16_data_t2 = 0xCD15564F;

    unsigned int c_1_data_t3 = 0x044C8CF5;
    unsigned int c_2_data_t3 = 0xBAF1E5D6;
    unsigned int c_3_data_t3 = 0xFBAB9E77;
    unsigned int c_4_data_t3 = 0xD6FB7B5F;
    unsigned int c_5_data_t3 = 0x964EFC9C;
    unsigned int c_6_data_t3 = 0x8D80DB7E;
    unsigned int c_7_data_t3 = 0x7B779F67;
    unsigned int c_8_data_t3 = 0x7D2C70C6;
    unsigned int c_9_data_t3 = 0x6933F239;
    unsigned int c_10_data_t3 = 0xCFBAD9A9;
    unsigned int c_11_data_t3 = 0x63E230A5;
    unsigned int c_12_data_t3 = 0x61142304;
    unsigned int c_13_data_t3 = 0xE205EBB2;
    unsigned int c_14_data_t3 = 0xFCE99BC3;
    unsigned int c_15_data_t3 = 0x07196CDA;
    unsigned int c_16_data_t3 = 0x1B9D6A8C;

    unsigned int c1_1_data = 0xa1caf13f;
    unsigned int c1_2_data = 0x09ac1f68;
    unsigned int c1_3_data = 0x30ca0e12;
    unsigned int c1_4_data = 0xa7e18675;

    unsigned int c2_1_data = 0x79e2b008;
    unsigned int c2_2_data = 0x81885988;
    unsigned int c2_3_data = 0xe6a920d9;
    unsigned int c2_4_data = 0xcd15564f;

    unsigned int c3_1_data = 0xe205ebb2;
    unsigned int c3_2_data = 0xfce99bc3;
    unsigned int c3_3_data = 0x07196cda;
    unsigned int c3_4_data = 0x1b9d6a8c;

    //aes_cbc_nist: 1
    //Mailbox 1
    *(volatile unsigned int *)0x60003f00 = 0x4;


    /*
        ===================================
            Plain text 1
        ===================================
      */
    //aes_cbc_nist: 2
    //Burst_write 64 bytes plain text data in SRAM
    *((volatile unsigned int *)0x00410000) = 0xe2bec16b;
    *((volatile unsigned int *)0x00410004) = 0x969f402e;
    *((volatile unsigned int *)0x00410008) = 0x117e3de9;
    *((volatile unsigned int *)0x0041000c) = 0x2a179373;
    *((volatile unsigned int *)0x00410010) = 0x578a2dae;
    *((volatile unsigned int *)0x00410014) = 0x9cac031e;
    *((volatile unsigned int *)0x00410018) = 0xac6fb79e;
    *((volatile unsigned int *)0x0041001c) = 0x518eaf45;
    *((volatile unsigned int *)0x00410020) = 0x461cc830;
    *((volatile unsigned int *)0x00410024) = 0x11e45ca3;
    *((volatile unsigned int *)0x00410028) = 0x19c1fbe5;
    *((volatile unsigned int *)0x0041002c) = 0xef520a1a;
    *((volatile unsigned int *)0x00410030) = 0x45249ff6;
    *((volatile unsigned int *)0x00410034) = 0x179b4fdf;
    *((volatile unsigned int *)0x00410038) = 0x7b412bad;
    *((volatile unsigned int *)0x0041003c) = 0x10376ce6;


    //aes_cbc_nist: 3
    //Check mailbox is empty
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }


    //aes_cbc_nist: 4
    //Burst write 100 bytes (token1) in mailbox @ 0x6000_0000
    *(volatile unsigned int *)0x60000000 = 0x0104d82c;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = 0x00000040;
    *(volatile unsigned int *)0x6000000c = 0x00410000;
    *(volatile unsigned int *)0x60000010 = 0x00000000;
    *(volatile unsigned int *)0x60000014 = 0x00000040;
    *(volatile unsigned int *)0x60000018 = 0x00411000;
    *(volatile unsigned int *)0x6000001c = 0x00000000;
    *(volatile unsigned int *)0x60000020 = 0x00000044;
    *(volatile unsigned int *)0x60000024 = 0x00000000;
    *(volatile unsigned int *)0x60000028 = 0x00000000;
    *(volatile unsigned int *)0x6000002c = 0x00018010;
    *(volatile unsigned int *)0x60000030 = 0x00000000;
    *(volatile unsigned int *)0x60000034 = 0x03020100;
    *(volatile unsigned int *)0x60000038 = 0x07060504;
    *(volatile unsigned int *)0x6000003c = 0x0b0a0908;
    *(volatile unsigned int *)0x60000040 = 0x0f0e0d0c;
    *(volatile unsigned int *)0x60000044 = 0x16157e2b;
    *(volatile unsigned int *)0x60000048 = 0xa6d2ae28;
    *(volatile unsigned int *)0x6000004c = 0x8815f7ab;
    *(volatile unsigned int *)0x60000050 = 0x3c4fcf09;
    *(volatile unsigned int *)0x60000054 = 0x00000000;
    *(volatile unsigned int *)0x60000058 = 0x00000000;
    *(volatile unsigned int *)0x6000005c = 0x00000000;
    *(volatile unsigned int *)0x60000060 = 0x00000000;


    //aes_cbc_nist: 5
    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;


    /*
        ===================================
            Plain text 2
        ===================================
      */
    //aes_cbc_nist: 6
    //Burst_write 64 bytes plain text data in SRAM
    *((volatile unsigned int *)0x00420000) = 0xe2bec16b;
    *((volatile unsigned int *)0x00420004) = 0x969f402e;
    *((volatile unsigned int *)0x00420008) = 0x117e3de9;
    *((volatile unsigned int *)0x0042000c) = 0x2a179373;
    *((volatile unsigned int *)0x00420010) = 0x578a2dae;
    *((volatile unsigned int *)0x00420014) = 0x9cac031e;
    *((volatile unsigned int *)0x00420018) = 0xac6fb79e;
    *((volatile unsigned int *)0x0042001c) = 0x518eaf45;
    *((volatile unsigned int *)0x00420020) = 0x461cc830;
    *((volatile unsigned int *)0x00420024) = 0x11e45ca3;
    *((volatile unsigned int *)0x00420028) = 0x19c1fbe5;
    *((volatile unsigned int *)0x0042002c) = 0xef520a1a;
    *((volatile unsigned int *)0x00420030) = 0x45249ff6;
    *((volatile unsigned int *)0x00420034) = 0x179b4fdf;
    *((volatile unsigned int *)0x00420038) = 0x7b412bad;
    *((volatile unsigned int *)0x0042003c) = 0x10376ce6;


    //aes_cbc_nist: 7
    //Polling to check mailbox is empty
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }


    //aes_cbc_nist: 8
    //Burst write 100 bytes (token2) in mailbox @ 0x6000_0000
    *(volatile unsigned int *)0x60000000 = 0x010455fe;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = 0x00000040;
    *(volatile unsigned int *)0x6000000c = 0x00420000;
    *(volatile unsigned int *)0x60000010 = 0x00000000;
    *(volatile unsigned int *)0x60000014 = 0x00000040;
    *(volatile unsigned int *)0x60000018 = 0x00421000;
    *(volatile unsigned int *)0x6000001c = 0x00000000;
    *(volatile unsigned int *)0x60000020 = 0x00000044;
    *(volatile unsigned int *)0x60000024 = 0x00000000;
    *(volatile unsigned int *)0x60000028 = 0x00000000;
    *(volatile unsigned int *)0x6000002c = 0x00028010;
    *(volatile unsigned int *)0x60000030 = 0x00000000;
    *(volatile unsigned int *)0x60000034 = 0x03020100;
    *(volatile unsigned int *)0x60000038 = 0x07060504;
    *(volatile unsigned int *)0x6000003c = 0x0b0a0908;
    *(volatile unsigned int *)0x60000040 = 0x0f0e0d0c;
    *(volatile unsigned int *)0x60000044 = 0xf7b0738e;
    *(volatile unsigned int *)0x60000048 = 0x52640eda;
    *(volatile unsigned int *)0x6000004c = 0x2bf310c8;
    *(volatile unsigned int *)0x60000050 = 0xe5799080;
    *(volatile unsigned int *)0x60000054 = 0xd2eaf862;
    *(volatile unsigned int *)0x60000058 = 0x7b6b2c52;
    *(volatile unsigned int *)0x6000005c = 0x00000000;
    *(volatile unsigned int *)0x60000060 = 0x00000000;


    //aes_cbc_nist: 9
    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;


    //aes_cbc_nist: 10
    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }


    //aes_cbc_nist: 11
    //Check output token 1 's Token ID
    r_output_token_data = *((volatile unsigned int *)0x60000000);

    if (r_output_token_data == c_output_token_id_1) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }


    //aes_cbc_nist: 12
    //Check block 64 bytes output encrypted data 1 in SRAM
    r_1_data = *((volatile unsigned int *)0x00411000);
    r_2_data = *((volatile unsigned int *)0x00411004);
    r_3_data = *((volatile unsigned int *)0x00411008);
    r_4_data = *((volatile unsigned int *)0x0041100c);
    r_5_data = *((volatile unsigned int *)0x00411010);
    r_6_data = *((volatile unsigned int *)0x00411014);
    r_7_data = *((volatile unsigned int *)0x00411018);
    r_8_data = *((volatile unsigned int *)0x0041101c);
    r_9_data = *((volatile unsigned int *)0x00411020);
    r_10_data = *((volatile unsigned int *)0x00411024);
    r_11_data = *((volatile unsigned int *)0x00411028);
    r_12_data = *((volatile unsigned int *)0x0041102c);
    r_13_data = *((volatile unsigned int *)0x00411030);
    r_14_data = *((volatile unsigned int *)0x00411034);
    r_15_data = *((volatile unsigned int *)0x00411038);
    r_16_data = *((volatile unsigned int *)0x0041103c);

    //compare output encrypted data of token 1
    if ((r_1_data == c_1_data_t1)
        &&(r_2_data == c_2_data_t1)
        &&(r_3_data == c_3_data_t1)
        &&(r_4_data == c_4_data_t1)
        &&(r_5_data == c_5_data_t1)
        &&(r_6_data == c_6_data_t1)
        &&(r_7_data == c_7_data_t1)
        &&(r_8_data == c_8_data_t1)
        &&(r_9_data == c_9_data_t1)
        &&(r_10_data == c_10_data_t1)
        &&(r_11_data == c_11_data_t1)
        &&(r_12_data == c_12_data_t1)
        &&(r_13_data == c_13_data_t1)
        &&(r_14_data == c_14_data_t1)
        &&(r_15_data == c_15_data_t1)
        &&(r_16_data == c_16_data_t1)
        ) {
        //compare pass
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        //compare fail
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }


    //aes_cbc_nist: 13
    //Check toke id in output encrypted data attached of token 1
    r_output_token_data = *((volatile unsigned int *)0x00411040);
    r_output_token_data_f = (r_output_token_data & 0xFFFF);

    if (r_output_token_data_f == c_output_token_id_1) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }


    //aes_cbc_nist: 14
    //Check 16 bytes in offset 0x0008
    r_1_data = *((volatile unsigned int *)0x60000008);
    r_2_data = *((volatile unsigned int *)0x6000000c);
    r_3_data = *((volatile unsigned int *)0x60000010);
    r_4_data = *((volatile unsigned int *)0x60000014);

    if ( (r_1_data == c1_1_data)
         &&(r_2_data == c1_2_data)
         &&(r_3_data == c1_3_data)
         &&(r_4_data == c1_4_data)
         ){
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }


    //aes_cbc_nist: 15
    //Write a word, job1 done and checked, clear mailbox
    *(volatile unsigned int *)0x60003f00 = 0x2;



    /*
        ===================================
            Plain text 3
        ===================================
      */
    //aes_cbc_nist: 16
    //Burst_write 64 bytes plain text data in SRAM
    *((volatile unsigned int *)0x00430000) = 0xe2bec16b;
    *((volatile unsigned int *)0x00430004) = 0x969f402e;
    *((volatile unsigned int *)0x00430008) = 0x117e3de9;
    *((volatile unsigned int *)0x0043000c) = 0x2a179373;
    *((volatile unsigned int *)0x00430010) = 0x578a2dae;
    *((volatile unsigned int *)0x00430014) = 0x9cac031e;
    *((volatile unsigned int *)0x00430018) = 0xac6fb79e;
    *((volatile unsigned int *)0x0043001c) = 0x518eaf45;
    *((volatile unsigned int *)0x00430020) = 0x461cc830;
    *((volatile unsigned int *)0x00430024) = 0x11e45ca3;
    *((volatile unsigned int *)0x00430028) = 0x19c1fbe5;
    *((volatile unsigned int *)0x0043002c) = 0xef520a1a;
    *((volatile unsigned int *)0x00430030) = 0x45249ff6;
    *((volatile unsigned int *)0x00430034) = 0x179b4fdf;
    *((volatile unsigned int *)0x00430038) = 0x7b412bad;
    *((volatile unsigned int *)0x0043003c) = 0x10376ce6;


    //aes_cbc_nist: 17
    //Polling to check mailbox is empty
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }


    //aes_cbc_nist: 18
    //Burst write 100 bytes (token3) in mailbox @ 0x6000_0000
    *(volatile unsigned int *)0x60000000 = 0x01040A68;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = 0x00000040;
    *(volatile unsigned int *)0x6000000c = 0x00430000;
    *(volatile unsigned int *)0x60000010 = 0x00000000;
    *(volatile unsigned int *)0x60000014 = 0x00000040;
    *(volatile unsigned int *)0x60000018 = 0x00431000;
    *(volatile unsigned int *)0x6000001c = 0x00000000;
    *(volatile unsigned int *)0x60000020 = 0x00000044;
    *(volatile unsigned int *)0x60000024 = 0x00000000;
    *(volatile unsigned int *)0x60000028 = 0x00000000;
    *(volatile unsigned int *)0x6000002c = 0x00038010;
    *(volatile unsigned int *)0x60000030 = 0x00000000;
    *(volatile unsigned int *)0x60000034 = 0x03020100;
    *(volatile unsigned int *)0x60000038 = 0x07060504;
    *(volatile unsigned int *)0x6000003c = 0x0b0a0908;
    *(volatile unsigned int *)0x60000040 = 0x0f0e0d0c;
    *(volatile unsigned int *)0x60000044 = 0x10EB3D60;
    *(volatile unsigned int *)0x60000048 = 0xBE71CA15;
    *(volatile unsigned int *)0x6000004c = 0xF0AE732B;
    *(volatile unsigned int *)0x60000050 = 0x81777D85;
    *(volatile unsigned int *)0x60000054 = 0x072C351F;
    *(volatile unsigned int *)0x60000058 = 0xD708613B;
    *(volatile unsigned int *)0x6000005c = 0xA310982D;
    *(volatile unsigned int *)0x60000060 = 0xF4DF1409;


    //aes_cbc_nist: 19
    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;


    //aes_cbc_nist: 20
    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }


    //aes_cbc_nist: 21
    //Check output token 2 's Token ID
    r_output_token_data = *((volatile unsigned int *)0x60000000);

    if (r_output_token_data == c_output_token_id_2) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }


    //aes_cbc_nist: 22
    //Check block 64 bytes output encrypted data 2 in SRAM
    r_1_data = *((volatile unsigned int *)0x00421000);
    r_2_data = *((volatile unsigned int *)0x00421004);
    r_3_data = *((volatile unsigned int *)0x00421008);
    r_4_data = *((volatile unsigned int *)0x0042100c);
    r_5_data = *((volatile unsigned int *)0x00421010);
    r_6_data = *((volatile unsigned int *)0x00421014);
    r_7_data = *((volatile unsigned int *)0x00421018);
    r_8_data = *((volatile unsigned int *)0x0042101c);
    r_9_data = *((volatile unsigned int *)0x00421020);
    r_10_data = *((volatile unsigned int *)0x00421024);
    r_11_data = *((volatile unsigned int *)0x00421028);
    r_12_data = *((volatile unsigned int *)0x0042102c);
    r_13_data = *((volatile unsigned int *)0x00421030);
    r_14_data = *((volatile unsigned int *)0x00421034);
    r_15_data = *((volatile unsigned int *)0x00421038);
    r_16_data = *((volatile unsigned int *)0x0042103c);

    //compare output encrypted data of token 2
    if ((r_1_data == c_1_data_t2)
        &&(r_2_data == c_2_data_t2)
        &&(r_3_data == c_3_data_t2)
        &&(r_4_data == c_4_data_t2)
        &&(r_5_data == c_5_data_t2)
        &&(r_6_data == c_6_data_t2)
        &&(r_7_data == c_7_data_t2)
        &&(r_8_data == c_8_data_t2)
        &&(r_9_data == c_9_data_t2)
        &&(r_10_data == c_10_data_t2)
        &&(r_11_data == c_11_data_t2)
        &&(r_12_data == c_12_data_t2)
        &&(r_13_data == c_13_data_t2)
        &&(r_14_data == c_14_data_t2)
        &&(r_15_data == c_15_data_t2)
        &&(r_16_data == c_16_data_t2)
        ) {
        //compare pass
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        //compare fail
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }


    //aes_cbc_nist: 23
    //Check toke id in output encrypted data attached of token 1
    r_output_token_data = *((volatile unsigned int *)0x00421040);
    r_output_token_data_f = (r_output_token_data & 0xFFFF);

    if (r_output_token_data_f == c_output_token_id_2) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }


    //aes_cbc_nist: 24
    //Check 16 bytes in offset 0x0008
    r_1_data = *((volatile unsigned int *)0x60000008);
    r_2_data = *((volatile unsigned int *)0x6000000c);
    r_3_data = *((volatile unsigned int *)0x60000010);
    r_4_data = *((volatile unsigned int *)0x60000014);

    if ( (r_1_data == c2_1_data)
         &&(r_2_data == c2_2_data)
         &&(r_3_data == c2_3_data)
         &&(r_4_data == c2_4_data)
         ){
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }


    //aes_cbc_nist: 25
    //Write a word, job2 done and checked, clear mailbox
    *(volatile unsigned int *)0x60003f00 = 0x2;


    //aes_cbc_nist: 26
    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }


    //aes_cbc_nist: 27
    //Check output token 3 's Token ID
    r_output_token_data = *((volatile unsigned int *)0x60000000);

    if (r_output_token_data == c_output_token_id_3) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }


    //aes_cbc_nist: 28
    //Check block 64 bytes output encrypted data 3 in SRAM
    r_1_data = *((volatile unsigned int *)0x00431000);
    r_2_data = *((volatile unsigned int *)0x00431004);
    r_3_data = *((volatile unsigned int *)0x00431008);
    r_4_data = *((volatile unsigned int *)0x0043100c);
    r_5_data = *((volatile unsigned int *)0x00431010);
    r_6_data = *((volatile unsigned int *)0x00431014);
    r_7_data = *((volatile unsigned int *)0x00431018);
    r_8_data = *((volatile unsigned int *)0x0043101c);
    r_9_data = *((volatile unsigned int *)0x00431020);
    r_10_data = *((volatile unsigned int *)0x00431024);
    r_11_data = *((volatile unsigned int *)0x00431028);
    r_12_data = *((volatile unsigned int *)0x0043102c);
    r_13_data = *((volatile unsigned int *)0x00431030);
    r_14_data = *((volatile unsigned int *)0x00431034);
    r_15_data = *((volatile unsigned int *)0x00431038);
    r_16_data = *((volatile unsigned int *)0x0043103c);

    //compare output encrypted data of token 2
    if ((r_1_data == c_1_data_t3)
        &&(r_2_data == c_2_data_t3)
        &&(r_3_data == c_3_data_t3)
        &&(r_4_data == c_4_data_t3)
        &&(r_5_data == c_5_data_t3)
        &&(r_6_data == c_6_data_t3)
        &&(r_7_data == c_7_data_t3)
        &&(r_8_data == c_8_data_t3)
        &&(r_9_data == c_9_data_t3)
        &&(r_10_data == c_10_data_t3)
        &&(r_11_data == c_11_data_t3)
        &&(r_12_data == c_12_data_t3)
        &&(r_13_data == c_13_data_t3)
        &&(r_14_data == c_14_data_t3)
        &&(r_15_data == c_15_data_t3)
        &&(r_16_data == c_16_data_t3)
        ) {
        //compare pass
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        //compare fail
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }


    //aes_cbc_nist: 29
    //Check toke id in output encrypted data attached of token 1
    r_output_token_data = *((volatile unsigned int *)0x00431040);
    r_output_token_data_f = (r_output_token_data & 0xFFFF);

    if (r_output_token_data_f == c_output_token_id_3) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }


    //aes_cbc_nist: 30
    //Check 16 bytes in offset 0x0008
    r_1_data = *((volatile unsigned int *)0x60000008);
    r_2_data = *((volatile unsigned int *)0x6000000c);
    r_3_data = *((volatile unsigned int *)0x60000010);
    r_4_data = *((volatile unsigned int *)0x60000014);

    if ( (r_1_data == c3_1_data)
         &&(r_2_data == c3_2_data)
         &&(r_3_data == c3_3_data)
         &&(r_4_data == c3_4_data)
         ){
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }


    //aes_cbc_nist: 31
    //Write a word, job2 done and checked, clear mailbox
    *(volatile unsigned int *)0x60003f00 = 0x2;


}


void test_onsram_scrt_aes_2KB_1mailbox_3tokens(void) //it pass in Firdous's simulation test
                                                     //AES 2K Bytes Encryption on mailbox 1 with 3 tokens
                                                     //output address is the same as input address
{
    unsigned int data;

    unsigned int result_addr                = 0x0040a800;
    unsigned int flag_addr                  = 0x0040a900;

    unsigned int r_output_token_data;
    unsigned int r_output_token_data_f;

    unsigned int w_output_token_pass_data = 0xf0f0f0f0;
    unsigned int w_output_token_fail_data = 0x48484848;

    unsigned int c_output_token_id_1 = 0xd82c;
    unsigned int c_output_token_id_2 = 0x55fe;
    unsigned int c_output_token_id_3 = 0x0a68;

    unsigned int r_1_data = 0;
    unsigned int r_2_data = 0;
    unsigned int r_3_data = 0;
    unsigned int r_4_data = 0;
    unsigned int r_5_data = 0;
    unsigned int r_6_data = 0;
    unsigned int r_7_data = 0;
    unsigned int r_8_data = 0;
    unsigned int r_9_data = 0;
    unsigned int r_10_data = 0;
    unsigned int r_11_data = 0;
    unsigned int r_12_data = 0;
    unsigned int r_13_data = 0;
    unsigned int r_14_data = 0;
    unsigned int r_15_data = 0;
    unsigned int r_16_data = 0;

    unsigned int w_data_word = 0x12345678;
    unsigned int i = 0;

    unsigned int o_data_word = 0;


    //aes_cbc_nist: 1
    //Mailbox 1
    *(volatile unsigned int *)0x60003f00 = 0x4;


    /*
        ===================================
            Plain text 1
        ===================================
      */
    //aes_cbc_nist: 2
    //Burst_write 2K bytes in SRAM
    for (i=0; i<500; i++) {
        *((volatile unsigned int *)(0x0040a000 + (i*4))) = w_data_word;
    }


    //aes_cbc_nist: 3
    //Check mailbox is empty
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }


    //aes_cbc_nist: 4
    //Burst write 100 bytes (token1) in mailbox @ 0x6000_0000
    *(volatile unsigned int *)0x60000000 = 0x0104d82c;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = 0x000007d0;
    *(volatile unsigned int *)0x6000000c = 0x0040a000;
    *(volatile unsigned int *)0x60000010 = 0x00000000;
    *(volatile unsigned int *)0x60000014 = 0x000007d0;
    *(volatile unsigned int *)0x60000018 = 0x0040a000;
    *(volatile unsigned int *)0x6000001c = 0x00000000;
    *(volatile unsigned int *)0x60000020 = 0x000007d4;
    *(volatile unsigned int *)0x60000024 = 0x00000000;
    *(volatile unsigned int *)0x60000028 = 0x00000000;
    *(volatile unsigned int *)0x6000002c = 0x00018010;
    *(volatile unsigned int *)0x60000030 = 0x00000000;
    *(volatile unsigned int *)0x60000034 = 0x03020100;
    *(volatile unsigned int *)0x60000038 = 0x07060504;
    *(volatile unsigned int *)0x6000003c = 0x0b0a0908;
    *(volatile unsigned int *)0x60000040 = 0x0f0e0d0c;
    *(volatile unsigned int *)0x60000044 = 0x16157e2b;
    *(volatile unsigned int *)0x60000048 = 0xa6d2ae28;
    *(volatile unsigned int *)0x6000004c = 0x8815f7ab;
    *(volatile unsigned int *)0x60000050 = 0x3c4fcf09;
    *(volatile unsigned int *)0x60000054 = 0x00000000;
    *(volatile unsigned int *)0x60000058 = 0x00000000;
    *(volatile unsigned int *)0x6000005c = 0x00000000;
    *(volatile unsigned int *)0x60000060 = 0x00000000;


    //aes_cbc_nist: 5
    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;


    /*
        ===================================
            Plain text 2
        ===================================
      */
    //aes_cbc_nist: 6
    //Burst_write 2K bytes in SRAM
     for (i=0; i<500; i++) {
         *((volatile unsigned int *)(0x0040b000 + (i*4))) = w_data_word;
     }


    //aes_cbc_nist: 7
    //Polling to check mailbox is empty
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }


    //aes_cbc_nist: 8
    //Burst write 100 bytes (token2) in mailbox @ 0x6000_0000
    *(volatile unsigned int *)0x60000000 = 0x010455fe;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = 0x000007d0;
    *(volatile unsigned int *)0x6000000c = 0x0040b000;
    *(volatile unsigned int *)0x60000010 = 0x00000000;
    *(volatile unsigned int *)0x60000014 = 0x000007d0;
    *(volatile unsigned int *)0x60000018 = 0x0040b000;
    *(volatile unsigned int *)0x6000001c = 0x00000000;
    *(volatile unsigned int *)0x60000020 = 0x000007d4;
    *(volatile unsigned int *)0x60000024 = 0x00000000;
    *(volatile unsigned int *)0x60000028 = 0x00000000;
    *(volatile unsigned int *)0x6000002c = 0x00018010;
    *(volatile unsigned int *)0x60000030 = 0x00000000;
    *(volatile unsigned int *)0x60000034 = 0x03020100;
    *(volatile unsigned int *)0x60000038 = 0x07060504;
    *(volatile unsigned int *)0x6000003c = 0x0b0a0908;
    *(volatile unsigned int *)0x60000040 = 0x0f0e0d0c;
    *(volatile unsigned int *)0x60000044 = 0x16157e2b;
    *(volatile unsigned int *)0x60000048 = 0xa6d2ae28;
    *(volatile unsigned int *)0x6000004c = 0x8815f7ab;
    *(volatile unsigned int *)0x60000050 = 0x3c4fcf09;
    *(volatile unsigned int *)0x60000054 = 0x00000000;
    *(volatile unsigned int *)0x60000058 = 0x00000000;
    *(volatile unsigned int *)0x6000005c = 0x00000000;
    *(volatile unsigned int *)0x60000060 = 0x00000000;


    //aes_cbc_nist: 9
    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;


    //aes_cbc_nist: 10
    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }


    //aes_cbc_nist: 11
    //Check output token 1 's Token ID
    r_output_token_data = *((volatile unsigned int *)0x60000000);

    if (r_output_token_data == c_output_token_id_1) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }


    //aes_cbc_nist: 12
    //Read 2K bytes output encrypted data 1 in SRAM (from 0x0040_a000)
    for (i=0; i<500; i++) {
        o_data_word = *((volatile unsigned int *)(0x0040a000 + (i*4)));
    }


    //aes_cbc_nist: 13
    //Check toke id in output encrypted data attached of token 1
    r_output_token_data = *((volatile unsigned int *)0x0040a7d0);
    r_output_token_data_f = (r_output_token_data & 0xFFFF);

    if (r_output_token_data_f == c_output_token_id_1) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }


    //aes_cbc_nist: 14
    //Check 16 bytes in offset 0x0008
    r_1_data = *((volatile unsigned int *)0x60000008);
    r_2_data = *((volatile unsigned int *)0x6000000c);
    r_3_data = *((volatile unsigned int *)0x60000010);
    r_4_data = *((volatile unsigned int *)0x60000014);


    //aes_cbc_nist: 15
    //Write a word, job1 done and checked, clear mailbox
    *(volatile unsigned int *)0x60003f00 = 0x2;


    /*
        ===================================
            Plain text 3
        ===================================
      */
    //aes_cbc_nist: 16
    //Burst_write 2K bytes in SRAM
    for (i=0; i<500; i++) {
        *((volatile unsigned int *)(0x0040c000 + (i*4))) = w_data_word;
    }


    //aes_cbc_nist: 17
    //Polling to check mailbox is empty
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }


    //aes_cbc_nist: 18
    //Burst write 100 bytes (token3) in mailbox @ 0x6000_0000
    *(volatile unsigned int *)0x60000000 = 0x01040A68;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = 0x000007d0;
    *(volatile unsigned int *)0x6000000c = 0x0040c000;
    *(volatile unsigned int *)0x60000010 = 0x00000000;
    *(volatile unsigned int *)0x60000014 = 0x000007d0;
    *(volatile unsigned int *)0x60000018 = 0x0040c000;
    *(volatile unsigned int *)0x6000001c = 0x00000000;
    *(volatile unsigned int *)0x60000020 = 0x000007d4;
    *(volatile unsigned int *)0x60000024 = 0x00000000;
    *(volatile unsigned int *)0x60000028 = 0x00000000;
    *(volatile unsigned int *)0x6000002c = 0x00018010;
    *(volatile unsigned int *)0x60000030 = 0x00000000;
    *(volatile unsigned int *)0x60000034 = 0x03020100;
    *(volatile unsigned int *)0x60000038 = 0x07060504;
    *(volatile unsigned int *)0x6000003c = 0x0b0a0908;
    *(volatile unsigned int *)0x60000040 = 0x0f0e0d0c;
    *(volatile unsigned int *)0x60000044 = 0x16157e2b;
    *(volatile unsigned int *)0x60000048 = 0xa6d2ae28;
    *(volatile unsigned int *)0x6000004c = 0x8815f7ab;
    *(volatile unsigned int *)0x60000050 = 0x3c4fcf09;
    *(volatile unsigned int *)0x60000054 = 0x00000000;
    *(volatile unsigned int *)0x60000058 = 0x00000000;
    *(volatile unsigned int *)0x6000005c = 0x00000000;
    *(volatile unsigned int *)0x60000060 = 0x00000000;


    //aes_cbc_nist: 19
    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;


    //aes_cbc_nist: 20
    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }


    //aes_cbc_nist: 21
    //Check output token 2 's Token ID
    r_output_token_data = *((volatile unsigned int *)0x60000000);

    if (r_output_token_data == c_output_token_id_2) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }


    //aes_cbc_nist: 22
    //Read 2K bytes output encrypted data 2 in SRAM (from 0x0040_b000)
    for (i=0; i<500; i++) {
        o_data_word = *((volatile unsigned int *)(0x0040b000 + (i*4)));
    }


    //aes_cbc_nist: 23
    //Check toke id in output encrypted data attached of token 2
    r_output_token_data = *((volatile unsigned int *)0x0040b7d0);
    r_output_token_data_f = (r_output_token_data & 0xFFFF);

    if (r_output_token_data_f == c_output_token_id_2) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }


    //aes_cbc_nist: 24
    //Check 16 bytes in offset 0x0008
    r_1_data = *((volatile unsigned int *)0x60000008);
    r_2_data = *((volatile unsigned int *)0x6000000c);
    r_3_data = *((volatile unsigned int *)0x60000010);
    r_4_data = *((volatile unsigned int *)0x60000014);


    //aes_cbc_nist: 25
    //Write a word, job2 done and checked, clear mailbox
    *(volatile unsigned int *)0x60003f00 = 0x2;


    //aes_cbc_nist: 26
    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }


    //aes_cbc_nist: 27
    //Check output token 3 's Token ID
    r_output_token_data = *((volatile unsigned int *)0x60000000);

    if (r_output_token_data == c_output_token_id_3) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }


    //aes_cbc_nist: 28
    //Read 2K bytes output encrypted data 3 in SRAM (from 0x0040_c000)
    for (i=0; i<500; i++) {
        o_data_word = *((volatile unsigned int *)(0x0040c000 + (i*4)));
    }


    //aes_cbc_nist: 29
    //Check toke id in output encrypted data attached of token 3
    r_output_token_data = *((volatile unsigned int *)0x0040c7d0);
    r_output_token_data_f = (r_output_token_data & 0xFFFF);

    if (r_output_token_data_f == c_output_token_id_3) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }


    //aes_cbc_nist: 30
    //Check 16 bytes in offset 0x0008
    r_1_data = *((volatile unsigned int *)0x60000008);
    r_2_data = *((volatile unsigned int *)0x6000000c);
    r_3_data = *((volatile unsigned int *)0x60000010);
    r_4_data = *((volatile unsigned int *)0x60000014);


    //aes_cbc_nist: 31
    //Write a word, job3 done and checked, clear mailbox
    *(volatile unsigned int *)0x60003f00 = 0x2;

}


void test_onsram_scrt_do_aes_2k_bytes_3times(void) //it pass in Firdous's simulation test
{
    int i;
    for(i=0; i<3; i++){
        test_onsram_scrt_aes_2KB_1mailbox_3tokens();
    }
}


void test_onsram_scrt_do_nop_in_4_mailboxs(void) //it pass in Firdous's simulation test
                                                             //Nop means "DMA copy by Security IP"
{
    unsigned int w_output_token_pass_data = 0xf0f0f0f0;
    unsigned int w_output_token_fail_data = 0x48484848;
    unsigned int result_addr = 0x0040c000;

    unsigned int c_output_token_1 = 0xcd9d;
    unsigned int c_output_token_2 = 0xd82c;
    unsigned int c_output_token_3 = 0x55fe;
    unsigned int c_output_token_4 = 0x0a68;

    unsigned int w_data_word_1 = 0x11223344;
    unsigned int w_data_word_2 = 0x55667788;
    unsigned int w_data_word_3 = 0x99aabbcc;
    unsigned int w_data_word_4 = 0xddeeff00;


    unsigned int o_data_word = 0;
    unsigned int r_1_data = 0;
    unsigned int r_2_data = 0;
    unsigned int r_3_data = 0;
    unsigned int r_4_data = 0;
    unsigned int c2_1_data = 0xa1caf13f;
    unsigned int c2_2_data = 0x09ac1f68;
    unsigned int c2_3_data = 0x30ca0e12;
    unsigned int c2_4_data = 0xa7e18675;
    unsigned int r_output_token_data;
    unsigned int data = 0;
    unsigned int i = 0;


    //Link 4 mailbox
    *(volatile unsigned int *)0x60003f00 = 0x4;

    *(volatile unsigned int *)0x60003f00 = 0x40;

    *(volatile unsigned int *)0x60003f00 = 0x400;

    *(volatile unsigned int *)0x60003f00 = 0x4000;



    //Burst_write 4 bytes in SRAM
    *(volatile unsigned int *)0x0040a100 = w_data_word_1;

    //Burst_write 4 bytes in SRAM
    *(volatile unsigned int *)0x0040a200 = w_data_word_2;

    //Burst_write 4 bytes in SRAM
    *(volatile unsigned int *)0x0040a300 = w_data_word_3;

    //Burst_write 4 bytes in SRAM
    *(volatile unsigned int *)0x0040a400 = w_data_word_4;



    //===============================
    // Mailbox 1
    //===============================

    //Polling -1
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Write Control Token
    *(volatile unsigned int *)0x60000000 = 0x0004cd9d;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = 0x00000004;
    *(volatile unsigned int *)0x6000000c = 0x0040a100;
    *(volatile unsigned int *)0x60000010 = 0x00000000;
    *(volatile unsigned int *)0x60000014 = 0x00000004;
    *(volatile unsigned int *)0x60000018 = 0x00410100;
    *(volatile unsigned int *)0x6000001c = 0x00000000;
    *(volatile unsigned int *)0x60000020 = 0x00000008;

    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;

    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }


    //Check output token status
    r_output_token_data = *((volatile unsigned int *)0x60000000);
    if (r_output_token_data == c_output_token_1) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Check a word
    r_1_data = *((volatile unsigned int *)0x00410100);
    if ( r_1_data == 0x11223344 )
    {
       *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
       *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Check a word
    r_2_data = *((volatile unsigned int *)0x00410104);
    if ( r_2_data == c_output_token_1 )
    {
       *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
       *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Write
    *(volatile unsigned int *)0x60003f00 = 0x2;



    //===============================
    // Mailbox 2
    //===============================

    //Polling -1
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x10 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Write Control Token
    *(volatile unsigned int *)0x60000400 = 0x0004d82c;
    *(volatile unsigned int *)0x60000404 = 0x00000000;
    *(volatile unsigned int *)0x60000408 = 0x00000004;
    *(volatile unsigned int *)0x6000040c = 0x0040a200;
    *(volatile unsigned int *)0x60000410 = 0x00000000;
    *(volatile unsigned int *)0x60000414 = 0x00000004;
    *(volatile unsigned int *)0x60000418 = 0x00410200;
    *(volatile unsigned int *)0x6000041c = 0x00000000;
    *(volatile unsigned int *)0x60000420 = 0x00000008;

    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x10;

    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x20 ) == 0x20) {
            //success
            break;
        } else {
            continue;
        }
    }


    //Check output token status
    r_output_token_data = *((volatile unsigned int *)0x60000400);
    if (r_output_token_data == c_output_token_2) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Check a word
    r_1_data = *((volatile unsigned int *)0x00410200);
    if ( r_1_data == 0x55667788 )
    {
       *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
       *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Check a word
    r_2_data = *((volatile unsigned int *)0x00410204);
    if ( r_2_data == c_output_token_2 )
    {
       *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
       *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Write
    *(volatile unsigned int *)0x60003f00 = 0x20;



    //===============================
    // Mailbox 3
    //===============================

    //Polling -1
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x100 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Write Control Token
    *(volatile unsigned int *)0x60000800 = 0x000455fe;
    *(volatile unsigned int *)0x60000804 = 0x00000000;
    *(volatile unsigned int *)0x60000808 = 0x00000004;
    *(volatile unsigned int *)0x6000080c = 0x0040a300;
    *(volatile unsigned int *)0x60000810 = 0x00000000;
    *(volatile unsigned int *)0x60000814 = 0x00000004;
    *(volatile unsigned int *)0x60000818 = 0x00410300;
    *(volatile unsigned int *)0x6000081c = 0x00000000;
    *(volatile unsigned int *)0x60000820 = 0x00000008;

    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x100;

    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x200 ) == 0x200) {
            //success
            break;
        } else {
            continue;
        }
    }


    //Check output token status
    r_output_token_data = *((volatile unsigned int *)0x60000800);
    if (r_output_token_data == c_output_token_3) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Check a word
    r_1_data = *((volatile unsigned int *)0x00410300);
    if ( r_1_data == 0x99aabbcc )
    {
       *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
       *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Check a word
    r_2_data = *((volatile unsigned int *)0x00410304);
    if ( r_2_data == c_output_token_3 )
    {
       *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
       *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Write
    *(volatile unsigned int *)0x60003f00 = 0x200;



    //===============================
    // Mailbox 4
    //===============================

    //Polling -1
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1000 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Write Control Token
    *(volatile unsigned int *)0x60000c00 = 0x00040a68;
    *(volatile unsigned int *)0x60000c04 = 0x00000000;
    *(volatile unsigned int *)0x60000c08 = 0x00000004;
    *(volatile unsigned int *)0x60000c0c = 0x0040a400;
    *(volatile unsigned int *)0x60000c10 = 0x00000000;
    *(volatile unsigned int *)0x60000c14 = 0x00000004;
    *(volatile unsigned int *)0x60000c18 = 0x00410400;
    *(volatile unsigned int *)0x60000c1c = 0x00000000;
    *(volatile unsigned int *)0x60000c20 = 0x00000008;

    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1000;

    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2000 ) == 0x2000) {
            //success
            break;
        } else {
            continue;
        }
    }


    //Check output token status
    r_output_token_data = *((volatile unsigned int *)0x60000c00);
    if (r_output_token_data == c_output_token_4) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Check a word
    r_1_data = *((volatile unsigned int *)0x00410400);
    if ( r_1_data == 0xddeeff00 )
    {
       *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
       *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Check a word
    r_2_data = *((volatile unsigned int *)0x00410404);
    if ( r_2_data == c_output_token_4 )
    {
       *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
       *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Write
    *(volatile unsigned int *)0x60003f00 = 0x2000;


    //Status Register Read
    r_1_data = *((volatile unsigned int *)0x60003f00);
    if ( r_1_data == 0x4444 )
    {
       *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
       *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }


}


void test_onsram_scrt_aes_cmac(void) //it pass in Firdous's simulation test
                                              //pass on FPGA
{
    unsigned int data                   = 0;
    unsigned int c_output_token_id      = 0x9d37;
    unsigned int r_output_token_data    = 0;
    unsigned int result_addr            = 0x0040a800;

    unsigned int r_1_data = 0;
    unsigned int r_2_data = 0;
    unsigned int r_3_data = 0;
    unsigned int r_4_data = 0;

    unsigned int c_1_data = 0x902199e1;
    unsigned int c_2_data = 0xd56e9f54;
    unsigned int c_3_data = 0x052c6a69;
    unsigned int c_4_data = 0x1054316c;

    unsigned int w_output_token_pass_data = 0xf0f0f0f0;
    unsigned int w_output_token_fail_data = 0x48484848;

    unsigned int w_1_data = 0x03009d37;
    unsigned int w_2_data = 0x00000000;
    unsigned int w_3_data = 0x00000040;
    unsigned int w_4_data = 0x00411700;
    unsigned int w_5_data = 0x00000000;
    unsigned int w_6_data = 0x00000040;
    unsigned int w_7_data = 0x00200008;
    unsigned int w_8_data = 0x00000000;
    unsigned int w_9_data = 0x00000000;
    unsigned int w_10_data = 0x00000000;
    unsigned int w_11_data = 0x00000000;
    unsigned int w_12_data = 0x00000000;
    unsigned int w_13_data = 0x00000000;
    unsigned int w_14_data = 0x00000000;
    unsigned int w_15_data = 0x00000000;
    unsigned int w_16_data = 0x00000000;
    unsigned int w_17_data = 0x00000000;
    unsigned int w_18_data = 0x00000000;
    unsigned int w_19_data = 0x00000000;
    unsigned int w_20_data = 0x00000000;
    unsigned int w_21_data = 0x00000000;
    unsigned int w_22_data = 0x00000000;
    unsigned int w_23_data = 0x00000000;
    unsigned int w_24_data = 0x00000000;
    unsigned int w_25_data = 0x00000000;
    unsigned int w_26_data = 0x00000000;
    unsigned int w_27_data = 0x00000000;
    unsigned int w_28_data = 0x00000000;
    unsigned int w_29_data = 0x10eb3d60;
    unsigned int w_30_data = 0xbe71ca15;
    unsigned int w_31_data = 0xf0ae732b;
    unsigned int w_32_data = 0x81777d85;
    unsigned int w_33_data = 0x072c351f;
    unsigned int w_34_data = 0xd708613b;
    unsigned int w_35_data = 0xa310982d;
    unsigned int w_36_data = 0xf4df1409;
    unsigned int w_37_data = 0x00000000;
    unsigned int w_38_data = 0x00000000;
    unsigned int w_39_data = 0x00000000;
    unsigned int w_40_data = 0x00000000;
    unsigned int w_41_data = 0x00000000;
    unsigned int w_42_data = 0x00000000;
    unsigned int w_43_data = 0x00000000;
    unsigned int w_44_data = 0x00000000;
    unsigned int w_45_data = 0x00000000;
    unsigned int w_46_data = 0x00000000;
    unsigned int w_47_data = 0x00000000;
    unsigned int w_48_data = 0x00000000;
    unsigned int w_49_data = 0x00000000;
    unsigned int w_50_data = 0x00000000;
    unsigned int w_51_data = 0x00000000;
    unsigned int w_52_data = 0x00000000;
    unsigned int w_53_data = 0x00000000;
    unsigned int w_54_data = 0x00000000;
    unsigned int w_55_data = 0x00000000;
    unsigned int w_56_data = 0x00000000;
    unsigned int w_57_data = 0x00000000;
    unsigned int w_58_data = 0x00000000;
    unsigned int w_59_data = 0x00000000;
    unsigned int w_60_data = 0x00000000;


    //Mailbox 1
    *(volatile unsigned int *)0x60003f00 = 0x4;

    //Write Data to SRAM
    *(volatile unsigned int *)0x00411700 = 0xe2bec16b;
    *(volatile unsigned int *)0x00411704 = 0x969f402e;
    *(volatile unsigned int *)0x00411708 = 0x117e3de9;
    *(volatile unsigned int *)0x0041170c = 0x2a179373;
    *(volatile unsigned int *)0x00411710 = 0x578a2dae;
    *(volatile unsigned int *)0x00411714 = 0x9cac031e;
    *(volatile unsigned int *)0x00411718 = 0xac6fb79e;
    *(volatile unsigned int *)0x0041171c = 0x518eaf45;
    *(volatile unsigned int *)0x00411720 = 0x461cc830;
    *(volatile unsigned int *)0x00411724 = 0x11e45ca3;
    *(volatile unsigned int *)0x00411728 = 0x19c1fbe5;
    *(volatile unsigned int *)0x0041172c = 0xef520a1a;
    *(volatile unsigned int *)0x00411730 = 0x45249ff6;
    *(volatile unsigned int *)0x00411734 = 0x179b4fdf;
    *(volatile unsigned int *)0x00411738 = 0x7b412bad;
    *(volatile unsigned int *)0x0041173c = 0x10376ce6;

    //Polling -1
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Write Token to mailbox
    *(volatile unsigned int *)0x60000000 = w_1_data;
    *(volatile unsigned int *)0x60000004 = w_2_data;
    *(volatile unsigned int *)0x60000008 = w_3_data;
    *(volatile unsigned int *)0x6000000c = w_4_data;
    *(volatile unsigned int *)0x60000010 = w_5_data;
    *(volatile unsigned int *)0x60000014 = w_6_data;
    *(volatile unsigned int *)0x60000018 = w_7_data;
    *(volatile unsigned int *)0x6000001c = w_8_data;
    *(volatile unsigned int *)0x60000020 = w_9_data;
    *(volatile unsigned int *)0x60000024 = w_10_data;
    *(volatile unsigned int *)0x60000028 = w_11_data;
    *(volatile unsigned int *)0x6000002c = w_12_data;
    *(volatile unsigned int *)0x60000030 = w_13_data;
    *(volatile unsigned int *)0x60000034 = w_14_data;
    *(volatile unsigned int *)0x60000038 = w_15_data;
    *(volatile unsigned int *)0x6000003c = w_16_data;
    *(volatile unsigned int *)0x60000040 = w_17_data;
    *(volatile unsigned int *)0x60000044 = w_18_data;
    *(volatile unsigned int *)0x60000048 = w_19_data;
    *(volatile unsigned int *)0x6000004c = w_20_data;
    *(volatile unsigned int *)0x60000050 = w_21_data;
    *(volatile unsigned int *)0x60000054 = w_22_data;
    *(volatile unsigned int *)0x60000058 = w_23_data;
    *(volatile unsigned int *)0x6000005c = w_24_data;
    *(volatile unsigned int *)0x60000060 = w_25_data;
    *(volatile unsigned int *)0x60000064 = w_26_data;
    *(volatile unsigned int *)0x60000068 = w_27_data;
    *(volatile unsigned int *)0x6000006c = w_28_data;
    *(volatile unsigned int *)0x60000070 = w_29_data;
    *(volatile unsigned int *)0x60000074 = w_30_data;
    *(volatile unsigned int *)0x60000078 = w_31_data;
    *(volatile unsigned int *)0x6000007c = w_32_data;
    *(volatile unsigned int *)0x60000080 = w_33_data;
    *(volatile unsigned int *)0x60000084 = w_34_data;
    *(volatile unsigned int *)0x60000088 = w_35_data;
    *(volatile unsigned int *)0x6000008c = w_36_data;
    *(volatile unsigned int *)0x60000090 = w_37_data;
    *(volatile unsigned int *)0x60000094 = w_38_data;
    *(volatile unsigned int *)0x60000098 = w_39_data;
    *(volatile unsigned int *)0x6000009c = w_40_data;
    *(volatile unsigned int *)0x600000a0 = w_41_data;
    *(volatile unsigned int *)0x600000a4 = w_42_data;
    *(volatile unsigned int *)0x600000a8 = w_43_data;
    *(volatile unsigned int *)0x600000ac = w_44_data;
    *(volatile unsigned int *)0x600000b0 = w_45_data;
    *(volatile unsigned int *)0x600000b4 = w_46_data;
    *(volatile unsigned int *)0x600000b8 = w_47_data;
    *(volatile unsigned int *)0x600000bc = w_48_data;
    *(volatile unsigned int *)0x600000c0 = w_49_data;
    *(volatile unsigned int *)0x600000c4 = w_50_data;
    *(volatile unsigned int *)0x600000c8 = w_51_data;
    *(volatile unsigned int *)0x600000cc = w_52_data;
    *(volatile unsigned int *)0x600000d0 = w_53_data;
    *(volatile unsigned int *)0x600000d4 = w_54_data;
    *(volatile unsigned int *)0x600000d8 = w_55_data;
    *(volatile unsigned int *)0x600000dc = w_56_data;
    *(volatile unsigned int *)0x600000e0 = w_57_data;
    *(volatile unsigned int *)0x600000e4 = w_58_data;
    *(volatile unsigned int *)0x600000e8 = w_59_data;
    *(volatile unsigned int *)0x600000ec = w_60_data;

    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;

    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Check output token status
    r_output_token_data = *((volatile unsigned int *)0x60000000);
    if (r_output_token_data == c_output_token_id) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Check 16 bytes in offset 0x0008
    r_1_data = *((volatile unsigned int *)0x60000008);
    r_2_data = *((volatile unsigned int *)0x6000000c);
    r_3_data = *((volatile unsigned int *)0x60000010);
    r_4_data = *((volatile unsigned int *)0x60000014);

    if ((r_1_data == c_1_data)
        &&(r_2_data == c_2_data)
        &&(r_3_data == c_3_data)
        &&(r_4_data == c_4_data)
        ){
       *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
       *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Write
    *(volatile unsigned int *)0x60003f00 = 0x2;

}


void test_onsram_scrt_hash(void) //it pass in Firdous's simulation test
                                        //pass on FPGA
{
    unsigned int data                   = 0;
    unsigned int c_output_token_id_1      = 0x6c12;
    unsigned int c_output_token_id_2      = 0x900c;
    unsigned int r_output_token_data    = 0;
    unsigned int result_addr            = 0x0040a800;

    unsigned int r_1_data = 0;
    unsigned int r_2_data = 0;
    unsigned int r_3_data = 0;
    unsigned int r_4_data = 0;
    unsigned int r_5_data = 0;
    unsigned int r_6_data = 0;
    unsigned int r_7_data = 0;
    unsigned int r_8_data = 0;

    unsigned int c_1_data = 0x942fc50c;
    unsigned int c_2_data = 0xb8c94ece;
    unsigned int c_3_data = 0x8ca4dbc3;
    unsigned int c_4_data = 0x91456373;
    unsigned int c_5_data = 0xa862fdc8;

    unsigned int c2_1_data = 0xdd5f05b5;
    unsigned int c2_2_data = 0xa4202940;
    unsigned int c2_3_data = 0x0c3d0841;
    unsigned int c2_4_data = 0x1ec2ad45;
    unsigned int c2_5_data = 0xc7e481f6;
    unsigned int c2_6_data = 0x6fbd5835;
    unsigned int c2_7_data = 0x79b9fdb1;
    unsigned int c2_8_data = 0xc685e183;

    unsigned int w_output_token_pass_data = 0xf0f0f0f0;
    unsigned int w_output_token_fail_data = 0x48484848;

    unsigned int w_1_data = 0x83945d41;
    unsigned int w_2_data = 0x63852dae;
    unsigned int w_3_data = 0x5d57ba9a;
    unsigned int w_4_data = 0xbea0d38f;
    unsigned int w_5_data = 0x26a195b8;
    unsigned int w_6_data = 0x0010ba9f;
    unsigned int w_7_data = 0x9006d8b6;
    unsigned int w_8_data = 0x6562e3b5;
    unsigned int w_9_data = 0x7605a994;
    unsigned int w_10_data = 0x64c7034d;
    unsigned int w_11_data = 0x1e05b593;
    unsigned int w_12_data = 0xfd0064e0;
    unsigned int w_13_data = 0x8e1454d0;
    unsigned int w_14_data = 0x068f1445;
    unsigned int w_15_data = 0x703b8fc7;
    unsigned int w_16_data = 0x60430c0d;
    unsigned int w_17_data = 0x4ec9a367;
    unsigned int w_18_data = 0xaa7b7638;
    unsigned int w_19_data = 0x0fbf1866;
    unsigned int w_20_data = 0x3ad60eba;
    unsigned int w_21_data = 0x4024c819;
    unsigned int w_22_data = 0x035cdde3;
    unsigned int w_23_data = 0xab776f3c;
    unsigned int w_24_data = 0x991f6271;
    unsigned int w_25_data = 0xc06853c9;
    unsigned int w_26_data = 0x582bc2d2;
    unsigned int w_27_data = 0xaddbfbfa;
    unsigned int w_28_data = 0xcea96818;
    unsigned int w_29_data = 0x3d07c8a2;
    unsigned int w_30_data = 0xd89318cf;
    unsigned int w_31_data = 0xe7f69976;
    unsigned int w_32_data = 0x49a82102;


    //Mailbox 1
    *(volatile unsigned int *)0x60003f00 = 0x4;

    //Mailbox 2
    *(volatile unsigned int *)0x60003f00 = 0x40;

    //Mailbox 3
    *(volatile unsigned int *)0x60003f00 = 0x400;

    //Mailbox 4
    *(volatile unsigned int *)0x60003f00 = 0x4000;

    //Mailbox 1 again
    *(volatile unsigned int *)0x60003f00 = 0x4;

    //Write Data to SRAM
    *(volatile unsigned int *)0x0040a000 = w_1_data;
    *(volatile unsigned int *)0x0040a004 = w_2_data;
    *(volatile unsigned int *)0x0040a008 = w_3_data;
    *(volatile unsigned int *)0x0040a00c = w_4_data;
    *(volatile unsigned int *)0x0040a010 = w_5_data;
    *(volatile unsigned int *)0x0040a014 = w_6_data;
    *(volatile unsigned int *)0x0040a018 = w_7_data;
    *(volatile unsigned int *)0x0040a01c = w_8_data;
    *(volatile unsigned int *)0x0040a020 = w_9_data;
    *(volatile unsigned int *)0x0040a024 = w_10_data;
    *(volatile unsigned int *)0x0040a028 = w_11_data;
    *(volatile unsigned int *)0x0040a02c = w_12_data;
    *(volatile unsigned int *)0x0040a030 = w_13_data;
    *(volatile unsigned int *)0x0040a034 = w_14_data;
    *(volatile unsigned int *)0x0040a038 = w_15_data;
    *(volatile unsigned int *)0x0040a03c = w_16_data;
    *(volatile unsigned int *)0x0040a040 = w_17_data;
    *(volatile unsigned int *)0x0040a044 = w_18_data;
    *(volatile unsigned int *)0x0040a048 = w_19_data;
    *(volatile unsigned int *)0x0040a04c = w_20_data;
    *(volatile unsigned int *)0x0040a050 = w_21_data;
    *(volatile unsigned int *)0x0040a054 = w_22_data;
    *(volatile unsigned int *)0x0040a058 = w_23_data;
    *(volatile unsigned int *)0x0040a05c = w_24_data;
    *(volatile unsigned int *)0x0040a060 = w_25_data;
    *(volatile unsigned int *)0x0040a064 = w_26_data;
    *(volatile unsigned int *)0x0040a068 = w_27_data;
    *(volatile unsigned int *)0x0040a06c = w_28_data;
    *(volatile unsigned int *)0x0040a070 = w_29_data;
    *(volatile unsigned int *)0x0040a074 = w_30_data;
    *(volatile unsigned int *)0x0040a078 = w_31_data;
    *(volatile unsigned int *)0x0040a07c = w_32_data;

    //Polling -1
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Write Token
    *(volatile unsigned int *)0x60000000 = 0x02006c12;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = 0x00000080;
    *(volatile unsigned int *)0x6000000c = 0x0040a000;
    *(volatile unsigned int *)0x60000010 = 0x00000000;
    *(volatile unsigned int *)0x60000014 = 0x00000080;
    *(volatile unsigned int *)0x60000018 = 0x00000001;
    *(volatile unsigned int *)0x6000001c = 0x00000000;
    *(volatile unsigned int *)0x60000020 = 0x00000000;
    *(volatile unsigned int *)0x60000024 = 0x00000000;
    *(volatile unsigned int *)0x60000028 = 0x00000000;
    *(volatile unsigned int *)0x6000002c = 0x00000000;
    *(volatile unsigned int *)0x60000030 = 0x00000000;
    *(volatile unsigned int *)0x60000034 = 0x00000000;
    *(volatile unsigned int *)0x60000038 = 0x00000000;
    *(volatile unsigned int *)0x6000003c = 0x00000000;
    *(volatile unsigned int *)0x60000040 = 0x00000000;
    *(volatile unsigned int *)0x60000044 = 0x00000000;
    *(volatile unsigned int *)0x60000048 = 0x00000000;
    *(volatile unsigned int *)0x6000004c = 0x00000000;
    *(volatile unsigned int *)0x60000050 = 0x00000000;
    *(volatile unsigned int *)0x60000054 = 0x00000000;
    *(volatile unsigned int *)0x60000058 = 0x00000000;
    *(volatile unsigned int *)0x6000005c = 0x00000000;
    *(volatile unsigned int *)0x60000060 = 0x00000080;
    *(volatile unsigned int *)0x60000064 = 0x00000000;
    *(volatile unsigned int *)0x60000068 = 0x00000000;
    *(volatile unsigned int *)0x6000006c = 0x00000000;

    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;

    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Check output token 1 status
    r_output_token_data = *((volatile unsigned int *)0x60000000);
    if (r_output_token_data == c_output_token_id_1) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }


    //Check 20 bytes in offset 0x0008
    r_1_data = *((volatile unsigned int *)0x60000008);
    r_2_data = *((volatile unsigned int *)0x6000000c);
    r_3_data = *((volatile unsigned int *)0x60000010);
    r_4_data = *((volatile unsigned int *)0x60000014);
    r_5_data = *((volatile unsigned int *)0x60000018);

    if ((r_1_data == c_1_data)
        &&(r_2_data == c_2_data)
        &&(r_3_data == c_3_data)
        &&(r_4_data == c_4_data)
        &&(r_5_data == c_5_data))
    {
       *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
       *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x2;

    //Write 132 bytes in SRAM
    *(volatile unsigned int *)0x0040b000 = 0x5e7f6915;
    *(volatile unsigned int *)0x0040b004 = 0x291a47b8;
    *(volatile unsigned int *)0x0040b008 = 0x02b1944e;
    *(volatile unsigned int *)0x0040b00c = 0x6c87ce8d;
    *(volatile unsigned int *)0x0040b010 = 0x9ef0edf4;
    *(volatile unsigned int *)0x0040b014 = 0xbded711e;
    *(volatile unsigned int *)0x0040b018 = 0xa34ddc8c;
    *(volatile unsigned int *)0x0040b01c = 0x679bb9b4;
    *(volatile unsigned int *)0x0040b020 = 0xa0425368;
    *(volatile unsigned int *)0x0040b024 = 0x57925bad;
    *(volatile unsigned int *)0x0040b028 = 0xb2c0f833;
    *(volatile unsigned int *)0x0040b02c = 0x815fac01;
    *(volatile unsigned int *)0x0040b030 = 0xb4263fee;
    *(volatile unsigned int *)0x0040b034 = 0xe2661688;
    *(volatile unsigned int *)0x0040b038 = 0xdd7fe265;
    *(volatile unsigned int *)0x0040b03c = 0xe546c4b3;
    *(volatile unsigned int *)0x0040b040 = 0xd8d3e9fb;
    *(volatile unsigned int *)0x0040b044 = 0x92fabd1c;
    *(volatile unsigned int *)0x0040b048 = 0x99b75d2d;
    *(volatile unsigned int *)0x0040b04c = 0xc3128a2a;
    *(volatile unsigned int *)0x0040b050 = 0xff4ad309;
    *(volatile unsigned int *)0x0040b054 = 0x8a97fee9;
    *(volatile unsigned int *)0x0040b058 = 0xdccb17c2;
    *(volatile unsigned int *)0x0040b05c = 0xbb676381;
    *(volatile unsigned int *)0x0040b060 = 0x5cd43686;
    *(volatile unsigned int *)0x0040b064 = 0xb080a406;
    *(volatile unsigned int *)0x0040b068 = 0x09629f86;
    *(volatile unsigned int *)0x0040b06c = 0x76854a3c;
    *(volatile unsigned int *)0x0040b070 = 0x4a691d69;
    *(volatile unsigned int *)0x0040b074 = 0xce01c8d6;
    *(volatile unsigned int *)0x0040b078 = 0xa48967c8;
    *(volatile unsigned int *)0x0040b07c = 0xe0e7c910;
    *(volatile unsigned int *)0x0040b080 = 0x00000002;

    //Polling -1
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Write Token
    *(volatile unsigned int *)0x60000000 = 0x0200900c;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = 0x00000081;
    *(volatile unsigned int *)0x6000000c = 0x0040b000;
    *(volatile unsigned int *)0x60000010 = 0x00000000;
    *(volatile unsigned int *)0x60000014 = 0x00000084;
    *(volatile unsigned int *)0x60000018 = 0x00000003;
    *(volatile unsigned int *)0x6000001c = 0x00000000;
    *(volatile unsigned int *)0x60000020 = 0x00000000;
    *(volatile unsigned int *)0x60000024 = 0x00000000;
    *(volatile unsigned int *)0x60000028 = 0x00000000;
    *(volatile unsigned int *)0x6000002c = 0x00000000;
    *(volatile unsigned int *)0x60000030 = 0x00000000;
    *(volatile unsigned int *)0x60000034 = 0x00000000;
    *(volatile unsigned int *)0x60000038 = 0x00000000;
    *(volatile unsigned int *)0x6000003c = 0x00000000;
    *(volatile unsigned int *)0x60000040 = 0x00000000;
    *(volatile unsigned int *)0x60000044 = 0x00000000;
    *(volatile unsigned int *)0x60000048 = 0x00000000;
    *(volatile unsigned int *)0x6000004c = 0x00000000;
    *(volatile unsigned int *)0x60000050 = 0x00000000;
    *(volatile unsigned int *)0x60000054 = 0x00000000;
    *(volatile unsigned int *)0x60000058 = 0x00000000;
    *(volatile unsigned int *)0x6000005c = 0x00000000;
    *(volatile unsigned int *)0x60000060 = 0x00000081;
    *(volatile unsigned int *)0x60000064 = 0x00000000;
    *(volatile unsigned int *)0x60000068 = 0x00000000;
    *(volatile unsigned int *)0x6000006c = 0x00000000;

    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;

    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Check output token 2 status
    r_output_token_data = *((volatile unsigned int *)0x60000000);
    if (r_output_token_data == c_output_token_id_2) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Check 32 bytes in offset 0x0008
    r_1_data = *((volatile unsigned int *)0x60000008);
    r_2_data = *((volatile unsigned int *)0x6000000c);
    r_3_data = *((volatile unsigned int *)0x60000010);
    r_4_data = *((volatile unsigned int *)0x60000014);
    r_5_data = *((volatile unsigned int *)0x60000018);
    r_6_data = *((volatile unsigned int *)0x6000001c);
    r_7_data = *((volatile unsigned int *)0x60000020);
    r_8_data = *((volatile unsigned int *)0x60000024);
    if (  (r_1_data == c2_1_data)
        &&(r_2_data == c2_2_data)
        &&(r_3_data == c2_3_data)
        &&(r_4_data == c2_4_data)
        &&(r_5_data == c2_5_data)
        &&(r_6_data == c2_6_data)
        &&(r_7_data == c2_7_data)
        &&(r_8_data == c2_8_data))
    {
       *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
       *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x2;

}


void test_onsram_scrt_trng(void) //FAIL
{
    unsigned int c_output_token_id_1    = 0x67aa;
    unsigned int c_output_token_id_2    = 0xc8a7;
    unsigned int c_output_token_id_3    = 0x4da5;
    unsigned int c_output_token_id_4    = 0x4826;
    unsigned int r_output_token_data    = 0;
    unsigned int data                   = 0;
    unsigned int result_addr            = 0x0040a800;
    unsigned int w_output_token_pass_data = 0xf0f0f0f0;
    unsigned int w_output_token_fail_data = 0x48484848;
    unsigned int r_1_data = 0;
    unsigned int r_2_data = 0;
    unsigned int r_3_data = 0;
    unsigned int r_4_data = 0;
    unsigned int r_5_data = 0;
    unsigned int r_6_data = 0;
    unsigned int r_7_data = 0;
    unsigned int r_8_data = 0;


    //Mailbox 1
    *(volatile unsigned int *)0x60003f00 = 0x4;


    //Polling -1
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }


    //Write token to d software reset
    *(volatile unsigned int *)0x60000000 = 0x2f0267aa;
    *(volatile unsigned int *)0x60000004 = 0x00000002;



    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;


    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }


    //Check output token 1 status
    r_output_token_data = *((volatile unsigned int *)0x60000000);
    if (r_output_token_data == c_output_token_id_1) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }


    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x2;


    //Mailbox Link Again
    *(volatile unsigned int *)0x60003f00 = 0x4;


    //Polling -1
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }


    //Write Token
    *(volatile unsigned int *)0x60000000 = 0x1402c8a7;
    *(volatile unsigned int *)0x60000004 = 0x4f5a3647;
    *(volatile unsigned int *)0x60000008 = 0x00000003;
    *(volatile unsigned int *)0x6000000c = 0x00030801;


    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;


    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }


    //Check output token 1 status
    r_output_token_data = *((volatile unsigned int *)0x60000000);
    if (r_output_token_data == c_output_token_id_2) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }


    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x2;


    //Write Data to SRAM
    *(volatile unsigned int *)0x0040a000 = 0xcccccccc;
    *(volatile unsigned int *)0x0040a004 = 0xcccccccc;
    *(volatile unsigned int *)0x0040a008 = 0xcccccccc;
    *(volatile unsigned int *)0x0040a00c = 0xcccccccc;


    //Polling -1
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }


    //Write Token
    *(volatile unsigned int *)0x60000000 = 0x04044da5;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = 0x00000010;
    *(volatile unsigned int *)0x6000000c = 0x0040a000;
    *(volatile unsigned int *)0x60000010 = 0x00000000;


    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;


    //Write Data to SRAM
    *(volatile unsigned int *)0x0040b000 = 0xcccccccc;
    *(volatile unsigned int *)0x0040b004 = 0xcccccccc;
    *(volatile unsigned int *)0x0040b008 = 0xcccccccc;
    *(volatile unsigned int *)0x0040b00c = 0xcccccccc;
    *(volatile unsigned int *)0x0040b010 = 0xcccccccc;
    *(volatile unsigned int *)0x0040b014 = 0xcccccccc;
    *(volatile unsigned int *)0x0040b018 = 0xcccccccc;
    *(volatile unsigned int *)0x0040b01c = 0xcccccccc;


    //Polling -1
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }


    //Write Token
    *(volatile unsigned int *)0x60000000 = 0x04044826;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = 0x00000020;
    *(volatile unsigned int *)0x6000000c = 0x0040b000;
    *(volatile unsigned int *)0x60000010 = 0x00000000;


    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;


    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }


    //Check output token 1 status
    r_output_token_data = *((volatile unsigned int *)0x60000000);
    if (r_output_token_data == c_output_token_id_3) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }


    //Read Block (4 words)
    r_1_data = *((volatile unsigned int *)0x0040a000);
    r_2_data = *((volatile unsigned int *)0x0040a004);
    r_3_data = *((volatile unsigned int *)0x0040a008);
    r_4_data = *((volatile unsigned int *)0x0040a00c);


    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x2;


    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }


    //Check output token 1 status
    r_output_token_data = *((volatile unsigned int *)0x60000000);
    if (r_output_token_data == c_output_token_id_4) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }


    //Read Block (8words)
    r_1_data = *((volatile unsigned int *)0x0040b000);
    r_2_data = *((volatile unsigned int *)0x0040b004);
    r_3_data = *((volatile unsigned int *)0x0040b008);
    r_4_data = *((volatile unsigned int *)0x0040b00c);
    r_5_data = *((volatile unsigned int *)0x0040b010);
    r_6_data = *((volatile unsigned int *)0x0040b014);
    r_7_data = *((volatile unsigned int *)0x0040b018);
    r_8_data = *((volatile unsigned int *)0x0040b01c);


    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x2;


}


void test_onsram_scrt_core_reg_test(void) //testing

{
    unsigned int data;

    unsigned int r_1_data = 0;


    //Mailbox 1
    *(volatile unsigned int *)0x60003f00 = 0x4;
    r_1_data = *((volatile unsigned int *)0x60003f00);

    //Mailbox 2
    *(volatile unsigned int *)0x60003f00 = 0x40;
    r_1_data = *((volatile unsigned int *)0x60003f00);

    //Mailbox 3
    *(volatile unsigned int *)0x60003f00 = 0x400;
    r_1_data = *((volatile unsigned int *)0x60003f00);

    //Mailbox 4
    *(volatile unsigned int *)0x60003f00 = 0x4000;
    r_1_data = *((volatile unsigned int *)0x60003f00);

    //Write
    *(volatile unsigned int *)0x60003f00 = 0x1;
    r_1_data = *((volatile unsigned int *)0x60003f00);

    //Write
    *(volatile unsigned int *)0x60003f00 = 0x2;
    r_1_data = *((volatile unsigned int *)0x60003f00);
}


void fpga_test_onrom_level1_scrt0_reg_access(void) //pass on FPGA
{
    unsigned int scrt0_base = 0x60000000;
    unsigned int scrt0_ctrl_addr = scrt0_base + EIP130_REGISTEROFFSET_MAILBOX_CTRL;
    unsigned int scrt0_stat_addr = scrt0_base + EIP130_REGISTEROFFSET_MAILBOX_STAT;
    unsigned int data;

    //write
    *(volatile unsigned int *)scrt0_ctrl_addr = 0x4444;

    //read
    data = *((volatile unsigned int *)scrt0_stat_addr);
}


void fpga_test_onrom_level1_scrt1_reg_access(void) //pass on FPGA
{
    unsigned int scrt1_base = 0x80000000;
    unsigned int scrt1_reg_scrt_ctrl_offset = 0x0;
    unsigned int scrt1_reg_scrt_ctrl_addr = scrt1_base + scrt1_reg_scrt_ctrl_offset;
    unsigned int data;

    //write
    *(volatile unsigned int *)scrt1_reg_scrt_ctrl_addr = 0x1;

    //read
    data = *((volatile unsigned int *)scrt1_reg_scrt_ctrl_addr);

}


void fpga_test_onrom_level1_sram_access(void) //pass on FPGA
{
    unsigned int sram_addr = 0x00410000;
    unsigned int data;

    //write
    *(volatile unsigned int *)sram_addr = 0x11223344;

    //There's something wrong in bus, if we didn't do delay ticks here, the read operation fail
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    //read
    data = *((volatile unsigned int *)sram_addr);
}


/*
In our FPGA environment (2017.03.10), We only have 1xx KB SRAM for using
SRAM address starts from 0x00400000
SRAM range: 0x004000000 ~ 0x00410000  => 128KB totally
=========================================================
  0x00410000 => input plain text data
  0x00411000 => output encrypted data
  0x00412000 => test result
  0x00413000 => flag for identify
*/
void fpga_test_onrom_level5_scrt_sram_burst(void) //pass on FPGA
{
    unsigned int sram_addr   = 0x00410000;
    unsigned int result_addr = 0x00412000;
    unsigned int flag_addr   = 0x00413000;

    unsigned int w_1_addr = (sram_addr + 0x00000000);
    unsigned int w_2_addr = (sram_addr + 0x00000004);
    unsigned int w_3_addr = (sram_addr + 0x00000008);
    unsigned int w_4_addr = (sram_addr + 0x0000000c);
    unsigned int w_5_addr = (sram_addr + 0x00000010);
    unsigned int w_6_addr = (sram_addr + 0x00000014);

    unsigned int w_1_data = 0x11223344;
    unsigned int w_2_data = 0x55667788;
    unsigned int w_3_data = 0x99aabbcc;
    unsigned int w_4_data = 0xddeef00f;
    unsigned int w_5_data = 0xf9ab1357;
    unsigned int w_6_data = 0x24689876;

    unsigned int r_1_data = 0;
    unsigned int r_2_data = 0;
    unsigned int r_3_data = 0;
    unsigned int r_4_data = 0;
    unsigned int r_5_data = 0;
    unsigned int r_6_data = 0;

    unsigned int val_pass = 0xf0f0f0f0;
    unsigned int val_fail = 0x48484848;
    unsigned int result_data = 0;

    //write
    *((volatile unsigned int *)w_1_addr) = w_1_data;
    *((volatile unsigned int *)w_2_addr) = w_2_data;
    *((volatile unsigned int *)w_3_addr) = w_3_data;
    *((volatile unsigned int *)w_4_addr) = w_4_data;
    *((volatile unsigned int *)w_5_addr) = w_5_data;
    *((volatile unsigned int *)w_6_addr) = w_6_data;

    //read
    r_1_data = *((volatile unsigned int *)w_1_addr);
    r_2_data = *((volatile unsigned int *)w_2_addr);
    r_3_data = *((volatile unsigned int *)w_3_addr);
    r_4_data = *((volatile unsigned int *)w_4_addr);
    r_5_data = *((volatile unsigned int *)w_5_addr);
    r_6_data = *((volatile unsigned int *)w_6_addr);

    //compare
    if ((r_1_data == w_1_data)
      &&(r_2_data == w_2_data)
      &&(r_3_data == w_3_data)
      &&(r_4_data == w_4_data)
      &&(r_5_data == w_5_data)
      &&(r_6_data == w_6_data))
    {
      *((volatile unsigned int *)result_addr) = val_pass;
    } else {
      *((volatile unsigned int *)result_addr) = val_fail;
    }

    result_data = *((volatile unsigned int *)result_addr);

    *(volatile unsigned int *)flag_addr = 0x10101010;

}


/*
In our FPGA environment (2017.03.10), We only have 1xx KB SRAM for using
SRAM address starts from 0x00400000
SRAM range: 0x004000000 ~ 0x00410000  => 128KB totally
=========================================================
  0x00410000 => input plain text data
  0x00411000 => output encrypted data
  0x00412000 => test result
  0x00413000 => flag for identify
*/
void fpga_test_onrom_level4_scrt0_aes(void) //pass on FPGA
{
    unsigned int data;
    unsigned int scrt0_write_addr_1 = 0x60003f00;//SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_CTRL;
    unsigned int scrt0_read_addr_1 = 0x60003f00;//SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_STAT;

    unsigned int sram_addr_base    = 0x00400000;
    unsigned int sram_addr_write_1 = 0x00410000;
    unsigned int sram_addr_output_encrypted_data = 0x00411000;

    unsigned int c_1_data = 0xacab4976;
    unsigned int c_2_data = 0x46b21981;
    unsigned int c_3_data = 0x9b8ee9ce;
    unsigned int c_4_data = 0x7d19e912;
    unsigned int c_5_data = 0x9bcb8650;
    unsigned int c_6_data = 0xee197250;
    unsigned int c_7_data = 0x3a11db95;
    unsigned int c_8_data = 0xb2787691;
    unsigned int c_9_data = 0xb8d6be73;
    unsigned int c_10_data = 0x3b74c1e3;
    unsigned int c_11_data = 0x9ee61671;
    unsigned int c_12_data = 0x16952222;
    unsigned int c_13_data = 0xa1caf13f;
    unsigned int c_14_data = 0x09ac1f68;
    unsigned int c_15_data = 0x30ca0e12;
    unsigned int c_16_data = 0xa7e18675;

    unsigned int w_output_token_result_addr = 0x10000000;
    unsigned int r_output_token_data;
    unsigned int c_output_token_id = 0xd82c;
    unsigned int w_output_token_pass_data = 0xf0f0f0f0;
    unsigned int w_output_token_fail_data = 0x48484848;

    unsigned int r_1_data = 0;
    unsigned int r_2_data = 0;
    unsigned int r_3_data = 0;
    unsigned int r_4_data = 0;
    unsigned int r_5_data = 0;
    unsigned int r_6_data = 0;
    unsigned int r_7_data = 0;
    unsigned int r_8_data = 0;
    unsigned int r_9_data = 0;
    unsigned int r_10_data = 0;
    unsigned int r_11_data = 0;
    unsigned int r_12_data = 0;
    unsigned int r_13_data = 0;
    unsigned int r_14_data = 0;
    unsigned int r_15_data = 0;
    unsigned int r_16_data = 0;

    unsigned int scrt0_read_addr_2 = (SCRT0_BASE_ADDR + 0x0008);

    unsigned int c2_1_data = 0xa1caf13f;
    unsigned int c2_2_data = 0x09ac1f68;
    unsigned int c2_3_data = 0x30ca0e12;
    unsigned int c2_4_data = 0xa7e18675;

    unsigned int result_addr = 0x00412000;

    //Mailbox 1
    *(volatile unsigned int *)0x60003f00 = 0x4;

    //Burst_write 64 bytes in SRAM, this is plain text data
    *(volatile unsigned int *)0x00410000 = 0xe2bec16b;
    *(volatile unsigned int *)0x00410004 = 0x969f402e;
    *(volatile unsigned int *)0x00410008 = 0x117e3de9;
    *(volatile unsigned int *)0x0041000c = 0x2a179373;
    *(volatile unsigned int *)0x00410010 = 0x578a2dae;
    *(volatile unsigned int *)0x00410014 = 0x9cac031e;
    *(volatile unsigned int *)0x00410018 = 0xac6fb79e;
    *(volatile unsigned int *)0x0041001c = 0x518eaf45;
    *(volatile unsigned int *)0x00410020 = 0x461cc830;
    *(volatile unsigned int *)0x00410024 = 0x11e45ca3;
    *(volatile unsigned int *)0x00410028 = 0x19c1fbe5;
    *(volatile unsigned int *)0x0041002c = 0xef520a1a;
    *(volatile unsigned int *)0x00410030 = 0x45249ff6;
    *(volatile unsigned int *)0x00410034 = 0x179b4fdf;
    *(volatile unsigned int *)0x00410038 = 0x7b412bad;
    *(volatile unsigned int *)0x0041003c = 0x10376ce6;

    //Polling -1
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Burst write 100 bytes
    *(volatile unsigned int *)0x60000000 = 0x0104d82c;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = 0x00000040;
    *(volatile unsigned int *)0x6000000c = 0x00410000;
    *(volatile unsigned int *)0x60000010 = 0x00000000;
    *(volatile unsigned int *)0x60000014 = 0x00000040;
    *(volatile unsigned int *)0x60000018 = 0x00411000;
    *(volatile unsigned int *)0x6000001c = 0x00000000;
    *(volatile unsigned int *)0x60000020 = 0x00000044;
    *(volatile unsigned int *)0x60000024 = 0x00000000;
    *(volatile unsigned int *)0x60000028 = 0x00000000;
    *(volatile unsigned int *)0x6000002c = 0x00018010;
    *(volatile unsigned int *)0x60000030 = 0x00000000;
    *(volatile unsigned int *)0x60000034 = 0x03020100;
    *(volatile unsigned int *)0x60000038 = 0x07060504;
    *(volatile unsigned int *)0x6000003c = 0x0b0a0908;
    *(volatile unsigned int *)0x60000040 = 0x0f0e0d0c;
    *(volatile unsigned int *)0x60000044 = 0x16157e2b;
    *(volatile unsigned int *)0x60000048 = 0xa6d2ae28;
    *(volatile unsigned int *)0x6000004c = 0x8815f7ab;
    *(volatile unsigned int *)0x60000050 = 0x3c4fcf09;
    *(volatile unsigned int *)0x60000054 = 0x00000000;
    *(volatile unsigned int *)0x60000058 = 0x00000000;
    *(volatile unsigned int *)0x6000005c = 0x00000000;
    *(volatile unsigned int *)0x60000060 = 0x00000000;

    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;

    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Check output token status
    r_output_token_data = *((volatile unsigned int *)0x60000000);

    if (r_output_token_data == c_output_token_id) {
        *((volatile unsigned int *)result_addr) = w_output_token_pass_data;
    } else {
        *((volatile unsigned int *)result_addr) = w_output_token_fail_data;
    }

    //Check block 64 bytes output encrypted data in SRAM
    r_1_data = *((volatile unsigned int *)0x00411000);
    r_2_data = *((volatile unsigned int *)0x00411004);
    r_3_data = *((volatile unsigned int *)0x00411008);
    r_4_data = *((volatile unsigned int *)0x0041100c);
    r_5_data = *((volatile unsigned int *)0x00411010);
    r_6_data = *((volatile unsigned int *)0x00411014);
    r_7_data = *((volatile unsigned int *)0x00411018);
    r_8_data = *((volatile unsigned int *)0x0041101c);
    r_9_data = *((volatile unsigned int *)0x00411020);
    r_10_data = *((volatile unsigned int *)0x00411024);
    r_11_data = *((volatile unsigned int *)0x00411028);
    r_12_data = *((volatile unsigned int *)0x0041102c);
    r_13_data = *((volatile unsigned int *)0x00411030);
    r_14_data = *((volatile unsigned int *)0x00411034);
    r_15_data = *((volatile unsigned int *)0x00411038);
    r_16_data = *((volatile unsigned int *)0x0041103c);

    if ((r_1_data == c_1_data)
        &&(r_2_data == c_2_data)
        &&(r_3_data == c_3_data)
        &&(r_4_data == c_4_data)
        &&(r_5_data == c_5_data)
        &&(r_6_data == c_6_data)
        &&(r_7_data == c_7_data)
        &&(r_8_data == c_8_data)
        &&(r_9_data == c_9_data)
        &&(r_10_data == c_10_data)
        &&(r_11_data == c_11_data)
        &&(r_12_data == c_12_data)
        &&(r_13_data == c_13_data)
        &&(r_14_data == c_14_data)
        &&(r_15_data == c_15_data)
        &&(r_16_data == c_16_data)
        ) {
        //compare pass
        *((volatile unsigned int *)result_addr) = w_output_token_pass_data;
    } else {
        //compare fail
        *((volatile unsigned int *)result_addr) = w_output_token_fail_data;
    }

    //Check Token ID in output data's last word
    r_output_token_data = *((volatile unsigned int *)0x10002040);

    if (r_output_token_data == c_output_token_id) {
        *((volatile unsigned int *)result_addr) = w_output_token_pass_data;
    } else {
        *((volatile unsigned int *)result_addr) = w_output_token_fail_data;
    }

    //Check 16 bytes in offset 0x0008
    r_1_data = *((volatile unsigned int *)0x60000008);
    r_2_data = *((volatile unsigned int *)0x6000000c);
    r_3_data = *((volatile unsigned int *)0x60000010);
    r_4_data = *((volatile unsigned int *)0x60000014);

    if ( (r_1_data == c2_1_data)
         &&(r_2_data == c2_2_data)
         &&(r_3_data == c2_3_data)
         &&(r_4_data == c2_4_data)
         ){
        *((volatile unsigned int *)result_addr) = w_output_token_pass_data;
    } else {
        *((volatile unsigned int *)result_addr) = w_output_token_fail_data;
    }

    //Write
    *(volatile unsigned int *)0x60003f00 = 0x2;

}


void fpga_test_onrom_level2_scrt0_all_reg_access(void) //pass on FPGA
{
    unsigned int data;

    unsigned int scrt0_write_addr_1 = SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_CTRL;
    //unsigned int scrt0_write_addr_2 = SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_RESET; //Not sure how to use it, skip it temporaly

    unsigned int scrt0_read_addr_1 = 0x60003F00; //SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_STAT;
    unsigned int scrt0_read_addr_2 = 0x60003f04; //SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_RAWSTAT;
    unsigned int scrt0_read_addr_3 = 0x60003f08; //SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_LINKID;
    unsigned int scrt0_read_addr_4 = 0x60003f0c; //SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_OUTID;
    unsigned int scrt0_read_addr_5 = 0x60003f10; //SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_LOCKOUT;
    unsigned int scrt0_read_addr_6 = 0x60003fe0; //SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MODULE_STATUS;
    unsigned int scrt0_read_addr_7 = 0x60003ff4; //SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_EIP_OPTIONS2;
    unsigned int scrt0_read_addr_8 = 0x60003ff8; //SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_EIP_OPTIONS;
    unsigned int scrt0_read_addr_9 = 0x60003ffc; //SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_EIP_VERSION;


    /*
        write - EIP130_REGISTEROFFSET_MAILBOX_CTRL
        read - EIP130_REGISTEROFFSET_MAILBOX_STAT
      */
    //Mailbox 1
    *(volatile unsigned int *)scrt0_write_addr_1 = 0x4;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    data = *((volatile unsigned int *)scrt0_read_addr_1);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    //Mailbox 2
    *(volatile unsigned int *)scrt0_write_addr_1 = 0x40;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    data = *((volatile unsigned int *)scrt0_read_addr_1);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
#endif
    //Mailbox 3
    *(volatile unsigned int *)scrt0_write_addr_1 = 0x400;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    data = *((volatile unsigned int *)scrt0_read_addr_1);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    //Mailbox 4
    *(volatile unsigned int *)scrt0_write_addr_1 = 0x4000;
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    data = *((volatile unsigned int *)scrt0_read_addr_1);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif

    /*
        write - EIP130_REGISTEROFFSET_MAILBOX_RESET
      */
    //Not sure how to use it, skip it temporaly
    //*(volatile unsigned int *)scrt0_write_addr_2 = 0x8; //Not sure how to use it, skip it temporaly


    /*
        read -
                 EIP130_REGISTEROFFSET_MAILBOX_RAWSTAT
                 EIP130_REGISTEROFFSET_MAILBOX_LINKID
                 EIP130_REGISTEROFFSET_MAILBOX_OUTID
                 EIP130_REGISTEROFFSET_MAILBOX_LOCKOUT
                 EIP130_REGISTEROFFSET_MODULE_STATUS
                 EIP130_REGISTEROFFSET_EIP_OPTIONS2
                 EIP130_REGISTEROFFSET_EIP_OPTIONS
                 EIP130_REGISTEROFFSET_EIP_VERSION
      */
    data = *((volatile unsigned int *)scrt0_read_addr_2);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    data = *((volatile unsigned int *)scrt0_read_addr_3);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    data = *((volatile unsigned int *)scrt0_read_addr_4);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    data = *((volatile unsigned int *)scrt0_read_addr_5);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    data = *((volatile unsigned int *)scrt0_read_addr_6);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    data = *((volatile unsigned int *)scrt0_read_addr_7);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    data = *((volatile unsigned int *)scrt0_read_addr_8);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
    data = *((volatile unsigned int *)scrt0_read_addr_9);
#ifdef HAS_NOP_DELAY
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
#endif
}


void fpga_test_onrom_level4_scrt0_aes_64bytes(void) //pass on FPGA
{
    unsigned int data;

    unsigned int result_addr = 0x00403200;
    unsigned int flag_addr   = 0x00403300;

    unsigned int c_1_data = 0xacab4976;
    unsigned int c_2_data = 0x46b21981;
    unsigned int c_3_data = 0x9b8ee9ce;
    unsigned int c_4_data = 0x7d19e912;
    unsigned int c_5_data = 0x9bcb8650;
    unsigned int c_6_data = 0xee197250;
    unsigned int c_7_data = 0x3a11db95;
    unsigned int c_8_data = 0xb2787691;
    unsigned int c_9_data = 0xb8d6be73;
    unsigned int c_10_data = 0x3b74c1e3;
    unsigned int c_11_data = 0x9ee61671;
    unsigned int c_12_data = 0x16952222;
    unsigned int c_13_data = 0xa1caf13f;
    unsigned int c_14_data = 0x09ac1f68;
    unsigned int c_15_data = 0x30ca0e12;
    unsigned int c_16_data = 0xa7e18675;

    unsigned int r_output_token_data;
    unsigned int c_output_token_id = 0xd82c;
    unsigned int w_output_token_pass_data = 0xf0f0f0f0;
    unsigned int w_output_token_fail_data = 0x48484848;

    unsigned int r_1_data = 0;
    unsigned int r_2_data = 0;
    unsigned int r_3_data = 0;
    unsigned int r_4_data = 0;
    unsigned int r_5_data = 0;
    unsigned int r_6_data = 0;
    unsigned int r_7_data = 0;
    unsigned int r_8_data = 0;
    unsigned int r_9_data = 0;
    unsigned int r_10_data = 0;
    unsigned int r_11_data = 0;
    unsigned int r_12_data = 0;
    unsigned int r_13_data = 0;
    unsigned int r_14_data = 0;
    unsigned int r_15_data = 0;
    unsigned int r_16_data = 0;

    unsigned int c2_1_data = 0xa1caf13f;
    unsigned int c2_2_data = 0x09ac1f68;
    unsigned int c2_3_data = 0x30ca0e12;
    unsigned int c2_4_data = 0xa7e18675;


    //Mailbox 1
    *(volatile unsigned int *)0x60003f00 = 0x4;

    //Burst_write 64 bytes in SRAM @ 0x1000_3000, this is plain text data
    *(volatile unsigned int *)0x00403000 = 0xe2bec16b;
    *(volatile unsigned int *)0x00403004 = 0x969f402e;
    *(volatile unsigned int *)0x00403008 = 0x117e3de9;
    *(volatile unsigned int *)0x0040300c = 0x2a179373;
    *(volatile unsigned int *)0x00403010 = 0x578a2dae;
    *(volatile unsigned int *)0x00403014 = 0x9cac031e;
    *(volatile unsigned int *)0x00403018 = 0xac6fb79e;
    *(volatile unsigned int *)0x0040301c = 0x518eaf45;
    *(volatile unsigned int *)0x00403020 = 0x461cc830;
    *(volatile unsigned int *)0x00403024 = 0x11e45ca3;
    *(volatile unsigned int *)0x00403028 = 0x19c1fbe5;
    *(volatile unsigned int *)0x0040302c = 0xef520a1a;
    *(volatile unsigned int *)0x00403030 = 0x45249ff6;
    *(volatile unsigned int *)0x00403034 = 0x179b4fdf;
    *(volatile unsigned int *)0x00403038 = 0x7b412bad;
    *(volatile unsigned int *)0x0040303c = 0x10376ce6;

    //Polling -1
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Burst write 100 bytes
    *(volatile unsigned int *)0x60000000 = 0x0104d82c;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = 0x00000040;
    *(volatile unsigned int *)0x6000000c = 0x00403000;
    *(volatile unsigned int *)0x60000010 = 0x00000000;
    *(volatile unsigned int *)0x60000014 = 0x00000040;
    *(volatile unsigned int *)0x60000018 = 0x00403100;
    *(volatile unsigned int *)0x6000001c = 0x00000000;
    *(volatile unsigned int *)0x60000020 = 0x00000044;
    *(volatile unsigned int *)0x60000024 = 0x00000000;
    *(volatile unsigned int *)0x60000028 = 0x00000000;
    *(volatile unsigned int *)0x6000002c = 0x00018010;
    *(volatile unsigned int *)0x60000030 = 0x00000000;
    *(volatile unsigned int *)0x60000034 = 0x03020100;
    *(volatile unsigned int *)0x60000038 = 0x07060504;
    *(volatile unsigned int *)0x6000003c = 0x0b0a0908;
    *(volatile unsigned int *)0x60000040 = 0x0f0e0d0c;
    *(volatile unsigned int *)0x60000044 = 0x16157e2b;
    *(volatile unsigned int *)0x60000048 = 0xa6d2ae28;
    *(volatile unsigned int *)0x6000004c = 0x8815f7ab;
    *(volatile unsigned int *)0x60000050 = 0x3c4fcf09;
    *(volatile unsigned int *)0x60000054 = 0x00000000;
    *(volatile unsigned int *)0x60000058 = 0x00000000;
    *(volatile unsigned int *)0x6000005c = 0x00000000;
    *(volatile unsigned int *)0x60000060 = 0x00000000;

    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;

    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }


    //Check output token status
    r_output_token_data = *((volatile unsigned int *)0x60000000);

    if (r_output_token_data == c_output_token_id) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Check block 64 bytes output encrypted data in SRAM
    r_1_data = *((volatile unsigned int *)0x00403100);
    r_2_data = *((volatile unsigned int *)0x00403104);
    r_3_data = *((volatile unsigned int *)0x00403108);
    r_4_data = *((volatile unsigned int *)0x0040310c);
    r_5_data = *((volatile unsigned int *)0x00403110);
    r_6_data = *((volatile unsigned int *)0x00403114);
    r_7_data = *((volatile unsigned int *)0x00403118);
    r_8_data = *((volatile unsigned int *)0x0040311c);
    r_9_data = *((volatile unsigned int *)0x00403120);
    r_10_data = *((volatile unsigned int *)0x00403124);
    r_11_data = *((volatile unsigned int *)0x00403128);
    r_12_data = *((volatile unsigned int *)0x0040312c);
    r_13_data = *((volatile unsigned int *)0x00403130);
    r_14_data = *((volatile unsigned int *)0x00403134);
    r_15_data = *((volatile unsigned int *)0x00403138);
    r_16_data = *((volatile unsigned int *)0x0040313c);

    if ((r_1_data == c_1_data)
        &&(r_2_data == c_2_data)
        &&(r_3_data == c_3_data)
        &&(r_4_data == c_4_data)
        &&(r_5_data == c_5_data)
        &&(r_6_data == c_6_data)
        &&(r_7_data == c_7_data)
        &&(r_8_data == c_8_data)
        &&(r_9_data == c_9_data)
        &&(r_10_data == c_10_data)
        &&(r_11_data == c_11_data)
        &&(r_12_data == c_12_data)
        &&(r_13_data == c_13_data)
        &&(r_14_data == c_14_data)
        &&(r_15_data == c_15_data)
        &&(r_16_data == c_16_data)
        ) {
        //compare pass
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        //compare fail
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Check Token ID in output data's last word
    r_output_token_data = *((volatile unsigned int *)0x00403140);

    if (r_output_token_data == c_output_token_id) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Check 16 bytes in offset 0x0008
    r_1_data = *((volatile unsigned int *)0x60000008);
    r_2_data = *((volatile unsigned int *)0x6000000c);
    r_3_data = *((volatile unsigned int *)0x60000010);
    r_4_data = *((volatile unsigned int *)0x60000014);

    if ( (r_1_data == c2_1_data)
         &&(r_2_data == c2_2_data)
         &&(r_3_data == c2_3_data)
         &&(r_4_data == c2_4_data)
         ){
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Write
    *(volatile unsigned int *)0x60003f00 = 0x2;

}


/*
 * 8-bit read/write.
 */

unsigned char readb_reg(unsigned int reg)
{
	unsigned char val;

	val = *((volatile unsigned char *)reg);

	return val;
}
void writeb_reg(unsigned int reg, unsigned char val)
{
	*(volatile unsigned char *)reg = val;
}


/*
 * 32-bit read/write.
 */
unsigned int read_reg(unsigned int reg)
{
	unsigned int val;

	val = *((volatile unsigned int *)reg);

	return val;
}
void write_reg(unsigned int reg, unsigned int val)
{
	*(volatile unsigned int *)reg = val;
}
void write_reg_buf(unsigned int addr, unsigned int sizeWords, unsigned int * bufWords)
{
    unsigned int i;
    for (i = 0; i < sizeWords; i++) {
        write_reg((addr + (i*4)), *(bufWords + i));
    }
}


void fpga_test_onrom_level4_scrt0_aes_64bytes_debug(void) //pass on FPGA
{
    unsigned int data = 0;
    unsigned int data2 = 0;

    unsigned int result_addr = 0x00403200;
    unsigned int flag_addr   = 0x00403300;

    unsigned int w_output_token_pass_data = 0xf0f0f0f0;
    unsigned int w_output_token_fail_data = 0x48484848;

    unsigned int r_output_token_data;
    unsigned int c_output_token_id = 0xd82c;

    unsigned int r_1_data = 0;
    unsigned int r_2_data = 0;
    unsigned int r_3_data = 0;
    unsigned int r_4_data = 0;
    unsigned int r_5_data = 0;
    unsigned int r_6_data = 0;
    unsigned int r_7_data = 0;
    unsigned int r_8_data = 0;
    unsigned int r_9_data = 0;
    unsigned int r_10_data = 0;
    unsigned int r_11_data = 0;
    unsigned int r_12_data = 0;
    unsigned int r_13_data = 0;
    unsigned int r_14_data = 0;
    unsigned int r_15_data = 0;
    unsigned int r_16_data = 0;

    unsigned int c_1_data = 0xacab4976;
    unsigned int c_2_data = 0x46b21981;
    unsigned int c_3_data = 0x9b8ee9ce;
    unsigned int c_4_data = 0x7d19e912;
    unsigned int c_5_data = 0x9bcb8650;
    unsigned int c_6_data = 0xee197250;
    unsigned int c_7_data = 0x3a11db95;
    unsigned int c_8_data = 0xb2787691;
    unsigned int c_9_data = 0xb8d6be73;
    unsigned int c_10_data = 0x3b74c1e3;
    unsigned int c_11_data = 0x9ee61671;
    unsigned int c_12_data = 0x16952222;
    unsigned int c_13_data = 0xa1caf13f;
    unsigned int c_14_data = 0x09ac1f68;
    unsigned int c_15_data = 0x30ca0e12;
    unsigned int c_16_data = 0xa7e18675;

    unsigned int c2_1_data = 0xa1caf13f;
    unsigned int c2_2_data = 0x09ac1f68;
    unsigned int c2_3_data = 0x30ca0e12;
    unsigned int c2_4_data = 0xa7e18675;


    //Mailbox 1
    *(volatile unsigned int *)0x60003f00 = 0x4;

    //Burst_write 64 bytes in SRAM, this is plain text data
    *(volatile unsigned int *)0x00403000 = 0xe2bec16b;
    *(volatile unsigned int *)0x00403004 = 0x969f402e;
    *(volatile unsigned int *)0x00403008 = 0x117e3de9;
    *(volatile unsigned int *)0x0040300c = 0x2a179373;
    *(volatile unsigned int *)0x00403010 = 0x578a2dae;
    *(volatile unsigned int *)0x00403014 = 0x9cac031e;
    *(volatile unsigned int *)0x00403018 = 0xac6fb79e;
    *(volatile unsigned int *)0x0040301c = 0x518eaf45;
    *(volatile unsigned int *)0x00403020 = 0x461cc830;
    *(volatile unsigned int *)0x00403024 = 0x11e45ca3;
    *(volatile unsigned int *)0x00403028 = 0x19c1fbe5;
    *(volatile unsigned int *)0x0040302c = 0xef520a1a;
    *(volatile unsigned int *)0x00403030 = 0x45249ff6;
    *(volatile unsigned int *)0x00403034 = 0x179b4fdf;
    *(volatile unsigned int *)0x00403038 = 0x7b412bad;
    *(volatile unsigned int *)0x0040303c = 0x10376ce6;

    //Polling -1
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Burst write 100 bytes
    *(volatile unsigned int *)0x60000000 = 0x0104d82c;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = 0x00000040;
    *(volatile unsigned int *)0x6000000c = 0x00403000;
    *(volatile unsigned int *)0x60000010 = 0x00000000;
    *(volatile unsigned int *)0x60000014 = 0x00000040;
    *(volatile unsigned int *)0x60000018 = 0x00403100;
    *(volatile unsigned int *)0x6000001c = 0x00000000;
    *(volatile unsigned int *)0x60000020 = 0x00000044;
    *(volatile unsigned int *)0x60000024 = 0x00000000;
    *(volatile unsigned int *)0x60000028 = 0x00000000;
    *(volatile unsigned int *)0x6000002c = 0x00018010;
    *(volatile unsigned int *)0x60000030 = 0x00000000;
    *(volatile unsigned int *)0x60000034 = 0x03020100;
    *(volatile unsigned int *)0x60000038 = 0x07060504;
    *(volatile unsigned int *)0x6000003c = 0x0b0a0908;
    *(volatile unsigned int *)0x60000040 = 0x0f0e0d0c;
    *(volatile unsigned int *)0x60000044 = 0x16157e2b;
    *(volatile unsigned int *)0x60000048 = 0xa6d2ae28;
    *(volatile unsigned int *)0x6000004c = 0x8815f7ab;
    *(volatile unsigned int *)0x60000050 = 0x3c4fcf09;
    *(volatile unsigned int *)0x60000054 = 0x00000000;
    *(volatile unsigned int *)0x60000058 = 0x00000000;
    *(volatile unsigned int *)0x6000005c = 0x00000000;
    *(volatile unsigned int *)0x60000060 = 0x00000000;

    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;

    //Polling - 2
    while(1){
        /*
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }*/

        //Do something here to switch for a while
        data2++;
        *(volatile unsigned int *)0x00403400 = data2;
    }

    //Check output token status
    r_output_token_data = *((volatile unsigned int *)0x60000000);

    if (r_output_token_data == c_output_token_id) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Check block 64 bytes output encrypted data in SRAM
    r_1_data = *((volatile unsigned int *)0x00403100);
    r_2_data = *((volatile unsigned int *)0x00403104);
    r_3_data = *((volatile unsigned int *)0x00403108);
    r_4_data = *((volatile unsigned int *)0x0040310c);
    r_5_data = *((volatile unsigned int *)0x00403110);
    r_6_data = *((volatile unsigned int *)0x00403114);
    r_7_data = *((volatile unsigned int *)0x00403118);
    r_8_data = *((volatile unsigned int *)0x0040311c);
    r_9_data = *((volatile unsigned int *)0x00403120);
    r_10_data = *((volatile unsigned int *)0x00403124);
    r_11_data = *((volatile unsigned int *)0x00403128);
    r_12_data = *((volatile unsigned int *)0x0040312c);
    r_13_data = *((volatile unsigned int *)0x00403130);
    r_14_data = *((volatile unsigned int *)0x00403134);
    r_15_data = *((volatile unsigned int *)0x00403138);
    r_16_data = *((volatile unsigned int *)0x0040313c);

    if ((r_1_data == c_1_data)
        &&(r_2_data == c_2_data)
        &&(r_3_data == c_3_data)
        &&(r_4_data == c_4_data)
        &&(r_5_data == c_5_data)
        &&(r_6_data == c_6_data)
        &&(r_7_data == c_7_data)
        &&(r_8_data == c_8_data)
        &&(r_9_data == c_9_data)
        &&(r_10_data == c_10_data)
        &&(r_11_data == c_11_data)
        &&(r_12_data == c_12_data)
        &&(r_13_data == c_13_data)
        &&(r_14_data == c_14_data)
        &&(r_15_data == c_15_data)
        &&(r_16_data == c_16_data)
        ) {
        //compare pass
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        //compare fail
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Check Token ID in output data's last word
    r_output_token_data = *((volatile unsigned int *)0x00403140);

    if (r_output_token_data == c_output_token_id) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Check 16 bytes in offset 0x0008
    r_1_data = *((volatile unsigned int *)0x60000008);
    r_2_data = *((volatile unsigned int *)0x6000000c);
    r_3_data = *((volatile unsigned int *)0x60000010);
    r_4_data = *((volatile unsigned int *)0x60000014);

    if ( (r_1_data == c2_1_data)
         &&(r_2_data == c2_2_data)
         &&(r_3_data == c2_3_data)
         &&(r_4_data == c2_4_data)
         ){
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //Write
    *(volatile unsigned int *)0x60003f00 = 0x2;

}
#endif //#if 0

void wrCore(unsigned int offset, unsigned int val) //test pass
{
    *((volatile unsigned int *)(SCRT0_BASE_ADDR + offset)) = val;
}


unsigned int rdCore(unsigned int offset) //test pass
{
	unsigned int val;

	val = *((volatile unsigned int *)(SCRT0_BASE_ADDR + offset));

	return val;
}

void wrCoreBlock(unsigned int offset, unsigned int * inDataAddr, int sizeWords)
{
    int i;

    for (i = 0; i < sizeWords; i++) {
        *((volatile unsigned int *)((SCRT0_BASE_ADDR + offset) + (i*4))) = *(inDataAddr + i);
    }
}

/*
 * SRAM Access
 */
void write_sram(unsigned int sramStartAddr, unsigned int * inDataAddr, int sizeWords) //test pass
{
    int i;

    for (i = 0; i < sizeWords; i++) {
        *((volatile unsigned int *)(sramStartAddr + (i*4))) = *(inDataAddr + i);
    }
}

//Note 1: Have a discussion with Vincent, it's better to access the RAM address from 0x0041a000
bool itp_scrt_scrt0_reg_access(void)
{
    unsigned int scrt0_base = 0x60000000;
    unsigned int scrt0_ctrl_addr;
    unsigned int scrt0_stat_addr;
    unsigned int data;

    scrt0_ctrl_addr = (scrt0_base + EIP130_REGISTEROFFSET_MAILBOX_CTRL);
    scrt0_stat_addr = (scrt0_base + EIP130_REGISTEROFFSET_MAILBOX_STAT);

    //write
    *((volatile unsigned int *)scrt0_ctrl_addr) = 0x4;
    *((volatile unsigned int *)scrt0_ctrl_addr) = 0x40;
    *((volatile unsigned int *)scrt0_ctrl_addr) = 0x400;
    *((volatile unsigned int *)scrt0_ctrl_addr) = 0x4000;

#ifdef HAS_NOP_DELAY
    delay(5);
#endif

    //read
    data = *((volatile unsigned int *)scrt0_stat_addr);

    if (data != 0x4444) {
        return false;
    } else {
        return true;
    }
}


bool itp_scrt_scrt1_reg_access(void)
{
    unsigned int scrt1_base = 0x80000000;
    unsigned int scrt1_reg_scrt_ctrl_offset = 0x0;
    unsigned int scrt1_reg_scrt_ctrl_addr;
    unsigned int data;

    scrt1_reg_scrt_ctrl_addr = scrt1_base + scrt1_reg_scrt_ctrl_offset;

    //write
    *(volatile unsigned int *)scrt1_reg_scrt_ctrl_addr = 0x1;

#ifdef HAS_NOP_DELAY
    delay(5);
#endif

    //read
    data = *((volatile unsigned int *)scrt1_reg_scrt_ctrl_addr);

    if (data != 1) {
        return false;
    } else {
        return true;
    }
}


bool itp_scrt_sram_access(void)
{
    //unsigned int sram_addr = 0x00410000;
    unsigned int sram_addr = ITP_SCRT_SRAM_BASE_ADDR;
    unsigned int data;
    unsigned int value = 0x11223344;

    //write
    *((volatile unsigned int *)sram_addr) = value;

#ifdef HAS_NOP_DELAY
    delay(5);
#endif

    //read
    data = *((volatile unsigned int *)sram_addr);

    if (data != value) {
        return false;
    } else {
        return true;
    }
}


bool itp_scrt_sram_burst(void)
{
    //unsigned int sram_addr   = 0x00410000;
    //unsigned int result_addr = 0x00412000;
    //unsigned int flag_addr   = 0x00413000;
    unsigned int sram_addr;
    unsigned int result_addr;
    //unsigned int flag_addr;

    unsigned int w_1_addr;
    unsigned int w_2_addr;
    unsigned int w_3_addr;
    unsigned int w_4_addr;
    unsigned int w_5_addr;
    unsigned int w_6_addr;

    unsigned int w_1_data = 0x11223344;
    unsigned int w_2_data = 0x55667788;
    unsigned int w_3_data = 0x99aabbcc;
    unsigned int w_4_data = 0xddeef00f;
    unsigned int w_5_data = 0xf9ab1357;
    unsigned int w_6_data = 0x24689876;

    unsigned int r_1_data = 0;
    unsigned int r_2_data = 0;
    unsigned int r_3_data = 0;
    unsigned int r_4_data = 0;
    unsigned int r_5_data = 0;
    unsigned int r_6_data = 0;

    unsigned int val_pass = 0xf0f0f0f0;
    unsigned int val_fail = 0x48484848;
    //unsigned int result_data = 0;

    sram_addr = ITP_SCRT_SRAM_BASE_ADDR;
    result_addr = (ITP_SCRT_SRAM_BASE_ADDR + 0x1000);
    //flag_addr   = (ITP_SCRT_SRAM_BASE_ADDR + 0x2000);;

    w_1_addr = (sram_addr + 0x00000000);
    w_2_addr = (sram_addr + 0x00000004);
    w_3_addr = (sram_addr + 0x00000008);
    w_4_addr = (sram_addr + 0x0000000c);
    w_5_addr = (sram_addr + 0x00000010);
    w_6_addr = (sram_addr + 0x00000014);

    //write
    *((volatile unsigned int *)w_1_addr) = w_1_data;
    *((volatile unsigned int *)w_2_addr) = w_2_data;
    *((volatile unsigned int *)w_3_addr) = w_3_data;
    *((volatile unsigned int *)w_4_addr) = w_4_data;
    *((volatile unsigned int *)w_5_addr) = w_5_data;
    *((volatile unsigned int *)w_6_addr) = w_6_data;

#ifdef HAS_NOP_DELAY
    delay(5);
#endif

    //read
    r_1_data = *((volatile unsigned int *)w_1_addr);
    r_2_data = *((volatile unsigned int *)w_2_addr);
    r_3_data = *((volatile unsigned int *)w_3_addr);
    r_4_data = *((volatile unsigned int *)w_4_addr);
    r_5_data = *((volatile unsigned int *)w_5_addr);
    r_6_data = *((volatile unsigned int *)w_6_addr);

    //compare
    if ((r_1_data == w_1_data)
      &&(r_2_data == w_2_data)
      &&(r_3_data == w_3_data)
      &&(r_4_data == w_4_data)
      &&(r_5_data == w_5_data)
      &&(r_6_data == w_6_data))
    {
      *((volatile unsigned int *)result_addr) = val_pass;
      return true;
    } else {
      *((volatile unsigned int *)result_addr) = val_fail;
      return false;
    }

    //result_data = *((volatile unsigned int *)result_addr);
    //*(volatile unsigned int *)flag_addr = 0x10101010;
}


bool itp_scrt_scrt0_all_reg_access(void)
{
    unsigned int data;

    unsigned int scrt0_write_addr_1 = SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_CTRL;
    //unsigned int scrt0_write_addr_2 = SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_RESET; //Not sure how to use it, skip it temporaly

    unsigned int scrt0_read_addr_1 = 0x60003F00; //SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_STAT;
    unsigned int scrt0_read_addr_2 = 0x60003f04; //SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_RAWSTAT;
    unsigned int scrt0_read_addr_3 = 0x60003f08; //SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_LINKID;
    unsigned int scrt0_read_addr_4 = 0x60003f0c; //SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_OUTID;
    unsigned int scrt0_read_addr_5 = 0x60003f10; //SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_LOCKOUT;
    unsigned int scrt0_read_addr_6 = 0x60003fe0; //SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MODULE_STATUS;
    unsigned int scrt0_read_addr_7 = 0x60003ff4; //SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_EIP_OPTIONS2;
    unsigned int scrt0_read_addr_8 = 0x60003ff8; //SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_EIP_OPTIONS;
    unsigned int scrt0_read_addr_9 = 0x60003ffc; //SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_EIP_VERSION;


    /*
        write - EIP130_REGISTEROFFSET_MAILBOX_CTRL
        read - EIP130_REGISTEROFFSET_MAILBOX_STAT
      */
    //Mailbox 1
    *(volatile unsigned int *)scrt0_write_addr_1 = 0x4;

#ifdef HAS_NOP_DELAY
    delay(5);
#endif

    data = *((volatile unsigned int *)scrt0_read_addr_1);

    //Mailbox 2
    *(volatile unsigned int *)scrt0_write_addr_1 = 0x40;

#ifdef HAS_NOP_DELAY
    delay(5);
#endif

    data = *((volatile unsigned int *)scrt0_read_addr_1);

    //Mailbox 3
    *(volatile unsigned int *)scrt0_write_addr_1 = 0x400;

#ifdef HAS_NOP_DELAY
    delay(5);
#endif

    data = *((volatile unsigned int *)scrt0_read_addr_1);

    //Mailbox 4
    *(volatile unsigned int *)scrt0_write_addr_1 = 0x4000;

#ifdef HAS_NOP_DELAY
    delay(5);
#endif

    data = *((volatile unsigned int *)scrt0_read_addr_1);

    /*
        write - EIP130_REGISTEROFFSET_MAILBOX_RESET
      */
    //Not sure how to use it, skip it temporaly
    //*(volatile unsigned int *)scrt0_write_addr_2 = 0x8; //Not sure how to use it, skip it temporaly

    if (data == 0) {
        return false;
    }


    /*
        read -
                 EIP130_REGISTEROFFSET_MAILBOX_RAWSTAT
                 EIP130_REGISTEROFFSET_MAILBOX_LINKID
                 EIP130_REGISTEROFFSET_MAILBOX_OUTID
                 EIP130_REGISTEROFFSET_MAILBOX_LOCKOUT
                 EIP130_REGISTEROFFSET_MODULE_STATUS
                 EIP130_REGISTEROFFSET_EIP_OPTIONS2
                 EIP130_REGISTEROFFSET_EIP_OPTIONS
                 EIP130_REGISTEROFFSET_EIP_VERSION
      */
    data = *((volatile unsigned int *)scrt0_read_addr_2);
    data = *((volatile unsigned int *)scrt0_read_addr_3);
    data = *((volatile unsigned int *)scrt0_read_addr_4);
    data = *((volatile unsigned int *)scrt0_read_addr_5);
    data = *((volatile unsigned int *)scrt0_read_addr_6);
    data = *((volatile unsigned int *)scrt0_read_addr_7);
    data = *((volatile unsigned int *)scrt0_read_addr_8);
    data = *((volatile unsigned int *)scrt0_read_addr_9);

    return true;
}


bool itp_scrt_aes_64bytes(void)
{
    unsigned int data;

    unsigned int sram_addr_in;
    unsigned int sram_addr_out;
    unsigned int result_addr;

    unsigned int c_1_data = 0xacab4976;
    unsigned int c_2_data = 0x46b21981;
    unsigned int c_3_data = 0x9b8ee9ce;
    unsigned int c_4_data = 0x7d19e912;
    unsigned int c_5_data = 0x9bcb8650;
    unsigned int c_6_data = 0xee197250;
    unsigned int c_7_data = 0x3a11db95;
    unsigned int c_8_data = 0xb2787691;
    unsigned int c_9_data = 0xb8d6be73;
    unsigned int c_10_data = 0x3b74c1e3;
    unsigned int c_11_data = 0x9ee61671;
    unsigned int c_12_data = 0x16952222;
    unsigned int c_13_data = 0xa1caf13f;
    unsigned int c_14_data = 0x09ac1f68;
    unsigned int c_15_data = 0x30ca0e12;
    unsigned int c_16_data = 0xa7e18675;

    unsigned int r_output_token_data;
    unsigned int c_output_token_id = 0xd82c;
    unsigned int w_output_token_pass_data = 0xf0f0f0f0;
    unsigned int w_output_token_fail_data = 0x48484848;

    unsigned int r_1_data = 0;
    unsigned int r_2_data = 0;
    unsigned int r_3_data = 0;
    unsigned int r_4_data = 0;
    unsigned int r_5_data = 0;
    unsigned int r_6_data = 0;
    unsigned int r_7_data = 0;
    unsigned int r_8_data = 0;
    unsigned int r_9_data = 0;
    unsigned int r_10_data = 0;
    unsigned int r_11_data = 0;
    unsigned int r_12_data = 0;
    unsigned int r_13_data = 0;
    unsigned int r_14_data = 0;
    unsigned int r_15_data = 0;
    unsigned int r_16_data = 0;

    unsigned int c2_1_data = 0xa1caf13f;
    unsigned int c2_2_data = 0x09ac1f68;
    unsigned int c2_3_data = 0x30ca0e12;
    unsigned int c2_4_data = 0xa7e18675;

    bool ret = true;

    sram_addr_in = ITP_SCRT_SRAM_BASE_ADDR;
    sram_addr_out = (sram_addr_in + 0x100);
    result_addr = (sram_addr_in + 0x1000);

    //Mailbox 1
    *(volatile unsigned int *)0x60003f00 = 0x4;

    //Burst_write 64 bytes in SRAM @ 0x1000_3000, this is plain text data
    *((volatile unsigned int *)(sram_addr_in + 0)) = 0xe2bec16b;
    *((volatile unsigned int *)(sram_addr_in + 4)) = 0x969f402e;
    *((volatile unsigned int *)(sram_addr_in + 8)) = 0x117e3de9;
    *((volatile unsigned int *)(sram_addr_in + 12)) = 0x2a179373;
    *((volatile unsigned int *)(sram_addr_in + 16)) = 0x578a2dae;
    *((volatile unsigned int *)(sram_addr_in + 20)) = 0x9cac031e;
    *((volatile unsigned int *)(sram_addr_in + 24)) = 0xac6fb79e;
    *((volatile unsigned int *)(sram_addr_in + 28)) = 0x518eaf45;
    *((volatile unsigned int *)(sram_addr_in + 32)) = 0x461cc830;
    *((volatile unsigned int *)(sram_addr_in + 36)) = 0x11e45ca3;
    *((volatile unsigned int *)(sram_addr_in + 40)) = 0x19c1fbe5;
    *((volatile unsigned int *)(sram_addr_in + 44)) = 0xef520a1a;
    *((volatile unsigned int *)(sram_addr_in + 48)) = 0x45249ff6;
    *((volatile unsigned int *)(sram_addr_in + 52)) = 0x179b4fdf;
    *((volatile unsigned int *)(sram_addr_in + 56)) = 0x7b412bad;
    *((volatile unsigned int *)(sram_addr_in + 60)) = 0x10376ce6;

    //Polling -1
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Burst write 100 bytes
    *(volatile unsigned int *)0x60000000 = 0x0104d82c;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = 0x00000040;
    *(volatile unsigned int *)0x6000000c = sram_addr_in;
    *(volatile unsigned int *)0x60000010 = 0x00000000;
    *(volatile unsigned int *)0x60000014 = 0x00000040;
    *(volatile unsigned int *)0x60000018 = sram_addr_out;
    *(volatile unsigned int *)0x6000001c = 0x00000000;
    *(volatile unsigned int *)0x60000020 = 0x00000044;
    *(volatile unsigned int *)0x60000024 = 0x00000000;
    *(volatile unsigned int *)0x60000028 = 0x00000000;
    *(volatile unsigned int *)0x6000002c = 0x00018010;
    *(volatile unsigned int *)0x60000030 = 0x00000000;
    *(volatile unsigned int *)0x60000034 = 0x03020100;
    *(volatile unsigned int *)0x60000038 = 0x07060504;
    *(volatile unsigned int *)0x6000003c = 0x0b0a0908;
    *(volatile unsigned int *)0x60000040 = 0x0f0e0d0c;
    *(volatile unsigned int *)0x60000044 = 0x16157e2b;
    *(volatile unsigned int *)0x60000048 = 0xa6d2ae28;
    *(volatile unsigned int *)0x6000004c = 0x8815f7ab;
    *(volatile unsigned int *)0x60000050 = 0x3c4fcf09;
    *(volatile unsigned int *)0x60000054 = 0x00000000;
    *(volatile unsigned int *)0x60000058 = 0x00000000;
    *(volatile unsigned int *)0x6000005c = 0x00000000;
    *(volatile unsigned int *)0x60000060 = 0x00000000;

    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;

    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }


    //Check output token status
    r_output_token_data = *((volatile unsigned int *)0x60000000);

    if (r_output_token_data == c_output_token_id) {
        *((volatile unsigned int *)result_addr) = w_output_token_pass_data;
    } else {
        *((volatile unsigned int *)result_addr) = w_output_token_fail_data;
        ret = false;
    }

    //Check block 64 bytes output encrypted data in SRAM
    r_1_data = *((volatile unsigned int *)(sram_addr_out + 0));
    r_2_data = *((volatile unsigned int *)(sram_addr_out + 4));
    r_3_data = *((volatile unsigned int *)(sram_addr_out + 8));
    r_4_data = *((volatile unsigned int *)(sram_addr_out + 12));
    r_5_data = *((volatile unsigned int *)(sram_addr_out + 16));
    r_6_data = *((volatile unsigned int *)(sram_addr_out + 20));
    r_7_data = *((volatile unsigned int *)(sram_addr_out + 24));
    r_8_data = *((volatile unsigned int *)(sram_addr_out + 28));
    r_9_data = *((volatile unsigned int *)(sram_addr_out + 32));
    r_10_data = *((volatile unsigned int *)(sram_addr_out + 36));
    r_11_data = *((volatile unsigned int *)(sram_addr_out + 40));
    r_12_data = *((volatile unsigned int *)(sram_addr_out + 44));
    r_13_data = *((volatile unsigned int *)(sram_addr_out + 48));
    r_14_data = *((volatile unsigned int *)(sram_addr_out + 52));
    r_15_data = *((volatile unsigned int *)(sram_addr_out + 56));
    r_16_data = *((volatile unsigned int *)(sram_addr_out + 60));

    if ((r_1_data == c_1_data)
        &&(r_2_data == c_2_data)
        &&(r_3_data == c_3_data)
        &&(r_4_data == c_4_data)
        &&(r_5_data == c_5_data)
        &&(r_6_data == c_6_data)
        &&(r_7_data == c_7_data)
        &&(r_8_data == c_8_data)
        &&(r_9_data == c_9_data)
        &&(r_10_data == c_10_data)
        &&(r_11_data == c_11_data)
        &&(r_12_data == c_12_data)
        &&(r_13_data == c_13_data)
        &&(r_14_data == c_14_data)
        &&(r_15_data == c_15_data)
        &&(r_16_data == c_16_data)
        ) {
        //compare pass
        *((volatile unsigned int *)result_addr) = w_output_token_pass_data;
    } else {
        //compare fail
        *((volatile unsigned int *)result_addr) = w_output_token_fail_data;
        ret = false;
    }

    //Check Token ID in output data's last word
    r_output_token_data = *((volatile unsigned int *)(sram_addr_out + 0x40));

    if (r_output_token_data == c_output_token_id) {
        *((volatile unsigned int *)result_addr) = w_output_token_pass_data;
    } else {
        *((volatile unsigned int *)result_addr) = w_output_token_fail_data;
        ret = false;
    }

    //Check 16 bytes in offset 0x0008
    r_1_data = *((volatile unsigned int *)0x60000008);
    r_2_data = *((volatile unsigned int *)0x6000000c);
    r_3_data = *((volatile unsigned int *)0x60000010);
    r_4_data = *((volatile unsigned int *)0x60000014);

    if ( (r_1_data == c2_1_data)
         &&(r_2_data == c2_2_data)
         &&(r_3_data == c2_3_data)
         &&(r_4_data == c2_4_data)
         ){
        *((volatile unsigned int *)result_addr) = w_output_token_pass_data;
    } else {
        *((volatile unsigned int *)result_addr) = w_output_token_fail_data;
        ret = false;
    }

    //Write
    *(volatile unsigned int *)0x60003f00 = 0x2;

    return ret;
}


bool itp_scrt_aes_cmac(void)
{
    unsigned int data                   = 0;
    unsigned int c_output_token_id      = 0x9d37;
    unsigned int r_output_token_data    = 0;
    unsigned int result_addr;

    unsigned int sram_addr_in;
    //unsigned int sram_addr_out;

    unsigned int r_1_data = 0;
    unsigned int r_2_data = 0;
    unsigned int r_3_data = 0;
    unsigned int r_4_data = 0;

    unsigned int c_1_data = 0x902199e1;
    unsigned int c_2_data = 0xd56e9f54;
    unsigned int c_3_data = 0x052c6a69;
    unsigned int c_4_data = 0x1054316c;

    unsigned int w_output_token_pass_data = 0xf0f0f0f0;
    unsigned int w_output_token_fail_data = 0x48484848;

    unsigned int w_1_data = 0x03009d37;
    unsigned int w_2_data = 0x00000000;
    unsigned int w_3_data = 0x00000040;
    unsigned int w_4_data = 0x00411700;
    unsigned int w_5_data = 0x00000000;
    unsigned int w_6_data = 0x00000040;
    unsigned int w_7_data = 0x00200008;
    unsigned int w_8_data = 0x00000000;
    unsigned int w_9_data = 0x00000000;
    unsigned int w_10_data = 0x00000000;
    unsigned int w_11_data = 0x00000000;
    unsigned int w_12_data = 0x00000000;
    unsigned int w_13_data = 0x00000000;
    unsigned int w_14_data = 0x00000000;
    unsigned int w_15_data = 0x00000000;
    unsigned int w_16_data = 0x00000000;
    unsigned int w_17_data = 0x00000000;
    unsigned int w_18_data = 0x00000000;
    unsigned int w_19_data = 0x00000000;
    unsigned int w_20_data = 0x00000000;
    unsigned int w_21_data = 0x00000000;
    unsigned int w_22_data = 0x00000000;
    unsigned int w_23_data = 0x00000000;
    unsigned int w_24_data = 0x00000000;
    unsigned int w_25_data = 0x00000000;
    unsigned int w_26_data = 0x00000000;
    unsigned int w_27_data = 0x00000000;
    unsigned int w_28_data = 0x00000000;
    unsigned int w_29_data = 0x10eb3d60;
    unsigned int w_30_data = 0xbe71ca15;
    unsigned int w_31_data = 0xf0ae732b;
    unsigned int w_32_data = 0x81777d85;
    unsigned int w_33_data = 0x072c351f;
    unsigned int w_34_data = 0xd708613b;
    unsigned int w_35_data = 0xa310982d;
    unsigned int w_36_data = 0xf4df1409;
    unsigned int w_37_data = 0x00000000;
    unsigned int w_38_data = 0x00000000;
    unsigned int w_39_data = 0x00000000;
    unsigned int w_40_data = 0x00000000;
    unsigned int w_41_data = 0x00000000;
    unsigned int w_42_data = 0x00000000;
    unsigned int w_43_data = 0x00000000;
    unsigned int w_44_data = 0x00000000;
    unsigned int w_45_data = 0x00000000;
    unsigned int w_46_data = 0x00000000;
    unsigned int w_47_data = 0x00000000;
    unsigned int w_48_data = 0x00000000;
    unsigned int w_49_data = 0x00000000;
    unsigned int w_50_data = 0x00000000;
    unsigned int w_51_data = 0x00000000;
    unsigned int w_52_data = 0x00000000;
    unsigned int w_53_data = 0x00000000;
    unsigned int w_54_data = 0x00000000;
    unsigned int w_55_data = 0x00000000;
    unsigned int w_56_data = 0x00000000;
    unsigned int w_57_data = 0x00000000;
    unsigned int w_58_data = 0x00000000;
    unsigned int w_59_data = 0x00000000;
    unsigned int w_60_data = 0x00000000;

    bool ret = true;

    sram_addr_in = 0x00411700;
    result_addr = 0x0040a800;


    //Mailbox 1
    *(volatile unsigned int *)0x60003f00 = 0x4;

    //Write Data to SRAM
    *((volatile unsigned int *)(sram_addr_in + 0)) = 0xe2bec16b;
    *((volatile unsigned int *)(sram_addr_in + 4)) = 0x969f402e;
    *((volatile unsigned int *)(sram_addr_in + 8)) = 0x117e3de9;
    *((volatile unsigned int *)(sram_addr_in + 12)) = 0x2a179373;
    *((volatile unsigned int *)(sram_addr_in + 16)) = 0x578a2dae;
    *((volatile unsigned int *)(sram_addr_in + 20)) = 0x9cac031e;
    *((volatile unsigned int *)(sram_addr_in + 24)) = 0xac6fb79e;
    *((volatile unsigned int *)(sram_addr_in + 28)) = 0x518eaf45;
    *((volatile unsigned int *)(sram_addr_in + 32)) = 0x461cc830;
    *((volatile unsigned int *)(sram_addr_in + 36)) = 0x11e45ca3;
    *((volatile unsigned int *)(sram_addr_in + 40)) = 0x19c1fbe5;
    *((volatile unsigned int *)(sram_addr_in + 44)) = 0xef520a1a;
    *((volatile unsigned int *)(sram_addr_in + 48)) = 0x45249ff6;
    *((volatile unsigned int *)(sram_addr_in + 52)) = 0x179b4fdf;
    *((volatile unsigned int *)(sram_addr_in + 56)) = 0x7b412bad;
    *((volatile unsigned int *)(sram_addr_in + 60)) = 0x10376ce6;

    //Polling -1
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Write Token to mailbox
    *(volatile unsigned int *)0x60000000 = w_1_data;
    *(volatile unsigned int *)0x60000004 = w_2_data;
    *(volatile unsigned int *)0x60000008 = w_3_data;
    *(volatile unsigned int *)0x6000000c = w_4_data;
    *(volatile unsigned int *)0x60000010 = w_5_data;
    *(volatile unsigned int *)0x60000014 = w_6_data;
    *(volatile unsigned int *)0x60000018 = w_7_data;
    *(volatile unsigned int *)0x6000001c = w_8_data;
    *(volatile unsigned int *)0x60000020 = w_9_data;
    *(volatile unsigned int *)0x60000024 = w_10_data;
    *(volatile unsigned int *)0x60000028 = w_11_data;
    *(volatile unsigned int *)0x6000002c = w_12_data;
    *(volatile unsigned int *)0x60000030 = w_13_data;
    *(volatile unsigned int *)0x60000034 = w_14_data;
    *(volatile unsigned int *)0x60000038 = w_15_data;
    *(volatile unsigned int *)0x6000003c = w_16_data;
    *(volatile unsigned int *)0x60000040 = w_17_data;
    *(volatile unsigned int *)0x60000044 = w_18_data;
    *(volatile unsigned int *)0x60000048 = w_19_data;
    *(volatile unsigned int *)0x6000004c = w_20_data;
    *(volatile unsigned int *)0x60000050 = w_21_data;
    *(volatile unsigned int *)0x60000054 = w_22_data;
    *(volatile unsigned int *)0x60000058 = w_23_data;
    *(volatile unsigned int *)0x6000005c = w_24_data;
    *(volatile unsigned int *)0x60000060 = w_25_data;
    *(volatile unsigned int *)0x60000064 = w_26_data;
    *(volatile unsigned int *)0x60000068 = w_27_data;
    *(volatile unsigned int *)0x6000006c = w_28_data;
    *(volatile unsigned int *)0x60000070 = w_29_data;
    *(volatile unsigned int *)0x60000074 = w_30_data;
    *(volatile unsigned int *)0x60000078 = w_31_data;
    *(volatile unsigned int *)0x6000007c = w_32_data;
    *(volatile unsigned int *)0x60000080 = w_33_data;
    *(volatile unsigned int *)0x60000084 = w_34_data;
    *(volatile unsigned int *)0x60000088 = w_35_data;
    *(volatile unsigned int *)0x6000008c = w_36_data;
    *(volatile unsigned int *)0x60000090 = w_37_data;
    *(volatile unsigned int *)0x60000094 = w_38_data;
    *(volatile unsigned int *)0x60000098 = w_39_data;
    *(volatile unsigned int *)0x6000009c = w_40_data;
    *(volatile unsigned int *)0x600000a0 = w_41_data;
    *(volatile unsigned int *)0x600000a4 = w_42_data;
    *(volatile unsigned int *)0x600000a8 = w_43_data;
    *(volatile unsigned int *)0x600000ac = w_44_data;
    *(volatile unsigned int *)0x600000b0 = w_45_data;
    *(volatile unsigned int *)0x600000b4 = w_46_data;
    *(volatile unsigned int *)0x600000b8 = w_47_data;
    *(volatile unsigned int *)0x600000bc = w_48_data;
    *(volatile unsigned int *)0x600000c0 = w_49_data;
    *(volatile unsigned int *)0x600000c4 = w_50_data;
    *(volatile unsigned int *)0x600000c8 = w_51_data;
    *(volatile unsigned int *)0x600000cc = w_52_data;
    *(volatile unsigned int *)0x600000d0 = w_53_data;
    *(volatile unsigned int *)0x600000d4 = w_54_data;
    *(volatile unsigned int *)0x600000d8 = w_55_data;
    *(volatile unsigned int *)0x600000dc = w_56_data;
    *(volatile unsigned int *)0x600000e0 = w_57_data;
    *(volatile unsigned int *)0x600000e4 = w_58_data;
    *(volatile unsigned int *)0x600000e8 = w_59_data;
    *(volatile unsigned int *)0x600000ec = w_60_data;

    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;

    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Check output token status
    r_output_token_data = *((volatile unsigned int *)0x60000000);
    if (r_output_token_data == c_output_token_id) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
        ret = false;
    }

    //Check 16 bytes in offset 0x0008
    r_1_data = *((volatile unsigned int *)0x60000008);
    r_2_data = *((volatile unsigned int *)0x6000000c);
    r_3_data = *((volatile unsigned int *)0x60000010);
    r_4_data = *((volatile unsigned int *)0x60000014);

    if ((r_1_data == c_1_data)
        &&(r_2_data == c_2_data)
        &&(r_3_data == c_3_data)
        &&(r_4_data == c_4_data)
        ){
       *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
       *(volatile unsigned int *)result_addr = w_output_token_fail_data;
       ret = false;
    }

    //Write
    *(volatile unsigned int *)0x60003f00 = 0x2;

    return ret;
}


bool itp_scrt_hash(void)
{
    unsigned int data                   = 0;

    unsigned int sram_addr_in;
    unsigned int sram_addr_in_2;
    unsigned int result_addr;

    unsigned int c_output_token_id_1      = 0x6c12;
    unsigned int c_output_token_id_2      = 0x900c;
    unsigned int r_output_token_data    = 0;

    unsigned int r_1_data = 0;
    unsigned int r_2_data = 0;
    unsigned int r_3_data = 0;
    unsigned int r_4_data = 0;
    unsigned int r_5_data = 0;
    unsigned int r_6_data = 0;
    unsigned int r_7_data = 0;
    unsigned int r_8_data = 0;

    unsigned int c_1_data = 0x942fc50c;
    unsigned int c_2_data = 0xb8c94ece;
    unsigned int c_3_data = 0x8ca4dbc3;
    unsigned int c_4_data = 0x91456373;
    unsigned int c_5_data = 0xa862fdc8;

    unsigned int c2_1_data = 0xdd5f05b5;
    unsigned int c2_2_data = 0xa4202940;
    unsigned int c2_3_data = 0x0c3d0841;
    unsigned int c2_4_data = 0x1ec2ad45;
    unsigned int c2_5_data = 0xc7e481f6;
    unsigned int c2_6_data = 0x6fbd5835;
    unsigned int c2_7_data = 0x79b9fdb1;
    unsigned int c2_8_data = 0xc685e183;

    unsigned int w_output_token_pass_data = 0xf0f0f0f0;
    unsigned int w_output_token_fail_data = 0x48484848;

    unsigned int w_1_data = 0x83945d41;
    unsigned int w_2_data = 0x63852dae;
    unsigned int w_3_data = 0x5d57ba9a;
    unsigned int w_4_data = 0xbea0d38f;
    unsigned int w_5_data = 0x26a195b8;
    unsigned int w_6_data = 0x0010ba9f;
    unsigned int w_7_data = 0x9006d8b6;
    unsigned int w_8_data = 0x6562e3b5;
    unsigned int w_9_data = 0x7605a994;
    unsigned int w_10_data = 0x64c7034d;
    unsigned int w_11_data = 0x1e05b593;
    unsigned int w_12_data = 0xfd0064e0;
    unsigned int w_13_data = 0x8e1454d0;
    unsigned int w_14_data = 0x068f1445;
    unsigned int w_15_data = 0x703b8fc7;
    unsigned int w_16_data = 0x60430c0d;
    unsigned int w_17_data = 0x4ec9a367;
    unsigned int w_18_data = 0xaa7b7638;
    unsigned int w_19_data = 0x0fbf1866;
    unsigned int w_20_data = 0x3ad60eba;
    unsigned int w_21_data = 0x4024c819;
    unsigned int w_22_data = 0x035cdde3;
    unsigned int w_23_data = 0xab776f3c;
    unsigned int w_24_data = 0x991f6271;
    unsigned int w_25_data = 0xc06853c9;
    unsigned int w_26_data = 0x582bc2d2;
    unsigned int w_27_data = 0xaddbfbfa;
    unsigned int w_28_data = 0xcea96818;
    unsigned int w_29_data = 0x3d07c8a2;
    unsigned int w_30_data = 0xd89318cf;
    unsigned int w_31_data = 0xe7f69976;
    unsigned int w_32_data = 0x49a82102;

    bool ret = true;


    sram_addr_in = ITP_SCRT_SRAM_BASE_ADDR;
    result_addr = (sram_addr_in + 0x1000);
    sram_addr_in_2 = (sram_addr_in + 0x2000);


    //Mailbox 1
    *(volatile unsigned int *)0x60003f00 = 0x4;

    //Mailbox 2
    *(volatile unsigned int *)0x60003f00 = 0x40;

    //Mailbox 3
    *(volatile unsigned int *)0x60003f00 = 0x400;

    //Mailbox 4
    *(volatile unsigned int *)0x60003f00 = 0x4000;

    //Mailbox 1 again
    *(volatile unsigned int *)0x60003f00 = 0x4;

    //Write Data to SRAM
    *((volatile unsigned int *)(sram_addr_in + 0)) = w_1_data;
    *((volatile unsigned int *)(sram_addr_in + 4)) = w_2_data;
    *((volatile unsigned int *)(sram_addr_in + 8)) = w_3_data;
    *((volatile unsigned int *)(sram_addr_in + 12)) = w_4_data;
    *((volatile unsigned int *)(sram_addr_in + 16)) = w_5_data;
    *((volatile unsigned int *)(sram_addr_in + 20)) = w_6_data;
    *((volatile unsigned int *)(sram_addr_in + 24)) = w_7_data;
    *((volatile unsigned int *)(sram_addr_in + 28)) = w_8_data;
    *((volatile unsigned int *)(sram_addr_in + 32)) = w_9_data;
    *((volatile unsigned int *)(sram_addr_in + 36)) = w_10_data;
    *((volatile unsigned int *)(sram_addr_in + 40)) = w_11_data;
    *((volatile unsigned int *)(sram_addr_in + 44)) = w_12_data;
    *((volatile unsigned int *)(sram_addr_in + 48)) = w_13_data;
    *((volatile unsigned int *)(sram_addr_in + 52)) = w_14_data;
    *((volatile unsigned int *)(sram_addr_in + 56)) = w_15_data;
    *((volatile unsigned int *)(sram_addr_in + 60)) = w_16_data;
    *((volatile unsigned int *)(sram_addr_in + 64)) = w_17_data;
    *((volatile unsigned int *)(sram_addr_in + 68)) = w_18_data;
    *((volatile unsigned int *)(sram_addr_in + 72)) = w_19_data;
    *((volatile unsigned int *)(sram_addr_in + 76)) = w_20_data;
    *((volatile unsigned int *)(sram_addr_in + 80)) = w_21_data;
    *((volatile unsigned int *)(sram_addr_in + 84)) = w_22_data;
    *((volatile unsigned int *)(sram_addr_in + 88)) = w_23_data;
    *((volatile unsigned int *)(sram_addr_in + 92)) = w_24_data;
    *((volatile unsigned int *)(sram_addr_in + 96)) = w_25_data;
    *((volatile unsigned int *)(sram_addr_in + 100)) = w_26_data;
    *((volatile unsigned int *)(sram_addr_in + 104)) = w_27_data;
    *((volatile unsigned int *)(sram_addr_in + 108)) = w_28_data;
    *((volatile unsigned int *)(sram_addr_in + 112)) = w_29_data;
    *((volatile unsigned int *)(sram_addr_in + 116)) = w_30_data;
    *((volatile unsigned int *)(sram_addr_in + 120)) = w_31_data;
    *((volatile unsigned int *)(sram_addr_in + 124)) = w_32_data;

    //Polling -1
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Write Token
    *(volatile unsigned int *)0x60000000 = 0x02006c12;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = 0x00000080;
    *(volatile unsigned int *)0x6000000c = sram_addr_in; //0x0040a000;
    *(volatile unsigned int *)0x60000010 = 0x00000000;
    *(volatile unsigned int *)0x60000014 = 0x00000080;
    *(volatile unsigned int *)0x60000018 = 0x00000001;
    *(volatile unsigned int *)0x6000001c = 0x00000000;
    *(volatile unsigned int *)0x60000020 = 0x00000000;
    *(volatile unsigned int *)0x60000024 = 0x00000000;
    *(volatile unsigned int *)0x60000028 = 0x00000000;
    *(volatile unsigned int *)0x6000002c = 0x00000000;
    *(volatile unsigned int *)0x60000030 = 0x00000000;
    *(volatile unsigned int *)0x60000034 = 0x00000000;
    *(volatile unsigned int *)0x60000038 = 0x00000000;
    *(volatile unsigned int *)0x6000003c = 0x00000000;
    *(volatile unsigned int *)0x60000040 = 0x00000000;
    *(volatile unsigned int *)0x60000044 = 0x00000000;
    *(volatile unsigned int *)0x60000048 = 0x00000000;
    *(volatile unsigned int *)0x6000004c = 0x00000000;
    *(volatile unsigned int *)0x60000050 = 0x00000000;
    *(volatile unsigned int *)0x60000054 = 0x00000000;
    *(volatile unsigned int *)0x60000058 = 0x00000000;
    *(volatile unsigned int *)0x6000005c = 0x00000000;
    *(volatile unsigned int *)0x60000060 = 0x00000080;
    *(volatile unsigned int *)0x60000064 = 0x00000000;
    *(volatile unsigned int *)0x60000068 = 0x00000000;
    *(volatile unsigned int *)0x6000006c = 0x00000000;

    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;

    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Check output token 1 status
    r_output_token_data = *((volatile unsigned int *)0x60000000);
    if (r_output_token_data == c_output_token_id_1) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
        ret = false;
    }


    //Check 20 bytes in offset 0x0008
    r_1_data = *((volatile unsigned int *)0x60000008);
    r_2_data = *((volatile unsigned int *)0x6000000c);
    r_3_data = *((volatile unsigned int *)0x60000010);
    r_4_data = *((volatile unsigned int *)0x60000014);
    r_5_data = *((volatile unsigned int *)0x60000018);

    if ((r_1_data == c_1_data)
        &&(r_2_data == c_2_data)
        &&(r_3_data == c_3_data)
        &&(r_4_data == c_4_data)
        &&(r_5_data == c_5_data))
    {
       *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
       *(volatile unsigned int *)result_addr = w_output_token_fail_data;
       ret = false;
    }

    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x2;

    //Write 132 bytes in SRAM
    *((volatile unsigned int *)(sram_addr_in_2 + 0)) = 0x5e7f6915;
    *((volatile unsigned int *)(sram_addr_in_2 + 4)) = 0x291a47b8;
    *((volatile unsigned int *)(sram_addr_in_2 + 8)) = 0x02b1944e;
    *((volatile unsigned int *)(sram_addr_in_2 + 12)) = 0x6c87ce8d;
    *((volatile unsigned int *)(sram_addr_in_2 + 16)) = 0x9ef0edf4;
    *((volatile unsigned int *)(sram_addr_in_2 + 20)) = 0xbded711e;
    *((volatile unsigned int *)(sram_addr_in_2 + 24)) = 0xa34ddc8c;
    *((volatile unsigned int *)(sram_addr_in_2 + 28)) = 0x679bb9b4;
    *((volatile unsigned int *)(sram_addr_in_2 + 32)) = 0xa0425368;
    *((volatile unsigned int *)(sram_addr_in_2 + 36)) = 0x57925bad;
    *((volatile unsigned int *)(sram_addr_in_2 + 40)) = 0xb2c0f833;
    *((volatile unsigned int *)(sram_addr_in_2 + 44)) = 0x815fac01;
    *((volatile unsigned int *)(sram_addr_in_2 + 48)) = 0xb4263fee;
    *((volatile unsigned int *)(sram_addr_in_2 + 52)) = 0xe2661688;
    *((volatile unsigned int *)(sram_addr_in_2 + 56)) = 0xdd7fe265;
    *((volatile unsigned int *)(sram_addr_in_2 + 60)) = 0xe546c4b3;
    *((volatile unsigned int *)(sram_addr_in_2 + 64)) = 0xd8d3e9fb;
    *((volatile unsigned int *)(sram_addr_in_2 + 68)) = 0x92fabd1c;
    *((volatile unsigned int *)(sram_addr_in_2 + 72)) = 0x99b75d2d;
    *((volatile unsigned int *)(sram_addr_in_2 + 76)) = 0xc3128a2a;
    *((volatile unsigned int *)(sram_addr_in_2 + 80)) = 0xff4ad309;
    *((volatile unsigned int *)(sram_addr_in_2 + 84)) = 0x8a97fee9;
    *((volatile unsigned int *)(sram_addr_in_2 + 88)) = 0xdccb17c2;
    *((volatile unsigned int *)(sram_addr_in_2 + 92)) = 0xbb676381;
    *((volatile unsigned int *)(sram_addr_in_2 + 96)) = 0x5cd43686;
    *((volatile unsigned int *)(sram_addr_in_2 + 100)) = 0xb080a406;
    *((volatile unsigned int *)(sram_addr_in_2 + 104)) = 0x09629f86;
    *((volatile unsigned int *)(sram_addr_in_2 + 108)) = 0x76854a3c;
    *((volatile unsigned int *)(sram_addr_in_2 + 112)) = 0x4a691d69;
    *((volatile unsigned int *)(sram_addr_in_2 + 116)) = 0xce01c8d6;
    *((volatile unsigned int *)(sram_addr_in_2 + 120)) = 0xa48967c8;
    *((volatile unsigned int *)(sram_addr_in_2 + 124)) = 0xe0e7c910;
    *((volatile unsigned int *)(sram_addr_in_2 + 128)) = 0x00000002;

    //Polling -1
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Write Token
    *(volatile unsigned int *)0x60000000 = 0x0200900c;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = 0x00000081;
    *(volatile unsigned int *)0x6000000c = sram_addr_in_2; //0x0040b000;
    *(volatile unsigned int *)0x60000010 = 0x00000000;
    *(volatile unsigned int *)0x60000014 = 0x00000084;
    *(volatile unsigned int *)0x60000018 = 0x00000003;
    *(volatile unsigned int *)0x6000001c = 0x00000000;
    *(volatile unsigned int *)0x60000020 = 0x00000000;
    *(volatile unsigned int *)0x60000024 = 0x00000000;
    *(volatile unsigned int *)0x60000028 = 0x00000000;
    *(volatile unsigned int *)0x6000002c = 0x00000000;
    *(volatile unsigned int *)0x60000030 = 0x00000000;
    *(volatile unsigned int *)0x60000034 = 0x00000000;
    *(volatile unsigned int *)0x60000038 = 0x00000000;
    *(volatile unsigned int *)0x6000003c = 0x00000000;
    *(volatile unsigned int *)0x60000040 = 0x00000000;
    *(volatile unsigned int *)0x60000044 = 0x00000000;
    *(volatile unsigned int *)0x60000048 = 0x00000000;
    *(volatile unsigned int *)0x6000004c = 0x00000000;
    *(volatile unsigned int *)0x60000050 = 0x00000000;
    *(volatile unsigned int *)0x60000054 = 0x00000000;
    *(volatile unsigned int *)0x60000058 = 0x00000000;
    *(volatile unsigned int *)0x6000005c = 0x00000000;
    *(volatile unsigned int *)0x60000060 = 0x00000081;
    *(volatile unsigned int *)0x60000064 = 0x00000000;
    *(volatile unsigned int *)0x60000068 = 0x00000000;
    *(volatile unsigned int *)0x6000006c = 0x00000000;

    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;

    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Check output token 2 status
    r_output_token_data = *((volatile unsigned int *)0x60000000);
    if (r_output_token_data == c_output_token_id_2) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
        ret = false;
    }

    //Check 32 bytes in offset 0x0008
    r_1_data = *((volatile unsigned int *)0x60000008);
    r_2_data = *((volatile unsigned int *)0x6000000c);
    r_3_data = *((volatile unsigned int *)0x60000010);
    r_4_data = *((volatile unsigned int *)0x60000014);
    r_5_data = *((volatile unsigned int *)0x60000018);
    r_6_data = *((volatile unsigned int *)0x6000001c);
    r_7_data = *((volatile unsigned int *)0x60000020);
    r_8_data = *((volatile unsigned int *)0x60000024);
    if (  (r_1_data == c2_1_data)
        &&(r_2_data == c2_2_data)
        &&(r_3_data == c2_3_data)
        &&(r_4_data == c2_4_data)
        &&(r_5_data == c2_5_data)
        &&(r_6_data == c2_6_data)
        &&(r_7_data == c2_7_data)
        &&(r_8_data == c2_8_data))
    {
       *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
       *(volatile unsigned int *)result_addr = w_output_token_fail_data;
       ret = false;
    }

    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x2;

    return ret;
}


bool itp_scrt_trng(void) //not test yet
{
    unsigned int c_output_token_id_1    = 0xc8a7;
    unsigned int c_output_token_id_2    = 0x4da5;
    unsigned int r_output_token_data    = 0;
    unsigned int data                   = 0;
    unsigned int result_addr            = 0x0040a800;
    unsigned int w_output_token_pass_data = 0xf0f0f0f0;
    unsigned int w_output_token_fail_data = 0x48484848;
    unsigned int r_1_data = 0;
    unsigned int r_2_data = 0;
    unsigned int r_3_data = 0;
    unsigned int r_4_data = 0;
    bool ret = true;

    //Mailbox 1
    *(volatile unsigned int *)0x60003f00 = 0x4;

    //Polling -1
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
        delay(5);
    }


    //Write Token - Configure Random Number
    *(volatile unsigned int *)0x60000000 = 0x1402c8a7;
    *(volatile unsigned int *)0x60000004 = 0x4f5a3647;
    *(volatile unsigned int *)0x60000008 = 0x00000003;
    *(volatile unsigned int *)0x6000000c = 0x00030801;


    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;


    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
        delay(5);
    }


    //Check output token 1 status
    r_output_token_data = *((volatile unsigned int *)0x60000000);
    if (r_output_token_data == c_output_token_id_1) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        ret = false;
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }


    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x2;




    //Write Data to SRAM
    *(volatile unsigned int *)0x0040a000 = 0xcccccccc;
    *(volatile unsigned int *)0x0040a004 = 0xcccccccc;
    *(volatile unsigned int *)0x0040a008 = 0xcccccccc;
    *(volatile unsigned int *)0x0040a00c = 0xcccccccc;


    //Polling -1
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
        delay(5);
    }

    //Write Token - Get Random Number
    *(volatile unsigned int *)0x60000000 = 0x04044da5;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = 0x00000010;
    *(volatile unsigned int *)0x6000000c = 0x0040a000;
    *(volatile unsigned int *)0x60000010 = 0x00000000;

    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;

    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
        delay(5);
    }


    //Check output token 1 status
    r_output_token_data = *((volatile unsigned int *)0x60000000);
    if (r_output_token_data == c_output_token_id_2) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        ret = false;
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }


    //Read Block (4 words)
    r_1_data = *((volatile unsigned int *)0x0040a000);
    r_2_data = *((volatile unsigned int *)0x0040a004);
    r_3_data = *((volatile unsigned int *)0x0040a008);
    r_4_data = *((volatile unsigned int *)0x0040a00c);


    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x2;

    return ret;
}

//copy from fpga_test_aes_ccm_3
bool itp_scrt_aes_ccm(void)               //It's working
                                               //Success: If we set the output tag length of value 0x10, it pass; the output cipher and tag are correct with software api
                                               //Fail: If we set the output tag length of value 0x04, it fail; security IP reports "error"
                                               //tage length: 4=>fail  8=>pass 12=>pass 16=>pass
                                               //note: important
                                               //  if the output tag length is configed to 0x4, fail
                                               //  if the output tag length is configed to 0x8, the output tag data is: 0x6072bed9 0xb14aab8c  => correct
                                               //  if the output tag length is configed to 0x10, the output tag data is: 0x173d1fcc 0xe8bcf82a 0xa299f3a5 0x64548338
{
    unsigned int scrt_ip_addr  = 0x60000000;
    unsigned int scrt_nl_addr  = 0x6E000000;
    unsigned int ram_base_addr = 0x00400000;

    unsigned int result_addr = 0x00403200;
    unsigned int flag_addr   = 0x00403300;

    unsigned int w_output_token_pass_data = 0xf0f0f0f0; //Pass Value
    unsigned int w_output_token_fail_data = 0x48484848; //Fail Value

    unsigned int r_output_token_data;
    unsigned int c_output_token_id = 0xd82c;            //Token ID

    unsigned int data = 0;

    unsigned int r_1_data = 0;
    unsigned int r_2_data = 0;
    unsigned int r_3_data = 0;
    unsigned int r_4_data = 0;
    unsigned int r_5_data = 0;

    unsigned int addr_input_data                = 0x00401000;       //Input Data Address
    unsigned int addr_associated_data           = 0x00402000;       //Associated Data Address
    unsigned int addr_output_data_encryption    = 0x00401500;       //Encryption: Output Data Address
    unsigned int addr_output_data_decryption    = 0x00402500;       //Decryption: Output Data Address

    unsigned int len_input_data = 0x10;             //Input Data Length
    unsigned int len_output_data = 0x14;            //Output Data Length
    unsigned int len_data_transfer = 0x1;           //The length of the data transfer
    unsigned int len_asso_data = 0x1;               //The length of the associated data

    unsigned int plain_text_1 = 0x6;            //plain text data word 1
    unsigned int plain_text_2 = 0x0;            //plain text data word 2
    unsigned int plain_text_3 = 0x0;            //plain text data word 3
    unsigned int plain_text_4 = 0x0;            //plain text data word 4

    unsigned int key_1 = 0x521bad99;            //key word 1
    unsigned int key_2 = 0x3e7ea326;            //key word 2
    unsigned int key_3 = 0x8e3b8e05;            //key word 3
    unsigned int key_4 = 0x66c6c227;            //key word 4

    unsigned int asso_data_1 = 0x3;             //associated data word 1

    unsigned int nonce_1 = 0x00000000;          //nonce word 1
    unsigned int nonce_2 = 0xdcab2480;          //nonce word 2
    unsigned int nonce_3 = 0xafbabeba;          //nonce word 3
    unsigned int nonce_4 = 0xde;                //nonce word 4

    unsigned int output_tag_1 = 0;                  //output tag word 1
    unsigned int output_tag_2 = 0;                  //output tag word 2
    unsigned int output_tag_3 = 0;                  //output tag word 3
    unsigned int output_tag_4 = 0;                  //output tag word 4

    unsigned int input_verified_tag_1 = 0;          //output tag word 1
    unsigned int input_verified_tag_2 = 0;          //output tag word 2
    unsigned int input_verified_tag_3 = 0;          //output tag word 3
    unsigned int input_verified_tag_4 = 0;          //output tag word 4

    unsigned int cypher_1_data = 0;
    unsigned int cypher_2_data = 0;
    unsigned int cypher_3_data = 0;
    unsigned int cypher_4_data = 0;

    bool ret = true;


    //Mailbox 1
    *(volatile unsigned int *)0x60003f00 = 0x4;
    //*(volatile unsigned int *)0x60003f00 = 0x40;
    //*(volatile unsigned int *)0x60003f00 = 0x400;
    //*(volatile unsigned int *)0x60003f00 = 0x4000;


//==============================
//Encrypt
//==============================

    //Input Data
    *(volatile unsigned int *)(addr_input_data + 0x0) = plain_text_1;   //address:0x00401000  plaintextdata:0x23222120
    *(volatile unsigned int *)(addr_input_data + 0x4) = plain_text_2;   //address:0x00401004  plaintextdata:0x0
    *(volatile unsigned int *)(addr_input_data + 0x8) = plain_text_3;   //address:0x00401008  plaintextdata:0x0
    *(volatile unsigned int *)(addr_input_data + 0xc) = plain_text_4;	//address:0x00401010  plaintextdata:0x0

    //Associated Data
    *(volatile unsigned int *)addr_associated_data = asso_data_1; //address:0x00402000 aData: 0x03020100


    //Polling -1
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
        delay(5);
    }

    //Burst write 37 words
    *(volatile unsigned int *)0x60000000 = 0x0104d82c;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = len_data_transfer;               //The length of the data transfer  //0x1
    *(volatile unsigned int *)0x6000000c = addr_input_data;                 //Input Data                     //0x00401000
    *(volatile unsigned int *)0x60000010 = 0x00000000;                      //Input Data
    *(volatile unsigned int *)0x60000014 = len_input_data;                  //Input Data Length         //0x10
    *(volatile unsigned int *)0x60000018 = addr_output_data_encryption;     //Output Data                  //0x00401500
    *(volatile unsigned int *)0x6000001c = 0x00000000;                      //Output Data
    *(volatile unsigned int *)0x60000020 = len_output_data;                 //Output Data Length      //0x14
    *(volatile unsigned int *)0x60000024 = addr_associated_data;            //Associated Data            //0x00402000
    *(volatile unsigned int *)0x60000028 = 0x00000000;                      //Associated Data
    *(volatile unsigned int *)0x6000002c = 0x10d18050;//0x10d18050;                    //Nonce length              => word 11[23:20]       => 13  => 0xd
                                                                            //Key length             => word 11[19:16]    => 1 (1 means 128 bits key)
                                                                            //Tag length             => word 11[28:24]    5 bits => 0x10  => 16
                                                                            //Encrypt or Decrypt  => word11[15]         => 1 (1 means encrypt)
    *(volatile unsigned int *)0x60000030 = 0x00000000;
    *(volatile unsigned int *)0x60000034 = 0x00000000;
    *(volatile unsigned int *)0x60000038 = 0x00000000;
    *(volatile unsigned int *)0x6000003c = 0x00000000;
    *(volatile unsigned int *)0x60000040 = 0x00000000;
    *(volatile unsigned int *)0x60000044 = key_1;                           //key word 1
    *(volatile unsigned int *)0x60000048 = key_2;                           //key word 2
    *(volatile unsigned int *)0x6000004c = key_3;                           //key word 3
    *(volatile unsigned int *)0x60000050 = key_4;                           //key word 4
    *(volatile unsigned int *)0x60000054 = 0x00000000;                      //key
    *(volatile unsigned int *)0x60000058 = 0x00000000;                      //key
    *(volatile unsigned int *)0x6000005c = 0x00000000;                      //key
    *(volatile unsigned int *)0x60000060 = 0x00000000;                      //key
    *(volatile unsigned int *)0x60000064 = len_asso_data;                   //Associated Data Length   0x1
    *(volatile unsigned int *)0x60000068 = 0x00000000;
    *(volatile unsigned int *)0x6000006c = 0x00000000;
    *(volatile unsigned int *)0x60000070 = 0x00000000;
    *(volatile unsigned int *)0x60000074 = nonce_1;                         //nonce word 1
    *(volatile unsigned int *)0x60000078 = nonce_2;                         //nonce word 2
    *(volatile unsigned int *)0x6000007c = nonce_3;                         //nonce word 3
    *(volatile unsigned int *)0x60000080 = nonce_4;                         //nonce word 4
    *(volatile unsigned int *)0x60000084 = 0x00000000;
    *(volatile unsigned int *)0x60000088 = 0x00000000;
    *(volatile unsigned int *)0x6000008c = 0x00000000;
    *(volatile unsigned int *)0x60000090 = 0x00000000;

    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x1;

    //Polling - 2
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
        delay(5);
    }

    //Check output token status
    r_output_token_data = *((volatile unsigned int *)0x60000000);
    if (r_output_token_data == c_output_token_id) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        ret = false;
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //We don't know what's the correct output data
#if 0
    //Check output data
    r_1_data = *((volatile unsigned int *)(addr_output_data));
    if (r_1_data == c_1_data)
    {
        //compare pass
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        //compare fail
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }
#endif

    //Get output cipher data from SRAM address
    cypher_1_data = *((volatile unsigned int *)(addr_output_data_encryption + 0x0)); //addr: 0x00401500
    cypher_2_data = *((volatile unsigned int *)(addr_output_data_encryption + 0x4)); //addr: 0x00401504
    cypher_3_data = *((volatile unsigned int *)(addr_output_data_encryption + 0x8)); //addr: 0x00401508
    cypher_4_data = *((volatile unsigned int *)(addr_output_data_encryption + 0xc)); //addr: 0x0040150c

    //Check Token ID in output data's last word
    r_output_token_data = *((volatile unsigned int *)(addr_output_data_encryption + len_input_data)); //0x00401500 + 0x10 = 0x00401510
    if (r_output_token_data == c_output_token_id) {
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        ret = false;
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }

    //We don't know what's the correct output data
#if 0
    //Check 4 words in offset 0x18
    r_1_data = *((volatile unsigned int *)0x60000018);
    r_2_data = *((volatile unsigned int *)0x6000001c);
    r_3_data = *((volatile unsigned int *)0x60000020);
    r_4_data = *((volatile unsigned int *)0x60000024);
    if ( (r_1_data == c2_1_data)
         &&(r_2_data == c2_2_data)
         &&(r_3_data == c2_3_data)
         &&(r_4_data == c2_4_data)
         ){
        *(volatile unsigned int *)result_addr = w_output_token_pass_data;
    } else {
        *(volatile unsigned int *)result_addr = w_output_token_fail_data;
    }
#endif

    //Get Output Tag from output token address
    output_tag_1 = *((volatile unsigned int *)0x60000018);
    output_tag_2 = *((volatile unsigned int *)0x6000001c);
    output_tag_3 = *((volatile unsigned int *)0x60000020);
    output_tag_4 = *((volatile unsigned int *)0x60000024);

    //Write
    *(volatile unsigned int *)0x60003f00 = 0x2;

    return ret;
}


//This api is for the purpose to do token test on a.Create Asset b.Load Asset c.TRNG d.Public Key Token
bool itp_scrt_ecdh_phase1(void) //PASS, it's working
{
    bool ret = true;
    unsigned int data;
    unsigned int asset_id_0;
    unsigned int asset_id_1;
    unsigned int asset_id_2;
    unsigned int asset_id_3;

    unsigned int data_sram_1[] = {
            0x0a802e59,
            0x5b33fc19,
            0x2bebd6bb,
            0xad0ef7ac,
            0x984057d8,
            0x68782c71,
            0x5b4f8291,
            0x038f40d6,
            0x470b55bd,
            0x495af43d,
            0x9ea2f25b,
            0xa66a63b7,
            0x3d553100,
            0x91e060a3,
            0x6bb3666a,
            0x42720ba9
    };

    unsigned int data_sram_2[] = {
            0xb73650ad,
            0x1987d200,
            0x47dfa61e,
            0x65da19fa,
            0xca734614,
            0xa68522dc,
            0x0447df58,
            0xf28eae0a,
            0xbf0ae8f0,
            0xe7b6a7b3,
            0x35895fa3,
            0x55260296,
            0x9e56b7ca,
            0x4afb290a,
            0x7a4b8c65,
            0x852d0c3d
    };

    unsigned int data_sram_3[] = {
            0x60a0ebe4,
            0x1510a3f9,
            0x64020fbf,
            0x957d7284,
            0xfd5e6cad,
            0x5b6ec642,
            0xe7b3dd10,
            0x001ead73
    };

    unsigned int data_sram_4[] = {
            0x906d1d4e,
            0x196aefec,
            0x7207bbc6,
            0xf6eb07af,
            0x5b1214b8,
            0xf9025907,
            0x35e412d1,
            0x1ff0ac34,
            0x9a065e01,
            0x581c2fdb,
            0x5cf721f5,
            0xf5ef4a79,
            0x1afe2fa2,
            0xf2e52894,
            0x00000000,
            0x00000000
    };

    unsigned int data_sram_5[] = {
            0x7115a36c,
            0x680ef25e,
            0x3452ba6a,
            0xcbf3784b,
            0xcbb08e46,
            0x6ec46672,
            0x24ef743f,
            0x00000000
    };

    unsigned int data_sram_6[] = {
            0x278ba1c4,
            0xc2ad78b4,
            0xfb47c33b,
            0x410ae21e,
            0x1a659486,
            0xe8c14840,
            0x4eca0998,
            0xeb3f8757,
            0x854a6a29,
            0x6988a092,
            0xc9c767a2,
            0x28a05f4a,
            0x239761b2,
            0x6a795aab,
            0xa651b17a,
            0x0dbe4c0f,
            0xe378069f,
            0xdd118602,
            0x01abbdaf,
            0xdfff9e0a,
            0xc03abab3,
            0x54751a49,
            0x6fe56b2b,
            0x2186b572,
            0x63ca71e9,
            0x323863ce,
            0x630c96f0,
            0x0e2f153b,
            0x9c9c2ca3,
            0xe04883b4,
            0x84a1755a,
            0xefeef4f4,
            0x37b47d94,
            0x00290b44,
            0xaec923a7,
            0x74eb65f8,
            0xcbe21a56,
            0x07537452,
            0x86355e0b,
            0xbcac3330,
            0x5841dc4f,
            0x12ef0bb6,
            0xcf0cb975,
            0x141e75fa
    };

    unsigned int data_sram_7[] = {
            0x070000c0,
            0xffffffff,
            0xffffffff,
            0xfffffffe,
            0xffffffff,
            0xffffffff,
            0xffffffff,
            0x070100c0,
            0xfffffffc,
            0xffffffff,
            0xfffffffe,
            0xffffffff,
            0xffffffff,
            0xffffffff,
            0x070200bf,
            0xc146b9b1,
            0xfeb8deec,
            0x72243049,
            0x0fa7e9ab,
            0xe59c80e7,
            0x64210519,
            0x070300c0,
            0xb4d22831,
            0x146bc9b1,
            0x99def836,
            0xffffffff,
            0xffffffff,
            0xffffffff,
            0x070400bd,
            0x82ff1012,
            0xf4ff0afd,
            0x43a18800,
            0x7cbf20eb,
            0xb03090f6,
            0x188da80e,
            0x070500bb,
            0x1e794811,
            0x73f977a1,
            0x6b24cdd5,
            0x631011ed,
            0xffc8da78,
            0x07192b95,
            0x07060001,
            0x00000001
    };

    //Token: TRNG configure
    unsigned int data_mb_1[] = {
            0x140019fb,
            0x4f5a3647,
            0x00000001,
            0x00030101
    };

    //Token: Create asset
    unsigned int data_mb_2[] = {
            0x1700428a,
            0x00000000,
            0x30000000,
            0x00000010,
            0x00000040,
            0x00000000,
            0x00000000
    };

    //Token: Create asset
    unsigned int data_mb_3[] = {
            0x1700b9bd,
            0x00000000,
            0x30000000,
            0x00000010,
            0x00000040,
            0x00000000,
            0x00000000
    };

    //Token: Load asset
    unsigned int data_mb_4[] = {
            0x27046d31,
            0x00000000,
            0x00015004,
            0x02000000,
            0x00000000,
            0x00000000,
            0x00000000,
            0x00000000,
            0x00000000,
            0x00000000
    };

    //Token: Create asset
    unsigned int data_mb_5[] = {
            0x1700c786,
            0x00000000,
            0x80000004,
            0x00001000,
            0x00000038,
            0x00000000,
            0x00000000
    };

    //Token: Create asset
    unsigned int data_mb_6[] = {
            0x17008cff,
            0x00000000,
            0x00000004,
            0x00001000,
            0x0000001c,
            0x00000000,
            0x00000000
    };

    //Token: Create asset
    unsigned int data_mb_7[] = {
            0x17009e65,
            0x00000000,
            0x00000000,
            0x00008000,
            0x000000b0,
            0x00000000,
            0x00000000
    };

    //Token: Load asset
    unsigned int data_mb_8[] = {
            0x27040a12,
            0x00000000,
            0x00015014,
            0x080000b0,
            0x0040a600,
            0x00000000,
            0x00000000,
            0x00000000,
            0x00000000,
            0x00000000
    };

    //Token: Public key token
    unsigned int data_mb_9[] = {
            0x1904d4f1,
            0x00000000,
            0x06060015,
            0x00000030,
            0x00015010,
            0x00015014,
            0x0001500c,
            0x0000002c,
            0x0040b000,
            0x00000000,
            0x00000000,
            0x00000000,
            0x00015004,
            0x7473754a,
            0x6d6f7320,
            0x73612065,
            0x69636f73,
            0x64657461,
            0x74616420,
            0x6f662061,
            0x656b2072,
            0x6f6c6279,
            0x65672062,
            0x6172656e,
            0x6e6f6974
    };


    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x4);
    //wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x40);
    //wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x400);
    //wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x4000);


    //===========================================================================  Block1
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }
    /*
     unsigned int data_mb_1[] = {
        0x140019fb,
        0x4f5a3647,
        0x00000001,
        0x00030101
    };
    */
    wrCoreBlock(EIP130_MAILBOX_IN_BASE, data_mb_1, sizeof(data_mb_1)/sizeof(data_mb_1[0]));
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x1);
    write_sram(0x0040a000, data_sram_1, sizeof(data_sram_1)/sizeof(data_sram_1[0]));
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }
    /*
    unsigned int data_mb_2[] = {
            0x1700428a,
            0x00000000,
            0x30000000,
            0x00000010,
            0x00000040,
            0x00000000,
            0x00000000
    };
    */
    //Create Asset
    wrCoreBlock(EIP130_MAILBOX_IN_BASE, data_mb_2, sizeof(data_mb_2)/sizeof(data_mb_2[0]));
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x1);
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }
    data = rdCore(EIP130_MAILBOX_OUT_BASE);  //it shoud be: 0x000019fb => test ok
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x2);
    printf("\r\n ECDH Phase 1 Block 1- data:%lx \r\n", data);
    if(data != 0x19fb) ret = false;
    //===========================================================================



    //===========================================================================  Block2
    write_sram(0x0040a100, data_sram_2, sizeof(data_sram_2)/sizeof(data_sram_2[0]));
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

    /*
    unsigned int data_mb_3[] = {
            0x1700b9bd,
            0x00000000,
            0x30000000,
            0x00000010,
            0x00000040,
            0x00000000,
            0x00000000
    };
    */
    //Create Asset
    wrCoreBlock(EIP130_MAILBOX_IN_BASE, data_mb_3, sizeof(data_mb_3)/sizeof(data_mb_3[0]));
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x1);
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }
    data = rdCore(EIP130_MAILBOX_OUT_BASE);    //it shoud be: 0x0000428a   => test ok
    asset_id_0 = rdCore(EIP130_MAILBOX_OUT_BASE + 4);//it shoud be: 0x00015004   => test ok
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x2);
    printf("\r\n ECDH Phase 1 Block 2- asset_id_0:%lx \r\n", asset_id_0);
    //if(asset_id_0 != 0x15004) ret = false;
    //===========================================================================





    //===========================================================================  Block3
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }
    data = rdCore(EIP130_MAILBOX_OUT_BASE);     //it shoud be: 0x0000b9bd  => test ok
    data = rdCore(EIP130_MAILBOX_OUT_BASE + 4); //it shoud be: 0x00015008  => test ok
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x2);
    printf("\r\n ECDH Phase 1 Block 3- data:%lx \r\n", data);
    //if(data != 0x15008) ret = false;
    //===========================================================================






    //===========================================================================  Block4
    write_sram(0x0040a200, data_sram_3, sizeof(data_sram_3)/sizeof(data_sram_3[0]));
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }


/*
        unsigned int data_mb_4[] = {
            0x27046d31,
            0x00000000,
            0x00015004,
            0x02000000,
            0x00000000,
            0x00000000,
            0x00000000,
            0x00000000,
            0x00000000,
            0x00000000
    };
*/
    //Load Asset
    data_mb_4[2] = asset_id_0;
    wrCoreBlock(EIP130_MAILBOX_IN_BASE, data_mb_4, sizeof(data_mb_4)/sizeof(data_mb_4[0]));
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x1);
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }
    data = rdCore(EIP130_MAILBOX_OUT_BASE);    //it shoud be: 0x00006D31  => test ok
    data = rdCore(EIP130_MAILBOX_OUT_BASE + 4);//it shoud be: 0x0  => test ok
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x2);
    printf("\r\n ECDH Phase 1 Block 4- data:%lx \r\n", data);
    if(data != 0x0) ret = false;
    //===========================================================================






    //===========================================================================  Block5
    write_sram(0x0040a300, data_sram_4, sizeof(data_sram_4)/sizeof(data_sram_4[0]));
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }


/*
    unsigned int data_mb_5[] = {
            0x1700c786,
            0x00000000,
            0x80000004,
            0x00001000,
            0x00000038,
            0x00000000,
            0x00000000
    };
*/
    //Create Asset
    wrCoreBlock(EIP130_MAILBOX_IN_BASE, data_mb_5, sizeof(data_mb_5)/sizeof(data_mb_5[0]));
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x1);
    write_sram(0x0040a400, data_sram_5, sizeof(data_sram_5)/sizeof(data_sram_5[0]));
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }


/*
    unsigned int data_mb_6[] = {
            0x17008cff,
            0x00000000,
            0x00000004,
            0x00001000,
            0x0000001c,
            0x00000000,
            0x00000000
    };
*/
    //Create Asset
    wrCoreBlock(EIP130_MAILBOX_IN_BASE, data_mb_6, sizeof(data_mb_6)/sizeof(data_mb_6[0]));
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x1);
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }
    data = rdCore(EIP130_MAILBOX_OUT_BASE);    //it shoud be: 0x0000c786  OK  => test ok
    asset_id_1 = rdCore(EIP130_MAILBOX_OUT_BASE + 4);//it shoud be: 0x0001500c  => test ok
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x2);
    printf("\r\n ECDH Phase 1 Block 5- asset_id_1:%lx \r\n", asset_id_1);
    //if(asset_id_1 != 0x1500c) ret = false;
    //===========================================================================






    //===========================================================================  Block6
    write_sram(0x0040a500, data_sram_6, sizeof(data_sram_6)/sizeof(data_sram_6[0]));
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

/*
    unsigned int data_mb_7[] = {
            0x17009e65,
            0x00000000,
            0x00000000,
            0x00008000,
            0x000000b0,
            0x00000000,
            0x00000000
    };
*/
    //Create Asset
    wrCoreBlock(EIP130_MAILBOX_IN_BASE, data_mb_7, sizeof(data_mb_7)/sizeof(data_mb_7[0]));
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x1);
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }
    data = rdCore(EIP130_MAILBOX_OUT_BASE);    //it shoud be: 0x00008cff  OK  => test ok
    asset_id_2 = rdCore(EIP130_MAILBOX_OUT_BASE + 4);//it shoud be: 0x00015010  => test ok
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x2);
    printf("\r\n ECDH Phase 1 Block 6- asset_id_2:%lx \r\n", asset_id_2);
    //if(asset_id_2 != 0x15010) ret = false;
    //===========================================================================





    //===========================================================================  Block7
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }
    data = rdCore(EIP130_MAILBOX_OUT_BASE);    //it shoud be: 0x00009e65  OK  => test ok
    asset_id_3 = rdCore(EIP130_MAILBOX_OUT_BASE + 4);//it shoud be: 0x00015014  => test ok
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x2);
    printf("\r\n ECDH Phase 1 Block 7- asset_id_3:%lx \r\n", asset_id_3);
    //if(asset_id_3 != 0x15014) ret = false;
    //===========================================================================






    //===========================================================================  Block8
    write_sram(0x0040a600, data_sram_7, sizeof(data_sram_7)/sizeof(data_sram_7[0]));
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

/*
    unsigned int data_mb_8[] = {
            0x27040a12,
            0x00000000,
            0x00015014,
            0x080000b0,
            0x0040a600,
            0x00000000,
            0x00000000,
            0x00000000,
            0x00000000,
            0x00000000
    };
*/
    //Load Asset
    data_mb_8[2] = asset_id_3;
    wrCoreBlock(EIP130_MAILBOX_IN_BASE, data_mb_8, sizeof(data_mb_8)/sizeof(data_mb_8[0]));
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x1);
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }
    data = rdCore(EIP130_MAILBOX_OUT_BASE);    //it shoud be: 0x0x00000a12  => test ok
    data = rdCore(EIP130_MAILBOX_OUT_BASE + 4);//it shoud be: 0x0  => test ok
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x2);
    printf("\r\n ECDH Phase 1 Block 8- data:%lx \r\n", data);
    if(data != 0x0) ret = false;
    //===========================================================================






    //===========================================================================  Block9
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

/*
    data_mb_9[25] = {
            0x1904d4f1,
            0x00000000,
            0x06060015,
            0x00000030,
            0x00015010, //asset_id_2
            0x00015014, //asset_id_3
            0x0001500c, //asset_id_1
            0x0000002c,
            0x0040b000,
            0x00000000,
            0x00000000,
            0x00000000,
            0x00015004,
            0x7473754a,
            0x6d6f7320,
            0x73612065,
            0x69636f73,
            0x64657461,
            0x74616420,
            0x6f662061,
            0x656b2072,
            0x6f6c6279,
            0x65672062,
            0x6172656e,
            0x6e6f6974
    };*/

    data_mb_9[0] = 0x1904d4f1;
    data_mb_9[1] = 0x00000000;
    data_mb_9[2] = 0x06060015;
    data_mb_9[3] = 0x00000030;
    data_mb_9[4] = asset_id_2; //modified
    data_mb_9[5] = asset_id_3; //modified
    data_mb_9[6] = asset_id_1; //modified
    data_mb_9[7] = 0x0000002c;
    data_mb_9[8] = 0x0040b000;
    data_mb_9[9] = 0x00000000;
    data_mb_9[10] = 0x00000000;
    data_mb_9[11] = 0x00000000;
    data_mb_9[12] = asset_id_0; //modified
    data_mb_9[13] = 0x7473754a;
    data_mb_9[14] = 0x6d6f7320;
    data_mb_9[15] = 0x73612065;
    data_mb_9[16] = 0x69636f73;
    data_mb_9[17] = 0x64657461;
    data_mb_9[18] = 0x74616420;
    data_mb_9[19] = 0x6f662061;
    data_mb_9[20] = 0x656b2072;
    data_mb_9[21] = 0x6f6c6279;
    data_mb_9[22] = 0x65672062;
    data_mb_9[23] = 0x6172656e;
    data_mb_9[24] = 0x6e6f6974;

    //Create PKCP Key Pair
    wrCoreBlock(EIP130_MAILBOX_IN_BASE, data_mb_9, sizeof(data_mb_9)/sizeof(data_mb_9[0]));
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x1);
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }
    data = rdCore(EIP130_MAILBOX_OUT_BASE);  //it shoud be: 0x0x0000d4f1  => test ok
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x2);
    printf("\r\n ECDH Phase 1 Block 9- data:%lx \r\n", data);
    if(data != 0xd4f1) ret = false;
    //===========================================================================


    return ret;
}


void itp_scrt_test(void)
{
    bool ret;
    ret = itp_scrt_scrt0_reg_access();
    printf("\r\n itp_scrt_scrt0_reg_access, test result:%s \r\n", (ret == 1) ? "Success" : "Fail");
    ret = itp_scrt_scrt1_reg_access();
    printf("\r\n itp_scrt_scrt1_reg_access, test result:%s \r\n", (ret == 1) ? "Success" : "Fail");
    ret = itp_scrt_sram_access();
    printf("\r\n itp_scrt_sram_access, test result:%s \r\n", (ret == 1) ? "Success" : "Fail");
    ret = itp_scrt_sram_burst();
    printf("\r\n itp_scrt_sram_burst, test result:%s \r\n", (ret == 1) ? "Success" : "Fail");
    ret = itp_scrt_scrt0_all_reg_access();
    printf("\r\n itp_scrt_scrt0_all_reg_access, test result:%s \r\n", (ret == 1) ? "Success" : "Fail");
    ret = itp_scrt_aes_64bytes();
    printf("\r\n itp_scrt_aes_64bytes, test result:%s \r\n", (ret == 1) ? "Success" : "Fail");
    ret =itp_scrt_aes_cmac();
    printf("\r\n itp_scrt_aes_cmac, test result:%s \r\n", (ret == 1) ? "Success" : "Fail");
    ret = itp_scrt_hash();
    printf("\r\n itp_scrt_hash, test result:%s \r\n", (ret == 1) ? "Success" : "Fail");
    ret = itp_scrt_trng();
    printf("\r\n itp_scrt_trng, test result:%s \r\n", (ret == 1) ? "Success" : "Fail");
    ret = itp_scrt_aes_ccm();
    printf("\r\n itp_scrt_aes_ccm, test result:%s \r\n", (ret == 1) ? "Success" : "Fail");
    ret = itp_scrt_ecdh_phase1();
    printf("\r\n itp_scrt_ecdh_phase1, test result:%s \r\n", (ret == 1) ? "Success" : "Fail");
}


