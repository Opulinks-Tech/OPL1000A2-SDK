#ifndef _IQAPIHNDLMATLAB_H_
#define _IQAPIHNDLMATLAB_H_

#include "iqapiHndlBase.h"



// ****************************************************************
// Modulation Wave Class
// ****************************************************************
//! Specifies the analysis parameters for modulation wave.
/*! If a call to the \c hndl->GenerateWave() function is successful, the \c hndl->wave parameter will point to an \c iqapiModulationWave object which is used in a call to the \c hndl->SetWave() object.
*/
class IQ_API iqapiModulationWave : public iqapiSignalData
{
public:	
   iqapiModulationWave(void) {}; //!< Constructor
   ~iqapiModulationWave(void){} ;	//!< Destructor
    
	
    //! A copy constructor that can be used to make a deep copy of an \c iqapiModulationWave object.
    /*! Use this constructor when you wish to create a copy of an \c iqapiModulationWave object.
    */
    iqapiModulationWave(const iqapiModulationWave &src);

    int	Save(char *fileName);		//!< Saves an \c iqapiModulationWave object to a \c .mod file(set by filename), which can be downloaded to a VSG via this API, or by the VSG panel in the IQsignal for MIMO application.    

    /*!< This function returns 0 (\c IQAPI_ERR_OK) if successful; if it returns a value that is less than 0, then it indicates a warning and if it returns a value that is greater than 0, then it indicates an error (see IQAPI_ERR_CODES, or the \c hndl->lastErr buffer). 
    */	

    IQV_CAPTURE_TYPE_ENUM GetCaptureType() { return IQV_WAVE_GENERATION_CAPTURE_TYPE; };

};


// ****************************************************************
// General matlab unpack function call
// ****************************************************************
int UnpackLoadFileFormat(void *mx_result_in, int *frameLen, int *zeroPrefixLen);

#endif			// _IQAPIHNDLMATLAB_H_
