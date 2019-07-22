#pragma once
#include "iqapiDefines.h"
//#include <time.h>
// ****************************************************************
// Internal define statement
// ****************************************************************
#define     SERIAL_NUMBER_LEN          8
#define     NUMBER_OF_FREQ_BAND        8

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
////IQ_API int iqapiPlot(int figNum, double *x, double *y, int length, char *plotArgs, char *title, char *xtitle, char *ytitle, int keepPlot=0);
//
//#ifdef WIN32
//class PerformanceTimer
//{
//public:
//    PerformanceTimer()
//    {
//        m_liHighPerformanceCounterStart = 0.0;
//        m_liHighPerformanceCounterStop  = 0.0;
//
//        SetHighPerformanceFreq();
//
//    };
//    ~PerformanceTimer() {};
//
//    void SetHighPerformanceFreq()
//    {
//       QueryPerformanceFrequency(&liHighPerformanceFreq);
//    }
//
//    void Start()
//    {
//        QueryPerformanceCounter(&m_liHighPerformanceCounterStart);
//    };
//
//    void Stop(char *message = NULL)
//    {
//         QueryPerformanceCounter(&m_liHighPerformanceCounterStop);
//         m_dbHighPerformanceDuration = double(m_liHighPerformanceCounterStop.QuadPart - m_liHighPerformanceCounterStart.QuadPart) / liHighPerformanceFreq.QuadPart;
//         if ( message == NULL )
//             printf("Time spent (us) %f\n", m_dbHighPerformanceDuration);
//         else
//             printf("%s %f\n", message, m_dbHighPerformanceDuration);
//    }
//
//    //{
//    //    QueryPerformanceCounter(&m_liHighPerformanceCounterStop);
//    //    m_dbHighPerformanceDuration = double(m_liHighPerformanceCounterStop.QuadPart - m_liHighPerformanceCounterStart.QuadPart) / liHighPerformanceFreq.QuadPart;
//    //    if ( message == NULL )
//    //        printf("Time spent (us) %f\n", m_dbHighPerformanceDuration);
//    //    else
//    //        printf("%s %f\n", message, m_dbHighPerformanceDuration);
//    //};
//
//// Attribute
//LARGE_INTEGER  m_liHighPerformanceCounterStart;
//LARGE_INTEGER  m_liHighPerformanceCounterStop;
//double         m_dbHighPerformanceDuration;    
//};
//#else
//// MAC - NOTDONE:
//#define LARGE_INTEGER long long
//class PerformanceTimer
//	{
//	public:
//		PerformanceTimer() { };
//		~PerformanceTimer() { };
//		
//		void Start() {  printf("PerformanceTimer NOT implemented on Mac\n"); };
//		void Stop(char *message = NULL) { printf("PerformanceTimer NOT implemented on Mac\n"); };
//
//		LARGE_INTEGER  m_liHighPerformanceCounterStart;
//		LARGE_INTEGER  m_liHighPerformanceCounterStop;
//		double         m_dbHighPerformanceDuration;    
//	};
//#endif

// ****************************************************************
// Generic Measurement Result Class
// ****************************************************************
//! Specifies the measurement analysis parameters.
class IQ_API iqapiMeasurement
{
public:	
	iqapiMeasurement(void); //!< Constructor
	virtual ~iqapiMeasurement(void); //!< Destructor
	
	double	*real;	//!< Pointer to the \c real vector. This value is set to NULL if no data is available.
	double	*imag;	//!< Pointer to \c imag (imaginary) vector. This value is set to NULL if no data is available.
	int	length;	//!< Length of \c real vector; also applies to \c imag vector if the value is not NULL.
	int	m;	//!< This value is reserved for future use.
	int	n;	//!< This value is reserved for future use.
	int	o;	//!< This value is reserved for future use.
	int ConsolePrint(char *resultTitle);	//!< Displays the measurement in the console window. Data and array index are displayed after the \c resultTitle parameter.
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
    
    iqapiVsg(const iqapiVsg &src);

    IQV_RF_ENABLE_ENUM  enabled;    //!< Enables or disables RF
    /*!< Available options are as follows;
    IQV_RF_DISABLED indicates that the RF is disabled. 
    IQV_RF_ENABLED indicates that the RF is enabled (default)
    */
    IQV_PORT_ENUM	    port;		//!< Represents RF N-connector port selection
    /*!< Available options are as follows:
    IQV_PORT_OFF   = 1 indicates that the port is disabled. (default)
    IQV_PORT_LEFT  = 2 indicates that the RF uses the  left port.      
    IQV_PORT_RIGHT = 3 indicates that the RF uses the right port. 
    */
    IQV_SOURCE_ENUM     source;	    //!< Represents the signal source of all VSGs.
    /*!< Available options are as follows:
    IQV_SOURCE_WAVE represents internal modulation from wave. (default)
    IQV_SOURCE_SIGNAL_GENERATOR represents internal modulation from CW signal generator.                                      
    IQV_SOURCE_UNDEFINED represents undefined default modulation source.
    */
    double		        sineFreqHz; //!< This field represents the VSG signal generator's sine wave frequency. Only applies if source is set to IQV_SOURCE_SIGNAL_GENERATOR. 100 kHz (default)
    double		        rfGainDb;   //!< Represents RF gain in dB. -130 (default)
    double		        bbGainDb;   //!< Represents BB gain in dB. -10 (default)
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
    \param[in] 0 = Uses middleware compensation method; calibration is always available
    \param[in] 1 = Uses FPGA real-time compensation; calibration may not be available
    \return 0  = Tables have been downloaded
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

class IQ_API iqapiVsgXstream
{
public:
    iqapiVsgXstream(void) {};	                    //!< Constructor
    virtual ~iqapiVsgXstream(void) {};	            //!< Destructor

    IQV_PORT_ENUM	            port;		        //!< Represents RF N-connector port selection
    IQV_RF_ENABLE_ENUM          enabled;            //!< Power state on or off. Enables or disables RF.
    double		                rfGainDb;           //!< Power level. Represents RF gain in dB.
    IQV_SOURCE_ENUM             source;	            //!< Represents the signal source of all VSGs.
    /*!< Available options are as follows:
    IQV_SOURCE_MODULATION_STATE_OFF, IQV_SOURCE_WAVE represents internal modulation from wave.
    IQV_SOURCE_MODULATION_STATE_ON, IQV_SOURCE_SIGNAL_GENERATOR represents internal modulation from CW signal generator.                                      
    */

};

class IQ_API iqapiTxBase
{
public:
    iqapiTxBase() {};
     virtual ~iqapiTxBase() {};
};

class IQ_API iqapiTxXstream : public iqapiTxBase
{
public:
    iqapiTxXstream();
    virtual ~iqapiTxXstream();

    iqapiVsgXstream         *vsg[N_MAX_TESTERS];
    double			        rfFreqHz;           //!< This field represents the RF frequency for the VSGs,in Hz.

    IQV_ALC_MODES			alcMode;            //!< Automatic level control
    
};

// ****************************************************************
// Tx Parameters Class - applies to all test systems (except member vsg)
// ****************************************************************
//! Specifies parameters for transmitting signals.
class IQ_API iqapiTx : public iqapiTxBase
{
public:
    iqapiTx(void);	        //!< Constructor
    virtual ~iqapiTx(void);	//!< Destructor

    iqapiTx(const iqapiTx &src);		//!< A Copy Constructor that can be used to make a deep copy of an iqapiCapture object. Use this constructor when you wish to create a copy of an iqapiCapture object.
    
    
    double			        rfFreqHz;           //!< This field represents the RF frequency for the VSGs,in Hz. IQxel 2.5 GHz (default)
    /*!< For IQview/flex/nxn systems valid values are in the 2.4 and 4.9-6.0 GHz ranges, in increments of 1 MHz. */
    double					sampleFreqHz;       //!< 160 MHz (default) 

    iqapiVsg				*vsg[N_MAX_TESTERS];//!< Represents individual VSG parameters	
    
    /*!< Used for backward compatibility */
    IQV_ALC_MODES			alcMode;            //!< Automatic level control. IQV_ALC_OFF (default)
    IQV_VSG_TRIG            triggerType;        //!< VSG trigger. IQV_VSG_TRIG_FREE_RUN (default)
    IQV_VSG_TRIG_REARM      triggerReArm;       //!< Indicates if VSG needs to be re-armed after the trigger event
    double                  freqShiftHz;        //!< Tx IF Frequency in Hz 

    // *********************************************************************
    // The following input parameters is used only for SCPI based tester
    // *********************************************************************
    double frequencySeparationInHz;             //!< default: 80e6 This is used in 80 + 80 MHz on IQXel 160 MHz. Like, ConInit(ip1, IQV_CONNECTION_DAYTONA_160MHZ).

    // *********************************************************************
    // The following input parameters is not applicable to SCPI based tester
    // *********************************************************************
    IQV_INPUT_MOD_ENUM		txMode;	//!< Represents the VSG signal transmission mode
    /*!< Available options are as follows:
    IQV_INPUT_MOD_DAC_RF indicates RF transmit mode                                                          
    IQV_INPUT_MOD_DAC_BB indicates BB transmit mode                                              
    IQV_INPUT_MOD_UNDEFINED indicates an undefined default value
    */

    IQV_MODULATION_CONTROL_ENUM	modulationMode;	//!< Represents VSG modulation mode.
    /*!<Available options are as follows:
    IQV_WAVE_DL_MOD_DISABLE indicates that the modulation is halted after the wave file is downloaded unless it is in continuous Tx mode.  
    IQV_WAVE_DL_MOD_ENABLE indicates that modulation is continued after the wav file is downloaded
    */

    IQV_GAP_POWER			gapPowerOff;        //!< Controls VSG power during signal gaps (idle time)

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

    iqapiVsa(const iqapiVsa &src);    // Copy constructor
    
    IQV_RF_ENABLE_ENUM	enabled;	//!< Specifies whether VSA RF input is enabled or disabled
    
    /*!< Available options are as follows:
    IQV_RF_DISABLED indicates that RF is disabled. 
    IQV_RF_ENABLED indicates that RF is enabled. (default)
    */
    IQV_PORT_ENUM		port;	    //!< Represents RF N-connector port selection 
    /*!<
    Available options are as follows:
    IQV_PORT_OFF   = 1 indicates that the port is disabled.     (default)
    IQV_PORT_LEFT  = 2 indicates that the RF uses the left port.      
    IQV_PORT_RIGHT = 3 indicates that the RF uses the right port.  
    */
    double				rfAmplDb;	//!< Represents the RF amplitude in dBm.  0.0 (default)

    double              extAttenDb; //!< Represents external attenuation in dB. 0.0 (default)

    // *********************************************************************
    // The following input parameters is not applicable to SCPI based tester
    // *********************************************************************

    /*!<
    \note This parameter replaces the \a rfGainDb; parameter.    
    */
    double				bbAmplDbv;	//!< Represents the Baseband amplitude in dBV
  
    double				bbGainDb;	//!< Represents baseband gain in dB.  	

   IQV_MIRROR_FREQ_ENUM mirrorFreq; //!< Mirrors the center frequency in the frequency spectrum of the captured signal. Calculates the complex conjugate of the signal
   /*!< Available options are as follows:
   IQV_MIRROR_FREQ_DISABLED indicates that mirror frequency spectrum is disabled. (default)                
   IQV_MIRROR_FREQ_ENABLED indicates that mirror frequency spectrum is enabled.
   */
};

// ****************************************************************
// Rx Parameters Class - applies to all test system (except member vsa)
// ****************************************************************
class IQ_API iqapiRxBase
{
public:
    iqapiRxBase() {};
     virtual ~iqapiRxBase() {};
};

//! Specifies parameters for receiving the signal.
class IQ_API iqapiRx : public iqapiRxBase
{
public:
    iqapiRx(void);	                //!< Constructor
    virtual ~iqapiRx(void);	        //!< Destructor

    iqapiRx(const iqapiRx &src);    //!< Copy constructor

    IQV_INPUT_ADC_ENUM  rxMode;		//!< Capture mode for the VSAs.
    /*!< This field represents the capture mode for the VSAs. 
    Available options are as follows:
    IQV_INPUT_ADC_RF, which represenst data captured from RF
    IQV_INPUT_ADC_BB, which represents data captured from base band
    IQV_INPUT_ADC_UNDEFINED, which represents undefined default value
    IQV_GET_RAW_DATA = -2, which represents getting the raw data with no compensation
    */
    double		rfFreqHz;		    //!< This field represents the RF Frequency for the VSAs,in Hz. IQxel 2.5 GHz (default)
    /*!< For IQview/flex/nxn systems valid values are in the 2.4 and 4.9-6.0 GHz ranges, in increments of 1 MHz. 
    */
    double		freqShiftHz;		//!< This field represents the Rx IF frequency in Hz 
    /*!< After capturing by the VSA(s), the signal is shifted by the indicated value; this can be used to analyze signals at intermediate frequencies not directly supported by the instrument */
    /*Rx Intermediate Frequency for all VSAs
    */
    double		samplingTimeSecs;	//!< Sampling time, in seconds, for all VSAs. 1ms (default) 
    double		sampleFreqHz;		//!< ADC sampling frequency in Hz, for all VSAs. Recommended frequency is 160 MHz. (default)
    IQV_TRIG_TYPE_ENUM	triggerType;//!< This field represents the trigger type used for the VSA master. IQV_TRIG_TYPE_FREE_RUN (default)
    /*!<The VSA slaves (including the VSG master), are always triggered by the VSA master. 
    */
    double		triggerLevelDb;		//!< Signal (IF) trigger level in dB, relative to VSA master's \c rfAmplDb setting. -25 (default)
    double		triggerPreTime;		//!< Amount of data to capture before the trigger point, in seconds. 0 (default)
    double		triggerTimeOut;		//!< Trigger timeout in seconds. 1.5 (default)
    iqapiVsa	*vsa[N_MAX_TESTERS];//!< Parameters unique to each vsa 
    
    IQV_RX_POWER_MODES	powerMode;	//!< Represents the AGC power mode.
    /*!< Available options are as follows:
    IQV_VSA_TYPE_0, where VSA Type 0 represents RMS Power Mode. 
    IQV_VSA_TYPE_1, where VSA Type 1 represents Peak Power Mode. (default)
    */
    double      agcTime;            //!< agc window time in second. default: 0.005 second ( 5 ms )

    // *********************************************************************
    // The following input parameters is used only for SCPI based tester
    // *********************************************************************
    double frequencySeparationInHz;             //!< default: 80e6 This is used in 80 + 80 MHz on IQXel 160 MHz. Like, ConInit(ip1, IQV_CONNECTION_DAYTONA_160MHZ).

};

class IQ_API iqapiSignalData
{
public:	
     iqapiSignalData(void); //!< Constructor
    virtual ~iqapiSignalData(void);	//!< Destructor
    //! A copy constructor that can be used to make a deep copy of an \c iqapiModulationWave object.
    /*! Use this constructor when you wish to create a copy of an \c iqapiModulationWave object.
    */
    iqapiSignalData(const iqapiSignalData &src);
    //! Assignment operation 
    iqapiSignalData & operator = (const iqapiSignalData &src);
    
    int         length[N_MAX_TESTERS];    //!< Integer array that represents the length of the \a real and \a imag vectors, with one integer for each VSA in the test systems. 
    
    /*!< \c Length[0] indicates the length of \c real[0] and \c imag[0] vectors, which is the sample data returned from the first VSA (0).
    */

    double      *real[N_MAX_TESTERS];	//!< Double pointer array that represents the (real) sample data captured on each VSA.
    
    /*!<The length of \c real[x] is indicated by \c length[x], where x is the VSA number minus one. For example, the test system 1 uses a value of \c real[0], test system 2 uses a value of \c real[1] etc.
    */

    double      *imag[N_MAX_TESTERS];	//!< Double pointer array that represents the (imaginary) sample data captured on each VSA.
    
    /*!< The length of \c imag[x] is indicated by \c length[x], where x is the VSA number minus one. For example, the test system 1 uses a value of \c imag[0], test system 2 uses a value of \c imag[1] etc.
    */

    double      sampleFreqHz[N_MAX_TESTERS];	//!< Double pointer array that represents the sample frequency of \c real and \c imag data.
    
    /*!<\c sampleFreqHz[x] is the sampling rate of \c real[x] and \c imag[x], where x is the VSA number minus one. For example, the test system 1 uses a value of \c sampleFreqHz[0], test system 2 uses a value of \c sampleFreqHz[1] etc.
    */	
    char     lastErr[MAX_LEN_ERR_TXT];	//!< Char array (\c hndl->wave->lastErr) that contains an error message if a call to the \c hndl->GenerateWave function fails.

    int dataDimension;  //!< Indicate number of data set 
		
    virtual int	Save(char *fileName) = 0;		//!< Saves an \c iqapiModulationWave object to a \c .mod file(set by filename), which can be downloaded to a VSG via this API, or by the VSG panel in the IQsignal for MIMO application.    
    /*!< This function returns 0 (\c IQAPI_ERR_OK) if successful; if it returns a value that is less than 0, then it indicates a warning and if it returns a value that is greater than 0, then it indicates an error (see IQAPI_ERR_CODES, or the \c hndl->lastErr buffer). 
    */
	
   //! Get the capture type of the signal capture or signal wave generation
   virtual IQV_CAPTURE_TYPE_ENUM GetCaptureType() = 0;

   //! Free real and imag memory and assign length to 0 if any memory has been created
   void ReleaseMemory();
   
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

    int Save(char *fileName) { return Save(fileName, IQV_DATA_IN_IQAPI); };
    int	Save(char *fileName, IQV_CAPTURE_DATA_HANDLING_ENUM captureDataHandling); 	//!< Saves the captured signal in a file (.sig)	
    int SaveNormalize(char *fileName, IQV_CAPTURE_DATA_HANDLING_ENUM captureDataHandling = IQV_DATA_IN_IQAPI, int lengthOfNormFactorDb=0, double *normFactorDb=NULL); //!< Saves normalized captured signal to a file (typically, a \c .mod file); this file can be used as a generator file to download to the VSG.

    int PlotPower(int figNum, char *plotArgs, int vsaNum)
    {
        return IQAPI_NOT_IMPLEMENTED_ERR;
    }
    /*! Displays the signal contents of an \c iqapiCapture object during the debug and development phases. The data will first be converted to dBm before plotting. This function returns 0 (IQAPI_ERR_OK) if successful; if it returns a value that is less than 0, then it indicates a warning and if it returns a value that is greater than 0, then it indicates an error (see IQAPI_ERR_CODES, or the \c hndl->lastErr buffer).
    */

    // Internal use
    void  SetCaptureType(IQV_CAPTURE_TYPE_ENUM capType);
    IQV_CAPTURE_TYPE_ENUM GetCaptureType();
    double   reserved[5];

private:
   int Save(char *fileName, enum IQV_SAVE_FILE_TYPE_ENUM eFileType, IQV_CAPTURE_DATA_HANDLING_ENUM captureDataHandling = IQV_DATA_IN_IQAPI, int lengthOfNormFactorDb=0, double *normFactorDb = NULL);

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
//////class IQ_API iqapiWaveGenParms
//////{
//////public:	
//////    iqapiWaveGenParms(void);	        //!< Constructor.
//////    virtual ~iqapiWaveGenParms(void);	//!< Destructor.
//////    
//////    int			mcsIndex;	            //!< Sets modulation and coding scheme. Supported range: 0 to 76
//////    /*!< Defines the modulation (BPSK, 64-QAM etc.) and coding rate (1/2, 5/6 etc.) and MIMO settings of the generated signal. The options follow the IEEE 802.11n standards specifications.
//////    
//////    \n \note Currently, the number of streams allowed is restricted to a maximum of four, and the modulation type is set to a maximum of 64-QAM.*/
//////    int			bandwidth;	            //!< Sets the bandwidth of the generated signal.
//////    /*!< The bandwidth values are as follows:
//////    \n 0 = 20 MHz
//////    \n 1 = 40 MHz
//////    \n The channel mapping follows the IEEE 802.11 standards specification. */
//////    char		*PSDU_mode;             //!< Represents PSDU mode.  Predefined with buffer size to be 64 bytes
//////    /*!< Values for this field are as follows:
//////    \n \c random
//////    \n \c userdef
//////    \n \c userdef_withcrc
//////    
//////    \n This field is reserved for future use.
//////    */
//////    int			PSDU_lengthBytes;	    //!< Sets the length of payload in bytes if PSDU_mode is set to 'random' which is default
//////
//////    double		*PSDU_bits;	            //!< Sets the PSDU value
//////    int         PSDU_bits_size;         //!< Specify the size for PSDU_bits
//////    /*!< This field is reserved for future use. */
//////    
//////    
//////    //! Defines Matrix Q for Spatial Mapping
//////    /*! Dimension must be NTx x NStreams.*/
//////    iqapiSpatialMap spatialMapMatr;	
//////    
//////	//int			soundingPacketHTLTF; //!< Deprecated, Should use numESS instead. Defines the sounding packet.
//////    /*!< Valid values are as follows:
//////    \n 0: no sounding packet (default)
//////    \n 2-4: number of HT-LTFs, if sounding packet is present. */
//////    int			aggregation;            //!< Defines the aggregation bit in HT-SIG.
//////    /*!< This field is reserved for future use.
//////    */
//////    int			advancedCoding;         //!< Defines the advanced coding bit in HT-SIG. 0 = BCC, 1= LDPC
//////    
//////    /*!< This field is reserved for future use.
//////    */
//////    int			shortGI;	            //!< Defines the Short Guard Interval bit in HT-SIG.
//////    int			legacyLength;	        //!< Defines the legacy length.
//////    /*!<-1: Length in legacy SIG is derived from HT-length. Otherwise, use this value for length field in legacy SIG.*/
//////    short		scramblerInit;	        //!< Defines the scrambler initialization value.
//////    /*!< Valid values are as follows:
//////    \n -1: Random scrambler init value 
//////    \n 0 to 127: Scrambler init value
//////    */
//////    
//////    //! Defines the cyclic shift in nanoseconds.
//////    /*! Cyclic shift on Tx chains > 1 (in nanoseconds). */
//////    iqapiMeasurement *cyclicShiftnS;    // change from double to iqapiMeasurement pointer. only real portion of cyclicShiftnS is used. It is pre-allocated to be 4. No need to allocate memory for member variable 'real'. Just use it. Maximum is 4
//////
//////    double		idleTimeuS;             //!< Defines the idle time (filled with zeros) between generation of two packets
//////    /*!< Specified in microseconds
//////    */
//////    char		*type;                  //!< Defines the type of packet to generate. 
//////
//////    /*!< Currently, only EWC is supported.
//////    */          
//////    int		    greenField;	            //!< Indicates mode of operation
//////    /*!< Valid values are as follows:
//////    \n 0 = mixed mode packet format; this is the default value
//////    \n 1 = green field packet format.
//////    */
//////    int		    soundingPacket;         //!< Indicates that the packet is to be used for channel sounding
//////    /*!< Valid values are as follows: 
//////    \n 0:   no sounding packet; this is the default value
//////    \n 1-3: number of Extension HT-LTFs
//////    */
//////    char		*ofdmWindowType;        //!< Indicates type of OFDM windowing. Predefined with buffer size to be 64 bytes
//////    /*!< Valid values are as follows;
//////    \n If the value is set to \c none, the first and last samples are multiplied by 1/2
//////    \n If the value is set to \c raised cosine, the samples use the raised cosine filter
//////    \n If the value is set to \c straight line, the samples use the straight line filter */
//////    int		    ofdmWindowLengthSamples;//!< Indicates length of the OFDM window
//////    /*!< Specified in number of samples (80 MHz)
//////    */
//////    int         stbc;                   //!< STBC field in HT-SIG field. 
//////    /*!< Indicates the difference between the number of space-time streams and spatial streams. 
//////    \n Valid values are as follows:
//////    \n 0 = non-stbc
//////    \n 1 = stbc value set to 1 in HT-SIG field
//////    \n 2 = stbc value set to 2 in HT-SIG field
//////    */
//////
//////    int             numESS;     //!< Number of extension spatial streams
//////
//////    int SetDefault(char *type);//!<Resets the parameters of an \c iqapiWaveGenParms object to default parameters.
//////    /*!< This function returns 0 (IQAPI_ERR_OK) if successful; if it returns a value that is less than 0, then it indicates a warning and if it returns a value that is greater than 0, then it indicates an error (see IQAPI_ERR_CODES, or the \c hndl->lastErr buffer object).
//////    */
//////
//////     static const int waveCommonBufferSize;     // bufferSize for PSDU_mode and ofdmWindowType
//////
//////private:
//////
//////    int UnpackWaveParam(void *mx_result_in);
//////};

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
// Analysis Parameters Base Class - DO NOT USE THIS CLASS
// ****************************************************************    
//! Specifies the analysis parameters for testing devices that are compliant with IEEE 802.11, IEEE 802.16, bluetooth, GPS and FM standards specifications. 
/*! This is a base class that points to all \c iqapiAnalysis parameters.*/
class IQ_API iqapiAnalysis
{

public:	
	iqapiAnalysis(void);	//!< Constructor
    virtual ~iqapiAnalysis(void); //!< Destructor
	virtual int SetDefault(); //!< Sets the default value
	char *type; //!< Indicates type of signal to be analyzed.
	/*!< \note The \c type parameter must be specified only for WiMAX device testing and MIMO.
	
	\n Valid values are as follows:
    \n For iqapiAnalysis80216 (WiMAX)
	\n \c 80216-2004= analyzes 802.16 d (fixed WiMAX) signals
	\n \c 80216e-2005= analyzes 802.16e (mobile WiMAX) signals	
    \n For iqapiAnalysisMimo
    \n \c ewc= analysis for mimo signal. please refer to 'mode' for further setting
    \n \c 11n= analysis for 802.11a by using iqapiAnalysisMimo. No 'mode' need to be specified

	*/
	char *mode;	//!< Indicates mode of analysis, if supported
	
	/*!< \note The \c mode parameter must be specified for WiMAX and MIMO device testing.
	
	\n For WiMAX device testing (iqapiAnalysis80216), valid values are as follows:
	\n \c 80216-2004= analyzes 802.16 d (fixed WiMAX) signals
	\n \c 80216e-2005= analyzes 802.16e (mobile WiMAX) signals
	\n \c powerPacketDetect= analyzes only power measurements using packet detection; does not perform the remaining EVM analysis
	
	\n For MIMO device testing (iqapiAnalysisMimo), valid values are as follows:
	\n \c nxn= analyzes signals using the LitePoint NxN test system, where signal is captured separately for each antenna
	\n \c composite= analyzes signals using the LitePoint NxN test system, where a signal capture has combined signals from all antennas 
	\n \c sequential_mimo= analyzes signals where sequential data capture is performed using a LitePoint NxN test system with MPTA
	*/
};


// ****************************************************************
// Analysis Results Classes - DO NOT USE THIS CLASS
// ****************************************************************
//! Specifies pointers to \c iqapiResult. This is the base class that points to all \c Result parameters.
//
class IQ_API iqapiResult 
{
public:	
	iqapiResult(void);	//!< Constructor
    virtual ~iqapiResult(void); //!< Destructor
	char *type;
	char *mode;	

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
    iqapiAnalysis80216(void) {};	        //!< Constructor
    virtual ~iqapiAnalysis80216(void) {};  //!< Destructor
    
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

    int SetDefault() {return 0;};
    
private:
    int Unpack80216Analysis(void *mx_result_in) {return 0;};
};

// ****************************************************************
// Zigbee Analysis Parameters Class (Derived from iqapiAnalysis)
// ****************************************************************
//! Specifies Zigbee analysis parameters.

class IQ_API iqapiAnalysisZigbee : public iqapiAnalysis
{
public: 
    iqapiAnalysisZigbee( void) {};	            //!< Constructor
    virtual ~iqapiAnalysisZigbee( void) {} ;    //!< Destructor 
            
    double					    sigType;    //!< Specifies the signal type. Default: 1 (for OQPSK)
    /*!< Currently, only the default value of 1 (for OQPSK) is supported. Valid values are as follows:
    \n IQV_FREQ_SYNC_ENUM=Long Training (Signal type is long preamble)
    \n IQV_FREQ_SYNC_ENUM=Full Packet (Signal type is full packet)
    \n IQV_AMPL_TRACK_ENUM This function is reserved for future use.
    */
    IQV_ZIGBEE_PH_CORR_ENUM     phaseCorrect;   //!< Symbol by Symbol phase correction. Default: IQV_ZIGBEE_PH_CORR_SYM_BY_SYM  (only this choice is available)
    IQV_ZIGBEE_FREQ_SYNC_ENUM   freqCorrect;    //!< frequency synchronization on full data packet. Default: IQV_ZIGBEE_FREQ_SYNC_FULL_PACKET (Only this choice is available)
    IQV_AMPL_TRACK_ENUM         amplitudeTrack; //!< Specifies the Amplitude Tracking mode. Default: IQV_AMPL_TRACK_OFF

    int SetDefault() {return 1;};
            
private:
    int	UnpackZigbeeAnalysis(void *mx_result_in) {return 1;};
};

// ****************************************************************
// Generic use of WIFI OFDM Analysis Parameters Class (Derived from iqapiAnalysis)
// ****************************************************************
//! Specifies analysis parameters for signals that are compliant with the IEEE 802.11a, 802.11g, 802.11n and 11ac
class IQ_API iqapiAnalysisWifiOfdm : public iqapiAnalysis
{
public:	
    iqapiAnalysisWifiOfdm(void);                //!< Constructor	
    virtual ~iqapiAnalysisWifiOfdm(void); 	    //!< Destructor 
    
    unsigned int    numberOfPacketToAnalysis;    
    /*!< It indicates how many packet user wants to do analyze for EVM result. default: 1
        0 means all packet(s), 
        1 for first packet, 
        2 for the first 2 packets
        . . . etc         
        If number of numberOfPacketToAnalysis is greater than the captured packet, the maximum captured packet (complete packet) will be used
    */

    IQV_ON_OFF		enablePhaseCorr;                //!< Phase Correlation Default: IQV_ON (phase correction is enabled)
    /*!<Valid values are as follows:
    \n IQV_OFF  = phase correction is disabled
    \n IQV_ON   = phase correction is enabled
    */
    IQV_ON_OFF		enableSymTimingCorr;            //!< Symbol Timing Error Default: IQV_ON (symbol timing correction is enabled)
    /*!<Valid values are as follows:
    \n IQV_OFF  = symbol timing (symbol clock) correction is disabled 
    \n IQV_ON   = symbol timing (symbol clock) correction is enabled
    */
    IQV_ON_OFF		enableAmplitudeTracking;        //!< Amplitude Tracking Default: IQV_OFF (amplitude tracking is disabled)
    /*!<Valid values are as follows:
    \n IQV_OFF  = amplitude tracking is disabled
    \n IQV_ON   = amplitude tracking is enabled
    */
    IQV_FREQUENCY_CORR		frequencyCorr;                  //!< Frequency Correlation Default: IQV_FREQUENCY_CORR_LTF (frequency correction on long legacy training fields)
    /*!<Valid values are as follows: 
    \n IQV_FREQUENCY_CORR_STF = frequency correction on short legacy training fields
    \n IQV_FREQUENCY_CORR_LTF = frequency correction on long legacy training fields
    \n IQV_FREQUENCY_CORR_DATA = frequency correction based on whole packet
    \n IQV_FREQUENCY_CORR_SIG = frequency correction on signal fields (legacy and HT) in addition to long training fields
    */  
    IQV_CHAN_ESTIMATION		enableFullPacketChannelEst;     //!< Full Packet Channel Estimation Default: IQV_CHAN_ESTIMATION_LTF (channel estimate on long training fields)
    /*!<Valid values are as follows:	 
    \n IQV_CHAN_ESTIMATION_LTF = channel estimate on long training fields
    \n IQV_CHAN_ESTIMATION_DATA = reestimate channel on full packet before doing EVM calculation
    */
//    int		enableMultipathEvm;             //!< Enable Multipath EVM for MIMO (IQV_MIMO_ENABLE_MULTIPATH_EVM) Default: 0 (multipath)
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
    int		prefOrderSignals[MAX_SIGNAL_NUMBER];//!< Preference order used to select input signals if the \c useAllSignals parameter is set to a value of 0. Default: 1,2,3,4 (Use any permutation of the numbers 1,2,3, and 4)
    /*!< Specifies the sequence of input signals that must be used for analysis. If the number of available input signals (captures) are more than that is required to demodulate a MIMO signal, this parameter allows you to specify the sequence of the input signals that must be used for the analysis. \n \note The number of required captures is equal to the number of data streams in a MIMO signal.
    Valid values are as follows:
    \n Default: [1 2 3 4 5 6 7 8] Use any permutation of the numbers 1,2,3, and 4.
    \n As an example, when you analyze a one-stream signal with the \c useAllSignals parameter set to 0 and the \c prefOrderSignals parameter set to [2 1 3 4], the analysis first checks the capture from VSA 2 (input signal 2) for a valid signal. If the signal is valid, the analysis will be performed on capture from VSA 1 (input signal 1). If it is not valid, the next signal capture will be performed on capture 1. This process is continued until the required number of valid signals has been found or until all available captures have been checked. In this case, the required number of valid signals is one, because this analysis is on a one-stream signal.
    */
    int     powerDetectionSignals[MAX_SIGNAL_NUMBER];
    /*!< \n \note Configures which input signals are used for packet detection (acquisition) in the current active instrument channel. 
    \n Default: [1] Currently only one signal can be selected.
    */

    char    *referenceFile;                 //!< Indicates name of the reference sequential_mimo analysis mode 
    /*!< Valid values are as follows:
    \n \c composite 
    \n \c sequential_mimo 
    \n This parameter is ignored for NxN systems.
    */
    int 	SetDefault();		
    
    //! Specifies fields for performing a MIMO analysis on a sequential MIMO data capture.
    //struct SEQ_MIMO
    //{
    //    int	   numSections;		        //!< Number of sections in capture Default: 0 
    //    /*!< \c numSections specifies the number of chains which are sequentially captured. The capture is then reshaped into dimensions numSections x (sectionLenSec + interSectionGapSec) before it is passed to the MIMO analysis.
    //    */
    //    double sectionLenSec;			    //!< Length of each section Default: 0.0
    //    /*!< Specified in seconds.
    //    */
    //    double interSectionGapSec;		    //!< Spacing between sections Default: 0.0
    //    /*!< Specified in seconds.
    //    */
    //    double sectionGainDb[N_MAX_TESTERS];//!< Gain compensation per section Default: 0,0,0,0
    //    /*!< \note The analysis mode must be set to \c sequential_mimo when using the SEQ_MIMO structure with sequential data captures.
    //    */
    //} SequentialMimo;                       //!< Specifies fields for performing a MIMO analysis on a sequential MIMO data capture

    IQV_PACKET_FORMAT     packetFormat;                   //!< Indicates format of the MIMO packet Default: 0 (auto-detect mode)
    /*!< Valid values are as follows:
    \n IQV_PACKET_FORMAT_AUTO (auto-detect mode)
    \n IQV_PACKET_FORMAT_MIX (mixed mode format) 
    \n IQV_PACKET_FORMAT_GRE (greenfield format)
    \n IQV_PACKET_FORMAT_NONHT (Non-HT format) 
    \n IQV_PACKET_FORMAT_VHT   (VHT format)            
    */

    IQV_NUM_SYMBOLS_MODE    numSymbolsMode;     //!< Mode for determining number of symbols for analysis of OFDM-based packets. Default: IQV_NUM_SYMBOLS_MODE_LSIG

    double prePowStartSec;          //!< Specify prePower start in second default to be 8.8e-6
    double prePowStopSec;           //!< Specify prePower start in second default to be 15.2e-6
    
    IQV_ANALYZED_FREQUENCY_SEGMENT_TYPE frequencySegment;
	/*!< Configures which frequency segments are selected for Tx quality/power/spectrum analysis (OFDM) in the current active instrument channel
    \n Default : IQV_ANALYZED_FREQUENCY_SEGMENT_ALL
    \n Non-default values are relevant only for 802.11ac packets in 160 and 80+80 MHz channels.
    \n Spectrum analysis in 160 and 80+80 MHz channels with only one of the frequency segments selected works as spectrum analysis in an 80 MHz channel.
    */

    double pwr_detect_diff;            //!< Specifies the maximum power difference between packets that are expected to be detected. Default: 10
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
    int		enableFullPacketChannelEst;     //!< Full Packet Channel Estimation Default: 0 (channel estimate on long training fields)
    /*!<Valid values are as follows:	 
    \n 0 = channel estimate on long training fields
    \n 1 = reestimate channel on full packet before doing EVM calculation
    */
    int     packetFormat;                   //!< Indicates format of the MIMO packet Default: 0 (auto-detect mode)
    /*!< Valid values are as follows:
    \n IQV_MIMO_PACKET_FORMAT = 0 (auto-detect mode)
    \n IQV_MIMO_PACKET_FORMAT = 1 (mixed mode format) 
    \n IQV_MIMO_PACKET_FORMAT = 2 (greenfield format)
    \n IQV_PACKET_FORMAT_NONHT (Non-HT format)
    \n IQV_PACKET_FORMAT_VHT  (VHT format)  Not applicable for MIMO case. For 11AC only.
    */

    int 	SetDefault();		

    // ************************************************************************************
    // Not available SCPI based tester
    // ************************************************************************************
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
    int     powerDetectionSignals[N_MAX_TESTERS];
    /*!< \n \note Configures which input signals are used for packet detection (acquisition) in the current active instrument channel. 
    \n Default: [1] Currently only one signal can be selected.
    */

    char    *referenceFile;                 //!< Indicates name of the reference sequential_mimo analysis mode 
    /*!< Valid values are as follows:
    \n \c composite 
    \n \c sequential_mimo 
    \n This parameter is ignored for NxN systems.
    */
    int		decodePSDU;                     //!< Decode PSDU Default: 1 (decode PSDU)
    /*!<Valid values are as follows: 
    \n 0 = skip decoding of PSDU (sufficient for EVM measurements)
    \n 1 = decode PSDU
    */

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

    double prePowStartSec;          //!< Specify prePower start in second default to be 8.8e-6
    double prePowStopSec;           //!< Specify prePower start in second default to be 15.2e-6
    double pwr_detect_diff;         //!< Specifies the maximum power difference between packets that are expected to be detected. Default: 10

private:
    ////int UnpackMimoAnalysis(void *mx_result_in);
};


// ****************************************************************
// 802.11 a/g (OFDM) Specific Analysis Parameters Class (Derived from iqapiAnalysis)
// ****************************************************************
//! Specifies the analysis parameters for testing devices that are compliant with the IEEE 802.11 a/g (OFDM) standards specification.

class IQ_API iqapiAnalysisOFDM : public iqapiAnalysis
{
public:	
    iqapiAnalysisOFDM(void);	            //!< Constructor
    virtual ~iqapiAnalysisOFDM(void); 	    //!< Destructor 

    IQV_PH_CORR_ENUM		ph_corr_mode;	//!< Specifies the Phase Correlation mode. Default: IQV_PH_CORR_SYM_BY_SYM
    /*!< Available options are as follows:
    IQV_PH_CORR_OFF         = 1 indicates phase correction off \n           
    IQV_PH_CORR_SYM_BY_SYM  = 2 indicates Symbol-by-symbol correction \n
    IQV_PH_CORR_MOVING_AVG  = 3 moving avg. corr, 10 symbols        // This option is NOT supported on SCPI based tester
    */  
    IQV_CH_EST_ENUM	        ch_estimate;    //!< Specifies the Channel Estimate mode. Default: IQV_CH_EST_RAW
    /*!< Available options are as follows:
    \n IQV_CH_EST_RAW       = 1 Raw Ch Est, long train              (same as LTF on SCPI based tester)
    \n IQV_CH_EST_2ND_ORDER = 2 2nd Order Polyfit                   (This option is NOT supported on SCPI based tester)
    \n IQV_CH_EST_RAW_FULL  = 3 Raw Ch Est, full packet             (Same as DATA on SCPI based tester)
    */ 
    IQV_SYM_TIM_ENUM		sym_tim_corr;	//!< Specifies the Symbol Timing Correction mode for SISO. Default: IQV_SYM_TIM_ON
    /*!< Available options are as follows:
    \n IQV_SYM_TIM_OFF = 1 Symbol Timing Correction Off
    \n IQV_SYM_TIM_ON  = 2 Symbol Timing Correction On
    */
    IQV_FREQ_SYNC_ENUM		freq_sync;	    //!< Specifies the Frequency Sync mode. Default: IQV_FREQ_SYNC_LONG_TRAIN
    /*!< Available options are as follows:              
    \n IQV_FREQ_SYNC_SHORT_TRAIN = 1 Short Training Symbol          (This option is NOT supported on SCPI based tester)
    \n IQV_FREQ_SYNC_LONG_TRAIN  = 2 Long Training Symbol           (same as LTF on SCPI based tester)
    \n IQV_FREQ_SYNC_FULL_PACKET = 3 Full Data Packet               (Same as DATA on SCPI based tester)
    */
    IQV_AMPL_TRACK_ENUM		ampl_track;	    //!< Specifies the Amplitude Tracking mode. Default: IQV_AMPL_TRACK_OFF
    /*!< Available options are as follows:
    \n IQV_AMPL_TRACK_OFF = 1  Amplitude tracking off 
    \n IQV_AMPL_TRACK_ON  = 2  Amplitude tracking on
    */                     

    int SetDefault();	//!< Sets the analysis parameters to their default condition
    /*!< \note There must be a valid connection in order to use this function. Also,  this function is called in the constructor of the class (i.e., \c SetDefault occurs when calling \c new for this object)
    */

    // ************************************************************************************
    // To be supported on SCPI based tester
    // ************************************************************************************
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

    // ************************************************************************************
    // Not available SCPI based tester
    // ************************************************************************************
    IQV_OFDM_EVM_METHODS	ofdmevmmethod;	//!< Specifies the OFDM EVM Method. Default: IQV_OFDM_EVM_STANDARD
    /*!< Available options are as follows:
    \n IQV_OFDM_EVM_MULTIPATH &mdash; More tolerant of notches in the channel response 
    \n IQV_OFDM_EVM_STANDARD &mdash; Standard 802.11 a/g EVM Method 
    */

    double		            frequencyOffsetHz;  //!< Specifies Frequency offset. Default: 0
    /*!< If the data capture was performed with a frequency offset (i.e., the VSA and DUT are not set to the same frequency), this field may be used to enable the analysis to compensate the signal with the offset by the specified amount, in Hz.
    */		
    int		                manual_pkt_start;   //!< Bypasses the burst detection algorithm of the analysis when it is set to 1. Default: 0
    /*!< When set to 1, this field can be used (effectively) to bypass the analysis burst detection algorithm. It also indicates that the packet is starting within the first few samples of the data capture.
    */
    IQV_VSA_NUM_ENUM        vsaNum;         //!< For MIMO case, tester can capture more than 1 data set. This parameter specifies which data set to be used for doing FFT analysis. Default data set to be analyzed is IQV_VSA_NUM_1
    
    double prePowStartSec;          //!< Specify prePower start in second default to be 8.8e-6
    double prePowStopSec;           //!< Specify prePower start in second default to be 15.2e-6
    double pwr_detect_diff;         //!< Specifies the maximum power difference between packets that are expected to be detected. Default: 10

private:
    //int UnpackOFDMAnalysis(void *mx_result_in);
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

    double prePowStartSec;          //!< Specify prePower start in second default to be 8.8e-6
    double prePowStopSec;           //!< Specify prePower start in second default to be 15.2e-6

    int SetDefault();
    
private:
////    int Unpack11bAnalysis(void *mx_result_in);
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
    double  pwr_detect_diff;            //!< Specifies the maximum power difference between packets that are expected to be detected. Default: 10
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
    double	dataRate;		            //!< Sets the signal data rate, and should correspond to the input signal.  Default: 0	
    /*!< Specified in Mbps.
    Valid values are as follows: 
    0 - data rate is detected automatically
    1 - data rate is 1 Mbps
    2 ?data rate is 2 Mbps (EDR)
    3 ?data rate is 3 Mbps (EDR)
    4 ?data rate is 1 Mbps Low Energy mode
    */
    double	fm_lowpass_F3dbHz;	        //!< Sets the bandwidth for the FM IF filter  Default: 650000
    /*!< IF bandwidth = 2 * fm_lowpass_F3dbHz Hz
    */
    char    *leSyncWord;                //!<Sync Word for Low Energy BT. Eight Hex character. Default: '94826E8E'. Optional field.

    // **************************************************
    // Not applicable to SCPI based tester
    // **************************************************
    char    syncWord[17];               //!< This field is reserved for future use  Default: '0'

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

    double	res_bw;		        //!< This field is used to specify the resolution bandwidth in Hz. Default: 100000

    // *******************************************
    // The following input parameters are not supported in SCPI based tester
    // *******************************************
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

    int     powerDetectionSignals[MAX_SIGNAL_NUMBER];
    /*!< \n \note Configures which input signals are used for packet detection (acquisition) in the current active instrument channel. 
    \n Default: [1] Currently only one signal can be selected.
    */

    IQV_PACKET_FORMAT     packetFormat;                   //!< Default: 0 (auto-detect mode)
    /*!< Valid values are as follows:
    \n IQV_PACKET_FORMAT_AUTO (auto-detect mode) 
    \n IQV_PACKET_FORMAT_MIX (mixed mode format) 
    \n IQV_PACKET_FORMAT_GRE (greenfield format)
    \n IQV_PACKET_FORMAT_NONHT (Non-HT format) 
    \n IQV_PACKET_FORMAT_VHT   (VHT format)            
    */

    // *****************************************
    // For SCPI based tester only
    // *****************************************
    IQV_CONFIGURE_STANDARD  configureStandard;

    virtual int SetDefault();

protected:
    ////virtual int UnpackFftAnalysis(void *mx_result_in);
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

    IQV_VSA_NUM_ENUM  vsaNum;    //!< For MIMO case, tester can capture more than 1 data set. This parameter specifies which data set to be used for doing FFT analysis. Default: IQV_VSA_NUM_1
    
    // Note: The following input parameters are not applicable to SCPI based tester
    double	T_interval;	        //!< This field is used to specify the interval that is used to determine if power is present or not (sec). Default: 3.2e-6
    double	max_pow_diff_dB;    //!< This filed is used to specify the maximum power difference between packets that are expected to be detected.  Default: 15

    int SetDefault();	        //!< Sets the analysis parameters to their default condition. Note that there must be a valid connection in order to use this function. Also, note that this function is called in the constructor of the class (ie, SetDefault will happen when calling /c new for this object).
    
private:
//    int UnpackPowerAnalysis(void *mx_result_in) {return 1;};
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

    IQV_PACKET_FORMAT     packetFormat;                   //!< Default: 0 (auto-detect mode)
    /*!< Valid values are as follows:
    \n IQV_PACKET_FORMAT_AUTO (auto-detect mode)
    \n IQV_PACKET_FORMAT_MIX (mixed mode format) 
    \n IQV_PACKET_FORMAT_GRE (greenfield format)
    \n IQV_PACKET_FORMAT_NONHT (Non-HT format) 
    \n IQV_PACKET_FORMAT_VHT   (VHT format)            
    */

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

    double	dbResolutionBW;	    //!< It has to be greater or equal to 1000, Default: 100000

    // *******************************************
    // The following input parameters are not supported in SCPI based tester
    // *******************************************
    static const double dbResolutionBWLowerLimit;
    IQV_WINDOW_TYPE_ENUM windowType;            
    IQV_VSA_NUM_ENUM  vsaNum;   //!< For MIMO case, tester can capture more than 1 data set. This parameter specifies which data set to be used for doing FFT analysis. Default: IQV_VSA_NUM_1

    IQV_PACKET_FORMAT     packetFormat;                   //!< Default: 0 (auto-detect mode)
    /*!< Valid values are as follows:
    \n IQV_PACKET_FORMAT_AUTO (auto-detect mode)
    \n IQV_PACKET_FORMAT_MIX (mixed mode format) 
    \n IQV_PACKET_FORMAT_GRE (greenfield format)
    \n IQV_PACKET_FORMAT_NONHT (Non-HT format) 
    \n IQV_PACKET_FORMAT_VHT   (VHT format)            
    */

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
// Generic multi-use WIFI OFDM Results Class
// ****************************************************************
//! Specifies analysis results generated using \c iqapiAnalysisWifiOfdm objects
class IQ_API iqapiResultWifiOfdm : public iqapiResult
{
public:	
    iqapiResultWifiOfdm(void);	                //!< Constructor
    virtual ~iqapiResultWifiOfdm(void); 	    //!< Destructor

    int	                completePacket;		//!< Indicates a complete packet was demodulated    
    int	                packetDetection;    //!< Indicates valid packet start detected.
    int	                acquisition;		//!< Indicates valid HT packet start detected.
    int	                demodulation;		//!< Indicates streams demodulated (packet may be truncated)    

    iqapiMeasurement	*evmAvgAll;	        //!< Averaged, per stream; dimension 1 x NStreams
    iqapiMeasurement	*evmAvgAllComposite;	//!< EVM over all streams 
    iqapiMeasurement	*evmSymbols;	    //!< Per symbol and stream; dimension NStreams x NSymbols
    iqapiMeasurement	*evmTones;	        //!< Per tone and stream; dimension NStreams x Ntones
    iqapiMeasurement	*freqErrorHz;	    //!< Frequency error in Hz
    iqapiMeasurement	*symClockErrorPpm;	//!< Symbol clock error in ppm
    iqapiMeasurement	*PhaseNoiseDeg_RmsAll;  //!< RMS phase noise, over all received signals
    iqapiMeasurement	*PhaseNoiseDeg_Symbols;	//!< Per symbol and VSA signal; dimension NRx x Nsymbols
    iqapiMeasurement	*IQImbal_amplDb;	//!< IQ gain imbalance in dB, per stream.   Not available in composite mode
    iqapiMeasurement	*IQImbal_phaseDeg;	//!< IQ phase imbalance in degree, per stream.	Not available in composite mode

    iqapiMeasurement	*dcLeakageDbc;	    //!< DC leakage in dBc per Rx; dimension 1 x NRx
    iqapiMeasurement	*rxRmsPowerDb;	    //!< RMS power in dBm
    iqapiMeasurement	*isolationDb;       //!< Matrix indicating isolation between streams
    /*!<Column 1 is for Rx signal containing main power for stream 1, etc.; dimension NStreams x NStreams 
    \n \note Isolation can only be measured with direct-mapping signals. 
    \n Example: isolationDb = [0 -20; -25 0]
    - Stream 1 leaks into stream 2 signal with -20 dB 
    - Stream 2 leaks into stream 1 signal with -25 dB
    */	

    iqapiMeasurement	*analyzedRange;		//!< Start and end point of signal part that was used to for analysis. The API picks the first packet in the input signal for analysis; following packets are ignored.
    char			    *rateInfo_modulation; //!< Modulation type, i.e., '64 QAM'
    iqapiMeasurement	*rateInfo_bandwidthMhz;	//!< signal Bandwidth in MHz
    iqapiMeasurement	*rateInfo_dataRateMbps;	//!< Data rate in Mbps
    char			    *rateInfo_codingRate; //!< Coding rate, i.e., '3/4'	
    iqapiMeasurement	*rateInfo_spatialStreams;	//!< Data rate in Mbps
    iqapiMeasurement	*rateInfo_spaceTimeStreams;	//!< Data rate in Mbps

    iqapiMeasurement	*psduBits;	        //!< Decoded PSDU bits
    iqapiMeasurement	*serviceField;	    //!< Bits of service field	
    iqapiMeasurement	*demodSymbols;		//!<Complex demodulated symbols; dimension NTones x NSymbols X Nstreams 
    iqapiMeasurement	*channelEst;		//!< Channel estimate matrix; dimension NStreams x Ntones x NRx

    int			        numPsduBytes;	    //!< Number of bytes in PSDU
    bool	            htSigFieldCRC;	    //!< Indicates valid CRC on HT-SIG      11n/11ac
    bool	            psduCRC;		    //!< Indicates valid CRC on PSDU   

    iqapiMeasurement	*mcsIndex;	//!< Information from HT-SIG1: MCS index as decimal number.
    iqapiMeasurement	*bandwidth;	//!< Information from HT-SIG1: bandwidth (0) 20 MHz (1) 40 MHz.	
    iqapiMeasurement	*htLength;	//!< Information from HT-SIG1: value in length field as decimal number

    iqapiMeasurement	*advancedCoding;	//!< Information from HT-SIG-2: advanced coding
    iqapiMeasurement	*shortGI;	//!< Information from HT-SIG-2: short guard interval
    iqapiMeasurement	*stbc;		//!< Information from HT-SIG-2: 2 bits as decimal
    iqapiMeasurement	*aggregation;//!< Information from HT-SIG-2: aggregation
    iqapiMeasurement	*soundingPacket;	//!< Information from HT-SIG-2: sounding packet		

    iqapiMeasurement	*idxAnalyzedSigs;	//!< Indexes of input signals that were used for analysis.
    iqapiMeasurement	*idxDataTones;		//!< Index numbers of data tones in the result data (channelEst etc.); DC component is at index 1.
    iqapiMeasurement	*idxPilotTones;	    //!< Index number of pilot tones in the result data (channel Est etc.); DC component is at index 1.

    // For 11ac only 
    bool                vhtSigBFieldCRC;    //!< Indicates valid CRC on HT-SIGB   11ac only
    iqapiMeasurement    *vhtSigA2ShortGI;
    iqapiMeasurement    *vhtSigA2AdvancedCoding;
    iqapiMeasurement    *vhtSigA2McsIndex;

    iqapiMeasurement    *vhtSigA1Bandwidth;
    iqapiMeasurement    *vhtSigA1Stbc;
    iqapiMeasurement    *powerPreambleDbm;             //!< power on preamble in dBm

    // For SCPI based tester only
    int                 status;                     //!< Indicate if getting the OFDM result OK or not. 0 -> OK. non-zero => error code. Refer to SCPI document for error code description
    IQV_OFDM_PACKET_TYPE    *packetType;            //!< signal packet type
    iqapiMeasurement	*spectralFlatness;	        //!< spectral flatness
    iqapiMeasurement	*spectralFlatnessHighLimit;	//!< spectral flatness high limit
    iqapiMeasurement	*spectralFlatnessLowLimit;	//!< spectral flatness low limit
    iqapiMeasurement    *spectralFlatnessMargin;     //!< Margin array of spectral flatness results for given analyzed signal per packet in the current active instrument channel
    iqapiMeasurement    *spectralFlatnessMarginOffsetTone;    //!< Margin offset tones array of spectral flatness results for given analyzed signal per packet in the current active instrument channel
    iqapiMeasurement	*channelEstScpi;		    //!< Channel estimate with SCPI tester format. matrix; dimension NStreams x Ntones x NRx

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

    int	                completePacket;		//!< Indicates a complete packet was demodulated    
    int	                packetDetection;    //!< Indicates valid packet start detected.
    int	                acquisition;		//!< Indicates valid HT packet start detected.
    int	                demodulation;		//!< Indicates streams demodulated (packet may be truncated)    

    iqapiMeasurement	*evmAvgAll;	        //!< Averaged, per stream; dimension 1 xNStreams
    iqapiMeasurement	*evmAvgAllComposite;	//!< EVM over all streams 
    iqapiMeasurement	*evmSymbols;	    //!< Per symbol and stream; dimension NStreams x NSymbols
    iqapiMeasurement	*evmTones;	        //!< Per tone and stream; dimension NStreams x Ntones
    iqapiMeasurement	*freqErrorHz;	    //!< Frequency error in Hz
    iqapiMeasurement	*symClockErrorPpm;	//!< Symbol clock error in ppm
    iqapiMeasurement	*PhaseNoiseDeg_RmsAll;  //!< RMS phase noise, over all received signals
    iqapiMeasurement	*PhaseNoiseDeg_Symbols;	//!< Per symbol and VSA signal; dimension NRx x Nsymbols
    iqapiMeasurement	*IQImbal_amplDb;	//!< IQ gain imbalance in dB, per stream.       Not available in composite mode
    iqapiMeasurement	*IQImbal_phaseDeg;	//!< IQ phase imbalance in degree, per stream.	Not available in composite mode
    iqapiMeasurement	*preambleFreqErrorHz;   //!< Preamble frequency error, in Hz
    iqapiMeasurement	*preambleFreqErrorTimeUs; //!< Preamble frequency error versus time


    iqapiMeasurement	*dcLeakageDbc;	    //!< DC leakage in dBc per Rx; dimension 1 x NRx
    iqapiMeasurement	*rxRmsPowerDb;	    //!< RMS power in dBm
    iqapiMeasurement	*isolationDb;       //!< Matrix indicating isolation between streams
    /*!<Column 1 is for Rx signal containing main power for stream 1, etc.; dimension NStreams x NStreams 
    \n \note Isolation can only be measured with direct-mapping signals. 
    \n Example: isolationDb = [0 -20; -25 0]
    - Stream 1 leaks into stream 2 signal with -20 dB 
    - Stream 2 leaks into stream 1 signal with -25 dB
    */	

    iqapiMeasurement	*analyzedRange;		//!< Start and end point of signal part that was used to for analysis. The API picks the first packet in the input signal for analysis; following packets are ignored.
    char			    *rateInfo_modulation; //!< Modulation type, i.e., '64 QAM'
    iqapiMeasurement	*rateInfo_bandwidthMhz;	//!< Bandwidth in MHz
    iqapiMeasurement	*rateInfo_dataRateMbps;	//!< Data rate in Mbps
    char			    *rateInfo_codingRate; //!< Coding rate, i.e., '3/4'	
    iqapiMeasurement	*rateInfo_spatialStreams;	//!< Number of spatial streams		
    iqapiMeasurement    *rateInfo_spaceTimeStreams;	//!< Indicates number of spatial streams.

    iqapiMeasurement	*psduBits;	        //!< Decoded PSDU bits
    iqapiMeasurement	*serviceField;	    //!< Bits of service field	
    iqapiMeasurement	*demodSymbols;		//!<Complex demodulated symbols; dimension NTones x NSymbols X Nstreams 
    iqapiMeasurement	*channelEst;		//!< Channel estimate matrix; dimension NStreams x Ntones x NRx  => Partly implemented

    bool	            htSigFieldCRC;	    //!< Indicates valid CRC on HT-SIG
    bool	            psduCRC;		    //!< Indicates valid CRC on PSDU   

    iqapiMeasurement	*htSig1_mcsIndex;	//!< Information from HT-SIG1: MCS index as decimal number.
    iqapiMeasurement	*htSig1_bandwidth;	//!< Information from HT-SIG1: bandwidth (0) 20 MHz (1) 40 MHz.	
    iqapiMeasurement	*htSig1_htLength;	//!< Information from HT-SIG1: value in length field as decimal number

    iqapiMeasurement	*htSig2_advancedCoding;	//!< Information from HT-SIG-2: advanced coding
    iqapiMeasurement	*htSig2_shortGI;	//!< Information from HT-SIG-2: short guard interval
    iqapiMeasurement	*htSig2_stbc;		//!< Information from HT-SIG-2: 2 bits as decimal
    iqapiMeasurement	*htSig2_aggregation;//!< Information from HT-SIG-2: aggregation
    iqapiMeasurement	*htSig2_soundingPacket;	//!< Information from HT-SIG-2: sounding packet		

    iqapiMeasurement	*idxAnalyzedSigs;	//!< Indexes of input signals that were used for analysis.
    iqapiMeasurement	*idxDataTones;		//!< Index numbers of data tones in the result data (channelEst etc.); DC component is at index 1.
    iqapiMeasurement	*idxPilotTones;	    //!< Index number of pilot tones in the result data (channel Est etc.); DC component is at index 1.

    iqapiMeasurement    *powerPreambleDbm;             //!< power on preamble in dBm

    // ************************************************************************************
    // Added extra result for SCPI based tester
    // ************************************************************************************
    IQV_OFDM_PACKET_TYPE    *packetType;             //!< signal packet type
    iqapiMeasurement	*spectralFlatness;	//!< spectral flatness
    iqapiMeasurement	*spectralFlatnessHighLimit;	//!< spectral flatness high limit
    iqapiMeasurement	*spectralFlatnessLowLimit;	//!< spectral flatness low limit
//    iqapiMeasurement	*channelBandwidth;	//!< channel bandwidth
//    bool                vhtSigBFieldCRC;    //!< Indicates valid CRC on HT-SIGB   11ac only
    iqapiMeasurement	*channelEstScpi;		    //!< Channel estimate with SCPI tester format. matrix; dimension NStreams x Ntones x NRx

    // ************************************************************************************
    // Not available for SCPI based tester ( 
    // ************************************************************************************

    // ************************************************************************************
    // Won't be supported in SCPI based tester 
    // ************************************************************************************
    char	            *errTxt;		    //!< Error messages text string if analysis failed	

    iqapiMeasurement	*CCDF_xPowerDb;	    //!< Complementary Cumulative Distribution Function. dB above average power (x-axis)
    iqapiMeasurement	*CCDF_yProb;	    //!< Complementary Cumulative Distribution Function. Probability (y-axis)	
    iqapiMeasurement	*slicedSymbols;	    //!< Sliced symbols
    iqapiMeasurement	*legacyBits;	    //!< Information from the legacy signal field: 24 bits 
    iqapiMeasurement	*legacyLength;	    //!< Information from the legacy signal field: value in length field as decimal number.
    iqapiMeasurement	*legacyRate;	    //!< Information from the legacy signal field: data rate information. 	

    iqapiMeasurement	*htSig1_bits;	    //!< Information from HT-SIG1:24 bits.	
    iqapiMeasurement	*htSig2_bits;		//!< Information from HT-SIG-2: 24 bits
    iqapiMeasurement	*htSig2_numHTLF;	//!< Information from HT-SIG-2: 2 bits as decimal number	

    iqapiMeasurement	*mainPathStreamPowerDb;	//!< Main path power per stream (maximum element from each row in channel power matrix rxRmsPowerDb); dimension NStreams x 1		

    iqapiMeasurement	*htSig2_numESS;	        //!< Information from HT-SIG-2: 2 bits as decimal number	

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
    int			        dataRate;	    //!< Data rate in Mbps
    int			        numSymbols;	    //!< Number of symbols
    int			        numPsduBytes;	//!< Number of bytes in PSDU

    iqapiMeasurement	*evmAll;	    //!< EVM for entire frame
    iqapiMeasurement	*rmsPhaseNoise;	//!< Frequency RMS Phase Noise
    iqapiMeasurement	*freqErr;	    //!< Frequency error
    iqapiMeasurement	*clockErr;	    //!< Symbol Clock Error
    iqapiMeasurement	*dcLeakageDbc;      //!< DC leakage in dBc 
    iqapiMeasurement	*ampErrDb;	    //!< IQ Match Amplitude Error in dB
    iqapiMeasurement	*ampErr;	    //!< IQ Match Amplitude Error
    iqapiMeasurement	*phaseErr;	    //!< IQ Match Phase Error
    iqapiMeasurement    *evmSymbols;    //!< the number of actually analyzed symbols 
    iqapiMeasurement	*psdu;		    //!< The PSDU data. 0/1 values. Includes the MAC Header and the FCS, if present.
    iqapiMeasurement	*codingRate;    //!< Coding rate.
    iqapiMeasurement	*hhEst;		    //!< 64 element complex vector that represents the FFT output of the 2 long symbols in the PLCP pre-amble of the OFDM signal.
    iqapiMeasurement	*freq_vector;	//!< Preamble frequency error
    iqapiMeasurement	*freq_vector_time; //!< Preamble frequency error versus time
    iqapiMeasurement	*startPointers;	//!< Provides the approximate starting locations of each packet detected in the input data. If no packet detected, this is an empty vector.
    iqapiMeasurement	*stopPointers;	// Reserved for future use

    iqapiMeasurement	*evmData;	    //!< EVM for data part of frame	
    iqapiMeasurement	*evmPilot;	    //!< EVM for pilot part of frame

    // power related result
    iqapiMeasurement	*rmsPowerNoGap;	//!< Power RMS No Gap
    iqapiMeasurement	*pkPower;	    //!< Power Peak
    iqapiMeasurement	*rmsPower;	    //!< Power RMS with gap

    iqapiMeasurement    *powerPreambleDbm;             //!< power on preamble in dBm

    // For SCPI based tester only
    IQV_OFDM_PACKET_TYPE    *packetType;             //!< signal packet type
    iqapiMeasurement	*spectralFlatness;	//!< spectral flatness
    iqapiMeasurement	*spectralFlatnessHighLimit;	//!< spectral flatness high limit
    iqapiMeasurement	*spectralFlatnessLowLimit;	//!< spectral flatness low limit


    // ************************************************************************************
    // Not available for SCPI based tester
    // ************************************************************************************
    int			        plcpCrcPass;	//!< 1 = PLCP CRC Check Passed 

    iqapiMeasurement	*plcp;		    //!< PLCP (binary) data as 1/0s

    iqapiMeasurement	*rmsMaxAvgPower;//!< Power RMS Max. Average

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
    iqapiMeasurement    *powerPreambleDbm; //!< power on preamble in dBm

    iqapiMeasurement	*freqErrTimeVect; //!< Frequency Error for the the entire packet plotted against time
    iqapiMeasurement	*freqErrVect;	//!< Frequency Error for the entire packet		
    iqapiMeasurement	*maxFreqErr;	//!< The highest Frequency Error value for the entire packet

    iqapiMeasurement	*loLeakageDb;	//!< Lo Leakage

    iqapiMeasurement	*evmErr;        //!< EVM error

    // For SCPI tester only
    iqapiMeasurement	*demodSymbols;		//!<Complex demodulated symbols; dimension NTones x NSymbols X Nstreams 

    // For IQ2010 tester only
    iqapiMeasurement	*evmInPlcp;     //!< EVM in PLCP
    iqapiMeasurement	*evmInPreamble; //!< EVM on preamble
    iqapiMeasurement	*evmInPsdu;     //!< EVM in PSDU


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

    // ************************************************
    // NOTE : The following result parameters are available for SCPI based tester only
    // ************************************************
    iqapiMeasurement    *spectrumCheck;                     //!< Indicate if spectrum pass mask check or not. 0 => within limit, 1 => exceed limit, 2 => not applicable
    iqapiMeasurement    *spectrumMarginDb;                  //!< Margin (dB) in different regions . negative value means limit is exceeded 
    iqapiMeasurement    *spectrumMarginOffsetFreqHz;        //!< frequency (Hz) occurs at margin point in different regions
    iqapiMeasurement    *spectrumAverViolationPercentage;   //!< Spectrum average violation percentage
    iqapiMeasurement    *spectrumAverObw;                   //!< The average occupied bandwidth (OBW) containing 99% of total power for the specified packets
    iqapiMeasurement    *spectrumAverObwLowerFreq;          //!< Lower frequency limit where OBW starts
    iqapiMeasurement    *spectrumAverObwUpperFreq;          //!< Upper frequency limit where OBW stops
    iqapiMeasurement    *spectrumHighLimit;                 //!< The spectrum mask value for each offset frequency point            (exists for FW >= 1.10)
    iqapiMeasurement    *spectrumAverHighLimit;             //!< The average spectrum mask value for each offset frequency point    (exists for FW >= 1.10)

    iqapiMeasurement	*Freq_start;                        //!< Start frequency in Hz.
    iqapiMeasurement	*Freq_stop;	                        //!< Stop frequency in Hz.
    iqapiMeasurement	*Delta_freq;                        //!< Frequency increment in Hz.

    // ************************************************
    // NOTE : The following result parameters are not available for SCPI based tester
    // ************************************************
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


    iqapiMeasurement *P_av_all_dBm;         //!< Average power of whole capture in dBm
    iqapiMeasurement *P_peak_all_dBm;       //!< Peak power of whole capture in dBm

    iqapiMeasurement *P_av_all;             //!< Average power of whole capture
    iqapiMeasurement *P_peak_all;           //!< Peak power of whole capture

    char		     *msg;                  //!<  Possible warning and error messages, text string is returned.
    int		         valid;                 //!< Indicates valid results (1 valid, 0 invalid).
    iqapiMeasurement *start_loc;            //!< Start location for each packet detected.
    iqapiMeasurement *stop_loc;             //!< Stop location for each packet detected.
    iqapiMeasurement *start_sec;            //!< Start time, in seconds, for each packet detected.
    iqapiMeasurement *stop_sec;             //!< Stop time, in seconds, for each packet detected
    iqapiMeasurement *complete_burst;       //!< Indicates complete packet (start/stop found).

    iqapiMeasurement *P_av_each_burst;      //!< Average power of each burst
    iqapiMeasurement *P_pk_each_burst;      //!< Peak power of each burst
    iqapiMeasurement *P_av_no_gap_all;      //!< Average power, not counting gaps
    iqapiMeasurement *P_av_each_burst_dBm;  //!< Average power of each burst in dBm
    iqapiMeasurement *P_pk_each_burst_dBm;  //!< Peak power of each burst in dBm
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

    // ***********************************************************************
    // For any analysis_type 
    // ***********************************************************************
    char			    *analysisType;	        //!< Indicates which analysis is performed.
    iqapiMeasurement	*validInput;	        //!< Indicates whether or not the input wave was valid.
    iqapiMeasurement	*complete_burst;	    //!< Vector indicating whether or not each burst is complete. All elements are either (1: beginning and end transitions detected), or (0: burst is missing either beginning or end transition).
    iqapiMeasurement	*start_sec;	            //!< Starting time of each burst, in seconds
    iqapiMeasurement	*stop_sec;	            //!< End time of each burst, in seconds


    // ***********************************************************************
    // For analysis_type = PowerOnly, 20dbBandwidthOnly, PowerAndFreq, AllPlus or All
    // ***********************************************************************
    iqapiMeasurement	*valid;	                //!< Flag indicating whether or not the power portion of the bluetooth analysis was successful (1), or not (0).

    iqapiMeasurement    *P_av_all;              //!< Average power of whole capture
    iqapiMeasurement    *P_peak_all;            //!< Peak power of whole capture
    iqapiMeasurement    *P_av_no_gap_all;       //!< Average power, not counting gaps
    iqapiMeasurement    *P_av_each_burst;	    //!< Average power of each burst detected, in mWatts.
    iqapiMeasurement	*P_pk_each_burst;	    //!< Peak power of each burst detected, in mWatts.

    iqapiMeasurement    *P_av_all_dBm;         //!< Average power of whole capture in dBm
    iqapiMeasurement    *P_peak_all_dBm;       //!< Peak power of whole capture in dBm
    iqapiMeasurement    *P_av_no_gap_all_dBm;  //!< Average power, not counting gaps in dBm
    iqapiMeasurement    *P_av_each_burst_dBm;  //!< Average power of each burst in dBm
    iqapiMeasurement    *P_pk_each_burst_dBm;  //!< Peak power of each burst in dBm

    // ***********************************************************************
    // For analysis_type = PowerAndFreq, AllPlus or All
    // ***********************************************************************
    // With data rate = 1, 2 3 or 4
    double              dataRateRequest;	    //!< The requested data rate
    double              dataRateDetect;	        //!< The detected data rate

    // With data rate = 1, 2 or 3
    iqapiMeasurement	*freq_est;	            //!< Initial freq offset of each burst detected, in Hz.
    iqapiMeasurement	*freq_deviation;	    //!< Similar to the measurement result for deltaF1Avg as specified in BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2. Result measured from Header data. Result in Hz.
    iqapiMeasurement	*freq_deviationpktopk;	//!< Peak to Peak Frequency Deviation, in Hz during header.
    iqapiMeasurement	*deltaF2AvAccess;	    //!< Similar to the measurement result for deltaF2Avg as specified in BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2. Result measured from Access data. Result in Hz.
    iqapiMeasurement	*deltaF2MaxAccess;	    //!< Similar to the measurement result for deltaF2Max as specified in BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2. Result measured from Access data. Result in Hz.

    iqapiMeasurement	*freqEstPacketPointer;	//!< Pointers to which packet is pointed to in each element of freq_est.
    iqapiMeasurement	*freqDeviationPointer;	//!< Pointers to which packet is pointed to in each element of the above two measurements.

    // With data rate = 1
    iqapiMeasurement	*freq_estHeader;	    //!< Estimates the Frequency Offset during the Header in Hz.
    iqapiMeasurement	*freq_drift;	        //!< The maximum difference between the average frequency of the 4 preamble bits and the average frequency of any 10 bits (10 pattern) in the payload field of the returned packets.
    iqapiMeasurement	*deltaF1Average;	    //!< The measurement result for deltaF1Avg as specified in BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2. Requires 00001111 data pattern. Result in Hz.
    iqapiMeasurement	*deltaF2Average;	    //!< The measurement result for deltaF2Avg as specified in BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2. Requires alternating data pattern. Result in Hz.
    iqapiMeasurement	*deltaF2Max;	        //!< The measurement result for deltaF2Max as specified in BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2. Requires alternating data pattern. Result in Hz.
    iqapiMeasurement    *maxfreqDriftRate;	    //!< 1 Mbps only, takes the maximum drift over specified time interval
    iqapiMeasurement    *payloadErrors;	        //!< Reports the number of data errors in Payload. Not counting CRC. If -1, this value has not been calculated. If larger negative number, it reports the length of the payload data vector. This happens when the length of the payload vector is shorter than the length indicated in the payload header.

    iqapiMeasurement	*deltaF2MaxFailCount;   //!< The number of measured deltaF2Max that Failed standard's requirement (below 115kHz) as specified in BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2. Requires alternating data pattern. Result in Hz.
    iqapiMeasurement	*deltaF2MaxTotalCount;   //!< The total number of measured deltaF2Max as specified in BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2. Requires alternating data pattern. Result in Hz.
    
    iqapiMeasurement	*freqDriftPacketPointer;//!< Pointers to which packet is pointed to in each element of freq_drift.

    // With data rate = 2 or 3 (EDR)
    iqapiMeasurement	*EdrEVMvalid;	        //!< Indicates validity of EDR EVM Measurements.

    iqapiMeasurement	*EdrEVMAv;	            //!< RMS Differential EVM value in % (EDR only).
    iqapiMeasurement	*EdrEVMpk;	            //!< Peak Differential EVM value in % (EDR only).
    iqapiMeasurement	*EdrprobEVM99pass;	    //!< The percentage of symbols with EVM below the threshold. Threshold for 2 Mbps is 0.3 for 3 Mbps is 0.2.
    iqapiMeasurement    *EdrOmegaI;	            //!< \c Omega_i, same as \c freq_estHeader
    iqapiMeasurement	*EdrFreqExtremeEdronly;	//!< Reports the extreme value of the frequency variation during DPSK in Hz. Does not include the offset during the header.
    iqapiMeasurement    *EdrExtremeOmega0;	    //!< Extreme value of \c Omega_0, same as \c EdrFreqExtremeEdronly
    iqapiMeasurement    *EdrExtremeOmegaI0;	    //!< Extreme value of (Omega_0 + Omega_i)
    iqapiMeasurement	*EdrPowDiffdB;	        //!< Relative power of EDR section to FM section of packet, in dB.
    iqapiMeasurement	*EdrEVMvsTime;	        //!< Max DEVM Average as specified in: BLUETOOTH TEST SPECIFICATION Ver. 1.2/2.0/2.0 + EDR [vol 2] version 2.0.E.2.
    iqapiMeasurement    *EdrFreqvsTime;	        //!< Enhanced data rate for frequency versus time
    iqapiMeasurement    *EdrFreqvsTimeLength;	//!< Number of elements in EdrFreqvsTime

    // With data rate = 4 ( Low energy )
    iqapiMeasurement    *leValid;               //!< Is 1 if all calculations are valid, else 0. Valid output for any LE packet

    iqapiMeasurement    *leFreqOffset;          //!< Initial Frequency Offset calculated from preamble (f0 inTRM-LE/CA/05/C). Valid for any LE packet.
    iqapiMeasurement    *leDeltaF1Avg;          //!< Delta F1 Average as defined in TRM-LE/CA/05/C. Valid only if LE payload has 00001111 pattern.
    iqapiMeasurement    *leDeltaF2Avg;          //!< Average value of LEDaltaF2Max, i.e. delta F2 Average as defined in TRM-LE/CA/05/C.  Valid only if LE payload has 10101010 pattern.
    iqapiMeasurement    *leDeltaF2MaxMinValue;  //!< It is the minimum value of the vector LEDeltaF2Max. This is a single value. Double.
    iqapiMeasurement    *leFreqDevSyncAv;       //!< Average freq. deviation during synch. Measured at single sample per symbol after frequency offset during pre-amble has been removed. Valid for any LE packet.
    iqapiMeasurement    *leFnMax;               //!< Maximum value of the absolute value of LEFn. Valid only if LE payload has 10101010 pattern.
    iqapiMeasurement    *leDeltaF0FnMax;        //!< Maximum value of |f0 ?fn|, with n = 2, 3, ? Valid only if LE payload has 10101010 pattern.
    iqapiMeasurement    *leDeltaF1F0;           //!< Absolute value of f1 ?f0. Valid only if LE payload has 10101010 pattern.
    iqapiMeasurement    *leDeltaFnFn_5Max;      //!< Maximum value of |fn ?fn-5|, with n = 6, 7, ? Valid only if LE payload has 10101010 pattern.
    iqapiMeasurement    *leBelow185F2Max;       //!< Number of symbols with deviation below 185 kHz
    iqapiMeasurement    *leIsCrcOk;             //!< Is 1 if CRC is valid, else 0. Valid output for any LE packet

    iqapiMeasurement    *leFn;                  //!< Average frequency over 8 symbols, as defined in TRM-LE/CA/06/C. Valid only if LE payload has 10101010 pattern.

    iqapiMeasurement    *leDeltaF2Max;          //!< Sequence of Delta F2 Max as defined in TRM-LE/CA/05/C. Valid only if LE payload has 10101010 pattern.

    iqapiMeasurement    *lePreambleSeq;         //!< Preamble bits 0/1 as defined in 7.2

    iqapiMeasurement    *leSyncWordSeq;         //!< Synch Word bits as defined in 7.2

    iqapiMeasurement    *lePduHeaderSeq;        //!< PDU Header of the test packet as defined in 7.2 

    iqapiMeasurement    *lePduLengthSeq;        //!< PDU Length of the test packet as defined in 7.2

    iqapiMeasurement    *lePayloadSeq;          //!< PDU of the test packet as defined in 7.2

    iqapiMeasurement    *leDeltaF2MaxTotalCount;//!< Total number of measured Delta F2 Max as defined in TRM-LE/CA/05/C. Valid only if LE payload has 10101010 pattern.

    iqapiMeasurement    *lePduLength;           //!< PDU length in bytes. Valid for any LE packet

    iqapiMeasurement    *leCrcSeq;              //!< CRC of the PDU of test packet as defined in 7.2

    // ***********************************************************************
    // For analysis_type = ACP
    // ***********************************************************************
    int                 acpErrValid;            //!< 1 if ACP results are valid. Otherwise, it will be 0.

    iqapiMeasurement    *maxPowerAcpDbm;        //!< Reports max power in 1 MHz bands at specific offsets from center frequency. The offset in MHz is given in sequenceDefinition. Method according to 5.1.8 TRM/CA/06/C
    
    iqapiMeasurement    *maxPowerEdrDbm;        //!< Reports max power in 1 MHz bands at specific offsets from center frequency. The power at 0 MHz offset and +/-1 MHz offset is calculated differently from above. maxPowerEDRdBm follows 5.1.15 TRM/CA/13/C
    
    iqapiMeasurement    *leMaxPowerDbm;         //!< Same format as earlier outputs for adjacent channel power. See earlier documentation for maxPowerACPdBm.

    iqapiMeasurement    *sequenceDefinition;    //!< Offset in MHz for results in maxPowerACPdBm and maxPowerEDRdBm

    iqapiMeasurement    *meanNoGapPowerCenterDbm;  //!< Mean power in 1 MHz band. Is no gap power, i.e. only averaged when signal exceeds threshold

    // ***********************************************************************
    // For analysis_type = 20dbBandwidthOnly, OutputSpecFreqRangeOnly, AllPlus or All
    // ***********************************************************************
	int					trmca04cValid;			//!< 1 if TRM/CA/04/C Output Spectrum-Frequency range results are valid, else invalid
	iqapiMeasurement	*trmca04cfLHz;			//!< fL Hz for TRM/CA/04/C Output Spectrum-Frequency range
	iqapiMeasurement	*trmca04cfHHz;			//!< fH Hz for TRM/CA/04/C Output Spectrum-Frequency range

    iqapiMeasurement	*bandwidth20dB;	        //!< 20dB bandwidth value Hz.

    // **************************************************************
    // Not support on SCPI based tester
    // **************************************************************
    char			    *versionString;	        //!< Indicates the version of the BT analysis module.
    char			    *sCaution;              //!< Possible error and warning messages.
    char                *leMsg;                 //!< Cell array of messages. Valid output for any LE packet
    char                *acpErrMsg;             //!< Text string reporting error for ACP calc

};

// ****************************************************************
// Bluetooth Analysis Results Class
// ****************************************************************
//! Specifies analysis results generated with \c iqapiResultObw objects. 
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

// ******************************************************************
// pt99 Not supported... For ZhiYong to get through compilation purpose.
// Should use iqapiAnalysisWifiOfdm instead
// ******************************************************************
class IQ_API iqapiAnalysis11ac : public iqapiAnalysisMimo
{
public:
    iqapiAnalysis11ac(void) { };
    virtual ~iqapiAnalysis11ac(void) {};

};

class IQ_API iqapiResult11ac : public iqapiResultMimo
{
public:
    iqapiResult11ac() {};
    ~iqapiResult11ac() {};

    // For VHTSigA1
    iqapiMeasurement    *VHTSigA1Bits;
    iqapiMeasurement    *VHTSigA1Bandwidth;
    iqapiMeasurement    *VHTSigA1Stbc;

    // For VHTSigA2
    iqapiMeasurement    *VHTSigA2Bits;
    iqapiMeasurement    *VHTSigA2ShortGI;
    iqapiMeasurement    *VHTSigA2AdvancedCoding;
    iqapiMeasurement    *VHTSigA2McsIndex;

    // For VHTSigB
    iqapiMeasurement    *VHTSigBBits;

    int                 vhtSigBFieldCRC;        //	Valid.vhtSigBFieldCRC

};

class IQ_API iqapiAnalysisVHT80WideBand : public iqapiAnalysis
{
public:	
    iqapiAnalysisVHT80WideBand(void) ;	        //!< Constructor
    virtual ~iqapiAnalysisVHT80WideBand(void); 	//!< Destructor

    static const double dbResolutionBWLowerLimit;
    IQV_WINDOW_TYPE_ENUM windowType;
    double	dbResolutionBW;	    //!< It has to be greater or equal to 1000, Default: 100000

    IQV_VSA_NUM_ENUM  vsaNum;   //!< For MIMO case, tester can capture more than 1 data set. This parameter specifies which data set to be used for doing FFT analysis. Default: IQV_VSA_NUM_1

    IQV_PACKET_FORMAT     packetFormat;                   //!< Indicates format of the MIMO packet Default: 0 (auto-detect mode)
    /*!< Valid values are as follows:
    \n IQV_PACKET_FORMAT_AUTO (auto-detect mode)
    \n IQV_PACKET_FORMAT_MIX (mixed mode format) 
    \n IQV_PACKET_FORMAT_GRE (greenfield format)
    \n IQV_PACKET_FORMAT_NONHT (Non-HT format) 
    \n IQV_PACKET_FORMAT_VHT   (VHT format)            
    */

    int SetDefault();
};

class IQ_API iqapiResultVHT80WideBand : public iqapiResult {
public:
    iqapiResultVHT80WideBand();	            //!< Constructor
    virtual ~iqapiResultVHT80WideBand();	//!< Destructor

    int len_of_ffts;                //!< the length of ffts 
    iqapiResultFFT ** ffts;
};

class IQ_API iqapiAnalysisVHT160WideBand : public iqapiAnalysisVHT80WideBand
{
};

class IQ_API iqapiResultVHT160WideBand : public iqapiResultVHT80WideBand
{
};

