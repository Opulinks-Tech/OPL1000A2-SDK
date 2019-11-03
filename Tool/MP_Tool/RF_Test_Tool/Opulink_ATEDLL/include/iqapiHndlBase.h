#ifndef _IQAPIHNDLBASE_H_
#define _IQAPIHNDLBASE_H_
// ****************************************************************
// file : iqapiHndlBase.h
// Note : Should not contain any code that is matlab related so that GPS/FM doesn't depend on matlab ( No MCR installed for GPS/FM api ) 
//        Matlab code can be placed to iqapiHndlMatlab.h
// ****************************************************************

#include "iqapiDefines.h"

// ****************************************************************
// Define statement
// ****************************************************************
#define _INCLUDE_GF_

#ifdef _INCLUDE_GF_
//#pragma comment(lib, "FMAnalysisDll.lib")
#endif

#define _INCLUDE_NFC_

#define  MAX_LEN_MES       512

enum IQV_FPGA_REFERENCE_CAPTURE_COMPENSATE
{
    IQV_FPGA_REFERENCE_CAPTURE_COMPENSATE_START_GET_STATUS,
    IQV_FPGA_REFERENCE_CAPTURE_COMPENSATE_END_GET_STATUS,

    IQV_FPGA_REFERENCE_CAPTURE_COMPENSATE_START_SET_TX_RX,
    IQV_FPGA_REFERENCE_CAPTURE_COMPENSATE_END_SET_TX_RX,

    IQV_FPGA_REFERENCE_CAPTURE_COMPENSATE_START_SET_RX,
    IQV_FPGA_REFERENCE_CAPTURE_COMPENSATE_END_SET_RX,

    IQV_FPGA_REFERENCE_CAPTURE_COMPENSATE_START_AGC,
    IQV_FPGA_REFERENCE_CAPTURE_COMPENSATE_END_AGC,

    IQV_FPGA_REFERENCE_CAPTURE_COMPENSATE_START_CAPTURE,
    IQV_FPGA_REFERENCE_CAPTURE_COMPENSATE_END_CAPTURE,

    IQV_FPGA_REFERENCE_CAPTURE_COMPENSATE_START_MISC_MW_CMD,
    IQV_FPGA_REFERENCE_CAPTURE_COMPENSATE_END_MISC_MW_CMD,

    IQV_FPGA_REFERENCE_CAPTURE_COMPENSATE_RETRIEVE_CENTER

};

// ! Mac OS X needs this to correctly set up working directory when launched from the Finder (no-op on Windows)
// passing null sets it to the application directory
IQ_API int iqapiSetWorkingDirectory(char *newpath);

////class IQ_API iqapiCaptureHT40Data3
////{
////public:
////   iqapiCaptureHT40Data3();
////   ~iqapiCaptureHT40Data3();
////
////   double *realCenter[N_MAX_TESTERS];
////   double *imagCenter[N_MAX_TESTERS];
////   int    lengthCenter[N_MAX_TESTERS];
////   double frequencyCenter[N_MAX_TESTERS];
////
////   double *realLeft[N_MAX_TESTERS];
////   double *imagLeft[N_MAX_TESTERS];
////   int    lengthLeft[N_MAX_TESTERS];
////   double frequencyLeft[N_MAX_TESTERS];
////
////   double *realRight[N_MAX_TESTERS];
////   double *imagRight[N_MAX_TESTERS];
////   int    lengthRight[N_MAX_TESTERS];
////   double frequencyRight[N_MAX_TESTERS];
////
////   double  sampleFreqHz[N_MAX_TESTERS];	//!< This double array represents the sample frequency of real and imag. sampleFreqHz[x] is the sampling rate of real[x] and imag[x], where x is the VSA number (minus one).
////
////   int Save(char *fileName);
////
////    char lastErr[1024];
////};

class iqapiCaptureHT40Data3;

class IQ_API iqapiHndlBase
{
public:
   iqapiHndlBase();
   ~iqapiHndlBase() {};

	static void StartMlDebug(void); //!< \note This function is reserved for Debugger Output
	static void EndMlDebug(void);	//!< \note This function is reserved for Debugger Output

	char			lastErr[MAX_LEN_ERR_TXT]; //!< Indicates the last error message text.

   iqapiCaptureHT40Data3 *dataHt40Data3;

   virtual int    IsLicenseAvailable(IQV_LICENSE_TYPE licenseType, bool *returnResult) = 0;  //!< Checks if license is available
   virtual bool   IsHardwareVersion(IQV_HARDWARE_VERSION_ENUM hardwareVersion) = 0; //!< Check hardware version
   virtual int    GetStatus() { return 0;};            //!< Retrieves hardware status from the test system.   

// Internal use
   virtual int    GetTxRfFreqHz(double *txRfFreqHz);   //!< Get the current transmiter frequency
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
    \n \c 11a= analysis for 802.11a by using iqapiAnalysisMimo. No 'mode' need to be specified
    \n \c 11ac= analysis for 11ac by using iqapiAnalysisMimo. 

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
// Generic Measurement Result Class
// ****************************************************************
//! Specifies the measurement analysis parameters.
class IQ_API iqapiMeasurement
{
public:	
	iqapiMeasurement(void); //!< Constructor
	~iqapiMeasurement(void); //!< Destructor
	
	double	*real;	//!< Pointer to the \c real vector. This value is set to NULL if no data is available.
	double	*imag;	//!< Pointer to \c imag (imaginary) vector. This value is set to NULL if no data is available.
	int	length;	//!< Length of \c real vector; also applies to \c imag vector if the value is not NULL.
	int	m;	//!< This value is reserved for future use.
	int	n;	//!< This value is reserved for future use.
	int	o;	//!< This value is reserved for future use.
	int ConsolePrint(char *resultTitle);	//!< Displays the measurement in the console window. Data and array index are displayed after the \c resultTitle parameter.
};

class IQ_API iqapiReferenceSignalData
{
public:
	iqapiReferenceSignalData();
	~iqapiReferenceSignalData();
	iqapiReferenceSignalData(const iqapiReferenceSignalData &src);
    iqapiReferenceSignalData & operator = (const iqapiReferenceSignalData &src);

	int length[N_MAX_TESTERS];
	double *real[N_MAX_TESTERS];
	double *imag[N_MAX_TESTERS];
	
	double reserved;

private:
	void deepCopy(const iqapiReferenceSignalData &src);
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
	int         dataEncrypted[N_MAX_TESTERS];   //!< Indicate if data is encrypted(1) or not(0)
    
	iqapiReferenceSignalData referenceDataLeft;
	iqapiReferenceSignalData referenceDataRight;
	iqapiReferenceSignalData referenceDataCenter;

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

private:
	void deepCopy(const iqapiSignalData &src);
   
};

#endif // _IQAPIHNDLBASE_H_
