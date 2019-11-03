/*! \mainpage IQmeasure Wrapper Library Documentation
 *
 * The IQmeasure Wrapper Library is the wrapper of "IQmeausre_IQapi_Legacy" and "IQmeasure_SCPI".
 * The IQmeasure Wrapper Library brings ease of use to programming LitePoint wireless test instruments,
 * including IQ2010, IQxel/IQxel-M/IQxel-MW, IQnfc, and IQlink.
 *
 * A number of high-level functions provided by the library greatly reduce the steep learning
 * curve associated with using the direct SCPI commands or lower level API.  The code below shows how easily an EVM measurement
 * for OFDM-54 can be done:
 *
 * \code
  void main(void)
  {
      LP_Init(IQTYPE_XEL, 1);
      LP_InitTester("192.168.100.254");
      LP_SetVsa(2412e6, 18, PORT_LEFT, 0, -25);
      LP_VsaDataCapture(200);
      LP_Analyze80211ag();
      double evm = LP_GetScalarMeasurement("evmAll");
      printf("EVM for OFDM-54 = %.2f\n", evm);
  }
 * \endcode
 *
 * Please see below the functions provided by The IQmeasure Wrapper Library:
 * -# Instrument Control Functions:
 *   - LP_Init();
 *   - LP_Term();
 *   - LP_InitTester();
 *   - LP_InitTester2();
 *   - LP_InitTester3();
 *   - LP_InitTester4();
 *   - LP_InitTesterN();
 *   - LP_SetTesterMode();
 *   - LP_SetTesterBank();
 *   - LP_GetVersion();
 *   - LP_GetSessionID();
 *   - LP_SetConcurrency();
 *   - LP_GetAlcMode();
 *   - LP_ConClose();
 *   - LP_ConOpen();
 *   - LP_CheckLicense();
 *   - LP_MptaAvailable();
 *   - LP_DualHead_ConClose();
 *   - LP_IQXEL160_DualHead_ConOpen();
 *   - LP_DualHead_ConOpen();
 *   - LP_DualHead_GetTokenID();
 *   - LP_DualHead_ObtainControl();
 *   - LP_DualHead_ReleaseControl();
 *   - LP_DualHead_TokenReset();
 *   - LP_IQ2010EXT_Init();
 *   - LP_IQ2010EXT_Terminate();
 *   - LP_TerminateNfc();
 *   - LP_Async_Init();
 *   - LP_Async_Initialize();
 *   - LP_Async_Term();
 *   - LP_Async_InitTxRxHandler();
 *   - LP_Async_GetTesterLock();
 *   - LP_Async_ReleaseTesterLock();
 *   - LP_Async_RequestTX();
 *   - LP_Async_ReleaseTX();
 *   - LP_Async_RequestRX();
 *   - LP_Async_ReleaseRX();
 *   - LP_Async_RequestTXRX();
 *   - LP_Async_ReleaseTXRX();
 *   - LP_Async_GetIsTesterLock();
 *   - LP_Async_GetIsReqTXLock();
 *   - LP_Async_GetIsReqRXLock();
 *   - LP_Async_GetIsReqTXRXLock();
 *   - LP_NFC_CheckCoilType();
 *   - LP_LockTesterDebug();
 *   - LP_IQLINK_Associate();
 *   - LP_IQLINK_Disassociate();
 *   - LP_IQLINK_Connect_Socket_Server();
 *   - LP_DisconnectRemoteTester();
 *   - LP_SetSocketPort();
 *   - LP_SetSocketTimeout();
 * -# Configuration Functions:
 *   - LP_Agc();
 *   - LP_SetVsg();
 *   - LP_SetVsg_Compensation();
 *   - LP_SetVsg_GapPower();
 *   - LP_SetVsg_PowerLevel();
 *   - LP_SetVsg_triggerType();
 *   - LP_SetVsgCw();
 *   - LP_SetVsgCwNxN();
 *   - LP_TransmitCw();
 *   - LP_TransmitCwNxN();
 *   - LP_SetVsgModulation();
 *   - LP_SetVsgModulation_MultiWaves();
 *   - LP_SetVsgModulation_SetPlayCondition();
 *   - LP_SetVsgNxN();
 *   - LP_SetVsgTriggerType();
 *   - LP_SetVsg_Trigger();
 *   - LP_SetVsg_MultiPort();
 *   - LP_SetVsa();
 *   - LP_SetVsaAmplitudeTolerance();
 *   - LP_SetVsaBluetooth();
 *   - LP_SetVsaDECT();
 *   - LP_SetVsaBluetooth_BTShiftHz();
 *   - LP_SetVsaNxN();
 *   - LP_SetVsaTriggerTimeout();
 *   - LP_SetVsa_Trigger();
 *   - LP_SetVsaVsg_SamePort();
 *   - LP_SetMarkerTrigger();
 *   - LP_SetPathLossTable();
 *   - LP_SetLpcPath();
 *   - LP_SetFrameCnt();
 *   - LP_SetAnalysisParameterInteger();
 *   - LP_SetAnalysisParameterIntegerArray();
 *   - LP_SetAgcWindowTime();
 *   - LP_SetAlcMode();
 *   - LP_SetCaptureData();
 *   - LP_SetDefault();
 *   - LP_SetPathLossTable();
 *   - LP_SetDefaultNfc();
 *   - LP_SetIQmeasureUseInsDrv();
 *   - LP_SetNfcTxAmplitude();
 *   - LP_SetTxNfc();
 *   - LP_SetRxNfc();
 *   - LP_SetTriggerMaskNfc();
 *   - LP_SetUserDataPtr();
 *   - LP_SaveVsaSignalFile();
 *   - LP_SaveVsaSignalFileText();
 *   - LP_SaveSignalArrays();
 *   - LP_SaveTruncateCapture();
 *   - LP_SaveUserDataToSigFile();
 *   - LP_SaveVsaGeneratorFile();
 *   - LP_SaveFileToLocal();
 *   - LP_SaveIQDataToModulationFile();
 *   - LP_StartIQmeasureTimer();
 *   - LP_StopIQmeasureTimer();
 *   - LP_ScpiCommandQuery();
 *   - LP_ScpiCommandSet();
 *   - LP_ScpiCommandSetInBinary();
 *   - LP_SelectCaptureRangeForAnalysis();
 *   - LP_TxDone();
 *   - LP_LoadUserData();
 *   - LP_LoadVsaSignalFile();
 *   - LP_EnableVsgRF();
 *   - LP_EnableVsgRFNxN();
 *   - LP_EnableSpecifiedVsaRF();
 *   - LP_EnableSpecifiedVsgRF();
 *   - LP_EnableMultiThread();
 *   - LP_FastCalGetPowerData();
 *   - LP_FastCalMeasPower();
 *   - LP_FastCalMeasPowerall();
 *   - LP_MptaDisable();
 *   - LP_MptaEnable();
 *   - LP_MptaSetupCapture();
 *   - LP_Plot();
 *   - LP_PlotDataCapture();
 *   - LP_FM_SetVsa();
 *   - LP_FM_SetVsg();
 *   - LP_FM_SetVsgDistortion();
 *   - LP_FM_SetCarrierPower();
 *   - LP_FM_SetFrequency();
 *   - LP_FM_StartVsg();
 *   - LP_FM_StartVsgInterference();
 *   - LP_FM_StopAudioPlay();
 *   - LP_FM_StopVsg();
 *   - LP_FM_StopVsgInterference();
 *   - LP_FM_LoadAudioCapture();
 *   - LP_FM_DeleteAudioTones();
 *   - LP_FM_SetAudioSingleTone();
 *   - LP_FM_SetAudioToneArray();
 *   - LP_FM_SetAudioToneArrayDeviation();
 *   - LP_FM_SetAudioToneModeAmplitude();
 *   - LP_FM_AudioStimulusGenerateAndPlaySingleTone();
 *   - LP_FM_AudioStimulusGenerateAndPlayMultiTone();
 *   - LP_Glonass_SetActivate();
 *   - LP_Glonass_SetDeactivate();
 *   - LP_Glonass_SetFrequency();
 *   - LP_Glonass_SetOperationMode();
 *   - LP_Glonass_SetPower();
 *   - LP_Glonass_SetRfOutput();
 *   - LP_PrintGlonassParameters();
 *   - LP_GPS_CaculateTesterPower();
 *   - LP_GPS_ContinueWave();
 *   - LP_GPS_LoadScenarioFile();
 *   - LP_GPS_ModulatedMode();
 *   - LP_GPS_PlayScenarioFile();
 *   - LP_GPS_SetActivate();
 *   - LP_GPS_SetChannelInfo();
 *   - LP_GPS_SetData();
 *   - LP_GPS_SetDeactivate();
 *   - LP_GPS_IQXEL_LoadScenarioFile();
 *   - LP_GPS_IQXEL_PlayScenarioFile();
 *   - LP_GPS_ModulationMode();
 *   - LP_COMPASS_CaculateTesterPower();
 *   - LP_COMPASS_ContinueWave();
 *   - LP_COMPASS_LoadScenarioFile();
 *   - LP_COMPASS_ModulatedMode();
 *   - LP_COMPASS_PlayScenarioFile();
 *   - LP_COMPASS_SetActivate();
 *   - LP_COMPASS_SetChannelInfo();
 *   - LP_COMPASS_SetData();
 *   - LP_COMPASS_SetDeactivate();
 *   - LP_InitiateNfc();
 *   - LP_IQ2010EXT_AddMultiAnalysisCapture();
 *   - LP_IQ2010EXT_AddWaveform();
 *   - LP_IQ2010EXT_InitiateMultiAnalysisCapture();
 *   - LP_IQ2010EXT_InitiateMultiCapture();
 *   - LP_IQ2010EXT_InitiateMultiCaptureHT40();
 *   - LP_IQ2010EXT_NewMultiSegmentWaveform();
 *   - LP_IQXELEXT_AddFastRxPer();
 *   - LP_IQXELEXT_AddMultiCalc();
 *   - LP_IQXELEXT_Execute();
 *   - LP_IQXELEXT_FinishSequence();
 *   - LP_IQXELEXT_InitiateMultiAnalysis();
 *   - LP_IQXELEXT_LoadWaveformFile();
 *   - LP_IQXELEXT_NewSequence();
 *   - LP_IQXELEXT_SetMeasurementParam_11AG();
 *   - LP_IQXELEXT_SetMeasurementParam_11B();
 *   - LP_IQXELEXT_SetMeasurementParam_11NAC();
 *   - LP_PwrCalFastGetElement();
 *   - LP_PwrCalFreeElement();
 *   - LP_ReportTimerDurations();
 *   - LP_Segms_SaveVsaSignalFile();
 *   - LP_NFC_CheckCoilType();
 *   - LP_NFC_Setup_InitiatorWaveformAnalysis();
 *   - LP_NFC_Setup_TargetWaveformAnalysis();
 *   - LP_NFC_SetTRxCommandType();
 *   - LP_CheckDllLoaded();
 *   - LP_ChamberCal();
 *   - LP_ChamberCalLoadPathLoss();
 *   - LP_IQBeam_CheckStatus();
 *   - LP_IQBeam_SetMode();
 *	 - LP_IQSwitch_SetMode();
 *   - LP_IQSwitch_ClearMode();
 *   - LP_IQLINK_EnableRfPort();
 *   - LP_IQLINK_SetTcp();
 *   - LP_IQLINK_SetUdp();
 *   - LP_IQLINK_SetAp();
 *   - LP_IQLINK_Tx_Power_Run();
 *   - LP_IQLINK_Rx_Per_Init();
 *   - LP_IQLINK_Rx_Per_Start();
 *   - LP_IQLINK_Rx_Per_Stop();
 *   - LP_IQLINK_Rx_Fast_Per_Start();
 *   - LP_IQLINK_OverwriteRxPerSettings();
 *   - LP_IQLINK_SetFastPer();
 *   - LP_IQLINK_SetPathLossTable();
 *   - LP_IQLINK_SetIperf();
 *   - LP_IQLINK_GetSSID();
 *   - LP_IQLINK_SetDutApRole();
 *   - LP_IQLINK_CheckDutConnection();
 *   - LP_WiSUN_FSK_SetSFDType();
 *   - LP_UploadBinaryFile();
 *   - LP_ConfigureSMU_Voltage();
 *   - LP_SetSsbiGenerator();
 *   - LP_SSBI_Query();
 *   - LP_SetRefGenerator();
 *   - LP_SetPatternFile();
 *   - LP_GetReadingSaveToFile();
 *   - LP_SMUCapture();
 * -# Data Capture Functions:
 *   - LP_VsaDataCapture();
 *   - LP_VsaDataCaptureSingleMimoStream();
 *   - LP_ContCapture();
 *   - LP_CopyVsaCaptureData();
 *   - LP_FM_AudioDataCapture();
 *   - LP_FM_VsaDataCapture();
 *   - LP_IQ2010EXT_NewMultiAnalysisCapture();
 *   - LP_IQ2010EXT_FinalizeMultiSegmentWaveform();
 *   - LP_IQ2010EXT_FinishMultiAnalysisCapture();
 *   - LP_IQ2010EXT_FinishMultiCapture();
 *   - LP_IQXELEXT_WaitForCaptureDone();
 *   - LP_Segms_VsaDataCapture();
 *   - LP_NFC_InitProtocolCapture();
 *   - LP_NFC_InitCapture();
 *   - LP_NFC_FreqSweepCal();
 *   - LP_NFC_FreqSweep();
 *   - LP_NFC_SaveCalRefFile();
 *   - LP_NFC_LoadCalRefFile();
 * -# Analysis Functions:
 *   - LP_SelectCaptureRangeForPreamPowerAnalysis();
 *   - LP_Analyze80216d();
 *   - LP_Analyze80216e();
 *   - LP_Analyze80211b();
 *   - LP_Analyze80211ag();
 *   - LP_Analyze80211p();
 *   - LP_Analyze80211ah();
 *   - LP_Analyze80211n();
 *   - LP_Analyze80211n_Seq();
 *   - LP_Analyze80211ac();
 *   - LP_AnalyzeBluetooth();
 *   - LP_AnalyzeZigbee();
 *   - LP_AnalyzeWiSUN();
 *   - LP_AnalyzeZwave();
 *   - LP_AnalyzeDECT();
 *   - LP_AnalysisNfc();
 *   - LP_FM_Analyze_Demod();
 *   - LP_FM_Analyze_Audio_Mono();
 *   - LP_FM_Analyze_Audio_Stereo();
 *   - LP_FM_Analyze_RF();
 *   - LP_AnalyzeTechnologyPower();
 *   - LP_AnalyzePower();
 *   - LP_AnalyzePowerRamp80211b();
 *   - LP_AnalyzePowerRampOFDM();
 *   - LP_AnalyzeFFT();
 *   - LP_AnalyzeTechnologyFFT();
 *   - LP_AnalyzeCCDF();
 *   - LP_AnalyzeCW();
 *   - LP_AnalyzeCWFreq();
 *   - LP_AnalysisWave();
 *   - LP_AnalyzeHT40Mask();
 *   - LP_AnalyzeVHT80Mask();
 *   - LP_AnalyzeVHT160Mask();
 *   - LP_AnalyzeObw();
 *   - LP_AnalyzeMimo();
 *   - LP_AnalyzeSidelobe();
 *   - LP_AnalyzeGPRFSpectrum();
 *   - LP_AnalyzeGPRFPowerbyFFT();
 *   - LP_IQ2010EXT_Analyze80211ag();
 *   - LP_IQ2010EXT_Analyze80211b();
 *   - LP_IQ2010EXT_Analyze80211nComposite();
 *   - LP_IQ2010EXT_Analyze80211nSISO();
 *   - LP_IQ2010EXT_AnalyzeBluetooth();
 *   - LP_IQ2010EXT_AnalyzeFFT();
 *   - LP_IQ2010EXT_AnalyzeMaskMeasurement();
 *   - LP_IQ2010EXT_AnalyzePower();
 *   - LP_IQ2010EXT_AnalyzePowerRamp80211b();
 *   - LP_IQ2010EXT_AnalyzeSpectralMeasurement();
 *   - LP_IQ2010EXT_RxPer();
 *   - LP_Segms_Analyze80211ac();
 *   - LP_Segms_Analyze80211ag();
 *   - LP_Segms_Analyze80211n();
 *   - LP_Segms_Analyze80211b();
 *   - LP_Segms_AnalyzePower();
 *   - LP_Segms_AnalyzeFFT();
 *   - LP_Segms_AnalyzeHT40Mask();
 *   - LP_Segms_AnalyzeVHT80Mask();
 *   - LP_Segms_AnalyzeTechnologyFFT();
 *   - LP_Segms_AnalyzeBluetooth();
 *   - LP_Segms_Analyze80211p();
 *   - LP_Segms_AnalyzeGPRFPowerbyFFT();
 *   - LP_Segms_AnalyzeGPRFSpectrum();
 *   - LP_NFC_AnalyzeSignal();
 * -# Result Retrieving Functions:
 *   - LP_GetTesterType();
 *   - LP_GetTesterAttribute();
 *   - LP_IQXEL_GetTesterType();
 *   - LP_GetScalarMeasurement();
 *   - LP_GetStringMeasurement();
 *   - LP_GetVectorMeasurement();
 *   - LP_Segms_GetVectorChannelEst();
 *   - LP_GetCapture();
 *   - LP_GetSampleData();
 *   - LP_GetHndlDataPointers();
 *   - LP_GetIQapiHndl();
 *   - LP_GetAgcWindowTime();
 *   - LP_FM_GetVsaSettings();
 *   - LP_FM_GetVsgSettings();
 *   - LP_FM_GetAudioToneArray();
 *   - LP_FM_GetScalarMeasurement();
 *   - LP_FM_GetVectorMeasurement();
 *   - LP_FM_GetVsgDistortion();
 *   - LP_FM_GetVsgInterferenceSettings();
 *   - LP_GPS_GetChannelInfo();
 *   - LP_IQ2010EXT_GetDoubleMeasurements();
 *   - LP_IQ2010EXT_GetIntMeasurement();
 *   - LP_IQ2010EXT_GetNumberOfMeasurementElements();
 *   - LP_IQ2010EXT_GetNumberOfPerPowerLevels();
 *   - LP_IQ2010EXT_GetStringMeasurement();
 *   - LP_IQ2010EXT_GetVectorMeasurement();
 *   - LP_IQXELEXT_GetDoubleMeasurements();
 *   - LP_IQXELEXT_GetIntMeasurement();
 *   - LP_IQXELEXT_GetNumberOfMeasurementElements();
 *   - LP_IQXELEXT_GetNumberOfPerPowerLevels();
 *   - LP_IQXELEXT_GetStringMeasurement();
 *   - LP_IQXELEXT_GetVectorMeasurement();
 *   - LP_Segms_GetScalarMeasurement();
 *   - LP_Segms_GetVectorMeasurement();
 *   - LP_GetVectorChannelEst();
 *   - LP_Segms_GetVectorChannelEst();
 * -# Error Handling Functions:
 *   - LP_GetErrorString();
 *   - LP_GetIQapiHndlLastErrMsg();
 *   - LP_Async_PrintTimeStamp();
 *   - LP_Async_printErrAll();
 * -# Customized Special Functions:
 *   - LP_AutoPLCalibration();
 *   - LP_AutoPLCalibration_FourPoint();
 *   - LP_FixtureHealthCheckRef();
 *   - LP_FixtureHealthCheckTest();
 *   - LP_DUTSense();
 *   - LP_UpdateOSLCali();
 *   - LP_ReturnLossTesterPort();
 *   - LP_ReturnLossFixtureRFProbe();
 *   - LP_IQXEL_FastPer_Sweep();
 *   - LP_IQXEL_MultiCaptureEVMAver();
 *   - LP_IQXEL_MultiCapturePowerAver();
 *   - LP_IQXEL_MeasureSpur();
 *
 * \section revision_sec Revision History
 *
 * \subsection revision_3_1_1 3.1.1
 * Date: Jan 18, 2016 
 * -# IQ2010 IQapi  Version:1.9.0.5
 *
 *
 */

/*! \file IQmeasure.h
 * \brief IQmeasure Library Interface functions
 */

/*! @defgroup group1 How to make an accurate measurement?
 * \image html AmplSweepWithIdealSignal.gif "Sweeping ampl Setting with Ideal Signal" width=300
 */

/*! @defgroup group2 How to re-use the capture for multiple analysis?
 *
 */

/*! @defgroup group3 How to transmit a waveform continuously?
 *
 */

/*! @defgroup group4 What is Packet Engine?
 *
 */

#ifndef _IQMEASURE_H_
#define _IQMEASURE_H_


#ifdef IQMEASURE_EXPORTS
	#define IQMEASURE_API extern "C" __declspec(dllexport)
#else
	#define IQMEASURE_API extern "C" __declspec(dllimport)
#endif

#include <string>
#include "iqapiDefines.h"

#define IQLegacy_Dll	"IQmeasure_IQapi_Legacy.dll"
//#define IQXel_Dll		"IQmeasure_IQapi_SCPI.dll"
#define IQXel_Dll		"IQmeasure_SCPI.dll"

//#define IQMeasureVersion                "3.0.0 (08-03-2012)"

#define MAX_MPTA_PORTS			4
#define  MAX_MODULE_NUM   8
#define VERY_LOW_VSG_POWER_DBM	-110
#define Pi 3.14159
#define MAX_MIMO_TESTER_NUM		4
#ifndef MAX_BUFFER_LEN
#define MAX_BUFFER_LEN 4096
#endif

#ifndef ON
#define ON 1
#endif

#ifndef OFF
#define OFF 0
#endif

#define IQMEASURE_IQAPI_CONTROL_ENABLE  1                   // Turn ON(1)/OFF(0) IQapi tester control method, in code compile
#define IQMEASURE_SCPI_CONTROL_ENABLE   1                   // Turn ON(1)/OFF(0) SCPI  tester control method, in code compile


#define  TESTER_USER_FOLDER			"\\user"
#define  TESTER_ROOT_FOLDER			"\\"

#define  NO_ACK_DEFAULTPOWER -9999 //Default no ack power value for APT use, by Yipeng Qu 11/27/2013

#define DATA_RATE_AUTO 255 // for IQlink auto data rate selection.

enum IQTESTER_TYPE
{
	IQTYPE_2010,						/*!< IQ2010 tester  */
	IQTYPE_XEL,							/*!< IQxel tester  */
	IQTYPE_NFC,							/*!< IQnfc tester  */
	IQTYPE_XEL_M,						/*!< IQxel-M tester  */
    IQTYPE_XEL_M8B,                     /*!< IQxel-M8B tester */
    IQTYPE_XEL_M16,                     /*!< IQxel-M16 tester */
	IQTYPE_XEL_M16_IQBEAM,              /*!< IQxel-M16+IQBeam tester */
	IQTYPE_XEL_M6,						/*!< IQxel-M6 tester */
	IQTYPE_XEL_M8,						/*!< IQxel-M8 tester */
	IQTYPE_XEL_80,						/*!< IQxel-80 tester */
	IQTYPE_XEL_160,						/*!< IQxel-160 tester */
	IQTYPE_XEL_280,						/*!< IQxel-280 tester */
	IQTYPE_Z,                           /*!< zSeries tester */
	IQTYPE_LINK,						/*!< IQlink tester */
	IQTYPE_NFC_PLUS,                    /*!< IQnfc tester  */
	IQTYPE_XEL_M8W,						/*!< IQxel-M8W tester */
	IQTYPE_XEL_M16W,					/*!< IQxel-M16W tester */
	IQTYPE_XEL_M2W ,					/*!< IQxel-M16W tester */
    IQTYPE_GIG_RF,                      /*!< IQgig tester */
	IQTYPE_XSTREAM,						/*!< IQxstream tester */
	IQTYPE_XSTREAM_M,			    	/*!< IQxstream-> tester */
};

typedef struct
{
	char IQmeasure[MAX_RESPONSE];
	char IQapi[MAX_RESPONSE];
	char Tester_Model[MAX_RESPONSE];
	char Tester_SN[MAX_RESPONSE];
	char Tester_Hardware_Version[MAX_RESPONSE];
	char Tester_FW[MAX_RESPONSE];
	char Instrument_Driver_Version[MAX_RESPONSE];
	char Tester_Calibration_Date[MAX_RESPONSE];
	char Tester_VSA_Version[MAX_RESPONSE];
	char Tester_VSG_Version[MAX_RESPONSE];
	char Tester_Flash_Version[MAX_RESPONSE];
	char Tester_VSAFPGA[MAX_RESPONSE];
	char Tester_VSGFPGA[MAX_RESPONSE];
	char IQapitc[MAX_RESPONSE];
	char IQ_NXN[MAX_RESPONSE];
	char IQV_mw[MAX_RESPONSE];
	char IQ_NXN_Signal[MAX_RESPONSE];
	char IQ_MAX_Signal[MAX_RESPONSE];
	char IQ_Zigbee_Signal[MAX_RESPONSE];
	char IQ_NFC_Signal[MAX_RESPONSE];
	char Bluetooth[MAX_RESPONSE];
	char IQV_Analysis[MAX_RESPONSE];
	char GPS_FM_FW_Interface_Version[MAX_RESPONSE];
	char GPS_FM_FW_Version[MAX_RESPONSE];
	char FM_Analysis_Version[MAX_RESPONSE];
	char IQ2010Ext[MAX_RESPONSE];
	char IQapiExt[MAX_RESPONSE];
	char Mpsif[MAX_RESPONSE];	
} IQ_VERSION;

enum IQTESTER_TYPE_MIMO_CONFIG
{
	IQTYPEMIMO_VIEW,
	IQTYPEMIMO_NxN
};

enum SEGMENT_CAPTURE_MODE
{
    SINGLE,
    MULTI
};

enum LP_TESTER_CONTROL
{
    TESTER_CONTROL_IQAPI    = 0,                            /*!< Using IQapi to control LP tester  */
    TESTER_CONTROL_SCPI     = 1,                             /*!< Using SCPI to control LP tester  */
	TESTER_CONTROL_SCPI_VERIFAST,                        /*!< Using SCPI to control LP tester  */
};

enum IQAPI_PORT_ENUM
{
    PORT_OFF   = 1,						/*!< Port has been disabled.	*/
    PORT_LEFT  = 2,						/*!< RF uses left port(RF1).	*/
    PORT_RIGHT = 3,						/*!< RF uses right port(RF2).	*/
    PORT_1	   = PORT_LEFT,				/*!< RF uses RF1A/RF1B.			*/
    PORT_2	   = PORT_RIGHT,			/*!< RF uses RF2A/RF2B.			*/
	PORT_3	   = 4,						/*!< RF uses RF3A/RF3B.			*/
    PORT_4     = 5,                     /*!< RF uses RF4A/RF4B.			*/
    PORT_VTH_1 = PORT_1,
    PORT_VTH_2 = PORT_2,
    PORT_VTH_3 = PORT_3,
    PORT_VTH_4 = PORT_4,
    PORT_IF_1  = 6,                     /*!< IF uses IF1A.				*/
    PORT_IF_2  = 7,                     /*!< IF uses IF2A.				*/
    PORT_IF_3  = 8,                     /*!< IF uses IF3A.				*/
    PORT_IF_4  = 9,                     /*!< IF uses IF4A.				*/
    PORT_CW_1  = 10,
    PORT_CW_2  = 11,
    PORT_CW_3  = 12,
    PORT_CW_4  = 13,
    PORT_BB    = 33,					/*!< BB ports in use.			*/

	MAX_PORT_INDEX
};

enum IQAPI_ROUT_ENUM
{
    ROUTOFF = 0,					/*!< For IQxe80      */
	ROUT1   = 1,                    /*!< For IQxel160, RF uses ROUT1      */
    ROUT2   = 2,                    /*!<  For IQxel160, RF uses ROUT2      */
	ROUTALL = 3                     /*!<  For IQxel160, RF uses both ROUT1 and ROUT2      */
};

enum VSA_ENUM
{
    VSAALL  = 0,					/*!< Using all VSA      */
	VSA1    = 1,                    /*!< Using VSA1         */
    VSA2    = 2,                    /*!< Using VSA2         */
	VSA3    = 3,                    /*!< Using VSA3         */
	VSA4    = 4                     /*!< Using VSA4         */
};

enum SOURCE_VSX_ENUM
{
	SOURCE_VSA1    = 1,                    /*!< Using VSA1         */
    SOURCE_VSA2    = 2,                    /*!< Using VSA2         */
	SOURCE_VSA3    = 3,                    /*!< Using VSA3         */
	SOURCE_VSA4    = 4,                     /*!< Using VSA4         */
	SOURCE_VSG1    = 5,                    /*!< Using VSG1         */
    SOURCE_VSG2    = 6,                    /*!< Using VSG2         */
	SOURCE_VSG3    = 7,                    /*!< Using VSG3         */
	SOURCE_VSG4    = 8                     /*!< Using VSG4         */
};

enum TRIGGER_TYPE_ENUM
{
	SWM,	/*Software marker*/
	VSGM,	/*Start of a waveform. Repeats with the repeat of waveform.*/
	VSGD,	/*End of a waveform*/
	RFD1,	/*Wideband RF power threshold detection on port RF1*/
	RFD2,	/*Wideband RF power threshold detection on port RF2*/
	RFD3,	/*Wideband RF power threshold detection on port RF3*/
	RFD4,	/*Wideband RF power threshold detection on port RF4*/
	WSM1,	/*Programmable marker in wave segment. Repeats with the repeat of wave segment.*/
	WSM2,	/*Programmable marker in wave segment. Repeats with the repeat of wave segment.*/
	WLD,	/*End of wavelist. Repeats with the repeat of wavelist.*/
	TYPE_OFF,	/*Power is set off*/
	HIGH,	/*Marker is set high*/
	LOW,	/*Marker is set low*/
	TIMEXP	/*Timer expiration*/
};

enum LP_MEASUREMENT_VSG_MODE
{
    VSG_NO_MOD_FILE_LOADED = 0,             /*!< No MOD file has been loaded*/
    VSG_SINGLE_MOD_FILE,                    /*!< Single MOD file has been loaded*/
    VSG_MULTI_SEGMENT_MOD_FILE,             /*!< Multi-segment MOD file has been loaded*/
    VSG_INVALID_MODE                        /*!< Invalid VSG mode*/
};

enum LP_ANALYSIS_TYPE_ENUM
{
	ANALYSIS_80211AG,                    /*!< 802.11a/g analysis*/
	ANALYSIS_80211B,                     /*!< 802.11b analysis*/
	ANALYSIS_MIMO,                       /*!< 802.11n MIMO analysis*/
	ANALYSIS_POWER,                      /*!< Power analysis*/
	ANALYSIS_FFT,                        /*!< FFT analysis*/
	ANALYSIS_CCDF,                       /*!< CCDF analysis*/
	ANALYSIS_CW,                         /*!< CW analysis*/
	ANALYSIS_WAVE,                       /*!< Wave analysis*/
	ANALYSIS_SIDELOBE,                   /*!< Sidelobe analysis*/
	ANALYSIS_POWERRAMPOFDM,              /*!< Power Ramp analysis for OFDM*/
	ANALYSIS_POWERRAMP80211B,            /*!< Power Ramp analysis for 802.11b*/
	ANALYSIS_BLUETOOTH,                  /*!< Bluetooth analysis*/
	ANALYSIS_ZIGBEE,                     /*!< Zigbee analysis*/
	ANALYSIS_HT40,                       /*!< 802.11n HT40 spectrum mask analysis*/
	ANALYSIS_80211AC,                    /*!< 802.11ac MIMO analysis*/
	ANALYSIS_80211AD,                    /*!< 802.11ad MIMO analysis*/
	ANALYSIS_80211p,					 /*!< 802.11p analysis*/
	ANALYSIS_80211ah,					 /*!< 802.11ah analysis*/
	ANALYSIS_VHT80,						 /*!< 802.11ac VHT80 analysis*/
	ANALYSIS_DECT,						 /*!< DECT analysis*/
	ANALYSIS_NFC_FREQSWEEP,
	ANALYSIS_NFC_TARGET,
	ANALYSIS_NFC_INITIATOR,
	ANALYSIS_IQLINK_THROUGHPUT,
	ANALYSIS_IQLINK_TX_POWER,
	ANALYSIS_IQLINK_TX_POWER_MAPI,
	ANALYSIS_IQLINK_PER,
	ANALYSIS_IQLINK_FAST_PER,
	ANALYSIS_IQLINK_PER_SWEEP_AUTO_SENSE,
	ANALYSIS_WISUN,
	ANALYSIS_GPRF_FFT,
	ANALYSIS_11AD_FFT,
	ANALYSIS_ZWAVE,                      /*!< Zwave analysis */
	ANALYSIS_MAX_NUM                     /*!< Invalid analysis type*/

};

enum LP_TECHNOLOGY_TYPE_ENUM
{
	TECHNOLOGY_80211AG,                    /*!< 802.11a/g analysis*/
	TECHNOLOGY_80211B,                     /*!< 802.11b analysis*/
	TECHNOLOGY_MIMO,                       /*!< 802.11n MIMO analysis*/
	TECHNOLOGY_WAVE,                       /*!< Wave analysis*/
	TECHNOLOGY_BLUETOOTH,                  /*!< Bluetooth analysis*/
	TECHNOLOGY_ZIGBEE,                     /*!< Zigbee analysis*/
	TECHNOLOGY_DECT,                     /*!< DECT analysis*/
	TECHNOLOGY_HT40,                       /*!< 802.11n HT40 spectrum mask analysis*/
    TECHNOLOGY_WIGIG,                      /*!< 802.11ad analysis*/
	TECHNOLOGY_ZWAVE,                      /*!< ZWAVE analysis */
	TECHNOLOGY_MAX_NUM                     /*!< Invalid analysis type*/
};

enum NAVIGATION_TECHNOLOGY_ENUM
{
	NAVIGATION_COMPASS,                   
	NAVIGATION_GLONASS,                  
	NAVIGATION_GPS                       
};

enum NAVIGATION_MODULATION_DATA_TYPE_ENUM
{
	ALL0,                   
	ALL1,                  
	NAV                       
};

enum FSK_MODULATION_TYPE
{                   
	MOD_2FSK,                  
	MOD_4FSK                       
};

enum FSK_FSD_TYPE
{                   
	FSK_SPEC,                  
	FSK_USER,
	FSK_NONE
};

enum WISUN_MODULATION_TYPE
{                   
	FSK                                         
};

enum IQXEL_CONNECTION_TYPE_ENUM
{
	IQXEL80_SISO,                   
	IQXEL80_MIMO,                     
	IQXEL160_SISO,
	IQXEL160_ROUT1,
	IQXEL160_ROUT2,
	SINGLE_IQXEL160_2X2_80MHZ_MIMO,
	DOUBLE_IQXEL160_4X4_80MHZ_MIMO,
	DOUBLE_IQXEL160_2X2_160MHZ_MIMO,
	IQXEL160_IQXEL80_3X3_80MHZ_MIMO,
	SINGLE_IQXEL_3X3_80MHZ_MIMO,
	SINGLE_IQXEL_4X4_80MHZ_MIMO,
	FOUR_IQXEL160_4X4_160MHZ_MIMO
};

enum SIGNAL_MODE
{
	UP_TO_80MHZ_SIGNAL,
	CONT_160MHZ_SIGNAL,
	NON_CONT_80_80MHZ_SIGNAL,
    UP_TO_2GHz_SIGNAL
};

//! VSA Capture Sample Rate.
enum {SAMPLE_640KHz, SAMPLE_5_12MHz};

//! FM Filter Selection.
enum {BLACKMAN_HARRIS, HANNING, FLAT_TOP};

//! FM Mode Selection.
enum {FM_MONO, FM_STEREO, FM_AUTO};

//! FM Pre-emphasis Time Constant Select.
enum {PRE_EMPHASIS_NONE, PRE_EMPHASIS_25US, PRE_EMPHASIS_50US, PRE_EMPHASIS_75US};

//! RDS Phase Shift Selection.
enum {RDS_PHASE_SHIFT_NONE, RDS_PHASE_SHIFT_90};

//! FM Channel Selection.
/*!
 * -LEFT_EQUALS_RIGHT and LEFT_EQUALS_MINUS_RIGHT can be used only for generating audio stimulus for FM RX Test.
 * -It cannot be used for setting up VSG Tones.
 */
enum {LEFT_ONLY, RIGHT_ONLY, LEFT_RIGHT, LEFT_EQUALS_RIGHT, LEFT_EQUALS_MINUS_RIGHT};

//! ALC mode
enum IQAPI_ALC_MODES
{
	ALC_MANUAL,					/*!< Indicates ALC Manual is on*/
	ALC_AUTO_ON_TIMEOUT,		/*!< Indicates ALC Automatic on Timeout*/
	ALC_OFF,					/*!< Indicates that ALC is off*/
	ALC_MANUAL_HIGH_PRECISION	/*!< Indicates ALC Manual with high precision is on*/
};

//! Error code list.
enum IQMEASURE_ERROR_CODES
{
	ERR_OK,
	ERR_NO_CONNECTION,
	ERR_NOT_INITIALIZED,
	ERR_SAVE_WAVE_FAILED,
	ERR_LOAD_WAVE_FAILED,
	ERR_SET_TX_FAILED,
	ERR_SET_WAVE_FAILED,
	ERR_SET_RX_FAILED,
	ERR_CAPTURE_FAILED,
	ERR_NO_CAPTURE_DATA,
	ERR_VSA_NUM_OUT_OF_RANGE,
	ERR_ANALYSIS_FAILED,
	ERR_NO_VALID_ANALYSIS,
	ERR_VSG_PORT_IS_OFF,
    ERR_NO_MOD_FILE_LOADED,
    ERR_NO_CONT_FOR_MULTI_SEGMENT,
    ERR_MEASUREMENT_NAME_NOT_FOUND,
    ERR_INVALID_ANALYSIS_TYPE,
    ERR_NO_ANALYSIS_RESULT_AVAILABLE,
    ERR_NO_MEASUREMENT_RESULT,
    ERR_INVALID_IP_ADDRESS,
    ERR_GENERAL_ERR,
	ERR_TX_NOT_DONE,
    ERR_SET_TOKEN_FAILED,
    ERR_TOKEN_RETRIEVAL_TIMEDOUT,
    ERR_GET_TOKEN_FAILED,
	ERR_INVALID_CAPTURE_INDEX,
	ERR_VSG_POWER_EXCEED_LIMIT,
    ERR_ANALYSIS_NULL_POINTER,
    ERR_ANALYSIS_UNSUPPORTED_PARAM_NAME,
    ERR_ANALYSIS_INVALID_PARAM_VALUE,
	ERR_INVALID_DATA_CAPTURE_RANGE,
	ERR_DATARATE_DOES_NOT_EXIST,
	ERR_BUFFER_OVERFLOW,
	ERR_SET_PATH_NOT_DONE,
		//FM Error Codes
	ERR_FM_NOT_INITIALIZED,
	ERR_FM_SET_TX_FAILED,
	ERR_FM_GET_TX_FAILED,
	ERR_FM_SET_AUDIO_TONE_FAILED,
	ERR_FM_GET_AUDIO_TONE_FAILED,
	ERR_FM_NO_CAPTURE_DATA,
	ERR_FM_VSA_CAPTURE_FAILED,
	ERR_FM_SET_RX_FAILED,
	ERR_FM_GET_RX_FAILED,
	ERR_FM_ANALYSIS_FAILED,
	ERR_FM_CAPTURE_AUDIO_FAILED,
	ERR_FM_GENERATE_AUDIO_FAILED,
	ERR_FM_QUERY_AIM_FAILED,
	ERR_FM_SAVE_WAV_AUDIO_FAILED,
	ERR_FM_PLAY_AUDIO_FAILED,
	ERR_FM_LOAD_AUDIO_FAILED,
	ERR_FM_STOP_AUDIO_PLAY_FAILED,
	ERR_GPS,
	ERR_SET_MULTI_THREAD_FAILED,
	ERR_UNKNOWN_TESTER_TYPE,
    ERR_WRONG_TESTER_TYPE,
    ERR_INVALID_TESTER_CONTROL,         // Tester control method not valid (SCPI, IQAPI, ...)
	ERR_NO_RESPONSE_FROM_TESTER,

	ERR_MPTA_NOT_ENABLE,
	ERR_MPTA_NOT_DISENABLE,
	ERR_MPTA_SET_TXRX,
	ERR_MPTA_CAPTURE_FAILED,

	//Add
	ERR_INVALID_TEST_TYPE,
	ERR_INVALID_SEQUENCE_NUM,
	ERR_TX_RX_FREQ_NOT_EQUAL,
	ERR_INVALID_RF_PORT_SETTING,
	ERR_INVALID_PACKET_COUNT,
	ERR_NOT_SUPPORTED_RF_MODE,
	ERR_EMPTY_WAVEFORM_NAME,
	ERR_ZERO_WAVEFROM_SIZE,
	ERR_NOT_DEFINED_CHANGE_DIRECTION_TIME,
	ERR_WRONG_EXEC_PARAM_SIZE,
	ERR_INVALID_PACKET_TYPE,
	ERR_INVALID_MODULATION,
	ERR_NOT_ENOUGH_RETURN_ACK,
	ERR_INVALID_PARAM_SETTING,
	ERR_UNKNOWN,
	ERR_FUNCTION_NOT_SUPPROTED,
	ERR_VSG_TRANSMIT_PKT_NOT_FINISHED,
	ERR_SET_VSG_POWER_STATE_FAILED,
	ERR_VSG_PLAY_WAVE_FAILED,
	ERR_WAVEFORM_NOT_EXISTED_IN_TESTER,
	ERR_INVALID_HANDLE,
	ERR_CANNOT_FIND_TESTER_FOLDER, 
	ERR_WAVEFORM_NOT_FOUND,
	ERR_SET_VSA_VSG_SEPARATION_FAILED,
	ERR_NO_VALUE_DEFINED,
    ERR_WRONG_SCPI_RETURN_SIZE,
	ERR_NAV_ACTIVATE_FAILED,
	ERR_NAV_DEACTIVATE_FAILED,
	ERR_NAV_SET_CONTINUE_WAVE_FAILED,
	ERR_NAV_SET_MODULATE_MODE_FAILED,
	ERR_NAV_SET_TIME_OF_WEEK_FAILED,
	ERR_NAV_SET_GLONASS_CHANNEL_FAILED,
	ERR_NAV_SET_POWER_FAILED,
	ERR_NAV_LOAD_SCENARIO_FILE_FAILED,
	ERR_NAV_PLAY_SCENARIO_FILE_FAILED,
	ERR_INVALID_FREQ_BAND,
	ERR_OPEN_FILE_FAILD,
    ERR_NOT_SUPPORTED_TECHNOLOGY,

	//IQlink
	ERR_IQLINK_DISASSOCIATE_FAILED,
	ERR_IQLINK_SET_TCP_FAILED,
	ERR_IQLINK_SET_UDP_FAILED,
	ERR_IQLINK_SET_AP_FAILED,
	ERR_IQLINK_SET_AP_OUT_OF_RANGE,
	ERR_IQLINK_UPLINK_FAILED,
	ERR_IQLINK_DOWNLINK_FAILED,
	ERR_IQLINK_PER_FAILED,
	ERR_IQLINK_PORT_ENABLE_FAILED,
	ERR_IQLINK_SET_PATH_LOSS_FAILED,
	ERR_IQLINK_SET_IPERF_FAILED,
	ERR_IQLINK_TX_POWER_FAILED,
	ERR_IQLINK_SET_RXPER_ACK_FAILED,
	ERR_IQLINK_SET_DUT_AP_FAILED,
	ERR_IQLINK_DUT_NOT_CONNECTED,
	//MVSA/GALL module define error
	ERR_MODULE_DEF_NOT_SUPPORTED,

	/*IQLiteAptRemoveBegin*/
	//MPS Sequence error code
	ERR_SEQ_ENG_NOT_STARTED,
	ERR_SEQ_MODGROUP_NOT_SUPPORTED,
	ERR_SEQ_TYPE_NOT_SUPPORTED,
	ERR_SEQ_MODGROUP_OUT_RANGE,
	ERR_SEQ_BUILD_FAILED,
	ERR_SEQ_DEF_FAILED,
	ERR_SEQ_NOT_DEFINED,
	ERR_SEQ_ARM_FAILED,
	ERR_SEQ_EXEC_FAILED,
	ERR_SEQ_ACK_QUERY_FAILED,
	/*IQLiteAptRemoveEnd*/

	ERR_INIT_GLOBAL_DATA_FAILED,

	ERR_MAX_ERR_CODE_NUMBER
};

//! Capture mode
enum IQMEASURE_CAPTURE_NONBLOCKING_STATES
{
	BLOCKING,	// NONBLOCKING is off
	ARM_TRIGGER,
	CHECK_DATA
};


//! WiFi packet format.
enum PACKET_FORMAT
{
	PKT_AUTO = 0,
	HT_MF = 1,
	HT_GF = 2,
	NON_HT = 3,
	VHT = 4,
	HE_SU, //HE-SU
	HE_MU, //HE-MU
	HE_TR, //HE-TRG
};

//! WiFi modulation.
enum MODULATION
{
	OFDM,
	DSSS
};

//! WiFi calculation type.
enum CALC_TYPE
{
	TX_QUALITY,
	AVG_POWER,
	PEAK_POWER,
	HT20_MASK,
	HT40_MASK,
	VHT80_MASK,
	VHT160_MASK,
	VHT80_80_MASK,
	SPECTRUM_FLATNESS
};

//! TXQuality Clock Rate.
enum TXQUALITY_CLOCK_RATE
{
	CLK_RATE_1_1,
	CLK_RATE_1_2,
	CLK_RATE_1_4,
	CLK_RATE_1_5,
	CLK_RATE_1_8,
	CLK_RATE_1_10
};


enum TX_PATH_ENUM
{
	eTX_PATH_12,
	eTX_PATH_13,
	eTX_PATH_1,
	eTX_PATH_2,
	eTX_PATH_3,
	eTX_PATH_123,
	eTX_PATH_ALL
};

enum NFC_STANDARD
{
	NFCA,
	NFCB,
	NFCF,
	AACT,
	FACT,
	APAS,
	FPAS
};

enum NFC_DUTMODE
{
	TARGET,
	INITIATOR
};

enum NFC_DATARATE
{
	R106,
	R212,
	R424
};

enum NFC_MOD_DEPTH
{
	AUTO,
	M7,
	M8P5,
	M10,
	M13,
	M16,
	M22,
	M30,
	M100
};

enum NFC_RES_FREQ
{
	RES_OFF,
	R13M56,
	R16M1,
	R15M
};

enum NFC_LMOD_FILTER
{
	NONE,
	BW1M,
	BW5M
};

enum NFC_TRIG_SOURCE
{
	IMM,
	VID,
	TRXS,
	TTXD
};

enum NFCA_COMMAND_TYPE
{
	REQA,
	WUPA,
	ATRREQ_A,
	ATQA,
	ATRRES_A,
	USER_A
};

enum NFCB_COMMAND_TYPE
{
	REQB,
	ATQB,
	USER_B
};

enum NFCF_COMMAND_TYPE
{
	POLLREQ,
	ATRREQ_F,
	POLLRES,
	ATRRES_F,
	USER_F
};

enum IQBEAM_MODE
{
	FOUR_TO_FOUR,
	FOUR_TO_ONE
};

enum IQLINK_BAND
{
	IQLINK_BAND_2G4 = 0,
	IQLINK_BAND_5G
};

enum IQLINK_TECH_MODE
{
	TECH_MODE_B,
	TECH_MODE_G,
	TECH_MODE_N,
	TECH_MODE_A,
	TECH_MODE_AC
};

enum IQLINK_ENCRYPT_TYPE
{
	WEP = 0,
	WPA2,
	WPA
};

enum IQLINK_BANDWIDTH
{
	LINK_BW_20 = 20,
	LINK_BW_40 = 40,
	LINK_BW_80 = 80
};

enum IQXEL_TRIG_SOURCE
{
	TRIG_IMM = 0,
	TRIG_EXT1,
	TRIG_EXT2,
	TRIG_EXT3,
	TRIG_EXT4
};
enum IQLINK_TEST_MODE
{
	LINK_AP_MODE,
	LINK_CLIENT_MODE
};

enum GPRF_PMODE
{
	PMODE_AVER, //Average value (for each frequency bin)
	PMODE_PEAK  //Peak value (for each frequency bin)
};

enum GPRF_SIGNAL
{
	SIGNAL_CW, //Continuous Wave; generates high-resolution spectrum
	SIGNAL_NB, //Narrow Band; generates mid-resolution spectrum
	SIGNAL_WB  //Wide Band; generates low-resolution spectrum
};

enum GPRF_WINDOW
{
	WINDOW_GAUSS,                //Gaussian Window
	WINDOW_FLATTOP,              //Flattop Window
	WINDOW_HAMMING,              //Hamming Window
	WINDOW_HANN,                 //Hann Window
	WINDOW_BLACKMANHARRIS,       //BLACKMANHARRIS Window
	WINDOW_NONE                  //No FFT Windowing
};

typedef struct
{
	const char* keyword[20][20];
	int attribute; 
	int max_keyword_per_measurement;
	int num_measurement;
}table;
typedef struct
{
	int Feedback_Length;	
	double *Result_Scalar_temp;
	double *Result_vector;
}container;
enum IQLINK_HT_PPDU_FORMAT
{
	LINK_AP_GREENFIELD = 1,
	LINK_AP_MIXED
};

enum IQLINK_GUARD_INTERVAL
{
	LINK_GI_SHORT,
	LINK_GI_LONG
};

enum IQLINK_OFDM_CODING
{
	LINK_BCC,
	LINK_LDPC
};

enum IQLINK_DGEN_MODE
{
	LINK_IPERF3,
	LINK_IPERF2,
	LINK_FPING
};

enum IQLINK_ASSOCIATION_MODE
{
	LINK_ASSO_AUTO,
	LINK_ASSO_USER,
	LINK_ASSO_AP
};

enum IQLINK_DATA_GENERATOR_MODE
{
	DATA_GENERATOR_MODE1 = 1,
	DATA_GENERATOR_MODE2,
	DATA_GENERATOR_MODE3,
	DATA_GENERATOR_MODE4
};

typedef struct IQSWITCH_STATUS
{
	int FESW_index;
	int port; // A port or B port
	int rsrc; // RF1~RF4
	int operation; // On 1 or OFF )
}IQSWITCH_STATUS;

//! APT settings.
/*!
 * -isolationGap is used for checking interference gap between 2 DUTs' TX to RX, RX to RX conditions on nearby channels. Higher isolationGap makes more tolerant of this conditions.
 */
typedef struct tagAPTSettings
{
    double isolationGap;
    double isoGapOnDiffRout;
    double isoGapOnSamePortDiffRout;
} APTSettings;

/**********************
* Extern Variables  *
**********************/
IQMEASURE_API int  loggerIQmeasure;
IQMEASURE_API int  timerIQmeasure;

/*********************/
/* Internal function */
/*********************/
int LP_Analyze(void);
//FM
int LP_Analyze_FMHndl(void);
int LP_FreeMemory(void);


/***************************/
/* Init and Term Functions */
/***************************/

//! Initializes the MATLAB environment for running IQmeasure
/*!
 * \param[in] IQtype Pointer to IQ tester type. IQXel or IQ legacy testers. It decides what dll to link.
 * \param[in] testerControlMethod indicates what method is used to control LP tester: 0 = IQapi, 1 = SCPI command
 * \return 0 if MATLAB initialized OK; non-zero indicates MATLAB failed to initialize.
 * \remark This function needs to be run only once, typically at the very beginning of a program.
 */
IQMEASURE_API int		LP_Init(int IQtype = IQTYPE_XEL,int testerControlMethod = 1);


//! Get IQmeasure Thread Index pertinent to the thread ID
/*!
 * \param[in] IQmeasureThreadID The thread ID that IQmeasure runs in.
 * \param[out] IQmeasureThreadIndex The thread index that IQmeasure uses in the pointer array.
 * \return 0 no error; non-zero indicates an error happens.
 */
IQMEASURE_API int		LP_GetIQmeasureThreadIndex(int IQmeasureThreadID, int* IQmeasureThreadIndex);


//! Get IQmeasure Thread Index pertinent to the thread ID
/*!
 * \param[in] IQmeasureThreadID The thread ID that IQmeasure runs in.
 * \param[in] IQmeasureThreadIndex The thread index that IQmeasure uses in the pointer array.
 * \return 0 no error; non-zero indicates an error happens.
 */
IQMEASURE_API int		LP_SetIQmeasureThreadIndex(int IQmeasureThreadID, int IQmeasureThreadIndex);

//! Terminates the MATLAB environment
/*!
 * \return 0 if MATLAB initialized OK; non-zero indicates MATLAB failed to terminate.
 * \remark This function only needs to be run at the very end when a programm is going to exit.
 *         Calling this function in the middle of a program will cause the program not to function.
 *         Since the programm is exiting anyway, you may skip calling LP_Term().
 */
IQMEASURE_API int		LP_Term(void);

//! Enable IQmeasure multiple threads to maintain separate data and states from each other.
/*!
*
* \return always return ERR_OK
*/
IQMEASURE_API int		LP_SetConcurrency();

//! Gets the IQapi handle and tester initialized status
/*!
* \param[in] getHndl Pointer to IQapi handle
* \param[in] testerInitialized Pointer to tester initialized status
*
* \return always return ERR_OK
*/
IQMEASURE_API int		LP_GetIQapiHndl(void **getHndl, bool *testerInitialized);

//! Gets the version information
/*!
 * \param[out] buffer The buffer that will return the version information, which includes:
 *   - IQmeasure version
 *   - Source code SVN version
 *   - IQapi version (if used)
 * \param[in] buf_size Indicates the size of the buffer
 *
 */
IQMEASURE_API bool		LP_GetVersion(char *buffer, int buf_size);

//! Gets the version information and puts them in a structure
/*!
 * \param[out] iqVersion Structure that contains all tester and software related version information
 *
 * \return always return ERR_OK
 */
IQMEASURE_API int		LP_GetIQVersion(IQ_VERSION iqVersion[MAX_MIMO_TESTER_NUM]);

//! Gets the Session ID
/*!
 * \param[out] m_sessionID The session ID that currently used.
 * \return always return ERR_OK
 *
 */
IQMEASURE_API int		LP_GetSessionID(int* m_sessionID);

//! Releases all the slave testers controlled by the master tester
/*!
 * \return always return ERR_OK
 */
IQMEASURE_API int		LP_DisconnectRemoteTester(void);

//! Sets the port number for TCP/IP connection to the tester 
/*!
 * \param[in] portNumber The port number for connection.
 * \return always return ERR_OK
 */
IQMEASURE_API int		LP_SetSocketPort(char portNumber[]);

//! Sets the timeout in milliseconds for TCP/IP connection to the tester; need to call before LP_InitTesterN(). 
/*!
 * \param[in] socketTimeoutMs The timeout for connection.
 * \return always return ERR_OK
 */
IQMEASURE_API int		LP_SetSocketTimeout(int socketTimeoutMs);

//! Check the Technology License on IQxel
/*!
 * \param[out] returnReault 1 means license available; 0 means license unavailable
 * \param[in] the string of the license type
 */

IQMEASURE_API int		LP_CheckLicense(const char *licenseType, bool *returnResult); 
//! Check whether the corresponding IQmeasure DLL has been loaded
/*!
 * \param[out] returnReault true means corresponding DLL loaded; false means DLL has not bee loaded
 */
IQMEASURE_API bool		LP_CheckDllLoaded(); 

//! Check the tester for the last error message
/*!
 * \param[out] errMsg error message got from the tester
 * \return ERR_OK if the tester has no error; non-zero value means the tester has errors.
 */
IQMEASURE_API int		LP_CheckTesterErrMsg(char *errMsg); 

/*********************************/
/* Connection Handling Functions */
/*********************************/

//! Initialize Testers
/*!
 * \param[in] ipAddresses The IP addresses of the testers to be connected
 * \param[in] numOfTesters The number of testers to be connected
 * \param[out] num_mods_per_ip The total number of available modules per IP(tester).
 * \return ERR_OK if the tester has been successfully initialized; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_InitTesterN(char *ipAddresses, int numOfTesters = 1  , int* num_mods_per_ip = NULL );

//! Configure testers based on different signal modes 
/*!
 * \param[in] signalMode The incoming/outgoing singal mode
 *				- = UP_TO_80MHZ_SIGNAL: 20/40/80MHz signal
 *				- = CONT_160MHZ_SIGNAL: Contiguous 160MHz signal
 *				- = NON_CONT_80_80_MHZ_SIGNAL: Non-contiguous 80+80 MHz signal
 * \param[in] selectedModules The selected modules array with specified module index in the module group
 * \param[in] numOfSelectedModules The number of selected modules
 * \return ERR_OK if the tester has been successfully initialized; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SetTesterMode( int signalMode = UP_TO_80MHZ_SIGNAL, int *selectedModules = NULL, int numOfSelectedModules = 1 );

//! Configure testers Bank ID : this is for IQxstream-M
/*!
 * \param[in] selectedBanks The selected bank id array
 * \return ERR_OK if the tester has been successfully initialized; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SetTesterBank( int *selectedBanks = NULL );

//! Initializes a tester
/*!
 * \param[in] ipAddress The IP address of the tester
 * \param[in] IQxelConnectionType (Ignore for IQ2010/IQnxn/IQview/IQflex; always set as 0 for IQxel80; )
 *				- =0: Default value for IQxel80
 *				- =1: ROUT1 for IQxel160
 *				- =2: ROUT2 for IQxel160
 *				- =3: 160Mhz or 80Mhz+80MHz mode for IQxel160
 * \return ERR_OK if the tester has been successfully initialized; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_InitTester(char *ipAddress, int IQxelConnectionType = 0 );

//! Initializes a 2x2 802.11n setup
/*!
 * \param[in] ipAddress1 The IP address of the master tester
 * \param[in] ipAddress2 The IP address of the slave tester
 * \param[in] IQxelConnectionType ((Ignore for IQ2010/IQnxn)
 *				- =0: Must set 0 for IQxel80
 *				- =1: Single IQxel160/IQxel280/IQxel-M8/IQxel-M16 tester 2x2 80Mhz MIMO mode;if ipAddress1 and ipAddress2 both must be the same (IQxel160's IP address)
 *                    Double IQxel160 tester 160M 2x2 MIMO mode or 80Mhz+80Mhz 2x2 MIMO mode;if ipAddress1 and ipAddress2 are different(two IQxel160s' IP addresses)
 * \return ERR_OK if testers has been successfully initialized; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_InitTester2(char *ipAddress1, char *ipAddress2, int IQxelConnectionType = 0);

//! Initializes a 3x3 802.11n setup
/*!
 * \param[in] ipAddress1 The IP address of the master tester
 * \param[in] ipAddress2 The IP address of the slave tester
 * \param[in] ipAddress3 The IP address of the slave tester
 *
 * \return ERR_OK if testers has been successfully initialized; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_InitTester3(char *ipAddress1, char *ipAddress2, char *ipAddress3);

//! Initializes a 4x4 802.11n setup
/*!
 * \param[in] ipAddress1 The IP address of the master tester
 * \param[in] ipAddress2 The IP address of the slave tester
 * \param[in] ipAddress3 The IP address of the slave tester
 * \param[in] ipAddress4 The IP address of the slave tester
 *
 * \return ERR_OK if testers has been successfully initialized; otherwise call LP_GetErrorString() for detailed error message.
 * Note: In IQxel system, for double IQxel160 4x4 80Mhz MIMO mode,need to set ipAddress1= ipAddress2 = Master Tester's IP; ipAddress3= ipAddress4 = Slave's IP 
 */
IQMEASURE_API int		LP_InitTester4(char *ipAddress1, char *ipAddress2, char *ipAddress3, char *ipAddress4);

//! Initializes a 4x4 160MHz setup
/*!
 * \param[in] ipAddress1 The IP address of the master tester
 * \param[in] ipAddress2 The IP address of the slave tester
 * \param[in] ipAddress3 The IP address of the slave tester
 * \param[in] ipAddress4 The IP address of the slave tester
 *
 * \return ERR_OK if testers has been successfully initialized; otherwise call LP_GetErrorString() for detailed error message. 
 */
IQMEASURE_API int		LP_IQXEL160_InitTester4(char *ipAddress1, char *ipAddress2, char *ipAddress3, char *ipAddress4);

/******************/
/* Error handling */
/******************/
//! Get the detailed error message for the specific error code
/*!
 * \param[in] err Error Code
 * \return Error message.
 */
IQMEASURE_API char*		LP_GetErrorString(int err);

//! Cet the last error message of IQapi
/*!
 * \return Error message.
 */
IQMEASURE_API char*		LP_GetIQapiHndlLastErrMsg();

//! Closes the connection to tester(s)
/*!
 * \return ERR_OK if the tester has been successfully initialized; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_ConClose(void);

//! Re-establish the connection to tester(s)
/*!
 * \return ERR_OK if the tester has been successfully initialized; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_ConOpen(void);
//! Set the tester to default (initial) status
/*!
 * \return ERR_OK if the tester has been successfully initialized; otherwise call LP_GetErrorString() for detailed error message.
 */

IQMEASURE_API int		LP_SetDefault(void);

//! Establish the connection to IQxel160/280 tester(s) with dual-head support
/*!
 * \param[in] IQxelConnectionType: 
 *				- =1: Dual head on ROUT1, the process will obtain control of ROUT1
 *				- =2: Dual head on ROUT2, the process will obtain control of ROUT2
 *				- =3: Dual head per tester, the process will obtain control of whole tester in 160Mhz mode
 * \return ERR_OK if the tester has been successfully initialized; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_IQXEL160_DualHead_ConOpen(int IQxelConnectionType, int tokenID, char *ipAddress1, char *ipAddress2=NULL, char *ipAddress3=NULL, char *ipAddress4=NULL, unsigned int timeOutMS = 0);
//! Establish the connection to tester(s) with dual-head support
/*!
 * \return ERR_OK if the tester has been successfully initialized; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_DualHead_ConOpen(int tokenID, char *ipAddress1, char *ipAddress2=NULL, char *ipAddress3=NULL, char *ipAddress4=NULL, unsigned int timeOutMS = 0);

//! Retrieve the Token ID being used for dual-head operation
/*!
 * \return ERR_OK if the tester has been successfully initialized; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_DualHead_GetTokenID(int *tokenID);


//! Closes the connection to tester(s)
/*!
 * \return ERR_OK if the tester has been successfully initialized; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_DualHead_ConClose();

//! Set the communication socket alive time in case the application program crashes or ethernet connection is lost.
/*!
 * \param[in] keepAliveTimeSec Sets Keep Alive probing start delay time.
 * \param[in] probeIntervalSec Sets Keep Alive probing interval time.
 * \return ERR_OK if the tester has been successfully initialized; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_DualHead_SetSocketAliveTime(double keepAliveTimeSec = 10, double probeIntervalSec = 1);

//! Obtain the control of tester(s) for dual-head operation
/*!
 * \return ERR_OK if the tester has been successfully initialized; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_DualHead_ObtainControl(unsigned int probeTimeMS=100, unsigned int timeOutMS=0);

//! Lock tester(s) to save the tester enviroment for debugging purpose
/*!
 * \return ERR_OK if the tester has been successfully locked; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_LockTesterDebug(void);
//! Release the control of tester(s) for dual-head operation
/*!
 * \return ERR_OK if the tester has been successfully initialized; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_DualHead_ReleaseControl();
//! Resets the token handling for the test system. Force to release the token.
/*!
 * \return ERR_OK if the tester has been successfully initialized; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_DualHead_TokenReset();

IQMEASURE_API int		LP_SetAlcMode(IQAPI_ALC_MODES alcMode);
IQMEASURE_API int		LP_GetAlcMode(IQAPI_ALC_MODES* alcMode);

/********************************/
/* Instrument Control Functions */
/********************************/


//! Sets up VSG
/*!
 * \param[in] rfFreqHz The center frequency of VSG (Hz). Set real center frequency for 160MHz contiguous signal. Set center frequency of the first 80MHz for 80+80 signal.
 * \param[in] rfPowerLeveldBm The output power level of VSG (dBm)
 * \param[in] port The port to which the VSG connects, with the following options:
 *				- =1: OFF
 *				- =2: RF port 1 (RF1)
 *				- =3: RF port 2 (RF2)
 *				- =4: RF port 3 (RF3)
 *				- =5: RF port 4 (RF4)
 * \param[in] setGapPowerOff
 *              - =true: Turn off RF power in the gap between packets
 *              - =false: Does not turn off RF power in the gap between packets
 * \param[in] dFreqShiftHz Frequency Shift
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SetVsg(double rfFreqHz, double rfPowerLeveldBm, int port, bool setGapPowerOff = true, double dFreqShiftHz = 0.0);

//! Sets up VSG
/*!
 * \param[in] rfFreqHz The center frequency of VSG (Hz)
 * \param[in] rfPowerLeveldBm The output power level of VSG (dBm)
 * \param[in] ports The ports to which the VSG connects, with the following options:
 *				- =1: OFF
 *				- =2: RF port 1 (RF1)
 *				- =3: RF port 2 (RF2)
 *				- =4: RF port 3 (RF3)
 *				- =5: RF port 4 (RF4)
 * \param[in] portsSize The number of ports to be connected to VSG.
 * \param[in] setGapPowerOff
 *              - =true: Turn off RF power in the gap between packets
 *              - =false: Does not turn off RF power in the gap between packets
 * \param[in] dFreqShiftHz Frequency Shift
 * \param[in] enableMultiPort
 *              - =true:  Enable VSG to connect to multiple ports at the same time, valid only for IQxel-M testers
 *              - =false: VSG can only connect to one port specified in port[] with one value
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SetVsg_MultiPort(double rfFreqHz, double rfPowerLeveldBm, int ports[], int portsSize, bool setGapPowerOff = true, double dFreqShiftHz = 0.0, bool enableMultiPort = true);

//! Sets up VSG Compensation Table
/*!
 * \param[in] dcErrI I part of the DC compensation
 * \param[in] dcErrQ Q part of the DC compensation
 * \param[in] phaseErr Phase compensation
 * \param[in] gainErr Gain compensation
 * \param[in] delayErr Delay compensation
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SetVsg_Compensation(double		  dcErrI,
										 double		  dcErrQ,
										 double		  phaseErr,
										 double		  gainErr,
										 double		  delayErr);
//! Sets up VSG triggerType
/*!
 * \param[in] triggerType Select VSG trigger Type. 0 = IQV_VSG_TRIG_FREE_RUN; 1 = IQV_VSG_TRIG_EXT_1; 2 = IQV_VSG_TRIG_EXT_2
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SetVsgTriggerType(int trigger);

//! Set VSG Trigger settings
/*!
 * \param[in] triggerSource Trigger source used for VSG transmitting.  Valid options are:
 *      - 0 = IQV_VSG_TRIG_FREE_RUN
 *      - 1 = IQV_VSG_TRIG_EXT_1
 *      - 2 = IQV_VSG_TRIG_EXT_2
 * \param[in] triggerType Trigger type used for VSG transmitting.  Valid options are:
 *      - 0 = Positive Edge
 *      - 1 = Negative Edge
 *      - 2 = Level
 * \param[in] triggerMode Trigger mode used for VSG transmitting.  Valid options are:
 *      - 0 = SSHot
 *      - 1 = MPACket
 *      - 2 = MSHot
 * \param[in] triggerTimeoutSec Trigger timeout(second)
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SetVsg_Trigger(int triggerSource = 0, int triggerType = 0, int triggerMode = 0, double triggerTimeoutSec = 1.5);

//! Sets up VSG PowerLevel
/*!
 * \param[in] port The port to which the VSG connects, with the following options:
 *				- =1: OFF
 *				- =2: RF port 1 (RF1)
 *				- =3: RF port 2 (RF2)
 *				- =4: RF port 3 (RF3)
 *				- =5: RF port 4 (RF4)
 * \param[in] rfGainDb The output power level of VSG (dBm)
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SetVsg_PowerLevel(int port, double rfPowerLeveldBm);

//! Sets VSA and VSG to the same port
/*!
 * \param[in] port The ports to which the VSA and VSG connect
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SetVsaVsg_SamePort(int port[]);

//! Outputs various trigger signal for different events
/*!
 * \param[in] marker The marker port for output
 * \param[in] source The hardware source for trigger output
 * \param[in] type The event type for trigger output
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SetMarkerTrigger(int marker,SOURCE_VSX_ENUM source,TRIGGER_TYPE_ENUM type);
//! Sets up VSG including gapPOwerrOff parameter
/*!
 * \param[in] rfFreqHz The center frequency of VSG (Hz)
 * \param[in] rfGainDb The output power level of VSG (dBm)
 * \param[in] port The port to which the VSG connects, with the following options:
 *				- =1: OFF
 *				- =2: RF port 1 (RF1)
 *				- =3: RF port 2 (RF2)
 *				- =4: RF port 3 (RF3)
 *				- =5: RF port 4 (RF4)
 * \param[in] gapPowerOff Turn the GapPowerOff feature On and Off On = 0( means that gap power is Off) Off = 1( means gap power is On)
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SetVsg_GapPower(double rfFreqHz, double rfPowerLeveldBm, int port, int gapPowerOff);

//! Sets up VSGs for IQnxn
/*!
 * \param[in] rfFreqHz The center frequency of all VSGs (Hz). Set real center frequency for 160MHz contiguous signal. Set center frequency of the first 80MHz for 80+80 signal.
 * \param[in] rfGainDb[4] The output power levels of VSGs (dBm).  There must be 4 elements in the double array
 * \param[in] port[4] The port to which the VSGs connect. A 4 integer array, with the following options for each element:
 *				- =1: OFF
 *				- =2: RF port 1 (RF1)
 *				- =3: RF port 2 (RF2)
 *				- =4: RF port 3 (RF3)
 *				- =5: RF port 4 (RF4)
 * \param[in] dFreqShiftHz Frequency Shift
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SetVsgNxN(double rfFreqHz, double rfPowerLeveldBm[], int port[], double dFreqShiftHz = 0.0);

//! Sets up VSGs for MIMO
/*!
 * \param[in] rfFreqHz The center frequency of all VSGs (Hz). Set real center frequency for 160MHz contiguous signal. Set center frequency of the first 80MHz for 80+80 signal.
 * \param[in] rfPowerLeveldBm[] The output power levels of VSGs (dBm).
 * \param[in] port[] The port to which the VSGs connect to. If multiple ports are connected to the same VSG, specify all the ports for VSG1, then VSG2/3/4... The following are options for each element:
 *				- =1: OFF
 *				- =2: RF port 1 (RF1)
 *				- =3: RF port 2 (RF2)
 *				- =4: RF port 3 (RF3)
 *				- =5: RF port 4 (RF4)
 * \param[in] portSize[] The port size to enable which the VSGs connect to the number of RFx port.
 * \param[in] dFreqShiftHz Frequency Shift
 * \param[in] enableMultiPort
 *              - =true:  Enable VSG to connect to multiple ports at the same time, valid only for IQxel-M testers
 *              - =false: VSG can only connect to one port specified in port[] with one value
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int       LP_SetVsgNxN_MultiPort(double rfFreqHz, double rfPowerLeveldBm[], int port[], int portSize[], double dFreqShiftHz = 0.0, bool enableMultiPort = false);

//! Sets up VSG for transmitting CW
/*!
 * \param[in] sineFreqHz The center frequency of VSG (Hz)
 * \param[in] offsetFrequencyMHz The offset frequency to the center.  The generated CW frequency (Hz) = sineFreqHz + offsetFrequencyMHz * 1e6
 * \param[in] rfPowerLeveldBm The output power level of CW (dBm)
 * \param[in] port The port to which the VSG connects, with the following options:
 *				- =1: OFF
 *				- =2: RF port 1 (RF1)
 *				- =3: RF port 2 (RF2)
 *				- =4: RF port 3 (RF3)
 *				- =5: RF port 4 (RF4)
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SetVsgCw(double sineFreqHz, double offsetFrequencyMHz, double rfPowerLeveldBm, int port);

//! Sets up VSG for transmitting CW; The difference between this function and LP_SetVsgCw() is that this function doesn't clear loaded waveforms and the VSG freq = sineFreqHz*1e6 + offsetFrequencyMHz
/*!
 * \param[in] sineFreqHz The center frequency of VSG (Hz)
 * \param[in] offsetFrequencyMHz The offset frequency to the center.  The generated CW frequency (Hz) = sineFreqHz + offsetFrequencyMHz * 1e6
 * \param[in] rfPowerLeveldBm The output power level of CW (dBm)
 * \param[in] port The port to which the VSG connects, with the following options:
 *				- =1: OFF
 *				- =2: RF port 1 (RF1)
 *				- =3: RF port 2 (RF2)
 *				- =4: RF port 3 (RF3)
 *				- =5: RF port 4 (RF4)
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_TransmitCw(double sineFreqHz, double offsetFrequencyMHz, double rfPowerLeveldBm, int port);


//! Sets up MIMO testers VSGs for transmitting CW (Only for IQxel)
/*!
 * \param[in] sineFreqHz The center frequency of VSG (Hz)
 * \param[in] offsetFrequencyMHz The offset frequency to the center.  The generated CW frequency (Hz) = sineFreqHz + offsetFrequencyMHz * 1e6
 * \param[in] rfPowerLeveldBm Array of output power level of CW (dBm) for corresponding VSG
 * \param[in] port Array of the port to corrsponding VSG connects, with the following options:
 *				- =1: OFF
 *				- =2: RF port 1 (RF1)
 *				- =3: RF port 2 (RF2)
 *				- =4: RF port 3 (RF3)
 *				- =5: RF port 4 (RF4)
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SetVsgCwNxN(double sineFreqHz, double offsetFrequencyMHz, double rfPowerLeveldBm[], int port[]);

//! Sets up VSG for transmitting CW; The difference between this function and LP_SetVsgCw() is that this function doesn't clear loaded waveforms and the VSG freq = sineFreqHz*1e6 + offsetFrequencyMHz
/*!
 * \param[in] sineFreqHz The center frequency of VSG (Hz)
 * \param[in] offsetFrequencyMHz The offset frequency to the center.  The generated CW frequency (Hz) = sineFreqHz + offsetFrequencyMHz * 1e6
 * \param[in] rfPowerLeveldBm Array of output power level of CW (dBm) for corresponding VSG
 * \param[in] port Array of the port to corrsponding VSG connects, with the following options:
 *				- =1: OFF
 *				- =2: RF port 1 (RF1)
 *				- =3: RF port 2 (RF2)
 *				- =4: RF port 3 (RF3)
 *				- =5: RF port 4 (RF4)
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_TransmitCwNxN(double sineFreqHz, double offsetFrequencyMHz, double rfPowerLeveldBm[], int port[]);


//! Saves Signal Arrays
/*!
 * \param[in] real[N_MAX_TESTERS]
 * \param[in] imag[N_MAX_TESTERS]
 * \param[in] length[N_MAX_TESTERS]
 * \param[in] sampleFreqHz[N_MAX_TESTERS]
 * \param[in] fileNameToSave
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SaveSignalArrays(double *real[N_MAX_TESTERS],
									  double *imag[N_MAX_TESTERS],
									  int length[N_MAX_TESTERS],
									  double sampleFreqHz[N_MAX_TESTERS],
									  char *fileNameToSave);

//! Save I/Q data in memory to MOD file and load to the VSG memory
/*!
 * \param[in] real A double pointer to the I data in memory, for multiple streams
 * \param[in] imag A double pointer to the Q data in memory, for multiple streams
 * \param[in] length An integer array indicating the I/Q data length for each stream
 * \param[in] modFileName The modulation file name
 * \param[in] normalization A flag indicating if normalization is needed.
 *				- 1=Normalize I/Q data (RMS power = 0dBm)
 *				- 0=Do not normalize I/Q data
 * \param[in] loadIQDataToVsg A flag indicating if the I/Q data will be loaded to VSG.
 *				- 1=load I/Q data to VSG
 *				- 0=do not load I/Q data to VSG
 *
 * \return ERR_OK if the function runs OK; otherwise call LP_GetErrorString() for detailed error message.
 *
 * \remark
 *         -# The sampling rate for I/Q data has to be 80MHz
 *         -# For multiple stream I/Q, "real" and "imag" assumes that two dimentional double
 *			  arrays are constructed in memory, the first dimension indicating number of streams, and
 *			  second dimension indicating the length of I/Q data, specified by the length array.
 */
IQMEASURE_API int		LP_SaveIQDataToModulationFile(double *real,
												double *imag,
												int length[N_MAX_TESTERS],
												char *modFileName,
												int  normalization,
												int loadIQDataToVsg);

//! Loads the modulation file (waveform) to VSG and performs auto transmit of the loaded VSG mod file in free run mode
/*!
 * \param[in] modFileName The waveform file to be loaded.
 * \param[in] loadInternalWaveform Choose to load modFileName from internal tester (1) or to upload it from PC (0). The default is (0).
 *            This option is only valid with IQxel tester otherwise it is ignored.
 *
 *            For example, using LP_SetVsgModulation( "./user/loadinternal.iqvsg", 1) means to load "./user/loadinternal.iqvsg" already existed in tester folder.
 *            Using LP_SetVsgModulation( "../../pc.iqvsg", 0) means to load "../../pc.iqvsg", one of waveform files in pc, into tester.
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SetVsgModulation(char *modFileName, int loadInternalWaveform = 0);

//! Loads the Multiple modulation file (waveform) to VSG and performs auto transmit of the loaded VSG mod file in free run mode
/*!
 * \param[in] modFileName Waveform names to be played; seperate in comma;
 * \param[in] Repeatplay Specify how many times each waveform is played;
 * \param[in] NumberofWaveforms Indicate number of waveforms to be played; This parameter is only effectvie when there is only one waveform name in the first parameter. 
 *	It will then add suffix to the waveforms in the format of "modFileName_1, modFileName_2....."; If the first parameter contains multiple waveforms, this parameter is ignored.
 * \param[in] loadInternalWaveform Choose to load modFile from internal tester (1) or to upload it from PC (0). The default is (0). This option is only valid with IQxel tester otherwise it is ignored.
 *            For more details, refer to the description in LP_SetVsgModulation().
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SetVsgModulation_MultiWaves(char *modFileName, int Repeatplay[], int NumberofWaveforms, int loadInternalWaveform = 0);

//! Set VSA integer parameter before doing analysis

/*!
 * \param[in] measurement The measurement name.  Please refer to \ref group_analysis_name "Analysis Name" for all
 * available analysis names
 * \param[in] parameter The parameter will specify which VSA parameter will be set.
 * \param[in] value The double value will be set in analysis.
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_SetAnalysisParameterDouble(char *measurement, char *parameter, double value);

//! Set VSA integer parameter before doing analysis
/*!
 * \param[in] measurement The measurement name.  Please refer to \ref group_analysis_name "Analysis Name" for all available analysis names
 * \param[in] parameter The parameter will specify which VSA parameter will be set.
 * \param[in] value The int value will be set in VSA.
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SetAnalysisParameterInteger(char *measurement, char *parameter, int value);

//! Set VSA integer array before doing analysis
/*!
 * \param[in] measurement The measurement name.  Please refer to \ref group_analysis_name "Analysis Name" for all available analysis names
 * \param[in] parameter The parameter will specify which VSA parameter will be set.
 * \param[in] value The Integer array will be set in VSA.
 * \param[in] valuesize The size of integer array.
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SetAnalysisParameterIntegerArray(char *measurement, char *parameter, int *value, int valuesize);

//! Copy one VSA capture data to another
/*!
 * \param[in] fromVsaNum The VSA number from which data capture will be copied.  Valid values are 1,2,3, or 4.
 * \param[in] toVsaNum The VSA number to which data capture will be copied.  Valid values are 1,2,3, or 4.
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 * \remark Both VSAs have to contain capture data, with the same length.
 */
IQMEASURE_API int		LP_CopyVsaCaptureData(int fromVsaNum, int toVsaNum);

//! Saves the captured data to a file (.sig)
/*!
 * \param[in] sigFileName Specifies the .sig file name
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SaveVsaSignalFile(char *sigFileName);

//! Saves a file from the tester to local pc
/*!
 * \param[in] sigFileNameOnTester Specifies the .iqvsg file name on tester
 * 
 * \param[in] sigFileNameOnPc Specifies the .iqvsg file name on PC
 *
 * \param[in] char* folderPath Specifies the folder path where the file gets saved to
 * 
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_SaveFileToLocal(char *sigFileNameOnTester, char *sigFileNameOnPc, char* folderPath);

//! Loads the signal file (.sig) for analysis
/*!
 * \param[in] sigFileName The path for the signal (.sig) file to be loaded
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_LoadVsaSignalFile(char *sigFileName);

//! Saves the captured data into .mod or .iqvsg(IQXel) (For VSG to play)
/*!
 * \param[in] modFileName The path for the signal (.mod) file to be saved
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 * \remark When a captured data is saved to a .mod file, the average power level will be normalized to 0dBm.
 */
IQMEASURE_API int		LP_SaveVsaGeneratorFile(char *modFileName);

//! Sets up VSA for Bluetooth data capturing
/*!
 * \param[in] rfFreqHz The center frequency of Bluetooth RF signal (Hz)
 * \param[in] rfAmplDb The amplitude of the peak power (dBm) of the signal
 * \param[in] port The port to which the VSG connects, with the following options:
 *				- =1: OFF
 *				- =2: RF port 1 (RF1)
 *				- =3: RF port 2 (RF2)
 *				- =4: RF port 3 (RF3)
 *				- =5: RF port 4 (RF4)
 * \param[in] triggerLevelDb The trigger level (dBm) used for signal trigger (ignored in Free-run and External Trigger Modes)
 * \param[in] triggerPreTime The pre-trigger time used for signal capture
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 * \remark For VSA to work optimally with the input signal, set rfAmplDb to the peak power of the input signal.
 */
IQMEASURE_API int		LP_SetVsaBluetooth(double rfFreqHz, double rfAmplDb, int port, double triggerLevelDb=-25, double triggerPreTime=10e-6);

//! Sets up VSA for data capturing
/*!
 * \param[in] rfFreqHz The center frequency of VSA (Hz)
 * \param[in] rfAmplDb The amplitude of the peak power (dBm) for VSA to work with
 * \param[in] port The port to which the VSG connects, with the following options:
 *				- =1: OFF
 *				- =2: RF port 1 (RF1)
 *				- =3: RF port 2 (RF2)
 *				- =4: RF port 3 (RF3)
 *				- =5: RF port 4 (RF4)
 * \param[in] extAttenDb The external attenuation (dB).  Set to 0 always.
 * \param[in] triggerLevelDb The trigger level (dBm) used for signal trigger (ignored in Free-run and External Trigger Modes)
 * \param[in] triggerPreTime The pre-trigger time used for signal capture
 * \param[in] dFreqShiftHz Frequency Shift
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 * \remark For VSA to work optimally with the input signal, set rfAmplDb to the peak power of the input signal.
 */
IQMEASURE_API int		LP_SetVsaDECT(double rfFreqHz, double rfAmplDb, int port, double extAttenDb=0, double triggerLevelDb=-25, double triggerPreTime=10e-6, double dFreqShiftHz = 0.0);

//! Sets up VSA for data capturing
/*!
 * \param[in] rfFreqHz The center frequency of VSA (Hz). Set real center frequency for 160MHz contiguous signal. Set center frequency of the first 80MHz for 80+80 signal.
 * \param[in] rfAmplDb The amplitude of the peak power (dBm) for VSA to work with
 * \param[in] port The port to which the VSG connects, with the following options:
 *				- =1: OFF
 *				- =2: RF port 1 (RF1)
 *				- =3: RF port 2 (RF2)
 *				- =4: RF port 3 (RF3)
 *				- =5: RF port 4 (RF4)
 * \param[in] extAttenDb The external attenuation (dB).  Set to 0 always.
 * \param[in] triggerLevelDb The trigger level (dBm) used for signal trigger (ignored in Free-run and External Trigger Modes)
 * \param[in] triggerPreTime The pre-trigger time used for signal capture
 * \param[in] dFreqShiftHz Frequency Shift
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 * \remark For VSA to work optimally with the input signal, set rfAmplDb to the peak power of the input signal.
 */
IQMEASURE_API int		LP_SetVsa(double rfFreqHz, double rfAmplDb, int port, double extAttenDb=0, double triggerLevelDb=-25, double triggerPreTime=10e-6, double dFreqShiftHz = 0.0);

//! Set VSA/VSG seperation in Mhz (IQxel160 only)
/*!
 * \param[in] frequencySeparationInHz Set the seperation between two VSA/VSG (For example use IQxel160 to test 160Mhz mode, two VSAs are seperated by 80 Mhz)
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SetVsaVsgSeperation(double frequencySeparationInHz);

//! Set timeout for VSA Trigger process
/*!
 * \param[in] triggerTimeoutSec Trigger timeout(second)
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SetVsaTriggerTimeout(double triggerTimeoutSec);

//! Set VSA Trigger settings
/*!
 * \param[in] triggerSource Trigger source used for capturing.  Valid options are:
 *      - 1 = Free-run
 *      - 2 = External trigger
 *      - 6 = Signal Trigger
 * \param[in] triggerType Trigger type used for capturing.  Valid options are:
 *      - 0 = Positive Edge
 *      - 1 = Negative Edge
 *      - 2 = Level
 * \param[in] triggerLevelDb The trigger level (dBm) used for signal trigger (ignored in Free-run and External Trigger Modes)
 * \param[in] triggerMode Trigger mode used for capturing.  Valid options are:
 *      - 0 = SSHot
 *      - 1 = MPACket
 *      - 2 = MSHot
 * \param[in] triggerGapSec Trigger gap time(second)
 * \param[in] triggerPreTime The pre-trigger time used for signal capture
 * \param[in] triggerTimeoutSec Trigger timeout(second)
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SetVsa_Trigger(int triggerSource = 6, int triggerType = 0, double triggerLevelDb = -25, int triggerMode = 0, double triggerGapSec = 6e-6, double triggerPreTime = 10e-6, double triggerTimeoutSec = 1.5);

//! Sets up VSAs for IQnxn
/*!
 * \param[in] rfFreqHz The center frequency of VSA (Hz). Set real center frequency for 160MHz contiguous signal. Set center frequency of the first 80MHz for 80+80 signal.
 * \param[in] rfAmplDb[4] The amplitude of the peak power (dBm) for VSA to work with.  A 4 double array.
 * \param[in] port[4] The port to which the VSG connects. A 4 integer array, with the following options for each element:
 *				- =1: OFF
 *				- =2: RF port 1 (RF1)
 *				- =3: RF port 2 (RF2)
 *				- =4: RF port 3 (RF3)
 *				- =5: RF port 4 (RF4)
 * \param[in] extAttenDb The external attenuation (dB).  Set to 0 always.
 * \param[in] triggerLevelDb The trigger level (dBm) used for signal trigger (ignored in Free-run and External Trigger Modes)
 * \param[in] triggerPreTime The pre-trigger time used for signal capture
 * \param[in] dFreqShiftHz Frequency Shift
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 * \remark For VSA to work optimally with the input signal, set rfAmplDb to the peak power of the input signal.
 */
IQMEASURE_API int		LP_SetVsaNxN(double rfFreqHz, double rfAmplDb[], int port[], double extAttenDb=0, double triggerLevelDb=-25, double triggerPreTime=10e-6, double dFreqShiftHz = 0.0);

//! Set Vsa amplitude tolerance in dB
/*!
 * \param[in] amplitudeToleranceDb, the setting of Vsa amplitude tolerance
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SetVsaAmplitudeTolerance(double amplitudeToleranceDb);

//! Set the AGC window time (for IQxel only)
/*!
 * \param[in] agc window time in second. default: 0.005 second ( 5 ms )
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SetAgcWindowTime(double WindowTime = 0.005);

//! Get the AGC window time (for IQxel only)
/*!
 * \param[out] agc window time in second.
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_GetAgcWindowTime(double *WindowTime);
//! Performs AGC (Automatic Gain Control) on VSA
/*!
 * \param[out] rfAmplDb The setting of rfAmplDb of VSA set by AGC
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_Agc(double *rfAmplDb, bool allTesters = true);

//! Sets up the number of frames for VSG to transmit
/*!
 * \param[in] frameCnt The number of frames to be transmitted between 0 and 65534, where "0" indicates continuous transmission
 *
 * \return ERR_OK if number of frames accepted; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SetFrameCnt(int frameCnt);

//! Sets path loss table for different ports
/*!
 * \param[in] VSGtableIndex Pathloss table index array applied to each port for VSG, 0 means turn off the pathloss table
 * \param[in] VSAtableIndex Pathloss table index array applied to each port for VSA, 0 means turn off the pathloss table
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SetPathLossTable(unsigned int VSGtableIndex[MAX_MPTA_PORTS], unsigned int VSAtableIndex[MAX_MPTA_PORTS]);

//! Check if TX is done
/*!
 * \return ERR_OK if TX is done; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_TxDone(void);

//! Turn ON/OFF the first VSG RF
/*!
 * \param[in] enabled 1 to turn on the first VSG RF; 0 to turn off the first VSG RF
 *
 * \return ERR_OK if the first VSG RF is turned on or off; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_EnableVsgRF(int enabled);

//! Turn ON/OFF the VSG RF for IQnxn
/*!
 * \param[in] rx1Enabled 1 to turn on the first RX RF; 0 to turn off the first RX RF
 * \param[in] rx2Enabled 1 to turn on the second RX RF; 0 to turn off the second RX RF
 * \param[in] rx3Enabled 1 to turn on the third RX RF; 0 to turn off the third RX RF
 * \param[in] rx4Enabled 1 to turn on the fourth RX RF; 0 to turn off the fourth RX RF
 *
 * \return ERR_OK if the first VSG RF is turned on or off; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_EnableVsgRFNxN(int rx1Enabled = 1, int rx2Enabled = 1, int rx3Enabled = 1, int rx4Enabled = 1);

//! Turn ON/OFF the specified VSG RF
/*!
 * \param[in] enabled 1 to turn on the specified VSG RF; 0 to turn off the specified VSG RF
 * \param[in] vsgNumber The specified VSG.
 *
 * \return ERR_OK if the specified VSG RF is turned on or off; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_EnableSpecifiedVsgRF(int enabled, int vsgNumber);

//! Turn ON/OFF the specified VSA RF
/*!
 * \param[in] enabled 1 to turn on the specified VSA RF; 0 to turn off the specified VSA RF
 * \param[in] vsaNumber The specified VSA.
 *
 * \return ERR_OK if the specified VSA RF is turned on or off; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_EnableSpecifiedVsaRF(int enabled, int vsaNumber);

//! Return the VSA settings
/*!
 * \param[out] freqHz VSA frequency (Hz) setting
 * \param[out] ampl VSA amplitude (dBm);
 * \param[out] port VSA port: 1=PORT_OFF, 2=PORT_LEFT, 3=PORT_RIGHT, 4=PORT_BB;
 * \param[out] rfEnabled VSA RF state: 0=disalbed; 1=enabled
 * \param[out] triggerLevel VSA trigger level
 *
 * \return ERR_OK if the VSA settings are returned; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_GetVsaSettings(double *freqHz, double *ampl, IQAPI_PORT_ENUM *port, int *rfEnabled, double *triggerLevel, int *triggertype=NULL);

//! Perform VSA data capture
/*!
 * \param[in] samplingTimeSecs Capture time in seconds
 * \param[in] triggerType Trigger type used for capturing.  Valid options are:
 *      - 1 = Free-run
 *      - 2 = External trigger
 *      - 6 = Signal Trigger
 *		- 16 = Level Based Signal Trigger
 * \param[optinal] modeHT40 Specifies the capture type. Valid options are:
 *		- 0 = normal capture, default
 *		- 1 = HT40 wideband capture, for HT40 mask on IQ2010
 *		- 6 = VHT80 wideband capture
 *		- 7 = VHT160 wideband capture
 *      - 8 = 20/40MHz in 80MHz wideband capture
 * \param[in] sampleFreqHz IQxel options: 80Mhz/160Mhz;IQ2010/IQnxn/IQview 80Mhz only (Forced to set as 80Mhz if set different from 80Mhz)
 * \return ERR_OK if the data capture is successful; otherwise call LP_GetErrorString() for detailed error message.
 * \remark modeHT40 only needs to set to 1 if the HT40 mask (120MHz) analysis is desired.  For LP_Analyze80211n(), this flag can be set to 0.
 */
IQMEASURE_API int		LP_VsaDataCapture(double samplingTimeSecs, int triggerType=6, double sampleFreqHz=160e6,
									int ht40Mode=OFF, IQMEASURE_CAPTURE_NONBLOCKING_STATES nonBlockingState=BLOCKING );

//! Perform VSA data capture

/*!
 * \param[in] samplingTimeSecs Capture time in seconds
 * \param[in] triggerType Trigger type used for capturing.  Valid options are:
 *      - 1 = Free-run
 *      - 2 = External trigger
 *      - 6 = Signal Trigger
 * \param[optinal] modeHT40 Specifies the capture type. Valid options are:
 *		- 0 = normal capture, default
 *		- 1 = HT40 wideband capture, for HT40 mask on IQ2010
 *		- 6 = VHT80 wideband capture
 *		- 7 = VHT160 wideband capture
 * \param[in] sampleFreqHz IQxel options: 80Mhz/160Mhz;IQ2010/IQnxn/IQview 80Mhz only (Forced to set as 80Mhz if set
 * different from 80Mhz)
 * \param[optional] segmentMode specifies if the capture should overwrite previous capture, or append to it. Valid
 * options are:
 *		- 0 = overwrite previous capture, default
 *		- 1 = append to previous capture
 * \return ERR_OK if the data capture is successful; otherwise call LP_GetErrorString() for detailed error message.
 * \remark modeHT40 only needs to set to 1 if the HT40 mask (120MHz) analysis is desired.  For LP_Analyze80211n(), this
 * flag can be set to 0.
 */
IQMEASURE_API int       LP_VsaCapture(double samplingTimeSecs,
                                      int triggerType = 6,
                                      double sampleFreqHz = 160e6,
                                      int ht40Mode = OFF,
                                      IQMEASURE_CAPTURE_NONBLOCKING_STATES nonBlockingState = BLOCKING,
                                      SEGMENT_CAPTURE_MODE segmentMode = SINGLE);

//! Perform VSA data capture on a single stream signal while working in the MIMO configuration, specifically for CW analysis
/*!
 * \param[in] samplingTimeSecs Capture time in seconds
 * \param[in] streamIndex stream index of the signal to be captured
 * \param[in] triggerType Trigger type used for capturing.  Valid options are:
 *      - 1 = Free-run
 *      - 2 = External trigger
 *      - 6 = Signal Trigger
 *		- 16 = Level Based Signal Trigger
 * \param[optinal] modeHT40 Specifies the capture type. Valid options are:
 *		- 0 = normal capture, default
 *		- 1 = HT40 wideband capture, for HT40 mask on IQ2010
 *		- 6 = VHT80 wideband capture
 *		- 7 = VHT160 wideband capture
 *      - 8 = 20/40MHz in 80MHz wideband capture
 * \param[in] sampleFreqHz IQxel options: 80Mhz/160Mhz;IQ2010/IQnxn/IQview 80Mhz only (Forced to set as 80Mhz if set different from 80Mhz)
 * \return ERR_OK if the data capture is successful; otherwise call LP_GetErrorString() for detailed error message.
 * \remark modeHT40 only needs to set to 1 if the HT40 mask (120MHz) analysis is desired.  For LP_Analyze80211n(), this flag can be set to 0.
 */
IQMEASURE_API int		LP_VsaDataCaptureSingleMimoStream(double samplingTimeSecs,int streamIndex, int triggerType=6, double sampleFreqHz=160e6,
									int ht40Mode=OFF, IQMEASURE_CAPTURE_NONBLOCKING_STATES nonBlockingState=BLOCKING );

//! Retrieves the captured I/Q raw data from VSA
/*!
 * \param[in] vsaNum Number of VSA (0-3)
 * \param[out] bufferReal[] Returns I samples
 * \param[out] bufferImag[] Returns Q samples
 * \param[in] bufferLength Indicates the length of bufferReal and bufferImag (in bytes)
 *
 * \return ERR_OK if no erros; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_GetSampleData(int vsaNum, double bufferReal[], double bufferImag[], int bufferLength);

//! Get memory addresses for a capture
/*!
 * \param[out] real[] Returns I samples.
 * \param[out] imag[] Returns Q samples.
 * \param[out] length[] Returnes length.
 * \param[in] arraySize Size of array real[], imag[], and length[]
 *
 * \return ERR_OK if no erros; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_GetHndlDataPointers(double *real[],double *imag[],int *length, double *sampleFreqHz, int arraySize);

//! Save I/Q samples in user's memory to LP sig file
/*!
 * \param[in] sigFileName The .sig file name
 * \param[in] real[] start pointer of I samples.
 * \param[in] imag[] start pointer of Q samples.
 * \param[in] length[] Length.
 * \param[in] arraySize Size of array real[], imag[], and length[]
 *
 * \return ERR_OK if no erros; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SaveUserDataToSigFile(char* sigFileName,
                                           double *real[],
                                           double *imag[],
                                           int *length,
                                           double *sampleFreqHz,
                                           int arraySize);

//! Select a portion of the capture for analysis
/*!
 * \param[in] startPositionUs Start position in the capture (us) for analysis
 * \param[in] lengthUs The length (us) in the capture for analysis
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SelectCaptureRangeForAnalysis(double startPositionUs, double lengthUs,
														 int packetsOffset = 0, int packetsLength = 1);

//! Select a portion of the capture for preamble power analysis
/*!
 * \param[in] startPositionUs Start position in the capture (us) for analysis
 * \param[in] stopPositionUs Stop position in the capture (us) for analysis
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SelectCaptureRangeForPreamPowerAnalysis(double startPositionUs = 8.8, double stopPositionUs = 15.2);


//! Specify capture packet data for analysis
/*!
 * \param[in] packetLengthUs : length of one packet in microseconds
 * \param[in] preTriggerTimeUs : the offset pre-trigger time in microseconds
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SetCaptureData( double packetLengthUs, double preTriggerTimeUs );

//! Specify special mode for analysis
/*!
 * \param[in] keyword 
 * \param[in] values : values for the keywords
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SetMode(char* keyword, int value);

//! Get special mode
/*!
 * \param[in] keyword 
 * \param[in] values : values for the keywords
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_GetMode(char* keyword, int* value);
//! Send a SCPI command to IQxel tester
/*!
 * \param[in] The string of SCPI command
 * \return ERR_OK if no errors;
 */
IQMEASURE_API int		LP_ScpiCommandSet(char *scpiCommand);

//! Send a SCPI command to IQxel tester
/*!
 * \param[in] The string of query SCPI command
 * \param[in] The Max size of response message from IQxel
 * \param[out] Response from IQxel tester
 * \param[out] Actual size of response message from IQxel
 * \return ERR_OK if no errors;
*/
IQMEASURE_API int		LP_ScpiCommandQuery(char query[], unsigned int max_size, char response[], unsigned int *actual_size);

//! Send a SCPI command with Binary to IQxel tester
/*!
 * \param[in] The string of SCPI command
 * \param[in] The size of following binary in byte
 * \param[in] The pointer to the binary buffer
 * \return ERR_OK if no errors;
*/

IQMEASURE_API int		LP_ScpiCommandSetInBinary(char *scpiCommand, unsigned int binary_data_size, char* binary_data); 
//! Upload binary data  to IQxel tester
/*!
 * \param[in] filename
 * \param[in] filename on tester
 * \return ERR_OK if no errors;
*/
IQMEASURE_API int		LP_UploadBinaryFile(char *filename, char *filenameOnTester); 

// Analysis Functions (TODO: Bluetooth)


//! Perform 802.16d Analysis on current capture
/*!
 * \param[in] sigType Indicates the type of signal. Default: -1
 *		      Valid values are as follows:
 *		      1=downlink signal
 *		      2=uplink signal
 *		     -1=auto-detect
 * \param[in] bandwidthHz Indicates signal bandwidth. Default: -1
 *		      Valid values for the signal bandwidth are as follows:
 *		      -1, 1.25, 1.5, 1.75, 2.5, 3, 3.5, 5, 5.5, 6, 7, 8.75, 10, 11, 12, 14, 15, 20
 *		      -1=auto detect
 * \param[in] cyclicPrefix Indicates signal cyclic prefix ratio. Default: -1
 *            Valid values for signal cyclic prefix ratio are as follows: -1, 1/4, 1/8, 1/16, 1/32
 *            -1=auto detect
 * \param[in] rateId Indicates signal modulation rate. Default: -1
 *            Valid values for signal modulation rate are as follows: 0, 1, 2, 3, 4, 5, 6 and
 *            correspond to {BPSK 1/2, QPSK 1/2, QPSK 3/4, 16-QAM 1/2, 16-QAM 3/4, 64-QAM 2/3, 64-QAM 3/4}, respectively
 *            -1=auto detect
 * \param[in] numSymbols Number of OFDM symbols in burst. Default: -1
 *            -1=auto detect
 * \param[in] ph_corr Phase Correction Mode with the following valid options:
 *         - 1: Phase correction off
 *         - 2: Symbol-by-symbol correction (Default)
 *         - 3: Moving avg. correction (10 symbols)
 * \param[in] ch_corr Channel Estimate with the following options:
 *         - 1: Raw Channel Estimate (based on long trainling symbols) (Default)
 *         - 2: 2nd Order Polyfit
 *         - 3: Full packet estimate
 * \param[in] freq_corr Frequency Sync. Mode with the following options:
 *         - 1: Short Training Symbol
 *         - 2: Long Training Symbol (Default)
 *         - 3: Full Data Packet
 * \param[in] timing_corr Symbol Timing Correction with the following options:
 *         - 1: Symbol Timing Correction Off
 *         - 2: Symbol Timing Correction ON (Default)
 * \param[in] ampl_track Amplitude Tracking with the following options:
 *         - 1: Amplitude tracking off (Default)
 *         - 2: Amplitude tracking on
 * \param[in] decode Decode the payload or not //TODO: check decode
 *         - 0: Decode OFF (Default)
 *         - 1: Decode ON
 *
 * \return ERR_OK if the 802.16d analysis succeeded; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_Analyze80216d( double sigType		= -1,
								    double bandwidthHz	= -1,
									double cyclicPrefix	= -1,
									double rateId		= -1,
									double numSymbols	= -1,
								    int ph_corr			= 2,
								    int ch_corr			= 1,
								    int freq_corr		= 2,
								    int timing_corr		= 2,
								    int ampl_track		= 1,
								    double decode		= 0
								    );

//! Perform 802.16e Analysis on current capture
/*!
 * \param[in] sigType Indicates the type of signal. Default: -1
 *		      Valid values are as follows:
 *		      1=downlink signal
 *		      2=uplink signal
 *		     -1=auto-detect
 * \param[in] bandwidthHz Indicates signal bandwidth. Default: -1
 *		      Valid values for the signal bandwidth are as follows:
 *		      -1, 1.25, 1.5, 1.75, 2.5, 3, 3.5, 5, 5.5, 6, 7, 8.75, 10, 11, 12, 14, 15, 20
 *		      -1=auto detect
 * \param[in] cyclicPrefix Indicates signal cyclic prefix ratio. Default: -1
 *            Valid values for signal cyclic prefix ratio are as follows: -1, 1/4, 1/8, 1/16, 1/32
 *            -1=auto detect
 * \param[in] rateId Indicates signal modulation rate. Default: -1
 *            Valid values for signal modulation rate are as follows: 0, 1, 2, 3, 4, 5, 6 and
 *            correspond to {BPSK 1/2, QPSK 1/2, QPSK 3/4, 16-QAM 1/2, 16-QAM 3/4, 64-QAM 2/3, 64-QAM 3/4}, respectively
 *            -1=auto detect
 * \param[in] numSymbols Number of OFDM symbols in burst. Default: -1
 *            -1=auto detect
 * \param[in] ph_corr Phase Correction Mode with the following valid options:
 *         - 1: Phase correction off
 *         - 2: Symbol-by-symbol correction (Default)
 *         - 3: Moving avg. correction (10 symbols)
 * \param[in] ch_corr Channel Estimate with the following options:
 *         - 1: Raw Channel Estimate (based on long trainling symbols) (Default)
 *         - 2: 2nd Order Polyfit
 *         - 3: Full packet estimate
 * \param[in] freq_corr Frequency Sync. Mode with the following options:
 *         - 1: Short Training Symbol
 *         - 2: Long Training Symbol (Default)
 *         - 3: Full Data Packet
 * \param[in] timing_corr Symbol Timing Correction with the following options:
 *         - 1: Symbol Timing Correction Off
 *         - 2: Symbol Timing Correction ON (Default)
 * \param[in] ampl_track Amplitude Tracking with the following options:
 *         - 1: Amplitude tracking off (Default)
 *         - 2: Amplitude tracking on
 * \param[in] decode Decode the payload or not //TODO: check decode
 *         - 0: Decode OFF (Default)
 *         - 1: Decode ON
 * \param[in] map_conf_file The map configuration file.
 *
 * \return ERR_OK if the 802.16e analysis succeeded; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_Analyze80216e( double sigType		= -1,
								    double bandwidthHz	= -1,
									double cyclicPrefix	= -1,
									double rateId		= -1,
									double numSymbols	= -1,
								    int ph_corr			= 2,
								    int ch_corr			= 1,
								    int freq_corr		= 2,
								    int timing_corr		= 2,
								    int ampl_track		= 1,
								    double decode		= 0,
								    char *map_conf_file = NULL
									);

//! Perform 802.11p Analysis on current capture
/*!
 * \param[in] ph_corr_mode Phase Correction Mode with the following valid options:
 *         - 1: Phase correction off
 *         - 2: Symbol-by-symbol correction (Default)
 *         - 3: Moving avg. correction (10 symbols)
 * \param[in] ch_estimate Channel Estimate with the following options:
 *         - 1: Raw Channel Estimate (based on long trainling symbols) (Default)
 *         - 2: 2nd Order Polyfit
 *         - 3: Full packet estimate
 * \param[in] sym_tim_corr Symbol Timing Correction with the following options:
 *         - 1: Symbol Timing Correction Off
 *         - 2: Symbol Timing Correction ON (Default)
 * \param[in] freq_sync Frequency Sync. Mode with the following options:
 *         - 1: Short Training Symbol
 *         - 2: Long Training Symbol (Default)
 *         - 3: Full Data Packet
 * \param[in] ampl_track Amplitude Tracking with the following options:
 *         - 1: Amplitude tracking off (Default)
 *         - 2: Amplitude tracking on
 * \param[in] ofdm_mode Specifies OFDM mode with the following options:
 *         - 0: OFDM mode based on IEEE 802.11a or 802.11g standards specification
 *         - 1: OFDM turbo mode based on IEEE 802.11a or 802.11g standards specification
 *         - 2: ASTM DSRC standards specification (Default)
 *         - 3: OFDM quarter rate
 *
 * \return ERR_OK if the 802.11p analysis succeeded; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_Analyze80211p(int ph_corr_mode=2, int ch_estimate=1, int sym_tim_corr=2, int freq_sync=2, int ampl_track=1, int ofdm_mode=2);

//! Perform 802.11ah Analysis on current capture
/*!
 * \param[in] ph_corr_mode Phase Correction Mode with the following valid options:
 *         - 1: Phase correction off
 *         - 2: Symbol-by-symbol correction (Default)
 *         - 3: Moving avg. correction (10 symbols)
 * \param[in] ch_estimate Channel Estimate with the following options:
 *         - 1: Raw Channel Estimate (based on long trainling symbols) (Default)
 *         - 2: 2nd Order Polyfit
 *         - 3: Full packet estimate
 * \param[in] sym_tim_corr Symbol Timing Correction with the following options:
 *         - 1: Symbol Timing Correction Off
 *         - 2: Symbol Timing Correction ON (Default)
 * \param[in] freq_sync Frequency Sync. Mode with the following options:
 *         - 1: Short Training Symbol
 *         - 2: Long Training Symbol (Default)
 *         - 3: Full Data Packet
 * \param[in] ampl_track Amplitude Tracking with the following options:
 *         - 1: Amplitude tracking off (Default)
 *         - 2: Amplitude tracking on
 * \return ERR_OK if the 802.11p analysis succeeded; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_Analyze80211ah(int ph_corr_mode=2, int ch_estimate=1, int sym_tim_corr=2, int freq_sync=2, int ampl_track=1);

//! Perform 802.11 a/g Analysis on current capture
/*!
 * \param[in] ph_corr_mode Phase Correction Mode with the following valid options:
 *         - 1: Phase correction off
 *         - 2: Symbol-by-symbol correction (Default)
 *         - 3: Moving avg. correction (10 symbols)
 * \param[in] ch_estimate Channel Estimate with the following options:
 *         - 1: Raw Channel Estimate (based on long trainling symbols) (Default)
 *         - 2: 2nd Order Polyfit
 *         - 3: Full packet estimate
 * \param[in] sym_tim_corr Symbol Timing Correction with the following options:
 *         - 1: Symbol Timing Correction Off
 *         - 2: Symbol Timing Correction ON (Default)
 * \param[in] freq_sync Frequency Sync. Mode with the following options:
 *         - 1: Short Training Symbol
 *         - 2: Long Training Symbol (Default)
 *         - 3: Full Data Packet
 * \param[in] ampl_track Amplitude Tracking with the following options:
 *         - 1: Amplitude tracking off (Default)
 *         - 2: Amplitude tracking on
 * \param[in] prePowStartSec indicates the start point of preamble for power measurement result "powerPreambleDbm" (IQxel Only):
          - 8.8e-6: 8.8 micro second (Default)
 * \param[in] prePowStopSec indicates the end point of preamble for power measurement result "powerPreambleDbm" (IQxel Only):
 *        - 15.2e-6: 15.2 micro second (Default)
 *
 * \return ERR_OK if the 802.11 a/g analysis succeeded; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_Analyze80211ag(int ph_corr_mode=2, int ch_estimate=1, int sym_tim_corr=2, int freq_sync=2, int ampl_track=1, double prePowStartSec = 8.8e-6, double prePowStopSec = 15.2e-6);

//! Perform 802.11 n Analysis on current capture
/*!
 * \param[in] type only "EWC" is supported
 * \param[in] mode Valid options are:
 *         - "nxn": for true 802.11n capture
 *         - "composite: for a composite capture
 *         - "evms": for EVM per signal analysis
 *         - "sequential_mimo": for a switched capture
 * \param[in] enablePhaseCorr Phase Correction Mode:
 *         - 0: Disable
 *         - 1: Enable (Default)
 * \param[in] enableSymTimingCorr Symbol Timing Correction:
 *         - 0: Disable
 *         - 1: Enable (Default)
 * \param[in] enableAmplitudeTracking Amplitude Tracking:
 *         - 0: Disable (Default)
 *         - 1: Enable
 * \param[in] decodePSDU Decode PSDU:
 *         - 0: Disable 
 *         - 1: Enable (Default)
 * \param[in] enableFullPacketChannelEst Channel Estimate on full packet:
 *         - 0: Disable (Default)
 *         - 1: Enable
 * \param[in] referenceFile PSDU reference file (needed for Composite EVM)
 * \param[in] packetFormat indicates format of the 80211n MIMO packet:
 *         - 0: auto-detect mode (Default)
 *         - 1: mixed format
 *         - 2: greenfield format
 * \param[in] frequency Correction options 
 *         - 2 = frequency correction on short and long legacy training fields (default)
 *         - 3 = frequency correction based on full packet
 *		   - 4 = frequency correction on signal fields (legacy and HT) in addition to short and long training fields
 * \param[in] prePowStartSec indicates the start point of preamble for power measurement result "powerPreambleDbm" (IQxel Only):
           - 8.8e-6: 8.8 micro second (Default)
 * \param[in] prePowStopSec indicates the end point of preamble for power measurement result "powerPreambleDbm" (IQxel Only):
 *         - 15.2e-6: 15.2 micro second (Default)
 *
 * \return ERR_OK if the 802.11n analysis succeeded; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_Analyze80211n(char *type,
                                   char *mode,
                                   int enablePhaseCorr            = 1,
                                   int enableSymTimingCorr        = 1,
                                   int enableAmplitudeTracking    = 0,
                                   int decodePSDU                 = 1,
                                   int enableFullPacketChannelEst = 0,
                                   char *referenceFile=NULL,
                                   int packetFormat = 0,
								   int frequencyCorr = 2,
								   double prePowStartSec = 8.8e-6,
								   double prePowStopSec = 15.2e-6);

//! LP_AnalyzeMimo has been replaced by LP_Analyze802.11n
IQMEASURE_API int		LP_AnalyzeMimo(char *type, char *mode, int enablePhaseCorr, int enableSymTimingCorr, int enableAmplitudeTracking, int decodePSDU, int enableFullPacketChannelEst, char *referenceFile);


//! Perform 802.11b Analysis on current capture
/*!
 * \param[in] eq_taps Number of equalizer taps, valid options are:
 *         - 1: Equalizer Off	(Default)
 *         - 5: 5 Taps Equalizer
 *         - 7: 7 Taps Equalizer
 *         - 9: 9 Taps Equalizer
 * \param[in] DCremove11b_flag DC removal, valid options are:
 *         - 0: DC Removal OFF (Default)
 *         - 1: DC Removal ON
 * \param[in] method_11b 802.11b method, valid options are:
 *         - 1: Use 11b standard TX accuracy (Default)
 *         - 2: Use 11b RMS error vector
  *        - 3: Use 11b Std 2007 TX accuracy
 * \param[in] prePowStartSec indicates the start point of preamble for power measurement result "powerPreambleDbm" (IQxel Only):
          - 8.8e-6: 8.8 micro second (Default)
 * \param[in] prePowStopSec indicates the end point of preamble for power measurement result "powerPreambleDbm" (IQxel Only):
 *        - 15.2e-6: 15.2 micro second (Default)
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_Analyze80211b(int eq_taps          = 1,
                                   int DCremove11b_flag = 0,
                                   int method_11b       = 1,
								   double prePowStartSec = 8.8e-6,
								   double prePowStopSec = 15.2e-6);

//! Perform power Analysis on current capture
/*!
 * \param[in] T_interval Gap between packets (bursts).  Ignored
 * \param[in] max_pow_diff_dB Power difference between noise floor and packets.  Ignored
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_AnalyzePower(double T_interval=0.0, double max_pow_diff_dB=0.0);



//! Perform power Analysis on current capture for specific technology
/*!
 * \param[in] technology type, specify the which technology standard limit to be used;
 *         - 0  WIFI
 *         - 1  Bluetooth
 *         - 2  ZigBee
 *         - 3  DECT
 *         - 4  Z-Wave 
 *         
 * \param[in] T_interval Gap between packets (bursts).  Ignored
 * \param[in] max_pow_diff_dB Power difference between noise floor and packets.  Ignored
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_AnalyzeTechnologyPower(int technology, double T_interval=0.0, double max_pow_diff_dB=0.0);




//! Perform Generic FFT Analysis on current capture
/*!
 * \param[in] NFFT Number of FFT.  Default to 0
 * \param[in] res_bw Resolution bandwidth.  Default to 100KHz
 * \param[in] window_type FFT window type.  Default to NULL
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_AnalyzeFFT(double NFFT=0, double res_bw=100e3, char *window_type=NULL);
//! Perform FFT Analysis on current capture and apply technology specified spectrum mask limit (Only for IQxel tester)
/*
 * \param[in] technology type, specify the which technology standard limit to be used;
 *		   - 0  11ac (auto detect 20/40 Mhz bandwidth; Since 11ac is not defined in 2.4G band, will use 11n limit in 2.4G band)
 *		   - 1  11n	(auto detect 20/40 Mhz bandwidth and 2.4G/5G band; )
 *		   - 2  11ag	
 *		   - 3  11b
 *		   - 4  11ac 80MHz and 160MHz
 *		   - 99 Auto detect packet format (use when technology is unknown. Using raw data for result calculation is recommended.)
 *
 *
 * \param[in] NFFT Number of FFT.  Default to 0
 * \param[in] res_bw Resolution bandwidth.  Default to 100KHz
 * \param[in] window_type FFT window type.  Default to NULL
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_AnalyzeTechnologyFFT(int technology=99, double NFFT=0, double res_bw=100e3, char *window_type=NULL);

IQMEASURE_API int		LP_AnalyzeVHT80TechnologyFFT(double NFFT = 0, double res_bw = 100e3, char *window_type = NULL);

//! Perform CCDF Analysis on current capture
/*!
 * \return ERR_OK if the CCDE analysis succeeded; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_AnalyzeCCDF();

//! Perform CW Frequency Analysis on current capture (Replaced by LP_AnalyzeCWFreq)
/*!
 * \return ERR_OK if the CW analysis succeeded; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_AnalyzeCW();

//! Perform CW Frequency Analysis on current capture
/*!
 * \return ERR_OK if the CW analysis succeeded; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_AnalyzeCWFreq();

//! Perform Wave Analysis on current capture //TODO: what are results from AnalysisWave()?
/*!
 * \return ERR_OK if the Wave analysis succeeded; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_AnalysisWave();

//! Perform Sidelobe Analysis on current capture
/*!
 * \return ERR_OK if the Sidelobe analysis succeeded; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_AnalyzeSidelobe();

//! Perform Power Ramp Analysis (OFDM) on current capture
/*!
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_AnalyzePowerRampOFDM();

//! Perform Power Ramp Analysis (80211b) on current capture
/*!
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_AnalyzePowerRamp80211b();

//! Perform Bluetooth Analysis on current capture
/*!
 * \param[in] data_rate, Bluetooth data rate, with the following options:
 *         - 1: 1Mbit/s BDR
 *         - 2: 2Mbit/s EDR
 *         - 3: 3Mbit/s EDR
 *         - 0: Auto detect
 * \param[in] analysis_type, Specifies what type of analysis to perform. Default: 'All'. Valid values are as follows:
 *         - PowerOnly
 *         - 20dbBandwidthOnly
 *         - PowerandFreq
 *         - All  (This is the set default value)
 *		   - ACP  (only does the new ACP analysis)
 *		   - AllPlus  (performs all the analyses that are done by ¡®All?plus the ACP analysis)
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_AnalyzeBluetooth(double data_rate, char *analysis_type="All"  );

//! Perform Zigbee Analysis on current capture
/*!
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_AnalyzeZigbee();

//! Perform Z-WAVE Analysis on current capture
/*!
 * \param[in] data_rate, Z-WAVE data rate, with the following options:
 *         - 1: R1, 9.6 kbps
 *         - 2: R2, 40  kbps
 *         - 3: R3, 100 kbps 
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_AnalyzeZwave(double data_rate);



//! Perform WiSUN Analysis on current capture
/*!
 * \param[in] fskFSDType, Sets the SFD type for FSK packet acquisition. Only signals with correct SFD word can be successfully analyzed. The options are as follows:
 *         - MOD_2FSK
 *         - MOD_4FSK
 * \param[in] fskFSDWord, Sets the user specified SFD word for FSK packet acquisition
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_WiSUN_FSK_SetSFDType(FSK_FSD_TYPE fskFSDType =  FSK_SPEC, char *fskFSDWord = NULL);

//! Perform WiSUN Analysis on current capture
/*!
 * \param[in] standard, Sets the standard type for WiSUN signal analysis, only FSK supported for now
 * \param[in] modType_FSK, Sets the modulation type for FSK signal analysis with the following options:
 *         - MOD_2FSK
 *         - MOD_4FSK
 * \param[in] dataRate_FSK, Sets the Data Rate for FSK packet analysis
 * \param[in] modIndex_FSK, Sets the modulation index used in FSK packet analysis
 * \param[in] BW_time_product, Sets the bandwidth time product(BT) for the gaussian filter used in FSK packet analysis
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_AnalyzeWiSUN(WISUN_MODULATION_TYPE standard =  FSK, FSK_MODULATION_TYPE modType_FSK = MOD_2FSK, int dataRate_FSK = 50000, double modIndex_FSK = 1.0, double BW_time_product = 0.5);

/*! Perform DECT Analysis Analysis on current capture
* \param[in] analysis_type, Specifies what type of analysis to perform. Default: 'All'. Valid values are as follows:
*         - PowerOnly
*         - All  (This is the set default value)
* \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int		LP_AnalyzeDECT( char *analysis_type = "ALL");

//! Perform HT40 mask (120MHz) analysis
/*!
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 * \remark Before calling LP_AnalyzeHT40Mask(), modeHT40 needs to set to 1 in LP_VsaDataCapture().
 */
IQMEASURE_API int		LP_AnalyzeHT40Mask();

//! Perform FFT Analysis on GPRF signals
/*
 * \param[in] pmode, Configures how multiple FFT frames are processed to obtain final spectrum result. Default AVER mode
 * \param[in] numberofPeaks, default 1
 * \param[in] res_bw Resolution bandwidth. default 100khz
 * \param[in] signal, Configures the signal type to enable adjustment of FFT resolution for spectrum measurement. default CW
 * \param[in] window_type FFT window type. dafault GAUSS
 * \param[in] startOffset, Offset in seconds into the capture data for start of calculation in second. default 0.
 * \param[in] length, Length of calculation in second. default 0.001
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_AnalyzeGPRFSpectrum(GPRF_PMODE pmode = PMODE_AVER, int numberofPeaks = 1, double res_bw = 100e3, GPRF_SIGNAL signal = SIGNAL_CW, GPRF_WINDOW window = WINDOW_GAUSS, double startOffset = 0.0, double length = 0.001);

/*****************************/
/* Result Handling Functions */
/*****************************/


// The following three functions are obsoleted due to the typo.
IQMEASURE_API double	LP_GetScalarMeasurment(char *measurement, int index=0);
IQMEASURE_API int		LP_GetVectorMeasurment(char *measurement, double bufferReal[], double bufferImag[], int bufferLength);
IQMEASURE_API int		LP_GetStringMeasurment(char *measurement, char bufferChar[], int bufferLength);

//! Get a scalar measurement result
/*!
 * \param[in] measurement The measurement name.  Please refer to \ref group_scalar_measurement "Scalar Measurements" for all available measurement names
 * \param[in] index The index of the measurement.  In most case, index would be zero.  For MIMO analysis, some measurements, such as EVM, may have more than one results
 *
 * \return The value of the measurement.  -99999.99 (a special defined negative value) will be returned if no measurement available
 */
IQMEASURE_API double	LP_GetScalarMeasurement(char *measurement, int index=0);

/*!
 * \param[in] measurement The measurement name.  Please refer to \ref group_scalar_measurement "Scalar Measurements" for all available measurement names
 * \param[in] index The index of the measurement.  In most case, index would be zero.  For MIMO analysis, some measurements, such as EVM, may have more than one results
 * \param[in] segment_index The segment index of the measurement. In most cases, segment index is SEGM1. For MPS multi-segment capture, it needs to be specified.
 *
 * \return The value of the measurement.  -99999.99 (a special defined negative value) will be returned if no measurement available
 */
IQMEASURE_API double	LP_GetScalarMeasurement_Segm(char *measurement, int index=0, int segment_index =1);

//! Get a vector measurement result
/*!
 * \param[in] measurement The measurement name.  Please refer to \ref group_vector_measurement "Vector Measurements" for all available measurement names
 * \param[out] bufferReal Returns the real part of vector
 * \param[out] bufferImag Returns the imag part of vector
 * \param[in] bufferLength Indicates the length of bufferReal and bufferImag (in Bytes)
 *
 * \return The number of elements in bufferReal or/and bufferImag
 */
IQMEASURE_API int		LP_GetVectorMeasurement(char *measurement, double bufferReal[], double bufferImag[], int bufferLength);

/*!
 * \param[in] measurement The measurement name.  Please refer to \ref group_vector_measurement "Vector Measurements" for all available measurement names
 * \param[out] bufferReal Returns the real part of vector
 * \param[out] bufferImag Returns the imag part of vector
 * \param[in] bufferLength Indicates the length of bufferReal and bufferImag (in Bytes)
 * \param[in] segment_index The segment index of the measurement. In most cases, segment index is SEGM1. For MPS multi-segment capture, it needs to be specified.
 *
 * \return The number of elements in bufferReal or/and bufferImag
 */
IQMEASURE_API int		LP_GetVectorMeasurement_Segm(char *measurement, double bufferReal[], double bufferImag[], int bufferLength, int segment_index =1);

//! Get a string measurement result
/*!
 * \param[in] measurement The measurement name.  Please refer to \ref group_string_measurement "String Measurements" for all available measurement names
 * \param[out] bufferChar Returns the string result
 * \param[in] bufferLength Indicates the length of bufferChar (in Bytes)
 *
 * \return The number of elements in bufferChar
 */
IQMEASURE_API int		LP_GetStringMeasurement(char *measurement, char bufferChar[], int bufferLength);

/***********************/
/* Debugging Functions */
/***********************/

//! Plot the data capture
/*!
 * \return ERR_OK
 */
IQMEASURE_API int		LP_PlotDataCapture();

//! Plot the specific data
/*!
 * \param[in] figNum Ploted figure number
 * \param[in] x The X-axis values of specific data
 * \param[in] y The Y-axis values of specific data
 * \param[in] length Indicates the length of specific data
 * \param[in] plotArgs Symbols used for plot
 * \param[in] title Figure title
 * \param[in] xtitle X-axis title
 * \param[in] ytitle Y-axis title
 * \param[in] keepPlot ???
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_Plot(int figNum, double *x, double *y, int length, char *plotArgs, char *title, char *xtitle, char *ytitle, int keepPlot=0);

//! Start IQmeasure internal timer
/*!
 * \return ERR_OK
 */
IQMEASURE_API int		LP_StartIQmeasureTimer();

//! Stop IQmeasure internal timer
/*!
 * \return ERR_OK
 */
IQMEASURE_API int		LP_StopIQmeasureTimer();

//! Start IQmeasure internal timer
/*!
 * \return ERR_OK
 */
IQMEASURE_API int		LP_ReportTimerDurations();


/*-------------------------*
*	IQ2010 Ext Functions   *
*--------------------------*/
//--

/*! @defgroup group_scalar_measurement Measurement Names for IQ2010EXT_GetDoubleMeasurements(), IQ2010EXT_GetStringMeasurement(), IQ2010EXT_GetVectorMeasurement(), IQ2010EXT_GetIntMeasurement()
 *
 * Available measurement names vary for various analysis.  After an analysis has been performed successfully, by calling one of the following functions:
 *   - IQ2010EXT_Analyze80211ag();
 *   - IQ2010EXT_Analyze80211nSISO();
 *   - IQ2010EXT_Analyze80211nComposite();
 *   - IQ2010EXT_Analyze80211b();
 *   - IQ2010EXT_AnalyzePower();
 *   - IQ2010EXT_AnalyzeFFT();
 *   - IQ2010EXT_AnalyzePowerRamp80211b();
 *   - IQ2010EXT_AnalyzeBluetooth();
 *   - IQ2010EXT_AnalyzeSpectralMeasurement();
 *   - IQ2010EXT_AnalyzeMaskMeasurement();
 *   - IQ2010EXT_AnalyzeSpectralMeasurement();
 *
 * \section analysis_80211ag Measurement Names for IQ2010EXT_Analyze80211ag() and IQ2010EXT_AnalyzeSpectralMeasurement() with spectral type 'IQ2010EXT_SPECTRAL_11AG'
 *  - <b>evmAll</b>:  EVM for entire frame.  index value: 0
 *  - <b>evmData</b>:  EVM for data part of the frame.  index value: 0
 *  - <b>evmPilot</b>:  EVM of all pilot part of the frame.  index value: 0
 *  - <b>codingRate</b>:  Coding Rate in the capture.  index value: 0
 *  - <b>freqErr</b>:  Frequency Error in the capture.  index value: 0
 *  - <b>clockErr</b>:  Symbol Clock Error in the capture.  index value: 0
 *  - <b>ampErr</b>:  IQ Match Amplitude Error in the capture.  index value: 0
 *  - <b>ampErrDb</b>:  IQ Match Amplitude Error in dB in the capture.  index value: 0
 *  - <b>phaseErr</b>:  IQ Match Phase Error in the capture.  index value: 0
 *  - <b>rmsPhaseNoise</b>:  Frequency RMS Phase Noise in rms in the capture.  index value: 0
 *  - <b>rmsPowerNoGap</b>:  RMS Power No Gap in the capture.  index value: 0
 *  - <b>rmsPower</b>:  RMS Power in the capture.  index  value: 0
 *  - <b>pkPower</b>:  Peak power in the capture.  index value: 0
 *  - <b>rmsMaxAvgPower</b>:  Average max RMS power in the capture.  index value: 0
 *  - <b>psduCrcFail</b>:  1 = PSDU CRC Check Failed, 0 = PSDU CRC Check Passed.  index value: 0
 *  - <b>plcpCrcPass</b>:  1 = PLCP CRC Check Passed, 0 = PLCP CRC Check Failed.  index value: 0
 *  - <b>dataRate</b>:  Data rate in Mbps in the capture.  index value: 0
 *  - <b>numSymbols</b>:  Number of Symbols in the capture.  index value: 0
 *  - <b>numPsduBytes</b>:  Number of bytes in PSDU.  index value: 0
 *  - <b>SUBCARRIER_LO_B_VSA1</b>:  Subcarrier number for lower region B ( tester VSA 1 ).  index value: 0
 *  - <b>VALUE_DB_LO_B_VSA1</b>:  Value at worst margin for lower region B (tester VSA 1).  index value: 0
 *  - <b>SUBCARRIER_LO_A_VSA1</b>:  Subcarrier number for lower region A ( tester VSA 1 ).  index value: 0
 *  - <b>VALUE_DB_LO_A_VSA1</b>:  Value at worst margin for lower region A (tester VSA 1).  index value: 0
 *  - <b>SUBCARRIER_UP_A_VSA1</b>:  Subcarrier number for upper region A ( tester VSA 1 ).  index value: 0
 *  - <b>VALUE_DB_UP_A_VSA1</b>:  Value at worst margin for upper region A (tester VSA 1).  index value: 0
 *  - <b>SUBCARRIER_UP_B_VSA1</b>:  Subcarrier number for upper region B ( tester VSA 1 ).  index value: 0
 *  - <b>VALUE_DB_UP_B_VSA1</b>:  Value at worst margin for upper region B (tester VSA 1).  index value: 0
 *  - <b>LO_LEAKAGE_DBR_VSA1</b>:  LO leakage (tester VSA 1).  index value: 0
 *
 * \section analysis_mimo Measurement Names for IQ2010EXT_Analyze80211nSISO, IQ2010EXT_Analyze80211nComposite() and IQ2010EXT_AnalyzeSpectralMeasurement() with 11N spectral type ( like, IQ2010EXT_SPECTRAL_11N_HT20 or IQ2010EXT_SPECTRAL_11N_HT40 )
 *  - <b>evmAvgAll</b>:  Average EVM of all symbols per stream in the capture. Each stream will have a corresponding value; therefore, the index value is [0 - (StreamNum-1)]
 *  - <b>packetDetection</b>:  Indicates at least one valid packet detected. Each stream will have a corresponding value; therefore, the index value is [0 - (StreamNum-1)]
 *  - <b>psduCRC</b>:  Indicates valid CRC on PSDU. Each stream will have a corresponding value; therefore, the index value is [0 - (StreamNum-1)]
 *  - <b>acquisition</b>:  Indicates at least one valid HT packet detected. Each stream will have a corresponding value; therefore, the index value is [0 - (StreamNum-1)]
 *  - <b>demodulation</b>:  Indicates demodulated streams (packet may be truncated). Each stream will have a corresponding value; therefore, the index value is [0 - (StreamNum-1)]
 *  - <b>dcLeakageDbc</b>:  DC Leakage in dBc per RX. Each stream will have a corresponding value; therefore, the index value is [0 - (StreamNum-1)]
 *  - <b>rxRmsPowerDb</b>:  RMS power of estimated channels per stream. Dimension is NStreams X NRx. Each stream will have a corresponding value; therefore, the index value is [0 - (StreamNum-1)]
 *  - <b>freqErrorHz</b>:  Frequency error in Hz. Each stream will have a corresponding value; therefore, the index value is [0 - (StreamNum-1)]
 *  - <b>symClockErrorPpm</b>:  Symbol clock error in ppm. Each stream will have a corresponding value; therefore, the index value is [0 - (StreamNum-1)]
 *  - <b>PhaseNoiseDeg_RmsAll</b>:  RMS phase noise, over all received signals. Each stream will have a corresponding value; therefore, the index value is [0 - (StreamNum-1)]
 *  - <b>IQImbal_amplDb</b>:  IQ gain imbalance in dB per stream. Each stream will have a corresponding value; therefore, the index value is [0 - (StreamNum-1)]
 *  - <b>IQImbal_phaseDeg</b>:  IQ phase imbalance in degree per stream. Each stream will have a corresponding value; therefore, the index value is [0 - (StreamNum-1)]
 *  - <b>rateInfo_bandwidthMhz</b>:  Bandwidth in MHz. Each stream will have a corresponding value; therefore, the index value is [0 - (StreamNum-1)]
 *  - <b>rateInfo_dataRateMbps</b>:  Data Rate in Mbps. Each stream will have a corresponding value; therefore, the index value is [0 - (StreamNum-1)]
 *  - <b>rateInfo_spatialStreams</b>:  Number in spatial streams. Each stream will have a corresponding value; therefore, the index value is [0 - (StreamNum-1)]
 *  - <b>analyzedRange</b>:  Start and end point of the signal part that was used for analysis. The API picks the first packet in the input signal for analysis; the following packets are ignored. Each stream will have a corresponding value; therefore, the index value is [0 - (StreamNum-1)]
 *  - <b>htSig1_htLength</b>:  Information from HT-SIG1: value in Length field as decimal number. Each stream will have a corresponding value; therefore, the index value is [0 - (StreamNum-1)]
 *  - <b>htSig1_mcsIndex</b>:  Information from HT-SIG1: MCS index as a decimal number. Each stream will have a corresponding value; therefore, the index value is [0 - (StreamNum-1)]
 *  - <b>htSig1_bandwidth</b>:  Information from HT-SIG1: bandwidth (0) 20MHz (1) 40MHz. Each stream will have a corresponding value; therefore, the index value is [0 - (StreamNum-1)]
 *  - <b>htSig2_advancedCoding</b>:  Information from HT-SIG2: advanced coding. Each stream will have a corresponding value; therefore, the index value is [0 - (StreamNum-1)]
 *  - <b>rateInfo_spaceTimeStreams</b>:  Number in Space-Time Streams. Each stream will have a corresponding value; therefore, the index value is [0 - (StreamNum-1)]
 *  - <b>SUBCARRIER_LO_B_VSA1</b>:  Subcarrier number for lower region B ( tester VSA 1 ).  index value: 0
 *  - <b>VALUE_DB_LO_B_VSA1</b>:  Value at worst margin for lower region B (tester VSA 1).  index value: 0
 *  - <b>SUBCARRIER_LO_A_VSA1</b>:  Subcarrier number for lower region A ( tester VSA 1 ).  index value: 0
 *  - <b>VALUE_DB_LO_A_VSA1</b>:  Value at worst margin for lower region A (tester VSA 1).  index value: 0
 *  - <b>SUBCARRIER_UP_A_VSA1</b>:  Subcarrier number for upper region A ( tester VSA 1 ).  index value: 0
 *  - <b>VALUE_DB_UP_A_VSA1</b>:  Value at worst margin for upper region A (tester VSA 1).  index value: 0
 *  - <b>SUBCARRIER_UP_B_VSA1</b>:  Subcarrier number for upper region B ( tester VSA 1 ).  index value: 0
 *  - <b>VALUE_DB_UP_B_VSA1</b>:  Value at worst margin for upper region B (tester VSA 1).  index value: 0
 *  - <b>LO_LEAKAGE_DBR_VSA1</b>:  LO leakage (tester VSA 1).  index value: 0
 *
 * \section analysis_80211b Measurement Names for IQ2010EXT_Analyze80211b()
 *  - <b>evmPk</b>:  EVM peak value of all symbols in the capture.  index value: 0
 *  - <b>evmAll</b>:  EVM for entire frame in the capture.  index value: 0
 *  - <b>evmInPreamble</b>:  EVM in the preamble of the capture.  index value: 0
 *  - <b>evmInPsdu</b>:  EVM in the PSDU of the capture.  index value: 0
 *  - <b>freqErr</b>:  Frequency Error in the capture.  index value: 0
 *  - <b>clockErr</b>:  Symbol Clock Error in the capture.  index value: 0
 *  - <b>ampErr</b>:  IQ Match Amplitude Error in the capture.  index value: 0
 *  - <b>ampErrDb</b>:  IQ Match Amplitude Error in the capture; specified in dB  index value: 0
 *  - <b>phaseErr</b>:  IQ Match Phase Error in the capture.  index value: 0
 *  - <b>rmsPhaseNoise</b>:  Frequency RMS Phase Noise in the capture.  index value: 0
 *  - <b>rmsPowerNoGap</b>:  RMS Power No Gap in the capture.  index value: 0
 *  - <b>rmsPower</b>:  RMS Power in the capture.  index value: 0
 *  - <b>pkPower</b>:  Peak power in the capture.  index value: 0
 *  - <b>rmsMaxAvgPower</b>:  Average max RMS power in the capture.  index value: 0
 *  - <b>bitRate</b>:  Bit Rate, see 802.11b standard.  index value: 0
 *  - <b>modType</b>:  Mod Type, see 802.11b standard.  index value: 0
 *  - <b>valid</b>:  Checks if the capture is valid.  index value: 0
 *  - <b>P_av_each_burst</b>:  Average power of each burst in the capture.  index value: 0
 *  - <b>P_pk_each_burst</b>:  Peak power of each burst in the capture.  index value: 0
 *  - <b>P_av_all</b>:  Average power in the capture.  index value: 0
 *  - <b>P_peak_all</b>:  Peak power in the capture.  index value: 0
 *  - <b>P_av_no_gap_all</b>:  Average power with no gap in the capture.  index value: 0
 *  - <b>P_av_all_dBm</b>:  Average power in the capture; specified in dBm.  index value: 0
 *  - <b>P_peak_all_dBm</b>:  Peak power in the capture; specified in dBm.  index value: 0
 *  - <b>P_av_no_gap_all_dBm</b>:  Average power with no gap in the capture; specified in dBm  index value: 0
 *  - <b>lockedClock</b>:  Locked Clock, see 802.11b standard.  index value: 0
 *  - <b>plcpCrcFail</b>:  1 = PLCP CRC Check Failed, 0 = PLCP CRC Check Passes.  index value: 0
 *  - <b>psduCrcFail</b>:  1 = PSDU CRC Check Failed, 0 = PSDU CRC Check Passes.  index value: 0
 *  - <b>longPreamble</b>:  1 = Long Preamble, 0 = Short Preamble.  index value: 0
 *  - <b>bitRateInMHz</b>:  Bit rate in Mhz in the capture.  index value: 0
 *  - <b>loLeakageDb</b>:  LO leakage in dBc.  index value: 0
 *  - <b>MaxfreqErr</b>:  Maximum frequency error.  index value: 0
 *
 * \section analysis_Power Measurement Names for IQ2010EXT_AnalyzePower()
 *  - <b>valid</b>:  Indicates valid results (1 valid, 0 invalid).  index value: 0
 *  - <b>P_av_each_burst</b>:  Average power of each burst in the capture.  index value: 0
 *  - <b>P_pk_each_burst</b>:  Peak power of each burst in the capture.  index value: 0
 *  - <b>P_av_all</b>:  Average power of whole capture.  index value: 0
 *  - <b>P_peak_all</b>:  Peak power of whole capture.  index value: 0
 *  - <b>P_av_no_gap_all</b>:  Average power with no gap in the capture.  index value: 0
 *  - <b>P_av_all_dBm</b>:  Average power of whole capture; specified in dBm.  index value: 0
 *  - <b>P_peak_all_dBm</b>:  Peak power of whole capture; specified in dBm.  index value: 0
 *  - <b>P_av_no_gap_all_dBm</b>:  Average power, not counting gaps in dBm.  index value: 0
 *
 * \section analysis_FFT Measurement Names for IQ2010EXT_AnalyzeFFT()
 *  - <b>valid</b>:  Returns (value) whether results are valid or not. 1 valid, 0 invalid.  index value: 0
 *  - <b>length</b>:  Returns length of X and Y Vectors above.  index value: 0
 *
 * \section analysis_Bluetooth Measurement Names for IQ2010EXT_AnalyzeBluetooth()
 *  - <b>dataRateDetect</b>:  Detects which data rate is in capture.  index value: 0
 *  - <b>valid</b>:  Flag indicating whether or not the power portion of the Bluetooth analysis was successful (1), or not (0).  index value: 0
 *  - <b>bandwidth20dB</b>:   20 dB bandwidth value Hz.  index value: 0
 *  - <b>deltaF1Average</b>:  The measurement result for deltaF1Avg as specified in BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2. Requires 00001111 data pattern. Results in Hz.  index value: 0
 *  - <b>deltaF2Max</b>:  The measurement result for deltaF2Max as specified in BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2. Requires alternating data pattern. Results in Hz.  index value: 0
 *  - <b>deltaF2Average</b>:  The measurement result for deltaF2Avg as specified in BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2. Requires alternating data pattern. Results in Hz.  index value: 0
 *  - <b>deltaF2MaxAccess</b>:  Similar to the measurement result for deltaF2Max as specified in BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2. Results measured from Access data. Results in Hz.  index value: 0
 *  - <b>deltaF2AvAccess</b>:  Similar to the measurement result for deltaF2Avg as specified in BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2. Results measured from Access data. Results in Hz.  index value: 0
 *  - <b>EdrEVMAv</b>:  RMS Differential EVM value (EDR only).  index value: 0
 *  - <b>EdrEVMpk</b>:  Peak Differential EVM value (EDR only).  index value: 0
 *  - <b>EdrEVMvalid</b>:  Indicates validity of EDR EVM Measurements.  index value: 0
 *  - <b>EdrPowDiffdB</b>:  Relative power of EDR section to FM section of packet, in dB.  index value: 0
 *  - <b>freq_deviation</b>:  Similar to the measurement result for deltaF1Avg as specified in BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2.
 *					  Results measured from Header data. Results in Hz.  index value: 0
 *  - <b>freq_deviationpktopk</b>:  Peak to Peak Frequency Deviation, in Hz during header.  index value: 0
 *  - <b>freq_estHeader</b>:  Estimates the Frequency Offset during the Header in Hz.  index value: 0
 *  - <b>EdrFreqExtremeEdronly</b>:  Reports the extreme value of the frequency variation during DPSK in Hz. Does not include the offset during the header.  index value: 0
 *  - <b>EdrprobEVM99pass</b>:  The percentage of symbols with EVM below the threshold. Threshold for 2 Mbps is 0.3 for 3 Mbps is 0.2.  index value: 0
 *  - <b>EdrEVMvsTime</b>:  Max DEVM Average as specified in BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2.  index value: 0
 *  - <b>validInput</b>:  Indicates whether or not the input wave was valid.  index value: 0
 *  - <b>maxfreqDriftRate</b>:  1 Mbps only, takes the maximum drift over specified time interval.  index value: 0
 *  - <b>payloadErrors</b>:  Reports the number of data errors in Payload. Not counting CRC. If -1, this value has not been calculated.
 *					 If larger negative number, it reports the length of the payload data vector. This happens when the length of the payload vector is shorter than the length indicated in the payload header.  index value: 0
 *
 * \section analysis_mask Measurement Names for IQ2010EXT_AnalyzeMaskMeasurement() with mask type WIFI_11B
 *  - <b>FREQ_MHZ_LO_B_VSA1</b>:    frequency (M Hz) in region LO_B at worst margin
 *  - <b>MARGIN_DB_LO_B_VSA1</b>:   power (Db) in region LO_B at worst margin
 *  - <b>FREQ_MHZ_LO_A_VSA1</b>:    frequency (M Hz) in region LO_A at worst margin
 *  - <b>MARGIN_DB_LO_A_VSA1</b>:   power (Db) in region LO_A at worst margin
 *  - <b>FREQ_MHZ_UP_A_VSA1</b>:    frequency (M Hz) in region UP_A at worst margin
 *  - <b>MARGIN_DB_UP_A_VSA1</b>:   power (Db) in region UP_A at worst margin
 *  - <b>FREQ_MHZ_UP_B_VSA1</b>:    frequency (M Hz) in region UP_B at worst margin
 *  - <b>MARGIN_DB_UP_B_VSA1</b>:   power (Db) in region UP_B at worst margin
 *  - <b>OBW_MHZ_VSA1</b>:          Occupied Bandwidth measurement in MHz
 *
 * \section analysis_mask Measurement Names for IQ2010EXT_AnalyzeMaskMeasurement() with mask type WIFI_11AG, WIFI_11N_HT20 and WIFI_11N_HT40
 *  - <b>FREQ_MHZ_LO_D_VSA1</b>:    frequency (M Hz) in region LO_D at worst margin
 *  - <b>MARGIN_DB_LO_D_VSA1</b>:   power (Db) in region LO_D at worst margin
 *  - <b>FREQ_MHZ_LO_C_VSA1</b>:    frequency (M Hz) in region LO_C at worst margin
 *  - <b>MARGIN_DB_LO_C_VSA1</b>:   power (Db) in region LO_C at worst margin
 *  - <b>FREQ_MHZ_LO_B_VSA1</b>:    frequency (M Hz) in region LO_B at worst margin
 *  - <b>MARGIN_DB_LO_B_VSA1</b>:   power (Db) in region LO_B at worst margin
 *  - <b>FREQ_MHZ_LO_A_VSA1</b>:    frequency (M Hz) in region LO_A at worst margin
 *  - <b>MARGIN_DB_LO_A_VSA1</b>:   power (Db) in region LO_A at worst margin
 *  - <b>FREQ_MHZ_UP_A_VSA1</b>:    frequency (M Hz) in region UP_A at worst margin
 *  - <b>MARGIN_DB_UP_A_VSA1</b>:   power (Db) in region UP_A at worst margin
 *  - <b>FREQ_MHZ_UP_B_VSA1</b>:    frequency (M Hz) in region UP_B at worst margin
 *  - <b>MARGIN_DB_UP_B_VSA1</b>:   power (Db) in region UP_B at worst margin
 *  - <b>FREQ_MHZ_UP_C_VSA1</b>:    frequency (M Hz) in region UP_C at worst margin
 *  - <b>MARGIN_DB_UP_C_VSA1</b>:   power (Db) in region UP_C at worst margin
 *  - <b>FREQ_MHZ_UP_D_VSA1</b>:    frequency (M Hz) in region UP_D at worst margin
 *  - <b>MARGIN_DB_UP_D_VSA1</b>:   power (Db) in region UP_D at worst margin
 *  - <b>OBW_MHZ_VSA1</b>:          Occupied Bandwidth measurement in MHz
 *
 * \section analysis_spectral Measurement Names for IQ2010EXT_AnalyzeSpectralMeasurement() with spectral type IQ2010EXT_SPECTRAL_11AG, IQ2010EXT_SPECTRAL_11N_HT20, IQ2010EXT_SPECTRAL_11N_HT40 and IQ2010EXT_SPECTRAL_11N
 *  - <b>SUBCARRIER_LO_B_VSA1</b>:  Subcarrier number in region LO_B at worst margin
 *  - <b>VALUE_DB_LO_B_VSA1</b>:    power (Db) in region LO_B at worst margin
 *  - <b>SUBCARRIER_LO_A_VSA1</b>:  Subcarrier number in region LO_A at worst margin
 *  - <b>VALUE_DB_LO_A_VSA1</b>:    power (Db) in region LO_A at worst margin
 *  - <b>SUBCARRIER_UP_B_VSA1</b>:  Subcarrier number in region UP_B at worst margin
 *  - <b>VALUE_DB_UP_B_VSA1</b>:    power (Db) in region UP_B at worst margin
 *  - <b>SUBCARRIER_UP_A_VSA1</b>:  Subcarrier number in region UP_A at worst margin
 *  - <b>VALUE_DB_UP_A_VSA1</b>:    power (Db) in region UP_A at worst margin
 *  - <b>LO_LEAKAGE_DBR_VSA1</b>:   LO leakage
 *
 * \section Rx per Measurement names for IQ2010EXT_RxPer()
 *  - <b>powerLeveldBm</b>:  powerLeveldBm the actual power levels for each enabled port at each power level
 *  - <b>ackRespMaxUs</b>:  ackRespMaxUs maximmum ack response time in us
 *  - <b>ackRespMinUs</b>:  ackRespMinUs minimum ack response time in us
 *  - <b>perPercent</b>:  perPercent the PER result at each power level (%)
 *
 */

/*! @defgroup group_vector_measurement Measurement Names for IQ2010EXT_GetVectorMeasurement()
 *
 * Available measurement names vary for various analysis.  After an analysis has been performed successfully, by calling
 * one of the following functions:
 *   - IQ2010EXT_Analyze80211ag();
 *   - IQ2010EXT_Analyze80211nSISO();
 *   - IQ2010EXT_Analyze80211nComposite();
 *   - IQ2010EXT_Analyze80211b();
 *   - IQ2010EXT_AnalyzeFFT();
 *   - IQ2010EXT_AnalyzePowerRamp80211b();
 *   - IQ2010EXT_AnalyzeBluetooth();
 *   - IQ2010EXT_AnalyzeMaskMeasurement();
 *
 * \section analysis_80211ag Measurement Names for IQ2010EXT_Analyze80211ag()
 *  - <b>hhEst</b>:  64-element complex vector that represents the FFT output of the two long symbols in the PLCP preamble of the OFDM signal.  index value: 0
 *  - <b>psdu</b>:  The PSDU data. 0/1 values. Includes the MAC Header and the FCS, if present .  index value: 0
 *  - <b>startPointers</b>:  Provides the approximate starting locations of each packet detected in the input data.
 *					 If no packet detected, this is an empty vector.  index value: 0
 *  - <b>plcp</b>:  PLCP (binary) data as 1/0s.  index value: 0
 *
 * \section analysis_mimo Measurement Names for IQ2010EXT_AnalyzeMimo()
 *  - <b>channelEst</b>:  Channel estimate matrix; dimension NStreams x NTones x NRx. Each stream will have a corresponding value; therefore, the index value is [0 - (StreamNum-1)]
 *  - <b>rxRmsPowerDb</b>:  RMS power of estimated channels; dimension NStreams x NRx. Each stream will have a corresponding value; therefore, the index value is [0 - (StreamNum-1)]
 *  - <b>isolationDb</b>:  Matrix that indicates isolation between streams; column 1 is for Rx signal containing main power for stream 1, etc; dimension NStreams x NStreams.
 *				   Each stream will have a corresponding value; therefore, the index value is [0 - (StreamNum-1)]
 *				   Note: Isolation can only be measured with direct-mapping signals.
 *  - <b>evmSymbols</b>:  Per symbol and stream; dimension NStreams x NSymbols. Each stream will have a corresponding value; therefore, the index value is [0 - (StreamNum-1)]
 *  - <b>evmTones</b>:  Per tone and stream; dimension NStreams x Ntones. Each stream will have a corresponding value; therefore, the index value is [0 - (StreamNum-1)]
 *  - <b>PhaseNoiseDeg_Symbols</b>:  Per symbol and VSA signal; dimension NRx x Nsymbols. Each stream will have a corresponding value; therefore, the index value is [0 - (StreamNum-1)]
 *  - <b>demodSymbols</b>: Complex demodulated symbols; dimension NTones x NSymbols x Nstreams.  This provides the symbol constellation I (Re) and Q (Im) arrays.
 *					Each stream will have a corresponding value; therefore, the index value is [0 - (StreamNum-1)]
 *
 * \section analysis_80211b Measurement Names for IQ2010EXT_Analyze80211b()
 *  - <b>evmInPlcp</b>: Evm in PLCP.  index value: 0
 *  - <b>evmErr</b>: Evm error.  index value: 0
 *  - <b>loLeakageDb</b>: LO/DC Leakage [dBc].  index value: 0
 *
 * \section analysis_FFT Measurement Names for IQ2010EXT_AnalyzeFFT()
 *  - <b>x</b>: X-axis values, typically frequency. Vector is returned.  index value: 0
 *  - <b>y</b>: Y-axis values, power in dBm. Vector is returned.  index value: 0
 *
 * \section analysis_PowerRamp80211b Measurement Names for IQ2010EXT_AnalyzePowerRamp80211b()
 *  - <b>on_mask_x</b>: Power on ramp mask x-axis.  index value: 0
 *  - <b>off_mask_x</b>: Power off ramp mask x-axis.  index value: 0
 *  - <b>on_mask_y</b>: Power on ramp mask y-axis.  index value: 0
 *  - <b>off_mask_y</b>: Power off ramp mask y-axis.  index value: 0
 *  - <b>on_power_inst</b>: Instant power on ramp.  index value: 0
 *  - <b>off_power_inst</b>: Instant power off ramp.  index value: 0
 *  - <b>on_power_peak</b>: Power on ramp peak values.  index value: 0
 *  - <b>off_power_peak</b>: Power off ramp peak values.  index value: 0
 *  - <b>on_time_vect</b>: Power on ramp time vector.  index value: 0
 *  - <b>off_time_vect</b>: Power off ramp time vector.  index value: 0
 *  - <b>on_time</b>: Ramp on time.  index value: 0
 *  - <b>off_time</b>: Ramp off time.  index value: 0
 *
 * \section analysis_Bluetooth Measurement Names for IQ2010EXT_AnalyzeBluetooth()
 *  - <b>P_av_each_burst</b>: Average power of each burst.  index value: 0
 *  - <b>P_pk_each_burst</b>: Peak power of each burst.  index value: 0
 *  - <b>complete_burst</b>: Vector indicating whether or not each burst is complete.
 *					  All elements are either (1 beginning and end transitions detected), or (0 burst is missing either beginning or end transition).
 *					  index value: 0
 *  - <b>start_sec</b>: Starting time of each burst, in seconds.  index value: 0
 *  - <b>stop_sec</b>: End time of each burst, in seconds.  index value: 0
 *  - <b>freq_est</b>: Initial freq offset of each burst detected, in Hz.  index value: 0
 *  - <b>freqEstPacketPointer</b>: Pointers to which packet is pointed to in each element of freq_est.  index value: 0
 *  - <b>freq_drift</b>: Initial freq carrier drift of each burst detected, in Hz.  index value: 0
 *  - <b>freqDriftPacketPointer</b>: Pointers to which packet is pointed to in each element of freq_drift.  index value: 0
 *  - <b>freqDeviationPointer</b>: Pointers to which packet is pointed to in each element of the above two measurements.  index value: 0
 *  - <b>EdrFreqvsTime</b>: Enhanced data rate for frequency versus time.  index value: 0
 *  - <b>EdrExtremeOmegaI0</b>: Extreme value of (Omega_0 + Omega_i).  index value: 0
 *
 * \section analysis_mask Measurement Names for IQ2010EXT_AnalyzeMaskMeasurement() with mask type WIFI_11B, WIFI_11AG, WIFI_11N_HT20 and WIFI_11N_HT40
 *  - <b>x</b>: X-axis values, typically frequency. Vector is returned.  index value: 0
 *  - <b>y</b>: Y-axis values, power in dBm. Vector is returned.  index value: 0
 *
 */


/*--------------*
 * TX Functions *
 *--------------*/

//! Perform multiple captures, then user can later perform different analysis based on these captures.
//! Use it along with IQ2010EXT_AddMultiAnalysisCapture() and IQ2010EXT_FinishMultiAnalysisCapture()
/*!
 * \param[in] dataRate Datarate the DUT uses to transmit the signal
 * \param[in] freqMHz Frequency (MHz) at which the DUT transmits
 * \param[in] rmsPowerLeveldBm Estimated DUT RMS power level (dBm) at the tester RF ports
 * \param[in] skipPktCnt Skip count before capture.                                         Max allowed: 255
 * \param[in] rfPort RF port. Valid values are PORT_LEFT (RF1) and PORT_RIGHT (RF2).
 * \param[in] triggerLeveldBm Trigger level in dBm
*/
IQMEASURE_API int		LP_IQ2010EXT_NewMultiAnalysisCapture( char		   *dataRate,
                                                        double          freqMHz,
                                                        double	        rmsPowerLeveldBm,
													    int	            skipPktCnt,
													    int		        rfPort,
													    double	        triggerLeveldBm);

//! Add multiple capture type with specified capture length and count. This function should be called after IQ2010EXT_NewMultiAnalysisCapture()
/*!
 * \param[in] analysisType  analysis type
 * \param[in] captureLengthUs Capture length in us.                                         Max allowed: 1023 us
 * \param[in] captureCnt Capture count.                                                     Max allowed: 255
*/
IQMEASURE_API int		LP_IQ2010EXT_AddMultiAnalysisCapture( int		    analysisType,
													    int			captureLengthUs,
													    int			captureCnt);

//! VSA Multi-packet analysis capture. Should use it along with IQ2010EXT_FinishMultiAnalysisCapture(). The function should be called first.
/*!
 * \return ERR_OK if the VSA capture is done successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
*/
IQMEASURE_API int		LP_IQ2010EXT_InitiateMultiAnalysisCapture(void);

//! VSA multi-packet capture based on the  . The function should be called after IQ2010EXT_AddMultiAnalysisCapture()
/*!
 * \return ERR_OK if the VSA capture is done successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
*/
IQMEASURE_API int		LP_IQ2010EXT_FinishMultiAnalysisCapture(void);

//! VSA Multi-packet capture. Should use it along with IQ2010EXT_FinishMultiCapture(). The function should be called first.
/*!
 * \param[in] dataRate Datarate the DUT uses to transmit
 * \param[in] freqMHz Frequency (MHz) at which the DUT transmits
 * \param[in] rmsPowerLeveldBm Estimated DUT RMS power level (dBm) at the tester RF ports
 * \param[in] captureLengthUs Capture length in us.                                         Max allowed: 1023 us
 * \param[in] skipPktCnt Skip count before capture.                                         Max allowed: 255
 * \param[in] captureCnt Capture count.                                                     Max allowed: 255
 * \param[in] rfPort RF port. Valid values are PORT_LEFT (RF1) and PORT_RIGHT (RF2).
 * \param[in] triggerLeveldBm Trigger level in dBm
 *
 * \return ERR_OK if the VSA capture is initiated successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
*/
IQMEASURE_API int		LP_IQ2010EXT_InitiateMultiCapture( char				*dataRate,
													 double              freqMHz,
													 double	             rmsPowerLeveldBm,
													 int	             captureLengthUs,
													 int	             skipPktCnt,
													 int	             captureCnt,
													 int				 rfPort,
													 double	             triggerLeveldBm);

//! VSA Multi-packet Capture for HT40. Should use it along with IQ2010EXT_FinishMultiCapture(). The function should be called first.
/*!
 * \param[in] dataRate Datarate the DUT uses to transmit
 * \param[in] freqMHz Frequency (MHz) at which the DUT transmits
 * \param[in] rmsPowerLeveldBm Estimated DUT RMS power level (dBm) at the tester RF ports
 * \param[in] captureLengthUs Capture length in us.                                         Max allowed: 1023 us
 * \param[in] skipPktCnt Skip count before capture.                                         Max allowed: 255
 * \param[in] captureCnt Capture count.                                                     Max allowed: 255
 * \param[in] rfPort RF port. Valid values are PORT_LEFT (RF1) and PORT_RIGHT(RF2).
 * \param[in] triggerLeveldBm Trigger-level in dBm
 *
 * \return ERR_OK if the VSA capture is initiated successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
*/
IQMEASURE_API int		LP_IQ2010EXT_InitiateMultiCaptureHT40( char			   *dataRate,
														 double             freqMHz,
														 double	            rmsPowerLeveldBm,
														 int	            captureLengthUs,
														 int	            skipPktCnt,
														 int	            captureCnt,
														 int                rfPort,
														 double	            triggerLeveldBm);


//! VSA multi-packet capture. Should use it along with IQ2010EXT_InitiateMultiCapture(). The function should be called after IQ2010EXT_InitiateMultiCapture() or IQ2010EXT_InitiateMultiCaptureHT40().
/*!
 * \return ERR_OK if the VSA capture is done successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
*/
IQMEASURE_API int		LP_IQ2010EXT_FinishMultiCapture(void);

//! Perform 802.11a/g Analysis on Multi-packet Capture
/*!
 * \param[in] ph_corr_mode Phase Correction Mode with the following valid options:
 *         - IQV_PH_CORR_OFF: Phase correction off
 *         - IQV_PH_CORR_SYM_BY_SYM: Symbol-by-symbol correction  (Default)
 *         - IQV_PH_CORR_MOVING_AVG: Moving avg. correction (10 symbols)
 * \param[in] ch_estimate Channel Estimate with the following options:
 *         - IQV_CH_EST_RAW_LONG: Raw Channel Estimate (based on long trainling symbols) (Default)
 *         - IQV_CH_EST_2ND_ORDER: 2nd Order Polyfit
 *         - IQV_CH_EST_RAW_FULL: Full packet estimate
 * \param[in] sym_tim_corr Symbol Timing Correction with the following options:
 *         - IQV_SYM_TIM_OFF: Symbol Timing Correction Off
 *         - IQV_SYM_TIM_ON: Symbol Timing Correction ON (Default)
 * \param[in] freq_sync Frequency Sync. Mode with the following options:
 *         - IQV_FREQ_SYNC_SHORT_TRAIN: Short Training Symbol
 *         - IQV_FREQ_SYNC_LONG_TRAIN: Long Training Symbol (Default)
 *         - IQV_FREQ_SYNC_FULL_PACKET: Full Data Packet
 * \param[in] ampl_track Amplitude Tracking with the following options:
 *         - IQV_AMPL_TRACK_OFF: Amplitude tracking off (Default)
 *         - IQV_AMPL_TRACK_ON: Amplitude tracking on
 *
 * \return ERR_OK if the 802.11 a/g analysis succeeded; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
 */
IQMEASURE_API int		LP_IQ2010EXT_Analyze80211ag( IQV_PH_CORR_ENUM ph_corr_mode    =IQV_PH_CORR_SYM_BY_SYM,
											   IQV_CH_EST_ENUM ch_estimate      =IQV_CH_EST_RAW_LONG,
											   IQV_SYM_TIM_ENUM sym_tim_corr    =IQV_SYM_TIM_ON,
											   IQV_FREQ_SYNC_ENUM freq_sync     =IQV_FREQ_SYNC_LONG_TRAIN,
											   IQV_AMPL_TRACK_ENUM ampl_track   =IQV_AMPL_TRACK_OFF);

//! Perform 802.11n Analysis on Multi-packet Capture
/*!
 * \param[in] enablePhaseCorr Phase Correction Mode:
 *         - 0: Disable
 *         - 1: Enable (Default)
 * \param[in] enableSymTimingCorr Symbol Timing Correction:
 *         - 0: Disable
 *         - 1: Enable (Default)
 * \param[in] enableAmplitudeTracking Amplitude Tracking:
 *         - 0: Disable (Default)
 *         - 1: Enable
 * \param[in] decodePSDU Decode PSDU:
 *         - 0: Disable (Default)
 *         - 1: Enable
 * \param[in] enableFullPacketChannelEst Channel Estimate on full packet:
 *         - 0: Disable (Default)
 *         - 1: Enable
 * \param[in] packetFormat indicates format of the 80211n MIMO packet:
 *         - 0: auto-detect mode (Default)
 *         - 1: mixed format
 *         - 2: greenfield format
 *
 * \return ERR_OK if the 802.11n analysis is successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
 */
IQMEASURE_API int		LP_IQ2010EXT_Analyze80211nSISO(  int enablePhaseCorr            = 1,
												  int enableSymTimingCorr        = 1,
												  int enableAmplitudeTracking    = 0,
												  int decodePSDU                 = 0,
												  int enableFullPacketChannelEst = 0,
												  int packetFormat               = 0);

//! Perform 802.11n Analysis on Multi-packet Capture in composite mode
/*!
 * \param[in] enablePhaseCorr Phase Correction Mode:
 *         - 0: Disable
 *         - 1: Enable (Default)
 * \param[in] enableSymTimingCorr Symbol Timing Correction:
 *         - 0: Disable
 *         - 1: Enable (Default)
 * \param[in] enableAmplitudeTracking Amplitude Tracking:
 *         - 0: Disable (Default)
 *         - 1: Enable
 * \param[in] enableFullPacketChannelEst Channel Estimate on full packet:
 *         - 0: Disable (Default)
 *         - 1: Enable
 * \param[in] referenceFile PSDU reference file (needed for Composite EVM)
 * \param[in] packetFormat indicates format of the 80211n MIMO packet:
 *         - 0: auto-detect mode (Default)
 *         - 1: mixed format
 *         - 2: greenfield format
 *
 * \return ERR_OK if the 802.11n analysis is successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
 */
IQMEASURE_API int		LP_IQ2010EXT_Analyze80211nComposite(int enablePhaseCorr            = 1,
													  int enableSymTimingCorr        = 1,
													  int enableAmplitudeTracking    = 0,
													  int enableFullPacketChannelEst = 0,
													  char *referenceFile            = NULL,
													  int packetFormat               = 0);

//! Perform 802.11b Analysis on Multi-packet Capture
/*!
 * \param[in] eq_taps Number of equalizer taps, valid options are as follows:
 *         - IQV_EQ_OFF: Equalizer Off  (Default)
 *         - IQV_EQ_5_TAPS: 5 Taps Equalizer
 *         - IQV_EQ_7_TAPS: 7 Taps Equalizer
 *         - IQV_EQ_9_TAPS: 9 Taps Equalizer
 * \param[in] DCremove11b_flag DC removal, valid options are:
 *         - IQV_DC_REMOVAL_OFF: DC Removal OFF (Default)
 *         - IQV_DC_REMOVAL_ON: DC Removal ON
 * \param[in] method_11b 802.11b method, valid options are as follows:
 *         - IQV_11B_STANDARD_TX_ACC: Use 11b standard TX accuracy (Default)
 *         - IQV_11B_RMS_ERROR_VECTOR: Use 11b RMS error vector
 *
 * \return ERR_OK if the 802.11b analysis is successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
 */
IQMEASURE_API int		LP_IQ2010EXT_Analyze80211b( IQV_EQ_ENUM eq_taps                   = IQV_EQ_OFF,
											  IQV_DC_REMOVAL_ENUM DCremove11b_flag  = IQV_DC_REMOVAL_OFF,
											  IQV_11B_METHOD_ENUM method_11b        = IQV_11B_STANDARD_TX_ACC);

//! Perform power analysis on Multi-packet Capture
/*!
 * \return ERR_OK if the power analysis is successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
*/
IQMEASURE_API int		LP_IQ2010EXT_AnalyzePower(void);

//! Perform FFT analysis on Multi-packet Capture
/*!
 * \param [in] NFFT        Number of points in FFT.                        Default: 0
 * \param [in] res_bwHz    The resolution bandwidth in Hz.                 Default: 100000
 * \param [in] window_type Window type with following options              Default: "blackmanharris"
 *                 - "blackmanharris"
 *                 - "hanning"
 *                 - "flattop"
 *                 - "rect"
*/
IQMEASURE_API int		LP_IQ2010EXT_AnalyzeFFT( double NFFT          = 0,
										   double res_bwHz      = 100000,
										   char *window_type    = "blackmanharris");

//! Perform mask verification and get OBW based on the OBW percentage
/*!
 * \param [in] maskType         indicate what mask type should be applied to the captured data
 * \param [in] obwPercentage    OBW percentage. The valid range should be between 0 - 1
 * \return ERR_OK if the mask measurement is successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
*/
IQMEASURE_API int		LP_IQ2010EXT_AnalyzeMaskMeasurement(int maskType, double obwPercentage);

//! Perform power-ramp analysis for 11B
/*!
 * \return ERR_OK if the mask measurement is successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
*/
IQMEASURE_API int		LP_IQ2010EXT_AnalyzePowerRamp80211b();

//! Perform Bluetooth Analysis on Multi-packet Capture
/*!
 * \param[in] data_rate Bluetooth data rate, with the following options:
 *         - 1: 1Mbit/s BDR
 *         - 2: 2Mbit/s EDR
 *         - 3: 3Mbit/s EDR
 *         - 0: Auto detect
 * \param[in] analysis_type Specifies what type of analysis to perform.                 Default: 'All'.
 *         Valid values are as follows:
 *         - PowerOnly
 *         - 20dbBandwidthOnly
 *         - PowerandFreq
 *         - All  (This is the set default value)
 *		   - ACP  (only does the new ACP analysis)
 *		   - AllPlus  (performs all the analyses that are done by ¡®All?plus the ACP analysis)
 * \return ERR_OK if the bluetooth analysis is successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
 */
IQMEASURE_API int		LP_IQ2010EXT_AnalyzeBluetooth(double data_rate, char *analysis_type="All");

//! Perform spectral analysis based on the selected spectral type
/*!
 * param [in] spectralType  indicate the spectral type to be applied to the captured data
 * \return ERR_OK if the spectral measurement is successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
*/
IQMEASURE_API int		LP_IQ2010EXT_AnalyzeSpectralMeasurement(int spectralType);

//////! Set Analysis Parameters
/////*!
//// * \param[in] analysisType One of the analysis types defined in IQ2010EXT_ANALYSIS_TYPE_ENUM
//// * \param[in] parameter One of the parameter names for the specified analysis type (see TODO for details)
//// * \param[in] value The value for the specified parameter
//// *
//// * \return ERR_OK if successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
//// */
//////IQ2010EXT_API int IQ2010EXT_SetAnalysisParameterInteger(Q2010EXT_ANALYSIS_TYPE_ENUM analysisType,
//////                                                        char *parameter,
//////                                                        int value);



/*--------------*
 * RX Functions *
 *--------------*/

//! IQ2010 ACK-based RX PER Test
/*!
 * \param[in] waveformIndex A wave form index (0 based) of the multi-segment wavefile, specifying which wave form should be used
 * \param[in] freqMHz Frequency (MHz) on which DUT operates
 * \param[in] powerLevelStartdBm The start of power level (dBm at IQ2010 RF ports)
 * \param[in] powerLevelStopdBm The stop of power level (dBm at IQ2010 RF ports)
 * \param[in] stepdB Power level step(dB).  Minimum step is 0.5dB.
 * \param[in] packetCount Number of packets to be transmitted for power level. Maximum number of packets is 32767.
 * \param[in] rfPort The RF ports used for RX PER testing. Valid values are PORT_LEFT (RF1) and PORT_RIGHT(RF2).
 * \param[in] ackPowerRmsdBm The DUT ACK RMS Power at the tester port
 * \param[in] ackTriggerLeveldBm The trigger level set to do ACK detection
 *
 * \return ERR_OK if the RX PER is successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
 *
 * \remark
 *         -# In most cases, the stop power level would be lower than the start power level, but it could be
 *            higher than the start power level.  In either way, step would be relative, meaning always positive.
 *         -# If only one power level is interested for RX PER, make the powerLevelStopdBm the same as powerLevelStartdBm.
 *         -# power level should be within [-10, -95] dBm
 */
IQMEASURE_API int		LP_IQ2010EXT_RxPer( int               waveformIndex,
									  double            freqMHz,
									  double            powerLevelStartdBm,
									  double            powerLevelStopdBm,
									  double            stepdB,
									  int               packetCount,
									  int				rfPort,
									  double            ackPowerRmsdBm,
									  double            ackTriggerLeveldBm
									  );

/*---------------------------------*
 * Multi-segment Waveform Handling *
 *---------------------------------*/
//! Start a new Multi-segment Waveform Creation
/*!
 *
 * \return ERR_OK if successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
 *
 * \remark
 *         -# After calling this function, you can then call IQ2010EXT_AddWaveform()
 */
IQMEASURE_API int		LP_IQ2010EXT_NewMultiSegmentWaveform();

//! Add a Waveform to the Multi-segment Waveform.
/*!
 * \param[in] modFile A .mod file that contains only one data rate
 * \param[out] waveformIndex The waveform index used by IQ2010EXT_RxPer() identifying this data rate
 *
 * \return ERR_OK if successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
 *
 * \remark
 *         -# You can only call this function after IQ2010EXT_NewMultiSegmentWaveform() and before IQ2010EXT_FinalizeMultiSegmentWaveform()
 *         - The maximum number of file can be added is 32
 */
IQMEASURE_API int		LP_IQ2010EXT_AddWaveform(const char *modFile, unsigned int *waveformIndex );

//! Finalize the Multi-segment Waveform Creation and Upload to VSG Memory
/*!
 * \return ERR_OK if successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
 * \remark
 *         -# You cannot call IQ2010EXT_AddWaveform() anytime after calling this function
 */
IQMEASURE_API int		LP_IQ2010EXT_FinalizeMultiSegmentWaveform();

//! Generate a wave form at VSG by the specified segment index with the packet count
/*!
 * \param[in] waveformIndex index (base 0) to wave form (wave form is preloaded by IQ2010EXT_LoadMultiSegmentWaveFile(...) ) to be played at VSG
 * \param[in] packetCount number of packets to be played
 * \return ERR_OK if successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
*/
//IQMEASURE_API int		LP_IQ2010EXT_VsgSingleSegmentPlay(int waveformIndex, int packetCount);

//! After calling IQ2010EXT_VsgSingleSegmentPlay(...). Should call IQ2010EXT_VsgSingleSegmentStop() to stop the segment played
/*!
 * \return ERR_OK if successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
*/
//IQMEASURE_API int		LP_IQ2010EXT_VsgSingleSegmentStop();

/*--------------------------*
 * Getting result Functions *
 *--------------------------*/

//! Get the number of results for a particular measurement element (pointed to by indexResult) by measurement name
/*!
 * \param [in] measurementName         measurement name
 * \param [in] indexResult             index to a particular measurement element
 * \param [out] numberOfMeasurementElement return number of measurement element
 * \return ERR_OK if successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
*/
IQMEASURE_API int		LP_IQ2010EXT_GetNumberOfMeasurementElements(char *measurementName, int indexResult, int *numberOfMeasurementElement);

//! Get the number of results for a particular measurement element (pointed to by indexResult) by measurement name
/*!
 * \param [in] measurementName         measurement name
 * \param [out] numberOfPerPowerLevel return number of per power level
 * \return ERR_OK if successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
*/
IQMEASURE_API int		LP_IQ2010EXT_GetNumberOfPerPowerLevels(char *measurementName, int *numberOfPerPowerLevel);

//! Retrieve Analysis Results [Double] in average, minimum and maximum value
/*!
 * \param[in]  measurementName          The measurement name.  Please refer to \ref group_scalar_measurement "Vector Measurements" for all available measurement names.
 * \param[out] average Average value    Average value for all the result(s). Result set can be more than one if the specified capture count is larger than one when using multi-capture function 'IQ2010EXT_VsaMultiCapture(...)'
 * \param[out] minimum Minimum value    Minimum value of all the result(s)
 * \param[out] maximum Maximum value    Maximum value of all the result(s)
 *
 * \return ERR_OK if successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
 */
IQMEASURE_API int		LP_IQ2010EXT_GetDoubleMeasurements(char *measurementName, double *average, double *minimum, double *maximum);

//! Retrieve Analysis Results [char]
/*!
 * \param[in] measurementName The measurement name.  Please refer to \ref group_scalar_measurement "Vector Measurements" for all available measurement names.
 * \param[out] result Result in 'char' pointer type
 * \param[in] bufferSize Indicates the buffer size of result
 * \param[in] indexResult (starts with 0 as the first set of result) indicates which set of result to be retrieved. It has to be less than the number of capture set in IQ2010EXT_VsaMultiCapture(...)
 *
 * \return ERR_OK if successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
 */
IQMEASURE_API int		LP_IQ2010EXT_GetStringMeasurement(char *measurementName, char *result, int  bufferSize, int indexResult = 0);

//! Retrieve Analysis Results [Double] in vector format ( more than one value in one particular result set as indicated by indexResult )
/*!
 * \param[in] measurementName The measurement name.  Please refer to \ref group_vector_measurement "Vector Measurements" for all available measurement names
 * \param[out] values Returns the real part of vector
 * \param[in] bufferLength Indicates the length of bufferReal and bufferImag (in number of element)
 * \param[out] *dataSize indicates the number of element in the result. If the dataSize is greater than bufferLength, then only the data with bufferLength will be filled up in the buffer
 * \param[in] indexResult (starts with 0 as the first set of result) indicates which set of result to be retrieved. It has to be less than the number of capture set in IQ2010EXT_VsaMultiCapture(...)
 *
 * \return ERR_OK if successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
 */
IQMEASURE_API int		LP_IQ2010EXT_GetVectorMeasurement(char *measurementName, double values[], int bufferLength, int *dataSize, int indexResult = 0);

//! Retrieve Analysis Results [int]
/*!
 * \param[in] measurementName The measurement name.  Please refer to \ref group_vector_measurement "Vector Measurements" for all available measurement names
 * \param[out] result Result in 'int' type
 * \param[in] indexResult (starts with 0 as the first set of result) indicates which set of result to be retrieved. It has to be less than the number of capture set in IQ2010EXT_VsaMultiCapture(...)
*/
IQMEASURE_API int		LP_IQ2010EXT_GetIntMeasurement(char *measurementName, int *result, int indexResult = 0);

//! Initializes IQ2010 Extension
/*!
 * \return ERR_OK if initialzation is done successfully; otherwise call IQ2010EXT_GetLastErr() for detailed error message.
 * \remark This function has to be called right after IQ2010 software has been connected to the test system
 */
IQMEASURE_API int		LP_IQ2010EXT_Init(void);

//! Exit IQ2010 extension
/*!
 * \return ERR_OK if exiting IQ2010 extension successfully; otherwise call IQ2010EXT_GetLastErr() for detailed error message.
 * \remark This function has to be called if user wants to switch the tester back to legacy mode after using IQ2010 extension.
 */
IQMEASURE_API int		LP_IQ2010EXT_Terminate(void);

/*------------------*
*	GPS Functions	*
*-------------------*/
enum LP_GPS_REPLAY_MODE
{
   GPS_REPLAY_OFF,
   GPS_REPLAY_ON
};

enum LP_GPS_CHANNEL_NUMBER
{
   GPS_CHANNEL_NUMBER_1,	//!< Indicates the first GPS channel
   GPS_CHANNEL_NUMBER_2,	//!< Indicates the second GPS channel
   GPS_CHANNEL_NUMBER_3,	//!< Indicates the third GPS channel
   GPS_CHANNEL_NUMBER_4,	//!< Indicates the fourth GPS channel
   GPS_CHANNEL_NUMBER_5,	//!< Indicates the fifth GPS channel
   GPS_CHANNEL_NUMBER_6	    //!< Indicates the sixth GPS channel
};


/*! @defgroup gps_measurement Measurement
 *  The process sequence is :
 * 1. Activate Tester
 *    LP_GPS_SetActivate
 * 2. Call the main test function :
 *    LP_GPS_LoadScenarioFile & LP_GPS_PlayScenarioFile
 *    LP_GPS_ModulatedMode
 *    LP_GPS_ContinueWave
 * 3. Deactivate Tester
 *    LP_GPS_SetDeactivate
 * @. Others
 *    LP_GPS_SetData
 *    LP_GPS_SetChannelInfo
 * @. Internal function
 *    LP_GPS_CaculateTesterPower
 */

//! Calling LP_GPS_SetActivate to activate Tester.
IQMEASURE_API int		LP_GPS_SetActivate();

//! Calling LP_GPS_LoadScenarioFile to load scenario file.
/*!
 * \param[in] fileName     : Path of the scenario file.
 * \param[in] triggerState : Trigger State.
*/
IQMEASURE_API int		LP_GPS_LoadScenarioFile(char* fileName, IQV_GPS_TRIGGER_STATE triggerState);

//! Calling LP_GPS_PlayScenarioFile to start playing scenario file.
/*!
 * \param[in] power        : Power at the DUT.
 * \param[in] pathloss     : Cable loss.
*/
IQMEASURE_API int		LP_GPS_PlayScenarioFile(double powerDbm, double pathlossDb);

//! Calling LP_GPS_IQXEL_LoadScenarioFile to load scenario file. New for IQxel-M6
/*!
 * \param[in] fileName     : Path of the scenario file.
 * \param[in] triggerSource : Trigger Source. Default: Immediate
*/
IQMEASURE_API int		LP_GPS_IQXEL_LoadScenarioFile(char* fileName, IQXEL_TRIG_SOURCE triggerSource=TRIG_IMM);

//! Calling LP_GPS_IQXEL_PlayScenarioFile to start playing scenario file. New for IQxel-M6
/*!
 * \param[in] powerDbm        : Output power in dBm from tester.
 * \param[in] relativePower     : relative power gain for GPS signal. This is relative to output power settings.
*/
IQMEASURE_API int		LP_GPS_IQXEL_PlayScenarioFile(double powerDbm, double relativePower=0);


//! Calling LP_GPS_ModulatedMode to transmit modulate mode signal, backward compatible with IQ2010
/*!
 * \param[in] Nav_Mode         : Navigation mode. 0 means OFF (Default), "IQV_OPERATION_MODE_MODULATION_NAV_DATA_OFF" and 1 means ON, "IQV_OPERATION_MODE_MODULATION".
 * \param[in] pathloss         : Cable loss.
 * \param[in] powerA[6]        : Power array of each channel at the DUT.
 * \param[in] satelliteNumA[6] : Satellite number array of each channel.
 * \param[in] dopplerFrequenc  : Doppler frequence revise.
 * \param[in] triggerState     : Trigger State, IQV_TRIGGER_STATE_INT means internal and IQV_TRIGGER_STATE_EXT means external.
*/
IQMEASURE_API int		LP_GPS_ModulatedMode(int Nav_Mode, double pathlossDb, double powerA[6], int satelliteNumA[6], int dopplerFrequency, IQV_GPS_TRIGGER_STATE triggerState);

//! Calling LP_GPS_ModulationMode to transmit modulate mode signal, new for IQxel-M6
/*!
 * \param[in] powerDbm                   : Power at the DUT.
 * \param[in] PRNnumber                  : Pseudo random number for GPS satellite.
 * \param[in] relativePowGain            : Relative power gain for GPS signal.
 * \param[in] modulationDataType         : Modulation data type. 0 means ALL0, 1 means ALL1, and 2 means NAV data bits
 * \param[in] timeOfWeek                 : The time of frame for GPS satellite.
 * \param[in] satelliteVel               : The velocity of GPS satellite in m/sec.
 * \param[in] triggerState               : Trigger State. 0 means INT and 1 means EXT1.
*/
IQMEASURE_API int		LP_GPS_ModulationMode(double powerDbm, int* PRNnumber, double* relativePowGain = NULL, int* modulationDataType = NULL, int* frequencyOffset = NULL, double* timeOfWeek = NULL, double* satelliteVel = NULL, int triggerState = 0);

//! Calling LP_GPS_ContinueWave to transmit continue wave signal.
/*!
 * \param[in] power        : Power at the DUT.
 * \param[in] pathloss     : Cable loss.
 * \param[in] triggerState : Trigger State.
 * \param[in] freqOffset   : Frequency offset (kHz) for Continuous Wave.
*/
IQMEASURE_API int		LP_GPS_ContinueWave(double powerDbm, double pathlossDb, IQV_GPS_TRIGGER_STATE triggerState, int freqOffset = 0);

//! Calling LP_GPS_SetDeactivate to stop and deactivate Tester.
IQMEASURE_API int		LP_GPS_SetDeactivate();

//! Calling LP_GPS_GetChannelInfo to get information of each channel.
/*!
 * \param[in] channelNumber   : channel number.
 * \param[in] satelliteNumber : apply satellite number of the channel.
 * \param[in] powerDbm        : apply power of the channel.
*/
IQMEASURE_API int		LP_GPS_GetChannelInfo(IQV_GPS_CHANNEL_NUMBER channelNumber,
										 int *satelliteNumber,
										 double *powerDbm,
										 IQV_GPS_MODULATION_STATE *modulationState);

//! Calling LP_GPS_SetChannelInfo to set information of each channel.
/*!
 * \param[in] channelNumber   : channel number.
 * \param[in] satelliteNumber : setup satellite number of the channel.
 * \param[in] powerDbm        : setup power of the channel.
*/
IQMEASURE_API int		LP_GPS_SetChannelInfo(IQV_GPS_CHANNEL_NUMBER channelNumber,
										int satelliteNumber,
										double powerDbm,
										IQV_GPS_MODULATION_STATE modulationState);

//! Calling LP_GPS_SetData to set information of the tester.
/*!
 * \param[in] inputName  : setup function name.
 * \param[in] inputValue : setup value of the function.
 * <b>SetParamOperationMode</b>    :
 *        <b>IQV_OPERATION_MODE_MODULATION_OFF</b>
 *        <b>IQV_OPERATION_MODE_MODULATION</b>
 *        <b>IQV_OPERATION_MODE_CW</b>
 *        <b>IQV_OPERATION_MODE_MODULATION_NAV_DATA_OFF</b>
 * <b>SetParamPowerDbm</b>         :
 *        <b>[tester Power]</b>
 * <b>SetParamModulationState</b>  :
 *        <b>IQV_MODULATION_STATE_OFF</b>
 *        <b>IQV_MODULATION_STATE_ON</b>
 * <b>SetParamDopplerFrequency</b> :
 *        <b>[doppler Frequency]</b>
 * <b>SetParamRfOutput</b>         :
 *        <b>IQV_RF_OUTPUT_OFF</b>
 *        <b>IQV_RF_OUTPUT_ON</b>
 * <b>SetParamTriggerState</b>     :
 *        <b>IQV_TRIGGER_STATE_INT</b>
 *        <b>IQV_TRIGGER_STATE_EXT</b>
 * <b>SetReplayScenarioMode</b>    :
 *        <b>IQV_GPS_REPLAY_OFF</b>
 *        <b>IQV_GPS_REPLAY_ON</b>
*/
IQMEASURE_API int		LP_GPS_SetData(const char* inputName, const char* inputValue);

IQMEASURE_API double	LP_GPS_CaculateTesterPower(double powerDbm, double pathlossDb);



/*! @defgroup Compass Measurement
 *  The process sequence is :
 * 1. Activate Tester
 *    LP_COMPASS_SetActivate
 * 2. Call the main test function :
 *    LP_COMPASS_ModulatedMode
 *    LP_COMPASS_ContinueWave
 * 3. Deactivate Tester
 *    LP_COMPASS_SetDeactivate
 * @. Others
 *    LP_COMPASS_SetData
 *    LP_COMPASS_SetChannelInfo
 * @. Internal function
 *    LP_COMPASS_CaculateTesterPower
 */

//! Calling LP_COMPASS_SetActivate to activate Tester.
IQMEASURE_API int		LP_COMPASS_SetActivate();

//! Calling LP_COMPASS_SetDeactivate to stop and deactivate Tester.
IQMEASURE_API int		LP_COMPASS_SetDeactivate();

//! Calling LP_COMPASS_ContinueWave to transmit continue wave signal.
/*!
 * \param[in] power        : Power at the DUT.
 * \param[in] pathloss     : Cable loss.
 * \param[in] triggerState : Trigger State.
 * \param[in] freqOffset   : Frequency offset (kHz) for Continuous Wave.
 */
IQMEASURE_API int		LP_COMPASS_ContinueWave(double powerDbm, double pathlossDb, IQV_GPS_TRIGGER_STATE triggerState, int freqOffset = 0);

//! Calling LP_COMPASS_ModulatedMode to transmit modulate mode signal.
/*!
 * \param[in] modulationDataType         : Modulation data type. 0 means ALL0, 1 means ALL1, and 2 means NAV data bits
 * \param[in] powerDbm                   : Power at the DUT.
 * \param[in] frequencyOffset            : Frequency offset.
 * \param[in] NHCode                     : Sets the BEIDOU Neuman-Hoffman code scrambling, 0 means OFF and 1 means ON.
 * \param[in] PRNnumber                  : Pseudo random number for COMPASS satellite.
 * \param[in] relativePowGain            : Relative power gain for COMPASS signal.
 * \param[in] timeOfWeek                 : The time of frame for COMPASS satellite.
 * \param[in] satelliteVel               : The velocity of COMPASS satellite in m/sec.
 * \param[in] triggerState               : Trigger State. 0 means INT and 1 means EXT1.
*/
IQMEASURE_API int		LP_COMPASS_ModulationMode(int* modulationDataType, double powerDbm, int* frequencyOffset, int NHCode, int* PRNnumber, 
									   double* relativePowGain=NULL, double* timeOfWeek=NULL, double* satelliteVel=NULL, int triggerState=0);


//! Print Current Glonass Parameters 
/*!
 * \param[in] forceUpdate Retrieve parameters from the tester, true:Yes, false:No.
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_PrintGlonassParameters(bool forceUpdate = true);

//! Set Glonass Operation Mode 
/*!
 * \param[in] operationMode One enum value of IQV_GPS_OPERATION_MODE.
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_Glonass_SetOperationMode(IQV_GPS_OPERATION_MODE operationMode);

//! Set Glonass TX Power 
/*!
 * \param[in] powerdBm Glonass TX Power(dBm)
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_Glonass_SetPower(double powerdBm);

//! Set Glonass TX frequency 
/*!
 * \param[in] frequencyChannel One enum value of IQV_GLONASS_FREQUENCY_CHANNEL.
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_Glonass_SetFrequency(int frequencyChannel);

//! Set Glonass TX RF Output On/Off
/*!
 * \param[in] rfOutput One enum value of IQV_GPS_RF_OUTPUT.
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_Glonass_SetRfOutput(IQV_GPS_RF_OUTPUT rfOutput);

//! Activate Glonass
/*!
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 * \remark This function activates Glonass. It must be activated prior to any usage of Glonass.
 *			If GPS is already activated, it deactivates GPS first.
 *			Upon activation, it automatically syncs parameter values.
 *			Some parameter values are affected by changes in Gps parameter values.(if already activated)
 */
IQMEASURE_API int LP_Glonass_SetActivate(void);

//! Deactivate Glonass
/*!
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 * \remark This function deactivates Glonass. The connection is still alive after deactivation.
 */
IQMEASURE_API int LP_Glonass_SetDeactivate();

//! Calling LP_Glonass_ContinueWave to transmit continue wave signal.
/*!
 * \param[in] power        : Power at the DUT.
 * \param[in] pathloss     : Cable loss.
 * \param[in] triggerState : Trigger State.
 * \param[in] freqOffset   : Frequency offset (kHz) for Continuous Wave.
 */
IQMEASURE_API int		LP_Glonass_ContinueWave(double powerDbm, double pathlossDb, IQV_GPS_TRIGGER_STATE triggerState, int freqOffset = 0);

//! Calling LP_Glonass_ModulationMode to transmit modulate mode signal.
/*!
 * \param[in] modulationDataType         : Modulation data type. 0 means ALL0, 1 means ALL1, and 2 means NAV data bits
 * \param[in] powerDbm                   : Power at the DUT.
 * \param[in] frequencyOffset            : Frequency offset.
 * \param[in] channel			         : Sets the channel number for GLONASS satellite.
 * \param[in] relativePowGain            : Relative power gain for COMPASS signal.
 * \param[in] timeOfWeek                 : The time of frame for COMPASS satellite.
 * \param[in] satelliteVel               : The velocity of COMPASS satellite in m/sec.
 * \param[in] triggerState               : Trigger State. 0 means INT and 1 means EXT1.
*/
IQMEASURE_API int		LP_Glonass_ModulationMode(int* modulationDataType, double powerDbm, int* frequencyOffset, int* channel, 
									   double* relativePowGain = NULL, double* timeOfWeek = NULL, double* satelliteVel = NULL, int triggerState = 0);


//! spcified the path that is used to invoke LitePoint Connectivity server.
/*!
 * \param[in] litePointConnectionPath The path of connectivity server.
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SetLpcPath(char *litePointConnectionPath);

/*----------------------------*
*		FM Functions		  *
*-----------------------------*/

//FM IQMeasure functions - merged on 08/09/2010

//! Set FM VSG Parameters
/*!
 * \param[in] carrierFreqHz Sets the FM Carrier Frequency (Hz).
 * \param[in] carrierPowerdBm Sets the FM Carrier Power (dBm).
 * \param[in] modulationEnable Sets the FM Modulation (ON/OFF).
 * \param[in] totalFmDeviationHz Sets the Total Peak FM Deviation (Hz). Note : totalFmPeakDeviationHz = Audio Deviation + Pilot Deviation( if stereo is enabled) + RDS Deviation( if RDS is enabled).
 * \param[in] stereoEnable Enable or Disable Stereo Mode ( ON/OFF).
 * \param[in] pilotDeviationHz Sets the Pilot Deviation(Hz) and will be used if Stereo is Enabled.
 * \param[in] rdsEnable Enable or Disable RDS ( ON/OFF).
 * \param[in] rdsDeviationHz Sets the RDS Deviation(Hz) and will be used if RDS is Enabled.
 * \param[in] preEmphasisMode Sets the Pre-Emphasis Filter value in us(Example: 0, 25, 50, 75).

 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_FM_SetVsg(unsigned int carrierFreqHz = 100000000,
							   double carrierPowerdBm = -40,
							   int modulationEnable = ON,
							   unsigned int totalFmDeviationHz = 75000,
							   int stereoEnable = OFF,
							   unsigned int pilotDeviationHz = 7500,
							   int rdsEnable = OFF,
							   unsigned int rdsDeviationHz = 2000,
							   unsigned int preEmphasisUs = PRE_EMPHASIS_75US,
							   char* rdsTransmitString = "LP_rds_transmit") ;

//! Return the FM VSG Parameters that is currently setup in the Tester
/*!
 * \param[out] carrierFreqHz Returns the FM Carrier Frequency (Hz).
 * \param[out] carrierPowerdBm Returns the FM Carrier Power (dBm).
 * \param[out] modulationEnabled Returns the FM Modulation State(1: ON/0: OFF).
 * \param[out] totalFmDeviationHz Returns the Total Peak FM Deviation (Hz). Note : totalFmPeakDeviationHz = Audio Deviation + Pilot Deviation( if stereo is enabled) + RDS Deviation( if RDS is enabled).
 * \param[out] stereoEnabled Returns Stereo Mode (1: ON/0: OFF).
 * \param[out] pilotDeviationHz Returns the Pilot Deviation(Hz) and will be used if Stereo is Enabled.
 * \param[out] rdsEnabled Returns RDS status(1: ON/0: OFF).
 * \param[out] rdsDeviationHz Returns the RDS Deviation(Hz) and will be used if RDS is Enabled.
 * \param[out] preEmphasisMode Returns the Pre-Emphasis Filter value.
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_FM_GetVsgSettings(unsigned int *carrierFreqHz,
									   double *carrierPowerdBm,
									   int *modulationEnabled,
									   unsigned int *totalFmDeviationHz,
									   int *stereoEnabled,
									   unsigned int *pilotDeviationHz,
									   int *rdsEnabled,
									   unsigned int *rdsDeviationHz,
									   unsigned int *preEmphasisMode
									   );

//! Sets the Single tone audio signal parameters for VSG Transmit: Stereo mode assigns the single tone to a left and right channel and the phase relationship between the channels is arbitrary.
/*!
 * \param[in] audioToneFreqHz Sets the single tone audio frequency (Hz).
 * \param[in] leftRightChannelSelect Sets the channel information for a stereo Tone. LEFT assigns single tone to LEFT Channel only ,RIGHT assigns single tone to RIGHT Channel only , LEFT_RIGHT assigns single tone to LEFT and RIGHT Channel with arbitrary phase relationship.
 * \param[in] audioToneAmpdB Sets the audio signal power for the single tone(dB). Note: 0 dB corresponds to 1Vpp.
 * \param[in] stereo Indicates if Stereo is Enabled or not ( ON / OFF).Note: The Stereo Mode is enabled in the LP_FM_SetVsg function.
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_FM_SetAudioSingleTone(double audioToneFreqHz,
										   int leftRightChannelSelect,
										   double audioToneAmpPercent,
										   int stereo);

//! Sets the multi tone audio signal parameters: upto maximum of 12 tones. Note: Stereo tone limits the maximum Left Channels and Right Channels to 6 and there is not defined phase relationship between the channels.
/*!
 * \param[in] freqHz Sets an array of audio tones (Hz). Note: Maximum size = 12.
 * \param[in] channelSelect Sets an array of audio tones channel corresponding to freqHz tones.LEFT assigns single tone to LEFT Channel only ,RIGHT assigns single tone to RIGHT Channel only. This array is ignored if Stereo is not enabled in the LP_FM_SetVsg Function. Note: Maximum size = 12.
 * \param[in] amplitudePercent Sets an array of audio signal power corresponding to freqHz tones (dB).
 * \param[in] stereo Indicates if Stereo is Enabled or not ( ON / OFF).Note: The Stereo Mode is enabled in the LP_FM_SetVsg function.
 * \param[in] toneCount Provides information about the total number of tones that will be set.
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_FM_SetAudioToneArray(double* freqHz,
										  int* channelSelect,
										  double* amplitudePercent,
										  int stereo,
										  unsigned int toneCount);

//! Sets the multi tone audio signal parameters: upto maximum of 12 tones. Note: Stereo tone limits the maximum Left Channels and Right Channels to 6 and there is not defined phase relationship between the channels.
/*!
 * \param[in] freqHz Sets an array of audio tones (Hz). Note: Maximum size = 12.
 * \param[in] channelSelect Sets an array of audio tones channel corresponding to freqHz tones.LEFT assigns single tone to LEFT Channel only ,RIGHT assigns single tone to RIGHT Channel only. This array is ignored if Stereo is not enabled in the LP_FM_SetVsg Function. Note: Maximum size = 12.
 * \param[in] amplitudeDeviationHz Sets an array of individual audio signal deviation corresponding to freqHz tones (Hz).
 * \param[in] stereo Indicates if Stereo is Enabled or not ( ON / OFF).Note: The Stereo Mode is enabled in the LP_FM_SetVsg function.
 * \param[in] toneCount Provides information about the total number of tones that will be set.
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_FM_SetAudioToneArrayDeviation(double* freqHz,
										  int* channelSelect,
										  double* amplitudeDeviationHz,
										  int stereo,
										  unsigned int toneCount);

//! Returns the multi tone audio signal Parameters
/*!
 * \param[out] freqHz Returns an array of audio tones (Hz).
 * \param[out] channelSelect Returns an array of audio tones channel corresponding to freqHz tones.
 * \param[out] amplitudePercent Returns an array of audio signal power corresponding to freqHz tones (dB). Note: 0 dB corresponds to 1Vpp.
 * \param[in] toneCount Provides information about the total number of tones that will be returned.
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_FM_GetAudioToneArray(double *freqHz,
										  int *channelSelect,
										  double *amplitudePercent,
										  unsigned int toneCount);

//! Clears all the Audio tones
/*!
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_FM_DeleteAudioTones();

//! Starts FM VSG Transmit
/*!
 * \return ERR_OK RR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_FM_StartVsg();

//! Stops FM VSG Transmit
/*!
 * \return ERR_OK
 */
IQMEASURE_API int		LP_FM_StopVsg();

//! IQMeasure FM VSG AM Distortion setup and Enable/Disable
/*!
 * \param[in] amDistortionEnable Enable AM Distortion settings ( ON/OFF).
 * \param[in] amFrequencyHz Sets the AM Modulating Frequency (Hz).
 * \param[in] amDepthPercent Sets the am Distortion depth (percent).
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_FM_SetVsgDistortion(int amDistortionEnable = OFF,
										 unsigned int amFrequencyHz = 1000,
										 unsigned int amDepthPercent = 30);

//! Get FM VSG AM Distortion Parameters currently applied to the tester
/*!
 * \param[out] amDistortionEnableStatus Returns AM Distortion Status (1: ON/0: OFF).
 * \param[out] amFrequencyHz Returns the AM Modulating Frequency (Hz).
 * \param[out] amDepthPercent Returns the am Distortion depth (percent).
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_FM_GetVsgDistortion(IQV_FM_VSG_DISTORTION_TYPES *amDistortionEnableStatus,
										 unsigned int *amFrequencyHz,
										 unsigned int *amDepthPercent);

//! Setup FM VSG Interference Parameters and Start transmit
/*!
 * \param[in] interfererRelativePowerdBm Sets the Interfering Carrier Power (dBm) - relative to the main carrier. Note: The total power(Main Carrier Power + Interference Carrier Power is limited to -40 dBm
 * \param[in] interfererCarrierOffsetHz Sets the FM Carrier Frequency Offset from the Main Carrier Frequency (Hz).
 * \param[in] interfererPeakFreqDeviationHz Sets the total peak FM Deviation (Hz) of the Interferer.
 * \param[in] interfererModulationEnable Enable or Disable FM Modulation on the Interferer ( ON/OFF).
 * \param[in] interfererAudioFreqHz Sets the Single tone audio frequency(Hz) corresponding to the interferencePeakDeviationHz.
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_FM_StartVsgInterference(double interfererRelativePowerdBm,
											 int interfererCarrierOffsetHz,
											 int interfererPeakFreqDeviationHz,
											 int interfererModulationEnable,
											 unsigned int interfererAudioFreqHz);

//! Get FM VSG Interference Parameters currently applied to the tester
/*!
 * \param[in] interfererRelativePowerdBm Returns the Interfering Carrier Power (dBm) - relative to the main carrier. Note: The total power is limited to -40 dBm
 * \param[in] interfererCarrierOffsetHz Returns the FM Carrier Frequency Offset from the Main Carrier Frequency (Hz).
 * \param[in] interfererPeakFreqDeviationHz Returns the total peak FM Deviation (Hz) of the Interferer.
 * \param[in] interfererModulationEnabled Returns FM Modulation Status on the Interferer (1: ON/0: OFF).
 * \param[in] interfererAudioFreqHz Returns the Single tone audio frequency(Hz).
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_FM_GetVsgInterferenceSettings(double *interfererRelativePowerdBm,
												   int *interfererCarrierOffsetHz,
												   int *interfererPeakFreqDeviationHz,
												   int *interfererModulationEnabled,
												   unsigned int *interfererAudioFreqHz);

//! Stops FM VSG Interference Transmit
/*!
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_FM_StopVsgInterference();

//! Set FM VSA Parameters
/*!
 * \param[in] carrierFreqHz Sets the VSA Carrier Frequency (Hz)
 * \param[in] expectedPeakInputPowerdBm Sets the Expected Peak input power (dBm)
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_FM_SetVsa(unsigned int carrierFreqHz = 100e6,
							   double expectedPeakInputPowerdBm = -20);

//! Get FM VSA Parameters currently applied to the tester
/*!
 * \param[out] getCarrierFreqHz Gets the VSA Carrier Frequency (Hz)
 * \param[out] getInputPowerdBm Gets the Expected Peak input power (dBm)
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_FM_GetVsaSettings(unsigned int *carrierFreqHz,
									   double *expectedPeakInputPowerdBm);

//! Start VSA Data capture
/*!
 * \param[in] captureTimemillisec Sets the VSA Capture Time (ms)
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_FM_VsaDataCapture(double captureTimemillisec = 170.67);

//! performs RF Analysis on the VSA Data Capture
/*!
 * \param[in] rfRBWHz Specifies the RF Resolution Bandwidth (Hz)
 * \param[in] rfOBWPercent Specifies the RF Occupied Bandwidth (percent)
 * \param[in] rfPowerMeasBWHz Specifies the RF Power Measurement Bandwidth (Hz)
 * \param[in] windowType Specifies the Window type (BLACKMAN_HARRIS, HANNING, FLAT_TOP)
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_FM_Analyze_RF(int rfRBWHz = 200,
								   int rfOBWPercent = 99,
								   int rfPowerMeasBWHz = 200000,
								   int windowType = BLACKMAN_HARRIS);

//! performs Demod Analysis on the VSA Data Capture
/*!
 * \param[in] audioRBWHz Specifies the Audio Resolution Bandwidth (Hz)
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_FM_Analyze_Demod(int audioRBWHz = 100);

//! performs Mono Audio Analysis on the VSA Data Capture ( or ) Audio waveform data loaded using LP_FM_LoadAudioCapture function
/*!
 * \param[in] audioRBWHz Specifies the Audio Resolution Bandwidth (Hz)
 * \param[in] audioFreqLowLimitHz Specifies the lower bound (Hz) of analyzed range for audio analysis (Hz)
 * \param[in] audioFreqHiLimitHz Specifies the Upper bound (Hz) of analyzed range for audio analysis (Hz)
 * \param[in] filterCount Specifies total number of audio filters to be applied Note: MAX = 3
 * \param[in] filterType1 Type of filter to be applied
 * \param[in] filterParam1 Filter parameter corresponding to filterType1
 * \param[in] filterType2 Type of filter to be applied
 * \param[in] filterParam2  Filter parameter corresponding to filterType2
 * \param[in] filterType3 Type of filter to be applied
 * \param[in] filterParam3  Filter parameter corresponding to filterType3
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_FM_Analyze_Audio_Mono(int audioRBWHz = 50,
										   int audioFreqLowLimitHz = 300,
										   int audioFreqHiLimitHz = 15000,
										   int filterCount = 3,
									       int filterType1 = 2,
									       double filterParam1 = 15000,
									       int filterType2 = 6,
									       double filterParam2 = 0,
									       int filterType3 = 3,
									       double filterParam3 = 75e-6 );

//! performs Stereo Audio Analysis on the VSA Data Capture ( or ) Audio waveform data loaded using LP_FM_LoadAudioCapture function
/*!
 * \param[in] audioRBWHz Specifies the Audio Resolution Bandwidth (Hz)
 * \param[in] audioFreqLowLimitHz Specifies the lower bound (Hz) of analyzed range for audio analysis (Hz)
 * \param[in] audioFreqHiLimitHz Specifies the Upper bound (Hz) of analyzed range for audio analysis (Hz)
 * \param[in] filterCount Specifies total number of audio filters to be applied Note: MAX = 3
 * \param[in] filterType1 Type of filter to be applied
 * \param[in] filterParam1 Filter parameter corresponding to filterType1
 * \param[in] filterType2 Type of filter to be applied
 * \param[in] filterParam2  Filter parameter corresponding to filterType2
 * \param[in] filterType3 Type of filter to be applied
 * \param[in] filterParam3  Filter parameter corresponding to filterType3
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_FM_Analyze_Audio_Stereo(int audioRBWHz,
										     int audioFreqLowLimitHz,
										     int audioFreqHiLimitHz,
											 int filterCount,
											 int filterType1,
											 double filterParam1,
											 int filterType2,
											 double filterParam2,
											 int filterType3,
											 double filterParam3);

//! Get an FM scalar measurement result
/*!
 * \param[in] measurement The measurement name.  Please refer to \ref group_fm_scalar_measurement "Scalar FM Measurements" for all available measurement names
 * \param[in] index The index of the measurement.  In most case, index would be zero.
 *
 * \return The value of the measurement.  -99999.99 (a special defined negative value) will be returned if no measurement available
 */
IQMEASURE_API double	LP_FM_GetScalarMeasurement(char *measurement, int index=0);

//! Get a vector measurement result
/*!
 * \param[in] measurement The measurement name.  Please refer to \ref group_fm_vector_measurement "Vector FM Measurements" for all available measurement names
 * \param[out] bufferReal Returns the real part of vector
 * \param[out] bufferImag Returns the imag part of vector
 * \param[in] bufferLength Indicates the length of bufferReal and bufferImag (in Bytes)
 *
 * \return The number of elements in bufferReal or/and bufferImag
 */
IQMEASURE_API int		LP_FM_GetVectorMeasurement(char *measurement, double bufferReal[], double bufferImag[], int bufferLength);

//! Captures the Audio data using the AIM Module
/*!
 * \param[in] saveFileLocation Specifies the location including the file name where the wav file needs to be stored( Path)
 * \param[in] captureDurationSec Specifies the capture duration ( seconds)
 * \param[in] samplingRate Specifies the sampling rate ( Hz)
 * \param[in] stereo Specifies the stereo status (ON/OFF)
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_FM_AudioDataCapture(char* saveFileLocation,
										 double captureDurationMilliSec,
										 int samplingRate,
										 int stereo);

//! Loads the Audio wave file for Audio Analysis
/*!
 * \param[in] fileName Specifies the full path of the file that is to be loaded ( Path)
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_FM_LoadAudioCapture(char* fileName);

IQMEASURE_API int		LP_FM_AudioStimulusGenerateAndPlayMultiTone(double sampleRateHz = 48000,
													double deviationPercent = 100,
													double peakVoltageLevelVolts = 1,
													int toneCount = 1,
													int stereoEnable = 0,
													int* leftRightRelation = NULL,
													double* freqHz = NULL,
													double durationMilliSeconds = 500,
													char* audioWaveFileOutput = NULL);


IQMEASURE_API int		LP_FM_AudioStimulusGenerateAndPlaySingleTone(double sampleRateHz = 48000,
													double deviationPercent = 100,
													double peakVoltageLevelVolts = 1,
													int stereoEnable = 0,
													int leftRightRelation = LEFT_EQUALS_RIGHT,
													double freqHz = 1000,
													double durationMilliSeconds = 500,
													char* audioWaveFileOutput = NULL);

//! Stop play FM audio
/*!
* \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int		LP_FM_StopAudioPlay();


//! Get memory addresses for a capture
/*!
* \param[in] dut
* \param[in] captureIndex
* \param[out] real
* \param[out] imag
* \param[out] length
*
* \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int		LP_GetCapture(int    dut,
                                int    captureIndex,
                                double *real[],
                                double *imag[],
                                int    length[]);

//! Enable/Disable Multi-thread
/*!
* \param[in] enableMultiThread
* \param[in] numberOfThreads
*
* \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int		LP_EnableMultiThread(int enableMultiThread,
									   int numberOfThreads);
IQMEASURE_API int		LP_SetUserDataPtr(void *userData);

//! Set default NFC
/*!
* \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int		LP_SetDefaultNfc();

//! Set Tx Amplitude
/*!
* \param[in] userSignalAmplitudeType - in A/m or Volt
* \param[in] userSignalAmplitude
*
* \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int		LP_SetNfcTxAmplitude(IQV_USER_SIGNAL_AMPLITUDE_TYPE userSignalAmplitudeType, double userSignalAmplitude);

//! Set Trigger Mask NFC
/*!
* \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int		LP_SetTriggerMaskNfc();

//! Initiate NFC
/*!
* \param[in] filename - File name of the wave file
*
* \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int		LP_InitiateNfc(char *fileName);

//! Set Rx NFC
/*!
* \param[in] triggerTimeout - trigger timeout
* \param[in] sampleingTimeSecs - sampleing time
*
* \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int		LP_SetRxNfc(double triggerTimeout, double sampleingTimeSecs);

//! Set Tx NFC
/*!
* \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int		LP_SetTxNfc();

//! Continuous Data Capture Handling
/*!
* \param[in] dcContEnum - type of data capture handling
*
* \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int		LP_ContCapture(IQV_DC_CONT_ENUM dcContEnum);

//! Terminate NFC
/*!
* \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int		LP_TerminateNfc();

//! Analysis NFC
/*!
* \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int		LP_AnalysisNfc();

//! Loads the modulation file (waveform) to VSG
/*!
 * \param[in] modFileName The .mod file to be loaded
 * \param[in] autoPlay - Lets the user to turn of autoPlay On or Off(TRUE or FALSE)
 * \param[in] loadInternalWaveform Choose to load modFile from internal tester (1) or to upload it from PC (0). The default is (0). This option is only valid with IQxel tester otherwise it is ignored.
 *            For more details, refer to the description in LP_SetVsgModulation().
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SetVsgModulation_SetPlayCondition(char *modFileName, bool autoPlay = false, int loadInternalWaveform = 0);

//! Sets up VSA for Bluetooth data capturing and lets user select the VSA IF
/*!
 * \param[in] rfFreqHz The center frequency of Bluetooth RF signal (Hz)
 * \param[in] rfAmplDb The amplitude of the peak power (dBm) of the signal
 * \param[in] port The port to which the VSG connects, with the following options:
 *				- =1: OFF
 *				- =2: RF port 1 (RF1)
 *				- =3: RF port 2 (RF2)
 *				- =4: RF port 3 (RF3)
 *				- =5: RF port 4 (RF4)
 * \param[in] triggerLevelDb The trigger level (dBm) used for signal trigger (ignored in Free-run and External Trigger Modes)
 * \param[in] triggerPreTime The pre-trigger time used for signal capture
 * \param[in] btShiftHz The VSA IF Freq (Hz). Default : 7MHz
 * \param[in] extAttenDb The external attenuation (dB).  Set to 0 always.
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 * \remark For VSA to work optimally with the input signal, set rfAmplDb to the peak power of the input signal.
 */
IQMEASURE_API int		LP_SetVsaBluetooth_BTShiftHz(double rfFreqHz, double rfAmplDb, int port, double extAttenDb = 0, double triggerLevelDb = -25, double triggerPreTime = 10e-6, double btShiftHz = 7e6);
IQMEASURE_API int		LP_FM_SetVsa_Agc_On(unsigned int carrierFreqHz, double expectedPeakInputPowerdBm);

//IQmeasure_DTNA

//! Sets up VSG including selecting the triggerType
/*!
 * \param[in] rfFreqHz The center frequency of VSG (Hz)
 * \param[in] rfGainDb The output power level of VSG (dBm)
 * \param[in] port The port to which the VSG connects, with the following options:
 *				- =1: OFF
 *				- =2: RF port 1 (RF1)
 *				- =3: RF port 2 (RF2)
 *				- =4: RF port 3 (RF3)
 *				- =5: RF port 4 (RF4)
 * \param[in] triggerType Select VSG trigger Type. 0 = IQV_VSG_TRIG_FREE_RUN; 1 = IQV_VSG_TRIG_EXT_1; 2 = IQV_VSG_TRIG_EXT_2
 * \param[in] triggerMode Select VSG trigger Mode. 0 = SSH; 1 = MPAC; 2 = MSH
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SetVsg_triggerType(double rfFreqHz, double rfPowerLeveldBm, int port, int triggerType);

//! Saves the captured data to a file (.sig)
/*!
 * \param[in] sigFileName Specifies the .sig file name
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_SaveVsaSignalFileText(char *txtFileName);

//! Saves the captured data to a txt file (.txt)
/*!
* \param[in] sigFileName Specifies the .sig file name
*
* \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int		LP_SaveTruncateCapture(char *sigFileName);

//! Perform VHT80 mask (240MHz) analysis
/*!
* \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
* \remark Before calling LP_AnalyzeVHT80Mask(), modeHT40 needs to set to 6 in LP_VsaDataCapture().
*/
IQMEASURE_API int		LP_AnalyzeVHT80Mask();

//! Perform VHT160 mask analysis
/*!
* \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
* \remark Before calling LP_AnalyzeVHT160Mask(), modeHT40 needs to set to 7 in LP_VsaDataCapture().
*/
IQMEASURE_API int		LP_AnalyzeVHT160Mask();

//!Following functions for fast power measurement, since API analysis power requires at least 10us data, but MPS for OFDM only need 8us
IQMEASURE_API int		LP_FastCalGetPowerData (int bGetOrFree);
IQMEASURE_API int		LP_PwrCalFreeElement(void);
IQMEASURE_API int		LP_PwrCalFastGetElement(void);
IQMEASURE_API int		LP_FastCalMeasPower (unsigned int start_time, unsigned int  stop_time, double *result);
IQMEASURE_API int		LP_FastCalMeasPowerall (unsigned int packetOffsetForAnalysis, unsigned int packetCountForAnalysis, char response[], unsigned int *actual_size);

IQMEASURE_API int		SetTesterHwVersion(double hwVersion);
IQMEASURE_API int		GetTesterHwVersion(double *hwVersion);


IQMEASURE_API int		LP_SetIQmeasureUseInsDrv(int bUseInsDrv = 0);

//! Perform 802.11 ac Analysis on current capture
/*!
* \param[in] mode Valid options are:
*         - "nxn": for true 802.11ac capture
*         - "composite": for a composite capture
*         - "evms": for EVM per signal analysis
*         - "sequential_mimo": for a switched capture
* \param[in] enablePhaseCorr Phase Correction Mode:
*         - 0: Disable
*         - 1: Enable (Default)
* \param[in] enableSymTimingCorr Symbol Timing Correction:
*         - 0: Disable
*         - 1: Enable (Default)
* \param[in] enableAmplitudeTracking Amplitude Tracking:
*         - 0: Disable (Default)
*         - 1: Enable
* \param[in] decodePSDU Decode PSDU:
*         - 0: Disable (Default)
*         - 1: Enable
* \param[in] enableFullPacketChannelEst Channel Estimate on full packet:
*         - 0: Disable (Default)
*         - 1: Enable
* \param[in] frequencyCorr Frequency Correlation:
*         - 1:Frequency correction on short legacy training fields
*         - 2:Frequency correction on long legacy training fields
*         - 3:Frequency correction based on whole packet
*         - 4:Frequency correction on signal fields (legacy and HT) in addition to long training fields
* \param[in] numberOfPacketToAnalysis number of packets to be analyzed for average EVM:
*         - 0 means all packet(s),
*         - 1 for first packet,
*         - 2 for the first 2 packets
*         . . . etc
*         If number of numberOfPacketToAnalysis is greater than the captured packet, the maximum captured packet (complete packet) will be used
* \param[in] referenceFile PSDU reference file (needed for Composite EVM)
* \param[in] packetFormat indicates format of the 80211AC MIMO packet:
*         - 0: auto-detect mode (Default)
*         - 1: mixed format (11n)
*         - 2: greenfield format (11n)
*         - 3: legacy format (11a/g)
*         - 4: VHT format (11ac)
* \param[in] prePowStartSec indicates the start point of preamble for power measurement result "powerPreambleDbm"(IQxel Only):
*         - 8.8e-6: 8.8 micro second (Default)
* \param[in] prePowStopSec indicates the end point of preamble for power measurement result "powerPreambleDbm" (IQxel Only):
*         - 15.2e-6: 15.2 micro second (Default)
*
*
* \return ERR_OK if the 802.11n analysis succeeded; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int		LP_Analyze80211ac(char *mode ="nxn",
									int enablePhaseCorr            = 1,
									int enableSymTimingCorr        = 1,
									int enableAmplitudeTracking    = 0,
									int decodePSDU                 = 0,
									int enableFullPacketChannelEst = 0,
									int frequencyCorr = 2,
									char *referenceFile=NULL,
									int packetFormat = 0,
									int numberOfPacketToAnalysis = 1,
									double prePowStartSec = 8.8e-6,
								    double prePowStopSec = 15.2e-6);

//! Perform 802.11 ad Analysis on current capture

/*!
 * \param[in] mode Valid options are:
 *         - "nxn": for true 802.11ac capture
 *         - "composite": for a composite capture
 *         - "evms": for EVM per signal analysis
 *         - "sequential_mimo": for a switched capture
 * \param[in] enablePhaseCorr Phase Correction Mode:
 *         - 0: Disable
 *         - 1: Enable (Default)
 * \param[in] enableSymTimingCorr Symbol Timing Correction:
 *         - 0: Disable
 *         - 1: Enable (Default)
 * \param[in] enableAmplitudeTracking Amplitude Tracking:
 *         - 0: Disable (Default)
 *         - 1: Enable
 * \param[in] decodePSDU Decode PSDU:
 *         - 0: Disable (Default)
 *         - 1: Enable
 * \param[in] enableFullPacketChannelEst Channel Estimate on full packet:
 *         - 0: Disable (Default)
 *         - 1: Enable
 * \param[in] frequencyCorr Frequency Correlation:
 *         - 1:Frequency correction on short legacy training fields
 *         - 2:Frequency correction on long legacy training fields
 *         - 3:Frequency correction based on whole packet
 *         - 4:Frequency correction on signal fields (legacy and HT) in addition to long training fields
 * \param[in] numberOfPacketToAnalysis number of packets to be analyzed for average EVM:
 *         - 0 means all packet(s),
 *         - 1 for first packet,
 *         - 2 for the first 2 packets
 *         . . . etc
 *         If number of numberOfPacketToAnalysis is greater than the captured packet, the maximum captured packet
 *(complete packet) will be used
 * \param[in] referenceFile PSDU reference file (needed for Composite EVM)
 * \param[in] packetFormat indicates format of the 80211AD MIMO packet:
 *         - 0: auto-detect mode (Default)
 *         - 1: mixed format (11n)
 *         - 2: greenfield format (11n)
 *         - 3: legacy format (11a/g)
 *         - 4: VHT format (11ac)
 * \param[in] prePowStartSec indicates the start point of preamble for power measurement result "powerPreambleDbm"(IQxel
 * Only):
 *         - 8.8e-6: 8.8 micro second (Default)
 * \param[in] prePowStopSec indicates the end point of preamble for power measurement result "powerPreambleDbm" (IQxel
 * Only):
 *         - 15.2e-6: 15.2 micro second (Default)
 * \param[in] segment indicates segment number to be analyzed:
 *         - 1 (Default)
 *
 *
 * \return ERR_OK if the 802.11n analysis succeeded; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int       LP_Analyze80211ad(char *mode = "nxn",
                                          int enablePhaseCorr = 1,
                                          int enableSymTimingCorr = 1,
                                          int enableAmplitudeTracking = 0,
                                          int decodePSDU = 0,
                                          int enableFullPacketChannelEst = 0,
                                          int frequencyCorr = 1,
                                          int dcOffsetCorr = 1,
                                          char *referenceFile = NULL,
                                          int packetFormat = 0,
                                          int numberOfPacketToAnalysis = 1,
                                          double prePowStartSec = 8.8e-6,
                                          double prePowStopSec = 15.2e-6);

//IQMEASURE_API int LpDllGetVersion (char *manufacturer, char* modelNumber, char* serialNumber, char *firmwareRevision);
IQMEASURE_API int		LpDllGetVersion (char *verInfo);
IQMEASURE_API int		LpGetSysError (char *errMsg);
IQMEASURE_API int		LP_LoadUserData(double bufferReal[], double bufferImag[], int bufferLength, double sampleFreqHz);

//! Set FM Audio Tone Mode to Amplitude
/*!
* \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int		LP_FM_SetAudioToneModeAmplitude(void);// IQlite Merge; Tracy Yu; 2012-04-06

//! Set TX carrier frequency and power for FM
/*!
* \param[in] carrierFreqHz  TX Carrier frequency(Hz)
* \param[in] carrierPowerdBm  TX Carrier Power(dBm), default = -(DBL_MAX)
* \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int		LP_FM_SetFrequency(unsigned int carrierFreqHz, double carrierPowerdBm);  // IQlite Merge; Tracy Yu; 2012-04-06

//! Set TX carrier power for FM
/*!
* \param[in] carrierPowerdBm  TX Carrier Power(dBm)
* \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int		LP_FM_SetCarrierPower(double carrierPowerdBm );  // IQlite Merge; Buddy; 2012-04-06

//! Get generic tester type, output includes IQTYPE_2010, IQTYPE_XEL, IQTYPE_Z, IQTYPE_NFC, IQTYPE_LINK 
/*!
* \return an enum as defined in IQTESTER_TYPE.
*/
IQMEASURE_API IQTESTER_TYPE  LP_GetTesterType();

//! Get specific tester type within the IQxel test family
/*!
* \return an enum as defined in IQTESTER_TYPE.
*/
IQMEASURE_API IQTESTER_TYPE  LP_IQXEL_GetTesterType();

//! Get an indication whether the tester is configured to do true MIMO test or composite MIMO test.
/*!
* \return an enum as defined in IQTESTER_TYPE_MIMO_CONFIG.
*/
IQMEASURE_API IQTESTER_TYPE_MIMO_CONFIG  LP_GetTesterTypeMimoConfig();

//! Check whether the current tester has a certain attribute
/*!
* \param[in] attributeKeyword  attribute keyword to query
* \return -1 if the attribute check specified by the attributeKeyword is not supported; return 0 is the current tester does not have the specified attribute
* \return 1 if the current tester has the specified attribute.
*/
IQMEASURE_API int  LP_GetTesterAttribute(char *attributeKeyword);


// MPTA Functions
//! Check if MPTA is available
/*!
 * \return true if MPTA is available; otherwise return false.
 */
IQMEASURE_API bool LP_MptaAvailable();
// conflict with IQsequence, removed 8/15/12
//IQMEASURE_API bool LP_MptaGetSerialNumber(char *serialNumber,int buf_size);

//! Check if MPTA is enabled
/*!
 * \return ERR_OK if MPTA is enabled; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_MptaEnable();

//! Check if MPTA is disabled
/*!
 * \return ERR_OK if MPTA is disabled; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_MptaDisable();

//! Setup capture setting for mpta
/*!
 * \param[in] tx_path Tx path
 * \param[in] rfFreqHz Channel frequency
 * \param[in] peakSignaldB Peak signal level
 * \param[in] CaptureTimeUs Capture Time(us)
 * \return ERR_OK if MPTA is disabled; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_MptaSetupCapture(TX_PATH_ENUM tx_path, double rfFreqHz, double peakSignaldB, double CaptureTimeUs);

IQMEASURE_API int LP_Analyze80211n_Seq(char *type,
                                   int enablePhaseCorr            = 1,
                                   int enableSymTimingCorr        = 1,
                                   int enableAmplitudeTracking    = 0,
                                   int decodePSDU                 = 0,
                                   int enableFullPacketChannelEst = 0,
                                   char *referenceFile			  = NULL,
								   int numSections				  = 2,
								   double sectionLenSec			  = 200e-6);

//! Analyze Occupied Bandwidth
/*!
* \param[in] obwPercentage The total power percentage, 0 <= obwPercentage <= 1
* \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int LP_AnalyzeObw(double obwPercentage = 0.99);	//0 <= obwPercentage <= 1

//! Set Load Sense reference using IQxel-M, add on 05/05/2014; It supports IQxel-M for now
/*!
* \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int LP_IQXEL_LoadSense_SetRef();
//! Load Sense test using IQxel-M, add on 05/05/2014; It supports IQxel-M for now
/*!
 * \param[in] port  The RF port used 
 *				   2 - Port 1
 *				   3 - Port 2
 *				   4 - Port 3
 *				   5 - Port 4
 * \param[in] threshold  Load Sense detection threshold
 * \param[out] statusChanged   Returns whether the load status is changed. True means load is detected and false means otherwise.
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int LP_IQXEL_LoadSense(int port, double threshold, bool *statusChanged);

/******************************For Internal Use Only Begin*******************************************/

IQMEASURE_API int LP_IQXEL_LoadSense_SetRef_returnLoss(int port, double *returnLoss, bool saveCapture = false, char *fileName = NULL);

IQMEASURE_API int LP_IQXEL_LoadSense_returnLoss(int port, double threshold, bool *statusChanged, double *returnLoss, bool saveCapture = false, char *fileName = NULL);

IQMEASURE_API int LP_IQXEL_LoadSense_CalOpen();

IQMEASURE_API int LP_IQXEL_LoadSense_CalMatch();

/************************************For Internal Use Only End***************************************/

//! Fast per rx tests using IQxel, add on 04/03/2013;  It supports IQxel 80, IQxel160 Rout1 and Rout2
/*!
 * \param[in] Port  The RF port used 
 *				   2 - Left RF port
 *				   3 - Right RF port
 * \param[in] FreqMHz  The testd frequency in MHz
 * \param[in] VsgTransmittPtkCnt  The transmitted packet count in VSG
 * \param[in] VsgPowerStartLevel  The power sweep start level in dBm
 * \param[in] VsgPowerStopLevel   The power sweep stop level in dBm (Note: return error if VsgPowerStartLevel equals VsgPowerStopLevel)
 * \param[in] PowerStep           The step size in dB
 * \param[in] pcWaveFileName  The name of waveform files played
 * \param[out] AckNumReturn   The returned ack number array
 * \param[out] AckArraysize   The returned ack array size
*/
IQMEASURE_API int LP_IQXEL_FastPer_Sweep(unsigned int Port,  int FreqMHz, unsigned int VsgTransmittPtkCnt, double VsgPowerStartLevel, 
											   double VsgPowerStopLevel, double PowerStep, char *pcWaveFileName, unsigned int *AckNumReturn, unsigned int *AckArraysize);

IQMEASURE_API int LP_IQXEL_FastPer(unsigned int *Port,  int FreqMHz, unsigned int VsgTransmittPtkCnt, double VsgPowerStartLevel, 
                                         double VsgPowerStopLevel, double PowerStep, char *pcWaveFileName, int num_ports=1);

IQMEASURE_API int	LP_IQXEL_FastPer_Get_Results(int *AckNumReturn, int *AckArraysize);


//! Caputre multiple packets and returns average, max and min EVM results over multiple packets using IQxel, add on 04/04/2013
//! It supports IQxel 80, IQxel160 Rout1 and Rout2
/*!
 * \param[in] Mode			"OFDM" or "DSSS"
 * \param[in] SamplingTime	Capture length
 * \param[in] CaptureCount	Number of capture to be analyzed and averaged
 * \param[in] SkipCount		Number of skipped packets before capture
 * \param[out] EVMValues	Returns the average EVM, Max EVM and Min EVM over multiple captures (only first packet in each capture will be analyzed)
*/
IQMEASURE_API int LP_IQXEL_MultiCaptureEVMAver(char *Mode, double SamplingTime, unsigned int CaptureCount, unsigned int SkipCount, double *EVMValues);

//! Caputre multiple packets and returns average, max and min Power results over multiple packets using IQxel, add on 01/27/2014
//! It supports IQxel 80, IQxel160 Rout1 and Rout2
/*!
 * \param[in] Mode			"WIFI" or "BT"
 * \param[in] SamplingTime	Capture length; should be smaller than or equal to the packet length for correct packet count.
 * \param[in] CaptureCount	Number of capture to be analyzed and averaged
 * \param[in] SkipCount		Number of skipped packets before capture
 * \param[out] PowerValues	Returns the average Power, Max Power and Min Power over multiple captures
*/
IQMEASURE_API int LP_IQXEL_MultiCapturePowerAver(char *Mode, double SamplingTime, unsigned int CaptureCount, unsigned int SkipCount, double *PowerValues);

//! Sweep user-defined frequency bands that IQxel can support and find potential spurs of the signal, add on 08/06/2013
//! It supports IQxel 80
/*!
 * \param[in] channelFreq			Signal waveform center frequency
 * \param[in] signalBandwith		Signal waveform bandwidth
 * \param[in] chainMask				Bit mask for chains that are of interest (bit 0 for chain 0, bit 1 for chain 1, etc.)
 * \param[in] startScanningFreqMHz	Starting point of user-defined frequency band
 * \param[in] stopScanningFreqMHz	Stop point of user-defined frequency band
 * \param[out] peakPower			Returns the peak power amplitude for each chain
 * \param[out] peakPowerFreq		Returns the peak power frequency for each chain
*/
IQMEASURE_API int LP_IQXEL_MeasureSpur(double channelFreq, double signalBandwidth, int chainMask, double startScanningFreqMHz, double stopScanningFreqMHz, double peakPower[], double peakPowerFreq[]);

//!Get a vector channelEst matrix, returned by scpi, add on 9/20/2012, DTNA only
/*!  
 *\param[in] removeCSD remove the CSD. 
 *\param[in] polarDisp get the polar representation. 
 *\param[in] numberPacket number of packets to average over
 *\param[out] bufferReal Returns the real/magnitude part of vector
 *\param[out] bufferImag Returns the imag/angular part of vector
 *\param[in] bufferLength Indicates the length of bufferReal and bufferImag (in Bytes)
 *
 *\return The number of elements in bufferP1 or/and bufferP2
 */
IQMEASURE_API int LP_GetVectorChannelEst(double bufferP1[], double bufferP2[], int bufferLength, bool removeCSD, bool polarDisp = false, int numberPacket = 1);

//**************************************************************************//
//                              ASYNC START                                 //
//**************************************************************************//

typedef enum tagBWType
{
    LP_Async_BW_20,
    LP_Async_BW_40,
    LP_Async_BW_80,
    LP_Async_BW_160,
    LP_Async_BW_All_Band, // Bandwidth occupies 2G or 5G all channels.
    LP_Async_BW_2G_5G_Bands, // Bandwidth occupies 2G + 5G all channels.
	LP_Async_BW_80_80,
	LP_Async_BW_RSDB // have different bandwidth or center frequency on different chains
} LP_Async_BWType;

typedef enum tagTXRXReq
{
    TX_REQ,
    RX_REQ,
	RSDB_TX_RX_REQ //// have different REQ type on different chains
}LP_Async_TXRXReq;

typedef struct tagPortInfo 
{
	int rout;
	int port;
	LP_Async_BWType bandwidth; 
	LP_Async_TXRXReq req;
	unsigned int BSS_Primary;
	unsigned int BSS_Secondary;
	double powerDbm;
	double ackPower; 
}LP_Async_PortInfo; 





//!Initialize environment for Asynchronized Parallel Testing (APT)
/*!  
 *\param[in] appID application ID for each application process controling each DUT. 
 *\param[in] lockTimeoutSec timeout for each process to wait in the queue to obtain tester control per test item. 
 *\param[in] IQxelConnectionType (Ignore for IQ2010/IQnxn/IQview/IQflex; always set as 0 for IQxel80; )
 *				- =0: Default value for IQxel80
 *				- =1: ROUT1 for IQxel280/IQxel-M8
 *				- =2: ROUT2 for IQxel280/IQxel-M8
 *
 *\return 0 if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_Async_Init(int appID, unsigned int lockTimeoutSec, int IQxelConnectionType = 0);

//!Initialize environment for Asynchronized Parallel Testing (APT)
/*!  
 *\param[in] DUT id, customer input parameter
 *\param[in] lockTimeoutSec timeout for each process to wait in the queue to obtain tester control per test item. 
 *
 *\return 0 if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_Async_Initialize(int dutID, unsigned int lockTimeoutSec);
//! Terminates the APT environment.
/*!
 * \return 0 if successful.
 */
IQMEASURE_API int LP_Async_Term();

//! Initialize the handler recording the test item queue for all DUTs (DUT number, TX/RX, freq, bandwidth).
/*!
 *\param[in] crosstalkMethod, default = 0 old crosstalk checking method, 1 new universal crosstalk method
 * \return 0 if successful.
 */
IQMEASURE_API int LP_Async_InitTxRxHandler(int crosstalkMethod = 0);

//!Compete to get the tester control, if the tester is busy, wait in the queue
/*!   
 *\param[out] isGot true if got the tester control, false if not after timeout
 *
 *\return 0 if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_Async_GetTesterLock(bool* isGot);

//! Release tester control
/*!
 * \return 0 if successful.
 */
IQMEASURE_API int LP_Async_ReleaseTesterLock();

//!Request TX testing, call it before running DUT TX start command.(vDUT_Run(SET_XXX/TX_START))
/*!  
 *\param[in] freq TX test item frequency in MHz. 
 *\param[in] bwType 
 *				- =0: 20 MHz
 *				- =1: 40 MHz
 *				- =2: 80 MHz
 *				- =3: 160 MHz
 *              - =4: Whole 2.4G channels(if freq is on 2.4G), or Whole 5G channels(if freq is on 5G)
 *              - =5: Both 2.4G and 5G channels.
 *
 *\return 0 if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_Async_RequestTX(unsigned int freq, LP_Async_BWType bwType, double powerDbm, int VSAGPort, double ackPower = NO_ACK_DEFAULTPOWER);

//! Release TX testing, call it before running DUT TX stop command.(vDUT_Run(TX_STOP)).
/*!
 * \return 0 if successful.
 */
IQMEASURE_API int LP_Async_ReleaseTX();

//!Request RX testing, call it before running DUT RX start command.(vDUT_Run(SET_XXX/TX_START))
/*!  
 *\param[in] freq RX test item frequency in MHz. 
 *\param[in] bwType 
 *				- =0: 20 MHz
 *				- =1: 40 MHz
 *				- =2: 80 MHz
 *				- =3: 160 MHz
 *              - =4: Whole 2.4G channels(if freq is on 2.4G), or Whole 5G channels(if freq is on 5G)
 *              - =5: Both 2.4G and 5G channels.
 *\param[in] rxPowerDbm RX test power level
 *
 *\return 0 if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_Async_RequestRX(unsigned int freq, LP_Async_BWType bwType, double rxPowerDbm, int VSAGPort,double ackPower = NO_ACK_DEFAULTPOWER);

//! Release RX testing, call it before running DUT RX stop command.(vDUT_Run(TX_STOP)).
/*!
 * \return 0 if successful.
 */
IQMEASURE_API int LP_Async_ReleaseRX();

//!Request TX/RX testing, call it before running DUT TX/RX start command.(vDUT_Run(SET_XXX/TX_START)) 2014.11.20 perform universal cross-talk check 
/*!  
 *\param[in] numPort Number of active port used by current test item
 *\param[in] VSAGPort active port information
 *\return 0 if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_Async_RequestTXRX(int numPort, LP_Async_PortInfo *VSAGPort);

//! Release TX/RX testing, call it after running DUT TXRX stop command.(vDUT_Run(TX_STOP)).
/*!
 * \return 0 if successful.
 */
IQMEASURE_API int LP_Async_ReleaseTXRX();

//! Get tester lock status
/*!
 * \return true if tester is still locked by the current application process.
 */
IQMEASURE_API bool LP_Async_GetIsTesterLock();

//! Get TX test item occupation status
/*!
 * \return true if current TX test item issued by this application process is at the front of the test item queue.
 */
IQMEASURE_API bool LP_Async_GetIsReqTXLock();

//! Get RX test item occupation status
/*!
 * \return true if current RX test item issued by this application process is at the front of the test item queue.
 */
IQMEASURE_API bool LP_Async_GetIsReqRXLock();

//! Get TXRX test item occupation status
/*!
 * \return true if current TXRX test item issued by this application process is at the front of the test item queue.
 */
IQMEASURE_API bool LP_Async_GetIsReqTXRXLock();

//!Compete to get the specific token control on tester, if the tester is busy, wait in the queue
/*!   
 *\param[out] isGot true if got the token control, false if not after timeout
 *
 *\return 0 if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_Async_GetTokenLock(int tokenNum, bool* isGot);

//! Release specific token control on tester.
/*!
 * \return 0 if successful.
 */
IQMEASURE_API int LP_Async_ReleaseTokenLock(int tokenNum);

//!Compete to get a list of specific tokens control on tester, if the tester is busy, wait in the queue
/*!   
 *\param[out] isGot true if got the token control, false if not after timeout
 *
 *\return 0 if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_Async_GetMultiTokenLock(int numOfTokens, int *tokens, bool* isGot);

//! Release a list of specific tokens control on tester.
/*!
 * \return 0 if successful.
 */
IQMEASURE_API int LP_Async_ReleaseMultiTokenLock(int numOfTokens, int *tokens);

//!Print time stamp
/*!  
 *\param[in] tag tag name used to mark the time stamp.
 *\param[in] first check if this is the first recorded item; if not, the elapsed time will also be recorded. 
 *
 *\return 0 if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_Async_PrintTimeStamp(const char* tag, bool first = false);

//!Print response from tester if query "ERR:ALL?"
/*!  
 *\param[out] hasError true if there is error returned from the tester
 *
 *\return 0 if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_Async_printErrAll(bool* hasError = NULL);

//! Perform multiple VSA data capture and store data to multiple segments. The functions calling sequence is LP_Segms_VsaDataCapture --> LP_Segms_Analyze80211ag --> LP_Segms_GetScalarMeasurement
/*!
 * \param[in] numSegms Number of segments to capture and store.
 * \param[in] samplingTimeSecs Capture time in seconds
 * \param[in] triggerType Trigger type used for capturing.  Valid options are:
 *      - 1 = Free-run
 *      - 2 = External trigger
 *      - 6 = Signal Trigger
 * \param[optinal] modeHT40 Specifies the capture type. Refer to IQV_CAPTURE_TYPE_ENUM on iqapidefines.h.
 * \param[in] sampleFreqHz IQxel options: 80Mhz/160Mhz;IQ2010/IQnxn/IQview 80Mhz only (Forced to set as 80Mhz if set different from 80Mhz)
 * \return ERR_OK if the data capture is successful; otherwise call LP_GetErrorString() for detailed error message.
 * \remark modeHT40 only needs to set to 1 if the HT40 mask (120MHz) analysis is desired.  For LP_Analyze80211n(), this flag can be set to 0.
 */
IQMEASURE_API int LP_Segms_VsaDataCapture(int numSegms, double samplingTimeSecs, int triggerType=6, double sampleFreqHz=160e6,
                                          int ht40Mode=OFF, IQMEASURE_CAPTURE_NONBLOCKING_STATES nonBlockingState = BLOCKING);


//! Perform multiple VSA data capture, store data to multiple segments and performs BT analysis on the segments. The function calling sequence is LP_Segms_VsaDataCaptureAndBTAnalysis --> LP_Segms_GetScalarMeasurement
/*!\param[in] dataRate An array to specify the Bluetooth data rate for each segment, with the following options: 
 *         - 1: 1Mbit/s BDR
 *         - 2: 2Mbit/s EDR
 *         - 3: 3Mbit/s EDR
 *         - 0: Auto detect
 * \param[in] captureLength An array of capture length for each segment to be captured.
 * \param[in] numSegms Number of segments to capture and store.
 * \param[in] triggerType Trigger type used for capturing.  Valid options are:
 *			- 1 = Free-run
 *			- 2 = External trigger
 *			- 6 = Signal Trigger
 * \param[in] sampleFreqHz IQxel options: 80Mhz/160Mhz;IQ2010/IQnxn/IQview 80Mhz only (Forced to set as 80Mhz if set different from 80Mhz)
 * \param[in] analysis_type, Specifies what type of analysis to perform. Default: 'AllPlus'. Valid values are as follows:
 *         - PowerOnly
 *         - 20dbBandwidthOnly
 *         - PowerandFreq
 *         - All  (This is the set default value)
 *		   - ACP  (only does the new ACP analysis)
 *		   - AllPlus  (performs all the analyses that are done by ¡®All?plus the ACP analysis)
 * \param[in] nonBlockingState Specify which mode to be used while triggering a data capture. Available modes specified in IQMEASURE_CAPTURE_NONBLOCKING_STATES
 * \return ERR_OK if the data capture is successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_Segms_VsaDataCaptureAndBTAnalysis(double* dataRate, double* captureLength, int numSegms, int triggerType=6, double sampleFreqHz=160e6,
													  char* analysisType = "AllPlus", IQMEASURE_CAPTURE_NONBLOCKING_STATES nonBlockingState = ARM_TRIGGER);

//! Waits till the previous operations are completed
/*!
 * \return ERR_OK when previous operations on this connection are complete.
 */
IQMEASURE_API int LP_QueryPreviousOperationStatus();

//! Perform 802.11 ac Analysis on current capture
/*!
* \param[in] numSegmsToAnalyze number of segements to analyze
* \param[in] mode Valid options are:
*         - "nxn": for true 802.11ac capture
*         - "composite": for a composite capture
*         - "evms": for EVM per signal analysis
*         - "sequential_mimo": for a switched capture
* \param[in] enablePhaseCorr Phase Correction Mode:
*         - 0: Disable
*         - 1: Enable (Default)
* \param[in] enableSymTimingCorr Symbol Timing Correction:
*         - 0: Disable
*         - 1: Enable (Default)
* \param[in] enableAmplitudeTracking Amplitude Tracking:
*         - 0: Disable (Default)
*         - 1: Enable
* \param[in] decodePSDU Decode PSDU:
*         - 0: Disable (Default)
*         - 1: Enable
* \param[in] enableFullPacketChannelEst Channel Estimate on full packet:
*         - 0: Disable (Default)
*         - 1: Enable
* \param[in] frequencyCorr Frequency Correlation:
*         - 1:Frequency correction on short legacy training fields
*         - 2:Frequency correction on long legacy training fields
*         - 3:Frequency correction based on whole packet
*         - 4:Frequency correction on signal fields (legacy and HT) in addition to long training fields
* \param[in] numberOfPacketToAnalysis number of packets to be analyzed for average EVM:
*         - 0 means all packet(s),
*         - 1 for first packet,
*         - 2 for the first 2 packets
*         . . . etc
*         If number of numberOfPacketToAnalysis is greater than the captured packet, the maximum captured packet (complete packet) will be used
* \param[in] referenceFile PSDU reference file (needed for Composite EVM)
* \param[in] packetFormat indicates format of the 80211AC MIMO packet:
*         - 0: auto-detect mode (Default)
*         - 1: mixed format (11n)
*         - 2: greenfield format (11n)
*         - 3: legacy format (11a/g)
*         - 4: VHT format (11ac)
* \param[in] prePowStartSec indicates the start point of preamble for power measurement result "powerPreambleDbm"(IQxel Only):
*         - 8.8e-6: 8.8 micro second (Default)
* \param[in] prePowStopSec indicates the end point of preamble for power measurement result "powerPreambleDbm" (IQxel Only):
*         - 15.2e-6: 15.2 micro second (Default)
*
*
* \return ERR_OK if the 802.11n analysis succeeded; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int LP_Segms_Analyze80211ac(int numSegmsToAnalyze, char *mode ="nxn",
                                          int enablePhaseCorr            = 1,
                                          int enableSymTimingCorr        = 1,
                                          int enableAmplitudeTracking    = 0,
                                          int decodePSDU                 = 0,
                                          int enableFullPacketChannelEst = 0,
                                          int frequencyCorr = 2,
                                          char *referenceFile=NULL,
                                          int packetFormat = 0,
                                          int numberOfPacketToAnalysis = 1,
                                          double prePowStartSec = 8.8e-6,
                                          double prePowStopSec = 15.2e-6);

//! Perform 802.11 ad Analysis on current capture

/*!
 * \param[in] numSegmsToAnalyze number of segements to analyze
 * \param[in] mode Valid options are:
 *         - "nxn": for true 802.11ad capture
 *         - "composite": for a composite capture
 *         - "evms": for EVM per signal analysis
 *         - "sequential_mimo": for a switched capture
 * \param[in] enablePhaseCorr Phase Correction Mode:
 *         - 0: Disable
 *         - 1: Enable (Default)
 * \param[in] enableSymTimingCorr Symbol Timing Correction:
 *         - 0: Disable
 *         - 1: Enable (Default)
 * \param[in] enableAmplitudeTracking Amplitude Tracking:
 *         - 0: Disable (Default)
 *         - 1: Enable
 * \param[in] decodePSDU Decode PSDU:
 *         - 0: Disable (Default)
 *         - 1: Enable
 * \param[in] enableFullPacketChannelEst Channel Estimate on full packet:
 *         - 0: Disable (Default)
 *         - 1: Enable
 * \param[in] frequencyCorr Frequency Correlation:
 *         - 1:Frequency correction on short legacy training fields
 *         - 2:Frequency correction on long legacy training fields
 *         - 3:Frequency correction based on whole packet
 *         - 4:Frequency correction on signal fields (legacy and HT) in addition to long training fields
 * \param[in] numberOfPacketToAnalysis number of packets to be analyzed for average EVM:
 *         - 0 means all packet(s),
 *         - 1 for first packet,
 *         - 2 for the first 2 packets
 *         . . . etc
 *         If number of numberOfPacketToAnalysis is greater than the captured packet, the maximum captured packet
 *(complete packet) will be used
 * \param[in] referenceFile PSDU reference file (needed for Composite EVM)
 * \param[in] packetFormat indicates format of the 80211AC MIMO packet:
 *         - 0: auto-detect mode (Default)
 *         - 1: mixed format (11n)
 *         - 2: greenfield format (11n)
 *         - 3: legacy format (11a/g)
 *         - 4: VHT format (11ac)
 * \param[in] prePowStartSec indicates the start point of preamble for power measurement result "powerPreambleDbm"(IQxel
 * Only):
 *         - 8.8e-6: 8.8 micro second (Default)
 * \param[in] prePowStopSec indicates the end point of preamble for power measurement result "powerPreambleDbm" (IQxel
 * Only):
 *         - 15.2e-6: 15.2 micro second (Default)
 *
 *
 * \return ERR_OK if the 802.11n analysis succeeded; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_Segms_Analyze80211ad(int numSegmsToAnalyze,
                                          char *mode = "nxn",
                                          int enablePhaseCorr = 1,
                                          int enableSymTimingCorr = 1,
                                          int enableAmplitudeTracking = 0,
                                          int decodePSDU = 0,
                                          int enableFullPacketChannelEst = 0,
                                          int frequencyCorr = 1,
                                          int dcOffsetCorr = 1,
                                          char *referenceFile = NULL,
                                          int packetFormat = 0,
                                          int numberOfPacketToAnalysis = 1,
                                          double prePowStartSec = 8.8e-6,
                                          double prePowStopSec = 15.2e-6);

//! Perform 802.11 a/g Analysis on current capture
/*!
 * \param[in] numSegmsToAnalyze number of segements to analyze
 * \param[in] ph_corr_mode Phase Correction Mode with the following valid options:
 *         - 1: Phase correction off
 *         - 2: Symbol-by-symbol correction (Default)
 *         - 3: Moving avg. correction (10 symbols)
 * \param[in] ch_estimate Channel Estimate with the following options:
 *         - 1: Raw Channel Estimate (based on long trainling symbols) (Default)
 *         - 2: 2nd Order Polyfit
 *         - 3: Full packet estimate
 * \param[in] sym_tim_corr Symbol Timing Correction with the following options:
 *         - 1: Symbol Timing Correction Off
 *         - 2: Symbol Timing Correction ON (Default)
 * \param[in] freq_sync Frequency Sync. Mode with the following options:
 *         - 1: Short Training Symbol
 *         - 2: Long Training Symbol (Default)
 *         - 3: Full Data Packet
 * \param[in] ampl_track Amplitude Tracking with the following options:
 *         - 1: Amplitude tracking off (Default)
 *         - 2: Amplitude tracking on
 * \param[in] prePowStartSec indicates the start point of preamble for power measurement result "powerPreambleDbm" (IQxel Only):
          - 8.8e-6: 8.8 micro second (Default)
 * \param[in] prePowStopSec indicates the end point of preamble for power measurement result "powerPreambleDbm" (IQxel Only):
 *        - 15.2e-6: 15.2 micro second (Default)
 *
 * \return ERR_OK if the 802.11 a/g analysis succeeded; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_Segms_Analyze80211ag(int numSegmsToAnalyze, int ph_corr_mode=2, int ch_estimate=1, int sym_tim_corr=2, int freq_sync=2, int ampl_track=1, double prePowStartSec = 8.8e-6, double prePowStopSec = 15.2e-6);


//! Perform 802.11 p Analysis on current capture
/*!
 * \param[in] numSegmsToAnalyze number of segements to analyze
 * \param[in] ph_corr_mode Phase Correction Mode with the following valid options:
 *         - 1: Phase correction off
 *         - 2: Symbol-by-symbol correction (Default)
 *         - 3: Moving avg. correction (10 symbols)
 * \param[in] ch_estimate Channel Estimate with the following options:
 *         - 1: Raw Channel Estimate (based on long trainling symbols) (Default)
 *         - 2: 2nd Order Polyfit
 *         - 3: Full packet estimate
 * \param[in] sym_tim_corr Symbol Timing Correction with the following options:
 *         - 1: Symbol Timing Correction Off
 *         - 2: Symbol Timing Correction ON (Default)
 * \param[in] freq_sync Frequency Sync. Mode with the following options:
 *         - 1: Short Training Symbol
 *         - 2: Long Training Symbol (Default)
 *         - 3: Full Data Packet
 * \param[in] ampl_track Amplitude Tracking with the following options:
 *         - 1: Amplitude tracking off (Default)
 *         - 2: Amplitude tracking on
 * \param[in] prePowStartSec indicates the start point of preamble for power measurement result "powerPreambleDbm" (IQxel Only):
          - 8.8e-6: 8.8 micro second (Default)
 * \param[in] prePowStopSec indicates the end point of preamble for power measurement result "powerPreambleDbm" (IQxel Only):
 *        - 15.2e-6: 15.2 micro second (Default)
 *
 * \return ERR_OK if the 802.11 p analysis succeeded; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_Segms_Analyze80211p(int numSegmsToAnalyze, int ph_corr_mode=2, int ch_estimate=1, int sym_tim_corr=2, int freq_sync=2, int ampl_track=1, double prePowStartSec = 8.8e-6, double prePowStopSec = 15.2e-6);

//! Perform 802.11 n Analysis on current capture
/*!
 * \param[in] numSegmsToAnalyze number of segements to analyze
 * \param[in] type only "EWC" is supported
 * \param[in] mode Valid options are:
 *         - "nxn": for true 802.11n capture
 *         - "composite: for a composite capture
 *         - "evms": for EVM per signal analysis
 *         - "sequential_mimo": for a switched capture
 * \param[in] enablePhaseCorr Phase Correction Mode:
 *         - 0: Disable
 *         - 1: Enable (Default)
 * \param[in] enableSymTimingCorr Symbol Timing Correction:
 *         - 0: Disable
 *         - 1: Enable (Default)
 * \param[in] enableAmplitudeTracking Amplitude Tracking:
 *         - 0: Disable (Default)
 *         - 1: Enable
 * \param[in] decodePSDU Decode PSDU:
 *         - 0: Disable 
 *         - 1: Enable (Default)
 * \param[in] enableFullPacketChannelEst Channel Estimate on full packet:
 *         - 0: Disable (Default)
 *         - 1: Enable
 * \param[in] referenceFile PSDU reference file (needed for Composite EVM)
 * \param[in] packetFormat indicates format of the 80211n MIMO packet:
 *         - 0: auto-detect mode (Default)
 *         - 1: mixed format
 *         - 2: greenfield format
 * \param[in] frequency Correction options 
 *         - 2 = frequency correction on short and long legacy training fields (default)
 *         - 3 = frequency correction based on full packet
 *		   - 4 = frequency correction on signal fields (legacy and HT) in addition to short and long training fields
 * \param[in] prePowStartSec indicates the start point of preamble for power measurement result "powerPreambleDbm" (IQxel Only):
           - 8.8e-6: 8.8 micro second (Default)
 * \param[in] prePowStopSec indicates the end point of preamble for power measurement result "powerPreambleDbm" (IQxel Only):
 *         - 15.2e-6: 15.2 micro second (Default)
 *
 * \return ERR_OK if the 802.11n analysis succeeded; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_Segms_Analyze80211n(int numSegmsToAnalyze, char *type, char *mode, 
                                         int enablePhaseCorr            = 1,
                                         int enableSymTimingCorr        = 1,
                                         int enableAmplitudeTracking    = 0,
                                         int decodePSDU                 = 1,
                                         int enableFullPacketChannelEst = 0,
                                         char *referenceFile=NULL,
                                         int packetFormat = 0,
                                         int frequencyCorr = 2,
                                         double prePowStartSec = 8.8e-6,
                                         double prePowStopSec = 15.2e-6);

//! Perform 802.11b Analysis on current capture
/*!
 * \param[in] numSegmsToAnalyze number of segements to analyze
 * \param[in] eq_taps Number of equalizer taps, valid options are:
 *         - 1: Equalizer Off	(Default)
 *         - 5: 5 Taps Equalizer
 *         - 7: 7 Taps Equalizer
 *         - 9: 9 Taps Equalizer
 * \param[in] DCremove11b_flag DC removal, valid options are:
 *         - 0: DC Removal OFF (Default)
 *         - 1: DC Removal ON
 * \param[in] method_11b 802.11b method, valid options are:
 *         - 1: Use 11b standard TX accuracy (Default)
 *         - 2: Use 11b RMS error vector
  *        - 3: Use 11b Std 2007 TX accuracy
 * \param[in] prePowStartSec indicates the start point of preamble for power measurement result "powerPreambleDbm" (IQxel Only):
          - 8.8e-6: 8.8 micro second (Default)
 * \param[in] prePowStopSec indicates the end point of preamble for power measurement result "powerPreambleDbm" (IQxel Only):
 *        - 15.2e-6: 15.2 micro second (Default)
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_Segms_Analyze80211b(int numSegmsToAnalyze, int eq_taps = 1,
                                         int DCremove11b_flag = 0,
                                         int method_11b       = 1,
                                         double prePowStartSec = 8.8e-6,
                                         double prePowStopSec = 15.2e-6);

//! Perform power Analysis on current capture
/*!
 * \param[in] numSegmsToAnalyze number of segements to analyze
 * \param[in] T_interval Gap between packets (bursts).  Ignored
 * \param[in] max_pow_diff_dB Power difference between noise floor and packets.  Ignored
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_Segms_AnalyzePower(int numSegmsToAnalyze, double T_interval = 0.0, double max_pow_diff_dB = 0.0);

//! Perform Generic FFT Analysis on current capture
/*!
 * \param[in] numSegmsToAnalyze number of segements to analyze
 * \param[in] NFFT Number of FFT.  Default to 0
 * \param[in] res_bw Resolution bandwidth.  Default to 100KHz
 * \param[in] window_type FFT window type.  Default to NULL
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_Segms_AnalyzeFFT(int numSegmsToAnalyze, double NFFT=0, double res_bw=100e3, char *window_type=NULL);

//! Perform HT40 mask (120MHz) analysis
/*!
 * \param[in] numSegmsToAnalyze number of segements to analyze
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 * \remark Before calling LP_AnalyzeHT40Mask(), modeHT40 needs to set to 1 in LP_VsaDataCapture().
 */
IQMEASURE_API int LP_Segms_AnalyzeHT40Mask(int numSegmsToAnalyze);

//! Perform VHT80 mask (240MHz) analysis
/*!
 * \param[in] numSegmsToAnalyze number of segements to analyze
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 * \remark Before calling LP_AnalyzeVHT80Mask(), modeHT40 needs to set to 6 in LP_VsaDataCapture().
 */
IQMEASURE_API int LP_Segms_AnalyzeVHT80Mask(int numSegmsToAnalyze);

//! Perform Generic FFT Analysis on current capture
/*!
 * \param[in] numSegmsToAnalyze number of segements to analyze
 * \param[in] technology type, specify the which technology standard limit to be used;
 *		   - 0  11ac (auto detect 20/40 Mhz bandwidth; Since 11ac is not defined in 2.4G band, will use 11n limit in 2.4G band)
 *		   - 1  11n	(auto detect 20/40 Mhz bandwidth and 2.4G/5G band; )
 *		   - 2  11ag	
 *		   - 3  11b
 *		   - 4  11ac 80MHz and 160MHz
 *		   - 99 Auto detect packet format (use when technology is unknown. Using raw data for result calculation is recommended.)
 * \param[in] NFFT Number of FFT.  Default to 0
 * \param[in] res_bw Resolution bandwidth.  Default to 100KHz
 * \param[in] window_type FFT window type.  Default to NULL
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_Segms_AnalyzeTechnologyFFT(int numSegmsToAnalyze, int technology, double NFFT=0, double res_bw=100e3, char *window_type=NULL);

//! Perform Bluetooth Analysis on current capture
/*!
 * \param[in] numSegmsToAnalyze number of segements to analyze
 * \param[in] data_rate, Bluetooth data rate, with the following options:
 *         - 1: 1Mbit/s BDR
 *         - 2: 2Mbit/s EDR
 *         - 3: 3Mbit/s EDR
 *         - 0: Auto detect
 * \param[in] analysis_type, Specifies what type of analysis to perform. Default: 'All'. Valid values are as follows:
 *         - PowerOnly
 *         - 20dbBandwidthOnly
 *         - PowerandFreq
 *         - All  (This is the set default value)
 *		   - ACP  (only does the new ACP analysis)
 *		   - AllPlus  (performs all the analyses that are done by ¡®All?plus the ACP analysis)
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_Segms_AnalyzeBluetooth( int numSegmsToAnalyze, double data_rate, char *analysis_type = "All");

//! Perform power Analysis on current capture

/*!
 * \param[in] numSegmsToAnalyze number of segements to analyze
 * \param[in] intervalLength set the interval length of the capture. Default to 0.
 * \param[in] measurementLength sets the measurement length starting at the begining of the interval. Default to 0.
 *
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_Segms_AnalyzeCW(int numSegms,
                                     double intervalLength = 0.0,
                                     double measurementLength = 0.0);

//! Perform FFT Analysis on GPRF signals
/*
 * \param[in] numSegmsToAnalyze number of segements to analyze
 * \param[in] pmode, Configures how multiple FFT frames are processed to obtain final spectrum result. Default AVER mode
 * \param[in] numberofPeaks, default 1
 * \param[in] res_bw Resolution bandwidth. default 100khz
 * \param[in] signal, Configures the signal type to enable adjustment of FFT resolution for spectrum measurement. default CW
 * \param[in] window_type FFT window type. dafault GAUSS
 * \param[in] startOffset, Offset in seconds into the capture data for start of calculation in second. default 0.
 * \param[in] length, Length of calculation in second. default 0.001
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int		LP_Segms_AnalyzeGPRFSpectrum(int numSegmsToAnalyze, GPRF_PMODE pmode = PMODE_AVER, int numberofPeaks = 1, double res_bw = 100e3, GPRF_SIGNAL signal = SIGNAL_CW, GPRF_WINDOW window = WINDOW_GAUSS, double startOffset = 0.0, double length = 0.001);


//! Perform Zigbee Analysis on current capture
/*!
 * \param[in] numSegmsToAnalyze number of segements to analyze
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_Segms_AnalyzeZigbee(int numSegmsToAnalyze);

//! Get a scalar measurement result
/*!
 * \param[in] segment_index segment index for result fetch
 * \param[in] measurement The measurement name.  Please refer to \ref group_scalar_measurement "Scalar Measurements" for all available measurement names
 * \param[in] index The index of the measurement.  In most case, index would be zero.  For MIMO analysis, some measurements, such as EVM, may have more than one results
 *
 * \return The value of the measurement.  -99999.99 (a special defined negative value) will be returned if no measurement available
 */
IQMEASURE_API double LP_Segms_GetScalarMeasurement(int segment_index, char *measurement, int index = 0);

//! Get a vector measurement result
/*!
 * \param[in] segment_index segment index for result fetch
 * \param[in] measurement The measurement name.  Please refer to \ref group_vector_measurement "Vector Measurements" for all available measurement names
 * \param[out] bufferReal Returns the real part of vector
 * \param[out] bufferImag Returns the imag part of vector
 * \param[in] bufferLength Indicates the length of bufferReal and bufferImag (in Bytes)
 *
 * \return The number of elements in bufferReal or/and bufferImag
 */
IQMEASURE_API int LP_Segms_GetVectorMeasurement(int segment_index, char *measurement, double bufferReal[], double bufferImag[], int bufferLength);

//!Get a vector channelEst matrix, returned by scpi, add on 12/09/2013, DTNA only
/*!  
 * \param[in] segment_index segment index for result fetch
 *\param[in] removeCSD remove the CSD. 
 *\param[in] polarDisp get the polar representation. 
 *\param[in] numberPacket number of packets to average over
 *\param[out] bufferReal Returns the real/magnitude part of vector
 *\param[out] bufferImag Returns the imag/angular part of vector
 *\param[in] bufferLength Indicates the length of bufferReal and bufferImag (in Bytes)
 *
 *\return The number of elements in bufferP1 or/and bufferP2
 */
IQMEASURE_API int LP_Segms_GetVectorChannelEst(int segment_index,double bufferP1[], double bufferP2[], int bufferLength, bool removeCSD, bool polarDisp = false, int numberPacket = 1);

//! Saves the captured data to a file (.iqvsa)
/*!
 * \param[in] segment_index segment index for capture save
 * \param[in] sigFileName Specifies the .iqvsa file name
 *
 * \return ERR_OK if successful; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_Segms_SaveVsaSignalFile(int segment_index, char *sigFileName);

//! Performs the chamber calibration and saves the cal data (path loss information) to a file in the tester.
/*!
 * \param[in] txAntenna specifes the dut chain that is radiating the signal.
 * \param[in] txPower Specifies the tx power at which the dut is transmitting the signal. 
 * \param[in] txfreq Specifies the frequency at which the dut is transmitting the signal
 *
 * \return ERR_OK if successful; 
 */
IQMEASURE_API int LP_ChamberCal(int txAntenna, double txPower, int txfreq);

//! Loads the path loss file from the tester based on DUT chain and frequency.
/*!
 * \param[in] txAntenna specifes the dut chain that is radiating the signal.
 * \param[in] txfreq Specifies the frequency at which the dut is transmitting the signal
 * This function will use both these parameters to load the appropriate path loss file to the tester.
 *
 * \return ERR_OK if successful; 
 */
IQMEASURE_API int	LP_ChamberCalLoadPathLoss(int txAntenna, int txFreq);

//! Set APT settings.
/*!
 * \param[in] settings includes APT settings parameters.
 */
IQMEASURE_API int LP_Async_SetAPTSettings(APTSettings *settings);
//! Enable or disable Reference threshold setting
/*!
 * \Do not need to set reference threshould for IQxel-M8 tester with FW 1.8.0 and up. Need to set RTHR with lower FW
 */
IQMEASURE_API int LP_EnableSetRTHR(int enable);

/*! Clears the RFCorrection set on all available modules
 *
 * \return ERR_OK if successful;
 */
IQMEASURE_API int LP_ClearAllRFCorrection();

/*! Inserts the path loss points in a format defined by the user to a table whose name is also defined by the user.
 * \param[in] tableDefineFormat specifes the format of entering the path loss Ex: "Freq,Loss".
 * \param[in] pathLossTableName Specifies the table name.
 * \param[in] channelArray - Array of Channel values
 * \param[in] pathLossArray - Array of PathLossValues
 * \param[in] arraySize - Size of the array (ChannelArray and PathLossArray size should be the same.
 * \return ERR_OK if successful;
 */
IQMEASURE_API int LP_InsertPathLossTablePoints(const char *tableDefineFormat,
                                               const char *pathLossTableName,
                                               double *channelArray,
                                               double *pathLossArray,
                                               int arraySize);

/*! Applies the RF correction. Currently supports only RF1
 * \param[in] pathLossTableName Specifies which table name you want to use while applying the RFC*
 * \param[in] port The port to which the VSA/VSG module connects, with the following options:
 *				- =1: OFF
 *				- =2: RF port 1 (RF1)
 *				- =3: RF port 2 (RF2)
 *				- =4: RF port 3 (RF3)
 *				- =5: RF port 4 (RF4)
  * \param[in] moduleIndex The VSA/VSG module to apply the pathloss table to, with the following options:
   *			  =1: VSA/VSG11
 *				- =2: VSA/VSG12
 *				- =3: VSA/VSG21
 *				- =4: VSA/VSG22
 * \param[in] optionVsaVsg - 1:Apply the RF correction to VSA, 2: Apply the RF correction to VSG 3: Apply the RF
 *Correction to both VSA/VSG.
 * \return ERR_OK if successful;
 */
IQMEASURE_API int LP_ApplyRFCorrection(char *pathLossTableName, int port, int moduleIndex,
                                       int optionVsaVsg);

//**************************************************************************//
//                              ASYNC END                                   //
//**************************************************************************//

//! Sync to wait for number of sessions.
//! Sessions call this function with same syncId will join and wait each other, until the specific number of sessions already join.
//! Note: This function works only on firmware 1.12.1 or later for IQxel80/280, and firmware 1.6.0 or later for IQxel-M.
/*!
 * \param[in] syncId. Maximum 2 for IQxel80/280 and maximum 4 for IQxel-M. 
 * \param[in] waitForNumSessions Number of sessions to sync.
 * \param[in] timeoutSecs timeout seconds.
 * \param[out] yourSequence your join sequence.
 * \param[in] masterCB The callback function for the 1st join session.
 * \param[in] dataToCB the data passed the masterCB.
 */
IQMEASURE_API int LP_SyncWaitForNumSessions(unsigned int syncId, unsigned int waitForNumSessions, unsigned int timeoutSecs, 
                                            unsigned int * yourSequence = NULL, void (*masterCB)(void *) = NULL, void *dataToCB = NULL);

//**************************************************************************//
//                              NFC START                                   //
//**************************************************************************//

//! Perform NFC frequency sweep calibration if using Litepoint testhead
/*!
 * \param[in] freqStart, Starting frquency in Hz
 * \param[in] freqStop, Stop frequency in Hz
 * \param[in] freqStep, Step size of frequency sweep in Hz
 * \param[in] sweepLevel, Sweep power level with unit "dBV"
 * \param[in] autoSweepTime, Whether to automatically determine the sweep time by the tester, 1 means true and 0 means false
 * \param[in] sweepTime, Sweep time in ms input if autoSweepTime is set to 0
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_NFC_FreqSweepCal( double freqStart = 10e6, double freqStop = 20e6, double freqStep = 10e3, double sweepLevel = -12, unsigned int autoSweepTime = 1, double sweepTime = 90.16 );

//! Save NFC frequency sweep calibration reference file
/*!
 * \param[in] FileName, saved reference file name
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_NFC_SaveCalRefFile(char *FileName);

//! Load NFC frequency sweep calibration reference file and apply it for calibration
/*!
 * \param[in] FileName, reference file name to be loaded
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_NFC_LoadCalRefFile(char *FileName);

//! Perform frequency sweep measurement and carry out the corresponding analysis, freqStart, freqStop, and freqStep must be the same as for the calibration
/*!
 * \param[in] freqStart, Starting frquency in Hz
 * \param[in] freqStop, Stop frequency in Hz
 * \param[in] freqStep, Step size of frequency sweep in Hz
 * \param[in] sweepLevel, Sweep power level with unit "dBV"
 * \param[in] autoSweepTime, Whether to automatically determine the sweep time by the tester, 1 means true and 0 means false
 * \param[in] sweepTime, Sweep time in ms input if autoSweepTime is set to 0
 * \param[in] sweepConnection, Sets frequency sweep test connection type
 *         - 	0: Coupled measurement via LitePoint test coil through over-the-air coupling
 *		   -	1: Conducted measurement where DUT is directly connected to the TR port. Only supported after FW1.5.0!!
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_NFC_FreqSweep( double freqStart = 10e6, double freqStop = 20e6, double freqStep = 10e3, double sweepLevel = -12, unsigned int autoSweepTime = 1, double sweepTime = 90.16, unsigned int sweepConnection = 0 );

//! Check the coil type that is connected to IQnfc
/*!
 * \param[out] coilType, options are as the following
 *         - 	0: NONLP coil
 *		   -	1: LP coil
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_NFC_CheckCoilType(unsigned int *coilType);

//! Setup IQnfc for Initiator waveform analysis, IQnfc works as a target
/*!
 * \param[in] standard, NFC standard with the following options:
 *         - 	NFCA: PHY same as ISO14443A
 *		   -	NFCB: PHY same as ISO14443B
 *		   -	NFCF: Felica
 *		   -	AACT: ISO18092 active peer to peer mode 106kbps
 *		   -	FACT: ISO18092 active peer to peer mode high data rate
 *		   -	APAS: ISO18092 and NFC Forum passive 106kbps peer to peer mode with PHY identical to NFCA
 *		   -	FPAS: ISO18092 and NFC Forum passive high data rate peer to peer mode with PHY identical to NFCF
 * \param[in] dataRate, data rate with the following options:
 *		   - 	R106: 106 kbps
 *		   -	R212: 212 kbps
 *		   -	R424: 424 kbps
 * \param[in] loadModAtten, Sets load modulation output level attenuation relative to max scale
 * \param[in] loadModAttenType, Sets configuration type of load modulation output level
 *		   - 	0: Percent of modulation
 *		   -	1: Attenuation in dB
 * \param[in] analyzerUnit, Sets the unit for analyzer input level, the options are as follows:
 *		   -	0: A/m, only used for Litepoint test head
 *		   -	1: Volt, support both Litepoint and non-Litepoint test head
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_NFC_Setup_InitiatorWaveformAnalysis( NFC_STANDARD standard, NFC_DATARATE dataRate, double loadModAtten = 10, unsigned int loadModAttenType = 0, unsigned int analyzerUnit = 0);

//! Setup IQnfc for Target waveform analysis, IQnfc works as an initiator
/*!
 * \param[in] standard, NFC standard with the following options:
 *         - 	NFCA: PHY same as ISO14443A
 *		   -	NFCB: PHY same as ISO14443B
 *		   -	NFCF: Felica
 *		   -	AACT: ISO18092 active peer to peer mode 106kbps
 *		   -	FACT: ISO18092 active peer to peer mode high data rate
 *		   -	APAS: ISO18092 and NFC Forum passive 106kbps peer to peer mode with PHY identical to NFCA
 *		   -	FPAS: ISO18092 and NFC Forum passive high data rate peer to peer mode with PHY identical to NFCF
 * \param[in] dataRate, data rate with the following options:
 *		   - 	R106: 106 kbps
 *		   -	R212: 212 kbps
 *		   -	R424: 424 kbps
 * \param[in] outputLevel, Sets the transceiver output level, with unit of either dBV or A/m
 * \param[in] TROutUnit, Sets the transceiver output level unit with the following two options:
 *		   -	0: A/m, only used for Litepoint test head
 *		   -	1: dBV, support both Litepoint and non-Litepoint test head
 * \param[in] freqOffset, Sets the frequency offset in Hz for NFC transeiver output
 * \param[in] pulseLength, Sets length of the modulation pulse in us.If pulse length equals to 0, then the pulse length is governed by the protocol selection. System would set default pulse length by different standards:
 *			For NFCA, default pulse length = 2.36us.
 *			For NFCB, default pulse length = 0.
 *			For NFCF, default pulse length = 0.
 *			For NFCP2P, default pulse length = 2.36us.
 * \param[in] modDepth, Sets the NFC transceiver modulation depth, the options are as follows:
 *		   -	AUTO:	Set default modulation depth by different standards: For NFCA, default modulation depth = M100; For NFCB, default modulation depth = M22;
						For NFCF, default modulation depth = M22; For NFCP2P, default modulation depth = M100 .
 *		   -	M7:		Amplitude Shift Keying(ASK) 7%
 *		   -	M8P5:	Amplitude Shift Keying(ASK) 8.5%
 *		   -	M10:	Amplitude Shift Keying(ASK) 10%
 *		   -	M13:	Amplitude Shift Keying(ASK) 13%
 *		   -	M16:	Amplitude Shift Keying(ASK) 16%
 *		   -	M22:	Amplitude Shift Keying(ASK) 22%
 *		   -	M30:	Amplitude Shift Keying(ASK) 30%
 *		   -	M100:	On-Off Keying(OOK) 100%
 * \param[in] analyzerUnit, Sets the unit for analyzer input level, the options are as follows:
 *		   -	0: A/m, only used for Litepoint test head
 *		   -	1: Volt, support both Litepoint and non-Litepoint test head
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_NFC_Setup_TargetWaveformAnalysis( NFC_STANDARD standard, NFC_DATARATE dataRate, double outputLevel, unsigned int TROutUnit = 0, double freqOffset = 0, double pulseLength = 0, NFC_MOD_DEPTH modDepth = AUTO, unsigned int analyzerUnit = 0 );

//IQMEASURE_API int LP_NFC_Setup_P2PWaveformAnalysis( NFC_DUTMODE dutMode, NFC_STANDARD standard, NFC_DATARATE dataRate, unsigned int antennaType, double zDistance, double outputLevel, double freqOffset, double pulseLength, NFC_MOD_DEPTH modDepth );

//! Carry out capture following several initial protocol steps
/*!
 * \param[in] captureLength, Sets the capture length in seconds
 * \param[in] resonatorFreq, Sets NFC antenna coil resonator with the following options:
 *		   - 		RES_OFF,
 *		   - 		R13M56,
 *		   - 		R16M1,
 *		   - 		R15M
 * \param[in] resonatorQfac, Sets quality factor of test head with the following options:
 *		   - 		0: Low
 *		   - 		1: High
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_NFC_InitProtocolCapture( double captureLength, NFC_RES_FREQ resonatorFreq , unsigned int resonatorQfac = 1 );

//! Arms the VSA capture trigger
/*!
 * \param[in] captureLength, Sets the capture length in seconds
 * \param[in] resonatorFreq, Sets NFC antenna coil resonator with the following options:
 *		   - 		RES_OFF,
 *		   - 		R13M56,
 *		   - 		R16M1,
 *		   - 		R15M
 * \param[in] resonatorQfac, Sets quality factor of test head with the following options:
 *		   - 		0: Low
 *		   - 		1: High
 * \param[in] triggerSource, Sets the trigger source with the following options:
 *		   - 	IMM:	Triggers when armed
 *		   - 	VID:	Triggers when an RF signal transition of the configured slope, level, and gap time occurs at baseband
 *		   - 	TRXS:	Triggers when nfc controller received an rx interrupt
 *		   - 	TTXD:	Triggers when nfc controller received an tx interrupt
 * \param[in] triggerLevel, Sets the trigger threshold level relative to the reference level.
 * \param[in] triggerGap, Sets the length of time a signal must be above or below the trigger threshold before arming for the next trigger event.
 * \param[in] triggerOffset, Sets the trigger offset time.
 * \param[in] triggerTimeout, Sets the trigger timeout.
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_NFC_InitCapture( double captureLength, NFC_RES_FREQ resonatorFreq, unsigned int resonatorQfac = 1, NFC_TRIG_SOURCE triggerSource = VID, double triggerLevel = -25, double triggerGap = 6, double triggerOffset = -10, double triggerTimeout = 1500 );

//! Performs waveform quality analysis
/*!
 * \param[in] dutMode, the DUT mode for waveform analysis:
 *         - 	TARGET
 *         -	INITIATOR
 * \param[in] standard, NFC standard with the following options:
 *         - 	NFCA: PHY same as ISO14443A
 *		   -	NFCB: PHY same as ISO14443B
 *		   -	NFCF: Felica
 *		   -	AACT: ISO18092 active peer to peer mode 106kbps
 *		   -	FACT: ISO18092 active peer to peer mode high data rate
 *		   -	APAS: ISO18092 and NFC Forum passive 106kbps peer to peer mode with PHY identical to NFCA
 *		   -	FPAS: ISO18092 and NFC Forum passive high data rate peer to peer mode with PHY identical to NFCF
 * \param[in] dataRate, data rate with the following options:
 *		   - 	R106: 106 kbps
 *		   -	R212: 212 kbps
 *		   -	R424: 424 kbps
 * \param[in] fieldDetectGap, Sets the field detection gap time requirement for acquisition in the current active instrument channel. An active field (CW burst) may contain one or multiple NFC modulated signals from initiator (PCD) or target (PICC)
 * \param[in] fieldDetectThreshold, Sets the log-scale field strength difference requirement for acquisition in the current active instrument channel. The log-scale field strength is calculated as 20log10(field strength) in dB-A/m.
 * \param[in] modDepthThreshold, Sets modulation depth threshold for NFC signal acquisition in the current active instrument channel
 * \param[in] loadModDataRateSearchRange, Sets the search range (uncertainty) of load modulation data rate from the nominal data rate. If uncertainty is 0, then a nominal load modulation data rate is detected and reported. Otherwise, the actual load modulation data rate is measured
 * \param[in] loadModFilterType, Sets load modulation measurement filter type with the following options:
 *		   - 	NONE,
 *		   - 	BW1M,
 *		   - 	BW5M
 * \param[in] numOfSkippedSignal, Offset in NFC modulated signals into the capture data for the start of the calculation
 * \param[in] numOfSkippedFieldStrength, Offset in active fields (CW bursts) into the capture data for the start of the calculation
 * \param[in] numOfCalcSignal, Number of NFC modulated signals to calculate
 * \param[in] numOfCalcFieldStrength, Number of active fields (CW bursts) to calculate
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_NFC_AnalyzeSignal( NFC_DUTMODE dutMode, NFC_STANDARD standard, double fieldDetectGap = 0.000032, double fieldDetectThreshold = 15, double modDepthThreshold = 0.5, double loadModDataRateSearchRange = 0, NFC_LMOD_FILTER loadModFilterType = BW1M, int numOfSkippedSignal = 0, int numOfSkippedFieldStrength = 0, int numOfCalcSignal = 1, int numOfCalcFieldStrength = 1 );

//! Set NFC transceiver command type, if user defined bits set bits properties. Supported only on FW1.6.0 and above!!
/*!
 * \param[in] commType_A, Sets the NFC transceiver command type for NFCA or P2P active A type or P2P passive A type with the following options:
 *		   - 		REQA,
 *		   - 		WUPA,
 *		   - 		ATRREQ_A,
 *		   - 		ATQA,
 *		   - 		ATRRES_A,
 *		   - 		USER_A
 * \param[in] commType_B, Sets the NFC transceiver command type for NFCB with the following options:
 *		   - 		REQB,
 *		   - 		ATQB,
 *		   - 		USER_B
 * \param[in] commType_F, Sets the NFC transceiver command type for NFCF or P2P active F type or P2P passive F type with the following options:
 *		   - 		POLLREQ,
 *		   - 		ATRREQ_F,
 *		   - 		POLLRES,
 *		   - 		ATRRES_F,
 *		   - 		USER_F
 * \param[in] userBitsCRC, Sets the CRC state in the user specified bits. 1 indicates user specified bits contain CRC when applicable. 0 indicates otherwise.
 * \param[in] userBitsParity, Sets the parity bit state in the user specified bits. 1 indicates user specified bits contain parity for applicable standards. 0 indicates otherwise.
 * \param[in] userBitsStartStop, Sets the start and stop bit state in the user specified bits. 1 indicates user specified bits contain start and stop bit for applicable standards. 0 indicates otherwise.
 * \param[in] userBits, Sets the user specified bits for transmission. Input must be binary bits in transmission order. Parity, start, stop and CRC bits may be included when applicable. For F type, either NFCF or P2P F type, payload excludes SYNC and length fields. For P2P A type, payload excludes start byte (F0) and length fields. Start of frame and end of frame are added internally.
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_NFC_SetTRxCommandType( NFCA_COMMAND_TYPE commType_A = REQA, NFCB_COMMAND_TYPE commType_B = REQB, NFCF_COMMAND_TYPE commType_F = POLLREQ, unsigned int userBitsCRC = 0, unsigned int userBitsParity = 0, unsigned int userBitsStartStop = 0, char *userBits = NULL );

//! Set the IQnfc+ to perform power transfer measurement.
/*!
 * \param[in] standard, NFC standard with the following options:
 *         - 	NFCA: PHY same as ISO14443A
 *		   -	NFCB: PHY same as ISO14443B
 * \param[in] outputLevel, Sets the transceiver output level, with unit of dBV.
 * \param[in] freqOffset, Sets the frequency offset in Hz for NFC transeiver output.
 * \param[in] captureLength, Sets the capture length in seconds.
 * \param[in] analyzeOffset, Offset in seconds into the capture data for start of calculation.
 * \param[in] analyzeLength, Length of calculation in seconds.
 * \param[out] resultPTR, Returns calculated DC mean value for PCD to PICC power transfer measurement.
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_NFC_PowerTransferMeasurement( NFC_STANDARD standard, double outputLevel, double freqOffset, double captureLength, double analyzeOffset, double analyzeLength, double *resultPTR = NULL );


//**************************************************************************//
//                              NFC END                                     //
//**************************************************************************//

//*****************************************************************************//
//                              IQlink START                                   //
//*****************************************************************************//

//! Associate wifi network on DUT side 
/*!
 * \param[in] controlIpAddress, IP address of the control device
 * \param[in] wifiIpAddress, wifi IP address of DUT. If assocaiteionMode = LINK_ASSO_AUTO, return wifi IP address. If assocaitionMode = LINK_ASSO_USER, use it as input to connect
 * \param[in] initAttenuation, set attenuation for association. Default 30dB
 * \param[in] initPower, set power value for association. Default -35dBm
 * \param[in] associationMode, Default: LINK_ASSO_AUTO
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_IQLINK_Associate(char *controlIpAddress, char *wifiIpAddress, double initAttenuation=50, double initPower=-35, IQLINK_ASSOCIATION_MODE associationMode=LINK_ASSO_AUTO);

//! Disassociate wifi network 
/*!
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_IQLINK_Disassociate();

//! Connect to Socket Server which is in charge of DUT control for throughput testing
/*!
 * \param[in] controlIpAddress, IP address of the control device
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_IQLINK_Connect_Socket_Server(char *controlIpAddress);

//! TCP Setting
/*!
 * \param[in] rfPort1Enabled, enable/disable RF1 port. 1:ON, 0:OFF
 * \param[in] rfPort2Enabled, enable/disable RF2 port. 1:ON, 0:OFF
 * \param[in] rfPort3Enabled, enable/disable RF3 port. 1:ON, 0:OFF
 * \param[in] useMapi, Use MAPI TFC
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_IQLINK_EnableRfPort(int rfPort1Enabled=1, int rfPort2Enabled=1, int rfPort3Enabled=1, bool useMapi = false);

//! TCP Setting
/*!
 * \param[in] bufferLength, buffer size in KBytes for TCP
 * \param[in] tcpWindowSize, window size in KBytes for TCP
 * \param[in] maxSegmSize, maximum segment size in KBytes for TCP
 * \param[in] port for TCP communication
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_IQLINK_SetTcp(double bufferLength, double tcpWindowSize, double maxSegmSize, int port = 5001);

//! UCP Settings
/*!
 * \param[in] udpBandwidth, bandwidth in Mbits/sec for UDP
 * \param[in] udpBufferSize, buffer size in KBytes for UDP
 * \param[in] udpPaketSize, packet size in Bytes for UDP
 * \param[in] port for UDP communication
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_IQLINK_SetUdp(double udpBandwidth, double udpBufferSize, int udpPaketSize, int port = 5001);

//! AP Settings
/*!
 * \param[in] apBand, 2G4 or 5G
 * \param[in] apMode, define standard for measurement
 * \param[in] apBandwidth, define bandwidth for measurement
 * \param[in] apChannel, define channel for measurement
 * \param[in] apMcsIndex, define data rate for measurement. Use DATA_RATE_AUTO or 255 for Auto datarate.
 * \param[in] apNumStream, define number of stream for measurement.
 * \param[in] apEndpointPower, define endpoint power for measurement.
 * \param[in] encryptionOn, turn on the encryption. Default off.
 * \param[in] encryptionType, define the encryption type if encryption is on. Option: WEP, WPA2. Default WPA2.
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_IQLINK_SetAp(IQLINK_BAND apBand, IQLINK_TECH_MODE apMode, IQLINK_BANDWIDTH apBandwidth, int apChannel, int apMcsIndex, int apNumStream, double apEndpointPower=15, bool encryptionOn = false, IQLINK_ENCRYPT_TYPE encryptionType = WPA2, double triggerLevel = -15, bool aggregateMpduOn = true, IQLINK_HT_PPDU_FORMAT htGreenfield = LINK_AP_MIXED, IQLINK_GUARD_INTERVAL guardInterval = LINK_GI_LONG, IQLINK_OFDM_CODING ofdmCoding = LINK_BCC, double attenuation = 50.0, bool beaconFixedRate = false);

//! Uplink Throughput Begin
/*!
 * \param[in] testMode, set test mode for AP or Client
 * \param[in] uplinkTpAttenuation, attenuation for uplink throughput test 
 * \param[in] uplinkTimeDuration, time duration in second of the throughput test
 * \param[in] timeSkip, amount of time in second to skip at begining
 * \param[in] enableTxPwr, to enable TX power measurement for Client Uplink test
 * \param[in] dgenMode, to select mode for dgen. IPERF2 or IPERF3
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_IQLINK_Uplink_Throughput_Run(IQLINK_TEST_MODE testMode, double uplinkTpAttenuation, int uplinkTimeDuration=4, int timeSkip=0, int enableTxPwr=1, IQLINK_DGEN_MODE dgenMode=LINK_IPERF3);

//! Downlink Throughput Begin
/*!
 * \param[in] testMode, set test mode for AP or Client
 * \param[in] downlinkTpAttenuation, attenuation for downlink throughput test 
 * \param[in] downlinkTimeDuration, time duration in second of the throughput test
 * \param[in] timeSkip, amount of time in second to skip at begining
 * \param[in] enableTxPwr, to enable TX power measurement for AP downlink test
 * \param[in] dgenMode, to select mode for dgen. IPERF2 or IPERF3
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_IQLINK_Downlink_Throughput_Run(IQLINK_TEST_MODE testMode, double downlinkTpAttenuation, int downlinkTimeDuration=4, int timeSkip=0, int enableTxPwr=1, IQLINK_DGEN_MODE dgenMode=LINK_IPERF3);

//! Tx Power Measurement Begin
/*!
 * \param[in] attenuationLevel, tx power attenuation level in dB.
 * \param[in] testTimeDuration, set time duration in second for tx power test. 
 * \param[in] psduLength, set PSDU length in Bytes.
 * \param[in] dgenMode, set DGEN mode for TX POWER test. Currently only FPING supported.
 * \param[in] triggerLevelRf1, set trigger level at RF port 1.
 * \param[in] triggerLevelRf2, set trigger level at RF port 2.
 * \param[in] triggerLevelRf3, set trigger level at RF port 3.
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_IQLINK_Tx_Power_Run(double attenuationLevel, double testTimeDuration=5, int psduLength=1090, IQLINK_DGEN_MODE dgenMode=LINK_FPING, double triggerLevelRf1=-15, double triggerLevelRf2=-15, double triggerLevelRf3=-15, bool useMapi = false);

//! Tx Power Measurement Begin using MAPI
/*!
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_IQLINK_Tx_Power_Mapi_Run();

//! LP_IQLINK_Rx_Per_Init
/*!
 * \Need to call this before LP_IQLINK_PER_Start()
 * \ param[in] dgenMode, set DGEN mode for RX PER test. Option: LINK_IPERF3, LINK_FPING.
 * \ param[in] packetCount, set packet counts for RX PER test.
 * \ param[in] psduLength, set PSDU length for RX PER test.
 * \ param[in] triggerLevel, set trigger level for RX PER test.
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_IQLINK_Rx_Per_Init(int packetCount, int psduLength, double triggerLevel = -15.0, IQLINK_DATA_GENERATOR_MODE dgenMode=DATA_GENERATOR_MODE1, double maxDuration = 7.0);

//! IQlink PER Begin
/*!
 * \param[in] outputPower, output power for PER test
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_IQLINK_Rx_Per_Start(double outputPower);

//! IQlink PER Begin
/*!
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_IQLINK_Rx_Fast_Per_Start();

//! IQlink MAPI version of RX Start Begin
/*!
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_IQLINK_Rx_Mapi_Start();

//! IQlink MAPI version of RX Start Begin
/*!
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_IQLINK_Rx_Sweep_Auto_Sense_Start();

//! IQlink Rx PER Stop
/*!
 * \Stop PER testing. Need to call this after LP_IQLINK_PER_Start()
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_IQLINK_Rx_Per_Stop();

//! IQlink RXPER Ack Parameters
/*!
 * \param[in] minAckRespTime, minimum Ack Response Time
 * \param[in] maxAckRespTime, maximum Ack Response Time
 * \param[in] minAckWidthTime, minimum Ack Width Time
 * \param[in] maxAckWidthTime, maximum Ack Width Time
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_IQLINK_OverwriteRxPerSettings(int minAckRespTime, int maxAckRespTime, int minAckWidthTime, int maxAckWidthTime);

//! IQlink FAST PER (DSENS) Parameters
/*!
 * \param[in] txPacketCountThreshold, transmit packets count threshold
 * \param[in] packetCount, packets count for PER testing
 * \param[in] psduLength, PSDU length
 * \param[in] maxDuration, maximum duration during PER test
 * \param[in] estimatedPower, estimated starting power for direct sensitivity sweep
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_IQLINK_SetFastPer(int txPacketCountThreshold, int packetCount, int psduLength, double maxDuration, double estimatedPower);

//! IQlink MAPI RX (DSENS) Parameters
/*!
 * \param[in] txPacketCountThreshold, transmit packets count threshold
 * \param[in] packetCount, packets count for PER testing
 * \param[in] ackWidthMinUs, minimum width of Ack signal
 * \param[in] ackWidthMaxUs, maximum width of Ack signal
 * \param[in] estimatedPower, estimated starting power for direct sensitivity sweep
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_IQLINK_SetRxMapi(IQLINK_BAND apBand, IQLINK_TECH_MODE apMode, IQLINK_BANDWIDTH apBandwidth, int apChannel, char* dataRate, int apNumStream, char* mapiAssocMode,	int rf1, int rf2, int rf3,
									  int txPacketCountThreshold, int packetCount, double ackWidthMinUs, double ackWidthMaxUs, int autoSense, double estimatedPower, double sweepPowerStop = 0.0, double sweepPowerStep = 0.0);

//! IQlink MAPI TX (Power) Parameters
/*!
 * \param[in] txPacketCountThreshold, transmit packets count threshold
 * \param[in] txpTime, TX power measuring time
 * \param[in] txpInterval, TX power measuring interval
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_IQLINK_SetTxMapi(IQLINK_BAND apBand, IQLINK_TECH_MODE apMode, IQLINK_BANDWIDTH apBandwidth, int apChannel, char* expectedDataRate, int apNumStream, char* mapiAssocMode,	int rf1, int rf2, int rf3,
									  double sweepStart, double sweepStop, double sweepStep, int autoSense, int txPacketCountThreshold = 100, double txpTime = 5.0, double txpInterval = 0.001);

//! IQlink MAPI TX (Power) Parameters
/*!
 * \param[out] dutMacAddr, DUT Mac Address
 * \param[out] dutWifiIp, DUT Wifi IP
 */
IQMEASURE_API int LP_IQLINK_Get_DUT_Info_Mapi(char* dutMacAddr, char* dutWifiIp);

//! IQlink PER LP_IQLINK_SetPathLossTable
/*!
 * \param[in] rfPort, choose RF port to set path loss
 * \param[in] freqList, list of frequency in MHz for path loss
 * \param[in] pathLossList, list of pathloss in dB. This should be corresponding to freqList.
 * \param[in] numOfPoint, total number of points for the list.
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_IQLINK_SetPathLossTable(IQAPI_PORT_ENUM rfPort, int* freqList, double* pathLossList, int numOfPoint);

//! IQlink PER LP_IQLINK_SetIperf
/*!
 * \param[in] iperfInterval, set interval in second for iperf report. Default: 1.
 * \param[in] iperfParallelStream, set number of paprallel streams for measurement. Default: 1.
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_IQLINK_SetIperf(int iperfInterval=1, int iperfParallelStream=1);

//! Gets the SSID information
/*!
 * \param[out] ssid The buffer that will return the ssid information.
 * \param[in] buf_size Indicates the size of the buffer
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 *
 */
IQMEASURE_API int LP_IQLINK_GetSSID(char *ssid, int buf_size);

//! Set DUT Role (whether acting as AP or Client)
/*!
 * \param[in] dutApRole Indicates the role: LINK_AP_MODE (0) or LINK_CLIENT_MODE (1)
 * \param[in] ssid DUT SSID when acting as AP
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 *
 */
IQMEASURE_API int LP_IQLINK_SetDutApRole(int dutApRole, char *ssid);

//! Check DUT connection
/*!
 * \return ERR_OK if connection between DUT and IQlink is good; otherwise call LP_GetErrorString() for detailed error message.
 *
 */
IQMEASURE_API int LP_IQLINK_CheckDutConnection();

//! Retrieve log file from tester
/*!
 * \return ERR_OK if connection between DUT and IQlink is good; otherwise call LP_GetErrorString() for detailed error message.
 *
 */
IQMEASURE_API int LP_IQLINK_RetrieveLogFileFromTester(char* filename, char* buffer, int maxLength, unsigned int *actualLength);

//! IQLINK Set End Point Power And Attenuation
/*!
 * \param[in] endpointPower
 * \param[in] attenuation
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_IQLINK_SetEndPointPowerAndAttenuation(double endpointPower, double attenuation);

//***************************************************************************//
//                              IQlink END                                   //
//***************************************************************************//

IQMEASURE_API int LP_BFchanEstApplet(char *outputWaveform);
//**************************************************************************//
//                            IQBeam START                                  //
//**************************************************************************//

//! Check IQBeam current status
/*!
 * \param[in] FESW_index, if multiple IQBeam is cascaded, use this argument to select which IQBeam to query, it starts with 1
 * \param[out] IQBeam_mode, returns the current status of the selected IQBeam, 0 represents FOUR_TO_FOUR true MIMO mode and 1 means FOUR_TO_ONE beamforming mode
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_IQBeam_CheckStatus( unsigned int FESW_index, unsigned int *IQBeam_mode );

//! Set IQBeam mode
/*!
 * \param[in] FESW_index, if multiple IQBeam is cascaded, use this argument to select which IQBeam to control, it starts with 1
 * \param[out] IQBeam_mode, specifies the selected IQBeam mode, options are as follows:
 *		   - 		FOUR_TO_FOUR,
 *		   - 		FOUR_TO_ONE,
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_IQBeam_SetMode( unsigned int FESW_index, IQBEAM_MODE IQBeam_mode );

//**************************************************************************//
//                            IQBeam END                                    //
//**************************************************************************//

//**************************************************************************//
//                            IQSwitch 3101 START                           //
//**************************************************************************//
//! Set IQswitch mode (turn on a port connection to the resource)
/*!
 * \param[in] FESW_index, use this argument to select which switch module to control, it starts with 1
 * \param[in] port_index, use this argument to select Port A or Port B, it starts with 1
 * \param[in] resource, use this argument to select resource (RF1~4), it starts with 1		
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_IQSwitch_SetMode( unsigned int FESW_index, unsigned int port_index, unsigned int resource );

//! Clear IQswitch mode (turn off all ports connecting to source)
/*!
 * \param[in] FESW_index, use this argument to select which switch module to control, it starts with 1
 * \param[in] resource, use this argument to select resource (RF1~4), it starts with 1		
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
 */
IQMEASURE_API int LP_IQSwitch_ClearMode( unsigned int FESW_index, unsigned int resource );
//**************************************************************************//
//                            IQSwitch 3101 END                           //
//**************************************************************************//



// FEFF Added by Linchao
//**************************************************************************//
//                            FEFF START                                    //
//**************************************************************************//

//! FEFF APLC (Automatic Path Loss Calibration)
/*!
 * \param[in] port array, which indicates the ports to be tested, for example port[]={PORT_1,PORT_1}.
 * \param[in] freqBandSelect. FEFF default sweeping frequency is from 1.8G to 2.6G (low) and 4.9G to 6G (High). 
			  So we use "freqBandSetting" to differentiate which band we want to sweep. freqBandSelect = 0 means disable both low and high band.
			  freqBandSelect = 1 means disable low band and enable high band. freqBandSelect = 2 means enable low band and disable high band. freqBandSelect = 3 means enable both.
 * \param[in] lowBandFreqSetting, indicate the low band sweeping spanning frequency. the format is [startFreq, endFreq, step].
 * \param[in] highBandFreqSetting, indicate the high band sweeping spanning frequency. the format is [startFreq, endFreq, step].
 * \param[out] lowBandpathLoss 2D array, return the low band path loss corresponding to port. Each row represents the path loss of each port on specified frequency.
 * \param[out] highBandpathLoss 2D array, return the high band path loss corresponding to port. Each row represents the path loss of each port on specified frequency. 

 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int LP_AutoPLCalibration(int port[], int freqBandSelect, double lowBandFreqSetting[], int numberOfLowBandChannels, double highBandFreqSetting[], int numberOfHighBandChannels, double **lowBandpathLoss, double **highBandpathLoss);

//! FEFF APLC (Automatic Path Loss Calibration)
/*!
 * \param[in] port array, which indicates the ports to be tested, for example port[]={PORT_1,PORT_1}.
 * \param[in] freqBandSelect. FEFF default sweeping frequency is from 1.8G to 2.6G (low) and 4.9G to 6G (High). 
			  So we use "freqBandSetting" to differentiate which band we want to sweep. freqBandSelect = 0 means disable both low and high band.
			  freqBandSelect = 1 means disable low band and enable high band. freqBandSelect = 2 means enable low band and disable high band. freqBandSelect = 3 means enable both.
 * \param[in] lowBandFreqSetting, indicate the low band sweeping spanning frequency. the format is [freq_1, freq_2, freq_3, freq_4].
 * \param[in] highBandFreqSetting, indicate the high band sweeping spanning frequency. the format is [freq_1, freq_2, freq_3, freq_4].
 * \param[out] lowBandpathLoss 2D array, return the low band path loss corresponding to port. Each row represents the path loss of each port on specified frequency.
 * \param[out] highBandpathLoss 2D array, return the high band path loss corresponding to port. Each row represents the path loss of each port on specified frequency. 
 
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int LP_AutoPLCalibration_FourPoint(int port[], int freqBandSelect, double lowBandFreqSetting[4], double highBandFreqSetting[4], double lowBandpathLoss[][MAX_BUFFER_LEN], double highBandpathLoss[][MAX_BUFFER_LEN]);

//FEFF FHCR (Fixture Health Check Reference)
/*!
 * \param[in] port array, which indicates the ports to be tested, for example port[]={PORT_1,PORT_1}.
 * \param[in] maxPL. The user specifies the approximate maximum path loss expected.
 * \param[in] res. Resolution bandwidth of sweeping frequency.
 * \param[out] TDR_magnitude, returns time-domain reflectometry (TDR) magnitude.
 * \param[out] pathDistance, returns RF path distance (inch).

 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int LP_FixtureHealthCheckRef(int port[], double maxPL, double res, double TDR_magnitude[][MAX_BUFFER_LEN], double pathDistance[][MAX_BUFFER_LEN]);

//FEFF FHCT(Fixture Health Check Test)
/*!
 * \param[in] port array, which indicates the ports to be tested, for example port[]={PORT_1,PORT_1}.
 * \param[in] HCH sensitivity, for more info please refer to LitePoint FEFF whitepaper.
 * \param[out] statusChanged, returns the current status. 0 means fixture health check success, 1 means fixture health check fails, 2 means an error in steup
 * \param[out] TDR_magnitude, returns time-domain reflectometry (TDR) magnitude
 * \param[out] pathDistance, returns RF path distance (inch)
 * \param[out] delta, returns the difference between reference and current testing.

 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int LP_FixtureHealthCheckTest(int port[], double sensitivity, int statusChanged[], double TDR_magnitude[][MAX_BUFFER_LEN], double pathDistance[][MAX_BUFFER_LEN], double delta[][MAX_BUFFER_LEN]);

//FEFF DUT sense
/*!
 * \param[in] port array, which indicates the ports to be tested, for example port[]={PORT_1,PORT_1}.
 * \param[in] This threshold specifies the amount of reflection energy from the fixture Open state that should be eliminated when the DUT is connected. For more info please refer to LitePoint FEFF whitepaper.
 * \param[out] statusChanged, returns the current status. 0 means DUT sense fails, 1 means DUT is connected, 2 means an error in steup
 * \param[out] TDR_magnitude, returns time-domain reflectometry (TDR) magnitude
 * \param[out] pathDistance, returns RF path distance (inch)
 * \param[out] delta, returns the difference between reference and current testing.

 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int LP_DUTSense(int port[],double threshold, int statusChanged[], double TDR_magnitude[][MAX_BUFFER_LEN], double pathDistance[][MAX_BUFFER_LEN], double delta[][MAX_BUFFER_LEN]);

//FEFF  Updating System Open-Short-Load (OSL) Calibration
/*!
 * \param[in] port array, which indicates the ports to be tested, for example port[]={PORT_1,PORT_1}.
*/
IQMEASURE_API int LP_UpdateOSLCali(int port[]);

//FEFF Measuring Return Loss at the Test Equipment Port
/*!
 * \param[in] port array, which indicates the ports to be tested, for example port[]={PORT_1,PORT_1}.
 * \param[in] freqBandSelect. FEFF default sweeping frequency is from 1.8G to 2.6G (low) and 4.9G to 6G (High). 
			  So we use "freqBandSetting" to differentiate which band we want to sweep. freqBandSelect = 0 means disable both low and high band.
			  freqBandSelect = 1 means disable low band and enable high band. freqBandSelect = 2 means enable low band and disable high band. freqBandSelect = 3 means enable both.
 * \param[in] lowBandFreqSetting, indicate the low band sweeping spanning frequency. the format is [startFreq, endFreq, step].
 * \param[in] highBandFreqSetting, indicate the high band sweeping spanning frequency. the format is [startFreq, endFreq, step].
 * \param[out] lowBandreturnLoss, 2D array, return the low band return loss corresponding to port. Each row represents the return loss of each port on specified frequency.
 * \param[out] highBandreturnLoss, 2D array, return the high band return loss corresponding to port. Each row represents the return loss of each port on specified frequency.

 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int LP_ReturnLossTesterPort(int port[], int freqBandSelect, double lowBandFreqSetting[3], double highBandFreqSetting[3], double lowBandreturnLoss[][MAX_BUFFER_LEN], double highBandreturnLoss[][MAX_BUFFER_LEN]);

//FEFF Measuring Return Loss at the Fixture RF Probe
/*!
 * \param[in] port array, which indicates the ports on tester, for example port[]={PORT_1,PORT_1}.
 * \param[in] freqBandSelect. FEFF default sweeping frequency is from 1.8G to 2.6G (low) and 4.9G to 6G (High). 
			  So we use "freqBandSetting" to differentiate which band we want to sweep. freqBandSelect = 0 means disable both low and high band.
			  freqBandSelect = 1 means disable low band and enable high band. freqBandSelect = 2 means enable low band and disable high band. freqBandSelect = 3 means enable both.
 * \param[in] lowBandFreqSetting, indicate the low band sweeping spanning frequency. the format is [startFreq, endFreq, step].
 * \param[in] highBandFreqSetting, indicate the high band sweeping spanning frequency. the format is [startFreq, endFreq, step].
 * \param[out] lowBandreturnLoss, 2D array, return the low band return loss corresponding to port. Each row represents the return loss of each port on specified frequency.
 * \param[out] highBandreturnLoss, 2D array, return the high band return loss corresponding to port. Each row represents the return loss of each port on specified frequency.

 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int LP_ReturnLossFixtureRFProbe(int port[], int freqBandSelect, double lowBandFreqSetting[3], double highBandFreqSetting[3], double lowBandreturnLoss[][MAX_BUFFER_LEN], double highBandreturnLoss[][MAX_BUFFER_LEN]);

//**************************************************************************//
//                            FEFF END                                    //
//**************************************************************************//

//**************************************************************************//
//        The following functions only apply to zSeries                     //
//**************************************************************************//

// Parse the specified file to call SSBI Read/Write functions (for zSeries ONLY)
/*!
* \param[in] filename, parses .tpf file to .csv file

* \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int LP_SetPatternFile(char *filename);

// Configure the AWG to produce a clock signal with specified power level (for zSeries ONLY)
/*!
* \param[in] powerLevVolt, the voltage of the output
* \param[in] offsetDC, offset DC value if any present
* \param[in] frequency, frequency of the output signal
* \param[in] waveformType, control type of waveform
		-0 for sine
		-1 for square
		-2 for triangle
* \param[in] loadImpedance, value of impedance if present
* \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int LP_SetRefGenerator(int mode, double powerLevVolt, double offsetDC, double frequency, int waveformType, double loadImpedance);

// Configure the VSG SSBI (for zSeries ONLY)
/*!
* \param[in] powerLevVolt, the voltage of the output
* \param[in] frequency, frequency of the output signal
* \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int LP_SetSSbiGenerator(double powerLevVolt, double frequency);

// Get Power measurement at offset using FFT (for zSeries ONLY)
/*!
* \param[in] offset, specifies offset frequency from center in Hz

* \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int LP_AnalyzeGPRFPowerbyFFT(double offset);

// Perform power analysis at on offset frequency from the center frequency.
/*!
* \param[in] numSegmsToAnalyze, segment index for analysis
* \param[in] offset, specifies offset frequency from center frequency in Hz
* \param[in] intervalLength, specifies the interval length in seconds
* \param[in] measurementLength, specifies the measurement length in seconds
* \param[in] filterType, Valid options are 0 : NONE, 1 : GAUSS, 2 : RRC specifies which filter to be used. Default 1 which is GAUSS 
* \param[in] filterBWinhz, specifies filter BW in Hz. Default is 5e6Hz
* \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int LP_Segms_AnalyzeGPRFPowerbyFFT(int numSegmsToAnalyze, double offsetInhz, double intervalLength, double measurementLength, int filterType =1 , int filterBWinhz = 5e6);

// Configure the SMU channels with specified voltage (for zSeries ONLY)
/*!
* \param[in] name, specifies the channel alias of the SMU
* \param[in] voltage, specifies the voltage at which SMU must be set
* \param[in] state, 0 for off and 1 for on.

* \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int LP_ConfigureSMU_Voltage(char* name, double voltage, int state);

// Query the address and get the data at that address (for zSeries ONLY)
/*!
* \param[in] pin, 0 for TX pin and 1 for RX pin
* \param[in] addr, addr to which the function queries
* \param[in] data, data returned from query

* \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int LP_SSBI_Query(int pin, int addr, int *data);

// Set the VSG DIO output level and frequecny (for zSeries ONLY)
/*!
* \param[in] level, set the level in volts
* \param[in] frequency, set the frequency in Hz

* \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int LP_SetSsbiGenerator(double level, double frequency);

// Measure the SMU voltage and current (for zSeries ONLY)
/*!
* \param[in] alias string, to specify the SMU
* \param[in] mode, 1 for single measurement or 0 for multiple measurements

* \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int LP_SMUCapture();
// Set technology (for zSeries ONLY)
/*!
* \param[in] technology : ZTPM_STANDARD_DOCSIS_3P1       = 0x0120

* \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int LP_SetTechnology(int technology); 
// AnalyzeDocsis pow, txq, sem, sem peak (for zSeries ONLY)
/*!
* \param[in] analyzeType : flag (#TPM_DOCSIS_3P1_ANALYZE_TXQ(1), ZTPM_DOCSIS_3P1_ANALYZE_PWR(2), ZTPM_DOCSIS_3P1_ANALYZE_SEM(4), ZTPM_DOCSIS_3P1_ANALYZE_SEM_PEAK(8) 
* \param[in] freqRange 
* \param[in] centerFreq
* \param[in] chanBw
* \param[in] is4KFftMode
* \param[in] minislotSymbolCount
* \param[in] cyclicPrefix
* \param[in] rolloffPeriod
* \param[in] pilotPattern
* \param[in] bitLoading
* \param[in] subcarrierOffset
* \param[in] minislotCount
* \param[in] enableDataChannelEst
* \param[in] enablePilotFreqEst
* \param[in] enableDataSymbolClkCor

* \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int LP_AnalyzeDocsis(int analyzeType,
                                   int freqRange,
                                   int centerFreq,
                                   int chanBw,
                                   int is4KFftMode,
                                   int minislotSymbolCount,
                                   int cyclicPrefix,
                                   int rolloffPeriod,
                                   int pilotPattern,
                                   int bitLoading,
                                   int subcarrierOffset,
                                   int minislotCount,
                                   int enableDataChannelEst,
                                   int enablePilotFreqEst,
                                   int enableDataSymbolClkCor);


//**************************************************************************//
//                       zSeries Functions End                              //
//**************************************************************************//


/*IQLiteAptRemoveBegin*/
//**************************************************************************//
//                            SequenceEngine BEGIN                         //
//**************************************************************************//

//MPS Tech layer API parameters for one measuremnt step (will break into sequence steps with size of numOfPowerLevels)
#ifndef MAX_NUM_SEQ_POW_LEVELS
    #define MAX_NUM_SEQ_POW_LEVELS 128
#endif

typedef struct MPS_MEAS_STEP_PARA
{
	//Common
	char  seqStepType[16];            //Type of the sequence step (WIFI_TX? WIFI_RX?)
	double  rfFreqHz;               //Fequency of amg subset for the sequence step
	int rfPort[MAX_MIMO_TESTER_NUM*MAX_MPTA_PORTS];    //PORT array for MIMO configuration (example 3x3 single DUT or APT on RF3A/3B/3C: [4 4 4 0]; example 2up SPT RX on RF1A/1B/1C and RF3A/3B/3C [2 2 2 0; 4 4 4 0])
	double cableLoss[MAX_MIMO_TESTER_NUM*MAX_MPTA_PORTS];  //cable loss array for MIMO configuration (usage convention is the same as rfPort[] )
	int    numOfPowerLevels;              //size of TXpowerLeveldBm or RXpowerLeveldBm array
	unsigned int multiPortSelection; //multi-port selection for SPT RX, composite and switched MIMO to indicate the number of ports connecting to the same VSA/VSG module, used in combination with rfPort[] and cableLoss[]

	double TXtargetPowerLeveldBm[MAX_NUM_SEQ_POW_LEVELS];  //for TX packet (for any combinations of E/M/P/S, W[]) or RX ACK target power
	double  TXpacketPARdB;           //for TX packet or RX ACK peak-to-average
	double  triggerleveldB;          //VSA trigger threshold level relative to the reference level

	//Tx only
	int	captureLengthUs;  
	int	preTriggerTimeUs;
	int	skipPktCnt;
	int	captureCnt;
	int measurementType[10];     //[0] for the number of measurements, the rest for measurement types, less than 10 elements
	int	dataRateType;
	int	bandwidthType; 
	int	useAllSignals; 
	int analysisOrder[MAX_MIMO_TESTER_NUM];   //analysis order for MIMO/CDD configuration (size = number of signals)
	int	analysisVsaNum;   
	int diversityCDD;
	int analysisSkipPktCount;
	int txifs;

	//Rx only
	double  RXtargetPowerLeveldBm[MAX_NUM_SEQ_POW_LEVELS];  // RX target powers (for any combinations of L,H,S)
	int	    packetCount;
	int     vsgSegment;                    //vsg wave list segment index
	double  vsgPacketLengthUs;             //vsg wave packet length in us
	int     vsgPacketDelayUs;
	int     ackTimeoutUs;
	int     ackWidthUs; 

	//Special
	unsigned int  moduleGroupSubsetIndex;  //Subset index of module group,satrting from 0 (ST: 0; MT: 1)
	bool    bQWait_for_ModuleSync;	   //RSDB module sync (needed by MT primary and secondary)
	int     bQWait_Trig_moduleGroupSubsetIndex; //to indicate Subset index of module group which serves as module sync trigger, default = 0
	int     bQwait_ModuleDelayUs;   //RSDB module delay in us
	double  qwaitSyncHoldOffms;     //trigger hold off time in ms for QWAIT module sync
	char    moduleSyncMarker[16];	//RSDB VSA/VSG module sync marker, needed only by MT primary
	bool	bRX_ready_required;     //SmaRxTx RX ready (#VSG:INIT+1)
	bool	bCatch_up_required;     //SmaRxTx catch up (#VSG:INIT+1)
	bool	bQWait_for_TX_done;     //SmaRxTx MTXD

	/*Note: internal logic for VSA/VSG/RFD is kept within sequence engine
	  1. RFD:TRIG:LEVEL is derived from (TXtargetPowerLeveldBm, *cableLoss)
	  2. VSA:RLEV is derived from (TXtargetPowerLeveldBm, TXpacketPARdB, *cableLoss)
	  3. VSA:TRIG:LEVEL is derived from (VSA:RLEV, triggerleveldB)
	  4. VSG:POWER:LEVEL is derived from (RXtargetPowerLeveldBm, *cableLoss)
	*/

	void reset()
	{
		//Common
		memset( seqStepType, '\0', 16 );
		rfFreqHz = 0.0;
		for (int i=0; i<MAX_MIMO_TESTER_NUM*MAX_MPTA_PORTS; i++)
		{
			rfPort[i] = 0;
			cableLoss[i] = 0.0;
		}
		numOfPowerLevels = 1;
		multiPortSelection = 1;

		for (int i=0; i<MAX_NUM_SEQ_POW_LEVELS; i++)
		{
			TXtargetPowerLeveldBm[i] = 0.0;
		}
		TXpacketPARdB = 0;
		triggerleveldB = 0;

		//Tx only
		captureLengthUs = 0;  
		preTriggerTimeUs = 0;
		skipPktCnt = 0;
		captureCnt = 0;
		for (int i=0; i<10; i++)
		{
			measurementType[i] = 0;
		}
		dataRateType = 0;
		bandwidthType = 0; 
		useAllSignals = 0; 
		for (int i=0; i<MAX_MIMO_TESTER_NUM; i++)
		{
			analysisOrder[i] = 0;
		}
		analysisVsaNum = 1;   
		diversityCDD = 0;
		analysisSkipPktCount = 0;
		txifs = 100;

		//Rx only
		for (int i=0; i<MAX_NUM_SEQ_POW_LEVELS; i++)
		{
			RXtargetPowerLeveldBm[i] = 0.0;
		}
		packetCount = 0;
		vsgSegment = 0;
		vsgPacketLengthUs = 0.0;
		vsgPacketDelayUs = 0;
		ackTimeoutUs = 0;

		//Special
		moduleGroupSubsetIndex = 0;
		bQWait_for_ModuleSync = false;
		bQWait_Trig_moduleGroupSubsetIndex = 0;
		bQwait_ModuleDelayUs = 0;
		qwaitSyncHoldOffms = 3.5;
		memset( moduleSyncMarker, '\0', 16 );
		bRX_ready_required = false;
		bCatch_up_required = false;
		bQWait_for_TX_done =false;
	}

}MPS_MEAS_ONE_STEP;


typedef struct MPS_ACK_INFO
{
	int		VsgPacketCount;		    
	int		AckCount;	
	double  perPercent;
	double	AckResponseTimeMax;
	double	AckResponseTimeMin;
	double	AckWidthMax;
	double	AckWidthMin;

	void reset()
	{
		VsgPacketCount = 0;		    
		AckCount = 0;	
		perPercent = 0.0;
		AckResponseTimeMax = 0.0;
		AckResponseTimeMin = 0.0;
		AckWidthMax = 0.0;
		AckWidthMin = 0.0;
	}

}MPS_ACK_INFO_ONE_STEP;


//! Start or update MetaSequenceEngine (MSE). If function is called for the first time, MSE will be started; if MSE has already been started, the function will update MSE settings
/*!
 * \param[in]  total number of AMG subsets; for MT (RSDB), numOfAmgSubsets = 2; for regular tests, numOfAmgSubsets = 1;
 * \param[in]  total number of modules used by all AMG subsets
 * \param[in]  2D boolean array (size = numOfAmgSubsets x TotNumOfModules) for module selection in all AMG subsets
 * \param[in]  if RFD needs to be disabled (for 4364RSDB mode, RFD needs to be disabled)
 * \param[out] none
 
	RSDB examples:
	1+1 mode    selectionTable[][]={{1, 0},{0, 1}};
	2+1 mode    selectionTable[][]={{1, 1, 0},{0, 0, 1}};
	2+2 mode    selectionTable[][]={{1, 1, 0, 0},{0, 0, 1, 1}};
	3+1 mode    selectionTable[][]={{1, 1, 1, 0},{0, 0, 0, 1}};
	2+1+1 mode  selectionTable[][]={{1, 1, 0, 0},{0, 0, 1, 0},{0, 0, 0, 1}};
	
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int LP_InitSequenceEngine(int numOfAmgSubsets, int totNumOfModules, bool* selectionTable[], bool disableRFD = false);


//! Pump measurement step parameters into MSE
/*!
 * \param[in] structured parameter for one MeasurementStep
 * \param[out] tottal number of Sequence steps (MPS:INITs) for current MeasurementStep
	
 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int LP_SetMeasurementStepParas(MPS_MEAS_ONE_STEP* sourceMeasurementStep, unsigned int *totNumofSeqSteps);


//! MSE builds sequence (from MeasurementMatrix) and sends sequence definition to tester
/*!
 * \param[in] none
 * \param[out] none

 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int LP_SequenceBuild();


//! Arm sequence (SEQ#:EXEC:HSN?) and return sequence start notification
/*!
 * \param[in] none
 * \param[out] none

 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int LP_InitiateSequence();


//! Wait for sequence to finish execution and query return errors
/*!
 * \param[in] none
 * \param[out] none

 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int LP_FinishSequence();


//! For a MPS block, load Rx waveforms and calculate attributes if not exist in VSG memory
/*!
 * \param[in] number of waveform files
 * \param[in] a list of waveform file keywords, separated by comma or semicon
 * \param[in] source folder of waveform files
 * \param[in] waveform file common header, ie "MP_WiFi_"
 * \param[in] Module group subset index, default -1 for all modules in single-Technology test, non-negative index for a specific subset in Multi-Technology test
 * \param[out] none

 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int LP_LoadMpsWaves(int numOfFiles, char *waveFileKeywordList, char *waveformFilePath, char *fileHeader = "MP_WiFi_", int amgSubGroupIndex = -1);

//! Query waveform attributes for MPS Rx sequence build
/*!
 * \param[in] keyword name of a waveform
 * \param[out] MPS switch time in microsec of the waveform
 * \param[out] wave segment index in the wavelist of the waveform
 * \param[in] Module group subset index, default -1 for all modules in single-Technology test, non-negative index for a specific subset in Multi-Technology test

 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int LP_GetMpsWaveAttributes(char *waveFileKeyword, double *switchTimeUs, int *wsegIndex, int amgSubGroupIndex = -1);

//! Fetch MPS ACK results for a sequence step
/*!
 * \param[in] VSG sequence step index
 * \param[in] ACK result structure
 * \param[in] Module group subset index, default -1 for all modules in single-Technology test, non-negative index for a specific subset in Multi-Technology test
 * \param[out] none

 * \return ERR_OK if no errors; otherwise call LP_GetErrorString() for detailed error message.
*/
IQMEASURE_API int LP_FetchMpsAckInfo(unsigned int vsgSeqStepIndex, MPS_ACK_INFO_ONE_STEP *rxResultsOneStep, int amgSubGroupIndex = -1);

//**************************************************************************//
//                            SequenceEngine END                            //
//**************************************************************************//
/*IQLiteAptRemoveEnd*/

#endif // end of _IQMEASURE_H_
