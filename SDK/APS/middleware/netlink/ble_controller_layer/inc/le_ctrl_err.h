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

#ifndef __LE_CTRL_ERR_H__
#define __LE_CTRL_ERR_H__

#define Error_Code_Success																			0x00
#define Error_Code_Unknown_HCI_Command																0x01
#define Error_Code_Unknown_Connection_Identifier													0x02
#define Error_Code_Hardware_Failure																	0x03
#define Error_Code_Page_Timeout																		0x04
#define Error_Code_Authentication_Failure															0x05
#define Error_Code_PIN_or_Key_Missing																0x06
#define Error_Code_Memory_Capacity_Exceeded															0x07
#define Error_Code_Connection_Timeout																0x08
#define Error_Code_Connection_Limit_Exceeded														0x09
#define Error_Code_Synchronous_Connection_Limit_To_A_Device_Exceeded								0x0A
#define Error_Code_ACL_Connection_Already_Exists													0x0B
#define Error_Code_Command_Disallowed																0x0C
#define Error_Code_Connection_Rejected_due_to_Limited_Resources										0x0D
#define Error_Code_Connection_Rejected_Due_To_Security_Reasons										0x0E
#define Error_Code_Connection_Rejected_due_to_Unacceptable_BD_ADDR									0x0F
#define Error_Code_Connection_Accept_Timeout_Exceeded												0x10
#define Error_Code_Unsupported_Feature_or_Parameter_Value											0x11
#define Error_Code_Invalid_HCI_Command_Parameters													0x12
#define Error_Code_Remote_User_Terminated_Connection												0x13
#define Error_Code_Remote_Device_Terminated_Connection_due_to_Low_Resources							0x14
#define Error_Code_Remote_Device_Terminated_Connection_due_to_Power_Off								0x15
#define Error_Code_Connection_Terminated_By_Local_Host												0x16
#define Error_Code_Repeated_Attempts																0x17
#define Error_Code_Pairing_Not_Allowed																0x18
#define Error_Code_Unknown_LMP_PDU																	0x19
#define Error_Code_Unsupported_Remote_Feature_Unsupported_LMP_Feature								0x1A
#define Error_Code_SCO_Offset_Rejected																0x1B
#define Error_Code_SCO_Interval_Rejected															0x1C
#define Error_Code_SCO_Air_Mode_Rejected															0x1D
#define Error_Code_Invalid_LMP_Parameters_Invalid_LL_Parameters										0x1E
#define Error_Code_Unspecified_Error																0x1F
#define Error_Code_Unsupported_LMP_Parameter_Value_Unsupported_LL_Parameter_Value					0x20
#define Error_Code_Role_Change_Not_Allowed															0x21
#define Error_Code_LMP_Response_Timeout_LL_Response_Timeout											0x22
#define Error_Code_LMP_Error_Transaction_Collision													0x23
#define Error_Code_LMP_PDU_Not_Allowed																0x24
#define Error_Code_Encryption_Mode_Not_Acceptable													0x25
#define Error_Code_Link_Key_cannot_be_Changed														0x26
#define Error_Code_Requested_QoS_Not_Supported														0x27
#define Error_Code_Instant_Passed																	0x28
#define Error_Code_Pairing_With_Unit_Key_Not_Supported												0x29
#define Error_Code_Different_Transaction_Collision													0x2A
// #define Error_Code_Reserved																			0x2B
#define Error_Code_QoS_Unacceptable_Parameter														0x2C
#define Error_Code_QoS_Rejected																		0x2D
#define Error_Code_Channel_Classification_Not_Supported												0x2E
#define Error_Code_Insufficient_Security															0x2F
#define Error_Code_Parameter_Out_Of_Mandatory_Range													0x30
// #define Error_Code_Reserved																			0x31
#define Error_Code_Role_Switch_Pending																0x32
// #define Error_Code_Reserved																			0x33
#define Error_Code_Reserved_Slot_Violation															0x34
#define Error_Code_Role_Switch_Failed																0x35
#define Error_Code_Extended_Inquiry_Response_Too_Large												0x36
#define Error_Code_Secure_Simple_Pairing_Not_Supported_By_Host										0x37
#define Error_Code_Host_Busy_Pairing																0x38
#define Error_Code_Connection_Rejected_due_to_No_Suitable_Channel_Found								0x39
#define Error_Code_Controller_Busy																	0x3A
#define Error_Code_Unacceptable_Connection_Parameters												0x3B
#define Error_Code_Directed_Advertising_Timeout														0x3C
#define Error_Code_Connection_Terminated_due_to_MIC_Failure											0x3D
#define Error_Code_Connection_Failed_to_be_Established												0x3E
#define Error_Code_MAC_Connection_Failed															0x3F
#define Error_Code_Coarse_Clock_Adjustment_Rejected_but_Will_Try_to_Adjust_Using_Clock_Dragging		0x40

#endif
