#pragma once

#include "WiFi_Mask_Defines.h"


enum IQ2010EXT_ERROR_CODES
{
 	IQ2010EXT_ERR_OK = 0,
    IQ2010EXT_ERR_NULL_POINTER,
    IQ2010EXT_ERR_MULTISEGMENT_WAVELOAD_FAILED,
    IQ2010EXT_ERR_INVALID_RF_PORT,
    IQ2010EXT_ERR_NO_IQ2010_FOUND,
    IQ2010EXT_ERR_NO_MPTA_FOUND,
    IQ2010EXT_ERR_VSA_SEQUENCE_STILL_RUNNING,
    IQ2010EXT_ERR_VSA_SEQUENCE_NOT_INITIATED,
    IQ2010EXT_ERR_INVALID_DATA_CAPTURE_RANGE,

    IQ2010EXT_ERR_INVALID_DATA_CAPTURE,
    IQ2010EXT_ERR_INVALID_ANALYSIS,
    IQ2010EXT_ERR_NO_RESULT,
    IQ2010EXT_ERR_PARAMETER,
    IQ2010EXT_ERR_MASK_TEST,
    IQ2010EXT_ERR_MULTI_SEGMENT_WAVE_FILE_GENERATION,
    IQ2010EXT_ERR_NOT_IQ2010,
    IQ2010EXT_ERR_VSG_PORT_IS_OFF,
    IQ2010EXT_ERR_FAIL_SPECTRAL_MEASUREMENT,
    IQ2010EXT_ERR_INVALID_INDEX,
    IQ2010EXT_ERR_INVALID_RESULT,
    IQ2010EXT_ERR_EXCEED_LIMITATION,
    IQ2010EXT_ERR_INVALID_MASK_TYPE,
    IQ2010EXT_ERR_INVALID_USER_DEFINED_POINT,
    


    IQ2010EXT_ERR_FOR_IQAPI_OFFSET = 1000,      // offset for error code from IQapi.h

    IQ2010EXT_ERR_FOR_IQAPIEXT_OFFSET = 2000,    // offset for error code from IQapiext.h

    IQ2010EXT_ERR_FOR_MSWF_OFFSET = 3000,
    IQ2010EXT_ERR_MSWF_ERR_MORE_THAN_ONE_BURST,
    IQ2010EXT_ERR_MSWF_ERR_ANALYSIS_FAILED,
    IQ2010EXT_ERR_MSWF_ERR_FAILED_TO_LOAD_MOD_FILE,
    IQ2010EXT_ERR_MSWF_ERR_MAXIMUM

};

IQ2010EXT_API enum IQ2010EXT_ANALYSIS_TYPE
{
 	EVM,
	MASK_20M,
	MASK_40M,
	POWER,
    POWER_RAMP,
    POWER_LTS,
	SPECTRUM
};


//! Data rate
IQ2010EXT_API enum IQ2010EXT_DATA_RATE
{
	IQ2010EXT_DSSS_1,                     /*!< 802.11b DSSS-1*/ 
	IQ2010EXT_DSSS_2,                     /*!< 802.11b DSSS-2*/
    IQ2010EXT_CCK_5_5,                    /*!< 802.11b CCK-5.5*/
    IQ2010EXT_CCK_11,                     /*!< 802.11b CCK-11*/
    IQ2010EXT_OFDM_6,                     /*!< 802.11ag OFDM-6*/
    IQ2010EXT_OFDM_9,                     /*!< 802.11ag OFDM-9*/
    IQ2010EXT_OFDM_12,                    /*!< 802.11ag OFDM-12*/
    IQ2010EXT_OFDM_18,                    /*!< 802.11ag OFDM-18*/
    IQ2010EXT_OFDM_24,                    /*!< 802.11ag OFDM-24*/
    IQ2010EXT_OFDM_36,                    /*!< 802.11ag OFDM-36*/
    IQ2010EXT_OFDM_48,                    /*!< 802.11ag OFDM-48*/
    IQ2010EXT_OFDM_54,                    /*!< 802.11ag OFDM-54*/
    IQ2010EXT_MCS0,                       /*!< 802.11n MCS0*/
    IQ2010EXT_MCS1,                       /*!< 802.11n MCS1*/
    IQ2010EXT_MCS2,                       /*!< 802.11n MCS2*/
    IQ2010EXT_MCS3,                       /*!< 802.11n MCS3*/
    IQ2010EXT_MCS4,                       /*!< 802.11n MCS4*/
    IQ2010EXT_MCS5,                       /*!< 802.11n MCS5*/
    IQ2010EXT_MCS6,                       /*!< 802.11n MCS6*/
    IQ2010EXT_MCS7,                       /*!< 802.11n MCS7*/
    IQ2010EXT_MCS8,                       /*!< 802.11n MCS8*/
    IQ2010EXT_MCS9,                       /*!< 802.11n MCS9*/
    IQ2010EXT_MCS10,                       /*!< 802.11n MCS10*/
    IQ2010EXT_MCS11,                       /*!< 802.11n MCS11*/
    IQ2010EXT_MCS12,                       /*!< 802.11n MCS12*/
    IQ2010EXT_MCS13,                       /*!< 802.11n MCS13*/
    IQ2010EXT_MCS14,                       /*!< 802.11n MCS14*/
    IQ2010EXT_MCS15,                       /*!< 802.11n MCS15*/
    IQ2010EXT_MCS16,                       /*!< 802.11n MCS16*/
    IQ2010EXT_MCS17,                       /*!< 802.11n MCS17*/
    IQ2010EXT_MCS18,                       /*!< 802.11n MCS18*/
    IQ2010EXT_MCS19,                       /*!< 802.11n MCS19*/
    IQ2010EXT_MCS20,                       /*!< 802.11n MCS20*/
    IQ2010EXT_MCS21,                       /*!< 802.11n MCS21*/
    IQ2010EXT_MCS22,                       /*!< 802.11n MCS22*/
    IQ2010EXT_MCS23,                       /*!< 802.11n MCS23*/
    IQ2010EXT_MCS24,                       /*!< 802.11n MCS24*/
    IQ2010EXT_MCS25,                       /*!< 802.11n MCS25*/
    IQ2010EXT_MCS26,                       /*!< 802.11n MCS26*/
    IQ2010EXT_MCS27,                       /*!< 802.11n MCS27*/
    IQ2010EXT_MCS28,                       /*!< 802.11n MCS28*/
    IQ2010EXT_MCS29,                       /*!< 802.11n MCS29*/
    IQ2010EXT_MCS30,                       /*!< 802.11n MCS30*/
    IQ2010EXT_MCS31,                       /*!< 802.11n MCS31*/
    IQ2010EXT_BT_1DH1,                    /*Bluetooth BDR 1DH1*/
    IQ2010EXT_BT_1DH3,                    /*Bluetooth BDR 1DH3*/
    IQ2010EXT_BT_1DH5,                    /*Bluetooth BDR 1DH5*/  
    IQ2010EXT_BT_2DH1,                    /*Bluetooth EDR 2DH1*/
    IQ2010EXT_BT_2DH3,                    /*Bluetooth EDR 2DH3*/
    IQ2010EXT_BT_2DH5,                    /*Bluetooth EDR 2DH5*/
    IQ2010EXT_BT_3DH1,                    /*Bluetooth EDR 3DH1*/
    IQ2010EXT_BT_3DH3,                    /*Bluetooth EDR 3DH3*/
    IQ2010EXT_BT_3DH5                     /*Bluetooth EDR 3DH5*/

};

enum IQ2010EXT_SPECTRAL_TYPE
{
    IQ2010EXT_SPECTRAL_11AG,
    IQ2010EXT_SPECTRAL_11N_HT20,
    IQ2010EXT_SPECTRAL_11N_HT40,
    IQ2010EXT_SPECTRAL_11N
};

