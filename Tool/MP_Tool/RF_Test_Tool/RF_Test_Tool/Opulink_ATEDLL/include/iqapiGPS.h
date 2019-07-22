#ifndef _IQAPIGPS_H_
#define _IQAPIGPS_H_

// ****************************************************************
// Date   : 3/24/2009
// ****************************************************************

#include "iqapiDefines.h"
#include "iqapiHndlBase.h"

//class iqapiHndl;

/// ********************************************************************************
/// GPS define statement
/// ********************************************************************************
#define     RESPONSE_SIZE              5000
#define     VERSION_SIZE               128
#define     HW_VERSION_SIZE            8
#define     SERIAL_NUMBER_SIZE         8
#define     MAX_CHANNEL_NUMBER         6     
#define     MAX_CHANNEL_POWER_IN_DBM   -60.0
#define     MIN_CHANNEL_POWER_IN_DBM   -145.0
#define     MAX_SATELLITE_NUMBER       37
#define     MIN_SATELLITE_NUMBER       1
#define     MAX_FREQUNECY_OFFSET       10000.0
#define     MIN_FREQUNECY_OFFSET       -10000.0
#define     MAX_CW_FREQUNECY_OFFSET    500000.0
#define     MIN_CW_FREQUNECY_OFFSET    -500000.0
#define     EEPROM_32BIT_SIZE          8        // 32 bits => 4 bits for one ASCII byte


//! Specifies analysis parameters for testing devices that are compliant with the GPS specification standards.
class IQ_API iqapiHndlGps
{      
   iqapiHndlGps() {};	//!< Constructor

public:
   iqapiHndlGps(const char *const ip1, const char *const serialNum, iqapiHndlBase *parentParam);
   //! Destructor
   ~iqapiHndlGps();

   int            lastErrCode;	//!< Indicates last error code
   char           lastErr[MAX_LEN_ERR_TXT];	//!< Indicates last error

  
   int   GetNumberOfChannel(int *numberOfChannel); //!< Returns the number of channels 
   int   SetTx(bool applyAll = true);	//!< Sets Tx parameters. If applyAll is false, it will set only the parameter that is called by the corresponding function. If applyAll is true, it will set all parameters to the tester
   int   GetTx();	//!< Gets Tx parameters      
   int   LoadScenarioFile(const char *const fileName);	//!< Loads scenario files
   int   PlayScenarioFile();	//!< Plays scenario file
   int   StopScenarioFile();  //!< Stops the scenario file when it is playing
   int   TotalTimeForScenarioFile(unsigned int *time);    //!< total time to play the scenario file. time in second
   int   TimeRemainForScenarioFile(unsigned int *time);    //!< Remaining time to play scenario file. time in second
   int   InitialTOW(unsigned int *time);                          //!< Initalial time of the week.  time in second
   int   GetEllipsoidalCoordinate(double *longitude, double *latitude, double *height);   //!< Get Ellipsoidal coordinate
   int   GetCartesianCoordinate(double *xAxis, double *yAxis, double *zAxis);    //!< Get Cartesian coordinate
   int   SetReplayScenarioMode(IQV_GPS_REPLAY_MODE replayMode);                  //!< Set replay scenario mode. ON/OFF. If on, it will keep on replaying scenario file. if OFF, play only once
   /*!< If this mode is set to on, the scenario files will be replayed; if set to off, the scenario files play only once
   */
   int   GetReplayScenarioMode(IQV_GPS_REPLAY_MODE *replayMode);                 //!< Get the replay scenario mode setting

   void  SetParamOperationMode(IQV_GPS_OPERATION_MODE operationModeParam);//!< Sets parameters for modulation type
   void  GetParamOperationMode(IQV_GPS_OPERATION_MODE *operationModeParam);	//!< Gets parameters for modulation type

   void  SetParamRfOutput(IQV_GPS_RF_OUTPUT rfOutputParam); //!< Sets RF output parameter
   void  GetParamRfOutput(IQV_GPS_RF_OUTPUT *rfOutputParam);	//!< Gets RF output parameter

   void  SetParamTriggerState(IQV_GPS_TRIGGER_STATE triggerStateParam);	//!< Controls the trigger state 
   void  GetParamTriggerState(IQV_GPS_TRIGGER_STATE *triggerStateParam); //!< Returns the trigger state

   int  SetParamModulationState(IQV_GPS_MODULATION_STATE modulationStateParam, IQV_GPS_CHANNEL_NUMBER channelNumber); //!< Sets the modulation state for multiple channels
   
   int  GetParamModulationState(IQV_GPS_MODULATION_STATE *modulationStateParam, IQV_GPS_CHANNEL_NUMBER channelNumber); //!< Gets the modulation state for multiple channels

   int  SetParamSatelliteNumber(int satelliteNumberParam, IQV_GPS_CHANNEL_NUMBER channelNumber); //!< Sets the satellite number for multiple channels 
   
   /*!< Sets the satellite number for a maximum of six channels
   */
   int  GetParamSatelliteNumber(int *satelliteNumberParam, IQV_GPS_CHANNEL_NUMBER channelNumber); //!< Gets the satellite number for multiple channels
   /*!< Gets the satellite number for a maximum of six channels
   */
   int  SetParamPowerDbm(double powerDbmParam, IQV_GPS_CHANNEL_NUMBER channelNumber); //!< Sets the signal power for multiple channels
   /*!< Sets the signal power for a maximum of six channels
   */   
   int  GetParamPowerDbm(double *powerDbmParam, IQV_GPS_CHANNEL_NUMBER channelNumber);	//!< Gets the signal power for multiple channels
   
   /*!< Gets the signal power for a maximum of six channels
   */
   int  SetParamDopplerFrequency(int dopplerFrequencyParam);   //!< Sets the doppler frequency parameters
   void GetParamDopplerFrequency(int *dopplerFrequencyParam);  //!< Gets the doppler frequency parameters

   int  SetParamCwFrequencyOffset(int frequencyOffset);   //!< Sets the frequency parameters
   void GetParamCwFrequencyOffset(int *frequencyOffset);  //!< Gets the frequency parameters

   int Activate(); //!< Activates the GPS application when the application is connected to the test system
   
   /*!< This is the first function that must be called after connecting the GPS application to the test system.
   */
   int Deactivate(); //!< Deactivates the GPS application
   
   /*!< This is last function that must be called before disconnecting the GPS application from the test system.
   */
  
   int IsActivate(bool *retActivate); //!< Indicates that the GPS application is activated
   int IsConnected(bool *retConnected); //!< Indicates that the GPS application is connected

   int GetFWInterfaceVersion(char *fwInterfaceVersion, int bufSize); //!< Gets firmware interface version
   int GetHardwareVersion(char *buff, int bufflen);	//!< Gets the hardware version of the test system
   int GetSoftwareVersion(char *buff, int bufflen);	//!< Gets the software version of the test system
   int GetDriverVersion(char *buff, int bufflen);	//!< Gets the USB driver version
   int GetFpgaRfVersion(char *buff, int bufflen);	//!< Gets the FPGA RF version of the test system
   int GetFpgaBbVersion(char *buff, int bufflen);	//!< Gets the FPGA BB version of the test system
   int GetSerialNumber(char  *buff, int bufflen);	//!< Gets the serial number of the test system

   int GetSystemConfigurationDate(char *buff, int bufflen);    //!< Get the GPS/FM HW configuration date
   int GetCalibrationDate(char *buff, int bufflen);            //!< Get GPS/FM calibration date
   int IsLicenseAvailable(IQV_LICENSE_TYPE licenseType, bool *returnResult);  //!< Checks if license is available for GPS, FM VSG or FM VSA. Not other license type
   bool IsGlonassAvailable();

   // N-Head implementation
   int  GetToken(unsigned int token, unsigned int *tokenID);
   int  SetToken (unsigned int token, unsigned int tokenID);    

//! This function is reserved for internal use.
   bool  GetConGps() { return conGps;};

private:
   iqapiHndlBase  *parentHndl;
   bool           conGps;	//!< Indicates whether GPS is connected or not 
   bool           isGlonassAvailable;
   char           response[RESPONSE_SIZE];	//!<  Buffer size for the response
   int            numberOfChannel;	//!< Indicates number of GPS channels
   char           hwVersion[VERSION_SIZE+1];	//!< Indicates hardware version number
   char           swVersion[VERSION_SIZE+1];	//!< Indicates software version number
   char           driverVersion[VERSION_SIZE+1];	//!< Indicates driver version number 
   char           serverVersion[VERSION_SIZE+1];	//!< Indicates firmware version number
   char           fpgaRfVersion[VERSION_SIZE+1];  //!< Indicates FPGA RF version number
   char           fpgaBbVersion[VERSION_SIZE+1];  //!< Indicates FPGA BB version number
   char           serialNumber[SERIAL_NUMBER_SIZE+1];	//!< Indicates serial number of the test system

   char           systemConfigurationDate[EEPROM_32BIT_SIZE+1];
   char           NotUsed32bit[EEPROM_32BIT_SIZE+1];
   char           calibrationDate[EEPROM_32BIT_SIZE+1];
   char           licenseInfo[EEPROM_32BIT_SIZE+1];
   int            licenseGps;
   bool           licenseGlonass;
   bool           licenseFmVsg;
   bool           licenseFmVsa;
   bool           licenseFmAudio;
   

   IQV_GPS_OPERATION_MODE        operationMode; //!< Sets the operation mode
   /*!< Sets CW or modulated mode.
   \n Valid values are as follows:
   \n 0=CA disabled
   \n 1=CA enabled
   */
   IQV_GPS_MODULATION_STATE      modulationState[MAX_CHANNEL_NUMBER]; //!< Sets the modulation state
   
   /*!< Valid values are as follows:
   \n 0=no modulation
   \n 1=modulated
   */
   IQV_GPS_RF_OUTPUT             rfOutput;	//!< Sets RF output power
   /*!< Valid values are as follows;
   \n -70 to -145 for modulation and modulation off
   \n -60 to -145 for no modulation (CW)
   */
   IQV_GPS_TRIGGER_STATE         triggerState;
   int                           satelliteNumber[MAX_CHANNEL_NUMBER];   //!< Indicates the satellite number   
   /*!< Valid values are between 1 to 37 for each channel
   */
   double                        powerDbm[MAX_CHANNEL_NUMBER];   //!< Sets the signal power   
   /*!< Valid values are between -150 to -60 with a resolution of 0.1 dBm
   */
   int                        dopplerFrequency;        //!< Sets the doppler frequency(Non-CW mode only)
   int                        cwFrequencyOffset;       //!< Sets the frequency offset(CW mode only)
   /*!< Valid values are between -10000 to 10000 Hz   */
   int   GetServerVersion(char *serverVersion, int bufSize); //!< Gets the firmware version
   int   GetProductInfo(char *productInfo, int bufSize); //!< Gets the product information
   int   GetErrorDescription(uint32 errCode, char *errMsg, unsigned int bufSize); //!< Gets the error description
   int   GetErrorCodeAndDescription();	//!< Gets the error code and description of the error
   int   ConInit(const char *const ip1 = "127.0.0.1", const char *const serialNum="0"); //!< Initializes a connection
   int   ConClose(); //!< Closes an open connection
   int   _glonass_check_availability();
   int   _glonass_deactivate();
   int   _glonass_is_activated(bool &retActivate);
   int   _glonass_get_power_and_doppler(double &powerDbmParam, int &dopplerDbmParam);

   int   ExecSingleCommandAndCheck(char *command, char **singleCommandResult); //!< Executes a single command and checks if the result is correct

   //! Error when retrieving product info
   unsigned int   errProductInfo; //!< Retrieves error, if any, in product information 
   char           *singleCommandResult; //!< Result of a single command
};

#endif // _IQAPIGPS_H_
