/*! \mainpage IQ2010 Extension

 * The IQ2010 Extension API is an extended library based on the LitePoint low-level IQapi, which provides a measurement-oriented API instead of a tester-control-oriented API. The IQ2010 Extension API is a simple-to-use library, yet more powerful and robust in features as compared with IQapi. This advantage of IQ2010 Extension over IQapi is achieved by enabling the advanced test engine (MPS) which is only available with IQ2010 (and therefore referred to as the IQ2010 Extension).  With the IQ2010 Extension API, not only will the test engineers spend much less time in developing their solutions, they will also be able to get more tested. This measurement-oriented library is an IQapi companion, which makes IQ2010 run much faster than IQflex when testing WiFi and Bluetooth. 
  
 * \section Mask_Spec LitePoint IQ201X Extension---802.11 Mask and Spectrum measurements
 
802.11Mask Measurement
  
  The Mask measurement uses the FFT analysis result, and then applies the IEEE specified MASK, and returns the worst case margins. IEEE specifies that the peak power level from the center (in-band) is used as the reference (0dB), and power levels in the lower and upper side (out-of-band) should be lower than the specified values (dB).  Typically, multiple segments on both lower side and upper side are specified with different requirements.  

Figure 1 shows the MASK specification for 802.11b signals.  We see that two lower segments, denoted by LO_B and LO_A, and two upper segments, denoted by UP_A and UP_B, are defined.

Figure 2 shows the MASK specification for 802.11a/g/n.  We can see that four lower segments, denoted by LO_D, LO_C, LO_B, and LO_A, and four upper segments, denoted by UP_A, UP_B, UP_C, and UP_D, are defined.

 
 \image html SpectrumMeas_b.jpg "Figure 1. Spectrum Measurement for 802.11b signals"
 \image latex SpectrumMeas_b.jpg "Figure 1. Spectrum Measurement for 802.11b signals" width=6in

 \image html SpectrumMeas11_agn.jpg "Figure 2. Spectrum Measurement for 802.11a/g/n signals"
 \image latex SpectrumMeas11_agn.jpg "Figure 2. Spectrum Measurement for 802.11a/g/n signals" width=6in

 
The Mask measurement will return a pair of (Frequency, Worst Margin) for every segment as defined above.  All keywords are listed below:
 *  - <b>FREQ_MHZ_LO_D_VSA1</b>:    frequency (M Hz) in region LO_D at worst margin
 *  - <b>MARGIN_DB_LO_D_VSA1</b>:   power (Db) in region LO_D at worst margin
 *  - <b>MASK_MARGIN_DB_LO_D_VSA1</b>:   power (Db) in region LO_D at worst margin (positive value means no mask violation)
 *  - <b>FREQ_MHZ_LO_C_VSA1</b>:    frequency (M Hz) in region LO_C at worst margin
 *  - <b>MARGIN_DB_LO_C_VSA1</b>:   power (Db) in region LO_C at worst margin
 *  - <b>MASK_MARGIN_DB_LO_C_VSA1</b>:   power (Db) in region LO_C at worst margin (positive value means no mask violation)
 *  - <b>FREQ_MHZ_LO_B_VSA1</b>:    frequency (M Hz) in region LO_B at worst margin
 *  - <b>MARGIN_DB_LO_B_VSA1</b>:   power (Db) in region LO_B at worst margin
 *  - <b>MASK_MARGIN_DB_LO_B_VSA1</b>:   power (Db) in region LO_B at worst margin (positive value means no mask violation)
 *  - <b>FREQ_MHZ_LO_A_VSA1</b>:    frequency (M Hz) in region LO_A at worst margin
 *  - <b>MARGIN_DB_LO_A_VSA1</b>:   power (Db) in region LO_A at worst margin
 *  - <b>MASK_MARGIN_DB_LO_A_VSA1</b>:   power (Db) in region LO_A at worst margin (positive value means no mask violation)
 *  - <b>FREQ_MHZ_UP_A_VSA1</b>:    frequency (M Hz) in region UP_A at worst margin
 *  - <b>MARGIN_DB_UP_A_VSA1</b>:   power (Db) in region UP_A at worst margin
 *  - <b>MASK_MARGIN_DB_UP_A_VSA1</b>:   power (Db) in region UP_A at worst margin (positive value means no mask violation)
 *  - <b>FREQ_MHZ_UP_B_VSA1</b>:    frequency (M Hz) in region UP_B at worst margin
 *  - <b>MARGIN_DB_UP_B_VSA1</b>:   power (Db) in region UP_B at worst margin
 *  - <b>MASK_MARGIN_DB_UP_B_VSA1</b>:   power (Db) in region UP_B at worst margin (positive value means no mask violation)
 *  - <b>FREQ_MHZ_UP_C_VSA1</b>:    frequency (M Hz) in region UP_C at worst margin
 *  - <b>MARGIN_DB_UP_C_VSA1</b>:   power (Db) in region UP_C at worst margin
 *  - <b>MASK_MARGIN_DB_UP_C_VSA1</b>:   power (Db) in region UP_C at worst margin (positive value means no mask violation)
 *  - <b>FREQ_MHZ_UP_D_VSA1</b>:    frequency (M Hz) in region UP_D at worst margin
 *  - <b>MARGIN_DB_UP_D_VSA1</b>:   power (Db) in region UP_D at worst margin
 *  - <b>MASK_MARGIN_DB_UP_D_VSA1</b>:   power (Db) in region UP_D at worst margin (positive value means no mask violation)
 *  - <b>VIOLATION_PERCENT_VSA1</b>:  Returns the percentage of points that violate the defined spectral mask over the whole range
 *  - <b>OBW_MHZ_VSA1</b>:          Occupied Bandwidth measurement in MHz
 *  - <b>OBW_FREQ_START_MHZ_VSA1</b>:  Start frequency for Occupied Bandwidth measurement in MHz
 *  - <b>OBW_FREQ_STOP_MHZ_VSA1</b>:  Stop frequency for Occupied Bandwidth measurement in MHz

Although IQ2010 has only one VSA, we keep '_VSA1' as part of the measurements, so that we will have the same naming convention when we come to IQnxn.
In addition to the above measurements, the following two measurements are also returned:
VIOLATION_PERCENT_VSA1: Returns the percentage of points that violate the defined spectral mask over the whole range.

 * \section revision_sec Revision History
 *
 * \subsection revision_1_8_68_20 1.8.68.20 IQ2010extension
 * Date: Oct 5, 2010
 * - Known issue:
 *  -# IQ2010EXT_FinalizeMultiSegmentWaveform() won't work if called the second times
 * - Modifications:
 *  -# Pre-trigger time is modified to be set from 10 us to 3 us
 *  -# total capture length equals to user defined capture length + pre-trigger time
 * - Features:
 *  -# Add POWER_LTS (power for long training symbols) to analysis type 
 * - Bug fixed:
 *  -# Fix tester re-connection issue
 *
 * \subsection revision_1_8_67_10 1.8.67.10 IQ2010extension
 * Date: Aug 3, 2010
 *   -# Initial version
 *
 */

/*! \file IQ2010Ext.h
 * \brief IQ2010Ext Library Interface functions 
 */ 

#pragma once

#ifdef IQ2010EXT_EXPORTS
	#define IQ2010EXT_API extern "C" __declspec(dllexport)
#else
	#define IQ2010EXT_API extern "C" __declspec(dllimport)
#endif

#include "iqapiDefines.h"
#include "iqapi.h"
#include "IQ2010ExtDefines.h"

#define IQ2010EXT_VERSION   ".35 (2012-3-12)"

/*-----------------------------------*
 * Version  *
 *-----------------------------------*/

//! Get Version Information of IQ2010 Extension
/*!
 * \param[out] buffer The buffer that will return the version information
 * \param[in] buf_size Indicates the size of the buffer
 * \return IQ2010EXT_ERR_OK if getting version for IQ2010Ext module version successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
 */
IQ2010EXT_API int  IQ2010EXT_GetVersion(char *buffer, int buf_size);

/*-----------------------------------*
 * Initialization and Error Handling *
 *-----------------------------------*/

 
//! Initializes IQ2010 Extension
/*!
 * \return IQ2010EXT_ERR_OK if initialzation is done successfully; otherwise call IQ2010EXT_GetLastErr() for detailed error message.
 * \remark This function has to be called right after IQ2010 software has been connected to the test system
 */
IQ2010EXT_API int  IQ2010EXT_Init(iqapiHndl *hndl);

//! Exit IQ2010 extension
/*!
 * \return IQ2010EXT_ERR_OK if exiting IQ2010 extension successfully; otherwise call IQ2010EXT_GetLastErr() for detailed error message.
 * \remark This function has to be called if user wants to switch the tester back to legacy mode after using IQ2010 extension.
 */
IQ2010EXT_API int  IQ2010EXT_Terminate();

//! Get the last error message
/*! \return last error message from the last failed operation. Should get it right after the operation failure. Otherwise, other failed operation will override the error message
*/
IQ2010EXT_API char *IQ2010EXT_GetLastErr();

/*----------------*
 * Mask Functions *
 *----------------*/
//! This function allows user to define the upper mask ( positive frequency side ) for HT20 or HT40
//! After setting the mask, user should define mask type to be WIFI_USER_DEFINE_MASK_HT20 or WIFI_USER_DEFINE_MASK_HT40 in IQ2010EXT_AnalyzeMaskMeasurement(...)
/*!
 * \param[in] isHT40Mask true if it is for HT40. false for other mask type
 * \param[in] s1FreqMHz frequency in MHz for the starting point of region UP_A
 * \param[in] s1MaskLevelDb mask level in dB for the starting point of region UP_A
 * \param[in] s2FreqMHz frequency in MHz for the starting point of region UP_B and ending point of region UP_A
 * \param[in] s2MaskLevelDb mask level in dB for the starting point of region UP_B and ending point of region UP_A
 * \param[in] s3FreqMHz frequency in MHz for the starting point of region UP_C and ending point of region UP_B
 * \param[in] s3MaskLevelDb mask level in dB for the starting point of region UP_C and ending point of region UP_B
 * \param[in] s4FreqMHz frequency in MHz for the starting point of region UP_D and ending point of region UP_C
 * \param[in] s4MaskLevelDb mask level in dB for the starting point of region UP_D and ending point of region UP_C
 * \return IQ2010EXT_ERR_OK if adding user defined mask type successfully; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.	
*/
IQ2010EXT_API int IQ2010EXT_AddUserDefinedUpperSegmentsMask(bool isHT40Mask, 
															double s1FreqMHz, double s1MaskLevelDb,
															double s2FreqMHz, double s2MaskLevelDb,
															double s3FreqMHz, double s3MaskLevelDb,
															double s4FreqMHz, double s4MaskLevelDb
															);

//! This function allows user to define the lower mask ( negative frequency side ) for HT20 or HT40
//! After setting the mask, user should define mask type to be WIFI_USER_DEFINE_MASK_HT20 or WIFI_USER_DEFINE_MASK_HT40 in IQ2010EXT_AnalyzeMaskMeasurement(...)
//! Note : If it is symmetrical to upper segment mask, no need to define the lower segment mask.
/*!
 * \param[in] isHT40Mask true if it is for HT40. false for other mask type
 * \param[in] s1FreqMHz frequency in MHz for the starting point of region LO_A
 * \param[in] s1MaskLevelDb mask level in dB for the starting point of region LO_A
 * \param[in] s2FreqMHz frequency in MHz for the starting point of region LO_B and ending point of region LO_A
 * \param[in] s2MaskLevelDb mask level in dB for the starting point of region LO_B and ending point of region LO_A
 * \param[in] s3FreqMHz frequency in MHz for the starting point of region LO_C and ending point of region LO_B
 * \param[in] s3MaskLevelDb mask level in dB for the starting point of region LO_C and ending point of region LO_B
 * \param[in] s4FreqMHz frequency in MHz for the starting point of region LO_D and ending point of region LO_C
 * \param[in] s4MaskLevelDb mask level in dB for the starting point of region UP_D and ending point of region UP_C
 * \return IQ2010EXT_ERR_OK if adding user defined mask type successfully; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.	
*/
IQ2010EXT_API int IQ2010EXT_AddUserDefinedLowerSegmentsMask(bool isHT40Mask, 
															double s1FreqMHz, double s1MaskLevelDb,
															double s2FreqMHz, double s2MaskLevelDb,
															double s3FreqMHz, double s3MaskLevelDb,
															double s4FreqMHz, double s4MaskLevelDb
															);

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
 * \param[in] rfPort RF port. Valid values are IQV_PORT_LEFT (RF1) and IQV_PORT_RIGHT (RF2).
 * \param[in] triggerLeveldBm Trigger level in dBm
*/
IQ2010EXT_API int IQ2010EXT_NewMultiAnalysisCapture(IQ2010EXT_DATA_RATE     dataRate,
                                                    double                  freqMHz,
                                                    double	                rmsPowerLeveldBm,
													int	                    skipPktCnt,
													IQV_PORT_ENUM           rfPort,
													double	                triggerLeveldBm);


//! Add multiple capture type with specified capture length and count. This function should be called after IQ2010EXT_NewMultiAnalysisCapture()
/*!
 * \param[in] analysisType  analysis type 
 * \param[in] captureLengthUs Capture length in us.                                         Max allowed: 1023 us
 * \param[in] captureCnt Capture count.                                                     Max allowed: 255
*/
IQ2010EXT_API int IQ2010EXT_AddMultiAnalysisCapture(IQ2010EXT_ANALYSIS_TYPE	    analysisType,
													int	    					captureLengthUs,  
													int	    					captureCnt);

//! VSA Multi-packet analysis capture. Should use it along with IQ2010EXT_FinishMultiAnalysisCapture(). The function should be called first.
/*!
 * \return IQ2010EXT_ERR_OK if the VSA capture is done successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
*/
IQ2010EXT_API int IQ2010EXT_InitiateMultiAnalysisCapture();

//! VSA multi-packet capture based on the IQ2010EXT_NewMultiAnalysisCapture() and IQ2010EXT_AddMultiAnalysisCapture(). The function should be called after IQ2010EXT_InitiateMultiAnalysisCapture()
/*!
 * \return IQ2010EXT_ERR_OK if the VSA capture is done successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
*/
IQ2010EXT_API int IQ2010EXT_FinishMultiAnalysisCapture();

 
//! VSA Multi-packet capture. Should use it along with IQ2010EXT_FinishMultiCapture(). The function should be called first.
/*!
 * \param[in] dataRate Datarate the DUT uses to transmit
 * \param[in] freqMHz Frequency (MHz) at which the DUT transmits
 * \param[in] rmsPowerLeveldBm Estimated DUT RMS power level (dBm) at the tester RF ports
 * \param[in] captureLengthUs Capture length in us.                                         Max allowed: 1023 us
 * \param[in] skipPktCnt Skip count before capture.                                         Max allowed: 255
 * \param[in] captureCnt Capture count.                                                     Max allowed: 255
 * \param[in] rfPort RF port. Valid values are IQV_PORT_LEFT (RF1) and IQV_PORT_RIGHT (RF2).
 * \param[in] triggerLeveldBm Trigger level in dBm
 *
 * \return IQ2010EXT_ERR_OK if the VSA capture is initiated successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
*/
IQ2010EXT_API int IQ2010EXT_InitiateMultiCapture(
                                            IQ2010EXT_DATA_RATE     dataRate,
                                            double                  freqMHz,
                                            double	                rmsPowerLeveldBm,
                                            int	                    captureLengthUs,  
                                            int	                    skipPktCnt,
                                            int	                    captureCnt,
                                            IQV_PORT_ENUM           rfPort,
                                            double	                triggerLeveldBm);

//! VSA Multi-packet Capture for HT40. Should use it along with IQ2010EXT_FinishMultiCapture(). The function should be called first.
/*!
 * \param[in] dataRate Datarate the DUT uses to transmit
 * \param[in] freqMHz Frequency (MHz) at which the DUT transmits
 * \param[in] rmsPowerLeveldBm Estimated DUT RMS power level (dBm) at the tester RF ports
 * \param[in] captureLengthUs Capture length in us.                                         Max allowed: 1023 us
 * \param[in] skipPktCnt Skip count before capture.                                         Max allowed: 255
 * \param[in] captureCnt Capture count.                                                     Max allowed: 255
 * \param[in] rfPort RF port. Valid values are IQV_PORT_LEFT (RF1) and IQV_PORT_RIGHT(RF2).
 * \param[in] triggerLeveldBm Trigger-level in dBm
 *
 * \return IQ2010EXT_ERR_OK if the VSA capture is initiated successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
*/
IQ2010EXT_API int IQ2010EXT_InitiateMultiCaptureHT40(
                                            IQ2010EXT_DATA_RATE     dataRate,
                                            double                  freqMHz,
                                            double	                rmsPowerLeveldBm,
                                            int	                    captureLengthUs,  
                                            int	                    skipPktCnt,
                                            int	                    captureCnt,
                                            IQV_PORT_ENUM           rfPort,
                                            double	                triggerLeveldBm);


//! VSA multi-packet capture. Should use it along with IQ2010EXT_InitiateMultiCapture(). The function should be called after IQ2010EXT_InitiateMultiCapture() or IQ2010EXT_InitiateMultiCaptureHT40().
/*!
 * \return IQ2010EXT_ERR_OK if the VSA capture is done successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
*/
IQ2010EXT_API int IQ2010EXT_FinishMultiCapture();

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
 * \return IQ2010EXT_ERR_OK if the 802.11 a/g analysis succeeded; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
 */
IQ2010EXT_API int IQ2010EXT_Analyze80211ag(IQV_PH_CORR_ENUM ph_corr_mode    =IQV_PH_CORR_SYM_BY_SYM, 
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
 * \return IQ2010EXT_ERR_OK if the 802.11n analysis is successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
 */
IQ2010EXT_API int IQ2010EXT_Analyze80211nSISO(int enablePhaseCorr            = 1, 
                                              int enableSymTimingCorr        = 1, 
                                              int enableAmplitudeTracking    = 0, 
                                              int decodePSDU                 = 0, 
                                              int enableFullPacketChannelEst = 0, 
                                              int packetFormat = 0);

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
 * \return IQ2010EXT_ERR_OK if the 802.11n analysis is successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
 */
IQ2010EXT_API int IQ2010EXT_Analyze80211nComposite(int enablePhaseCorr            = 1, 
                                                  int enableSymTimingCorr        = 1, 
                                                  int enableAmplitudeTracking    = 0, 
                                                  int enableFullPacketChannelEst = 0, 
                                                  char *referenceFile=NULL,
                                                  int packetFormat = 0);

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
 * \return IQ2010EXT_ERR_OK if the 802.11b analysis is successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
 */
IQ2010EXT_API int IQ2010EXT_Analyze80211b(IQV_EQ_ENUM eq_taps                   = IQV_EQ_OFF, 
                                          IQV_DC_REMOVAL_ENUM DCremove11b_flag  = IQV_DC_REMOVAL_OFF, 
                                          IQV_11B_METHOD_ENUM method_11b        = IQV_11B_STANDARD_TX_ACC);

//! Perform power analysis on Multi-packet Capture
/*!
 * \return IQ2010EXT_ERR_OK if the power analysis is successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
*/
IQ2010EXT_API int IQ2010EXT_AnalyzePower();

//! Perform power analysis on LTS (long training symbols). This function can be used for 11AG and 11N only (data rate to OFDM or MCS).
/*!
 * \return IQ2010EXT_ERR_OK if the power analysis is successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
*/
IQ2010EXT_API int IQ2010EXT_AnalyzePowerLts();

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
IQ2010EXT_API int IQ2010EXT_AnalyzeFFT(double NFFT          = 0, 
                                       double res_bwHz      = 100000, 
                                       char *window_type    = "blackmanharris");

//! Perform mask verification and get OBW based on the OBW percentage
/*!
 * \param [in] maskType         indicate what mask type should be applied to the captured data
 * \param [in] obwPercentage    OBW percentage. The valid range should be between 0 - 1
 * \return IQ2010EXT_ERR_OK if the mask measurement is successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
*/
IQ2010EXT_API int IQ2010EXT_AnalyzeMaskMeasurement(WIFI_MASK_TYPE maskType, double obwPercentage);

//! Perform power-ramp analysis for 11B
/*!
 * \return IQ2010EXT_ERR_OK if the mask measurement is successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
*/
IQ2010EXT_API int IQ2010EXT_AnalyzePowerRamp80211b();

//! Perform Bluetooth Analysis on Multi-packet Capture
/*!
 * \param[in] data_rate Bluetooth data rate, with the following options:
 *         - 1: 1Mbit/s BDR
 *         - 2: 2Mbit/s EDR
 *         - 3: 3Mbit/s EDR
 *		   - 4: 1Mbit/s Low Energy mode
 *         - 0: Auto detect
 * \param[in] analysis_type Specifies what type of analysis to perform.                 Default: 'All'. 
 *         Valid values are as follows:
 *         - PowerOnly
 *         - 20dbBandwidthOnly
 *         - PowerAndFreq
 *         - All  (This is the set default value)
 *		   - ACP  (only does the new ACP analysis)
 *		   - AllPlus  (performs all the analyses that are done by ‘All?plus the ACP analysis)
 * \return IQ2010EXT_ERR_OK if the bluetooth analysis is successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
 */
IQ2010EXT_API int IQ2010EXT_AnalyzeBluetooth(double data_rate, char *analysis_type="All");

//! Perform spectral analysis based on the selected spectral type
/*!
 * param [in] spectralType  indicate the spectral type to be applied to the captured data
 * \return IQ2010EXT_ERR_OK if the spectral measurement is successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
*/
IQ2010EXT_API int IQ2010EXT_AnalyzeSpectralMeasurement(IQ2010EXT_SPECTRAL_TYPE spectralType);


/*--------------*
 * RX Functions *
 *--------------*/

/*---------------------------------*
 * Multi-segment Waveform Handling *
 *---------------------------------*/
//! Start a new Multi-segment Waveform Creation
/*!
 *
 * \return IQ2010EXT_ERR_OK if successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
 *
 * \remark 
 *         -# After calling this function, you can then call IQ2010EXT_AddWaveform()
 */
IQ2010EXT_API int IQ2010EXT_NewMultiSegmentWaveform();

 
//! Add a Waveform to the Multi-segment Waveform. 
/*!
 * \param[in] modFile A .mod file that contains only one data rate
 * \param[out] waveformIndex The waveform index used by IQ2010EXT_RxPer() identifying this data rate
 *
 * \return IQ2010EXT_ERR_OK if successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
 *
 * \remark 
 *         -# You can only call this function after IQ2010EXT_NewMultiSegmentWaveform() and before IQ2010EXT_FinalizeMultiSegmentWaveform()
 *         - The maximum number of file can be added is 32
 */
IQ2010EXT_API int IQ2010EXT_AddWaveform(const char *modFile, unsigned int *waveformIndex );
 
//! Finalize the Multi-segment Waveform Creation and Upload to VSG Memory
/*!
 * \return IQ2010EXT_ERR_OK if successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
 * \remark 
 *         -# You cannot call IQ2010EXT_AddWaveform() anytime after calling this function
 */
IQ2010EXT_API int IQ2010EXT_FinalizeMultiSegmentWaveform();


//! Generate a wave form at VSG by the specified segment index with the packet count 
/*!
 * \param[in] waveformIndex index (base 0) to wave form (wave form is preloaded by IQ2010EXT_AddWaveform(...) ) to be played at VSG
 * \param[in] packetCount number of packets to be played. if 0, play continually until IQ2010EXT_VsgSingleSegmentStop() is called
 * \param[in] freqMHz Frequency (MHz) on which VSG operates
 * \param[in] rmsPowerLeveldBm RMS power level (dBm) at the tester RF ports
 * \param[in] rfPort The RF ports used for VSG. Valid values are IQV_PORT_LEFT (RF1) and IQV_PORT_RIGHT(RF2).
 * \return IQ2010EXT_ERR_OK if successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
*/
IQ2010EXT_API int IQ2010EXT_VsgSingleSegmentPlay(int waveformIndex, int packetCount, double freqMHz, double rmsPowerLeveldBm, IQV_PORT_ENUM rfPort);

//! After calling IQ2010EXT_VsgSingleSegmentPlay(...). Should call IQ2010EXT_VsgSingleSegmentStop() to stop the segment played
/*!
 * \return IQ2010EXT_ERR_OK if successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
*/
IQ2010EXT_API int IQ2010EXT_VsgSingleSegmentStop();

//! Generate a wave form that user selected at VSG 
/*!
 * \param[in] freqMHz Frequency (MHz) on which VSG operates
 * \param[in] rmsPowerLeveldBm RMS power level (dBm) at the tester RF ports
 * \param[in] rfPort The RF ports used for VSG. Valid values are IQV_PORT_LEFT (RF1) and IQV_PORT_RIGHT(RF2).
 * \param[in] sigFile the full path of waveform file.
 * \return IQ2010EXT_ERR_OK if successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
*/
IQ2010EXT_API int IQ2010EXT_VsgPlayBluetoothDirtyPacket(double freqMHz, double rmsPowerLeveldBm, IQV_PORT_ENUM rfPort, const char *sigFile = NULL);

//! IQ2010 ACK-based RX PER Test
/*!
 * \param[in] waveformIndex A wave form index (0 based) of the multi-segment wavefile, specifying which wave form should be used
 * \param[in] freqMHz Frequency (MHz) on which DUT operates
 * \param[in] powerLevelStartdBm The start of power level (dBm at IQ2010 RF ports)
 * \param[in] powerLevelStopdBm The stop of power level (dBm at IQ2010 RF ports)
 * \param[in] stepdB Power level step(dB).  Minimum step is 0.5dB.
 * \param[in] packetCount Number of packets to be transmitted for power level. Maximum number of packets is 32767.
 * \param[in] rfPort The RF ports used for RX PER testing. Valid values are IQV_PORT_LEFT (RF1) and IQV_PORT_RIGHT(RF2).
 * \param[in] ackPowerRmsdBm The DUT ACK RMS Power at the tester port
 * \param[in] ackTriggerLeveldBm The trigger level set to do ACK detection
 *
 * \return IQ2010EXT_ERR_OK if the RX PER is successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
 *
 * \remark 
 *         -# In most cases, the stop power level would be lower than the start power level, but it could be
 *            higher than the start power level.  In either way, step would be relative, meaning always positive.
 *         -# If only one power level is interested for RX PER, make the powerLevelStopdBm the same as powerLevelStartdBm.
 *         -# power level should be within [-10, -95] dBm
 */
IQ2010EXT_API int IQ2010EXT_RxPer(int               waveformIndex,
                                  double            freqMHz,
                                  double            powerLevelStartdBm, 
                                  double            powerLevelStopdBm, 
                                  double            stepdB,
                                  int               packetCount,
                                  IQV_PORT_ENUM     rfPort,
                                  double            ackPowerRmsdBm,
                                  double            ackTriggerLeveldBm
                                  );

/*--------------------------*
 * Getting result Functions *
 *--------------------------*/

//! Get the number of results for a particular measurement element (pointed to by indexResult) by measurement name
/*!
 * \param [in] measurementName         measurement name
 * \param [in] indexResult             index to a particular measurement element
 * \param [out] numberOfMeasurementElement return number of measurement element 
 * \return IQ2010EXT_ERR_OK if successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
*/
IQ2010EXT_API int IQ2010EXT_GetNumberOfMeasurementElements(char *measurementName, int indexResult, int *numberOfMeasurementElement);

//! Get the number of results for a particular measurement element (pointed to by indexResult) by measurement name
/*!
 * \param [in] measurementName         measurement name
 * \param [out] numberOfPerPowerLevel return number of per power level 
 * \return IQ2010EXT_ERR_OK if successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
*/
IQ2010EXT_API int IQ2010EXT_GetNumberOfPerPowerLevels(char *measurementName, int *numberOfPerPowerLevel);

//! Retrieve Analysis Results [Double] in average, minimum and maximum value
/*!
 * \param[in]  measurementName          The measurement name.  Please refer to \ref group_scalar_measurement "Vector Measurements" for all available measurement names.
 * \param[out] average Average value    Average value for all the result(s). Result set can be more than one if the specified capture count is larger than one when using multi-capture function 'IQ2010EXT_VsaMultiCapture(...)'
 * \param[out] minimum Minimum value    Minimum value of all the result(s)
 * \param[out] maximum Maximum value    Maximum value of all the result(s)
 *
 * \return IQ2010EXT_ERR_OK if successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
 */
IQ2010EXT_API int IQ2010EXT_GetDoubleMeasurements(char *measurementName,
                                                  double *average,
                                                  double *minimum,
                                                  double *maximum);


//! Retrieve Analysis Results [char] 
/*!
 * \param[in] measurementName The measurement name.  Please refer to \ref group_scalar_measurement "Vector Measurements" for all available measurement names.
 * \param[out] result Result in 'char' pointer type
 * \param[in] bufferSize Indicates the buffer size of result
 * \param[in] indexResult (starts with 0 as the first set of result) indicates which set of result to be retrieved. It has to be less than the number of capture set in IQ2010EXT_VsaMultiCapture(...)
 *
 * \return IQ2010EXT_ERR_OK if successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
 */
IQ2010EXT_API int IQ2010EXT_GetStringMeasurement(char *measurementName,
                                                 char *result,
                                                 int  bufferSize,
                                                 int indexResult = 0);


//! Retrieve Analysis Results [Double] in vector format ( more than one value in one particular result set as indicated by indexResult )
/*!
 * \param[in] measurementName The measurement name.  Please refer to \ref group_vector_measurement "Vector Measurements" for all available measurement names
 * \param[out] values Returns the real part of vector
 * \param[in] bufferLength Indicates the length of bufferReal and bufferImag (in number of element)
 * \param[out] *dataSize indicates the number of element in the result. If the dataSize is greater than bufferLength, then only the data with bufferLength will be filled up in the buffer
 * \param[in] indexResult (starts with 0 as the first set of result) indicates which set of result to be retrieved. It has to be less than the number of capture set in IQ2010EXT_VsaMultiCapture(...)
 *
 * \return IQ2010EXT_ERR_OK if successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
 */
IQ2010EXT_API int IQ2010EXT_GetVectorMeasurement(char *measurementName, double values[], int bufferLength, int *dataSize, int indexResult = 0);


//! Retrieve Analysis Results [int] 
/*!
 * \param[in] measurementName The measurement name.  Please refer to \ref group_vector_measurement "Vector Measurements" for all available measurement names
 * \param[out] result Result in 'int' type
 * \param[in] indexResult (starts with 0 as the first set of result) indicates which set of result to be retrieved. It has to be less than the number of capture set in IQ2010EXT_VsaMultiCapture(...)
*/
IQ2010EXT_API int IQ2010EXT_GetIntMeasurement(char *measurementName, int *result, int indexResult = 0);

//! Save the captured data to file(s) 
/*!
 * \param[in] fileName file name to be used for saving the captured data.
 *                     the file name will be modified with '_' and the index of the capture. Like fileName_0.sig for the first capture, fileName_1.sig for the second capture...etc
 * \param[in] specificCaptureIndex  Less than 0 ( default is -1) means saving all captured data (depends on captureCnt specified) into file(s) with different in capture index as shown above in file name.
 *                                  Greater than or equal 0 means saving only that specific captured data into a file.
 *                                  For ex. : if the captureCnt is 100, the valid range for user to set 'specificCaptureIndex' is from 0 to 99
 * \return IQ2010EXT_ERR_OK if successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
*/
IQ2010EXT_API int IQ2010EXT_SaveCapture(char *fileName, int specificCaptureIndex = -1);

//! Set capture time out ( same as trigger time out)
/*!
 * \param[in] captureTimeOut capture time out specified in second
 * \return IQ2010EXT_ERR_OK if successful; otherwise, call IQ2010EXT_GetLastErr() for detailed error message.
*/
IQ2010EXT_API int IQ2010EXT_SetCaptureTimeOut(double captureTimeOut);

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
 *
 *
 * \section analysis_80211ag Measurement Names for IQ2010EXT_Analyze80211ag() and IQ2010EXT_AnalyzeSpectralMeasurement() with spectral type 'IQ2010EXT_SPECTRAL_11AG'
 *  - <b>evmAll</b>:  EVM for entire frame.   
 *  - <b>evmData</b>:  EVM for data part of the frame.   
 *  - <b>evmPilot</b>:  EVM of all pilot part of the frame.   
 *  - <b>codingRate</b>:  Coding Rate in the capture.   
 *  - <b>freqErr</b>:  Frequency Error in the capture.   
 *  - <b>clockErr</b>:  Symbol Clock Error in the capture.   
 *  - <b>ampErr</b>:  IQ Match Amplitude Error in the capture.   
 *  - <b>ampErrDb</b>:  IQ Match Amplitude Error in dB in the capture.   
 *  - <b>phaseErr</b>:  IQ Match Phase Error in the capture.  
 *  - <b>rmsPhaseNoise</b>:  Frequency RMS Phase Noise in rms in the capture.   
 *  - <b>rmsPowerNoGap</b>:  RMS Power No Gap in the capture.   
 *  - <b>rmsPower</b>:  RMS Power in the capture.  
 *  - <b>pkPower</b>:  Peak power in the capture.   
 *  - <b>rmsMaxAvgPower</b>:  Average max RMS power in the capture.   
 *  - <b>psduCrcFail</b>:  1 = PSDU CRC Check Failed, 0 = PSDU CRC Check Passed.   
 *  - <b>plcpCrcPass</b>:  1 = PLCP CRC Check Passed, 0 = PLCP CRC Check Failed.   
 *  - <b>dataRate</b>:  Data rate in Mbps in the capture.   
 *  - <b>numSymbols</b>:  Number of Symbols in the capture.   
 *  - <b>numPsduBytes</b>:  Number of bytes in PSDU.   
 *  - <b>SUBCARRIER_LO_B_VSA1</b>:  Subcarrier number for lower region B ( tester VSA 1 ).   
 *  - <b>VALUE_DB_LO_B_VSA1</b>:  Value at worst margin for lower region B (tester VSA 1).   
 *  - <b>SUBCARRIER_LO_A_VSA1</b>:  Subcarrier number for lower region A ( tester VSA 1 ).   
 *  - <b>VALUE_DB_LO_A_VSA1</b>:  Value at worst margin for lower region A (tester VSA 1).   
 *  - <b>SUBCARRIER_UP_A_VSA1</b>:  Subcarrier number for upper region A ( tester VSA 1 ).   
 *  - <b>VALUE_DB_UP_A_VSA1</b>:  Value at worst margin for upper region A (tester VSA 1).   
 *  - <b>SUBCARRIER_UP_B_VSA1</b>:  Subcarrier number for upper region B ( tester VSA 1 ).   
 *  - <b>VALUE_DB_UP_B_VSA1</b>:  Value at worst margin for upper region B (tester VSA 1).   
 *  - <b>LO_LEAKAGE_DBR_VSA1</b>:  LO leakage (tester VSA 1).   
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
 *  - <b>SUBCARRIER_LO_B_VSA1</b>:  Subcarrier number for lower region B ( tester VSA 1 ).   
 *  - <b>VALUE_DB_LO_B_VSA1</b>:  Value at worst margin for lower region B (tester VSA 1).   
 *  - <b>SUBCARRIER_LO_A_VSA1</b>:  Subcarrier number for lower region A ( tester VSA 1 ).   
 *  - <b>VALUE_DB_LO_A_VSA1</b>:  Value at worst margin for lower region A (tester VSA 1).   
 *  - <b>SUBCARRIER_UP_A_VSA1</b>:  Subcarrier number for upper region A ( tester VSA 1 ).   
 *  - <b>VALUE_DB_UP_A_VSA1</b>:  Value at worst margin for upper region A (tester VSA 1).   
 *  - <b>SUBCARRIER_UP_B_VSA1</b>:  Subcarrier number for upper region B ( tester VSA 1 ).   
 *  - <b>VALUE_DB_UP_B_VSA1</b>:  Value at worst margin for upper region B (tester VSA 1).   
 *  - <b>LO_LEAKAGE_DBR_VSA1</b>:  LO leakage (tester VSA 1).   
 * 
 * \section analysis_80211b Measurement Names for IQ2010EXT_Analyze80211b()
 *  - <b>evmPk</b>:  EVM peak value of all symbols in the capture.   
 *  - <b>evmAll</b>:  EVM for entire frame in the capture.   
 *  - <b>evmInPreamble</b>:  EVM in the preamble of the capture.   
 *  - <b>evmInPsdu</b>:  EVM in the PSDU of the capture.   
 *  - <b>freqErr</b>:  Frequency Error in the capture.   
 *  - <b>clockErr</b>:  Symbol Clock Error in the capture.   
 *  - <b>ampErr</b>:  IQ Match Amplitude Error in the capture.   
 *  - <b>ampErrDb</b>:  IQ Match Amplitude Error in the capture; specified in dB   
 *  - <b>phaseErr</b>:  IQ Match Phase Error in the capture.  
 *  - <b>rmsPhaseNoise</b>:  Frequency RMS Phase Noise in the capture.   
 *  - <b>rmsPowerNoGap</b>:  RMS Power No Gap in the capture.   
 *  - <b>rmsPower</b>:  RMS Power in the capture.   
 *  - <b>pkPower</b>:  Peak power in the capture.   
 *  - <b>rmsMaxAvgPower</b>:  Average max RMS power in the capture.   
 *  - <b>bitRate</b>:  Bit Rate, see 802.11b standard.  
 *  - <b>modType</b>:  Mod Type, see 802.11b standard.  
 *  - <b>valid</b>:  Checks if the capture is valid.  
 *  - <b>P_av_each_burst</b>:  Average power of each burst in the capture.  
 *  - <b>P_pk_each_burst</b>:  Peak power of each burst in the capture.  
 *  - <b>P_av_all</b>:  Average power in the capture.  
 *  - <b>P_peak_all</b>:  Peak power in the capture.  
 *  - <b>P_av_no_gap_all</b>:  Average power with no gap in the capture.  
 *  - <b>P_av_all_dBm</b>:  Average power in the capture; specified in dBm.  
 *  - <b>P_peak_all_dBm</b>:  Peak power in the capture; specified in dBm.  
 *  - <b>P_av_no_gap_all_dBm</b>:  Average power with no gap in the capture; specified in dBm  
 *  - <b>lockedClock</b>:  Locked Clock, see 802.11b standard.  
 *  - <b>plcpCrcFail</b>:  1 = PLCP CRC Check Failed, 0 = PLCP CRC Check Passes.   
 *  - <b>psduCrcFail</b>:  1 = PSDU CRC Check Failed, 0 = PSDU CRC Check Passes.   
 *  - <b>longPreamble</b>:  1 = Long Preamble, 0 = Short Preamble.   
 *  - <b>bitRateInMHz</b>:  Bit rate in Mhz in the capture.  
 *  - <b>loLeakageDb</b>:  LO leakage in dBc.  
 *  - <b>MaxfreqErr</b>:  Maximum frequency error.  
 * 
 * \section analysis_Power Measurement Names for IQ2010EXT_AnalyzePower()
 *  - <b>valid</b>:  Indicates valid results (1 valid, 0 invalid).   
 *  - <b>P_av_each_burst</b>:  Average power of each burst in the capture.   
 *  - <b>P_pk_each_burst</b>:  Peak power of each burst in the capture.   
 *  - <b>P_av_all</b>:  Average power of whole capture.   
 *  - <b>P_peak_all</b>:  Peak power of whole capture.   
 *  - <b>P_av_no_gap_all</b>:  Average power with no gap in the capture.   
 *  - <b>P_av_all_dBm</b>:  Average power of whole capture; specified in dBm.   
 *  - <b>P_peak_all_dBm</b>:  Peak power of whole capture; specified in dBm.   
 *  - <b>P_av_no_gap_all_dBm</b>:  Average power, not counting gaps in dBm.  
 *  - <b>POWER_LTS_RMS_DBM</b>:  Average power, not counting gaps in dBm.  The result exists only if the data rate is OFDM or MCS. 
 * 
 * \section analysis_Power LTS Measurement Names for IQ2010EXT_AnalyzePowerLts(). This function can be used for 11AG and 11N only (data rate to OFDM or MCS).
 *  - <b>POWER_LTS_RMS_DBM</b>:  Average power, not counting gaps in dBm.  
 *
 * \section analysis_FFT Measurement Names for IQ2010EXT_AnalyzeFFT()
 *  - <b>valid</b>:  Returns (value) whether results are valid or not. 1 valid, 0 invalid.   
 *  - <b>length</b>:  Returns length of X and Y Vectors above.   
 * 
 * \section analysis_Bluetooth Measurement Names for IQ2010EXT_AnalyzeBluetooth()
 *  - <b>dataRateDetect</b>:  Detects which data rate is in capture.   
 *  - <b>valid</b>:  Flag indicating whether or not the power portion of the Bluetooth analysis was successful (1), or not (0).   
 *  - <b>bandwidth20dB</b>:   20 dB bandwidth value Hz.   
 *  - <b>deltaF1Average</b>:  The measurement result for deltaF1Avg as specified in BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2. Requires 00001111 data pattern. Results in Hz.   
 *  - <b>deltaF2Max</b>:  The measurement result for deltaF2Max as specified in BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2. Requires alternating data pattern. Results in Hz.   
 *  - <b>deltaF2Average</b>:  The measurement result for deltaF2Avg as specified in BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2. Requires alternating data pattern. Results in Hz.   
 *  - <b>deltaF2MaxAccess</b>:  Similar to the measurement result for deltaF2Max as specified in BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2. Results measured from Access data. Results in Hz.   
 *  - <b>deltaF2AvAccess</b>:  Similar to the measurement result for deltaF2Avg as specified in BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2. Results measured from Access data. Results in Hz.   
 *  - <b>EdrEVMAv</b>:  RMS Differential EVM value (EDR only).  
 *  - <b>EdrEVMpk</b>:  Peak Differential EVM value (EDR only).   
 *  - <b>EdrEVMvalid</b>:  Indicates validity of EDR EVM Measurements.   
 *  - <b>EdrPowDiffdB</b>:  Relative power of EDR section to FM section of packet, in dB.   
 *  - <b>freq_deviation</b>:  Similar to the measurement result for deltaF1Avg as specified in BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2. 
 *					  Results measured from Header data. Results in Hz.   
 *  - <b>freq_deviationpktopk</b>:  Peak to Peak Frequency Deviation, in Hz during header.   
 *  - <b>freq_estHeader</b>:  Estimates the Frequency Offset during the Header in Hz.   
 *  - <b>EdrFreqExtremeEdronly</b>:  Reports the extreme value of the frequency variation during DPSK in Hz. Does not include the offset during the header.   
 *  - <b>EdrprobEVM99pass</b>:  The percentage of symbols with EVM below the threshold. Threshold for 2 Mbps is 0.3 for 3 Mbps is 0.2.   
 *  - <b>EdrEVMvsTime</b>:  Max DEVM Average as specified in BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2.   
 *  - <b>validInput</b>:  Indicates whether or not the input wave was valid.  
 *  - <b>maxfreqDriftRate</b>:  1 Mbps only, takes the maximum drift over specified time interval.   
 *  - <b>payloadErrors</b>:  Reports the number of data errors in Payload. Not counting CRC. If -1, this value has not been calculated. 
 *					 If larger negative number, it reports the length of the payload data vector. This happens when the length of the payload vector is shorter than the length indicated in the payload header.   
 * 
 * \section analysis_Bluetooth low energy Measurement Names for IQ2010EXT_AnalyzeBluetooth()
 *  - <b>leFreqOffset</b>: Initial Frequency Offset calculated from preamble (f0 inTRM-LE/CA/05/C). Valid for any LE packet.
 *  - <b>leDeltaF1Avg</b>: Delta F1 Average as defined in TRM-LE/CA/05/C. Valid only if LE payload has 00001111 pattern.
 *  - <b>leDeltaF2Max</b>: Sequence of Delta F2 Max as defined in TRM-LE/CA/05/C. Valid only if LE payload has 10101010 pattern.
 *  - <b>leDeltaF2Avg</b>: Average value of LEDaltaF2Max, i.e. delta F2 Average as defined in TRM-LE/CA/05/C.  Valid only if LE payload has 10101010 pattern.
 *  - <b>leBelow185F2Max</b>: Number of symbols with deviation below 185 kHz
 *  - <b>leFn</b>: Average frequency over 8 symbols, as defined in TRM-LE/CA/06/C. Valid only if LE payload has 10101010 pattern.
 *  - <b>leFnMax</b>: Maximum value of the absolute value of LEFn. Valid only if LE payload has 10101010 pattern.
 *  - <b>leDeltaF0FnMax</b>: Maximum value of |f0 – fn|, with n = 2, 3, …. Valid only if LE payload has 10101010 pattern.
 *  - <b>leDeltaF1F0</b>: Absolute value of f1 – f0. Valid only if LE payload has 10101010 pattern.
 *  - <b>leDeltaFnFn_5Max</b>: Maximum value of |fn – fn-5|, with n = 6, 7, …. Valid only if LE payload has 10101010 pattern.
 *  - <b>leFreqDevSyncAv</b>: Average freq. deviation during synch. Measured at single sample per symbol after frequency offset during pre-amble has been removed. Valid for any LE packet.
 *  - <b>lePduLength</b>: PDU length in bytes. Valid for any LE packet
 *  - <b>leIsCrcOk</b>: Is 1 if CRC is valid, else 0. Valid output for any LE packet
 *  - <b>leValid</b>: Is 1 if all calculations are valid, else 0. Valid output for any LE packet
 *  - <b>leMaxPowerDbm</b>: Same format as earlier outputs for adjacent channel power. See earlier documentation for maxPowerACPdBm.
 *  - <b>leMsg</b>: Cell array of messages. Valid output for any LE packet
 *  - <b>lePreambleSeq</b>: Preamble bits 0/1 as defined in 7.2
 *  - <b>leSyncWordSeq</b>: Synch Word bits as defined in 7.2
 *  - <b>lePduHeaderSeq</b>: PDU Header of the test packet as defined in 7.2 
 *  - <b>lePduLengthSeq</b>: PDU Length of the test packet as defined in 7.2
 *  - <b>lePayloadSeq</b>: PDU of the test packet as defined in 7.2
 *  - <b>leCrcSeq</b>: CRC of the PDU of test packet as defined in 7.2
 *   
 * \section analysis_mask Measurement Names for IQ2010EXT_AnalyzeMaskMeasurement() with mask type WIFI_11B
 *  - <b>FREQ_MHZ_LO_B_VSA1</b>:    frequency (M Hz) in region LO_B at worst margin
 *  - <b>MARGIN_DB_LO_B_VSA1</b>:   power (Db) in region LO_B at worst margin 
 *  - <b>MASK_MARGIN_DB_LO_B_VSA1</b>:   power (Db) in region LO_B at worst margin (positive value means no mask violation)
 *  - <b>FREQ_MHZ_LO_A_VSA1</b>:    frequency (M Hz) in region LO_A at worst margin
 *  - <b>MARGIN_DB_LO_A_VSA1</b>:   power (Db) in region LO_A at worst margin
 *  - <b>MASK_MARGIN_DB_LO_A_VSA1</b>:   power (Db) in region LO_A at worst margin (positive value means no mask violation)
 *  - <b>FREQ_MHZ_UP_A_VSA1</b>:    frequency (M Hz) in region UP_A at worst margin
 *  - <b>MARGIN_DB_UP_A_VSA1</b>:   power (Db) in region UP_A at worst margin
 *  - <b>MASK_MARGIN_DB_UP_A_VSA1</b>:   power (Db) in region UP_A at worst margin (positive value means no mask violation)
 *  - <b>FREQ_MHZ_UP_B_VSA1</b>:    frequency (M Hz) in region UP_B at worst margin
 *  - <b>MARGIN_DB_UP_B_VSA1</b>:   power (Db) in region UP_B at worst margin
 *  - <b>MASK_MARGIN_DB_UP_B_VSA1</b>:   power (Db) in region UP_B at worst margin (positive value means no mask violation)
 *  - <b>VIOLATION_PERCENT_VSA1</b>:  Returns the percentage of points that violate the defined spectral mask over the whole range
 *  - <b>OBW_MHZ_VSA1</b>:          Occupied Bandwidth measurement in MHz
 *  - <b>OBW_FREQ_START_MHZ_VSA1</b>:  Start frequency for Occupied Bandwidth measurement in MHz
 *  - <b>OBW_FREQ_STOP_MHZ_VSA1</b>:  Stop frequency for Occupied Bandwidth measurement in MHz
*
 * \section analysis_mask1 Measurement Names for IQ2010EXT_AnalyzeMaskMeasurement() with mask type WIFI_11AG, WIFI_11N_HT20, WIFI_USER_DEFINE_MASK_HT20, WIFI_11N_HT40 and WIFI_USER_DEFINE_MASK_HT40
 *  - <b>FREQ_MHZ_LO_D_VSA1</b>:    frequency (M Hz) in region LO_D at worst margin
 *  - <b>MARGIN_DB_LO_D_VSA1</b>:   power (Db) in region LO_D at worst margin
 *  - <b>MASK_MARGIN_DB_LO_D_VSA1</b>:   power (Db) in region LO_D at worst margin (positive value means no mask violation)
 *  - <b>FREQ_MHZ_LO_C_VSA1</b>:    frequency (M Hz) in region LO_C at worst margin
 *  - <b>MARGIN_DB_LO_C_VSA1</b>:   power (Db) in region LO_C at worst margin
 *  - <b>MASK_MARGIN_DB_LO_C_VSA1</b>:   power (Db) in region LO_C at worst margin (positive value means no mask violation)
 *  - <b>FREQ_MHZ_LO_B_VSA1</b>:    frequency (M Hz) in region LO_B at worst margin
 *  - <b>MARGIN_DB_LO_B_VSA1</b>:   power (Db) in region LO_B at worst margin
 *  - <b>MASK_MARGIN_DB_LO_B_VSA1</b>:   power (Db) in region LO_B at worst margin (positive value means no mask violation)
 *  - <b>FREQ_MHZ_LO_A_VSA1</b>:    frequency (M Hz) in region LO_A at worst margin
 *  - <b>MARGIN_DB_LO_A_VSA1</b>:   power (Db) in region LO_A at worst margin
 *  - <b>MASK_MARGIN_DB_LO_A_VSA1</b>:   power (Db) in region LO_A at worst margin (positive value means no mask violation)
 *  - <b>FREQ_MHZ_UP_A_VSA1</b>:    frequency (M Hz) in region UP_A at worst margin
 *  - <b>MARGIN_DB_UP_A_VSA1</b>:   power (Db) in region UP_A at worst margin
 *  - <b>MASK_MARGIN_DB_UP_A_VSA1</b>:   power (Db) in region UP_A at worst margin (positive value means no mask violation)
 *  - <b>FREQ_MHZ_UP_B_VSA1</b>:    frequency (M Hz) in region UP_B at worst margin
 *  - <b>MARGIN_DB_UP_B_VSA1</b>:   power (Db) in region UP_B at worst margin
 *  - <b>MASK_MARGIN_DB_UP_B_VSA1</b>:   power (Db) in region UP_B at worst margin (positive value means no mask violation)
 *  - <b>FREQ_MHZ_UP_C_VSA1</b>:    frequency (M Hz) in region UP_C at worst margin
 *  - <b>MARGIN_DB_UP_C_VSA1</b>:   power (Db) in region UP_C at worst margin
 *  - <b>MASK_MARGIN_DB_UP_C_VSA1</b>:   power (Db) in region UP_C at worst margin (positive value means no mask violation)
 *  - <b>FREQ_MHZ_UP_D_VSA1</b>:    frequency (M Hz) in region UP_D at worst margin
 *  - <b>MARGIN_DB_UP_D_VSA1</b>:   power (Db) in region UP_D at worst margin
 *  - <b>MASK_MARGIN_DB_UP_D_VSA1</b>:   power (Db) in region UP_D at worst margin (positive value means no mask violation)
 *  - <b>VIOLATION_PERCENT_VSA1</b>:  Returns the percentage of points that violate the defined spectral mask over the whole range
 *  - <b>OBW_MHZ_VSA1</b>:          Occupied Bandwidth measurement in MHz
 *  - <b>OBW_FREQ_START_MHZ_VSA1</b>:  Start frequency for Occupied Bandwidth measurement in MHz
 *  - <b>OBW_FREQ_STOP_MHZ_VSA1</b>:  Stop frequency for Occupied Bandwidth measurement in MHz
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
 *  - <b>hhEst</b>:  64-element complex vector that represents the FFT output of the two long symbols in the PLCP preamble of the OFDM signal.   
 *  - <b>psdu</b>:  The PSDU data. 0/1 values. Includes the MAC Header and the FCS, if present .   
 *  - <b>startPointers</b>:  Provides the approximate starting locations of each packet detected in the input data. 
 *					 If no packet detected, this is an empty vector.   
 *  - <b>plcp</b>:  PLCP (binary) data as 1/0s.   
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
 *  - <b>evmInPlcp</b>: Evm in PLCP.   
 *  - <b>evmErr</b>: Evm error.   
 *  - <b>loLeakageDb</b>: LO/DC Leakage [dBc].   
 * 
 * \section analysis_FFT Measurement Names for IQ2010EXT_AnalyzeFFT()
 *  - <b>x</b>: X-axis values, typically frequency. Vector is returned.   
 *  - <b>y</b>: Y-axis values, power in dBm. Vector is returned.   
 * 
 * \section analysis_PowerRamp80211b Measurement Names for IQ2010EXT_AnalyzePowerRamp80211b()
 *  - <b>on_mask_x</b>: Power on ramp mask x-axis.   
 *  - <b>off_mask_x</b>: Power off ramp mask x-axis.   
 *  - <b>on_mask_y</b>: Power on ramp mask y-axis.   
 *  - <b>off_mask_y</b>: Power off ramp mask y-axis.   
 *  - <b>on_power_inst</b>: Instant power on ramp.   
 *  - <b>off_power_inst</b>: Instant power off ramp.   
 *  - <b>on_power_peak</b>: Power on ramp peak values.   
 *  - <b>off_power_peak</b>: Power off ramp peak values.   
 *  - <b>on_time_vect</b>: Power on ramp time vector.  
 *  - <b>off_time_vect</b>: Power off ramp time vector.   
 *  - <b>on_time</b>: Ramp on time.   
 *  - <b>off_time</b>: Ramp off time.   
 * 
 * \section analysis_Bluetooth Measurement Names for IQ2010EXT_AnalyzeBluetooth()
 *  - <b>P_av_each_burst</b>: Average power of each burst.   
 *  - <b>P_pk_each_burst</b>: Peak power of each burst.   
 *  - <b>complete_burst</b>: Vector indicating whether or not each burst is complete. 
 *					  All elements are either (1 beginning and end transitions detected), or (0 burst is missing either beginning or end transition).  
 *					   
 *  - <b>start_sec</b>: Starting time of each burst, in seconds.   
 *  - <b>stop_sec</b>: End time of each burst, in seconds.   
 *  - <b>freq_est</b>: Initial freq offset of each burst detected, in Hz.   
 *  - <b>freqEstPacketPointer</b>: Pointers to which packet is pointed to in each element of freq_est.   
 *  - <b>freq_drift</b>: Initial freq carrier drift of each burst detected, in Hz.   
 *  - <b>freqDriftPacketPointer</b>: Pointers to which packet is pointed to in each element of freq_drift.  
 *  - <b>freqDeviationPointer</b>: Pointers to which packet is pointed to in each element of the above two measurements.   
 *  - <b>EdrFreqvsTime</b>: Enhanced data rate for frequency versus time.   
 *  - <b>EdrExtremeOmegaI0</b>: Extreme value of (Omega_0 + Omega_i).   
 * 
 * \section analysis_mask Measurement Names for IQ2010EXT_AnalyzeMaskMeasurement() with mask type WIFI_11B, WIFI_11AG, WIFI_11N_HT20 and WIFI_11N_HT40
 *  - <b>x</b>: X-axis values, typically frequency. Vector is returned.   
 *  - <b>y</b>: Y-axis values, power in dBm. Vector is returned.  
 *
 * \section Rx per Measurement names for IQ2010EXT_RxPer()
 *  - <b>powerLeveldBm</b>:  powerLeveldBm the actual power levels for each enabled port at each power level
 *  - <b>ackRespMaxUs</b>:  ackRespMaxUs maximmum ack response time in us
 *  - <b>ackRespMinUs</b>:  ackRespMinUs minimum ack response time in us
 *  - <b>perPercent</b>:  perPercent the PER result at each power level (%)
 *
 */
