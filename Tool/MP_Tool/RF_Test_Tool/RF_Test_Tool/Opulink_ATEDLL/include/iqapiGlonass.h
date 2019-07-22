#ifndef IQAPIGLONASS_H_
#define IQAPIGLONASS_H_

#include "iqapiDefines.h"
#include "iqapiHndlBase.h"

#define IQV_GLONASS_RESPONSE_SIZE			5000
#define IQV_GLONASS_MAX_POWER				(-60.0)
#define IQV_GLONASS_MIN_POWER				(-145.0)
#define IQV_GLONASS_MAX_SATELLITE_NUMBER	24
#define IQV_GLONASS_MIN_SATELLITE_NUMBER	1
#define IQV_GLONASS_MAX_FREQ_OFFSET			10000.0 // in kHz
#define IQV_GLONASS_MIN_FREQ_OFFSET			(-10000.0) // in kHz
#define IQV_GLONASS_CENTER_FREQUENCY		1602.0 // in MHz
#define IQV_GLONASS_FREQ_INTERVAL			0.5625 // in MHz

/*!
 * \class	iqapiHndlGlonass
 *
 * \brief	This class includes functions to control Glonass
 *
 */
class IQ_API iqapiHndlGlonass
{
public:
	/*!
	 * \fn	iqapiHndlGlonass(iqapiHndlBase *parentParam)
	 *
	 * \brief	Constructor
	 *
	 * \param	parentParam	A base class object of iqapiHndl
	 */
	iqapiHndlGlonass(iqapiHndlBase *parentParam);

	/*!
	 * \fn	iqapiHndlGlonass(iqapiHndlBase *parentParam, bool isAvailable)
	 *
	 * \brief	Constructor
	 *
	 * \param	parentParam		A base class object of iqapiHndl
	 * \param	isAvailable		Indicates whether the tester supports Glonass or not
	 */
	iqapiHndlGlonass(iqapiHndlBase *parentParam, bool isAvailable); //!< Constructor

	/*!
	 * \fn	~iqapiHndlGlonass()
	 *
	 * \brief	Destructor.
	 */
	~iqapiHndlGlonass();

	/*!
	 * \fn	int Activate()
	 *
	 * \brief	This function activates Glonass. It must be activated prior to any usage of Glonass.<br>
	 *			If GPS is already activated, it deactivates GPS first.<br>
	 *			Upon activation, it automatically syncs parameter values.<br>
	 *			Some parameter values are affected by changes in Gps parameter values.(if already activated)
	 */
	int Activate();

	/*!
	 * \fn	int Deactivate()
	 *
	 * \brief	This function deactivates Glonass. The connection is still alive after deactivation.
	 */
	int Deactivate();

	/*!
	 * \fn	int IsActivate(bool *retActivate)
	 *
	 * \brief	This function check if Glonass has been activated.
	 *
	 * \param	[out] retActivate	True if activated, false otherwise
	 *
	 * \return	IQAPI_ERR_OK if no error, returns an error code otherwise
	 */
	int IsActivate(bool *retActivate);

	/*!
	 * \fn	int IsConnected(bool *retConnected)
	 *
	 * \brief	This function check if it is connected to a tester.
	 *
	 * \param	[out] retConnected	True if connected, false otherwise
	 *
	 * \return	IQAPI_ERR_OK if no error, returns an error code otherwise
	 */
	int IsConnected(bool *retConnected);

	/*!
	 * \fn	int SetTx(bool applyAll)
	 *
	 * \brief	This function sets local parameter values to a tester.
	 *
	 * \param	applyAll	True to set all parameter values. False to set only updated parameter values
	 *
	 * \return	IQAPI_ERR_OK if no error, returns an error code otherwise
	 */
	int SetTx(bool applyAll = true);

	/*!
	 * \fn	int GetTx()
	 *
	 * \brief	This function updates local parameter values from a tester.
	 *
	 * \return	IQAPI_ERR_OK if no error, returns an error code otherwise
	 */
	int GetTx();

	/*!
	 * \fn	int SetParamPowerDbm(double powerDbmParam)
	 *
	 * \brief	This function sets a power level.<br>
	 *			It does not make any changes to the tester. Invoke SetTx() to actually apply changes to the tester.
	 *
	 * \param	powerDbmParam	Power level to set (ranges : IQV_GLONASS_MIN_POWER ~ IQV_GLONASS_MAX_POWER)
	 *
	 * \return	IQAPI_ERR_OK if no error, returns an error code otherwise
	 */
	int SetParamPowerDbm(double powerDbmParam);

	/*!
	 * \fn	int GetParamPowerDbm(double *powerDbmParam)
	 *
	 * \brief	This function get a power level.<br>
	 *			It does not directly retrieve from a tester. Invoke GetTx() first to do so.
	 *
	 * \param	[out] powerDbmParam	Power level(ranges : IQV_GLONASS_MIN_POWER ~ IQV_GLONASS_MAX_POWER)
	 *
	 * \return	IQAPI_ERR_OK if no error, returns an error code otherwise
	 */
	int GetParamPowerDbm(double *powerDbmParam);

	/*!
	 * \fn	int SetParamDopplerFrequency(int dopplerFrequencyParam)
	 *
	 * \brief	This function sets a doppler frequency.<br>
	 *			It does not make any changes to the tester. Invoke SetTx() to actually apply changes to the tester.
	 *
	 * \param	dopplerFrequencyParam	Doppler frequency to set (ranges : IQV_GLONASS_MIN_FREQ_OFFSET ~ IQV_GLONASS_MAX_FREQ_OFFSET)
	 *
	 * \return	IQAPI_ERR_OK if no error, returns an error code otherwise
	 */
	int SetParamDopplerFrequency(int dopplerFrequencyParam);

	/*!
	 * \fn	int GetParamDopplerFrequency(int *dopplerFrequencyParam)
	 *
	 * \brief	This function get a doppler frequency.<br>
	 *			It does not directly retrieve from a tester. Invoke GetTx() first to do so.
	 *
	 * \param	[out] dopplerFrequencyParam	Doppler frequency(ranges : IQV_GLONASS_MIN_FREQ_OFFSET ~ IQV_GLONASS_MAX_FREQ_OFFSET)
	 *
	 * \return	IQAPI_ERR_OK if no error, returns an error code otherwise
	 */
	int GetParamDopplerFrequency(int *dopplerFrequencyParam);

	/*!
	 * \fn	int SetParamFrequencyChannel(IQV_GLONASS_FREQUENCY_CHANNEL frequencyChannelParam)
	 *
	 * \brief	This function sets a signal frequency channel.<br>
	 *			It does not make any changes to the tester. Invoke SetTx() to actually apply changes to the tester.
	 *
	 * \param	frequencyChannelParam	Signal frequency channel to set
	 *
	 * \return	IQAPI_ERR_OK if no error, returns an error code otherwise
	 */
	int SetParamFrequencyChannel(IQV_GLONASS_FREQUENCY_CHANNEL frequencyChannelParam);

	/*!
	 * \fn	int GetParamFrequencyChannel(IQV_GLONASS_FREQUENCY_CHANNEL *frequencyChannelParam)
	 *
	 * \brief	This function get a signal frequency channel.<br>
	 *			It does not directly retrieve from a tester. Invoke GetTx() first to do so.
	 *
	 * \param	[out] frequencyChannelParam	Signal frequency channel
	 *
	 * \return	IQAPI_ERR_OK if no error, returns an error code otherwise
	 */
	int GetParamFrequencyChannel(IQV_GLONASS_FREQUENCY_CHANNEL *frequencyChannelParam);

	/*!
	 * \fn	void SetParamRfOutput(IQV_GLONASS_RF_OUTPUT rfOutputParam)
	 *
	 * \brief	This function sets a RF output mode.<br>
	 *			It does not make any changes to the tester. Invoke SetTx() to actually apply changes to the tester.
	 *
	 * \param	frequencyChannelParam	RF output mode to set\n
	 *									<em>IQV_RF_OUTPUT_ON</em>	: Turn RF output on\n
	 *									<em>IQV_RF_OUTPUT_ON</em>	: Turn RF output off
	 */
	void SetParamRfOutput(IQV_GLONASS_RF_OUTPUT rfOutputParam);

	/*!
	 * \fn	void GetParamRfOutput(IQV_GLONASS_RF_OUTPUT *rfOutputParam)
	 *
	 * \brief	This function gets a RF output mode.<br>
	 *			It does not directly retrieve from a tester. Invoke GetTx() first to do so.
	 *
	 * \param	frequencyChannelParam	RF output mode\n
	 *									<em>IQV_RF_OUTPUT_ON</em>	: Turn RF output on\n
	 *									<em>IQV_RF_OUTPUT_ON</em>	: Turn RF output off
	 */
	void GetParamRfOutput(IQV_GLONASS_RF_OUTPUT *rfOutputParam);

	/*!
	 * \fn	void SetParamOperationMode(IQV_GLONASS_OPERATION_MODE operationModeParam)
	 *
	 * \brief	This function sets a operation/modulation mode.<br>
	 *			It does not make any changes to the tester. Invoke SetTx() to actually apply changes to the tester.
	 *
	 * \param	operationModeParam	Operation mode to set\n
	 *								<em>IQV_OPERATION_MODE_CW</em>						: Continuous Wave(CW) mode\n
	 *								<em>IQV_OPERATION_MODE_MODULATION</em>				: CA Code modulation is turned on\n
	 *								<em>IQV_OPERATION_MODE_MODULATION_NAV_DATA_OFF</em>	: CA Code modulation is turned on with navigation data off\n
	 *								<em>IQV_OPERATION_MODE_MODULATION_OFF</em>			: CA Code modulation is turned off
	 */
	void SetParamOperationMode(IQV_GLONASS_OPERATION_MODE operationModeParam);

	/*!
	 * \fn	void GetParamOperationMode(IQV_GLONASS_OPERATION_MODE *operationModeParam)
	 *
	 * \brief	This function gets a operation/modulation mode.<br>
	 *			It does not directly retrieve from a tester. Invoke GetTx() first to do so.
	 *
	 * \param	operationModeParam	Operation mode\n
	 *								<em>IQV_OPERATION_MODE_CW</em>						: Continuous Wave(CW) mode\n
	 *								<em>IQV_OPERATION_MODE_MODULATION</em>				: CA Code modulation is turned on\n
	 *								<em>IQV_OPERATION_MODE_MODULATION_NAV_DATA_OFF</em>	: CA Code modulation is turned on with navigation data off\n
	 *								<em>IQV_OPERATION_MODE_MODULATION_OFF</em>			: CA Code modulation is turned off
	 */
	void GetParamOperationMode(IQV_GLONASS_OPERATION_MODE *operationModeParam);

	//! @cond
	bool GetConGlonass();
	//! @endcond

public:
	int lastErrCode; //!< Last error code
	char lastErr[MAX_LEN_ERR_TXT]; //!< Last error message

//! @cond
private:
	iqapiHndlGlonass() {}
	int GetErrorDescription(uint32 errCode, char *errMsg, unsigned int bufSize);
	int GetErrorCodeAndDescription();
	int ExecSingleCommandAndCheck(char *command, char **singleCommandResult);
	
	void _reset_parameters();
	void _make_get_tx_commands(char *commands, int &count);
	int _check_license();
	int _load_navigation_data();
	int _parse_and_get_operation_mode(char *responseToken, char *allCommands = NULL);
	int _parse_and_get_rf_output(char *responseToken, char *allCommands = NULL);
	int _parse_and_get_power(char *responseToken, char *allCommands = NULL);
	int _parse_and_get_doppler_frequency(char *responseToken, char *allCommands = NULL);
	int _parse_and_get_signal_frequency(char *responseToken, char *allCommands = NULL);
	int _gps_deactivate();
	int _gps_is_activated(bool &retActivate);
	int _gps_get_power_and_doppler(double &powerDbmParam, int &dopplerDbmParam);

private:
	iqapiHndlBase *parentHndl;
	char *singleCommandResult;
	double _power_dbm;
	int _doppler_frequency;
	int _signal_frequency_index;
	IQV_GLONASS_OPERATION_MODE _operation_mode;
	IQV_GLONASS_RF_OUTPUT _rf_output;
	char _response[IQV_GLONASS_RESPONSE_SIZE];
	bool _con_glonass;
	bool _is_glonass_supported;
	char *_navigation_data;
//! @endcond
};

#endif			// IQAPIGLONASS_H_
