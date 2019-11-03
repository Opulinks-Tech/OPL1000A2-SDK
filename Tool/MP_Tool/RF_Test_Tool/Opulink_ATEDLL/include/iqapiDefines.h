#ifndef _IQAPIDEFINES_H_
#define _IQAPIDEFINES_H_

// ****************************************************************
// DLL Import / Export Defines
// ****************************************************************
#ifdef IQAPI_EXPORTS
	#define IQ_API  __declspec(dllexport)
#elif defined(IQAPI_STATIC)
	#define IQ_API
#elif defined(WIN32)
	#define IQ_API __declspec(dllimport)
#else
    #define IQ_API __attribute__((visibility("default")))
#endif



// Definitions
#define N_MAX_TESTERS			4
#define MAX_LEN_ERR_TXT			1024
#define AUTO_DETECT				-1
#define N_MAX_ATTENUATORS		4
#define MAX_RESPONSE			1024

#define NEW_NFC_ANALYSIS		dynamic_cast<iqapiAnalysis *>(new iqapiAnalysisNfc)
#define NEW_80216_ANALYSIS		dynamic_cast<iqapiAnalysis *>(new iqapiAnalysis80216)
#define NEW_MIMO_ANALYSIS		dynamic_cast<iqapiAnalysis *>(new iqapiAnalysisMimo)
#define NEW_OFDM_ANALYSIS		dynamic_cast<iqapiAnalysis *>(new iqapiAnalysisOFDM)
#define NEW_11B_ANALYSIS		dynamic_cast<iqapiAnalysis *>(new iqapiAnalysis11b)
#define NEW_WAVE_ANALYSIS		dynamic_cast<iqapiAnalysis *>(new iqapiAnalysisWave)
#define NEW_SIDELOBE_ANALYSIS   dynamic_cast<iqapiAnalysis *>(new iqapiAnalysisSidelobe)
#define NEW_PWR_RAMP_ANALYSIS   dynamic_cast<iqapiAnalysis *>(new iqapiAnalysisPowerRamp)
#define NEW_CW_ANALYSIS			dynamic_cast<iqapiAnalysis *>(new iqapiAnalysisCW)
#define NEW_CCDF_ANALYSIS		dynamic_cast<iqapiAnalysis *>(new iqapiAnalysisCCDF)
#define NEW_FFT_ANALYSIS		dynamic_cast<iqapiAnalysis *>(new iqapiAnalysisFFT)
#define NEW_POWER_ANALYSIS		dynamic_cast<iqapiAnalysis *>(new iqapiAnalysisPower)
#define NEW_BLUETOOTH_ANALYSIS  dynamic_cast<iqapiAnalysis *>(new iqapiAnalysisBluetooth)
#define NEW_ZIGBEE_ANALYSIS		dynamic_cast<iqapiAnalysis *>(new iqapiAnalysisZigbee)

#define NEW_ACPR_RESULTS        dynamic_cast<iqapiResult *>(new iqapiResultAcpr)
#define NEW_OBW_RESULTS         dynamic_cast<iqapiResult *>(new iqapiResultObw)
#define NEW_NFC_RESULTS         dynamic_cast<iqapiResult *>(new iqapiResultNfc)
#define NEW_80216_RESULTS		dynamic_cast<iqapiResult *>(new iqapiResult80216)
#define NEW_MIMO_RESULTS		dynamic_cast<iqapiResult *>(new iqapiResultMimo)
#define NEW_OFDM_RESULTS		dynamic_cast<iqapiResult *>(new iqapiResultOFDM)
#define NEW_11B_RESULTS			dynamic_cast<iqapiResult *>(new iqapiResult11b)
#define NEW_WAVE_RESULTS		dynamic_cast<iqapiResult *>(new iqapiResultWave)
#define NEW_SIDELOBE_RESULTS    dynamic_cast<iqapiResult *>(new iqapiResultSidelobe)
#define NEW_PWR_RAMP_RESULTS    dynamic_cast<iqapiResult *>(new iqapiResultPowerRamp)
#define NEW_CW_RESULTS			dynamic_cast<iqapiResult *>(new iqapiResultCW)
#define NEW_CCDF_RESULTS		dynamic_cast<iqapiResult *>(new iqapiResultCCDF)
#define NEW_FFT_RESULTS			dynamic_cast<iqapiResult *>(new iqapiResultFFT)
#define NEW_POWER_RESULTS		dynamic_cast<iqapiResult *>(new iqapiResultPower)
#define NEW_BLUETOOTH_RESULTS   dynamic_cast<iqapiResult *>(new iqapiResultBluetooth)
#define NEW_ZIGBEE_RESULTS		dynamic_cast<iqapiResult *>(new iqapiResultZigbee)

#define NFC_RESULTS_CAST		dynamic_cast<iqapiResultNfc *>(hndl->results)
#define WIMAX_RESULTS_CAST		dynamic_cast<iqapiResult80216 *>(hndl->results)
#define MIMO_RESULTS_CAST		dynamic_cast<iqapiResultMimo *>(hndl->results)
#define OFDM_RESULTS_CAST		dynamic_cast<iqapiResultOFDM *>(hndl->results)
#define _11B_RESULTS_CAST		dynamic_cast<iqapiResult11b *>(hndl->results)
#define WAVE_RESULTS_CAST		dynamic_cast<iqapiResultWave *>(hndl->results)
#define SIDELOBE_RESULTS_CAST   dynamic_cast<iqapiResultSidelobe *>(hndl->results)
#define PWR_RAMP_RESULTS_CAST   dynamic_cast<iqapiResultPowerRamp *>(hndl->results)
#define CW_RESULTS_CAST			dynamic_cast<iqapiResultCW *>(hndl->results)
#define CCDF_RESULTS_CAST		dynamic_cast<iqapiResultCCDF *>(hndl->results)
#define FFT_RESULTS_CAST		dynamic_cast<iqapiResultFFT *>(hndl->results)
#define POWER_RESULTS_CAST		dynamic_cast<iqapiResultPower *>(hndl->results)
#define BLUETOOTH_RESULTS_CAST  dynamic_cast<iqapiResultBluetooth *>(hndl->results)
#define ZIGBEE_RESULTS_CAST		dynamic_cast<iqapiResultZigbee *>(hndl->results)

#define MCS_MAX	31

#define IQV_TOKEN_AVAIL      0
/*! Sets the gap key mode of the VSGs.
*/
#define IQV_GAP_KEY_MODES IQV_GAP_POWER    //!< Used for backward compatibility 

/*! Provides enumeration for error codes */
enum IQVNXN_ERR_CODES
{
    IQAPI_FAIL_TO_CHECK_USB_CONNECTION_STATUS_WARNING = -3,
    IQAPI_COMMAND_WARNING = -2,
    IQAPI_LPC_NOT_START_WARNING = -1,      //!< LPC is supposed to execute automatically, but is not executed. Therefore, IQapi makes an attempt to execute LPC.
    IQAPI_FM_VSA_EXCEED_ALLOW_CAPTURE_TIME = -3,

    IQAPI_ERR_OK = 0,
    IQAPI_NULL_POINTER,
    IQAPI_CONNECTION_ERR,
    IQAPI_DATA_ERR,
    IQAPI_GENERAL_ERROR,
    IQAPI_NOT_CONNECTED_ERROR,
    IQAPI_NO_VALID_CAPTURE_ERROR,
    IQAPI_FW_SERVER_ERROR,
    IQAPI_PORT_CONFLICT_ERROR,
    IQAPI_FREQUENCY_CONFLICT_ERROR,
    IQAPI_LPC_NOT_START_ERR,               //!< Cannot start \c LitePointConnectivityServer.exe
    IQV_TOKEN_ERR_TIMEOUT,
    IQAPI_FILE_NOT_FOUND_ERR,
    IQAPI_SOCKET_ERR,
    IQAPI_SENT_ERR,
    IQAPI_RECEIVE_ERR,
    IQAPI_TERM_SOCKET_ERR,
    IQAPI_FAIL_TO_GET_ERROR_MESSAGE_ERR,
    IQAPI_INVALID_INPUT,
    IQAPI_FIRMWARE_VERSION_NOT_MATCH,
    IQAPI_FPGA_VERSION_NOT_MATCH,
    IQAPI_FAIL_ALLOCATE_MEMORY,
    IQAPI_INCORRECT_CONNECTION_TYPE,
    IQAPI_HT40_CAPTURE_ERR,                //!< Fail to capture in HT 40 mode
    IQAPI_INCORRECT_TRIGGER_TYPE,
    IQAPI_ERR_LICENSE,                   //! license is not available
	IQAPI_ERR_INCOMPATIBLE_BOARD_TYPE,	 //! An incompatible board is installed

    //! BlueTooth analysis 
    IQAPI_BLUE_TOOTH_LOW_ENERGY_NOT_SUPPORT_ERR,         //! Low energy bluetooth only supported in IQ201x
    IQAPI_HARDWARE_NOT_SUPPORTED_ERR,

    IQAPI_FAIL_COMM_MUTEX_ERR,

    /*! GPS error code*/
    //   IQAPI_GPS_ERR_CODE_OFFSET  = 1000,
    IQAPI_CONNECTION_GPS_FM_ERR,
    //! From GPS command object
    IQAPI_SYSTEM_ERR,
    IQAPI_PARAMETER_ERR,
    IQAPI_FW_SERVER_ERR,
    IQAPI_FAIL_PARSE_RESULT_ERR,
    IQAPI_INCORRECT_RESULT_ERR,
    IQAPI_COMMAND_FAIL_ERR,
    IQAPI_CREATE_COMMAND_ERR,

    //! From GPS object
    IQAPI_GPS_CHANNEL_NOT_EXIST_ERR,
    IQAPI_GPS_INVALID_VALUE_ERR,
    IQAPI_GPS_GET_HW_VERSION_ERR,
    IQAPI_GPS_GET_SW_VERSION_ERR,
    IQAPI_GPS_GET_TW_VERSION_ERR,
    IQAPI_GPS_GET_SERIAL_NUMBER_ERR,
    IQAPI_GPS_GET_ALL_VERSION_ERR,   

    //! FM Error Code
    // FM VSG
    IQAPI_FM_DUPLICATE_FREQ_CHANNEL_ERR,
    IQAPI_FM_TONE_INDEX_NOT_FOUND_ERR,
    IQAPI_FM_VSG_EXCEED_MAX_ALLOWED_TONE_ERR,

    // FM VSA
    IQAPI_FM_VSA_INCORRECT_DATA_CAPTURE_ERR,

    //! FM Analysis error code
    IQAPI_FM_ANALYSIS_ERROR_CODE_OFFSET = 2000,
    IQAPI_FM_ANALYSIS_FM_RF_ANALYSIS_FAIL,
    IQAPI_FM_ANALYSIS_FM_DEMOD_FAIL,
    IQAPI_FM_ANALYSIS_FM_MONO_ANALYSIS_FAIL,
    IQAPI_FM_ANALYSIS_FM_STEREO_ANALYSIS_FAIL,
    IQAPI_FM_ANALYSIS_AUDIO_MONO_ANALYSIS_FAIL,
    IQAPI_FM_ANALYSIS_AUDIO_STEREO_ANALYSIS_FAIL,
    IQAPI_FM_ANALYSIS_PARAM_PARSE_FAIL,
    IQAPI_FM_ANALYSIS_PARAM_DEFAULT_SET_FAIL,
    IQAPI_FM_ANALYSIS_RDS_ANALYSIS_FAIL,
    IQAPI_FM_ANALYSIS_INVALID_INPUT_STRUCTURE,
    // End of FM error code => error code offset = 2000

	
	//! GLONASS Error Code
	IQAPI_GLONASS_ERROR_CODE_OFFSET = 2100,
	IQAPI_GLONASS_ERROR_INVALID_FREQ,
	IQAPI_GLONASS_ERROR_INVALID_SATELLITE,
	IQAPI_GLONASS_ERROR_INVALID_POWER,
	IQAPI_GLONASS_ERROR_INVALID_TRIGGER,
	IQAPI_GLONASS_ERROR_NOT_SUPPORTED,

	//! iqapiWave Error Code
	IQAPI_WAVE_ERR_INVALID_INPUT_GENERAL = 2500,
	IQAPI_WAVE_ERR_INVALID_WIFI_BIT_RATE,
	IQAPI_WAVE_ERR_INVALID_WIFI_PSDU_LENGTH,
	IQAPI_WAVE_ERR_INVALID_WIFI_PSDU_MODE,
	IQAPI_WAVE_ERR_INVALID_WIFI_MOD_TYPE,
	IQAPI_WAVE_ERR_INVALID_WIFI_CLOCK_TYPE,
	IQAPI_WAVE_ERR_INVALID_WIFI_PREAMBLE_TYPE,
	IQAPI_WAVE_ERR_INVALID_WIFI_SPREADING_TYPE,
	IQAPI_WAVE_ERR_INVALID_WIFI_PREAMBLE_DIS_TYPE,
	IQAPI_WAVE_ERR_INVALID_WIFI_SAMPLING_RATE,
	IQAPI_WAVE_ERR_INVALID_WIFI_OFDM_INPUT = 2601,
    IQAPI_WAVE_ERR_INVALID_WIFI_OFDM_PSDU,
    IQAPI_WAVE_ERR_INVALID_WIFI_OFDM_MCS,
    IQAPI_WAVE_ERR_INVALID_WIFI_OFDM_SS,
    IQAPI_WAVE_ERR_INVALID_WIFI_OFDM_STBC,
    IQAPI_WAVE_ERR_INVALID_WIFI_OFDM_NUMESS,
    IQAPI_WAVE_ERR_INVALID_WIFI_OFDM_WAVE_AGGR = 2622,
    IQAPI_WAVE_ERR_INVALID_WIFI_OFDM_WAVE_PSDU,
    IQAPI_WAVE_ERR_INVALID_WIFI_OFDM_WAVE_PSDU_LEN,
    IQAPI_WAVE_ERR_INVALID_WIFI_OFDM_WAVE_PSDU_MODE,
    IQAPI_WAVE_ERR_INVALID_WIFI_OFDM_WAVE_MCS,
    IQAPI_WAVE_ERR_INVALID_WIFI_OFDM_WAVE_NSS,
    IQAPI_WAVE_ERR_INVALID_WIFI_OFDM_WAVE_STBC,
    IQAPI_WAVE_ERR_INVALID_WIFI_OFDM_WAVE_NESS,
    IQAPI_WAVE_ERR_INVALID_WIFI_OFDM_WAVE_INP,
    IQAPI_WAVE_ERR_INVALID_WIFI_OFDM_WAVE_SOUN,
    IQAPI_WAVE_ERR_INVALID_WIFI_OFDM_WAVE_BW,
    IQAPI_WAVE_ERR_INVALID_WIFI_OFDM_WAVE_GRE,
    IQAPI_WAVE_ERR_INVALID_WIFI_OFDM_WAVE_SGI,
    IQAPI_WAVE_ERR_INVALID_WIFI_OFDM_WAVE_LLEN,
    IQAPI_WAVE_ERR_INVALID_WIFI_OFDM_WAVE_SCR,
    IQAPI_WAVE_ERR_INVALID_WIFI_OFDM_WAVE_IDLE,
    IQAPI_WAVE_ERR_INVALID_WIFI_OFDM_WAVE_CYC,
    IQAPI_WAVE_ERR_INVALID_WIFI_OFDM_WAVE_COD,
    IQAPI_WAVE_ERR_INVALID_WIFI_OFDM_WAVE_MPDU_LEN,
    IQAPI_WAVE_ERR_INVALID_WIFI_OFDM_WAVE_MPDU_NUM,
    IQAPI_WAVE_ERR_INVALID_WIFI_OFDM_WAVE_MPDU_LVEC,
    IQAPI_WAVE_ERR_INVALID_WIFI_OFDM_WAVE_COMB,
    IQAPI_WAVE_ERR_INVALID_WIFI_OFDM_WAVE_SMAP,
	IQAPI_WAVE_ERR_INVALID_NUM_SIGNAL_STREAM = 3000
};

enum IQV_CONNECTION_TYPE
{
	IQV_CONNECTION_ANY_TYPE = -1, //!< Any testers(might take longer for non-IQ201x testers)
    IQV_CONNECTION_IQ201X = 0,    //!< Any IQ201x testers
    IQV_CONNECTION_IQFM,          //!< IQfm
    IQV_CONNECTION_IQNAV,         //!< IQnav
    IQV_CONNECTION_OTHER          //!< For testers not defined above(i.e. IQflex, IQmax, etc)
};

enum IQV_FPGA_TYPE
{
    IQV_FPGA_VSA,
    IQV_FPGA_VSG,
    IQV_FPGA_FL,
    IQV_FPGA_GPS_FM_RF,
    IQV_FPGA_GPS_FM_BB
};

enum
{
    DISABLE, 
    ENABLE
};

///**********************************************************************************************
/// FFT analysis enum
///**********************************************************************************************
enum IQV_SPECTRUM_ANALYSIS_MODE_ENUM	

{
    IQV_DIGITAL_SPECTRUM_ANALYSIS_MODE,	//!< FFT-based spectrum analysis mode
    /*!<
    This is the default value.
    */
    IQV_CLASSICAL_SPECTRUM_ANALYSIS_MODE	//!< Emulates classical spectrum analyzer
};

///**********************************************************************************************
/// MIMO analysis enum
///**********************************************************************************************

enum IQV_MIMO_FREQUENCY_CORR
{
    IQV_MIMO_FREQ_SYNC_SHORT   = 1, //!< Performs frequency synchronization using   short training symbol
    IQV_MIMO_FREQ_SYNC_LONG    = 2, //!< Performs frequency synchronization using short and long training symbol
    IQV_MIMO_FREQ_SYNC_FULL    = 3, //!< Performs frequency synchronization on  full packet
    IQV_MIMO_FREQ_SYNC_SIG     = 4  //!< Performs frequency synchronization using short and long training symbols and L-SIG and HT-SIG fields
};

enum IQV_MIMO_ENABLE_MULTIPATH_EVM
{
    IQV_MIMO_EVM_STANDARD,	//!< Indicates standard EVM calculation mode ( no multipath )
    /*!< 
    This is the default value.
    */
    IQV_MIMO_EVM_MULTIPATH 	//!< Indicates EVM calculation mode for multipath MIMO
};

//! ENUM values for signals
enum IQV_USE_ALL_SIGNALS	
{
    IQV_MIMO_FIRST_NSTREAM,	//!< Indicates the first 'n' VSA signals, where 'n' is the number of streams in the signal. 
    /*!< This is the default value.
    */
    IQV_MIMO_ALL_VALID_SIGNAL	//!< Indicates that all valid VSA signals are used for analysis
};

enum IQV_MIMO_PACKET_FORMAT
{
    IQV_MIMO_AUTO_DETECT,   //!< Indicates auto detection
    IQV_MIMO_MIXED, 	    //!< Indicates mixed mode
    IQV_MIMO_GREEN_FIELD,	//!< Indicates green field
    IQV_MIMO_11AC_NON_HT,	//!< Indicates 11ac Non-HT, legacy a/g
    IQV_MIMO_11AC_VHT	    //!< Indicates 11ac VHT
};

//!Indicates whether enumeration has a PASS/FAIL value.
enum IQV_PF_ENUM
{
    IQV_PF_FAIL,         //!< Indicates that measurement has failed.
    IQV_PF_PASS,         //!< Indicates that measurement has passed.
    IQV_PF_UNDEFINED     //!< Indicates that pass/fail status is undefined. 
};

/*!Enumeration for ADC input switch setting. */
enum IQV_INPUT_ADC_ENUM
{
    IQV_INPUT_ADC_RF,          //!< Indicates data captured from RF.
    IQV_INPUT_ADC_BB,          //!< Indicates data captured from base band.
    IQV_INPUT_ADC_UNDEFINED,   //!< Indicates undefined default value.
    IQV_GET_RAW_DATA = -2      //!< Gets the raw data with no compensation.
};

/*!Enumeration for modulation switch setting. */
enum IQV_INPUT_MOD_ENUM
{
    IQV_INPUT_MOD_DAC_RF,       //!< Indicates VSG modulation on RF                                               
    IQV_INPUT_MOD_DAC_BB,       //!< Indicates VSG modulation on BB                                               
    IQV_INPUT_MOD_UNDEFINED     //!< Indicates undefined default value
};

/*!Enumeration for data capture handling. */
enum IQV_DC_ENUM
{
    IQV_DC_ARM_NEW,             //!< Arm the IQview tester for new data capture 
    IQV_DC_ARM_RESERVED,        //!< Arm reserved mode
    IQV_DC_UNDEFINED            //!< Undefined default value
};

/*!Enumeration for continuous data capture handling. */
enum IQV_DC_CONT_ENUM
{
    IQV_DC_CONT_OFF,      //!< Used to clear the continuous mode.
    IQV_DC_CONT_ON,       //!< Used to run the continuous mode.
    IQV_DC_CONT_NEW_ADC,  //!< Used for running new ADC in continuous mode.
    IQV_DC_CONT,          //!< Used when running in continuous mode.
    IQV_DC_NON_BLOCKING = 33,	//!< Initiates a new ADC background task, does not get any data
    IQV_DC_WAIT_FOR_TRIGGER = 34,	//!< Wait for trigger. Get ADC data
    IQV_DC_CONT_UNDEFINED,    //!< Undefined default value.
    IQV_DC_CONT_CLEAN_UP=-1   //!< Clean up continuous mode
};

/*!Enumeration for signal modulation source. */
enum IQV_SOURCE_ENUM 
{
    IQV_SOURCE_WAVE,              //!< Modulation from loaded waveform.
    IQV_SOURCE_SIGNAL_GENERATOR,  //!< Modulation from CW signal generator.                                      
    IQV_SOURCE_UNDEFINED         //!< Undefined default modulation source.
                                            
};

// VSA
/*!Enumeration for trigger selection */
enum IQV_TRIG_TYPE_ENUM
{
    IQV_TRIG_TYPE_AUTO       = -6,     //!< Indicates that a free run capture is done if IF Trigger times out 

    IQV_TRIG_TYPE_FREE_RUN   = 1,    //!< Indicates free-run capture mode 
    IQV_TRIG_TYPE_EXT        = 2,    //!< Indicates that the trigger type is external 
    IQV_TRIG_TYPE_EXT_1      = 2,    //!< Indicates External Trigger port 1 is selected at the positive edge (rising edge)
    IQV_TRIG_TYPE_IF2_NO_CAL = 6,    //!< Indicates IF Trigger is selected (triggered by signal)
    IQV_TRIG_TYPE_EXT_NEG    = 7,    //!< Negative
    IQV_TRIG_TYPE_EXT_1_NEG  = 7,    //!< Indicates External Trigger is selected - negative (falling edge) - for port 1 
    IQV_TRIG_TYPE_EXT_2      = 8,    //!< Indicates External trigger is selected - positive (rising edge) - for port 2
    IQV_TRIG_TYPE_EXT_2_NEG  = 9,    //!< Indicates External Trigger is selected - negative (falling edge) - for port 2

    IQV_TRIG_TYPE_FOR_IQ2010            = 13,    //!< Trigger for IQ 2010 / Pebble Beach 
    IQV_TRIG_TYPE_FOR_IQ2010_DIG        = 13,    //!< Trigger for IQ 2010 / Digital trigger in FPGA
    IQV_TRIG_TYPE_FOR_IQ2010_DIG_I      = 14,    //!< Trigger for IQ 2010 / Digital trigger in FPGA I only
    IQV_TRIG_TYPE_FOR_IQ2010_DIG_Q      = 15,     //!< Trigger for IQ 2010 / Digital trigger in FPGA Q only
	IQV_TRIG_TYPE_VIDEO_LEVEL  = 16,    //!< Indicates IF Trigger is selected (triggered by level)

};

enum IQV_PH_CORR_ENUM
{
    IQV_PH_CORR_OFF        = 1,     //!< Indicates that Phase correction is off.
    IQV_PH_CORR_SYM_BY_SYM = 2,     //!< Indicates symbol-by-symbol correction.
    IQV_PH_CORR_MOVING_AVG = 3      //!< Indicates moving avg. correction 
};

enum IQV_CH_EST_ENUM
{
    IQV_CH_EST_RAW       = 1,   //!< Raw Channel Estimate on long training field 
    /*!< This is the default value.

                                               - long train. */
    IQV_CH_EST_RAW_LONG  = IQV_CH_EST_RAW,    //!< Same as IQV_CH_EST_RAW.
    IQV_CH_EST_2ND_ORDER = 2,                 //!< 2nd Order Polyfit.
    IQV_CH_EST_RAW_FULL  = 3                  //!< Raw Channel Estimate on full packet 
 };

/*! Enumeration for Symbol Timing Correction - 802.11 a/g setting. */
enum IQV_SYM_TIM_ENUM
{
    IQV_SYM_TIM_OFF = 1,  //!< Symbol Timing Correction Off.
    IQV_SYM_TIM_ON  = 2   //!< Symbol Timing Correction On
                           /*!< This is the default value */
};

/*! Enumeration for Frequency Sync. Mode - 802.11 a/g setting. */
enum IQV_FREQ_SYNC_ENUM
{
    IQV_FREQ_SYNC_SHORT_TRAIN = 1,            //!< Indicates frequency synchronization on short training symbol. */
    IQV_FREQ_SYNC_LONG_TRAIN  = 2,            //!< Indicates frequency synchronization on short and long training symbol 
    /*!< Indicates the default value. */
    IQV_FREQ_SYNC_FULL_PACKET = 3             //!< Indicates frequency synchronization on full data packet 
};

/*! Enumeration for Amplitude Tracking - 802.11 a/g setting. */
enum IQV_AMPL_TRACK_ENUM
{
    IQV_AMPL_TRACK_OFF = 1,     //!< Amplitude tracking off.
    IQV_AMPL_TRACK_ON  = 2      //!< Amplitude tracking on 
                                 /*!< Default value. */
};

/*! Enumeration for Number of equalizer taps - 802.11 b setting. */
enum IQV_EQ_ENUM
{
    IQV_EQ_OFF    = 1,        //!< Equalizer Off.
    IQV_EQ_5_TAPS = 5,        //!< 5 taps equalizer 
                            /*!< Default value. */
    IQV_EQ_7_TAPS = 7,        //!< 7 taps equalizer. 
    IQV_EQ_9_TAPS = 9         //!< 9 taps equalizer.
};

/*! Enumeration for DC removal - 802.11 b setting. */
enum IQV_DC_REMOVAL_ENUM
{
    IQV_DC_REMOVAL_OFF = 0,     //!< DC removal Off 
                               /*!< Default value. */
    IQV_DC_REMOVAL_ON  = 1       //!< DC removal On.
};

/*! Enumeration for 802.11 b method - 802.11 b setting. */
enum IQV_11B_METHOD_ENUM
{
    IQV_11B_STANDARD_TX_ACC  = 1,   //!< Use 11b standard TX accuracy
    /*!< This is the default value. */
    IQV_11B_RMS_ERROR_VECTOR = 2,    //!< Use 11b RMS error vector.

    IQV_11B_STANDARD_2007_TX_ACC = 3    //!< Use 11b standard 2007 TX accuracy
};

/*! Enumeration for OFDM EVM method - OFDM setting. */
enum IQV_OFDM_EVM_METHODS
{
    IQV_OFDM_EVM_MULTIPATH,		//!< More tolerant of notches in the channel response */
    IQV_OFDM_EVM_STANDARD		//!< Standard 802.11 a/g EVM Method /*!< This is the default value */
};

/*! Enumeration for enabling and disabling the RF. */
enum IQV_RF_ENABLE_ENUM
{
    IQV_RF_DISABLED,    //!< RF is disabled. 
    IQV_RF_ENABLED      //!< RF is enabled.  
};

/*! Enumeration for RF N-connector port selection. */
enum IQV_PORT_ENUM
{
    IQV_PORT_OFF   = 1,         //!< Port has been disabled. 
    IQV_PORT_LEFT  = 2,         //!< RF uses left port.      
    IQV_PORT_RIGHT = 3          //!< RF uses right port.     
};

/*! Enumeration for VSA number that is used to specified which VSA should be used for capture or analysis*/
enum IQV_VSA_NUM_ENUM
{
    IQV_VSA_NUM_1 = 1,
    IQV_VSA_NUM_2 = 2,
    IQV_VSA_NUM_3 = 3,
    IQV_VSA_NUM_4 = 4,

    IQV_VSA_NUM_ALL = -1       // Use all data streams in 'data' object. It is used by FFT/Power/CCDF analysis.
};

/*! Enumeration for enabling and disabling swapping of I and Q signals. Mirrors the frequency spectrum of the wave downloaded to VSG about the center frequency. (Calculates the complex conjugate of the signal) */

enum IQV_MIRROR_FREQ_ENUM
{
    IQV_MIRROR_FREQ_DISABLED,         //!< IQ swap has been disabled.
    IQV_MIRROR_FREQ_ENABLED           //!< IQ swap has been enabled.
};

enum IQV_COMP_SINC_ENUM
{
    IQV_COMP_SINC_OFF,  //  No sinc comp
    IQV_COMP_SINC_ON    //  Sinc comp up to 30 MHz

};

/*! Enumeration for structure IQV_MISC_TYPE.*/
enum IQV_MISC_ENUM
{
    IQV_MISC_INT,     //!< Indicates that the parameter is an integer.
    IQV_MISC_REAL,    //!< Indicates that the parameter is real.
    IQV_MISC_COMPLEX, //!< Indicates that the parameter is complex
    IQV_MISC_TXT,     //!< Indicates that the parameter is a text string                       
    IQV_MISC_COMPLEX_ARRAY, //!< Indicates that the parameter is a real or complex array 
    IQV_MISC_UNUSED        //!< This is an unused value
};

/*! Enumeration for setting VSA Type*/
enum IQV_RX_POWER_MODES
{
    IQV_VSA_TYPE_0,           //!< VSA amplitude value that represents average power

    /*!< This is the default value and represents RMS Power Mode.*/

    IQV_VSA_TYPE_1            //!< VSA amplitude value that represents peak power

    /*!< This is the recommended value and represents Peak Power Mode.*/
}; 

/*! Enumeration for Video AV Method, input to IQV_GROUP_FFT */
enum IQV_VIDEO_AV_METHOD_ENUM
{
    IQV_AV_PEAK_HOLD,	//!< Indicates peak hold (for classical spectrum analyzer mode only)
    IQV_AV_MEAN		//!< Indicates average mode (digital mode) or clear write (classical mode)
};

/*! Enumeration for Window Type, input to IQV_GROUP_FFT */
enum IQV_WINDOW_TYPE_ENUM
{
    IQV_WIN_BLACKMANHARRIS, //!< Window type is Blackmanharris
    IQV_WIN_HANNING,	 //!< Window type is Hanning
    IQV_WIN_RECT,		 //!< Window type is Rectangular
    IQV_WIN_FLATTOP 	 //!< Window type is Flattop 
};

/*! Enumeration for OFDM Mode */
enum IQV_OFDM_MODE_ENUM
{
    IQV_OFDM_80211_AG,	//!< Indicates OFDM mode based on IEEE 802.11a or 802.11g standards specification
    IQV_OFDM_80211_AG_TURBO, //!< Indicates OFDM turbo mode based on IEEE 802.11a or 802.11g standards specification
    IQV_OFDM_ASTM_DSRC,	//!< Indicates ASTM DSRC standards specification
    IQV_OFDM_QUARTER_RATE  //!< Indicates OFDM quarter rate
};

/*! Enumeration for Modulation Control mode */
enum IQV_MODULATION_CONTROL_ENUM
{
    IQV_WAVE_DL_MOD_DISABLE,	//!< No modulation after wave download unless in continuous Tx mode  
    IQV_WAVE_DL_MOD_ENABLE	//!< Enable continue modulation after wave download (default)
};

/*! Enumeration for MIMO Bandwidth Modes */
enum IQV_MIMO_BANDWIDTH_MODES
{
    IQV_MIMO_BW_20_MHZ,	//!< Indicates that the bandwidth for MIMO is 20 MHz
    IQV_MIMO_BW_40_MHZ	//!< Indicates that the bandwidth for MIMO is 40 MHz
};

enum IQV_GAP_POWER
{
    IQV_GAP_POWER_ON,        //!< VSG Power on during signal gaps (idle time)
    IQV_GAP_POWER_OFF,       //!< VSG Power off during signal gaps (idle time)

    IQV_DISABLE_MARKER_SIGNAL  = IQV_GAP_POWER_ON,         //!< Gap power on functionality is disabled

    /*!< Used for backward compatibility */ 
    IQV_ENABLE_GAP_SEARCH      = IQV_GAP_POWER_OFF         //!< Gap power off functionality is enabled 

    /*!< Used for backward compatibility */
};

/*! Enumeration for Automatic Level Control Modes*/
enum IQV_ALC_MODES
{
    IQV_ALC_MANUAL,	//!< Indicates ALC Manual is on
    /*!< This is the default value */
    IQV_ALC_AUTO_ON_TIMEOUT, //!< Indicates ALC Automatic on Timeout
    IQV_ALC_OFF,		//!< Indicates that ALC is off 
    IQV_ALC_MANUAL_HIGH_PRECISION	//!< Indicates ALC Manual High Precision. Will set ALC off, then wait for a specific settling time then do ALC on again
};

enum IQV_VSG_TRIG
{
    IQV_VSG_TRIG_FREE_RUN   = 0,      //!< Indicates immediate VSG transmission after wave download
    IQV_VSG_TRIG_EXT_2      = 2,      //!< Indicates VSG transmission triggered by external trigger input 2 
    /*!< Recommended for triggering VSG */
    IQV_VSG_TRIG_EXT_1      = 1       //!< Indicates VSG transmission triggered by external trigger input 1
};

enum IQV_VSG_TRIG_REARM
{
    IQV_VSG_TRIG_REARM_MANUAL  = 0,    //!< VSG needs to be re-armed after the trigger event

    /*!<  This is the default value. */
    IQV_VSG_TRIG_REARM_AUTO    = 1     //!< Indicates auto re-arm
};

//----------------------------------------------------------
// enum for general setting in Tx
//----------------------------------------------------------
/*! Indicates ENUM value for Tx settings*/
//! \note RF LO (Local oscillator) is available for LitePoint test system hardware versions 1.6.2 and higher
enum IQV_LO_PORT_ENUM
{
    IQV_LO_PORT_NOT_DEFINED = -1,      //!< Indicates no change in setting 
    IQV_LO_PORT_INT = 0,		//!< Indicates that internal LO is used 
    IQV_LO_PORT_EXT		//!< Indicates that external LO is used
};

//----------------------------------------------------------
// enum for dual header token number
//----------------------------------------------------------
/*! Indicates ENUM value for dual head token number */
enum IQV_TOKEN_ENUM
{
	IQV_TOKEN_NO_1,	//!< Indicates value for token number 1
	IQV_TOKEN_NO_2, //!< Indicates value for token number 2
	IQV_TOKEN_CNT	//!< Indicates token count
};

//----------------------------------------------------------
// enum for dual header token number
//----------------------------------------------------------
/*! Indicates ENUM value for the hardware version */
enum IQV_HARDWARE_VERSION_ENUM
{
    IQV_HARDWARE_VERSION_1_3,	//!< Indicates LitePoint test system hardware version 1.3
    IQV_HARDWARE_VERSION_1_4,	//!< Indicates LitePoint test system hardware version 1.4
    IQV_HARDWARE_VERSION_1_5,	//!< Indicates LitePoint test system hardware version 1.5
    IQV_HARDWARE_VERSION_1_6,	//!< Indicates LitePoint test system hardware version 1.6
    IQV_HARDWARE_VERSION_1_7,	//!< Indicates LitePoint test system hardware version 1.7
    IQV_HARDWARE_VERSION_1_8,	//!< Indicates LitePoint test system hardware version 1.8
    IQV_HARDWARE_VERSION_1_9,	//!< Indicates LitePoint test system hardware version 1.9
    IQV_HARDWARE_VERSION_1_10,  //!< Indicates LitePoint test system hardware version 1.10
    IQV_HARDWARE_VERSION_1_11,  //!< Indicates LitePoint test system hardware version 1.11
    IQV_HARDWARE_VERSION_1_12	//!< Indicates LitePoint test system hardware version 1.12
};
/*! Indicates ENUM value for the type of file */
enum IQV_SAVE_FILE_TYPE_ENUM
{
    IQV_SIG_FILE,	//!< Indicates that file is saved as a \c .sig file
    IQV_MOD_FILE,	//!< Indicates that file is saved as a \c .mod file
    IQV_NORMALIZE_FILE	//!< Indicates that file is saved as a normalized file
};

//! This is used to control if data should be passed back to IQapi or not.
/*! The IQV_DATA_IN_MATLAB ENUM indicates that the captured data must be stored in MATLAB. In some cases, the data does not have to be passed back from MATLAB to iqapi to the test application. In order to save time, the captured data is stored in MATLAB. All subsequent operations, such as analyze, will be operated on the captured data in MATLAB */

/*! This ENUM value indicates that after data is captured,  this function gets a copy of the captured data to the object 'data' in \c iqapi. This data will then be passed on to MATLAB for analysis.*/
enum IQV_CAPTURE_DATA_HANDLING_ENUM
{
    IQV_DATA_IN_IQAPI,	//!< Indicates captured data passed on to the object 'data' in \c iqapi 
    /*!< This is the default value. */
    IQV_DATA_IN_MATLAB	//!< Indicates data is stored in MATLAB for analysis
};
//! This ENUM value indicates type of data capture
enum IQV_CAPTURE_TYPE_ENUM
{
    IQV_NORMAL_CAPTURE_TYPE,	//!< Indicates normal data capture 
    IQV_HT_40_CAPTURE_TYPE,	//!< Indicates an HT 40 data capture
	IQV_HT_40_WIDE_BAND_LO_ACCURACY_CAPTURE_TYPE = IQV_HT_40_CAPTURE_TYPE,	//!< Indicates an HT 40 data capture
    IQV_WAVE_GENERATION_CAPTURE_TYPE,
    IQV_HT_20_WIDE_BAND_HI_ACCURACY_CAPTURE_TYPE,           //!< Indicate as HT20 wind-band capture with high accuracy
    IQV_HT_20_WIDE_BAND_LO_ACCURACY_CAPTURE_TYPE,           //!< Indicate as HT20 wind-band capture with lower accuracy but faster
	IQV_HT_40_WIDE_BAND_HI_ACCURACY_CAPTURE_TYPE,			//!< Indicate as HT40 wind-band capture with high accuracy
	IQV_VHT_80_WIDE_BAND_CAPTURE_TYPE,                      //!< Indicate as 11ac wind-band capture
	IQV_HT_40_WIDE_BAND_LO_ACCURACY_SIMPLE_CAPTURE_TYPE		//!< Indicate as HT40 wind-band capture with lower accuracy. Faster, but may see spurs.
};

enum IQV_LICENSE_TYPE
{
    IQV_BLUETOOTH_CPP_API,  //!< License for bluetooth application
	IQV_BLUETOOTH_LOW_ENERGY_CPP_API,	// License for bluetooth low energy 	
    IQV_MIMO_CPP_API,       //!< License for MIMO application
    IQV_MPS_CPP_API,	      //!< License for MPS application
    IQV_NFC_CPP_API,	      //!< License for NFC application
    IQV_WIFI_CPP_API,	      //!< License for WIFI application
    IQV_WIMAX_CPP_API,      //!< License for WIMAX application
    IQV_UWB_CPP_API,        //!< License for UWB application
    IQV_ZIGBEE_CPP_API,     //!< License for Zigbee application

    IQV_GPS_CPP_API,        //!< License for GPS application. either 1 channel or 6 channel
    IQV_GPS_1_CHANNEL_CPP_API, //!< License for GPS application for 1 channel
    IQV_GPS_6_CHANNEL_CPP_API, //!< License for GPS application for 6 channel
    IQV_FM_VSG_CPP_API,     //!< License for FM VSG application
    IQV_FM_VSA_CPP_API,      //!< License for FM VSA application
    IQV_FM_AUDIO_CPP_API,     //!< License for FM audio analysis
	IQV_GLONASS_CPP_API,			//!< License fo Glonass
	IQV_OTA_CPP_API,			//!< License fo OTA
	IQV_11Q_OPTION_1_CPP_API,	//!< License Option 1 for IQ2011Q
	IQV_11Q_OPTION_2_CPP_API,	//!< License Option 2 for IQ2011Q
	IQV_11Q_OPTION_3_CPP_API,	//!< License Option 3 for IQ2011Q
	IQV_11Q_OPTION_4_CPP_API,	//!< License Option 4 for IQ2011Q
	IQV_11Q_OPTION_5_CPP_API,	//!< License Option 5 for IQ2011Q
	IQV_11Q_OPTION_6_CPP_API,	//!< License Option 6 for IQ2011Q
	IQV_11Q_OPTION_7_CPP_API,	//!< License Option 7 for IQ2011Q
	IQV_11Q_OPTION_8_CPP_API,	//!< License Option 8 for IQ2011Q
    
    IQV_WIFI_11AC_CPP_API                //!< License for MIMO 11ac
};

enum IQV_FREQUENCY_BAND
{
    IQV_VSG_BAND, //!< Frequency band for VSG
    IQV_VSA_BAND //!< Frequecy band for VSA
};


//----------------------------------------------------------
// internal use
//----------------------------------------------------------
enum IQV_FPGA_VERSION_DATE_CHECK
{
    IQV_FPGA_VERSION_DATE_MATCH,
    IQV_FPGA_VERSION_DATE_NEWER,
    IQV_FPGA_VERSION_DATE_OLDER,
    IQV_FPGA_TYPE_NOT_MATCH
};
enum IQV_SET_TX_RX_TYPE
{
    IQV_SET_TX,
    IQV_SET_RX,
    IQV_SET_TX_RX
};
//----------------------------------------------------------
// data structure for multiport test adaptor (MPTA)
//----------------------------------------------------------
//! Defines the data structure for MPTA
#define MPTA_DEFAULT_ATTN	17		//!< Attenuation at 17dB 
/*!< Indicates default attenuation */ 
#define MPTA_MAX_ATTN		60	//!< Attenuation set at maximum
#define MPTA_MAX_PER_STEPS	100	
#define MAX_FILENAME			256

typedef struct {
	double fPower;		//!< Power level (dBm)
	double fPerPercent;	//!< Packet error rate (%)
} MPTA_PER_ARRAY;

typedef struct {
	//! setup parameters
	double fFreqHz;		//!< Indicates channel frequency (Hz)
	double fCableLoss;	//!< Indicates external cable loss (dB)
	double fTargetLeveldB;	//!< Indicates target power level for PER test (dBm)
	double fStartLevel;	//!< Indicates sensitivity test sweep start level (dBm)
	double fStopLevel;	//!< Indicates sensitivity test sweep stop level (dBm)
	int nNumOfPackets;	//!< Indicates number of packets for PER test
	int nTimeoutUs;		//!< Indicates wait timeout for the ACK packet (us)
	int nPortMask;		//!< Indicates bit mask for individual ports, bit 0=PORT1, bit 1=PORT2, bit 2=PORT3 and bit 3=PORT4
	char sWaveFile[MAX_FILENAME];	//!< Indicates filename of wave file

	//! test results
	double fPerPercent;		//!< Indicates packet error rate (%)
	double fSensitivity;		//!< Indicates Rx sensitivity (dBm)
	MPTA_PER_ARRAY PerList[MPTA_MAX_PER_STEPS];	//!< Indicates PER result array
} MPTA_RX_TEST;


typedef struct {
	double fFreqHz;		//!< Indicates channel frequency (Hz)
	double fCaptureTimeUs;	//!< Indicates single chain packet capture time (us)
	double fPeakSignaldB;	//!< Indicates peak signal level (dBm)
	int nSequenceCount;	//!< Indicates number of sequential capture frame
	int nSkipCount;		//!< Indicates number of packets to skip before capture
	int nPortMask;		//!< Indicates which port is used for data capture, bit 0=PORT1, bit 1=PORT2, bit 2=PORT3 and bit 3=PORT4
	int *pPortList;	//!< Indicates port list
} MPTA_TX_TEST;

/// ******************************************************************************
/// GPS enum
/// ******************************************************************************
//! Indicates ENUM values for GPS
#ifndef TYPE_DEF_H
#define TYPE_DEF_H
typedef unsigned    char        bit8;     //!< Indicates that the range is between byte:  0 ... 255
typedef unsigned    char        byte;     //!<  Indicates that the range is between byte:  0 ... FFH
typedef unsigned    char        uint8;    //!<  Indicates that the range is between Uint8  0 ... 255
typedef unsigned    short int   uint16;   //!<  Indicates that the range is between Uint16:0 ... 65535
typedef unsigned    int         uint32;   //!<  Indicates that the range is between Uint32:0 ... 4294967295
typedef signed      char        int8;     //!<  Indicates that the range is between Int8:  -128 ... +127
typedef signed      short int   int16;    //!<  Indicates that the range is between Int16: -32768 ... +32767
typedef signed      int         int32;    //!<  Indicates that the range is between Int32: -2147483650 ...
typedef float                   float32;  //!<  Indicates that the range is between 3.4e-38 ... 3.4e38 (7 digits)
typedef double                  float64;  //!<  Indicates that the range is between 1.7e-308 ... 1.7e308 (15 digits)
#endif

//! ENUM values for the GPS channel number
enum IQV_GPS_CHANNEL_NUMBER
{
    IQV_GPS_CHANNEL_NUMBER_1,	//!< Indicates the first GPS channel
    IQV_GPS_CHANNEL_NUMBER_2,	//!< Indicates the second GPS channel
    IQV_GPS_CHANNEL_NUMBER_3,	//!< Indicates the third GPS channel
    IQV_GPS_CHANNEL_NUMBER_4,	//!< Indicates the fourth GPS channel
    IQV_GPS_CHANNEL_NUMBER_5,	//!< Indicates the fifth GPS channel
    IQV_GPS_CHANNEL_NUMBER_6	    //!< Indicates the sixth GPS channel
};

//! ENUM values for GPS RF output
enum IQV_GPS_RF_OUTPUT
{
    IQV_RF_OUTPUT_OFF,	        //!< Indicates that RF output is off
    IQV_RF_OUTPUT_ON	        //!< Indicates that RF output is on
};

//! ENUM values for GPS trigger state
enum IQV_GPS_TRIGGER_STATE
{
    IQV_TRIGGER_STATE_INT,	    //!< Indicates internal trigger state
    IQV_TRIGGER_STATE_EXT	    //!< Indicates external trigger state
};

//! ENUM values for GPS modulation state
enum IQV_GPS_MODULATION_STATE
{
    IQV_MODULATION_STATE_OFF,	//!< Indicates that modulation state is off
    IQV_MODULATION_STATE_ON	    //!< Indicates that modulation state is on
};

//! ENUM values for GPS operation mode
enum IQV_GPS_OPERATION_MODE
{
    IQV_OPERATION_MODE_CW,	                //!< Indicates that operation mode is Continuous Wave (CW)
    IQV_OPERATION_MODE_MODULATION,	        //!< Indicates that CA Code modulation is turned on
    IQV_OPERATION_MODE_MODULATION_NAV_DATA_OFF,	   //!< Indicates that CA Code modulation is turned on with navigation data off
    IQV_OPERATION_MODE_MODULATION_OFF	    //!< Indicates that CA Code modulation is turned off
};

enum IQV_GPS_REPLAY_MODE
{
    IQV_GPS_REPLAY_OFF,
    IQV_GPS_REPLAY_ON
};

/// ******************************************************************************
/// FM VSG and VSA enum
/// ******************************************************************************
enum IQV_FM_TECH_ACTIVATION
{
    IQV_FM_VSG_TECH_ACT,        //!< Indicates VSG tech activation
    IQV_FM_VSA_TECH_ACT         //!< Indicates VSA tech activation
};

/// ******************************************************************************
/// FM VSG enum
/// ******************************************************************************
//! ENUM values for FM
#define     MAX_OUTPUT_WAVE_LENGTH	   32768    //!< Indicates that the maximum wave length of the output is 32768 samples
#define     MAX_NUM_OF_TONES		      30    //!< Indicates maximum number of tones for analysis

/*!< Maximum number of tones allowed is 12 for FM VSG */

//!< Indicate if user wants to specify the audio tone by ampllitude or deviation
enum IQV_FM_VSG_AUDIO_TONE_MODE
{
    IQV_FM_AUDIO_TONE_AMPLITUDE,     
    IQV_FM_AUDIO_TONE_DEVIATION
};

//! Indicates FM modulation state
enum IQV_FM_VSG_MODULATION_STATE
{
    IQV_FM_VSG_MODULATION_STATE_OFF,	//!< Indicates that FM modulation state is off
    IQV_FM_VSG_MODULATION_STATE_ON	    //!< Indicates that FM modulation state is on
};
//! Indicates FM RF output
enum IQV_FM_VSG_RF_OUTPUT
{
    IQV_FM_VSG_RF_OUTPUT_OFF,	        //!< Indicates that FM RF output mode is off
    IQV_FM_VSG_RF_OUTPUT_ON	            //!< Indicates that FM RF output mode is on
};
//! Indicates FM VSG stereo modes
enum IQV_FM_VSG_STEREO_MODES
{
    IQV_FM_VSG_STEREO_MODE_OFF,   //!< Indicates that transmission is mono only
    IQV_FM_VSG_STEREO_MODE_ON     //!< Indicates that transmission mode is stero containing L+R, L-R and pilot
};

//! ENUM value for VSG preemphasis mode
enum IQV_FM_VSG_PREEMPHASIS_MODES
{
    IQV_FM_VSG_PREEMPHASIS_NO_PREEMPHASIS,	//!< Indicates that there is no preemphasis
    IQV_FM_VSG_PREEMPHASIS_25_MICRO_SEC,	//!< Indicates preemphasis time constant is 25 micro seconds
    IQV_FM_VSG_PREEMPHASIS_50_MICRO_SEC,	//!< Indicates preemphasis time constant is 50 micro seconds
    IQV_FM_VSG_PREEMPHASIS_75_MICRO_SEC	    //!< Indicates preemphasis time constant is 75 micro seconds
};
//! ENUM value for FM VSG audio channels for each tone
enum IQV_FM_VSG_AUDIO_CHANNELS
{
    IQV_FM_VSG_AUDIO_LEFT,                      //!< Indicates audio signal transmission on left channel
    IQV_FM_VSG_AUDIO_RIGHT,                     //!< Indicates audio signal transmission on right channel
    IQV_FM_VSG_AUDIO_LEFT_RIGHT,                //!< Indicates audio signal transmission on left and right channels with identical frequency and amplitude, but different phase 
    IQV_FM_VSG_AUDIO_LEFT_EQUAL_RIGHT,          //!< Indicates identical audio signal transmission on left and right channel, with identical frequency, amplitude and phase, i.e., L=R
    IQV_FM_VSG_AUDIO_LEFT_EQUAL_NEGATIVE_RIGHT  //!< Indicates audio signal transmission on left and right channel, with identical frequency, amplitude but opposite phase, i.e., L=-R
};
//! ENUM value for interfering carrier modes
enum IQV_FM_VSG_INTERFERING_CARRIER_MODES
{
    IQV_FM_VSG_INTERFERING_CARRIER_OFF,         //!< Indicates interfering carrier mode is off
    IQV_FM_VSG_INTERFERING_CARRIER_ON           //!< Indicates interfering carrier mode is on
};

//! ENUM values for VSG modulation types for the interfering carrier
enum IQV_FM_VSG_MODULATION_TYPES
{
    IQV_FM_VSG_MODULATION_OFF_CW, //!< Indicates VSG frequency modulation is in Continuous Wave (CW) mode
    IQV_FM_VSG_MODULATION_FM      //!< Indicates Frequency Modulation (FM) mode is on
};
//! ENUM values for VSG audio tone index for the main carrier
enum IQV_FM_VSG_AUDIO_TONE_INDEX
{
    IQV_FM_VSG_AUDIO_TONE_INDEX_0,	//!< Indicates that the audio tone index is 0
    IQV_FM_VSG_AUDIO_TONE_INDEX_1,	//!< Indicates that the audio tone index is 1
    IQV_FM_VSG_AUDIO_TONE_INDEX_2,	//!< Indicates that the audio tone index is 2
    IQV_FM_VSG_AUDIO_TONE_INDEX_3,	//!< Indicates that the audio tone index is 3
    IQV_FM_VSG_AUDIO_TONE_INDEX_4,	//!< Indicates that the audio tone index is 4
    IQV_FM_VSG_AUDIO_TONE_INDEX_5,	//!< Indicates that the audio tone index is 5
    IQV_FM_VSG_AUDIO_TONE_INDEX_6,	//!< Indicates that the audio tone index is 6
    IQV_FM_VSG_AUDIO_TONE_INDEX_7,	//!< Indicates that the audio tone index is 7
    IQV_FM_VSG_AUDIO_TONE_INDEX_8,	//!< Indicates that the audio tone index is 8
    IQV_FM_VSG_AUDIO_TONE_INDEX_9,	//!< Indicates that the audio tone index is 9
    IQV_FM_VSG_AUDIO_TONE_INDEX_10,	//!< Indicates that the audio tone index is 10
    IQV_FM_VSG_AUDIO_TONE_INDEX_11	//!< Indicates that the audio tone index is 11
};
//! ENUM values for VSG RDS modes
enum IQV_FM_VSG_RDS_MODES
{
    IQV_FM_VSG_RDS_MODE_OFF,    //!< Indicates RDS mode is off
    IQV_FM_VSG_RDS_MODE_ON      //!< Indicates RDS mode is on
};
//! ENUM values for the VSG RDS phase relationship relative to the 19 KHz pilot
enum IQV_FM_VSG_RDS_PHASE_SHIFT
{
    IQV_FM_VSG_RDS_PHASE_SHIFT_NONE,  //!< Indicates that RDS and 19 KHz pilot are in phase
    IQV_FM_VSG_RDS_PHASE_SHIFT_90     //!< Indicates 90 degree phase shift; indicates RDS and 19 KHz pilot are out of phase
};
//! ENUM values for VSG amplitude distortion type for the main FM carrier
enum IQV_FM_VSG_DISTORTION_TYPES
{
    IQV_FM_VSG_DISTORTION_OFF,    //!< Indicates no amplitude distortion
    IQV_FM_VSG_DISTORTION_AM      //!< Indicates Amplitude Modulation (AM) is on for the main FM signal
};


/// ******************************************************************************
/// FM VSA enum
/// ******************************************************************************
//! FM VSA RF input
enum IQV_FM_VSA_RF_INPUT
{
    IQV_FM_VSA_RF_INPUT_OFF,    //!< Indicates that VSA RF input for FM is off
    IQV_FM_VSA_RF_INPUT_ON      //!< Indicates that VSA RF input for FM is on
};

/// FM VSA Automatic Gain (Amplitude) Control
enum IQV_FM_VSA_AGC_MODES
{
    IQV_FM_VSA_AGC_MODE_OFF,  ///< No automatic gain/amplitude control
    IQV_FM_VSA_AGC_MODE_ON    ///< Automatic gain/amplitude control
};

enum IQV_FM_VSA_SAMPLE_RATES
{
    IQV_FM_VSA_SAMPLE_RATE_128,     //!< VSA sample rate is Fs/128   => 640K Hz
    //  IQV_FM_VSA_SAMPLE_RATE_64,  //!< VSA sample rate is Fs/64    => 1.2 MHz   (Currently not supported)
    //  IQV_FM_VSA_SAMPLE_RATE_32,  //!< VSA sample rate is Fs/32  => 2.5 MHz     (Currently not supported) 
    IQV_FM_VSA_SAMPLE_RATE_16       //!< VSA sample rate is Fs/16    => 5.1 MHz
    //  IQV_FM_VSA_SAMPLE_RATE_1    //!< VSA sample rate is 1  (Not to be used/Calibration) 81 MHz (Currently not supported) 
};


/// ******************************************************************************
/// FM Analysis enum
/// ******************************************************************************
enum IQV_FM_FILTER_TYPE
{
    IQV_NO_FILTER_TYPE,
    IQV_HPF_TYPE,                       //!< if cutoff frequency is 20Hz, slope is 18dB/octave, else it is 12dB/octave
    IQV_LPF_TYPE,                       //!< 30dB/octave 
    IQV_DEEMPHASIS_TYPE,
    IQV_CCITT_P53_TYPE,
    IQV_C_MESSAGE_TYPE,
    IQV_A_WEIGHTING_TYPE,
    IQV_C_WEIGHTING,
    IQV_ITU_R_468_WEIGHTED_TYPE,
    IQV_ITU_R_468_UNWEIGHTED_TYPE       //!< 18dB/octave slope before 22.4Hz, 30dB/octave slope after after 22.4KHz
};

#define RDS_N_BITS_PER_BLOCK_16         16

enum IQV_FM_WINDOW_TYPE
{
    IQV_WIN_TYPE_BLACKMANHARRIS,
    IQV_WIN_TYPE_HANNING,
    IQV_WIN_TYPE_FLATTOP
};

/// ******************************************************************************
/// Multiwave enum
/// ******************************************************************************
enum IQV_MULTI_WAVE_COMPENSATION_SINC
{
    IQV_NO_COMPENSATION_SINC, //!< Indicates compensation sinc is off
    IQV_COMPENSATION_SINC //!< Indicates compensation sinc is on
};

/// ******************************************************************************
/// Zigbee enum
/// ******************************************************************************
enum IQV_ZIGBEE_PH_CORR_ENUM
{
    IQV_ZIGBEE_PH_CORR_SYM_BY_SYM = 2       //!< Indicates symbol-by-symbol correction.
};

enum IQV_ZIGBEE_FREQ_SYNC_ENUM
{
    IQV_ZIGBEE_FREQ_SYNC_FULL_PACKET = 3    //!< Indicates frequency synchronization on full data packet 
};

/// ******************************************************************************
/// NFC enum
/// ******************************************************************************
enum IQV_NFC_ISO_TYPE
{
    IQV_NFC_ISO_18092
//   IQV_NFC_ISO_14443A,
//   IQV_NFC_ISO_14443B
};

enum IQV_NFC_WAVE_SIG_TYPE
{
    IQV_NFC_WAVE_INITIATOR_ACTIVE_MODE = 3,      
    IQV_NFC_WAVE_TARGET_ACTIVE_MODE
};

enum IQV_NFC_BANDWIDTH_KHZ
{
    IQV_NFC_BANDWIDTH_106 = 106,
    IQV_NFC_BANDWIDTH_212 = 212,
    IQV_NFC_BANDWIDTH_424 = 424
};

enum IQV_NFC_MANCHESTER_POLARITY
{
    IQV_NFC_REVERSE_POLARITY,
    IQV_NFC_OBVERSE_POLARITY
};

enum IQV_NFC_ENABLE_QCH_BIAS
{
    IQV_NFC_QCH_BIAS_DISABLE,
    IQV_NFC_QCH_BIAS_ENABLE
};

enum IQV_NFC_USE_PAYLOAD_TYPE
{
    IQV_NFC_PAYLOAD_TYPE_ASCII,
    IQV_NFC_PAYLOAD_TYPE_BINARY_ARRAY
};

enum IQV_NFC_VALID_FLAG
{
    IQV_NFC_NOT_VALID,
    IQV_NFC_VALID
};

enum IQV_NFC_ANALYSIS_SIG_TYPE
{
    IQV_NFC_AUTO_DETECT = -1,     // default
    IQV_NFC_INITIATOR_ACTIVE_MODE = 3,      
    IQV_NFC_TARGET_ACTIVE_MODE
};

enum IQV_NFC_MOD_TYPE
{
    IQV_NFC_MOD_UNKNOWN = 0,      // Default
    IQV_NFC_MOD_MODIFIED_MILLER,
    IQV_NFC_MOD_MANCHESTER
};

enum IQV_NFC_POWER_DETECTION_CONTROL
{
    IQV_NFC_NO_POWER_DETECT = -1,
    IQV_NFC_DO_POWER_DETECT = 0,        // Default
    IQV_NFC_USE_PREVIOUS_POWER_IF_EXIST
};

enum IQV_USER_SIGNAL_AMPLITUDE_TYPE
{
    IQV_USER_SIGNAL_AMPLITUDE_AS_VOLT,            //!< Indicates that user signal amplitude is used as volt in unit. default is volt
    IQV_USER_SIGNAL_AMPLITUDE_AS_AMP_PER_METER    //!< Indicates that user signal amplitude is used as ampere per meter in unit   
};

/// ******************************************************************************
/// GLONASS enum
/// ******************************************************************************
#define IQV_GLONASS_RF_OUTPUT				IQV_GPS_RF_OUTPUT
#define IQV_GLONASS_TRIGGER_STATE			IQV_GPS_TRIGGER_STATE
#define IQV_GLONASS_OPERATION_MODE			IQV_GPS_OPERATION_MODE

enum IQV_GLONASS_FREQUENCY_CHANNEL
{
    IQV_GLONASS_FREQUENCY_CHANNEL_1598062500Hz = -7,	//!< 1598.0625 MHz
	IQV_GLONASS_FREQUENCY_CHANNEL_1598625000Hz,			//!< 1598.6250 MHz
	IQV_GLONASS_FREQUENCY_CHANNEL_1599187500Hz,			//!< 1599.1875 MHz
	IQV_GLONASS_FREQUENCY_CHANNEL_1599750000Hz,			//!< 1599.7500 MHz
	IQV_GLONASS_FREQUENCY_CHANNEL_1600312500Hz,			//!< 1600.3125 MHz
	IQV_GLONASS_FREQUENCY_CHANNEL_1600875000Hz,			//!< 1600.8750 MHz
	IQV_GLONASS_FREQUENCY_CHANNEL_1601437500Hz,			//!< 1601.4375 MHz
	IQV_GLONASS_FREQUENCY_CHANNEL_1602000000Hz,			//!< 1602.0000 MHz
	IQV_GLONASS_FREQUENCY_CHANNEL_1602562500Hz,			//!< 1602.5625 MHz
	IQV_GLONASS_FREQUENCY_CHANNEL_1603125000Hz,			//!< 1603.1250 MHz
	IQV_GLONASS_FREQUENCY_CHANNEL_1603687500Hz,			//!< 1603.6875 MHz
	IQV_GLONASS_FREQUENCY_CHANNEL_1604250000Hz,			//!< 1604.2500 MHz
	IQV_GLONASS_FREQUENCY_CHANNEL_1604812500Hz,			//!< 1604.8125 MHz
	IQV_GLONASS_FREQUENCY_CHANNEL_1605375000Hz			//!< 1605.3750 MHz
};

#endif // _IQAPIDEFINES_H_
