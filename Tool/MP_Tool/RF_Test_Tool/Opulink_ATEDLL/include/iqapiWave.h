/*!
    \file iqapiWave.h
*/
#pragma once
#ifndef _IQAPI_WAVE_H_
#define _IQAPI_WAVE_H_

#include "iqapi.h"

#ifdef WIN32
#pragma region enums and structs
#endif
/// @cond
enum IQV_WAVE_BLUETOOTH_PACKET_TYPES
{
	IQV_WAVE_BLUETOOTH_PACKET_TYPE_DH1,
	IQV_WAVE_BLUETOOTH_PACKET_TYPE_DH3,
	IQV_WAVE_BLUETOOTH_PACKET_TYPE_DH5,
	IQV_WAVE_BLUETOOTH_PACKET_TYPE_2DH1,
	IQV_WAVE_BLUETOOTH_PACKET_TYPE_2DH3,
	IQV_WAVE_BLUETOOTH_PACKET_TYPE_2DH5,
	IQV_WAVE_BLUETOOTH_PACKET_TYPE_3DH1,
	IQV_WAVE_BLUETOOTH_PACKET_TYPE_3DH3,
	IQV_WAVE_BLUETOOTH_PACKET_TYPE_3DH5,
	IQV_WAVE_BLUETOOTH_PACKET_TYPE_LE
};

enum IQV_WAVE_BLUETOOTH_EDR_ROLLOFF_TYPES
{
	IQV_WAVE_BLUETOOTH_EDR_ROLLOFF_030, // 0.3
	IQV_WAVE_BLUETOOTH_EDR_ROLLOFF_035, // 0.35
	IQV_WAVE_BLUETOOTH_EDR_ROLLOFF_040, // 0.4
	IQV_WAVE_BLUETOOTH_EDR_ROLLOFF_045, // 0.45
	IQV_WAVE_BLUETOOTH_EDR_ROLLOFF_050  // 0.5
};

enum IQV_WAVE_BLUETOOTH_PAYLOAD_TYPES
{
	IQV_WAVE_BLUETOOTH_PAYLOAD_TYPE_ALTERNATING_ONE_ZERO,
	IQV_WAVE_BLUETOOTH_PAYLOAD_TYPE_ALL_ONE,
	IQV_WAVE_BLUETOOTH_PAYLOAD_TYPE_ALL_ZERO,
	IQV_WAVE_BLUETOOTH_PAYLOAD_TYPE_FOUR_ONES_FOUR_ZEROES,
	IQV_WAVE_BLUETOOTH_PAYLOAD_TYPE_USER,
	IQV_WAVE_BLUETOOTH_PAYLOAD_TYPE_RANDOM
};

enum IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPES
{
	IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_None,              // clean wave
	IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_1_POS_FREQ_DRIFT,  // 1a
	IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_1_NEG_FREQ_DRIFT,  // 1b
	IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_2_POS_FREQ_DRIFT,  // 2a
	IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_2_NEG_FREQ_DRIFT,  // 2b
	IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_3_POS_FREQ_DRIFT,  // 3a
	IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_3_NEG_FREQ_DRIFT,  // 3b
	IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_4_POS_FREQ_DRIFT,  // 4a
	IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_4_NEG_FREQ_DRIFT,  // 4b
	IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_5_POS_FREQ_DRIFT,  // 5a
	IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_5_NEG_FREQ_DRIFT,  // 5b
	IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_6_POS_FREQ_DRIFT,  // 6a
	IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_6_NEG_FREQ_DRIFT,  // 6b
	IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_7_POS_FREQ_DRIFT,  // 7a
	IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_7_NEG_FREQ_DRIFT,  // 7b
	IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_8_POS_FREQ_DRIFT,  // 8a
	IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_8_NEG_FREQ_DRIFT,  // 8b
	IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_9_POS_FREQ_DRIFT,  // 9a
	IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_9_NEG_FREQ_DRIFT,  // 9b
	IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_10_POS_FREQ_DRIFT, // 10a
	IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_10_NEG_FREQ_DRIFT  // 10b
};

#define IQV_WAVE_BLUETOOTH_LE_DIRTY_PACKET_TYPES                   IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPES
#define IQV_WAVE_BLUETOOTH_LE_DIRTY_PACKET_TYPE_None               IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_None
#define IQV_WAVE_BLUETOOTH_LE_DIRTY_PACKET_TYPE_1_POS_FREQ_DRIFT   IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_1_POS_FREQ_DRIFT
#define IQV_WAVE_BLUETOOTH_LE_DIRTY_PACKET_TYPE_1_NEG_FREQ_DRIFT   IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_1_NEG_FREQ_DRIFT
#define IQV_WAVE_BLUETOOTH_LE_DIRTY_PACKET_TYPE_2_POS_FREQ_DRIFT   IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_2_POS_FREQ_DRIFT
#define IQV_WAVE_BLUETOOTH_LE_DIRTY_PACKET_TYPE_2_NEG_FREQ_DRIFT   IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_2_NEG_FREQ_DRIFT
#define IQV_WAVE_BLUETOOTH_LE_DIRTY_PACKET_TYPE_3_POS_FREQ_DRIFT   IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_3_POS_FREQ_DRIFT
#define IQV_WAVE_BLUETOOTH_LE_DIRTY_PACKET_TYPE_3_NEG_FREQ_DRIFT   IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_3_NEG_FREQ_DRIFT
#define IQV_WAVE_BLUETOOTH_LE_DIRTY_PACKET_TYPE_4_POS_FREQ_DRIFT   IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_4_POS_FREQ_DRIFT
#define IQV_WAVE_BLUETOOTH_LE_DIRTY_PACKET_TYPE_4_NEG_FREQ_DRIFT   IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_4_NEG_FREQ_DRIFT
#define IQV_WAVE_BLUETOOTH_LE_DIRTY_PACKET_TYPE_5_POS_FREQ_DRIFT   IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_5_POS_FREQ_DRIFT
#define IQV_WAVE_BLUETOOTH_LE_DIRTY_PACKET_TYPE_5_NEG_FREQ_DRIFT   IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_5_NEG_FREQ_DRIFT
#define IQV_WAVE_BLUETOOTH_LE_DIRTY_PACKET_TYPE_6_POS_FREQ_DRIFT   IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_6_POS_FREQ_DRIFT
#define IQV_WAVE_BLUETOOTH_LE_DIRTY_PACKET_TYPE_6_NEG_FREQ_DRIFT   IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_6_NEG_FREQ_DRIFT
#define IQV_WAVE_BLUETOOTH_LE_DIRTY_PACKET_TYPE_7_POS_FREQ_DRIFT   IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_7_POS_FREQ_DRIFT
#define IQV_WAVE_BLUETOOTH_LE_DIRTY_PACKET_TYPE_7_NEG_FREQ_DRIFT   IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_7_NEG_FREQ_DRIFT
#define IQV_WAVE_BLUETOOTH_LE_DIRTY_PACKET_TYPE_8_POS_FREQ_DRIFT   IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_8_POS_FREQ_DRIFT
#define IQV_WAVE_BLUETOOTH_LE_DIRTY_PACKET_TYPE_8_NEG_FREQ_DRIFT   IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_8_NEG_FREQ_DRIFT
#define IQV_WAVE_BLUETOOTH_LE_DIRTY_PACKET_TYPE_9_POS_FREQ_DRIFT   IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_9_POS_FREQ_DRIFT
#define IQV_WAVE_BLUETOOTH_LE_DIRTY_PACKET_TYPE_9_NEG_FREQ_DRIFT   IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_9_NEG_FREQ_DRIFT
#define IQV_WAVE_BLUETOOTH_LE_DIRTY_PACKET_TYPE_10_POS_FREQ_DRIFT  IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_10_POS_FREQ_DRIFT
#define IQV_WAVE_BLUETOOTH_LE_DIRTY_PACKET_TYPE_10_NEG_FREQ_DRIFT  IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPE_10_NEG_FREQ_DRIFT

enum IQV_WAVE_WIFI_PSDU_TYPES
{
	IQV_WAVE_WIFI_PSDU_TYPE_RANDOM,
	IQV_WAVE_WIFI_PSDU_TYPE_USER_DEFINED,
	IQV_WAVE_WIFI_PSDU_TYPE_USER_DEFINED_CRC,
	IQV_WAVE_WIFI_PSDU_TYPE_ALL_ZERO,
	IQV_WAVE_WIFI_PSDU_TYPE_ALL_ONE,
	IQV_WAVE_WIFI_PSDU_TYPE_ALT_ZERO_ONE
};

enum IQV_WAVE_WIFI_MAC_FRAME_TYPES
{
	IQV_WAVE_WIFI_MAC_FRAME_TYPE_USER,
	IQV_WAVE_WIFI_MAC_FRAME_TYPE_DATA,
	IQV_WAVE_WIFI_MAC_FRAME_TYPE_ACK,
	IQV_WAVE_WIFI_MAC_FRAME_TYPE_RTS,
	IQV_WAVE_WIFI_MAC_FRAME_TYPE_CTS,
	IQV_WAVE_WIFI_MAC_FRAME_TYPE_BEACON,
	IQV_WAVE_WIFI_MAC_FRAME_TYPE_PROBE_RES
};

enum IQV_WAVE_WIFI_BIT_RATES
{
	IQV_WAVE_WIFI_BIT_RATE_1_MBPS = 1,
	IQV_WAVE_WIFI_BIT_RATE_2_MBPS,
	IQV_WAVE_WIFI_BIT_RATE_5_5_MBPS,
	IQV_WAVE_WIFI_BIT_RATE_11_MBPS
};

enum IQV_WAVE_TYPES
{
	IQV_WAVE_TYPE_WIFI_11_B = 0,
	IQV_WAVE_TYPE_WIFI_11_AG,
	IQV_WAVE_TYPE_WIFI_11_N,
	IQV_WAVE_TYPE_BLUETOOTH = 20,
	IQV_WAVE_TYPE_CW,
	IQV_WAVE_TYPE_INVALID = 99
};

enum IQV_WAVE_WIFI_OFDM_TYPES
{
	IQV_WAVE_WIFI_OFDM_6 = 0,
	IQV_WAVE_WIFI_OFDM_9,
	IQV_WAVE_WIFI_OFDM_12,
	IQV_WAVE_WIFI_OFDM_18,
	IQV_WAVE_WIFI_OFDM_24,
	IQV_WAVE_WIFI_OFDM_36,
	IQV_WAVE_WIFI_OFDM_48,
	IQV_WAVE_WIFI_OFDM_54
};

enum IQV_WAVE_WIFI_MCS_INDEX
{
	IQV_WAVE_WIFI_MCS_0 = 0,
	IQV_WAVE_WIFI_MCS_1,
	IQV_WAVE_WIFI_MCS_2,
	IQV_WAVE_WIFI_MCS_3,
	IQV_WAVE_WIFI_MCS_4,
	IQV_WAVE_WIFI_MCS_5,
	IQV_WAVE_WIFI_MCS_6,
	IQV_WAVE_WIFI_MCS_7,
	IQV_WAVE_WIFI_MCS_8,
	IQV_WAVE_WIFI_MCS_9,
	IQV_WAVE_WIFI_MCS_10,
	IQV_WAVE_WIFI_MCS_11,
	IQV_WAVE_WIFI_MCS_12,
	IQV_WAVE_WIFI_MCS_13,
	IQV_WAVE_WIFI_MCS_14,
	IQV_WAVE_WIFI_MCS_15,
	IQV_WAVE_WIFI_MCS_16,
	IQV_WAVE_WIFI_MCS_17,
	IQV_WAVE_WIFI_MCS_18,
	IQV_WAVE_WIFI_MCS_19,
	IQV_WAVE_WIFI_MCS_20,
	IQV_WAVE_WIFI_MCS_21,
	IQV_WAVE_WIFI_MCS_22,
	IQV_WAVE_WIFI_MCS_23,
	IQV_WAVE_WIFI_MCS_24,
	IQV_WAVE_WIFI_MCS_25,
	IQV_WAVE_WIFI_MCS_26,
	IQV_WAVE_WIFI_MCS_27,
	IQV_WAVE_WIFI_MCS_28,
	IQV_WAVE_WIFI_MCS_29,
	IQV_WAVE_WIFI_MCS_30,
	IQV_WAVE_WIFI_MCS_31
};

enum IQV_WAVE_OFDM_AGGREGATION_TYPES
{
	IQV_WAVE_OFDM_AGGREGATION_OFF = 0,
	IQV_WAVE_OFDM_AGGREGATION_ON,
	IQV_WAVE_OFDM_AGGREGATION_AUTO
};

typedef struct
{
	char frameControl[4];
	char durID[4];
	char seqControl[4];
	char macAddr1[12];
	char macAddr2[12];
	char macAddr3[12];
	char macAddr4[12];
	IQV_WAVE_WIFI_MAC_FRAME_TYPES frameMode;
} MAC_INFO;
/// @endcond
#ifdef WIN32
#pragma endregion
#endif

//! Wave impairment parameter
class IQ_API iqapiWaveImpairmentParam
{
public :
	/// @cond
	iqapiWaveImpairmentParam();
	~iqapiWaveImpairmentParam();
	/// @endcond

	//! Sets default parameter values
	void SetDefault();

	double freqErrorHz;
	double symClockErrorPpm;
	double iqImbalAmplDb[8];
	double iqImbalPhaseDeg[8];
	double dcOffsetIpercent[8];
	double dcOffsetQpercent[8];
	double paBackoffDb[8];
	double smoothFactor[8];
	double pllNoiseDbcHz;
	double pllBWHz;
	double thermalNoiseDbcHz;
	double cutoffFreqHz;
	double txSNR[8];
	double sigBWHz;
};

//! Base class for waveform generation
class IQ_API iqapiWaveBase
{
public :
	/// @cond
	iqapiWaveBase(iqapiHndl *hndl, IQV_WAVE_TYPES type);
	virtual ~iqapiWaveBase();
	/// @endcond

	//! Sets default parameter values
	virtual void SetDefault() = 0;

	//! Generate waveform data based on given parameters
	/*!
	 * \return IQAPI_ERR_OK if no error
	 * \note the result gets stored in iqapiWaveBase::data
	 */
	virtual int Generate() = 0;

	//! Save .mod file
	/*!
	 * \param[in] fileName Path of the .mod file
	 *
	 * \return IQAPI_ERR_OK if no error
	 * \note uses iqapiWaveBase::data(if generated before) to save
	 */
	virtual int Save(const char* fileName);

	//! Adds impairments to waveforms for generic use
	static void AddImpairment(iqapiSignalData *dataInOut, iqapiWaveImpairmentParam *impairParam);

	iqapiCapture *data;            //!< Contains generated waveform data
	const IQV_WAVE_TYPES &type;    //!< Contains wave type
	char lastErr[MAX_LEN_ERR_TXT]; //!< Contains the last error message
	iqapiWaveImpairmentParam *impairment; //!< impairment parameters to use when generating waveforms. Set to NULL(default) to not use impairment
	/// @cond
protected :
	void reallocDataObject(int singleStreamLength, int streamCount = 1);
	void releaseMemory(bool releaseAll = true);
	
	virtual void resetLastError();
	iqapiHndl *iqapiObject;
	IQV_WAVE_TYPES _type;

private :
	iqapiWaveBase();
	iqapiWaveBase(const iqapiWaveBase &src);
    iqapiWaveBase & operator = (const iqapiWaveBase & src);
	/// @endcond
};

//! Bluetooth Waveform Generation class
class IQ_API iqapiWaveBluetooth : public iqapiWaveBase
{
public :
	iqapiWaveBluetooth(iqapiHndl *hndl);  //!< Constructor
	~iqapiWaveBluetooth(); //!< Destructor
	
	void SetDefault(); //!< \see iqapiWaveBase::SetDefault()
	int Generate(); //!< \see iqapiWaveBase::Generate()

	//! Set user-defined payload(applicable only when the payload type is user-defined)
	/*!
	 * \param[in] payloadHex User-defined payload in hexadecimal representation
	 * \param[in] length Length of payload
	 *
	 * \return IQAPI_ERR_OK if no error
	 */
	int SetPayload(const char *payloadHex, int length);

	// public member variables
	IQV_WAVE_BLUETOOTH_PACKET_TYPES packetType;                  //!< Sets Bluetooth packet type
	IQV_WAVE_BLUETOOTH_PAYLOAD_TYPES payloadType;                //!< Sets payload type for non-LE packets
	IQV_WAVE_BLUETOOTH_EDR_ROLLOFF_TYPES edrRollOff;             //!< Sets EDR Rolloff
	/*! Sets dirty packet type, must set packe type first, LE dirty packets are per RCV-LE/CA/01/C, 
	\n  classic packets are per RCV/CA/01/C and RCV/CA/02/C, EDR packets are per RCV/CA/07/C
	\n  (Note EDR only has 3 dirty packet types with positive and negative drift). 
	\n  This sets distortion parameters only, user still needs to set other parameters such as payload, repeatTime in slots.
	*/
	IQV_WAVE_BLUETOOTH_DIRTY_PACKET_TYPES dirtyPacketType;

	double btProduct;                //!< Sets BT product 0.1 to 0.9 (default : 0.5)
	double modulationIndex;          //!< Sets modulation index (default : 0.32) 
	double edrGuardTime;             //!< Sets EDR guard time (default : 5e-6)
	int payloadLengthBits;           //!< Sets payload length in bits, for all packet type (default : 296)
	int repeatTimeInSlots;           //!< Sets repeat time in number of slots (default : 1)
	int startBit;                    //!< Sets start bit in differential coding (default : 0)
	int packetHeader3BitLTADDR;      //!< Sets 3-bit LTADDR in packet header, 0 to 7, where 3=011 (default : 0)
	int packetHeaderARONOnOff;       //!< Sets ARON on(=1)/off(=0) in packet header (default : 0)
	int packetHeaderFLOWOnOff;       //!< Sets FLOW on(=1)/off(=0) in packet header (default : 0)
	int packetHeaderSEQNOnOff;       //!< Sets SEQN on(=1)/off(=0) in packet header (default : 0)
	int packetHeaderOnOff;           //!< Enables (=1) or disables(=0) packet header (default : 1)
	int payloadHeaderFLOWOnOff;      //!< Sets FLOW in payload header on(=1)/off(=0) (default : 1)
	int payloadHeader2BitLLID;       //!< Sets 2-bit LLID in payload header, 0 to 3, where 1=01 (default : 1)
	int enableFrontEndZero;          //!< Enables (=1) or disables (=0) front 10 zero symbols preceding the packet (default 0)
	int extraSymbolsPacketEnd;       //!< Enables (=1) or disables (=0) extra 6 symbols at packet end (default 0)
	char uapHex[3];                  //!< Sets 8-bit UAP in HEX in bluetooth device address (default : "6B")
	char lapHex[7];                  //!< Sets 48-bit LAP in HEX in bluetooth device address (default "C6967E")
	int	userPayloadIncludeHeaderCrc; //!< Used only when payload type is user. 1 if user payload includes header and crc, 0 else (to be generated inside, default : 1)
	int lePayloadType;               //!< Sets LE payload type 0 to 7 for pattern defined in the standard. -1 for user-defined payload(use SetPayload())
	char leSyncWordHex[9];           //!< Sets LE syncword in HEX, leftmost bit transmitted first (default : "94826E8E")
	/*! number of extra transmit signals generated in the waveform (default : 0, max : 3)
	\n These extra signals are in addition to the number of space-time streams (Nsts) determined by the input settings for waveform generation.
	\n This is also referred to as spatial expansion.
	*/
	unsigned int netx;

	// Deprecated variables
	IQV_WAVE_BLUETOOTH_LE_DIRTY_PACKET_TYPES& leDirtyPacketType; //!< Deprecated. Use dirtyPacketType
	
private :
	/// @cond
	iqapiWaveBluetooth();
	iqapiWaveBluetooth(const iqapiWaveBluetooth &src);
    iqapiWaveBluetooth & operator = (const iqapiWaveBluetooth & src);
	void copyToBase();
	void copyFromBase();

	void *baseObject;
	int userPayloadBitsLengthBits;
	/// @endcond
};

//! 802.11b Waveform Generation class
class IQ_API iqapiWave11b : public iqapiWaveBase
{
public :
	iqapiWave11b(iqapiHndl *hndl);  //!< Constructor
	~iqapiWave11b(); //!< Destructor

	void SetDefault(); //!< \see iqapiWaveBase::SetDefault()
	int Generate(); //!< \see iqapiWaveBase::Generate()

	//! Set user-defined PSDU bits(applicable only when the PSDU type is user-defined). To unload, use SetPsduBits(NULL, 0)
	/*!
	 * \param[in] psduBits User-defined bits to use
	 * \param[in] length Length of bits
	 *
	 * \return IQAPI_ERR_OK if no error
	 */
	int SetPsduBits(const char *psduBits, int length);

	// public member variables
	IQV_WAVE_WIFI_BIT_RATES bitRate; //!< bit rate for the packet (default : IQV_WAVE_WIFI_BIT_RATE_11_MBPS)
	IQV_WAVE_WIFI_PSDU_TYPES mode;   //!< PSDU mode RANDOM, ALL0, ALL1, ALT01, USER_DEF (default : IQV_WAVE_WIFI_PSDU_TYPE_RANDOM)
	unsigned int psduLengthOct;      //!< length of PSDU in octets (default : 400, max : 2346)
	unsigned int lockedClock;        //!< whether the clock is locked. (default : 0)
	unsigned int longPreamble;       //!< 1 - long preamble, 0 - short preamble (default : 1)
	unsigned int spreadingOff;       //!< 0 - no spreading, 1 - spreading on (default : 0)
	unsigned int preambleDis;        //!< 1 - disable preamble, 0 - enable preamble (default : 0)
	unsigned int samplingRate;       //!< sampling rate (default : 80e6)
	/*! number of extra transmit signals generated in the waveform (default : 0, max : 3)
	\n These extra signals are in addition to the number of space-time streams (Nsts) determined by the input settings for waveform generation.
	\n This is also referred to as spatial expansion.
	*/
	unsigned int netx;
	unsigned int macEnable;          //!< add mac header to the PSDU (default : 1)
	unsigned int crcEnable;          //!< add crc to the psdu (default : 1)
	int scramblerInit;               //!< 0 - 127 number (default : 0)
	/*! idle time in microsecond (range : 1uS ~ 1s)
	\n >=11us: 10 us idle time will be in the beginning, and rest will be at the end
    \n <11us, >=1us: 1us will be at the end, rest will be at the beginning
	\n <1us, >=0: all will be at the end, 0us at the beginning
	\n <0: invalid
	*/
	double idleTimeuS;
	MAC_INFO macParams;              //!< structure for mac params
	
private :
	/// @cond
	iqapiWave11b();
	iqapiWave11b(const iqapiWave11b &src);
    iqapiWave11b & operator = (const iqapiWave11b &src);
	void copyToBase();
	void copyFromBase();

	void *baseObjectParam;
	unsigned int modType;
	int userPsduBitsLengthBits;
	/// @endcond
};

//! Base class for OFDM-based waveform generation classes
class IQ_API iqapiWaveOfdmBase : public iqapiWaveBase
{
public :
	iqapiWaveOfdmBase(iqapiHndl *hndl, IQV_WAVE_TYPES type);  //!< Constructor
	~iqapiWaveOfdmBase(); //!< Destructor

	virtual void SetDefault(); //!< \see iqapiWaveBase::SetDefault()
	virtual int Generate(); //!< \see iqapiWaveBase::Generate()

	//! Set the spatial map matrix. To unload current spatial map matrix and use the default value instead, use SetSpatialMapMatrix(NULL).
	/*!
	 * \param[in] spatialMap Spatial map matrix to set
	 * \param[in] length Length of spatialMap
	 *
	 * \return IQAPI_ERR_OK if no error
	 */
	int SetSpatialMapMatrix(iqapiMeasurement *spatialMap = NULL);

	//! Set user-defined PSDU bits(applicable only when the PSDU type is user-defined). To unload, use SetPsduBits(NULL, 0)
	/*!
	 * \param[in] psduBits User-defined bits to use
	 * \param[in] length Length of bits
	 *
	 * \return IQAPI_ERR_OK if no error
	 */
	int SetPsduBits(const char *psduBits, int length);

	//! Set MPDU. To unload current spatial map matrix and use the default value instead, use SetMpdu(NULL, 0)
	/*!
	 * \param[in] mpdu MPDU
	 * \param[in] length Length of mpdu
	 *
	 * \return IQAPI_ERR_OK if no error
	 */
	int SetMpdu(const double *mpdu, double length);

	// public member variables
	IQV_WAVE_WIFI_PSDU_TYPES mode; //!< PSDU mode RANDOM, ALL0, ALL1, ALT01, USER_DEF (default : IQV_WAVE_WIFI_PSDU_TYPE_RANDOM)
	MAC_INFO macParams;            //!< structure for mac params
	bool macEnable;                //!< add mac header to the PSDU (default : true)
	bool crcEnable;                //!< add crc to the psdu (default : true)
	int scramblerInit;             //!< Number used for scrambler init 0-127 (default : 12)
	/*! idle time in microsecond (range : 1uS ~ 1s)
	\n >=11us: 10 us idle time will be in the beginning, and rest will be at the end
    \n <11us, >=1us: 1us will be at the end, rest will be at the beginning
	\n <1us, >=0: all will be at the end, 0us at the beginning
	\n <0: invalid
	 */
	double idleTimeuS;
	double psduLengthOct; //!< psdu length in octets
	/*! number of extra transmit signals generated in the waveform (default : 0, max : 3)
	\n These extra signals are in addition to the number of space-time streams (Nsts) determined by the input settings for waveform generation. 
	\n This is also referred to as spatial expansion. For 802.11n/ac and Netx > 0, a spatial mapping matrix needs to be defined with Nsts+Netx
	\n rows and Nsts columns. By default the parameter is set to zero.
	\n For example, for 802.11n , MCS = 10, netx = 0, the total number of signals is 2 since MCS 10 has 2 streams. For the same settings, if 
	\n netx = 1, the total number of signals is 3 and a 3x2 spatial mapping matrix needs to be defined to describe the mapping from streams to 
	\n transmit signals (spatial expansion). For 802.11a/g, the single-stream signal gets directly expanded onto Netx additional signals 
	\n (spatial mapping matrix does not apply).
	\n The total number of signals for any combination cannot exceed 4.
	*/
	double netx;

protected :
	/// @cond
	iqapiWaveOfdmBase();
	iqapiWaveOfdmBase(const iqapiWaveOfdmBase &src);
    iqapiWaveOfdmBase & operator = (const iqapiWaveOfdmBase &src);
	void copyToBase();
	void copyFromBase();

	int userPsduBitsLengthBits;
	void *baseObjectPsdu;
	void *baseObjectParam;

	IQV_WAVE_WIFI_MCS_INDEX _mcsIndex;
	IQV_WAVE_WIFI_OFDM_TYPES _ofdm;
	int _bandwidth;
	int _stbc;
	int _numESS;
	int _numSS;
	int _soundingPacket;
	IQV_WAVE_OFDM_AGGREGATION_TYPES _aggregation;
	int _advancedCoding;
	int _shortGI;
	bool _greenField;
	/// @endcond
};

//! 802.11a/g Waveform Generation class
class IQ_API iqapiWave11ag : public iqapiWaveOfdmBase
{
public :
	iqapiWave11ag(iqapiHndl *hndl);  //!< Constructor
	~iqapiWave11ag(); //!< Destructor

	void SetDefault(); //!< \see iqapiWaveBase::SetDefault()

	IQV_WAVE_WIFI_OFDM_TYPES& ofdm; //!< OFDM types (default : IQV_WAVE_WIFI_OFDM_6)

private :
	/// @cond
	iqapiWave11ag();
	iqapiWave11ag(const iqapiWave11ag &src);
    iqapiWave11ag & operator = (const iqapiWave11ag &src);
	/// @endcond
};

//! 802.11n Waveform Generation class
class IQ_API iqapiWave11n : public iqapiWaveOfdmBase
{
public :
	iqapiWave11n(iqapiHndl *hndl);  //!< Constructor
	~iqapiWave11n(); //!< Destructor

	void SetDefault(); //!< \see iqapiWaveBase::SetDefault()

	IQV_WAVE_WIFI_MCS_INDEX& mcsIndex;            //!< MCS Index (default : 7)
	IQV_WAVE_OFDM_AGGREGATION_TYPES& aggregation; //!< Aggregation (default : IQV_WAVE_OFDM_AGGREGATION_AUTO)

	int& bandwidth;      //!< BW for signal 0 (20 MHz), 1 (40 MHz), 2(80 MHz), 3(160 MHz) (default : 0)
	int& stbc;           //!< Space time block coding 0 or 1 (default : 0)
	int& numESS;         //!< Number of expansion signal Streams (default : 0)
	int& soundingPacket; //!< Sounding packet 0 (OFF), 1 (ON) (default : 0)
	int& advancedCoding; //!< Coding Scheme 0 (BCC) 1 (LDPC) (default : 0)
	int& shortGI;        //!< Short Guard Interval 1 (Short Guard) 0 (Long Guard) (default : 0)
	bool& greenField;    //!< greenfield ON (1)/OFF (0) - Applies to 802.11n (default : false)

private :
	/// @cond
	iqapiWave11n();
	iqapiWave11n(const iqapiWave11n &src);
    iqapiWave11n & operator = (const iqapiWave11n &src);
	/// @endcond
};

//! Continuous Wave(CW) Waveform Generation class
class IQ_API iqapiWaveContinuousWave : public iqapiWaveBase
{
public :
	//! Tone Parameters class
	class IQ_API toneParams
	{
	public :
		toneParams(); //!< Constructor
		~toneParams(); //!< Destructor
		unsigned int freqOffsetHz; //!< Frequency offset for the tone
		int gainDb; //!< Gain to be applied to the tone
	};

	iqapiWaveContinuousWave(iqapiHndl *hndl);  //!< Constructor
	~iqapiWaveContinuousWave(); //!< Destructor
	
	void SetDefault(); //!< \see iqapiWaveBase::SetDefault()
	int Generate(); //!< \see iqapiWaveBase::Generate()

	//! Set number of tones and parameters for each tone
	/*!
	 * \param[in] parameter parameters for each tone
	 * \param[in] count number of tones
	 *
	 * \return IQAPI_ERR_OK if no error
	 */
	int SetToneParameters(toneParams *parameter, int count);

	unsigned int samplingRate;         //!< Sampling rate (default : 80e6)
	bool normalizeGain;                //!< Glag to specify whether the gain for all the tones should be normalized to 1 (default : true)
	const int& numTones;               //!< Number of tones (use SetToneParameters() to change)
	toneParams* const& toneParameters; //!< Parameters for each tone (use SetToneParameters() to change its size)
	/*! number of extra transmit signals generated in the waveform (default : 0, max : 3)
	\n These extra signals are in addition to the number of space-time streams (Nsts) determined by the input settings for waveform generation.
	\n This is also referred to as spatial expansion.
	*/
	unsigned int netx;

private :
	/// @cond
	iqapiWaveContinuousWave();
	iqapiWaveContinuousWave(const iqapiWaveBluetooth &src);
    iqapiWaveContinuousWave & operator = (const iqapiWaveBluetooth & src);

	int _numTones;
	toneParams *_toneParameters;
	/// @endcond
};
#endif // _IQAPI_WAVE_H_
