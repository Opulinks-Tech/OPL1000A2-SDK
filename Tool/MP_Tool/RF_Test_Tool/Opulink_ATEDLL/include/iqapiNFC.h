// ****************************************************************
// Date   : 5/26/2009
// ****************************************************************
#ifndef _IQAPINFC_H_
#define _IQAPINFC_H_

#include "iqapiDefines.h"
#include "iqapiHndlBase.h"
#include "iqapiHndlMatlab.h"


class IQ_API iqapiMeasurementInt
{
public:
   iqapiMeasurementInt();
   ~iqapiMeasurementInt();

   int      length;
   int      *content;

};


class IQ_API iqapiMeasurementDouble
{
public:
   iqapiMeasurementDouble();
   ~iqapiMeasurementDouble();

   int      length;
   double   *content;

};

class IQ_API iqapiTxNfc
{
public:
   iqapiTxNfc() 
   {
      userSignalAmplitudeType       = IQV_USER_SIGNAL_AMPLITUDE_AS_VOLT;
      userSignalAmplitude           = 0.2;

      userAntennaLoadAmplitudeVolts = -0.5;
   };
   ~iqapiTxNfc() {};

   double                              userSignalAmplitude;   
   IQV_USER_SIGNAL_AMPLITUDE_TYPE      userSignalAmplitudeType;

   double                              userAntennaLoadAmplitudeVolts;   

   int   GetScaleAndTxAmp(double *txAmpVolts, double *sigScale, double *antennaLoadScale);

};

class IQ_API iqapiRxNfc
{
public:
   iqapiRxNfc() 
   {
      triggerType    = IQV_TRIG_TYPE_FOR_IQ2010_DIG_I;
      triggerTimeOut = 10;
      triggerLevelDb = -30;
      samplingTimeSecs = 0.015;

      frameLen       = 0;
      zeroPrefixLen  = 0;

   };
   ~iqapiRxNfc() {};

   //%control FPGA registers 0x7fa8 for triggering VSA I channel
   IQV_TRIG_TYPE_ENUM               triggerType;     //<! IQV_TRIG_TYPE_FREE_RUN or IQV_TRIG_TYPE_FOR_IQ2010_DIG_I

   double		samplingTimeSecs;	   //!< Sampling time, in seconds
   double		triggerTimeOut;		//!< Trigger timeout in seconds
   double		triggerLevelDb;		//!< Signal (IF) trigger level in dB, relative to VSA master's \c rfAmplDb setting.

   // Internal use
   int         GetFrameLen() { return frameLen; }
   void        SetFrameLen(int frameLenNfc) { frameLen = frameLenNfc; }
   int         GetZeroPrefixLen() { return zeroPrefixLen; }
   void        SetZeroPrefixLen(int zeroPrefixLenNfc);
 
private:
   int         frameLen;
   int         zeroPrefixLen;

};


class IQ_API iqapiWaveGenParmsNfc
{
public:	
	iqapiWaveGenParmsNfc(void);	//!< Constructor.
	~iqapiWaveGenParmsNfc(void);	//!< Destructor.

	int      SetDefault();    //!<Resets the parameters of an \c iqapiWaveGenParmsNfc object to default parameters.

   IQV_NFC_ISO_TYPE     isoType;


   IQV_NFC_WAVE_SIG_TYPE         waveSpecPhySigType;
   IQV_NFC_BANDWIDTH_KHZ         waveSpecPhyBandwidthkHz;
   IQV_NFC_MANCHESTER_POLARITY   waveSpecPhyManchesterPolarityReverse;
   double                        waveSpecPhyManchesterModulationIndex;
   int                           waveSpecPhyFrameGapMs;
   IQV_NFC_ENABLE_QCH_BIAS       waveSpecPhyEnableQChBias;

   
   IQV_NFC_USE_PAYLOAD_TYPE      waveSpecMacUsePayloadType;
   char                          waveSpecMacPayloadASCIIText[237];      // Maximum allow
   iqapiMeasurementInt           waveSpecMacPayloadBits;
   iqapiMeasurementDouble        waveSpecMacPayloadBitsTest;
   char                          waveSpecMacNfcId3iAsciiText[11];
   char                          waveSpecMacNfcId3tAsciiText[11];

   char                          lastErr[MAX_LEN_ERR_TXT];	//!< Char array (\c hndl->wave->lastErr) that contains an error message if a call to the \c hndl->GenerateWave function fails.
   const char *                  GetType() { return type; };

private:
   char     *type;
	int      UnpackWaveParam(void *mx_result_in);


};


class IQ_API iqapiModulationWaveNfc : public iqapiModulationWave
{
public:	
   iqapiModulationWaveNfc(void);    //!< Constructor
   ~iqapiModulationWaveNfc(void);	//!< Destructor

   int                  waveOutValid;
   char                 *waveOutErrTxt;

   iqapiMeasurementDouble *waveOutRawMsg;

   char                 *waveOutFrameInfoStartByte;
   char                 *waveOutFrameInfoPreemble;
   char                 *waveOutFrameInfoLength;
   char                 *waveOutFrameInfoPayload;
   char                 *waveOutFrameInfoCrc;

};

//! Specifies analysis parameters for NFC analysis
class IQ_API iqapiAnalysisNfc : public iqapiAnalysis
{
public:
   iqapiAnalysisNfc();
   ~iqapiAnalysisNfc();

   int SetDefault();
   int UnpackNfcAnalysis(void *mx_result_in);

   IQV_NFC_ISO_TYPE     isoType;

	char		lastErr[MAX_LEN_ERR_TXT]; //!< Indicates the last error message text.

   IQV_NFC_ANALYSIS_SIG_TYPE     analysisPhySigType;
   IQV_NFC_MOD_TYPE              analysisPhyModType;
   double                        analysisPhyFreqDomainDetectTh;
   bool                          analysisPhyDecodeTransportMsg;
   int                           analysisPhyIdOfBurstToAnalyze;
   double                        analysisPhyAnalyzeRangeSecStart;    //!< Start analyzed range in sec
   double                        analysisPhyAnalyzeRangeSecStop;     //!< Stop analyzed range in sec
   double                        analysisPhyMeasureCarrierFreq;
   bool                          analysisPhyDoRfAnalysis;         //!< true: Do RF analysis. False: No RF analysis. Default: false
   int                           analysisPhyPercentObw;           //!< Valid range 0 - 100. Default: 99 
   IQV_WINDOW_TYPE_ENUM          analysisPhyRfAnaWinType;         //!< Default: hanning
   bool                          analysisPhyPlotRawSigPsd;        //!< 1= plot PSD/time for Raw signal, subject to AnalyzedRange constraints (actual plotted signal is also bounded by PowerDetect if it is valid)

   // Zoom over the generated spectrum plot. To plot the entire available range, use [-1, -1]. To plot over 4Mhz to 23Mhz, use [4, 23]
   double                        analysisPhyRawSigPsdPlotZoomRangeMhzStart;     //!< zoom start in unit of MHz
   double                        analysisPhyRawSigPsdPlotZoomRangeMhzStop;      //!< zoom stop in unit of MHz

   double                        analysisPhyRfAnaResBw;           //!< Resolution bandwidth, min 1khz, max 300kHz
   IQV_USER_SIGNAL_AMPLITUDE_TYPE   analysisPhyUseAmpsPerMeter;   //!< indicate if it should report as volts or ampere per meter
   double                        analysisPhyHPerV;                //!< Correction factor that converts volts into A/m

};

//! Specifies NFC analysis results generated using \c iqapiAnalysisNfc objects
class IQ_API iqapiResultNfc : public iqapiResult
{
public:
   iqapiResultNfc();
   ~iqapiResultNfc();

   // Valid flag for different analysis
   int                  valid;
   char                 *errTxt;
   int	               packetDetection;     //!< Indicates valid packet detection
   int	               acquisition;	      //!< Indicates valid packet acquisition
   int	               demodulation;	      //!< Indicates valid packet demodulation
   int                  completePacket;	   //!< Indicates that at least one complete packet was found by the analysis
   int                  rfAnalysis;          //!< Indicates that rf analysis passed, and if doRfAnalysis is true

   // Power detect
   iqapiMeasurement     *start_sec;          //!< Start time, in seconds, for each packet detected.
   iqapiMeasurement     *stop_sec;           //!< Stop time, in seconds, for each packet detected
   iqapiMeasurement     *complete_burst;     //!< Indicates complete packet (start/stop found).

   // Wave
   iqapiMeasurement     *waveData;              //!< Baseband NFC waveform array
   iqapiMeasurement     *waveFs;                //!< Sampling rate
   int                  waveLen;                //!< Length of the array
   int                  waveStartPt;            //!< Start index of the array
   iqapiMeasurement     *waveSaX;               //!< Spectrum plot x-axis
   iqapiMeasurement     *waveSaY;               //!< Spectrum plot y-axis
   char                 *waveSaX_string;        //!< X label
   char                 *waveSaY_string;        //!< Y label
   iqapiMeasurement     *waveSaLength;          //!< Fft size
   iqapiMeasurement     *waveSaRes_bw;          //!< Resolution bandwidth
   iqapiMeasurement     *waveSaNoise_bw;        //!< Noise bandwidth
   iqapiMeasurement     *waveX;                 //!< plot x-axis in time 
   iqapiMeasurement     *waveY;                 //!< plot y-axis in power
   char                 *waveX_string;          //!< X label
   char                 *waveY_string;          //!< Y label

   // CapSigInfo
   iqapiMeasurement     *capSigInfoCarrierFreq;          //!< Carrier frequency is available if analysisPhyMeasureCarrierFreq >0
   iqapiMeasurement     *capSigInfoBurstDurationSec;     //!< Length of analyzed frame in second
   iqapiMeasurement     *capSigInfoSaX;                  //!< Spectrum plot x-axis
   iqapiMeasurement     *capSigInfoSaY;                  //!< Spectrum plot y-axis
   char                 *capSigInfoSaX_string;           //!< X label
   char                 *capSigInfoSaY_string;           //!< Y label
   iqapiMeasurement     *capSigInfoSaLength;             //!< Fft size
   iqapiMeasurement     *capSigInfoSaRes_bw;             //!< Resolution bandwidth
   iqapiMeasurement     *capSigInfoSaNoise_bw;           //!< Noise bandwidth
   iqapiMeasurement     *capSigInfoWaveX;                //!< Spectrum plot x-axis
   iqapiMeasurement     *capSigInfoWaveY;                //!< Spectrum plot y-axis
   char                 *capSigInfoWaveX_string;         //!< X label
   char                 *capSigInfoWaveY_string;         //!< Y label

   // Wave form info
   iqapiMeasurement     *waveInfoModIndex;                  //!< Modulation index
   iqapiMeasurement     *waveInfoModDepth;                  //!< Modulation depth
   iqapiMeasurement     *waveInfoCarrierAmplitudeDbV;      //!< 20log10(Amp/v)
   char                 *waveInfoModType;                   //!< Modulation type string: Modified miller, Manchester
   char                 *waveInfoIsoType;                   //!< ISO type string: 18092, 14443a, 14443b
   int                  waveInfoBandwidthKHz;               //!< 106, 212, 424
   iqapiMeasurement     *waveInfoNominalBitDuration;     //!< Nominal bit duration
   iqapiMeasurement     *waveInfoNominalDataRate;         //!< Nominal data rate
   iqapiMeasurement     *waveInfoMeasuredDataRate;        //!< Measured data rate
   iqapiMeasurement     *waveInfoMeasuredBitDuration;    //!< Measured bit duration
   iqapiMeasurement     *waveInfoOnWidthSec;                //!< On width in second
   iqapiMeasurement     *waveInfoOffWidthSec;               //!< Off width in second
   iqapiMeasurement     *waveInfoDutyCycle;                 //!< Duty cycle

   // Frame info
   iqapiMeasurement     *frameInfoAnalyzedFrameId;          //!< Index of frame analyzed in the capture, starting from 1 Length(AnalysisRes.PowerDetect.start_sec) indicates total number of frames available
   bool                 frameInfoDecodeTransportMsg;        //!< true if transport message is decoded. Otherwise, false.
   char                 *frameInfoStartByte;                //!< Available only for 106kbps
   char                 *frameInfoPreamble;                 //!< Available only for 212kbps, 424kbps or higher rate
   char                 *frameInfoLen;                      //!< Length field
   char                 *frameInfoPayload;                  //!< Payload field
   char                 *frameInfoCrc;                      //!< Crc field
   bool                 frameInfoCrcFail;                   //!< true = fail, false = pass
   iqapiMeasurement     *frameInfoParityCheckErrorByteInd;  //!< Index of bytes whose odd parity check is failed Empty if no check is failed
   iqapiMeasurement     *frameInfoSigType;                  //!< 3 = Active mode initiator, 4 = Active mode target, -1 = unknown
   char                 *frameInfoSender;                   //!< Initiator, target, or unknown
   iqapiMeasurement     *frameInfoEvmDb;                    //!< EVM in dB
   iqapiMeasurement     *frameInfoSigConstellation;         //!< Normalized Complex array

   char                 *frameInfoPayloadContentMsgType;    //!< Command type ATR_REQ, WUP_REQ, PSL_REQ, DEP_REQ, DSL_REQ, RLS_REQ, xxx_RES
   int                  frameInfoPayloadContentCmdType;     //!< Corresponds to msgType, for example, 1 = initialization, 4 = data exchange protocol

   // Contains decoded payload content
   char                 *frameInfoPayloadContentNfcid;      //!< NFC ID, only available for initialization frames
   char                 *frameInfoPayloadContentDid;        //!< 
   char                 *frameInfoPayloadContentGeneralBytesAscii;   //!< 
   char                 *frameInfoPayloadContentPfb;                 //!< 
   char                 *frameInfoPayloadContentPduType;             //!< 

   // Pulse Spec
   iqapiMeasurement     *pulseSpecOnAmp;                    //!< Pulse on amplitude
   iqapiMeasurement     *pulseSpecOffAmp;                   //!< Pulse off amplitude
   iqapiMeasurement     *pulseSpecFallTime;                 //!< Pulse fall time
   iqapiMeasurement     *pulseSpecRiseTime;                 //!< Pulse rise time
   iqapiMeasurement     *pulseSpecOnOvershoot;              //!< Pulse on overshoot
   iqapiMeasurement     *pulseSpecOnUndershoot;             //!< Pulse on undershoot
   iqapiMeasurement     *pulseSpecOnRipple;                 //!< Pulse on ripple
   iqapiMeasurement     *pulseSpecOnSettlingTime;           //!< Pulse on settling time
   iqapiMeasurement     *pulseSpecOffUndershoot;            //!< Pulse off undershoot
   iqapiMeasurement     *pulseSpecOffOvershoot;             //!< Pulse off overshoot
   iqapiMeasurement     *pulseSpecOffRipple;                //!< Pulse off ripple
   iqapiMeasurement     *pulseSpecOffSettlingTime;          //!< Pulse off settling time
   iqapiMeasurement     *pulseSpecT1;                                //!< 
   iqapiMeasurement     *pulseSpecT2;                                //!< 
   iqapiMeasurement     *pulseSpecT3;                                //!< 
   iqapiMeasurement     *pulseSpecT4;                                //!< 

   //OBW
   iqapiMeasurement     *obw;                               //!< Occupied BW
   iqapiMeasurement     *actualObw;                         //!< Actual obw percentage between 0 to 1

};

////
////class iqapiHndlNfc
////{
////public:
////   iqapiHndlNfc();
////   ~iqapiHndlNfc();
////
////   int   GenerateWave(iqapiWaveGenParmsNfc *waveGen);
////   void  ResetLastErr();
////
////	char	lastErr[MAX_LEN_ERR_TXT]; //!< Indicates the last error message text.
////
////   iqapiModulationWaveNfc     *waveNfc;
////
////private:
////   int PackWaveParams(void **mx_input_in, iqapiWaveGenParmsNfc *params);
////   int UnpackModulationData(void *mx_result_in, iqapiModulationWaveNfc *waveNfc);
////   void StartMlDebug(void);
////   void EndMlDebug(void);
////
////
////};

#endif			// _IQAPINFC_H_

