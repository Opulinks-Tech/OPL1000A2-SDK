/*!
    \file iqapi.h
*/
#ifndef _IQAPI_H_
#define _IQAPI_H_

// ****************************************************************
// Version Information
// ****************************************************************
#define IQAPI_VERSION                        "1.9.0.5"
#define IQAPI_RELEASE_DATE                   "(Dec 5, 2013)"

#define LPC_VERSION_TO_MATCH                 "1.0.20"
#define FW_VERSION_TO_MATCH                  "0.4.71"
#define FW_GF_INTERFACE_VERSION_TO_MATCH     "0.51.11"
#define FW_GF_VERSION_TO_MATCH               "1.0.3"
#define FPGA_VSA_VERSION_TO_MATCH            "091109A0"
#define FPGA_VSG_VERSION_TO_MATCH            "033109B0"
#define FPGA_FL_VERSION_TO_MATCH             "022609C0"
#define FPGA_GPS_FM_RF_VERSION_TO_MATCH      "122308D0"
#define FPGA_GPS_FM_BB_VERSION_TO_MATCH      "100609E0"

#include "iqapiDefines.h"

// ****************************************************************
// Internal Header files
// ****************************************************************
#ifdef IQAPI_USE_INTERNAL_FILES
//	#include "iqapi_lp_private.h"
    #include "A21Control.h"
#endif // IQAPI_USE_INTERNAL_FILES


// ****************************************************************
// Include Files
// ****************************************************************
//#include <windows.h>
#include <stdlib.h>
#ifdef WIN32
#pragma warning( disable : 4251 )
#endif
#include "iqapiHndlBase.h"
#include "iqapiHndlMatlab.h"
#include "DualHeadBase.h"

enum WHAT_VERSION_TO_CHECK
{
   IQV_LEGACY_AND_GF_VERSION,    // Check version for WIFI/BT/WiMax and GPS/FM
   IQV_LEGACY_VERSION_ONLY,      // Check version for WIFI/BT/WiMax 
   IQV_GF_VERSION_ONLY,          // Check version for GPS/FM   
   IQV_LEGACY_VERSION_ONLY_HW18   // Check version for WIFI/BT/WiMax. HW 18X doesn't contain Flash FPGA. No need to check it
};


#ifdef _INCLUDE_GF_
	#include "iqapiGPS.h"
	#include "iqapiGlonass.h"
	#include "iqapiFM.h"
#endif // _INCLUDE_GF_

#ifdef _INCLUDE_NFC_
   #include "iqapiNFC.h"
#endif // _INCLUDE_NFC_

class CA21Control;



//#include "iqapiExternalDevices.h"

// ****************************************************************
// Internal define statement
// ****************************************************************
#define     SERIAL_NUMBER_LEN          8
#define     NUMBER_OF_FREQ_BAND        8

// ****************************************************************
// Global Function Declarations
// ****************************************************************

//! Poly Fit
IQ_API int iqapiGetPolyFitMax(int order, int numElements, double xValues[], double yValues[], double *maxXValue, double *maxYValue);

//! Initializes API libraries
/*!
Initializes the API libraries and Matlab Run Time. Call this function once(and only once) before any other application that uses this API is called.
*/
IQ_API int iqapiInit(bool enableJvmAndJit = true);

//! Terminates API libraries
/*!
Terminates the API libraries and Matlab Runtime. Call this function once (and only once) after each application that uses the API is run.
*/
IQ_API int iqapiTerm(void);

//! Clear connection status, etc.
/*!
Causes the API to clear the current state of all IQapi connections. Call this function if you have to call the \c ConInit function for more than once in an application. You must call this function before you call the \c ConInit function for the second time.
*/ 
IQ_API int iqapiClearAll(void);

//! Plot Data
/*!
Plots data in a Matlab Figure window. 

The function takes the following inputs:

\param figNum
    Indicates the non-zero figure number. If the figure number does not exist, it will be created.
\param x
    Indicates the pointer to the data for the x-axis. This value can be set to NULL. If this value is set to NULL, then x will be the index of y.
\param y
    Indicates the pointer to the data for the y-axis. This value cannot be set to NULL.
\param length
    Indicates the length of the data to which the y axis points to. This value also indicates the length of data to which the x axis points to, if the value of x is not NULL.
\param title
    Indicates the NULL-terminated char string that points to the name of the plot.
\param xtitle
    Indicates the NULL-terminated char string that points to the name on the x-axis of the plot.
\param ytitle
    Indicates the NULL-terminated char string that points to the name on the y-axis of the plot.
\param plotArgs
    Indicates the NULL-terminated char string that is created from an element comprised of any or all of the data contained in each of the following three columns, and has a value of NULL for default options:
        b 	blue	\n
        g 	green	\n
        r 	red	\n
        c 	cyan	\n
        m 	magenta	\n
        y 	yellow	\n
        k 	black	\n
        . 	point	\n
        o 	circle	\n
        x 	x-mark	\n
        + 	plus	\n
        * 	star	\n
        s 	square	\n
        d 	diamond	\n
        v 	triangle (down)	\n
        ^ 	triangle (up)	\n
        < 	triangle (left)	\n
        > 	triangle (right)	\n
        p 	pentagram	\n
        h 	hexagram	\n
        - 	solid	\n
        : 	dotted	\n
        -. 	dashdot	\n
        -- 	dashed 	\n
\param keepPlot 
    It can be 0 or 1. \n
    For keepPlot = 0, it will erase the previous plot before plotting the new data. \n 
    For keepPlot = 1, it will plot the new data on the same plot as the previous plot. \n 
*/
IQ_API int iqapiPlot(int figNum, double *x, double *y, int length, char *plotArgs, char *title, char *xtitle, char *ytitle, int keepPlot=0);

//! Get Version Information
/*!
Retrieves version information for various parts of the API. This function copies a maximum of \c nMaxChars into the buffer that the \c versionStr parameter points to.
*/
IQ_API int iqapiVersion(char *versionStr, int nMaxChars);


// ****************************************************************
// API Classes
// ****************************************************************

//! Specifies the analysis parameters for modulation multi-wave
class IQ_API iqapiModulationMultiWave : public iqapiModulationWave
{
public:
   iqapiModulationMultiWave() {compensationSinc = IQV_COMPENSATION_SINC; } ;
   virtual ~iqapiModulationMultiWave() {};

   char waveName[256];                                      // optional: define the name of the wave
   IQV_MULTI_WAVE_COMPENSATION_SINC  compensationSinc;      // default is 1 => do compensation sinc
   int   lengthExtraForPadding;                             // Extra data length for padding
};

// ****************************************************************
// Spatial Map Matrix Class
// ****************************************************************    
//!Specifies Spatial Mapping in generation of IEEE 802.11n signals.
class IQ_API iqapiSpatialMap
{
public:	
    iqapiSpatialMap(void);	            //!< Constructor
    virtual ~iqapiSpatialMap(void); 	//!< Destructor
    
    int		m;                          //!< Defines NTx. The maximum dimemsion is 4.
    int		n;                          //!< Defines NStreams. The maximum dimemsion is 4.
    double	real[16];	                //!< Double pointer array that holds the real values for the Spatial Map Matrix. 
    /*!< Values at indexes 0 through ((m*n)-1) must be set. The real values for the MxN dimensional matrix will be built column-wise from this array, i.e., col 1, row 1 first, col 1, row 2 next and so on, to col n.
    */
    double	imag[16];	                //!< Double pointer array that holds the imaginary values for the Spatial Map Matrix. 
    /*!< Values at indexes 0 through ((m*n)-1) must be set. The imaginary values for the MxN dimensional matrix will be built column-wise from this array, i.e., col 1, row 1 first, col 1 row 2 next and so on, to col n.
    */
};

// ****************************************************************
// Wave Generator Parameters Class
// ****************************************************************

//! Specifies wave generation configuration parameters to be used during a call to the \c hndl->GenerateWave object.
class IQ_API iqapiWaveGenParms
{
public:	
    iqapiWaveGenParms(void);	        //!< Constructor.
    virtual ~iqapiWaveGenParms(void);	//!< Destructor.
    
    int			mcsIndex;	            //!< Sets modulation and coding scheme. Supported range: 0 to 76
    /*!< Defines the modulation (BPSK, 64-QAM etc.) and coding rate (1/2, 5/6 etc.) and MIMO settings of the generated signal. The options follow the IEEE 802.11n standards specifications.
    
    \n \note Currently, the number of streams allowed is restricted to a maximum of four, and the modulation type is set to a maximum of 64-QAM.*/
    int			bandwidth;	            //!< Sets the bandwidth of the generated signal.
    /*!< The bandwidth values are as follows:
    \n 0 = 20 MHz
    \n 1 = 40 MHz
    \n The channel mapping follows the IEEE 802.11 standards specification. */
    char		*PSDU_mode;             //!< Represents PSDU mode.  Predefined with buffer size to be 64 bytes
    /*!< Values for this field are as follows:
    \n \c random
    \n \c userdef
    \n \c userdef_withcrc
    
    \n This field is reserved for future use.
    */
    int			PSDU_lengthBytes;	    //!< Sets the length of payload in bytes if PSDU_mode is set to 'random' which is default

    double		*PSDU_bits;	            //!< Sets the PSDU value
    int         PSDU_bits_size;         //!< Specify the size for PSDU_bits
    /*!< This field is reserved for future use. */
    
    
    //! Defines Matrix Q for Spatial Mapping
    /*! Dimension must be NTx x NStreams.*/
    iqapiSpatialMap spatialMapMatr;	
    
	//int			soundingPacketHTLTF; //!< Deprecated, Should use numESS instead. Defines the sounding packet.
    /*!< Valid values are as follows:
    \n 0: no sounding packet (default)
    \n 2-4: number of HT-LTFs, if sounding packet is present. */
    int			aggregation;            //!< Defines the aggregation bit in HT-SIG.
    /*!< This field is reserved for future use.
    */
    int			advancedCoding;         //!< Defines the advanced coding bit in HT-SIG. 0 = BCC, 1= LDPC
    
    /*!< This field is reserved for future use.
    */
    int			shortGI;	            //!< Defines the Short Guard Interval bit in HT-SIG.
    int			legacyLength;	        //!< Defines the legacy length.
    /*!<-1: Length in legacy SIG is derived from HT-length. Otherwise, use this value for length field in legacy SIG.*/
    short		scramblerInit;	        //!< Defines the scrambler initialization value.
    /*!< Valid values are as follows:
    \n -1: Random scrambler init value 
    \n 0 to 127: Scrambler init value
    */
    
    //! Defines the cyclic shift in nanoseconds.
    /*! Cyclic shift on Tx chains > 1 (in nanoseconds). */
    iqapiMeasurement *cyclicShiftnS;    // change from double to iqapiMeasurement pointer. only real portion of cyclicShiftnS is used. It is pre-allocated to be 4. No need to allocate memory for member variable 'real'. Just use it. Maximum is 4

    double		idleTimeuS;             //!< Defines the idle time (filled with zeros) between generation of two packets
    /*!< Specified in microseconds
    */
    char		*type;                  //!< Defines the type of packet to generate. 

    /*!< Currently, only EWC is supported.
    */          
    int		    greenField;	            //!< Indicates mode of operation
    /*!< Valid values are as follows:
    \n 0 = mixed mode packet format; this is the default value
    \n 1 = green field packet format.
    */
    int		    soundingPacket;         //!< Indicates that the packet is to be used for channel sounding
    /*!< Valid values are as follows: 
    \n 0:   no sounding packet; this is the default value
    \n 1-3: number of Extension HT-LTFs
    */
    char		*ofdmWindowType;        //!< Indicates type of OFDM windowing. Predefined with buffer size to be 64 bytes
    /*!< Valid values are as follows;
    \n If the value is set to \c none, the first and last samples are multiplied by 1/2
    \n If the value is set to \c raised cosine, the samples use the raised cosine filter
    \n If the value is set to \c straight line, the samples use the straight line filter */
    int		    ofdmWindowLengthSamples;//!< Indicates length of the OFDM window
    /*!< Specified in number of samples (80 MHz)
    */
    int         stbc;                   //!< STBC field in HT-SIG field. 
    /*!< Indicates the difference between the number of space-time streams and spatial streams. 
    \n Valid values are as follows:
    \n 0 = non-stbc
    \n 1 = stbc value set to 1 in HT-SIG field
    \n 2 = stbc value set to 2 in HT-SIG field
    */

    int             numESS;     //!< Number of extension spatial streams

    int SetDefault(char *type);//!<Resets the parameters of an \c iqapiWaveGenParms object to default parameters.
    /*!< This function returns 0 (IQAPI_ERR_OK) if successful; if it returns a value that is less than 0, then it indicates a warning and if it returns a value that is greater than 0, then it indicates an error (see IQAPI_ERR_CODES, or the \c hndl->lastErr buffer object).
    */

     static const int waveCommonBufferSize;     // bufferSize for PSDU_mode and ofdmWindowType

private:

    int UnpackWaveParam(void *mx_result_in);
};

// ****************************************************************
// Class to use with reference generation functions
// ****************************************************************

//! Specifies classes that must be used with reference generation functions.
class IQ_API iqapiResultFindPsduDataRef
{
public:	

    int		returnCode;
    char	*message;
    
    iqapiResultFindPsduDataRef(void)	//!< Constructor
    {
        returnCode = -99; // Invalid code
        message = NULL;
    }

    virtual ~iqapiResultFindPsduDataRef(void) {} //!< Destructor
};


// ****************************************************************
// 802.16 (WiMAX) Analysis Parameters Class (Derived from iqapiAnalysis)
// 
// type = '80216-2004' or '80216e-2005'
// mode = '80216-2004' or '80216e-2005'or 'powerPacketDetect'
// For powerPacketDetect, it will only measure power with packet detect. quicker to get power measurement with packet detect
//
// ****************************************************************
//! Specifies the analysis parameters for testing devices that are compliant with the IEEE 802.16 standards specification.
class IQ_API iqapiAnalysis80216 : public iqapiAnalysis
{
public:	
    iqapiAnalysis80216(void);	        //!< Constructor
    virtual ~iqapiAnalysis80216(void);  //!< Destructor
    
//! Specifies the required physical layer parameters for signals compliant with the IEEE 802.16 standards specification.    
    struct PHY_80216
    {
        double		sigType;		    //!< Indicates the type of signal. Default: -1
        /*!< Valid values are as follows:
        \n 1 = downlink signal
        \n 2 = uplink signal
        \n -1=auto-detect
*/		
        double		bandwidthHz;        //!< Indicates signal bandwidth. Default: -1
        /*!< Valid values for the signal bandwidth are as follows:
        -1, 1.25, 1.5, 1.75, 2.5, 3, 3.5, 5, 5.5, 6, 7, 8.75, 10, 11, 12, 14, 15, 20
        \n \note -1 = auto detect
        */		
        double		cyclicPrefix;       //!< Indicates signal cyclic prefix ratio. Default: -1			
        /*!< Valid values for signal cyclic prefix ratio are as follows: -1, 1/4, 1/8, 1/16, 1/32
        \n \note -1 = auto detect
        */
        double      rateId;			    //!< Indicates signal modulation rate. Default: -1			
        /*!< Valid values for signal modulation rate are as follows: 0, 1, 2, 3, 4, 5, 6 and  correspond to {BPSK 1/2, QPSK 1/2, QPSK 3/4, 16-QAM 1/2, 16-QAM 3/4, 64-QAM 2/3, 64-QAM 3/4}, respectively
        \n \note -1 = auto detect
        */
        double      numSymbols;         //!< Number of OFDM symbols in burst. Default: -1	
        /*!<-1 = auto-detect
        */		
    } Phy;	
//! Specifies parameters for the acquisition of signals compliant with the IEEE 802.16 standards specification.
    struct ACQ_80216	
    {
        IQV_PH_CORR_ENUM		phaseCorrect;	//!< Specifies the phase tracking mode used for the analysis. Default: IQV_PH_CORR_SYM_BY_SYM	
        /*!< Available options are as follows:
        \n 1 = Phase tracking off
        \n 2 = Symbol by symbol phase tracking (fast){1, 2, 3}
        \n 3 = 10-symbol moving average (slow) (NOT supported currently)} 
        */
        IQV_CH_EST_ENUM			channelCorrect;	//!< Specifies the channel estimation and correction mode. Default: IQV_CH_EST_RAW	
        /*!< Available options are as follows:
        \n 1 = Channel estimate based on long preamble symbol
        \n 3 = Channel estimate based on whole burst
        */
        IQV_FREQ_SYNC_ENUM		freqCorrect;	//!< Specifies the frequency offset correction mode. Default: IQV_FREQ_SYNC_LONG_TRAIN	 
        /*!< Available options are as follows:
        \n 1 = Does not perform frequency correction
        \n 2 = Performs frequency correction by using both coarse and fine frequency estimates
        \n 3 = Performs time-domain correction based on full packet measurement.
        */
 
        IQV_SYM_TIM_ENUM		timingCorrect; //!< Specifies the timing offset correction mode. Default: IQV_SYM_TIM_ON		 	 
        /*!< Available options are as follows:
        \n 1 = Does not perform timing correction
        \n 2 = Correction for symbol clock offset
        */
        
        IQV_AMPL_TRACK_ENUM		amplitudeTrack; //!< Indicates whether symbol to symbol amplitude tracking is enabled. Default: IQV_AMPL_TRACK_OFF 	 
        /*!< Available options are as follows:
        \n 1 = Disabled
        \n 2 = Enabled
        */
    } Acq;			

//! Specifies decoding parameters for the frame structure of signals compliant with the IEEE 802.16 standards specification.

/*! This structure contains fields for setting the BSID, IUC, Frame Number and decode option for 802.16d signals. When decode is set to 1 (indicating that the packet should be fully decoded), the BSID, IUC and Frame Number fields are used, otherwise these fields are ignored.
*/
    struct DEC_80216
    {
        double	decode;	        //!< Indicates whether bursts are decoded. Default: 0			 
        /*!< Valid value are as follows:
        \n 0 = disabled 
        \n 1 = enabled
        */
        double	bsid[4];        //!< Indicates the base station ID. Default: 0001			  
        /*!< The Base Station ID is necessary to derandomize. Most Significant Bit must be represented first; example: [0111]
        */
        double	iuc[4];  		//!< Indicates downlink or uplink interval usage code. Default: 0111
        /*!< This downlink or uplink interval usage code ID is necessary to derandomize. Most Significant Bit must be represented first; example: [0111]
        */
        double	frameNum[4];    //!< Indicates frame number. This ID is necessary to derandomize. Default: 0001
        /*!< This frame number ID is necessary to derandomize. Most Significant Bit must be represented first; example: [0111]
        */
    } Dec;			            // Decoding Parameters... not used in this release. 
    //! Used for non-auto-detect operations

    /*! Decoding parameters are reserved for future use. 
        */		
    char    *mapConfigFile;	    //!< Map configuration file
    /*!< \note For 802.16e analysis, a map configuration file (*.mcf) is required for non-auto-detect operation. Map configuration files can be created using the IQsignal for WiMAX GUI.
    */
    
    int	    loggingMode;        //!< Indicates the verbosity of the WiMAX analysis. Default: 0 (no logging)	
    /*!<Valid values are as follows:
    \n 0 = no logging 
    \n 1 = console (text to screen) logging 
    \n 2 = file 
    \n 3 = console and file logging
    \n Default value is 1.
    */
    
    int     masterRxSig;        //!< Indicates the Rx signal used for detection/acquisition. Default: 1 (detection)	
    /*!< Specifies which VSA signal should be used for detection/acquisition when analyzing MIMO signals. The default value is 1, which indicates that the signal captured by the first VSA is used.
    \n When MIMO signals are analyzed, this function specifies which signal must be used as the master signal.
    \n Valid values are as follows:
    \n 1 = detection
    \n 2 = acquisition. This field is reserved for future use
    \note The default value is 1 and must not be changed.
    */

    int SetDefault();
    
private:
    int Unpack80216Analysis(void *mx_result_in);
};

// ****************************************************************
// Zigbee Analysis Parameters Class (Derived from iqapiAnalysis)
// ****************************************************************
//! Specifies Zigbee analysis parameters.

class IQ_API iqapiAnalysisZigbee : public iqapiAnalysis
{
public: 
    iqapiAnalysisZigbee( void);	            //!< Constructor
    virtual ~iqapiAnalysisZigbee( void);    //!< Destructor 
            
    double					    sigType;    //!< Specifies the signal type. Default: 1 (for OQPSK)
    /*!< Currently, only the default value of 1 (for OQPSK) is supported. Valid values are as follows:
    \n IQV_FREQ_SYNC_ENUM=Long Training (Signal type is long preamble)
    \n IQV_FREQ_SYNC_ENUM=Full Packet (Signal type is full packet)
    \n IQV_AMPL_TRACK_ENUM This function is reserved for future use.
    */
    IQV_ZIGBEE_PH_CORR_ENUM     phaseCorrect;   //!< Symbol by Symbol phase correction. Default: IQV_ZIGBEE_PH_CORR_SYM_BY_SYM  (only this choice is available)
    IQV_ZIGBEE_FREQ_SYNC_ENUM   freqCorrect;    //!< frequency synchronization on full data packet. Default: IQV_ZIGBEE_FREQ_SYNC_FULL_PACKET (Only this choice is available)
    IQV_AMPL_TRACK_ENUM         amplitudeTrack; //!< Specifies the Amplitude Tracking mode. Default: IQV_AMPL_TRACK_OFF

    int SetDefault();
            
private:
   int	UnpackZigbeeAnalysis(void *mx_result_in);
};


// ****************************************************************
// Generic multi-use MIMO Analysis Parameters Class (Derived from iqapiAnalysis)
// ****************************************************************
//! Specifies analysis parameters for signals that are compliant with the IEEE 802.11n standards specification.
class IQ_API iqapiAnalysisMimo : public iqapiAnalysis
{
public:	
    iqapiAnalysisMimo(void);                //!< Constructor	
    virtual ~iqapiAnalysisMimo(void); 	    //!< Destructor 
    
    int		enablePhaseCorr;                //!< Phase Correlation Default: 1 (phase correction is enabled)
    /*!<Valid values are as follows:
    \n 0 = phase correction is disabled
    \n 1 = phase correction is enabled
    */
    int		enableSymTimingCorr;            //!< Symbol Timing Error Default: 1 (symbol timing correction is enabled)
    /*!<Valid values are as follows:
    \n 0 = symbol timing (symbol clock) correction is disabled 
    \n 1 = symbol timing (symbol clock) correction is enabled
    */
    int		enableAmplitudeTracking;        //!< Amplitude Tracking Default: 0 (amplitude tracking is disabled)
    /*!<Valid values are as follows:
    \n 0 = amplitude tracking is disabled
    \n 1 = amplitude tracking is enabled
    */
    int		frequencyCorr;                  //!< Frequency Correlation Default: 2 (frequency correction on short and long legacy training fields)
    /*!<Valid values are as follows: 
    \n 2 = frequency correction on short and long legacy training fields
    \n 3 = frequency correction based on full packet
    \n 4 = frequency correction on signal fields (legacy and HT) in addition to short and long training fields
    */  
    int		decodePSDU;                     //!< Decode PSDU Default: 1 (decode PSDU)
    /*!<Valid values are as follows: 
    \n 0 = skip decoding of PSDU (sufficient for EVM measurements)
    \n 1 = decode PSDU
    */
    int		enableFullPacketChannelEst;     //!< Full Packet Channel Estimation Default: 0 (channel estimate on long training fields)
    /*!<Valid values are as follows:	 
    \n 0 = channel estimate on long training fields
    \n 1 = reestimate channel on full packet before doing EVM calculation
    */
    int		enableMultipathEvm;             //!< Enable Multipath EVM for MIMO (IQV_MIMO_ENABLE_MULTIPATH_EVM) Default: 0 (multipath)
    /*!<Valid values are as follows:
    \n 0 = multipath
    \n 1 = standard
    */	
    int		useAllSignals;                  //!< Signals for Stream Analysis Default: 0 (use the first NStreams valid input signals for stream analysis)
    /*!< \n \note This setting is only relevant if more input signals than streams(*) are available.	
    \n (*) To demodulate a MIMO signal with N streams, at least N different input signals are needed. If \c AnalysisParam.useAllSignals is set to 0 (default), the analysis is based on the first N valid input signals only. The order of preference is determined by the parameter \c Analysis.prefOrderSignals. If \c AnalysisParam.useAllSignals is set to 1, all input signals are used.
    Valid values are as follows:
    \n 0 = use the first NStreams valid input signals for stream analysis  
    \n 1 = use all valid input signals for stream analysis 
    */
    int		prefOrderSignals[N_MAX_TESTERS];//!< Preference order used to select input signals if the \c useAllSignals parameter is set to a value of 0. Default: 1,2,3,4 (Use any permutation of the numbers 1,2,3, and 4)
    /*!< Specifies the sequence of input signals that must be used for analysis. If the number of available input signals (captures) are more than that is required to demodulate a MIMO signal, this parameter allows you to specify the sequence of the input signals that must be used for the analysis. \n \note The number of required captures is equal to the number of data streams in a MIMO signal.
    Valid values are as follows:
    \n Default: [1 2 3 4] Use any permutation of the numbers 1,2,3, and 4.
    \n As an example, when you analyze a one-stream signal with the \c useAllSignals parameter set to 0 and the \c prefOrderSignals parameter set to [2 1 3 4], the analysis first checks the capture from VSA 2 (input signal 2) for a valid signal. If the signal is valid, the analysis will be performed on capture from VSA 1 (input signal 1). If it is not valid, the next signal capture will be performed on capture 1. This process is continued until the required number of valid signals has been found or until all available captures have been checked. In this case, the required number of valid signals is one, because this analysis is on a one-stream signal.
    */
    char    *referenceFile;                 //!< Indicates name of the reference sequential_mimo analysis mode 
    /*!< Valid values are as follows:
    \n \c composite 
    \n \c sequential_mimo 
    \n This parameter is ignored for NxN systems.
    */
    int 	SetDefault();		
    
    //! Specifies fields for performing a MIMO analysis on a sequential MIMO data capture.
    struct SEQ_MIMO
    {
        int	   numSections;		        //!< Number of sections in capture Default: 0 
        /*!< \c numSections specifies the number of chains which are sequentially captured. The capture is then reshaped into dimensions numSections x (sectionLenSec + interSectionGapSec) before it is passed to the MIMO analysis.
        */
        double sectionLenSec;			    //!< Length of each section Default: 0.0
        /*!< Specified in seconds.
        */
        double interSectionGapSec;		    //!< Spacing between sections Default: 0.0
        /*!< Specified in seconds.
        */
        double sectionGainDb[N_MAX_TESTERS];//!< Gain compensation per section Default: 0,0,0,0
        /*!< \note The analysis mode must be set to \c sequential_mimo when using the SEQ_MIMO structure with sequential data captures.
        */
    } SequentialMimo;                       //!< Specifies fields for performing a MIMO analysis on a sequential MIMO data capture

    int	    loggingMode;		            //!< Indicates the verbosity of the MIMO analysis. Default: 0 (no logging)
    /*!< Valid values are as follows:
    \n 0 = no logging 
    \n 1 = console (text to screen) logging
    \n 2 = file
    \n 3 = console and file logging
    \n Default value is 1.
    */
    int     packetFormat;                   //!< Indicates format of the MIMO packet Default: 0 (auto-detect mode)
    /*!< Valid values are as follows:
    \n IQV_MIMO_PACKET_FORMAT = 0 (auto-detect mode)
    \n IQV_MIMO_PACKET_FORMAT = 1 (mixed mode format) 
    \n IQV_MIMO_PACKET_FORMAT = 2 (greenfield format)
    */

    double prePowStartSec;          //!< Specify prePower start in second default to be 8.8e-6
    double prePowStopSec;           //!< Specify prePower start in second default to be 15.2e-6

private:
    int UnpackMimoAnalysis(void *mx_result_in);
};


// ****************************************************************
// 802.11 a/g (OFDM) Specific Analysis Parameters Class (Derived from iqapiAnalysis)
// ****************************************************************
//! Specifies the analysis parameters for testing devices that are compliant with the IEEE 802.11 a/g (OFDM) standards specification.

class IQ_API iqapiAnalysisOFDM : public iqapiAnalysis
{
public:	

/*! This class is used to specify OFDM analysis parameters.
*/
    
    iqapiAnalysisOFDM(void);	            //!< Constructor
    virtual ~iqapiAnalysisOFDM(void); 	    //!< Destructor 

    IQV_PH_CORR_ENUM		ph_corr_mode;	//!< Specifies the Phase Correlation mode. Default: IQV_PH_CORR_SYM_BY_SYM
    /*!< Available options are as follows:
    IQV_PH_CORR_OFF         = 1 indicates phase correction off \n
    IQV_PH_CORR_SYM_BY_SYM  = 2 indicates Symbol-by-symbol correction \n
    IQV_PH_CORR_MOVING_AVG  = 3 moving avg. corr, 10 symbols
    */  
    IQV_CH_EST_ENUM	        ch_estimate;    //!< Specifies the Channel Estimate mode. Default: IQV_CH_EST_RAW
    /*!< Available options are as follows:
    \n IQV_CH_EST_RAW       = 1 Raw Ch Est, long train 
    \n IQV_CH_EST_2ND_ORDER = 2 2nd Order Polyfit
    \n IQV_CH_EST_RAW_FULL  = 3 Raw Ch Est, full packet
    */ 
    IQV_SYM_TIM_ENUM		sym_tim_corr;	//!< Specifies the Symbol Timing Correction mode for SISO. Default: IQV_SYM_TIM_ON
    /*!< Available options are as follows:
    \n IQV_SYM_TIM_OFF = 1 Symbol Timing Correction Off
    \n IQV_SYM_TIM_ON  = 2 Symbol Timing Correction On
    */
    IQV_FREQ_SYNC_ENUM		freq_sync;	    //!< Specifies the Frequency Sync mode. Default: IQV_FREQ_SYNC_LONG_TRAIN
    /*!< Available options are as follows:
    \n IQV_FREQ_SYNC_SHORT_TRAIN = 1 Short Training Symbol
    \n IQV_FREQ_SYNC_LONG_TRAIN  = 2 Long Training Symbol
    \n IQV_FREQ_SYNC_FULL_PACKET = 3 Full Data Packet 
    */
    IQV_AMPL_TRACK_ENUM		ampl_track;	    //!< Specifies the Amplitude Tracking mode. Default: IQV_AMPL_TRACK_OFF
    /*!< Available options are as follows:
    \n IQV_AMPL_TRACK_OFF = 1  Amplitude tracking off 
    \n IQV_AMPL_TRACK_ON  = 2  Amplitude tracking on
    */                     
    IQV_OFDM_EVM_METHODS	ofdmevmmethod;	//!< Specifies the OFDM EVM Method. Default: IQV_OFDM_EVM_STANDARD
    /*!< Available options are as follows:
    \n IQV_OFDM_EVM_MULTIPATH &mdash; More tolerant of notches in the channel response 
    \n IQV_OFDM_EVM_STANDARD &mdash; Standard 802.11 a/g EVM Method 
    */
    int				        OFDM_demod_on;  //!< Specifies whether or not full data demodulation is done. Default: 1
    /*!< Valid values are as follows:
    \n \c 0 = does not perform demodulation 
    \n \note This option is used for faster analysis performance; however, some of the results will not be available 
    \n \c 1 = performs full demodulation
    */
    IQV_OFDM_MODE_ENUM		OFDM_mode;      //!< Specifies OFDM mode. Default: IQV_OFDM_80211_AG
    /*!< Available options are as follows:
    \n IQV_OFDM_80211_AG
    \n IQV_OFDM_80211_AG_TURBO 
    \n IQV_OFDM_ASTM_DSRC
    \n IQV_OFDM_QUARTER_RATE
    */    
    double		            frequencyOffsetHz;  //!< Specifies Frequency offset. Default: 0
    /*!< If the data capture was performed with a frequency offset (i.e., the VSA and DUT are not set to the same frequency), this field may be used to enable the analysis to compensate the signal with the offset by the specified amount, in Hz.
    */		
    int		                manual_pkt_start;   //!< Bypasses the burst detection algorithm of the analysis when it is set to 1. Default: 0
    /*!< When set to 1, this field can be used (effectively) to bypass the analysis burst detection algorithm. It also indicates that the packet is starting within the first few samples of the data capture.
    */
    IQV_VSA_NUM_ENUM        vsaNum;         //!< For MIMO case, tester can capture more than 1 data set. This parameter specifies which data set to be used for doing FFT analysis. Default data set to be analyzed is IQV_VSA_NUM_1

    double prePowStartSec;          //!< Specify preamble power start in second. default to be 8.8e-6
    double prePowStopSec;           //!< Specify preamble power stop in second. default to be 15.2e-6

    int SetDefault();	//!< Sets the analysis parameters to their default condition
    /*!< \note There must be a valid connection in order to use this function. Also,  this function is called in the constructor of the class (i.e., \c SetDefault occurs when calling \c new for this object)
    */
    
private:
    int UnpackOFDMAnalysis(void *mx_result_in);
};

// ****************************************************************
// 802.11 b (DSSS) Specific Analysis Parameters Class (Derived from iqapiAnalysis)
// ****************************************************************
//! Specifies the analysis parameters for testing devices that are compliant with the IEEE 802.11b standards specification.
class IQ_API iqapiAnalysis11b : public iqapiAnalysis
{
public:	
    

    iqapiAnalysis11b(void);	                //!< Constructor.
    virtual ~iqapiAnalysis11b(void);        //!< Destructor.
    
    IQV_EQ_ENUM	            eq_taps;	    //!< Specifies the number of equalizer taps. Default: IQV_EQ_OFF
    /*!< Available options are as follows:
    \n IQV_EQ_OFF    = 1    (Equalizer Off)
    \n IQV_EQ_5_TAPS = 5    (5 taps equalizer) 
    \n IQV_EQ_7_TAPS = 7    (7 taps equalizer)
    \n IQV_EQ_9_TAPS = 9    (9 taps equalizer)
    */
    IQV_DC_REMOVAL_ENUM		DCremove11b_flag;//!< Specifies the DC removal mode. Default: IQV_DC_REMOVAL_OFF
    /*!< Available options are as follows:
    \n IQV_DC_REMOVAL_OFF = 0 (DC removal is off)  
    \n IQV_DC_REMOVAL_ON  = 1 (DC removal is on)
    */
    IQV_11B_METHOD_ENUM		method_11b;     //!< Specifies the 11b EVM method. Default: IQV_11B_STANDARD_TX_ACC
    /*!< Available options are as follows:
    \n IQV_11B_STANDARD_TX_ACC  = 1 (11b Std TX accuracy)
    \n IQV_11B_RMS_ERROR_VECTOR = 2 (Use 11b RMS error vector)
	\n IQV_11B_STANDARD_2007_TX_ACC = 3 (11b Std 2007 TX accuracy)
    */  
    double					frequencyOffsetHz;//!< Enables analysis to handle frequency offset by a specified amount. Default: 0
    /*!< This field enables analysis for preset frequency offsets, i.e., the VSA and DUT are not set to the same frequency.  The frequency offset is specified in Hz.
    */
    int					    manual_pkt_start;//!< Bypasses the burst detection algorithm of the analysis. Default: 0
    /*!< If \c manual_pkt_start is set to 1, this field is used to effectively bypass the burst detection algorithm of the analysis and to indicate  that the packet is starting within the first few samples of the data capture.
    */
   IQV_VSA_NUM_ENUM         vsaNum;          //!< For MIMO case, tester can capture more than 1 data set. This parameter specifies which data set to be used for doing analysis. Default data set to be analyzed is IQV_VSA_NUM_1

    double prePowStartSec;          //!< Specify preamble power start in second. default to be 8.8e-6
    double prePowStopSec;           //!< Specify preamble power stop in second. default to be 15.2e-6

    int SetDefault();
    
private:
    int Unpack11bAnalysis(void *mx_result_in);
};


// ****************************************************************
// Bluetooth Specific Analysis Parameters Class (Derived from iqapiAnalysis)
// ****************************************************************

//! Specifies the analysis parameters for testing devices that are compliant with the bluetooth standards specification.

class IQ_API iqapiAnalysisBluetooth : public iqapiAnalysis
{
public:	
    iqapiAnalysisBluetooth(void);       //!< Constructor
    virtual ~iqapiAnalysisBluetooth(void); 	//!< Destructor

    double   pwr_detect_gap_time;       //!< Specifies the interval that is used to determine if power is present or not (sec). Default: 4e-6
    /*!<Specified in seconds.
    */
    double  pwr_detect_diff;            //!< Specifies the maximum power difference between packets that are expected to be detected. Default: 15
    char	*analysis_type;             //!< Specifies what type of analysis to perform. Default: 'All'
    /*!< Valid values are as follows:
    \n \c PowerOnly 
    \n \c 20dbBandwidthOnly
    \n \c PowerAndFreq
    \n \c All  This is the set default value.
    \n \c ACP  only does the new ACP analysis
    \n \c AllPlus  performs all the analyses that are done by All plus the ACP analysis
	\n \c OutputSpecFreqRangeOnly  TRM/CA/04/C Output Spectrum-Frequency Range only
    */
    char    syncWord[17];               //!< This field is reserved for future use  Default: '0'
    double	dataRate;		            //!< Sets the signal data rate, and should correspond to the input signal.  Default: 0	
    /*!< Specified in Mbps.
    Valid values are as follows: 
    0 - data rate is detected automatically
    1 - data rate is 1 Mbps
    2 ?data rate is 2 Mbps (EDR)
    3 ?data rate is 3 Mbps (EDR)
    4 ?data rate is 4 Mbps Low Energy mode
    */
    double	fm_lowpass_F3dbHz;	        //!< Sets the bandwidth for the FM IF filter  Default: 650000
    /*!< IF bandwidth = 2 * fm_lowpass_F3dbHz Hz
    */
    char    *leSyncWord;                  //!< Sync Word for Low Energy BT. Eight Hex character. Default: '94826E8E'. Optional field.
    double trmca04cPowerThdBm;			  //!< Sets the power threshold per TRM/CA/04C standard to determine lower and upper frequency boundaries in the PSD trace. Default -30
	bool lePayloadPatternAutoRecognition; //!< Sets LE packet payload pattern detection method. True(Default) to perform auto recognition of pattern based on payload bits. False to read pattern based on LE packet payload type field in the PDU header
	bool continuousModeAnalysis;          //!< true to perform continuous mode analysis, where users must specify a valid data rate (1, 2, 3, or 4), and the expected signal is continuous without header; false to perform regular packet-based analysis.

	int SetDefault();
	
        
private:
    int UnpackBluetoothAnalysis(void *mx_result_in);
};


// ****************************************************************
// Wave Specific Analysis Parameters Class (Derived from iqapiAnalysis)
// ****************************************************************
//! Specifies Wave analysis parameters.
class IQ_API iqapiAnalysisWave : public iqapiAnalysis
{
public:	
    iqapiAnalysisWave(void);	        //!< Constructor
    virtual ~iqapiAnalysisWave(void); 	//!< Destructor
};

// ****************************************************************
// Sidelobe Specific Analysis Parameters Class (Derived from iqapiAnalysis)
// ****************************************************************
//! Specifies Sidelobe analysis parameters.
class IQ_API iqapiAnalysisSidelobe : public iqapiAnalysis
{
public:	
    iqapiAnalysisSidelobe(void);	        //!< Constructor
    virtual ~iqapiAnalysisSidelobe(void);	//!< Destructor

};

// ****************************************************************
// Power Ramp Specific Analysis Parameters Class (Derived from iqapiAnalysis)
// ****************************************************************

//! Specifies Power Ramp analysis parameters.
class IQ_API iqapiAnalysisPowerRamp : public iqapiAnalysis
{
public:
    /*! If the overloaded constructor for \c iqapiAnalysisPowerRamp is called with \c doOFDM set to true, then the analysis will be performed for an OFDM signal. Otherwise, the analysis will perform the analysis for an 11b signal.
    */ 	
    iqapiAnalysisPowerRamp(bool doOFDM = false); //!< Constructor
    virtual ~iqapiAnalysisPowerRamp(void); 	//!< Destructor

	IQV_VSA_NUM_ENUM vsaNum;            //!< For MIMO case, tester can capture more than 1 data set. This parameter specifies which data set to be used for doing the analysis. Default data set to be analyzed is IQV_VSA_NUM_1
};

// ****************************************************************
// CW Specific Analysis Parameters Class (Derived from iqapiAnalysis)
// ****************************************************************
//! Specifies Continuous Wave analysis parameters.

class IQ_API iqapiAnalysisCW : public iqapiAnalysis
{
public:	
    iqapiAnalysisCW(void);	            //!< Constructor
    virtual ~iqapiAnalysisCW(void);     //!< Destructor
};

// ****************************************************************
// CCDF Specific Analysis Parameters Class (Derived from iqapiAnalysis)
// ****************************************************************
//! Specifies CCDF analysis parameters.

class IQ_API iqapiAnalysisCCDF : public iqapiAnalysis
{
public:	
    iqapiAnalysisCCDF(void);            //!< Constructor
    virtual ~iqapiAnalysisCCDF(void);   //!< Destructor

    IQV_VSA_NUM_ENUM vsaNum;            //!< For MIMO case, tester can capture more than 1 data set. This parameter specifies which data set to be used for doing FFT analysis. Default data set to be analyzed is IQV_VSA_NUM_1
};

// ****************************************************************
// Generic FFT Specific Analysis Parameters Class (Derived from iqapiAnalysis)
// ****************************************************************
//! Specifies FFT analysis parameters.

class IQ_API iqapiAnalysisFFT : public iqapiAnalysis
{
public:	
    iqapiAnalysisFFT(void);	    //!< Constructor
    virtual ~iqapiAnalysisFFT(void);    //!< Destructor

    double  NFFT;	            //!< Specifies the number of points in FFT. Default: 0
    int	    NFFT_over;          //!< Specifies the minimum oversampling factor. Default: 2
    double	F_sample;	        //!< Specifies the sample frequency in Hz. Default: 80e6
    /*!< \note This field will automatically become updated with the sample frequency in the \c hndl->data object.
    */
    double	Freq_start;	        //!< Specifies start frequency, in Hz, for the x-axis in returned results.  Default: -33e6
    /*!< If the input is less than minus one-half of the sampling frequency,  this input is ignored. The default value is minus one-half of the sampling frequency. For example, if sampling frequency is 80 MHz, then the default value is -40 MHz.
    */
    double	Freq_stop;	        //!< Specifies stop frequency, in Hz, for the  x-axis in returned results.  Default: +33e6
    /*!<If larger than half the input sampling frequency this input is ignored. The default value is equal to (freq_sample / 2) - freq_delta [half of the sampling frequency minus frequency_delta]. 
    */
    int	    Delta_freq;	        //!< Frequency increment in Hz for x axis in returned results. Default: -1
    /*!< If larger than half the resolution bandwidth, this input is ignored. If the output vector would be larger than 2^15 elements, this input is also ignored. If ignored, the frequency increment is determined by internal algorithm on basis of window type, resolution bandwidth, and the \c nfft_over value.
    */
    double	res_bw;		        //!< This field is used to specify the resolution bandwidth in Hz. Default: 100000
    char	*window_type;       //!< This field is used  to specify the window type.  Default: 'blackmanharris'
    /*!< Valid options are as follows:
    \n \li blackmanharris; this is the default value 
    \n \li rectangular   
    \n \li hanning
    */
    double	video_bw;	        //!< This field is used to specify the video bandwidth in Hz. Default: 1000
    /*!< This setting is ignored in digital spectrum mode
    */
    char	*video_av_method;   //!< This field is used to specify the video averaging method. For the digital spectrum mode, this value is "average"; for the classical spectrum mode, this value is either "clear write" or "max hold". Default: 'average'
    /*!< Description of values:
    \n average&mdash;digital PSD estimation algorithm based on averaging 
    \n clear write&mdash;trace overwritten in each measurement 
    \n max hold&mdash;maximum value taken over measurements
    */
    IQV_SPECTRUM_ANALYSIS_MODE_ENUM spectrumAnalysisMode;    //!< This field is used to specify spectrum analysis mode Default: IQV_DIGITAL_SPECTRUM_ANALYSIS_MODE
    /*!< Available options are as follows:
    \n IQV_DIGITAL_SPECTRUM_ANALYSIS_MODE&mdash;FFT in digital mode 
    \n IQV_ CLASSICAL_SPECTRUM_ANALYSIS_MODE&mdash;FFT in classical mode 
    */ 
    IQV_VSA_NUM_ENUM vsaNum;     //!< For MIMO case, tester can capture more than 1 data set. This parameter specifies which data set to be used for doing FFT analysis. Default: IQV_VSA_NUM_1 

	bool                autoCorrectEnabled;

    virtual int SetDefault();

protected:
    virtual int UnpackFftAnalysis(void *mx_result_in);
};

// ****************************************************************
// Power Specific Analysis Parameters Class (Derived from iqapiAnalysis)
// ****************************************************************
//! Specifies Power analysis parameters.

class IQ_API iqapiAnalysisPower : public iqapiAnalysis
{
public:	
    iqapiAnalysisPower(void);	//!< Constructor
    virtual ~iqapiAnalysisPower(void); 	//!< Destructor
    
   double	T_interval;	        //!< This field is used to specify the interval that is used to determine if power is present or not (sec). Default: 3.2e-6
   double	max_pow_diff_dB;    //!< This filed is used to specify the maximum power difference between packets that are expected to be detected.  Default: 15
   IQV_VSA_NUM_ENUM  vsaNum;    //!< For MIMO case, tester can capture more than 1 data set. This parameter specifies which data set to be used for doing FFT analysis. Default: IQV_VSA_NUM_1

   int SetDefault();	        //!< Sets the analysis parameters to their default condition. Note that there must be a valid connection in order to use this function. Also, note that this function is called in the constructor of the class (ie, SetDefault will happen when calling /c new for this object).
    
private:
    int UnpackPowerAnalysis(void *mx_result_in);
};

class IQ_API iqapiAnalysisHT20WideBandLoAccuracy : public iqapiAnalysis
{
public:	
    iqapiAnalysisHT20WideBandLoAccuracy(void);	            //!< Constructor
    virtual ~iqapiAnalysisHT20WideBandLoAccuracy(void); 	//!< Destructor

    static const double dbResolutionBWLowerLimit;
    IQV_WINDOW_TYPE_ENUM windowType;
    double	dbResolutionBW;	    //!< It has to be greater or equal to 1000, Default: 100000

    IQV_VSA_NUM_ENUM  vsaNum;   //!< For MIMO case, tester can capture more than 1 data set. This parameter specifies which data set to be used for doing FFT analysis. Default: IQV_VSA_NUM_1
	bool autoCorrectEnabled;

    int SetDefault();
};

class IQ_API iqapiAnalysisHT20WideBandHiAccuracy : public iqapiAnalysisHT20WideBandLoAccuracy
{
public:	
    iqapiAnalysisHT20WideBandHiAccuracy(void);	            //!< Constructor
    virtual ~iqapiAnalysisHT20WideBandHiAccuracy(void); 	//!< Destructor
};

// ****************************************************************
// HT40 Analysis Parameters Class (Derived from iqapiAnalysis)
// ****************************************************************
//! Specifies parameters for the wideband FFT analysis of 40 MHz 802.11n signals.
class IQ_API iqapiAnalysisHT40 : public iqapiAnalysis
{
    //friend class iqapiHndl;

public:	
    iqapiAnalysisHT40(void);	//!< Constructor
    virtual ~iqapiAnalysisHT40(void); 	//!< Destructor

    static const double dbResolutionBWLowerLimit;
    IQV_WINDOW_TYPE_ENUM windowType;
    double	dbResolutionBW;	    //!< It has to be greater or equal to 1000, Default: 100000
    IQV_VSA_NUM_ENUM  vsaNum;   //!< For MIMO case, tester can capture more than 1 data set. This parameter specifies which data set to be used for doing FFT analysis. Default: IQV_VSA_NUM_1
	bool autoCorrectEnabled;

    int SetDefault();
};

class IQ_API iqapiAnalysisHT40Data3 : public iqapiAnalysisHT40
{

public:	
    iqapiAnalysisHT40Data3(void);	//!< Constructor
    virtual ~iqapiAnalysisHT40Data3(void) {}; 	//!< Destructor
};

class IQ_API iqapiAnalysisHT40WideBandHiAccuracy : public iqapiAnalysisHT40
{
public:
	iqapiAnalysisHT40WideBandHiAccuracy();
	virtual ~iqapiAnalysisHT40WideBandHiAccuracy() {}	//!< Destructor
};

class IQ_API iqapiAnalysisObw : public iqapiAnalysis
{
public:
    iqapiAnalysisObw();
    virtual ~iqapiAnalysisObw() {};

    double              obwPercentage;      // OBW percentage should be within 0 <= x <= 1
    IQV_VSA_NUM_ENUM    vsaNum;             //!< For MIMO case, tester can capture more than 1 data set. This parameter specifies which data set to be used for doing FFT analysis. Default: IQV_VSA_NUM_1

};

class IQ_API iqapiAnalysisAcpr : public iqapiAnalysis
{
public:
    iqapiAnalysisAcpr();
    virtual ~iqapiAnalysisAcpr() {};

    static const int    maxNumberOfCenterAndSpan = 10;
    struct 
    {
        double              centerFrequencyHz;
        double              spanPerChannelHz;
    } centerAndSpan[maxNumberOfCenterAndSpan];

    int                 numberOfCenterAndSpan;

    IQV_VSA_NUM_ENUM    vsaNum;   //!< For MIMO case, tester can capture more than 1 data set. This parameter specifies which data set to be used for doing FFT analysis. Default: IQV_VSA_NUM_1

};

// ****************************************************************
// 80216 Results Class
// type = '80216-2004' or '80216e-2005'
// mode = '80216-2004' or '80216e-2005'or 'powerPacketDetect'
// Note : 
// (1) If mode is 'powerPacketDetect', only the following result parameters are needed
//     => acquisition, type, avgPowerNoGapDb and errTxt
// ****************************************************************

//! Specifies analysis results generated using \c iqapiResult80216 objects.

class IQ_API iqapiResult80216 : public iqapiResult
{
public:	
    iqapiResult80216(void);	                //!< Constructor
    virtual ~iqapiResult80216(void);        //!< Destructor
    
    int                 packetDetection;    //!< Indicates valid packet detection
    int	                acquisition;	    //!< Indicates valid packet acquisition
    int	                demodulation;	    //!< Indicates valid packet demodulation
    int	                completePacket;	    //!< Indicates that at least one complete packet was found by the analysis
    int		            fchHcs;	            //!< Indicates valid fchHcs
    char	            *errTxt;	        //!< Indicates possible warning and error messages.
    iqapiMeasurement	*analyzedRange;		//!< Indicates the start/stop pointers and the packet selected for analysis
    /*!< \c iqapiMeasurement is a data type used to return result values
    */
    // power 
    iqapiMeasurement    *rxRmsPowerDb;		//!< RMS power in dBm
    iqapiMeasurement    *avgPowerNoGapDb;   //!< Average power
    iqapiMeasurement    *rxPreambleRmsPowerDb; //!< Preamble average power

    // FrameInfo
    iqapiMeasurement	*sigType;	        //!< Indicates uplink or downlink burst
    iqapiMeasurement	*numSymbols;	    //!< Indicates the number of symbols in the analyzed packet
    iqapiMeasurement	*modOrder;	        //!< 802.16-2004 modOrder
    iqapiMeasurement	*rateId;	        //!< 802.16-2004 rateId
    iqapiMeasurement	*bandwidthHz;	    //!< 802.16-2004 bandwidth
    iqapiMeasurement	*cyclicPrefix;	    //!< 802.16-2004 cyclic prefix
    iqapiMeasurement	*numPreambleSymbols;//!< Indicates the number of preamble symbols

    // FrameInfo.Fch
    iqapiMeasurement	*validHcs;	        //!< Indicates valid Hcs
    iqapiMeasurement	*bits;		        //!< Indicates FramInfo.Fch bits
    iqapiMeasurement	*bsid;		        //!< Base station ID
    iqapiMeasurement	*frameNum;	        //!< 802.16-2004 frame number
    iqapiMeasurement	*changeCount;	    //!< 802.16-2004 change count
    iqapiMeasurement	*Bursts;	        //!< 802.16-2004 bursts

    // EVM Measurements
    iqapiMeasurement	*evmAvgAll;	        //!< EVM Average (All)
    iqapiMeasurement	*evmAvgData;	    //!< EVM Average (Data)
    iqapiMeasurement	*evmAvgPilot;	    //!< EVM Average (Pilots)
    iqapiMeasurement	*evmTones;	        //!< EVM per Tone
    iqapiMeasurement	*evmSymbols;	    //!< EVM per Symbol
    iqapiMeasurement	*evmCinrDb;	        //!< EVM Carrier to Interference and Carrier Noise Ratio (CINR in Db)
    iqapiMeasurement    *avgUnmodData;	    //!< EVM of the unmodulated data carrier

    // Indexes
    iqapiMeasurement	*dataTones;	        //!< Data tones
    iqapiMeasurement	*pilotTones;	    //!< Pilot tones

    // Data
    iqapiMeasurement	*demodSymbols;	    //!< Demodulated Symbols
    iqapiMeasurement	*slicedSymbols;	    //!< Sliced Symbols
    iqapiMeasurement	*validFec;	        //!< Valid Forward Error Correction
    iqapiMeasurement	*MacPdu;	        //!< MAC PDU

    iqapiMeasurement	*phaseNoiseDegRmsAll; //!< RMS phase noise in degrees
    iqapiMeasurement	*phaseNoiseDegSymbols;//!< Phase noise per symbol
    iqapiMeasurement	*freqOffsetTotalHz;	//!< Frequency offset in Hz
    iqapiMeasurement	*freqErrorHz;	    //!< Frequency error in Hz
    iqapiMeasurement	*symClockErrorPpm;  //!< Symbol clock error in PPM    	
    iqapiMeasurement	*iqImbalAmplDb;	    //!< IQ Amplitude Imbalance in dB
    iqapiMeasurement	*iqImbalPhaseDeg;	//!< IQ Phase Imbalance in degrees
    iqapiMeasurement	*channelEst;		//!< Channel Estimate
    char				*type;	            //!< Signal/analysis type   

    iqapiMeasurement	*dcLeakageDbc;      //!< DC leakage in dBc per Rx; dimension 1 x NRx

    int                 numberOfZone;	    //!< Number of zones
};

// ****************************************************************
// Generic multi-use MIMO Results Class
// ****************************************************************
//! Specifies analysis results generated using \c iqapiAnalysisMimo objects

class IQ_API iqapiResultMimo : public iqapiResult
{
public:	
    iqapiResultMimo(void);	                //!< Constructor
    virtual ~iqapiResultMimo(void); 	    //!< Destructor

    int	                packetDetection;    //!< Indicates valid packet start detected.
    int	                acquisition;		//!< Indicates valid HT packet start detected.
    int	                demodulation;		//!< Indicates streams demodulated (packet may be truncated)
    int	                completePacket;		//!< Indicates a complete packet was demodulated    
    bool	            htSigFieldCRC;	    //!< Indicates valid CRC on HT-SIG
    bool	            psduCRC;		    //!< Indicates valid CRC on PSDU   
    char	            *errTxt;		    //!< Error messages text string if analysis failed	

    iqapiMeasurement	*analyzedRange;		//!< Start and end point of signal part that was used to for analysis. The API picks the first packet in the input signal for analysis; following packets are ignored.
    iqapiMeasurement	*idxAnalyzedSigs;	//!< Indexes of input signals that were used for analysis.
    iqapiMeasurement	*channelEst;		//!< Channel estimate matrix; dimension NStreams x Ntones x NRx
    iqapiMeasurement	*idxDataTones;		//!< Index numbers of data tones in the result data (channelEst etc.); DC component is at index 1.
    iqapiMeasurement	*idxPilotTones;	    //!< Index number of pilot tones in the result data (channel Est etc.); DC component is at index 1.
    iqapiMeasurement	*evmAvgAll;	        //!< Averaged, per stream; dimension 1 xNStreams
    iqapiMeasurement	*evmSymbols;	    //!< Per symbol and stream; dimension NStreams x NSymbols
    iqapiMeasurement	*evmTones;	        //!< Per tone and stream; dimension NStreams x Ntones
    iqapiMeasurement	*freqErrorHz;	    //!< Frequency error in Hz
    iqapiMeasurement	*symClockErrorPpm;	//!< Symbol clock error in ppm
    iqapiMeasurement	*PhaseNoiseDeg_RmsAll;  //!< RMS phase noise, over all received signals
    iqapiMeasurement	*PhaseNoiseDeg_Symbols;	//!< Per symbol and VSA signal; dimension NRx x Nsymbols
    iqapiMeasurement	*CCDF_xPowerDb;	    //!< Complementary Cumulative Distribution Function. dB above average power (x-axis)
    iqapiMeasurement	*CCDF_yProb;	    //!< Complementary Cumulative Distribution Function. Probability (y-axis)	
    iqapiMeasurement	*IQImbal_amplDb;	//!< IQ gain imbalance in dB, per stream.
    iqapiMeasurement	*IQImbal_phaseDeg;	//!< IQ phase imbalance in degree, per stream.	
    iqapiMeasurement	*demodSymbols;		//!<Complex demodulated symbols; dimension NTones x NSymbols X Nstreams 
    iqapiMeasurement	*slicedSymbols;	    //!< Sliced symbols
    iqapiMeasurement	*psduBits;	        //!< Decoded PSDU bits
    iqapiMeasurement	*serviceField;	    //!< Bits of service field	
    iqapiMeasurement	*preambleFreqErrorHz;   //!< Preamble frequency error, in Hz
    iqapiMeasurement	*preambleFreqErrorTimeUs; //!< Preamble frequency error versus time
    iqapiMeasurement	*legacyBits;	    //!< Information from the legacy signal field: 24 bits 
    iqapiMeasurement	*legacyLength;	    //!< Information from the legacy signal field: value in length field as decimal number.
    iqapiMeasurement	*legacyRate;	    //!< Information from the legacy signal field: data rate information. 	

    iqapiMeasurement	*htSig1_bits;	    //!< Information from HT-SIG1:24 bits.	
    iqapiMeasurement	*htSig1_mcsIndex;	//!< Information from HT-SIG1: MCS index as decimal number.
    iqapiMeasurement	*htSig1_bandwidth;	//!< Information from HT-SIG1: bandwidth (0) 20 MHz (1) 40 MHz.	
    iqapiMeasurement	*htSig1_htLength;	//!< Information from HT-SIG1: value in length field as decimal number  Note: Not supported in 11ac

    iqapiMeasurement	*htSig2_bits;		//!< Information from HT-SIG-2: 24 bits
    iqapiMeasurement	*htSig2_aggregation;//!< Information from HT-SIG-2: aggregation     Note: Not supported in 11ac
    iqapiMeasurement	*htSig2_stbc;		//!< Information from HT-SIG-2: 2 bits as decimal
    iqapiMeasurement	*htSig2_advancedCoding;	//!< Information from HT-SIG-2: advanced coding
    iqapiMeasurement	*htSig2_shortGI;	//!< Information from HT-SIG-2: short guard interval
    iqapiMeasurement	*htSig2_numHTLF;	//!< Information from HT-SIG-2: 2 bits as decimal number	Note: Not supported in 11ac
    iqapiMeasurement	*htSig2_soundingPacket;	//!< Information from HT-SIG-2: sounding packet		

    iqapiMeasurement	*rateInfo_bandwidthMhz;	//!< Bandwidth in MHz
    iqapiMeasurement	*rateInfo_dataRateMbps;	//!< Data rate in Mbps
    iqapiMeasurement	*rateInfo_spatialStreams;	//!< Number of spatial streams		
    char			    *rateInfo_modulation; //!< Modulation type, i.e., '64 QAM'
    char			    *rateInfo_codingRate; //!< Coding rate, i.e., '3/4'	
    iqapiMeasurement    *rateInfo_spaceTimeStreams;	//!< Indicates number of spatial streams.
    iqapiMeasurement	*isolationDb;       //!< Matrix indicating isolation between streams
    /*!<Column 1 is for Rx signal containing main power for stream 1, etc.; dimension NStreams x NStreams 
    \n \note Isolation can only be measured with direct-mapping signals. 
    \n Example: isolationDb = [0 -20; -25 0]
    - Stream 1 leaks into stream 2 signal with -20 dB 
    - Stream 2 leaks into stream 1 signal with -25 dB
    */	
    iqapiMeasurement	*dcLeakageDbc;	    //!< DC leakage in dBc per Rx; dimension 1 x NRx
    iqapiMeasurement	*rxRmsPowerDb;	    //!< RMS power in dBm
    iqapiMeasurement	*mainPathStreamPowerDb;	//!< Main path power per stream (maximum element from each row in channel power matrix rxRmsPowerDb); dimension NStreams x 1		

    iqapiMeasurement	*evmAvgAllComposite;	//!< EVM over all streams 
    iqapiMeasurement	*htSig2_numESS;	        //!< Information from HT-SIG-2: 2 bits as decimal number	Note: Not supported in 11ac

    iqapiMeasurement    *powerPreambleDbm;         //!< preamble power result based on the setting of prePowStartSec and prePowStopSec

};

// ****************************************************************
// OFDM Results Class
// ****************************************************************
//! Specifies analysis results generated with \c iqapiAnalysisOFDM objects. 
class IQ_API iqapiResultOFDM : public iqapiResult
{
public:	
    iqapiResultOFDM(void);	            //!< Constructor
    virtual ~iqapiResultOFDM(void);	    //!< Destructor

    int			        psduCrcFail;    //!< 1 = PSDU CRC Check Failed, 0 = PSDU CRC Check Passes 
    int			        plcpCrcPass;	//!< 1 = PLCP CRC Check Passed 
    double		        dataRate;	    //!< Data rate in Mbps
    int			        numSymbols;	    //!< Number of symbols
    int			        numPsduBytes;	//!< Number of bytes in PSDU

    iqapiMeasurement	*evmAll;	    //!< EVM for entire frame
    iqapiMeasurement	*evmData;	    //!< EVM for data part of frame	
    iqapiMeasurement	*evmPilot;	    //!< EVM for pilot part of frame
    iqapiMeasurement	*psdu;		    //!< The PSDU data. 0/1 values. Includes the MAC Header and the FCS, if present.
    iqapiMeasurement	*startPointers;	//!< Provides the approximate starting locations of each packet detected in the input data. If no packet detected, this is an empty vector.
    iqapiMeasurement	*stopPointers;	// Reserved for future use
    iqapiMeasurement	*hhEst;		    //!< 64 element complex vector that represents the FFT output of the 2 long symbols in the PLCP pre-amble of the OFDM signal.
    iqapiMeasurement	*plcp;		    //!< PLCP (binary) data as 1/0s
    iqapiMeasurement	*codingRate;    //!< Coding rate.
    iqapiMeasurement	*freqErr;	    //!< Frequency error
    iqapiMeasurement	*clockErr;	    //!< Symbol Clock Error
    iqapiMeasurement	*ampErr;	    //!< IQ Match Amplitude Error
    iqapiMeasurement	*ampErrDb;	    //!< IQ Match Amplitude Error in dB
    iqapiMeasurement	*phaseErr;	    //!< IQ Match Phase Error
    iqapiMeasurement	*rmsPhaseNoise;	//!< Frequency RMS Phase Noise

    iqapiMeasurement	*rmsPowerNoGap;	//!< Power RMS No Gap
    iqapiMeasurement	*rmsPower;	    //!< Power RMS
    iqapiMeasurement	*pkPower;	    //!< Power Peak
    iqapiMeasurement	*rmsMaxAvgPower;//!< Power RMS Max. Average

    iqapiMeasurement	*freq_vector;	//!< Preamble frequency error
    iqapiMeasurement	*freq_vector_time; //!< Preamble frequency error versus time

    iqapiMeasurement     *evmSymbols;    //!< the number of actually analyzed symbols 

    iqapiMeasurement	*dcLeakageDbc;      //!< DC leakage in dBc 

    iqapiMeasurement	*demodSymbols;		//!<Complex demodulated symbols; dimension NTones x NSymbols 
    iqapiMeasurement	*slicedSymbols;		//!< Sliced Symbols    

    iqapiMeasurement    *powerPreambleDbm;     //!< preamble power result based on the setting of prePowStartSec and prePowStopSec

};

// ****************************************************************
// 802.11 b Results Class
// ****************************************************************
//! Specifies analysis results generated using \c iqapiAnalysis11b objects.
class IQ_API iqapiResult11b : public iqapiResult
{
public:	
    iqapiResult11b(void);	            //!< Constructor
    virtual ~iqapiResult11b(void);	    //!< Destructor

    int			        lockedClock;	//!< Locked Clock, see 802.11b standard
    int			        plcpCrcFail;	//!< 1 = PLCP CRC Check Failed, 0 = PLCP CRC Check Passes
    int			        psduCrcFail;	//!< 1 = PSDU CRC Check Failed, 0 = PSDU CRC Check Passes
    int			        longPreamble;	//!< 1 = Long Preamble, 0 = Short Preamble
    int			        numPsduBytes;	//!< Number of bytes in PSDU

    double              bitRateInMHz;     

    iqapiMeasurement	*evmPk;		    //!< EVM peak value
    iqapiMeasurement	*psdu;		    //!< The PSDU data. 0/1 values. Includes the MAC Header and the FCS, if present.
    iqapiMeasurement	*startPointers;	//!< Provides the approximate starting locations of each packet detected in the input data. If no packet detected, this is an empty vector.
    iqapiMeasurement	*stopPointers;	//!< Provides the approximate end location of each packet.
    iqapiMeasurement	*selectedIndex;	//!< Provides the start and end pointer for the packet selected by the analysis program for analysis.
    iqapiMeasurement	*eye;		    //!< Represents the values of the In-phase receiver channel at 19 samples per chip. Can be used to plot the eye-diagram by plotting samples (0:18)+n*19 on the same plot.
    iqapiMeasurement	*scramblerInit;	//!< Scrambler Initialization. 7 element (or empty) Real vector is returned.
    iqapiMeasurement	*plcp;		    //!< PLCP (binary) data as 1/0s
    iqapiMeasurement	*bitRate;	    //!< Bit Rate, see 802.11b standards specification.	
    iqapiMeasurement	*modType;	    //!< Mod Type, see 802.11b standards specification.	
    iqapiMeasurement	*evmAll;	    //!< EVM for entire frame	
    iqapiMeasurement	*freqErr;	    //!< Frequency Error
    iqapiMeasurement	*clockErr;	    //!< Symbol Clock Error
    iqapiMeasurement	*ampErr;	    //!< IQ Match Amplitude Error
    iqapiMeasurement	*ampErrDb;	    //!< IQ Match Amplitude Error in dB
    iqapiMeasurement	*phaseErr;	    //!< IQ Match Phase Error
    iqapiMeasurement	*rmsPhaseNoise;	//!< Frequency RMS Phase Noise

    iqapiMeasurement	*rmsPowerNoGap;	//!< Power RMS No Gap
    iqapiMeasurement	*rmsPower;	    //!< Power RMS
    iqapiMeasurement	*pkPower;	    //!< Power Peak
    iqapiMeasurement	*rmsMaxAvgPower;//!< Power RMS Max. Average

    iqapiMeasurement	*freqErrTimeVect; //!< Frequency Error for the the entire packet plotted against time
    iqapiMeasurement	*freqErrVect;	//!< Frequency Error for the entire packet		
    iqapiMeasurement	*maxFreqErr;	//!< The highest Frequency Error value for the entire packet

    iqapiMeasurement	*loLeakageDb;	//!< Lo Leakage

    iqapiMeasurement	*evmInPlcp;     //!< EVM in PLCP
    iqapiMeasurement	*evmInPreamble; //!< EVM on preamble
    iqapiMeasurement	*evmInPsdu;     //!< EVM in PSDU
    iqapiMeasurement	*evmErr;        //!< EVM error

    iqapiMeasurement    *powerPreambleDbm;     //!< preamble power result based on the setting of prePowStartSec and prePowStopSec

};

// ****************************************************************
// Wave Analysis Results Class
// ****************************************************************
//! Specifies results generated with \c iqapiResultWave objects. 
class IQ_API iqapiResultWave : public iqapiResult
{
public:	
    iqapiResultWave(void);	        //!< Constructor
    virtual ~iqapiResultWave(void);	//!< Destructor

    iqapiMeasurement	*dcDc;	    //!< Dc DC value
    iqapiMeasurement	*dcRms;	    //!< DC RMS value
    iqapiMeasurement	*dcMin;	    //!< DC Minimum value
    iqapiMeasurement	*dcMax;	    //!< DC Maximum value
    iqapiMeasurement	*dcPkToPk;  //!< DC Peak to Peak value	
    iqapiMeasurement	*dcRmsI;    //!< DC RMS for I channel 
    iqapiMeasurement	*dcRmsQ;    //!< DC RMS for Q channel
    iqapiMeasurement	*acDc;	    //!< AC DC value
    iqapiMeasurement	*acRms;	    //!< AC RMS value
    iqapiMeasurement	*acMin;	    //!< AC Minimum value
    iqapiMeasurement	*acMax;	    //!< AC Maximum value
    iqapiMeasurement	*acPkToPk;  //!< AC Peak to Peak value
    iqapiMeasurement	*acRmsI;    //!< AC RMS for I Channel
    iqapiMeasurement	*acRmsQ;    //!< AC RMS for Q Channel
    iqapiMeasurement	*rmsDb;	    //!< AC RMS value in dB	
};

// ****************************************************************
// Sidelobe Results Class
// ****************************************************************
//! Specifies analysis results generated with \c iqapiAnalysisSidelobe objects.
class IQ_API iqapiResultSidelobe : public iqapiResult
{
public:	
    iqapiResultSidelobe(void);	        //!< Constructor
    virtual ~iqapiResultSidelobe(void);	//!< Destructor

    iqapiMeasurement	*res_bw_Hz;	    //!< Resolution bandwidth in Hz	
    iqapiMeasurement	*fft_bin_size_Hz;//!< FFT Bin Size in Hz
    iqapiMeasurement	*peak_center;	//!< Peak Center in dB
    iqapiMeasurement	*peak_1_left;	//!< Peak 1st Lower Side Lobe in dB
    iqapiMeasurement	*peak_2_left;	//!< Peak 2nd Lower Side Lobe in dB 
    iqapiMeasurement	*peak_1_right;	//!< Peak 1st Higher Side Lobe in dB
    iqapiMeasurement	*peak_2_right;	//!< Peak 2nd Higher Side Lobe in dB
    iqapiMeasurement	*psd_dB;	    //!< PSD Plot data in dB
};

// ****************************************************************
// Power Ramp Results Class
// ****************************************************************
//! Specifies analysis results generated with \c iqapiAnalysisPowerRamp objects.
class IQ_API iqapiResultPowerRamp : public iqapiResult
{
public:	
    iqapiResultPowerRamp(void);	        //!< Constructor
    virtual ~iqapiResultPowerRamp(void);//!< Destructor

    iqapiMeasurement	*on_power_inst; //!< Instant power on ramp
    iqapiMeasurement	*off_power_inst;//!< Instant power off ramp
    iqapiMeasurement	*on_power_peak;	//!< Power on ramp peak values
    iqapiMeasurement	*off_power_peak;//!< Power off ramp peak values
    iqapiMeasurement	*on_time_vect;	//!< Power on ramp time vector
    iqapiMeasurement	*off_time_vect;	//!< Power off ramp time vector
    iqapiMeasurement	*on_mask_x;		//!< Power on ramp mask x-axis
    iqapiMeasurement	*off_mask_x;	//!< Power off ramp mask x-axis		
    iqapiMeasurement	*on_mask_y;		//!< Power on mask y-axis
    iqapiMeasurement	*off_mask_y;	//!< Power off ramp mask y-axis
    iqapiMeasurement	*on_time;		//!< Ramp on time
    iqapiMeasurement	*off_time;		//!< Ramp off time
};

// ****************************************************************
// CW Analysis Results Class
// ****************************************************************
//! Specifies analysis results generated with \c iqapiAnalysisCW objects.
class IQ_API iqapiResultCW : public iqapiResult
{
public:	
    iqapiResultCW(void);	        //!< Constructor
    virtual ~iqapiResultCW(void);   //!< Destructor

    double frequency;	            //!< Frequency result
};

// ****************************************************************
// CCDF Results Class
// ****************************************************************
//! Specifies analysis results generated with \c iqapiAnalysisCCDF objects.
class IQ_API iqapiResultCCDF : public iqapiResult
{
public:	
    iqapiResultCCDF(void);	            //!< Constructor
    virtual ~iqapiResultCCDF(void);	    //!< Destructor

    iqapiMeasurement	*prob;	        //!< Real vector containing CCDF probability values (Y-axis of CCDF plot)
    iqapiMeasurement	*power_rel_dB;	//!< Real vector containing CCDF power relative to average power in dB values (X-axis of CCDF plot)
    iqapiMeasurement	*percent_pow;	//!< CCDF power percentage
};

// ****************************************************************
// FFT Results Class
// ****************************************************************
//! Specifies analysis results generated with \c iqapiAnalysisFFT objects.
class IQ_API iqapiResultFFT : public iqapiResult
{
public:	
    iqapiResultFFT(void);	            //!< Constructor
    virtual ~iqapiResultFFT(void);	    //!< Destructor

    iqapiMeasurement	*x;	            //!< X-axis values, typically frequency; vector is returned. 
    iqapiMeasurement	*y;	            //!< y-axis values, power in dBm. Vector is returned.
    char			    *x_string;	    //!< X-axis label. String value is returned.
    char			    *y_string;	    //!< Y-axis label. String value is returned.
    char			    *title;		    //!< Text for title. String value is returned.
    int			        valid;		    //!< Returns (value) whether results are valid or not. 1:valid, 0: invalid
    int			        warning;	    //!< Returns (value) that if equal to 1, indicates that the results should be looked at with caution.
    char			    *error;		    //!< Returns a text string that explains the reason for the valid flag being 0 or the warning flag beint 1.
    iqapiMeasurement	*length;	    //!< Returns length of X and Y vectors above.
    iqapiMeasurement	*res_bw;	    //!< Resolution bandwidth used in calculations.
    iqapiMeasurement	*noise_bw;	    //!< Noise bandwidth used in calculations. May be different than resolution bandwidth.
    iqapiMeasurement	*video_bw;	    //!< Video bandwidth used in calculation.
    iqapiMeasurement	*Freq_start;    //!< Start frequency in Hz.
    iqapiMeasurement	*Freq_stop;	    //!< Stop frequency in Hz.
    iqapiMeasurement	*Delta_freq;    //!< Frequency increment in Hz.
    iqapiMeasurement	*Delta_power;    //!< 
    
};

// ****************************************************************
// HT40 Results Class
// ****************************************************************
//! Specifies analysis results generated with \c iqapiAnalysisHT40 objects.
class IQ_API iqapiResultHT40 : public iqapiResult {
public:
    iqapiResultHT40();	    //!< Constructor
    virtual ~iqapiResultHT40();	//!< Destructor

    int len_of_ffts;        //the length of ffts 
    iqapiResultFFT ** ffts;
};

class IQ_API iqapiResultHT40WideBandHiAccuracy : public iqapiResult {
public:
    iqapiResultHT40WideBandHiAccuracy();	    //!< Constructor
    virtual ~iqapiResultHT40WideBandHiAccuracy();	//!< Destructor

    int len_of_ffts;        //the length of ffts 
    iqapiResultFFT ** ffts;
};

class IQ_API iqapiResultHT20WideBandLoAccuracy : public iqapiResult {
public:
    iqapiResultHT20WideBandLoAccuracy();	            //!< Constructor
    virtual ~iqapiResultHT20WideBandLoAccuracy();	    //!< Destructor

    int len_of_ffts;        //the length of ffts 
    iqapiResultFFT ** ffts;
};

class IQ_API iqapiResultHT20WideBandHiAccuracy : public iqapiResult {
public:
    iqapiResultHT20WideBandHiAccuracy();	            //!< Constructor
    virtual ~iqapiResultHT20WideBandHiAccuracy();	    //!< Destructor

    int len_of_ffts;        //the length of ffts 
    iqapiResultFFT ** ffts;
};

// ****************************************************************
// Power Analysis Results Class
// ****************************************************************
//! Specifies analysis results generated with \c iqapiAnalysisPower objects.
class IQ_API iqapiResultPower : public iqapiResult
{
public:	
    iqapiResultPower(void);	                //!< Constructor
    virtual ~iqapiResultPower(void);	    //!< Destructor

    char		     *msg;                  //!<  Possible warning and error messages, text string is returned.
    int		         valid;                 //!< Indicates valid results (1 valid, 0 invalid).
    iqapiMeasurement *start_loc;            //!< Start location for each packet detected.
    iqapiMeasurement *stop_loc;             //!< Stop location for each packet detected.
    iqapiMeasurement *start_sec;            //!< Start time, in seconds, for each packet detected.
    iqapiMeasurement *stop_sec;             //!< Stop time, in seconds, for each packet detected
    iqapiMeasurement *complete_burst;       //!< Indicates complete packet (start/stop found).

    iqapiMeasurement *P_av_each_burst;      //!< Average power of each burst
    iqapiMeasurement *P_pk_each_burst;      //!< Peak power of each burst
    iqapiMeasurement *P_av_all;             //!< Average power of whole capture
    iqapiMeasurement *P_peak_all;           //!< Peak power of whole capture
    iqapiMeasurement *P_av_no_gap_all;      //!< Average power, not counting gaps
    iqapiMeasurement *P_av_each_burst_dBm;  //!< Average power of each burst in dBm
    iqapiMeasurement *P_pk_each_burst_dBm;  //!< Peak power of each burst in dBm
    iqapiMeasurement *P_av_all_dBm;         //!< Average power of whole capture in dBm
    iqapiMeasurement *P_peak_all_dBm;       //!< Peak power of whole capture in dBm
    iqapiMeasurement *P_av_no_gap_all_dBm;  //!< Average power, not counting gaps in dBm
};

// ****************************************************************
// Zigbee Results Class
// ****************************************************************

//! Specifies Zigbee analysis for the IEEE 802.15.4 standards specification wireless protocol.
class IQ_API iqapiResultZigbee : public iqapiResult
{
public: 
    iqapiResultZigbee( void);	            //!< Constructor
    virtual ~iqapiResultZigbee( void);      //!< Destructor

    int				    bValid;	            //!< A value of 1 indicates a valid analysis, 0 indicates a failed analysis.
    char			    *errTxt;	        //!< Contains possible error messages and warnings returned from the analysis.

    // members extracted from the IQsignal for Zigbee GUI
    iqapiMeasurement    *rxPeakPower;	    //!< Peak linear power
    iqapiMeasurement    *rxRmsPowerAll;	    //!< Average linear power of the whole signal
    iqapiMeasurement    *rxRmsPowerNoGap;	//!< Average linear power, excluding gaps between packets

    iqapiMeasurement    *rxPeakPowerDbm;	//!< Peak power in dBm
    iqapiMeasurement    *rxRmsPowerAllDbm;	//!< Average power of the whole signal, in dBm
    iqapiMeasurement    *rxRmsPowerNoGapDbm;//!< Average power, excluding gaps between packets, in dBm 

    int		            numSymbols;		    //!< Number of symbols in the PSDU

    // members extracted from the ResDem Structure
    // FrameInfo
    iqapiMeasurement    *sigType;	        //!< Indicates the signal type, currently only 1 is returned (for OQPSK).
    iqapiMeasurement    *fsWaveHz;	        //!< Indicates the sampling rate used in the analysis.
    iqapiMeasurement    *overSampling;	    //!< Oversampling ratio per symbol.
        
    // EVM Measurements
    iqapiMeasurement    *evmAll;	        //!< Average EVM for all symbols.
    iqapiMeasurement    *evmAllOffset;	    //!< Average offset EVM for all symbols.
    iqapiMeasurement    *evmSymbols;	    //!< EVM for each symbol in the PSDU.

    // Constellation    
    iqapiMeasurement    *constData;	        //!< Demodulated symbols in the PSDU.

    // Data
    iqapiMeasurement    *phaseNoiseDegRmsAll;//!< RMS phase noise, in degrees.
    iqapiMeasurement    *phaseNoiseDegError;//!< Phase noise in chip duration, in degrees.	

    iqapiMeasurement    *freqOffsetFineHz;	//!< Frequency offset error, in Hz.
    iqapiMeasurement    *symClockErrorPpm;	//!< Symbol clock error in PPM                
    iqapiMeasurement    *eyeDiagramData;	//!< Complex baseband signal in the form of eye diagram.
    iqapiMeasurement    *eyeDiagramTime;	//!< Time points corresponding to eyeDiagramData

    // Code Domain
    iqapiMeasurement    *codeDomain;	    //!< Output of received signal matched with spreading sequences.

    //iqapiMeasurement  *avgPsdu;           //!< Average physical service data unit
    //iqapiMeasurement  *avgShrPhr;         //!< Average synchronization header and physical header

    iqapiMeasurement    *evmPsdu;           //!< Average physical service data unit
    iqapiMeasurement    *evmShrPhr;         //!< Average synchronization header and physical header

    iqapiMeasurement    *evmAllLinear;
    iqapiMeasurement    *evmPsduLinear;     //!< Average physical service data unit in linear
    iqapiMeasurement    *evmShrPhrLinear;   //!< Average synchronization header and physical header
    iqapiMeasurement    *evmAllOffsetLinear;//!< Average offset EVM in linear
    iqapiMeasurement    *evmPsduOffsetLinear;//!< Average physical service data unit offset in linear
    iqapiMeasurement    *evmShrPhrOffsetLinear;//!< Average synchronization header and physical header offset in linear
    iqapiMeasurement    *demodSymbolsOversamp;
    char                *type; 	            //!< Indicates the type of analysis performed. 
};

// ****************************************************************
// Bluetooth Analysis Results Class
// ****************************************************************
//! Specifies analysis results generated with \c iqapiAnalysisBluetooth objects. 
class IQ_API iqapiResultBluetooth : public iqapiResult
{
public:	
    iqapiResultBluetooth(void);	                //!< Constructor
    virtual ~iqapiResultBluetooth(void);	    //!< Destructor

    iqapiMeasurement    *P_av_each_burst;	    //!< Average power of each burst detected, in mWatts.
    iqapiMeasurement	*P_pk_each_burst;	    //!< Peak power of each burst detected, in mWatts.
    iqapiMeasurement    *P_av_all;              //!< Average power of whole capture
    iqapiMeasurement    *P_peak_all;            //!< Peak power of whole capture
    iqapiMeasurement    *P_av_no_gap_all;       //!< Average power, not counting gaps

    iqapiMeasurement    *P_av_each_burst_dBm;  //!< Average power of each burst in dBm
    iqapiMeasurement    *P_pk_each_burst_dBm;  //!< Peak power of each burst in dBm
    iqapiMeasurement    *P_av_all_dBm;         //!< Average power of whole capture in dBm
    iqapiMeasurement    *P_peak_all_dBm;       //!< Peak power of whole capture in dBm
    iqapiMeasurement    *P_av_no_gap_all_dBm;  //!< Average power, not counting gaps in dBm

    iqapiMeasurement	*complete_burst;	    //!< Vector indicating whether or not each burst is complete. All elements are either (1: beginning and end transitions detected), or (0: burst is missing either beginning or end transition).
    iqapiMeasurement	*start_sec;	            //!< Starting time of each burst, in seconds
    iqapiMeasurement	*stop_sec;	            //!< End time of each burst, in seconds
    iqapiMeasurement	*valid;	                //!< Flag indicating whether or not the power portion of the bluetooth analysis was successful (1), or not (0).
	iqapiMeasurement    *basicRateAnalysisValid;//!< Indicates validity of Bluetooth basic rate (1Mbps) measurements
    iqapiMeasurement	*freq_est;	            //!< Initial freq offset of each burst detected, in Hz.
    iqapiMeasurement	*freqEstPacketPointer;	//!< Pointers to which packet is pointed to in each element of freq_est.
    iqapiMeasurement	*freq_drift;	        //!< The maximum difference between the average frequency of the 4 preamble bits and the average frequency of any 10 bits (10 pattern) in the payload field of the returned packets.
    iqapiMeasurement	*freqDriftPacketPointer;//!< Pointers to which packet is pointed to in each element of freq_drift.
    iqapiMeasurement	*bandwidth20dB;	        //!< 20dB bandwidth value Hz.
    iqapiMeasurement	*deltaF1Average;	    //!< The measurement result for deltaF1Avg as specified in BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2. Requires 00001111 data pattern. Result in Hz.
    iqapiMeasurement	*deltaF2Max;	        //!< The measurement result for deltaF2Max as specified in BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2. Requires alternating data pattern. Result in Hz.
    iqapiMeasurement	*deltaF2Average;	    //!< The measurement result for deltaF2Avg as specified in BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2. Requires alternating data pattern. Result in Hz.
    iqapiMeasurement	*deltaF2MaxAccess;	    //!< Similar to the measurement result for deltaF2Max as specified in BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2. Result measured from Access data. Result in Hz.
    iqapiMeasurement	*deltaF2AvAccess;	    //!< Similar to the measurement result for deltaF2Avg as specified in BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2. Result measured from Access data. Result in Hz.
    iqapiMeasurement	*EdrEVMAv;	            //!< RMS Differential EVM value in % (EDR only).
    iqapiMeasurement	*EdrEVMpk;	            //!< Peak Differential EVM value in % (EDR only).
    iqapiMeasurement	*EdrEVMvalid;	        //!< Indicates validity of EDR EVM Measurements.
    iqapiMeasurement	*EdrPowDiffdB;	        //!< Relative power of EDR section to FM section of packet, in dB.
    iqapiMeasurement	*freq_deviation;	    //!< Similar to the measurement result for deltaF1Avg as specified in BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2. Result measured from Header data. Result in Hz.
    iqapiMeasurement	*freq_deviationpktopk;	//!< Peak to Peak Frequency Deviation, in Hz during header.
    iqapiMeasurement	*freqDeviationPointer;	//!< Pointers to which packet is pointed to in each element of the above two measurements.
    iqapiMeasurement	*freq_estHeader;	    //!< Estimates the Frequency Offset during the Header in Hz.
    iqapiMeasurement	*EdrFreqExtremeEdronly;	//!< Reports the extreme value of the frequency variation during DPSK in Hz. Does not include the offset during the header.
    iqapiMeasurement	*EdrprobEVM99pass;	    //!< The percentage of symbols with EVM below the threshold. Threshold for 2 Mbps is 0.3 for 3 Mbps is 0.2.
    iqapiMeasurement	*EdrEVMvsTime;	        //!< Max DEVM Average as specified in: BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2.
    iqapiMeasurement	*validInput;	        //!< Indicates whether or not the input wave was valid.
    char			    *sCaution;              //!< Possible error and warning messages.
    char			    *analysisType;	        //!< Indicates which analysis is performed.
    char			    *versionString;	        //!< Indicates the version of the BT analysis module.
    iqapiMeasurement	*deltaF2MaxTotalCount;   //!< The total number of measured deltaF2Max as specified in BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2. Requires alternating data pattern. Result in Hz.
    iqapiMeasurement	*deltaF2MaxFailCount;   //!< The number of measured deltaF2Max that Failed standard's requirement (below 115kHz) as specified in BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2. Requires alternating data pattern. Result in Hz.

    double              dataRateDetect;	        //!< The detected data rate
    double              dataRateRequest;	    //!< The requested data rate

    iqapiMeasurement    *EdrFreqvsTime;	        //!< Enhanced data rate for frequency versus time

    iqapiMeasurement    *maxfreqDriftRate;	    //!< 1 Mbps only, takes the maximum drift over specified time interval
    iqapiMeasurement    *EdrFreqvsTimeLength;	//!< Number of elements in EdrFreqvsTime
    iqapiMeasurement    *EdrOmegaI;	            //!< \c Omega_i, same as \c freq_estHeader
    iqapiMeasurement    *EdrExtremeOmega0;	    //!< Extreme value of \c Omega_0, same as \c EdrFreqExtremeEdronly
    iqapiMeasurement    *EdrExtremeOmegaI0;	    //!< Extreme value of (Omega_0 + Omega_i)
    iqapiMeasurement    *payloadErrors;	        //!< Reports the number of data errors in Payload. Not counting CRC. If -1, this value has not been calculated. If larger negative number, it reports the length of the payload data vector. This happens when the length of the payload vector is shorter than the length indicated in the payload header.

    int                 acpErrValid;            //!< 1 if ACP results are valid. Otherwise, it will be 0.
    char                *acpErrMsg;             //!< Text string reporting error for ACP calc
    iqapiMeasurement    *maxPowerAcpDbm;        //!< Reports max power in 1 MHz bands at specific offsets from center frequency. The offset in MHz is given in sequenceDefinition. Method according to 5.1.8 TRM/CA/06/C
    iqapiMeasurement    *maxPowerEdrDbm;        //!< Reports max power in 1 MHz bands at specific offsets from center frequency. The power at 0 MHz offset and +/-1 MHz offset is calculated differently from above. maxPowerEDRdBm follows 5.1.15 TRM/CA/13/C
    iqapiMeasurement    *meanNoGapPowerCenterDbm;  //!< Mean power in 1 MHz band. Is no gap power, i.e. only averaged when signal exceeds threshold
    iqapiMeasurement    *sequenceDefinition;    //!< Offset in MHz for results in maxPowerACPdBm and maxPowerEDRdBm

    // low energy boluetooth   
    iqapiMeasurement    *leFreqOffset;          //!< Initial Frequency Offset calculated from preamble (f0 inTRM-LE/CA/05/C). Valid for any LE packet.
    iqapiMeasurement    *leDeltaF1Avg;          //!< Delta F1 Average as defined in TRM-LE/CA/05/C. Valid only if LE payload has 00001111 pattern.
    iqapiMeasurement    *leDeltaF2Max;          //!< Sequence of Delta F2 Max as defined in TRM-LE/CA/05/C. Valid only if LE payload has 10101010 pattern.
    iqapiMeasurement    *leDeltaF2MaxMinValue;  //!< It is the minimum value of the vector LEDeltaF2Max. This is a single value. Double.
    iqapiMeasurement    *leDeltaF2Avg;          //!< Average value of LEDaltaF2Max, i.e. delta F2 Average as defined in TRM-LE/CA/05/C.  Valid only if LE payload has 10101010 pattern.
    iqapiMeasurement    *leBelow185F2Max;       //!< Number of symbols with deviation below 185 kHz
    iqapiMeasurement    *leFn;                  //!< Average frequency over 8 symbols, as defined in TRM-LE/CA/06/C. Valid only if LE payload has 10101010 pattern.
    iqapiMeasurement    *leFnMax;               //!< Maximum value of the absolute value of LEFn. Valid only if LE payload has 10101010 pattern.
    iqapiMeasurement    *leDeltaF0FnMax;        //!< Maximum value of |f0 ?fn|, with n = 2, 3, ? Valid only if LE payload has 10101010 pattern.
    iqapiMeasurement    *leDeltaF1F0;           //!< Absolute value of f1 ?f0. Valid only if LE payload has 10101010 pattern.
    iqapiMeasurement    *leDeltaFnFn_5Max;      //!< Maximum value of |fn ?fn-5|, with n = 6, 7, ? Valid only if LE payload has 10101010 pattern.
    iqapiMeasurement    *leFreqDevSyncAv;       //!< Average freq. deviation during synch. Measured at single sample per symbol after frequency offset during pre-amble has been removed. Valid for any LE packet.
    iqapiMeasurement    *lePduLength;           //!< PDU length in bytes. Valid for any LE packet
    iqapiMeasurement    *leIsCrcOk;             //!< Is 1 if CRC is valid, else 0. Valid output for any LE packet
    iqapiMeasurement    *leValid;               //!< Is 1 if all calculations are valid, else 0. Valid output for any LE packet
    iqapiMeasurement    *leMaxPowerDbm;         //!< Same format as earlier outputs for adjacent channel power. See earlier documentation for maxPowerACPdBm.
    iqapiMeasurement    *leDeltaF2MaxTotalCount;//!< Total number of measured Delta F2 Max as defined in TRM-LE/CA/05/C. Valid only if LE payload has 10101010 pattern.
    char *leMsg;                                 //!< Cell array of messages. Valid output for any LE packet

    iqapiMeasurement    *lePreambleSeq;         //!< Preamble bits 0/1 as defined in 7.2
    iqapiMeasurement    *leSyncWordSeq;         //!< Synch Word bits as defined in 7.2
    iqapiMeasurement    *lePduHeaderSeq;        //!< PDU Header of the test packet as defined in 7.2 
    iqapiMeasurement    *lePduLengthSeq;        //!< PDU Length of the test packet as defined in 7.2
    iqapiMeasurement    *lePayloadSeq;          //!< PDU of the test packet as defined in 7.2
    iqapiMeasurement    *leCrcSeq;              //!< CRC of the PDU of test packet as defined in 7.2

	int					trmca04cValid;			//!< 1 if TRM/CA/04/C Output Spectrum-Frequency range results are valid, else invalid
	iqapiMeasurement	*trmca04cfLHz;			//!< fL Hz for TRM/CA/04/C Output Spectrum-Frequency range
	iqapiMeasurement	*trmca04cfHHz;			//!< fH Hz for TRM/CA/04/C Output Spectrum-Frequency range


};

// ****************************************************************
// Bluetooth Analysis Results Class
// ****************************************************************
//! Specifies analysis results generated with \c iqapiAnalysisBluetooth objects. 
class IQ_API iqapiResultObw : public iqapiResult
{
public:	
    iqapiResultObw(void);	            //!< Constructor
    virtual ~iqapiResultObw(void);	    //!< Destructor

    iqapiMeasurement    *fCentroid;             // In Hz
    iqapiMeasurement    *actualLowerMargin;     // 
    iqapiMeasurement    *actualUpperMargin;
    iqapiMeasurement    *actualPowPart;
    iqapiMeasurement    *upperFreq;             // In Hz
    iqapiMeasurement    *lowerFreq;             // In Hz
    iqapiMeasurement    *actualPowerDbInObw;    // In dBm
    iqapiMeasurement    *obwHz;                 // In Hz

};

class IQ_API iqapiResultAcpr : public iqapiResult
{
public:	
    iqapiResultAcpr(void);	            //!< Constructor
    virtual ~iqapiResultAcpr(void);	    //!< Destructor

    iqapiMeasurement    *powerValues;       // In dBm
    iqapiMeasurement    *powerRatios;

};


// ****************************************************************
// VSG Parameters Class - applies individually to each test system. 
// ****************************************************************
//! Specifies transmitter parameters that are specific for each Vector Signal Generator.
class IQ_API iqapiVsg
{
public:
    iqapiVsg(void);	            //!< Constructor
    virtual ~iqapiVsg(void);	//!< Destructor
    
    IQV_RF_ENABLE_ENUM  enabled;    //!< Enables or disables RF
    /*!< Available options are as follows;
    IQV_RF_DISABLED indicates that the RF is disabled. 
    IQV_RF_ENABLED indicates that the RF is enabled
    */
    IQV_PORT_ENUM	    port;		//!< Represents RF N-connector port selection
    /*!< Available options are as follows:
    IQV_PORT_OFF   = 1 indicates that the port is disabled. 
    IQV_PORT_LEFT  = 2 indicates that the RF uses the  left port.      
    IQV_PORT_RIGHT = 3 indicates that the RF uses the right port. 
    */
    IQV_SOURCE_ENUM     source;	    //!< Represents the signal source of all VSGs.
    /*!< Available options are as follows:
    IQV_SOURCE_WAVE represents internal modulation from wave.
    IQV_SOURCE_SIGNAL_GENERATOR represents internal modulation from CW signal generator.                                      
    IQV_SOURCE_UNDEFINED represents undefined default modulation source.
    */
    double		        sineFreqHz; //!< This field represents the VSG signal generator's sine wave frequency. Only applies if source is set to IQV_SOURCE_SIGNAL_GENERATOR.
    double		        rfGainDb;   //!< Represents RF gain in dB.
    double		        bbGainDb;   //!< Represents BB gain in dB.
    double		        dcErrI;	    //!< Represents signal modulation DC offset (I Channel)
    double		        dcErrQ;	    //!< Represents signal modulation DC offset (Q Channel)
    double		        phaseErr;	//!< Represents signal generator's phase error
    double		        gainErr;	//!< Represents signal generator's gain error
    double		        delayErr;	//!< Represents signal generator's delay error
    double		        cmvI;		//!< Represents common mode voltage setting for the I channel
    double		        cmvQ;		//!< Represents common mode voltage setting for the Q channel
    int			        powerOutOfRange;  //!< Flag that indicates if VSG power level set by user is outside of valid range
    double              fpgaLoadCalTable; //!< Specifies values for loading FPGA calibration table 
    /*!< This parameter applies to the LitePoint test system hardware versions 1.6.x version or higher.
    \param[in] 0 = Uses middleware compensation method; calibration is always available. Default: 0 for HW1.3, 1.4, 1.5 and 1.6. Default: 1 for all other testers
    \param[in] 1 = Uses FPGA real-time compensation; calibration may not be available
    \return 2  = Tables have been downloaded
    \return -1 = Tables are not available
    */
    iqapiMeasurement    *timeGapUpSec;  //!< Position(s) in downloaded waveform where power should be turned on (in seconds)
    /*!< This is typically at the beginning of a packet; applies only if \c tx.gapPowerOff is true
    */
    iqapiMeasurement    *timeGapDnSec;  //!< Position(s) in downloaded waveform where power should be turned off (in seconds)
    /*!< This is typically at the end of a packet; applies only if \c tx.gapPowerOff is true
    */ 
    IQV_MIRROR_FREQ_ENUM mirrorFreq;   

    IQV_COMP_SINC_ENUM  compSinc;       // For IQ201x only
};

// ****************************************************************
// Tx Parameters Class - applies to all test systems (except member vsg)
// ****************************************************************
//! Specifies parameters for transmitting signals.
class IQ_API iqapiTx 
{
public:
    iqapiTx(void);	        //!< Constructor
    virtual ~iqapiTx(void);	//!< Destructor
    
    IQV_INPUT_MOD_ENUM		txMode;	//!< Represents the VSG signal transmission mode
    /*!< Available options are as follows:
    IQV_INPUT_MOD_DAC_RF indicates RF transmit mode                                                          
    IQV_INPUT_MOD_DAC_BB indicates BB transmit mode                                              
    IQV_INPUT_MOD_UNDEFINED indicates an undefined default value
    */
    double			        rfFreqHz;           //!< This field represents the RF frequency for the VSGs,in Hz.
    /*!< For IQview/flex/nxn systems valid values are in the 2.4 and 4.9-6.0 GHz 								ranges, in increments of 1 MHz. */
    double					sampleFreqHz;
    IQV_MODULATION_CONTROL_ENUM	modulationMode;	//!< Represents VSG modulation mode.
    /*!<Available options are as follows:
    IQV_WAVE_DL_MOD_DISABLE indicates that the modulation is halted after the wave file is downloaded unless it is in continuous Tx mode.  
    IQV_WAVE_DL_MOD_ENABLE indicates that modulation is continued after the wav file is downloaded
    */
    iqapiVsg				*vsg[N_MAX_TESTERS];//!< Represents individual VSG parameters	
    IQV_GAP_POWER			gapPowerOff;        //!< Controls VSG power during signal gaps (idle time)
    
    /*!< Used for backward compatibility */
    IQV_ALC_MODES			alcMode;            //!< Automatic level control
    IQV_VSG_TRIG            triggerType;        //!< VSG trigger
    IQV_VSG_TRIG_REARM      triggerReArm;       //!< Indicates if VSG needs to be re-armed after the trigger event
    double                  freqShiftHz;        //!< Tx IF Frequency in Hz 
    /*!< Before downloading to the VSG, the waveform is shifted by the indicated value in Hz; this can be used to get higher resolution on systems that support only 1 MHz resolution
    */
    IQV_LO_PORT_ENUM        c_loRfPort;         //!< Indicates ENUM value for RF LO port for nxn plus system only
    /*!<RF LO is available for hardware versions 1.6.2 and higher
    */
    IQV_LO_PORT_ENUM        c_loIfPort;         //!< Indicates ENUM value for IF LO port. Not used.  
    double                  waveIntervalSecs;   //!< Used for padding the wave with zero by the \c waveIntervalSecs time; 
    /*!<The default value is 0
    */

};

// ****************************************************************
// VSA Parameters Class - applies to each test system individually
// ****************************************************************
//! Specifies receiver parameters that are specific for each Vector Signal Analyzer.
class IQ_API iqapiVsa
{
public:
    iqapiVsa(void);	                //!< Constructor
    virtual ~iqapiVsa(void);	    //!< Destructor
    
    IQV_RF_ENABLE_ENUM	enabled;	//!< Specifies whether VSA RF input is enabled or disabled
    
    /*!< Available options are as follows:
    IQV_RF_DISABLED indicates that RF is disabled. 
    IQV_RF_ENABLED indicates that RF is enabled.
    */
    IQV_PORT_ENUM		port;	    //!< Represents RF N-connector port selection 
    /*!<
    Available options are as follows:
    IQV_PORT_OFF   = 1 indicates that the port is disabled. 
    IQV_PORT_LEFT  = 2 indicates that the RF uses the left port.      
    IQV_PORT_RIGHT = 3 indicates that the RF uses the right port.  
    */
    double				rfAmplDb;	//!< Represents the RF amplitude in dBm 
    /*!<
    \note This parameter replaces the \a rfGainDb; parameter.    
    */
    double				bbAmplDbv;	//!< Represents the Baseband amplitude in dBV
  
    double				bbGainDb;	//!< Represents baseband gain in dB.  	

   IQV_MIRROR_FREQ_ENUM mirrorFreq; //!< Mirrors the center frequency in the frequency spectrum of the captured signal. Calculates the complex conjugate of the signal
   /*!< Available options are as follows:
   IQV_MIRROR_FREQ_DISABLED indicates that mirror frequency spectrum is disabled.                          
   IQV_MIRROR_FREQ_ENABLED indicates that mirror frequency spectrum is enabled.
   */
   double               extAttenDb; //!< Represents external attenuation in dB. 
};

// ****************************************************************
// Rx Parameters Class - applies to all test system (except member vsa)
// ****************************************************************
//! Specifies parameters for receiving the signal.
class IQ_API iqapiRx 
{
public:
    iqapiRx(void);	                //!< Constructor
    virtual ~iqapiRx(void);	        //!< Destructor

	iqapiRx(const iqapiRx &src);
    iqapiRx & operator = (const iqapiRx & src);

    IQV_INPUT_ADC_ENUM  rxMode;		//!< Capture mode for the VSAs.
    /*!< This field represents the capture mode for the VSAs. 
    Available options are as follows:
    IQV_INPUT_ADC_RF, which represenst data captured from RF
    IQV_INPUT_ADC_BB, which represents data captured from base band
    IQV_INPUT_ADC_UNDEFINED, which represents undefined default value
    IQV_GET_RAW_DATA = -2, which represents getting the raw data with no compensation
    */
    double		rfFreqHz;		    //!< This field represents the RF Frequency for the VSAs,in Hz. 
    /*!< For IQview/flex/nxn systems valid values are in the 2.4 and 4.9-6.0 GHz ranges, in increments of 1 MHz. 
    */
    double		freqShiftHz;		//!< This field represents the Rx IF frequency in Hz 
    /*!< After capturing by the VSA(s), the signal is shifted by the indicated value; this can be used to analyze signals at intermediate frequencies not directly supported by the instrument */
    /*Rx Intermediate Frequency for all VSAs
    */
    double		samplingTimeSecs;	//!< Sampling time, in seconds, for all VSAs.
    double		sampleFreqHz;		//!< ADC sampling frequency in Hz, for all VSAs. Recommended frequency is 80 MHz.		
    IQV_TRIG_TYPE_ENUM	triggerType;//!< This field represents the trigger type used for the VSA master. 
    /*!<The VSA slaves (including the VSG master), are always triggered by the VSA master. 
    */
    double		triggerLevelDb;		//!< Signal (IF) trigger level in dB, relative to VSA master's \c rfAmplDb setting.
    double		triggerPreTime;		//!< Amount of data to capture before the trigger point, in seconds.
    double		triggerTimeOut;		//!< Trigger timeout in seconds.
    iqapiVsa	*vsa[N_MAX_TESTERS];//!< Parameters unique to each vsa 
    
    IQV_RX_POWER_MODES	powerMode;	//!< Represents the AGC power mode.
    /*!< Available options are as follows:
    IQV_VSA_TYPE_0, where VSA Type 0 is the default value and represents RMS Power Mode.
    IQV_VSA_TYPE_1, where VSA Type 1 is the recommended value and represents Peak Power Mode.
    */

private :
	void Init();
	void DeepCopy(const iqapiRx &object);
};

// ****************************************************************
// Data Capture Class
// ****************************************************************
//! Specifies parameters to capture a signal. 
class IQ_API iqapiCapture : public iqapiSignalData
{
public:	
    iqapiCapture(void);	//!< Constructor
    virtual ~iqapiCapture(void);	//!< Destructor
    //Copy constructor
    iqapiCapture(const iqapiCapture &src);		//!< A Copy Constructor that can be used to make a deep copy of an iqapiCapture object. Use this constructor when you wish to create a copy of an iqapiCapture object.
    iqapiCapture & operator = (const iqapiCapture & src);

    //int         length[N_MAX_TESTERS];        //!< This integer array represents the length of the real and imag vectors described below (one integer for each VSA in the LitePoint Test Instrument(s)). Length[0] indicates the length of \c real[0] and \c imag[0]  and is the sample data returned from the first VSA (0).
    //double      *real[N_MAX_TESTERS];	        //!< This double pointer array represents the (real) sample data captured on each VSA. The length of real[x] is indicated by length[x], where x is the VSA number (minus one).
    //double      *imag[N_MAX_TESTERS];	        //!< This double pointer array represents the (imaginary) sample data captured on each VSA. The length of imag[x] is indicated by length[x], where x is the VSA number (minus one).
    //double      sampleFreqHz[N_MAX_TESTERS];	//!< This double array represents the sample frequency of real and imag. sampleFreqHz[x] is the sampling rate of real[x] and imag[x], where x is the VSA number (minus one).
    
    int         adc_clip[N_MAX_TESTERS];		//!< This integer array indicates whether or not clipping occurred during the data capture stored in \c real and \c imag. \c adc_clip[x] indicates clipping in \c real[x] and \c imag[x], where x is the VSA number (minus one).

    // Text of last error
    char		lastErr[MAX_LEN_ERR_TXT];	    //!< Saves the iqapiCapture object to a \c .sig file, (set by filename) which can be read and analyzed by IQsignal for MIMO. This function returns 0 (IQAPI_ERR_OK) if successful; if it returns a value that is less than 0, then it indicates a warning and if it returns a value that is greater than 0, then it indicates an error (see IQAPI_ERR_CODES, or the \c hndl->lastErr buffer).

	int Save(char *fileName) { return Save(fileName, IQV_VSA_NUM_ALL); };
    int Save(char *fileName, IQV_VSA_NUM_ENUM vsaNum) { return Save(fileName, IQV_DATA_IN_IQAPI, vsaNum); };
    int	Save(char *fileName, IQV_CAPTURE_DATA_HANDLING_ENUM captureDataHandling, IQV_VSA_NUM_ENUM vsaNum = IQV_VSA_NUM_ALL); 	//!< Saves the captured signal in a file (.sig)	
    int SaveNormalize(char *fileName, IQV_CAPTURE_DATA_HANDLING_ENUM captureDataHandling = IQV_DATA_IN_IQAPI, int lengthOfNormFactorDb=0, double *normFactorDb=NULL); //!< Saves normalized captured signal to a file (typically, a \c .mod file); this file can be used as a generator file to download to the VSG.

    int PlotPower(int figNum, char *plotArgs, int vsaNum);
    /*! Displays the signal contents of an \c iqapiCapture object during the debug and development phases. The data will first be converted to dBm before plotting. This function returns 0 (IQAPI_ERR_OK) if successful; if it returns a value that is less than 0, then it indicates a warning and if it returns a value that is greater than 0, then it indicates an error (see IQAPI_ERR_CODES, or the \c hndl->lastErr buffer).
    */

    // Internal use
    void  SetCaptureType(IQV_CAPTURE_TYPE_ENUM capType);
    IQV_CAPTURE_TYPE_ENUM GetCaptureType();

    double   reserved[5];
    iqapiRx  *rxAtCapture;            // Used for plotting purpose. Will be saved into .sig file as well. Retrieved during LoadSignalFile, Capture.

private:
	int Save(char *fileName, enum IQV_SAVE_FILE_TYPE_ENUM eFileType, IQV_CAPTURE_DATA_HANDLING_ENUM captureDataHandling = IQV_DATA_IN_IQAPI, int lengthOfNormFactorDb=0, double *normFactorDb = NULL, IQV_VSA_NUM_ENUM vsaNum = IQV_VSA_NUM_ALL);
   void DeepCopyIqapiCapture(const iqapiCapture &object);

    
   IQV_CAPTURE_TYPE_ENUM  captureType;

};

class IQ_API iqapiCaptureHT40Data3
{
public:
   iqapiCaptureHT40Data3();
   ~iqapiCaptureHT40Data3();

   double *realCenter[N_MAX_TESTERS];       //!< Real part of data for center captured data
   double *imagCenter[N_MAX_TESTERS];       //!< Imag part of data for center captured data
   int    lengthCenter[N_MAX_TESTERS];      //!< Number of data for center captured data
   double frequencyCenter[N_MAX_TESTERS];   //!< Center captured frequence used in Hz 

   double *realLeft[N_MAX_TESTERS];         //!< Real part of data for left captured data
   double *imagLeft[N_MAX_TESTERS];         //!< Imag part of data for left captured data
   int    lengthLeft[N_MAX_TESTERS];        //!< Number of data for left captured data
   double frequencyLeft[N_MAX_TESTERS];     //!< Left captured frequence used in Hz 

   double *realRight[N_MAX_TESTERS];        //!< Real part of data for right captured data
   double *imagRight[N_MAX_TESTERS];        //!< Imag part of data for right captured data
   int    lengthRight[N_MAX_TESTERS];       //!< Number of data for right captured data
   double frequencyRight[N_MAX_TESTERS];    //!< Right captured frequence used in Hz 

   double  sampleFreqHz[N_MAX_TESTERS];	//!< This double array represents the sample frequency of real and imag. sampleFreqHz[x] is the sampling rate of real[x] and imag[x], where x is the VSA number (minus one).

   int Save(char *fileName);    //!< save the captured data in HT40 type

   char lastErr[MAX_LEN_ERR_TXT];
};

//! Specifies parameters for the signal frequency range.
struct IQ_API iqapiFreqBands
{
   double   freqMin;	//!< Indicates minimum signal frequency
   double   freqMax;	//!< Indicates maximum signal frequency
};

//! Specifies parameters for VSA and VSG calibration; middleware table.
class IQ_API mwTable
{
private:
    double  *table;
    int		numberOfRow;
    int		numberOfCol;

public:
   mwTable();	//!< Constructor
   virtual ~mwTable();	//!< Destructor
   void SetTable(double *tbl);
   void SetNumberOfRow(int numOfRow) { numberOfRow = numOfRow; };
   void SetNumberOfCol(int numOfCol) { numberOfCol = numOfCol; };

   const double *GetTable() { return table; };
   int	GetNumberOfRow()    { return numberOfRow;};
   int	GetNumberOfCol()    { return numberOfCol;};
};

class IQ_API DualHead : public DualHeadBase
{
public:
   DualHead(iqapiHndlBase *hndl);
   virtual ~DualHead();

   bool         SetToken (unsigned int token, unsigned int value);    
   unsigned int GetToken (unsigned int token);

   void         DoTesterExtraSetting();
};


#ifdef _INCLUDE_GF_
class IQ_API DualHeadGps : public DualHeadBase
{
public:
   DualHeadGps(iqapiHndlBase *hndl);

   virtual unsigned int GetToken (unsigned int token);
   virtual bool         SetToken (unsigned int token, unsigned int value);    
};

class IQ_API DualHeadFm : public DualHeadBase
{
public:
   DualHeadFm(iqapiHndlBase *hndl);

   virtual unsigned int GetToken (unsigned int token);
   virtual bool         SetToken (unsigned int token, unsigned int value);    
};
#endif // _INCLUDE_GF_

// ****************************************************************
// iqapi API Handle Class - Main Object
// ****************************************************************
//! Specifies parameters to control the test system hardware, perform signal analysis and generate waveform data.
class IQ_API iqapiHndl : public iqapiHndlBase
{
private:
   int A21SequenceLimitation;

public:
    iqapiHndl(void);            //!< Constructor
    virtual ~iqapiHndl(void);   //!< Destructor

    // ****************************************************************
    // Connection Handling Functions
    // ****************************************************************    
    int ConInitBySerialNumber(char *ip1, char *sn1, char *sn2, char *sn3=NULL, char *sn4=NULL, char *ip2 = NULL, char *ip3 = NULL, char *ip4 = NULL);   //!< For IQ2010 used as nxn system, if tester2(sn2) is on the different PC with ip2, ip2 must be specified. ip3 and ip4 have the same requirement.
	int ConInitBySerialNumber(char *ip1, char *sn1, IQV_CONNECTION_TYPE connectionType = IQV_CONNECTION_ANY_TYPE); //!< For IQ201x only. Allow to specify which tester ( if more than one tester connected to the PC) to be connecetd. Note: By default, the tester connected has the port number to be 4000. For connecting to the second tester, the port number has to be different than 4000. Ex., Say 4001, then, ip1 should be specified along with a port number. like, 127.0.0.1:4001
	int ConInit(char *ip1, IQV_CONNECTION_TYPE connectionType = IQV_CONNECTION_ANY_TYPE); //!< Connects to and initializes a single test system.		
    /*!<Initializes a connection to a test system. The input must be a valid IP address, such as 192.168.100.254. This function returns 0 (IQAPI_ERR_OK) if successful; if it returns a value that is less than 0, then it indicates a warning and if it returns a value that is greater than 0, then it indicates an error (see IQAPI_ERR_CODES, or the \c hndl->lastErr buffer).
    */
    int ConInit(char *ip1, char *ip2); //!< Connects to and initializes two test systems.
    /*!<Initializes a connection to a test system unit that contains two IQnxn test systems. The input must be a valid IP address such as 192.168.100.254. This function returns 0 (IQAPI_ERR_OK) if successful; if it returns a value that is less than 0, then it indicates a warning and if it returns a value that is greater than 0, then it indicates an error (see IQAPI_ERR_CODES, or the \c hndl->lastErr buffer).
    */
    int ConInit(char *ip1, char *ip2, char *ip3);//!< Connects to and initializes three test systems.
    /*!< Initializes a connection to a test system unit that contains three IQnxn test systems. The input must be a valid IP address such as 192.168.100.254. This function returns 0 (IQAPI_ERR_OK) if successful; if it returns a value that is less than 0, then it indicates a warning and if it returns a value that is greater than 0, then it indicates an error (see IQAPI_ERR_CODES, or the \c hndl->lastErr buffer).
    */
    int ConInit(char *ip1, char *ip2, char *ip3, char *ip4); //!< Connects to and initializes four test systems.
    /*!< Initializes a connection to a test system unit that contains four IQnxn Test Instruments. The input must be a valid IP addresses such as 192.168.100.254. This function returns 0 (IQAPI_ERR_OK) if successful; if it returns a value that is less than 0, then it indicates a warning and if it returns a value that is greater than 0, then it indicates an error (see IQAPI_ERR_CODES, or the \c hndl->lastErr buffer).
    */ 
    int ConOpen(char *ip1=NULL, char *ip2=NULL, char *ip3=NULL, char *ip4=NULL, IQV_CONNECTION_TYPE connectionType = IQV_CONNECTION_ANY_TYPE);  //!< Connects to or reconnects to the test systems.
    /*!< Reopens a connection to a test system which was previously established using the \c ConInit() function. This function returns 0 (IQAPI_ERR_OK) if successful; if it returns a value that is less than 0, then it indicates a warning and if it returns a value that is greater than 0, then it indicates an error (see IQAPI_ERR_CODES, or the \c hndl->lastErr buffer).
    \note Entering IP addresses is optional. If the IP address has been provided previously by the \c ConInit() function, then you do not have to enter the IP address.
    */   	
    int ConClose();     //!< Closes one or more connections to the test systems.
    /*!< Closes a connection to a test system which was previously established with the \c ConInit function or opened with the \c ConOpen() function. This function returns 0 (IQAPI_ERR_OK) if successful; if it returns a value that is less than 0, then it indicates a warning and if it returns a value that is greater than 0, then it indicates an error (see IQAPI_ERR_CODES, or the \c hndl->lastErr buffer).
    */  
    bool ConValid();    //!< Indicates valid, open connection.
    /*!< Checks the validity of a connection to a test system. This function returns a value of true if the connection is open and valid, or false if the connection is closed or invalid.
    */

    // Reserved for future use - DO NOT USE
    int ConInit2(char *ip1);                                    //!< Connects to and initializes a single test system. This function is reserved for internal use.
    int ConInit2(char *ip1, char *ip2);                         //!< Connects to and initializes two test systems. This function is reserved for internal use.
    int ConInit2(char *ip1, char *ip2, char *ip3);              //!< Connects to and initializes three test systems. This function is reserved for internal use.
    int ConInit2(char *ip1, char *ip2, char *ip3, char *ip4);   //!< Connects to and initializes four test systems. This function is reserved for internal use.  
    // End Reserved

    int GetAllVersions(char *version, int versionSize);   //!< Gets all versions related to software and hardware of the test system
    int GetIqapiVersion(char *version, int versionSize);
#ifdef _INCLUDE_GF_
    int GetGpsFmVersion(char *version, int versionSize);
    int GetIqFmVersion(char *version, int versionSize);
#endif // _INCLUDE_GF_
    int GetHardwareVersion(char *buff, int bufflen, int testerNumber = 1);  //!< Gets the hardware version of the test system.
    int GetFirmwareVersion(char *buff, int bufflen, int testerNumber = 1);  //!< Gets the firmware version of the test system.
    int GetCalibrationDate(char *buff, int bufflen, int testerNumber = 1);  //!< Gets the calibration date of the test system.
    int GetSerialNumber(char *buff, int bufflen, int testerNumber = 1);     //!< Gets the serial number of the tester. buffer length has to be at least 9 in length. ex.: IQP00100
    int GetFpgaVersion(char *buff, int bufflen, IQV_FPGA_TYPE fpgaType, int testerNumber = 1);   //!< Gets FPGA version for the specified FPGA type 

    // ****************************************************************    
    // Set-up and Configuration Functions
    // ****************************************************************    
    int SetDefault();//!< Resets and initializes test systems
    /*!< Resets a test system, and initializes the associated objects in an \c iqapiHndl object. The test system and software are set to default conditions.
    \note This function does not have to be called after calling the \c ConInit() function. The \c ConInit() function resets and initializes the test system and therefore the SetDefault() function does not have to be called after calling the SetInit() function. This function returns 0 (IQAPI_ERR_OK) if successful; if it returns a value that is less than 0, then it indicates a warning and if it returns a value that is greater than 0, then it indicates an error (see IQAPI_ERR_CODES, or the \c hndl->lastErr buffer).
    */
    int SetTx(); 	//!< Sets Tx configuration. Note: Should use SetTxRx() instead
    /*!< Applies Tx settings to a test system, as configured in the \c iqapiTx object in \c iqapiHndl (\c hndl->tx). This function returns 0 (IQAPI_ERR_OK) if successful; if it returns a value that is less than 0, then it indicates a warning and if it returns a value that is greater than 0, then it indicates an error (see IQAPI_ERR_CODES, or the \c hndl->lastErr buffer).
    */
    int SetRx(); //!< Sets Rx configuration. Note: Should use SetTxRx() instead.
    /*!< Applies Rx settings to a test system, as configured in the \c iqapiRx object in \c iqapiHndl (\c hndl->rx). This function returns 0 (IQAPI_ERR_OK) if successful; if it returns a value that is less than 0, then it indicates a warning and if it returns a value that is greater than 0, then it indicates an error (see IQAPI_ERR_CODES, or the \c hndl->lastErr buffer).
    */
    int FrameTx(int numFrames);    //!< Starts frame Tx mode for the number of frames indicated in the \c numFrames parameter
    /*!< Transmits the number of frames indicated in the \c numFrames parameter from the test system VSG. 
    \n The \c numFrames parameter can have an integer value between 1 and 65534.
    \n If \c numFrames=0, then the VSG  resumes continuous transmission. 
    \n This function returns a value of 0 (IQAPI_ERR_OK) if successful; if it returns a value that is less than 0, then it indicates a warning and if it returns a value that is greater than 0, then it indicates an error (see IQAPI_ERR_CODES, or the \c hndl->lastErr buffer).
    */
    int GetStatus(); //!< Retrieves hardware status from the test system.
    /*!< Retrieves the current hardware status of a test system. The hardware-related objects in \c iqapiHndl will become updated with the current settings on the hardware. This could be used, for instance, to detect if another application has changed some settings on the system. This function returns a value of 0 (IQAPI_ERR_OK) if successful; if it returns a value that is less than 0, then it indicates a warning and if it returns a value that is greater than 0, then it indicates an error (see IQAPI_ERR_CODES, or the \c hndl->lastErr buffer).
    */ 
    bool TxDone(int *errCode);    //!< Indicates if  the frame Tx mode is complete
     /*!< Retrieves the current Tx status of the VSGs in the test systems. This function returns true if a \c FrameTx() operation is done, or false if the VSGs are still transmitting (which will be the case during continuous Tx). The integer pointed to by \c errCode will become updated with the current error level, 0, (IQAPI_ERR_OK) if successful; if it returns a value that is less than 0, then it indicates a warning and if it returns a value that is greater than 0, then it indicates an error (see IQAPI_ERR_CODES, or the \c hndl->lastErr buffer).
    */
    int Agc(bool AgcAll);    //!< Performs autorange, optionally, individually for all test systems.
    /*!< Performs automatic gain control (AGC) on the VSAs in a test system. The \c iqapiRx objects of an \c iqapiHndl will get updated with the new settings, if successful. Setting the \c AgcAll value to \c TRUE causes an AGC to be performed on all the VSAs in the system; a value of \c FALSE causes an AGC to be performed only on the VSA Master. The remaining IQnxn Test Instrument VSAs will be set to the same value as the VSA Master. This function returns 0 (IQAPI_ERR_OK) if successful; if it returns a value that is less than 0, then it indicates a warning and if it returns a value that is greater than 0, then it indicates an error (see IQAPI_ERR_CODES, or the \c hndl->lastErr buffer)
    */
    int SetTxRx();    //!< Sets Tx and Rx configurations  
    /*!< Applies Tx and Rx settings to a test system, as configured in the \c iqapiTx and \c iqapiRx objects in \c iqapiHndl (\c hndl->tx and \c hndl->rx). This function returns 0 (IQAPI_ERR_OK) if successful; if it returns a value that is less than 0, then it indicates a warning and if it returns a value that is greater than 0, then it indicates an error (see IQAPI_ERR_CODES, or the \c hndl->lastErr buffer).
    */

    // ****************************************************************    
    // Wave Generation and Downloading Functions
    // ****************************************************************    
    int GenerateWave(iqapiWaveGenParms *waveGen); //!< Generates Tx modulation waveform 
    /*!< Generates modulation data to be downloaded to the VSGs in a test system. If successful, the \c iqapiModulationWave object in \c iqapiHndl (hndl->wave) gets updated with the waveform data specified by the configuration in the \c waveGen parameter. This function returns 0 (IQAPI_ERR_OK) if successful; if it returns a value that is less than 0, then it indicates a warning and if it returns a value that is greater than 0, then it indicates an error (see IQAPI_ERR_CODES, or the \c hndl->lastErr buffer).
    */	
    int SetWave(iqapiSignalData *modWave = NULL, bool autoPlay = true); //!< Downloads waveform to the VSG using data generated by the GenerateWave() function
    /*!< Downloads the \c iqapiModulationWave object currently pointed to by the \c hndl->wave object, to the VSGs in a test system. 
    \note If the number of streams is less than the number of test systems, zeros will be downloaded to the remaining VSG test system. Also, if the number of streams is greater than the number of test systems, the remaining streams will be ignored in the download. This function supports legacy IQview software \c .mod files as well as the IQapi software \c .mod files. This function returns 0 (IQAPI_ERR_OK) if successful; if it returns a value that is less than 0, then it indicates a warning and if it returns a value that is greater than 0, then it indicates an error (see IQAPI_ERR_CODES, or the \c hndl->lastErr buffer).
    */

    int SetWave(char *fileName, bool autoPlay = true); //!< Downloads waveform data to VSG loaded from a file 
    /*!< 
     * \param[in] fileName signal file name to be loaded to tester
     * \param[in] autoPlay if true (default), loaded signal file will be loaded to tester and played automatically at VSG. if false, file will only be loaded to tester, not to be played at VSG until FrameTx() function is called
     * \return    Performs the same operations as \c SetWave, and additionally sets marker information in the VSG. This function returns 0 (IQAPI_ERR_OK) if successful; if it returns a value that is less than 0, then it indicates a warning and if it returns a value that is greater than 0, then it indicates an error (see IQAPI_ERR_CODES, or the \c hndl->lastErr buffer).
    */
    int SetWaveWithSegments(char *fileName); 

    // ****************************************************************    
    // Multi-wave add, download, select Functions
    // ****************************************************************    
    int MultiWaveAddByFile(char *fileName, int *index); //!< Adds a waveform from a modulation file to cache
    int MultiWaveAddCw(double frequency, int *index, double*frequencyActual); //!< Adds a continuous waveform to cache
    int MultiWaveAddByWave(iqapiModulationMultiWave *multiwave, int *indexWave); //!< Adds a waveform from RAM to cache
    int MultiWaveDownload();            //!< Downloads a multi-waves from cache to active memory 
    int MultiWaveSelect(int index);     //!< Selects a waveform by index
    int MultiWaveClear();               //!< Clears all the cached waveforms; does not clear the waveform running on the VSG
    int MultiWaveGetDescription();      //!< Gets the description of the multiwave waveform

    int  mulWaveDescNumOfWave;	        //!< Indicates description of the number of multiwaves
    double  *mulWaveDescMenuIndexes;    //!< Indicates the description of the menu indexes
    char mulWaveDescFull[32][256];      //!< Indicates description of the multiwave waveform
    char mulWaveDescMenuTxt[32][256];   //!< Indicates description of the menu text
    char mulWaveDescWaveName[32][256];  //!< Indicates description of the name of the multiwave waveform

    // ****************************************************************    
    // Data Capture and Analysis Functions
    // ****************************************************************    
    int Capture();  //!< Performs a data capture
    /*!< Performs a data capture using the current settings applied in \c hndl->rx. If successful, \c hndl->data will point to an \c iqapiCapture object which contains the sample data. This function returns 0 (IQAPI_ERR_OK) if successful; if it returns a value that is less than 0, then it indicates a warning and if it returns a value that is greater than 0, then it indicates an error (see IQAPI_ERR_CODES, or the \c hndl->lastErr buffer).
    */	
    int Capture(int vsaNum); //!< Performs a data capture on a single VSA in an IQnxn test system configuration
    /*!< In an IQnxn configuration, this function performs a data capture using the current settings applied in a \c hndl->rx object, on only the requested VSA (as specified by \c vsaNum). If successful, the \c hndl->data object will point to an \c iqapiCapture object which contains the sample data. This function returns 0 (IQAPI_ERR_OK) if successful; if it returns a value that is less than 0, then it indicates a warning and if it returns a value that is greater than 0, then it indicates an error (see IQAPI_ERR_CODES, or the \c hndl->lastErr buffer).
    */	
    int Capture(double sampleTimeSecs);	//!< Performs a data capture of length indicated in the \c sampleTimeSecs parameter
    /*!< This function is implemented in the header file, and calls \c Capture(), but sets the sampling time first.
    */

    int Capture(iqapiCapture *userData); //!< Performs a data capture using the current settings applied to the  \c hndl->rx object
    /*!< Performs a data capture using the current settings applied in \c hndl->rx. If successful, userData will point to an iqapiCapture object which contains the sample data. This function returns 0 (IQAPI_ERR_OK) if successful; if it returns a value that is less than 0, then it indicates a warning and if it returns a value that is greater than 0, then it indicates an error (see IQAPI_ERR_CODES, or the \c hndl->lastErr buffer)
    \note \c userData must be created before calling this function.
    */
    int ContCapture(IQV_DC_CONT_ENUM mode, int vsaNum = 1); //!< Performs continuous data capture modes; this function is reserved for future use
    int Analyze(iqapiSignalData *userData); //!< Analyzes the data from the user using the current settings
    /*!< Performs analysis on the \c iqapiCapture object pointed to by \c userData, using the analysis parameters pointed to by the \c hndl->analysis object. If successful, the \c hndl->results object will point to a result object (derived from iqapiResult) of the type specified by the \c hndl->analysis object. For instance, if after a data capture \c hndl-analysis points to an \c iqapiAnalysisOFDM object, \c hndl->analyze object will cause \c hndl->results object to point to an object of type \c iqapiResultOFDM. This function returns 0 (IQAPI_ERR_OK) if successful; if it returns a value that is less than 0, then it indicates a warning and if it returns a value that is greater than 0, then it indicates an error (see IQAPI_ERR_CODES, or the \c hndl->lastErr buffer).
    */
    int Analyze(); //!< Analyzes the current capture buffer using the current settings
    /*!< Performs analysis on the \c iqapiCapture object pointed to by the \c hndl->data object, using the analysis parameters pointed to by the \c hndl->analysis object. If successful, the \c hndl->results object will point to a result object (derived from the \c iqapiResult object) of the type specified by the \c hndl->analysis object. For instance, if after a data capture the \c hndl-analysis object points to an \c iqapiAnalysisOFDM object, the \c hndl->analyze object will cause the \c hndl->results object to point to an object of type \c iqapiResultOFDM. This function returns 0 (IQAPI_ERR_OK) if successful; if it returns a value that is less than 0, then it indicates a warning and if it returns a value that is greater than 0, then it indicates an error (see IQAPI_ERR_CODES, or the \c hndl->lastErr buffer).
    */
    int LoadSignalFile(char *fileName); //!< Loads a signal file

    // ****************************************************************    
    // Multiport Test Adaptor Functions
    // ****************************************************************    
    int    MptaEnable(void); //!< Enables MPTA control
    int    MptaDisable(void); //!< Disables MPTA control
    int    MptaSetupSwitchCapture(MPTA_TX_TEST *tx_test); //!< Performs Sequential MIMO setup and capture
    int    MptaSwitchCapture(void); //!< Performs Sequential MIMO capture only
    int    MptaSetAttn(int port, double attn_dB, double freq_Hz, double *attnError = NULL); //!< Sets attenuation value to individual ports (0-3), and returns attenuation error
    int    MptaSetAttn(double attn_dB, double freq_Hz, double *attnErrors = NULL ); //!< Sets attenuation value to all ports and returns attenuation error
    double MptaRxPer(MPTA_RX_TEST *rx_test); //!< Performs Rx PER test at specified signal level and returns a PER % value (0-100)
    double MptaRxSens(MPTA_RX_TEST *rx_test); //!< Performs sensitivity level test and returns sensitivity value (dBm)
    void   MptaPrintDebug(bool bEnable);
    void   MptaExtTrigger(double fTriggerLevel, double fFreq);
    bool   MptaGetSerialNumber(char *serialNumber, int bufSize); //!< Gets serial number for all the connected devices
    int    MptaGetNumberOfDevice(); //!< Gets number of devices (reserved for future use)
    bool   MptaGetSettings(int port, double *attn_dB, double *freq_Hz, double *attnError); //!< Gets setting (attenuation, frequency and attenuation error) for the active device
    bool   MptaSetActiveA21(const char *serialNumber);   //!< Sets active A21
    bool   MptaAvailable(); //!< Checks if MPTA is available or not
    void   MptaInit(bool createA21);  //!< Initializes a flag that controls if the A21 control should be created or not during the connection initialization using \c ConInit
    /*!< \note  If this function is not called, the default value of \c createA21Flag is true => A21 control will be created during ConInit(...)
    */

    // ****************************************************************    
    // iqapiHndl Misc Functions
    // ****************************************************************    
    int   MiscReqParm(char *parm, char *buff, unsigned int buff_len, int *ack = NULL, int testerNumber = 1);//!< This function is reserved for internal use
    int   MiscSendCmd(char *cmd, int testerNumber = 1); //!< This function is reserved for internal use
    int   MiscCmd(char *cmd, double input, double *output); //!<  This function is reserved for internal use
    int   MiscCmdLocalStatus(char *cmd, double input, double *output);  //!<  This function is reserved for internal use
    int   MiscCmdNoStatus(char *cmd, char *input, char *output, unsigned int outputSize);  //!<  This function is reserved for internal use
    int   MiscMwCmd(char *cmd, int index=-1, double value=-1); //!<  This function is reserved for internal use
    bool  IsHardwareVersion(IQV_HARDWARE_VERSION_ENUM hardwareVersion); //!< Check hardware version
    int   CleanWave(double frameLenSec); //!< Saves clean \c *.mod file for WiMAX
    /*!< Saves the signal that was generated from the analysis results of a previously run  Analyze() function call. This function works for WiMAX only. The \c frameLenSec &mdash; parameter specifies frame length of the generated waveform. 
    */ 
    int   ExportPayload(char *fileName, char *type);      //!< Exports pay load for different wireless standards. The \c Analyze() function must be called before calling this function.
    /*!< The parameter values are as follows:
    \n \c fileName &mdash; indicates the name of the file the user wants the payload to be written to.
    \n \c type &mdash; indicates the type of wireless standard. Supported wireless standards are as follows:
    \n OFDM (for 802.11 a/g)
    \n 802.11 n
    \n 802.11 b
    \n 80216-2004
    \n 80216e-2005
    \n Bluetooth
    */
    int   ExportSymbolData(char *fileName, bool binaryFormat);        //!< Exports symbol data per subcarrier for WiMAX. The \c analysis function must be called before calling this function
    /*!< The parameter values are as follows:
    \n \c fileName &mdash; indicates the name of the file the user wants the symbol data to be written to.
    \n \c binaryFormat &mdash; indicates a value of "TRUE" or "FALSE"
    */

    // ****************************************************************    
    // iqapiHndl Firmware server
    // **************************************************************** 
    //char     serialNumber[SERIAL_NUMBER_LEN +1];
    char            pbPort[6];
    int             DeviceDiscover(char *SerialNumbers, int bufLen);

    // ****************************************************************    
    // iqapiHndl Member Variables
    // ****************************************************************    
    iqapiRx         *rx;    //!< Indicates test system Rx and VSA settings
    /*!< Represents the Rx settings in test system. The top level members represent parameters that apply to all VSAs in a test system. Members of the VSA object array apply uniquely to each test unit within a test system.
    */   
    iqapiTx			*tx;    //!< Indicates test system Tx and VSG settings
    /*!< Represents the Tx settings in a test system. The top level members represent parameters that apply to all VSGs in a LitePoint Test Instrument. Members of the vsg object array apply uniquely to each test unit within a test system.
    */
    iqapiCapture	*data;  //!< Indicates local data capture storage
    /*!< Stores captured sample data and is used as input to the analysis functions. */
    iqapiAnalysis	*analysis; //!< Indicates local analysis parameters
    /*!< Performs a specific type of analysis and returns a specific type of analysis result; used by \c hndl->Analyze().
    \note The \c iqapiAnalysis is a base class for the various Analysis classes and should NOT be used directly.
    */
    iqapiResult		*results; //!< Indicates local measurement results.
    /*!< If a call to \c hndl->Analyze() function is successful, the \c hndl->result object will point to a derived \c iqapiResult class. 
    \note the \c iqapiResult object is a base class for the various Result classes and should NOT be used directly.
    */
    iqapiModulationWave	*wave; //!< Indicates local modulation wave for the VSG.
    /*!< If a call to a  \c hndl->GenerateWave() function is successful, the \c hndl->wave object will point to an \c iqapiModulationWave object which will be used in a call to the  \c hndl->SetWave() function.
    */    
    int			    nTesters; //!< Indicates number of connected test systems.
    /*!< This field indicates the number of test systems to which a connection is open. This field should not be changed by the user.
    */ 
    // Defined in iqapiHndlBase:	char			lastErr[MAX_LEN_ERR_TXT]; //!< Indicates the last error message text.
    /*!< This field acts as an error buffer, containing the last error message generated by the API. It is useful during debug, when the error code does not (by itself) give enough information. The exact error is often described in this field.
    */
    int			    connectionNumber;	//!< Indicates connection number
    IQV_CAPTURE_DATA_HANDLING_ENUM captureDataHandling; //!< Indicates captured data.
    /*!< IQV_DATA_IN_MATLAB&mdash;In some cases, the user does not have to pass the data back from MATLAB to the \c iqapi function to test the application. To save time, the captured data is stored in MATLAB. All subsequent operations will be carried out on the captured data in MATLAB.	
    \n IQV_DATA_IN_IQAPI&mdash;After data capture, a copy of the captured data can be obtained from the \c iqapi function in the object \c data
    */

    int   UnpackMeasurements(void *mx_result_in, bool skipCreateResult = false); //!< This function is reserved for internal use
    int   SetApiMode(const char *key);  //!< Internal use

    // Use with MiscMwCmd. 
    mwTable         calVsaTable;
    mwTable         calVsgTable;

    char            hwVersion[8];  //!< Hardware version number

//HT40Capture
private:
    //Perform HT40 analysis:
    //int AnalyzeHT40(iqapiSignalData *userData = NULL);    // Use AnalyzeHTWideBand instead
    int AnalyzeHTWideBand(iqapiSignalData *userData); // HT40 and HT20 wide band capture

    // available vsa frequency band from test system
    iqapiFreqBands      vsaFreqBands[NUMBER_OF_FREQ_BAND];

public:
    //Perform combined data capture (HT40 capture):
    int Capture(IQV_CAPTURE_TYPE_ENUM captureType);     //!< Use to capture HT40 

    int IsUsbConnected();  //!< Indicates that test system is connected via USB

    int IsLicenseAvailable(IQV_LICENSE_TYPE licenseType, bool *returnResult);  //!< Checks if license is available
    int IsAppLicSupported(char *AppLicNameToCheck, int *iSupport); //!< Checks if application license is valid
    /*!< The description of parameters is as follows:
    \n \c AppLicNameToCheck&mdash; checks the documentation for the name of an application license. \c WIMAX_CPP_API is an example of a name of an application license
    \n \c iSupport&mdash; indicates whether or not license is available
    \n 0 = License is available
    \n 1 = License is not available
    */

    int GetTesterFreqBand(IQV_FREQUENCY_BAND whichFreqBand, iqapiFreqBands *freqBands, unsigned int numberOfFreqBand);  //!< Gets frequency band of the test system

    void SetLpcPath(char *litePointConnectionPath); //!< Set the path that is used to invoke LitePoint Connectivity server. Note: this LPC server is used to invoke fw_server.exe and GF_fwserver.exe for IQ201x in the same folder as LPC server
    char *GetLpcPath();                             //!< Get the path that is used to invoke LitePoint Connectivity Server.

    int  GetTemperature(IQV_VSA_NUM_ENUM vsaNum, double *paTemperature, double *ifTemperature, double *swTemperature, double *vsaGainTemperature, double *vsgGainOffsetTemperature);
    int  UnpackTemperature(void *mx_result_in, double *paTemperature, double *ifTemperature, double *swTemperature, double *vsaGainTemperature, double *vsgGainOffsetTemperature);

    // ****************************************************************    
    // Internal use functions
    // ****************************************************************    
    int  GetTxRfFreqHz(double *txRfFreqHz);    //!< Get the current transmiter frequency
    int  GetLs(IQV_VSA_NUM_ENUM vsaNum, double *rxIfFreq, double *txIfFreq);     // Get IF freq for TX and RX directly from iq_nxn that is updated from MW
	int  GetLs(IQV_VSA_NUM_ENUM vsaNum, char *lsStrutureName, iqapiMeasurement *lsValue);		

    int  GetTesterInfoSub(char *testerCommand, char *testerInfo, unsigned int sizeOfTesterInfo, char *specificParam = NULL, double *outSpecificParam = NULL);

    // ****************************************************************    
    // Private iqapiHndl Members 
    // ****************************************************************    
private: 
    bool    connected;
	bool    isGlonassAvailable;
    bool    fpgaWithReferenceCapture;       //!< This is to indicate if the tester with this version of FPGA can do reference capture or not

	void ConSetup();
    int  DoConInit(char *ip1, char *ip2, char *ip3, char *ip4);
    int  DoConOpen(char *ip1, char *ip2, char *ip3, char *ip4, IQV_CONNECTION_TYPE connectionType, bool launchFWServer);
    int  UnpackVsaVsg(void *mx_result);
    int  PackVsaVsg(void **mx_input_in, void **mx_tx_in, void **mx_rx_in, void **mx_vsg_in, void **mx_vsa_in);
    int  UnpackCapture(void *mx_result_in, bool UnpackCon = true, bool fromCapture = true, IQV_CAPTURE_DATA_HANDLING_ENUM captureDataHandling = IQV_DATA_IN_IQAPI, iqapiCapture *userData = NULL);
    int  UnpackSingleCapture(void *mx_result_in);
    int  PackCaptureAndAnalysis(void **mx_input_in, void **mx_dataCapture_in, void **mx_analysis_in, iqapiSignalData *userData = NULL);
    int  PackWaveParams(void **mx_input_in, void **mx_params_in, void **mx_psdu_in, iqapiWaveGenParms *params);
    int  UnpackModulationData(void *mx_result_in, iqapiModulationWave *wave);
    char *masterIP;
    int  DoConInit2(char *ip1, char *ip2, char *ip3, char *ip4);
    int  PrecheckTxRx(IQV_SET_TX_RX_TYPE setTxRxType);

    int  GetTesterInfo(char *testerCommand, char **unscrambleData);
    int  Unscramble(char *scrambleData, char* unscambleData, unsigned int bufSize);
    int  CreateFreqBand(const char *const unscrambleData, iqapiFreqBands *freqBands);
    void ResetLastErr();
    int  CheckVersion(WHAT_VERSION_TO_CHECK  versionToCheck);
    int  CheckVersionFw(WHAT_VERSION_TO_CHECK  fwVersionToCheck);
    int  CheckVersionFpga(WHAT_VERSION_TO_CHECK  fwVersionToCheck);
    IQV_FPGA_VERSION_DATE_CHECK CheckFPGAVersionUpTodate(const char *DateFromTester, const char *DateExpected);

    int  CheckLpcRunning();  
    bool IsProcessRunning(char *processName);

    int  CaptureHT20WideBand(IQV_CAPTURE_TYPE_ENUM captureType);
	int  CaptureHT40WideBand(IQV_CAPTURE_TYPE_ENUM captureType);

	bool FastLicenseChecker(IQV_LICENSE_TYPE licenseType, bool init = false);
	int GetReferenceCapture(iqapiCapture **captureData);

    // FpgaLoadCalTable
    void SetFpgaLoadCalTable(double value);  // Refer to fpgaLoadCalTable
    // For FPGA reference capture
    int FpgaReferenceCaptureCompensate( IQV_FPGA_REFERENCE_CAPTURE_COMPENSATE fpgaRefCapCompensate, iqapiSignalData *userData = NULL);

    int Capture(iqapiCapture *userData, bool updatePlotData);


#ifdef _INCLUDE_GF_
    int  ConnectGpsFm(char *ip1, char *sn1 = (char *)"0");
    int  ConInitGpsFm(char *ip1);
#endif // _INCLUDE_GF_

    CA21Control *a21;
    bool        createA21Flag;
    char        *lpcPath;
public:  
    DualHead    *pDualHead;     //!< Dual head object for Wifi/BT/Wimax

#ifdef _INCLUDE_GF_
    DualHeadGps *pDualHeadGps;  //!< Dual head object for GPS
    DualHeadFm  *pDualHeadFm;   //!< Dual head object for FM

    iqapiHndlGps *hndlGps;      //!< Specifies parameters for analyzing GPS signals
    iqapiHndlFm *hndlFm;        //!< Specifies parameters for analyzing FM signals
	iqapiHndlGlonass *hndlGlonass;	//!< Specifies parameters for Glonass
#endif // _INCLUDE_GF_

#ifdef _INCLUDE_NFC_
private:
    int PackWaveParams(void **mx_input_in, iqapiWaveGenParmsNfc *params);
    int UnpackModulationData(void *mx_result_in, iqapiModulationWaveNfc *waveNfc, iqapiWaveGenParmsNfc *waveGenParam);
public:
    iqapiModulationWaveNfc	*waveNfc; //!< Indicates local modulation wave for the VSG.

    int GenerateWave(iqapiWaveGenParmsNfc *waveGenParam);

    int SetDefaultNfc();

    iqapiTxNfc *txNfc;
    iqapiRxNfc *rxNfc;

    int SetTxNfc();
    int SetRxNfc();
    int SetTriggerMaskNfc();
    int ClearTriggerMaskNfc();
    int InitiateNfc(char *fileName = NULL);
    int TerminateNfc();
#endif // _INCLUDE_NFC_
};

// Composite Reference Generation Function
IQ_API int iqapiFindPsduDataRef(iqapiResultMimo *SigData, iqapiResultFindPsduDataRef *resultFindPsduDataRef, bool init); //!< Finds PSDU of data reference of composite MIMO

// Composite Reference Generation Function
IQ_API int iqapiWritePsduDataRef(char *txtFileName); //!< Exports PSDU data reference

// Composite Reference Generation Function
IQ_API int iqapiWriteDataRef(char *txtFileName, char *refFileName, char *type, int mcsIndex, IQV_MIMO_BANDWIDTH_MODES bw, int advancedCoding=0, int stbc=0); //!< Exports data reference

IQ_API int iqapiSaveMapConfigFile(char *mcfFileName); //!< Saves map configuration file

IQ_API int iqapiGetShortPreRamp(iqapiHndl *hndl, bool shortPreRramp[N_MAX_TESTERS]);

IQ_API int iqapiSetShortPreRamp(iqapiHndl *hndl, bool shortPreRramp);

#endif			// _IQAPI_H_
