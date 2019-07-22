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
#include "basic_defs.h"


//Disable Non-ITP test codes
#if 0

 //Run the test on ROM
void test_onrom_level1_scrt0_reg_access(void);
void test_onrom_level1_scrt1_reg_access(void);
void test_onrom_level1_scrt0_scrt1_cross_reg_access(void);
void test_onrom_level1_sram_access(void);
void test_onrom_level2_scrt0_all_reg_access(void);
void test_onrom_level2_sram_block_write_read_compare(void);
void test_onrom_level3_scrt0_aes_64bytes_with_delay(void);
void test_onrom_level4_scrt0_aes_64bytes_no_delay(void);
void test_onrom_level5_scrt0_aes_64bytes_with_new_sram_address_400000(void);
void test_onrom_level5_scrt0_aes_2k(void);
void test_onrom_level5_scrt0_aes_64b_1mailbox_3tokens(void);


//Run the test on SRAM
void test_onsram_scrt_sram_burst(void);
void test_onsram_scrt_sram_burst_oneword(void);
void test_onsram_scrt_mailbox_link(void);
void test_onsram_scrt_do_aes_sixty_four_bytes(void);
void test_onsram_scrt_do_aes_2k_bytes(void);
void test_onsram_scrt_do_aes_2k_bytes_3times(void);
void test_onsram_scrt_do_aes_2k_bytes_src_dst_addr_the_same(void);
void test_onsram_scrt_do_aes_64b_1mailbox_3tokens(void);
void test_onsram_scrt_core_reg_test(void);


//NOP, it means "Memory Copy by Security IP's DMA"
void test_onsram_scrt_do_nop_in_4_mailboxs(void);


//CMAC
void test_onsram_scrt_aes_cmac(void);


//HASH
void test_onsram_scrt_hash(void);


//TRNG
void test_onsram_scrt_trng(void);


//FPGA test on ROM
void fpga_test_onrom_level1_scrt0_reg_access(void);
void fpga_test_onrom_level1_scrt1_reg_access(void);
void fpga_test_onrom_level1_sram_access(void);
void fpga_test_onrom_level5_scrt_sram_burst(void);
void fpga_test_onrom_level4_scrt0_aes(void);
void fpga_test_onrom_level2_scrt0_all_reg_access(void);
void fpga_test_onrom_level4_scrt0_aes_64bytes();
void fpga_test_onrom_level4_scrt0_aes_64bytes_debug(void);

#endif //#if 0


//ITP test
bool itp_scrt_scrt0_reg_access(void);
bool itp_scrt_scrt1_reg_access(void);
bool itp_scrt_sram_access(void);
bool itp_scrt_sram_burst(void);
bool itp_scrt_scrt0_all_reg_access(void);
bool itp_scrt_aes_64bytes(void);
bool itp_scrt_aes_cmac(void);
bool itp_scrt_hash(void);
bool itp_scrt_trng(void);
bool itp_scrt_aes_ccm(void);
bool itp_scrt_ecdh_phase1(void);


