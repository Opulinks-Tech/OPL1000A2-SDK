#ifndef _DUALHEADBASE_H_
#define _DUALHEADBASE_H_

//#include "iqapi.h"
#include <fstream>
#include "iqapiDefines.h" 
#include "iqapiHndlBase.h"

using namespace std;

#ifdef WIN32
#include <sys/timeb.h>
#else
#include <stdint.h>
#endif

namespace highrestimer
{
#ifdef WIN32
	typedef struct __timeb64 lp_time_t;
#else
	typedef uint64_t lp_time_t;
#endif
}

#define  MAX_LEN_MES1       512
class IQ_API DualHeadBase
{
public:
   DualHeadBase(iqapiHndlBase *hndl);
   ~DualHeadBase();

public:
   iqapiHndlBase  *m_phndl;	//!< Creates a new \c iqapi handle
   unsigned int   TokenID[2];	//!< Unique non-zero id (1 - 0xFFFFFFFF) used to distinguish between the two test programs.
   
   bool           bRelOK[2];        //!< Indicates if the token releases OK or not. 
   /*!< True=release Ok
   */    
   bool           bInitialized[2];	//!< Indicates if tokens have been initialized or not. Only one of the two available tokens can be initialized. 	
   bool           bState[2];	//!< Indicates if token release should be set to OK or not.
   int            iStatus[2];	//!< Indicates error code for the GetToken, SetToken and TokenTimeout functions. 
   bool           bDualHeadCurFreqInLowRange;	//!< Indicates if the test system setting is in low frequency range or not and prevents from switching to high band frequency. This function is reserved for internal use.

private:
   virtual bool            SetToken (unsigned int token, unsigned int value) = 0;    
   virtual unsigned int    GetToken (unsigned int token) = 0;     
   virtual void            DoTesterExtraSetting() {} ;

   void           TokenInit (void);
   bool           IsTokenValid (unsigned int token);
   bool           IsTokenExist (unsigned int token, int expectedTokenID);
   bool           IsTokenTimeout (highrestimer::lp_time_t time_start,
                               unsigned int     timeout);
   bool           IsTokenAvail (unsigned int token);



protected:
   unsigned int   diagnostics;
   std::ofstream  *ofsDiagnostics;
   char           diagnosticsMes[MAX_LEN_MES1];

public:


   bool        SetTokenID (unsigned int token, unsigned int tokenID=0); //!< Sets the token ID for test programs

   /*!< This method is used to setup the token ID with a particular token. 

\note Although the program allows you to set up more than one token, it currently supports the use of only one token. 

\n The value for this parameter is an enum value that is defined as follows: enum IQV_TOKEN_ENUM { IQV_TOKEN_NO_1, IQV_TOKEN_NO_2, IQV_TOKEN_CNT };

\c tokenID &mdash; This is a unique non-zero id, with a value that ranges  between 1 and 0xFFFFFFFF, and is used to distinguish between two test programs. Setting this parameter is optional. 

The program allows you to set up the two token IDs manually or automatically.

Setting the token ID manually &mdash; You can set the token ID value manually by assigning two different values that range between 1 and 0xFFFFFFFF for the tokens assigned to each of the two test programs.

\n Example: Set program in the first PC as follows: \c SetToken(IQV_TOKEN_NO_1, 0x1); set program in the second PC as follows: \c SetToken(IQV_TOKEN_NO_1, 0x2); 

Setting the token ID automatically &mdash; If you do not pass the token number parameter, the function automatically generates the token IDs.

\n Example: Set program in the first PC as follows: \c SetToken(IQV_TOKEN_NO_1); set program in the second PC as follows: \c SetToken(IQV_TOKEN_NO_1); 

\note The chances of these two programs generating the same token ID is very slim.
*/


   bool        TokenRetrieveTimeout (unsigned int   token,
                                     unsigned int   probe_time,
                                     unsigned int   timeout = 0); //!< Acquires the token during every probe

/*!< This method is used to acquire the token during every probe. At each probe, it waits either until the token is acquired or until the time-out is reached.
\n
\n If successful, it returns a value of \c TRUE and allows you to start accessing the test system until you release the token. If it fails, it returns a value of \c FALSE; it will then wait for another probe time and will try until it can get the token or until it reaches a timeout.

\note This method is used to retrieve a token from the test system when it becomes available. When the token is retrieved, it also retrieves the test system status into the program and therefore synchronizes the test system with the control software. Therefore, when the token is retrieved, the program and the test system are maintained in the same state.

\n \c token &mdash; One of two supported tokens, i.e., the enum IQV_TOKEN_ENUM value
\n
\n \c probe_time &mdash; A time indicating how often the program shall attempt to retrieve the token. This is specified in milliseconds.
\n
\n \c timeout &mdash; A time indicating how long the program takes to attempt the retrieval of the token. This is specified in milliseconds. The default value is 0. However, if this value is set to the default value of 0, then the function waits until a token is retrieved; therefore, during this timeout period, the code is blocked and cannot run until it retrieves the token. 
*/

   bool        TokenRelease (unsigned int   token); //!< Releases a token
   
/*!< This method is used to release the token. It is intended to be executed after the program has completed the required testing with the test system. 

The function takes one input, \c token, which is the token that was set using \c SetTokenID. 

\n If the token has a valid value and was released without encountering any errors, it returns a value of TRUE; if the value is not valid, it returns a value of FALSE.
*/
    bool        TokenReleaseProblem (unsigned int token); //!< Tests if a problem has occurred while releasing the token
   
/*!< This method is used to test if a problem has occurred while releasing the token. The method takes one input parameter, \c token mdash; the token that was released. If a problem had occurred, it returns a value of TRUE; if no problem had occurred, it returns a value of FALSE. 

\note If a problem occurs, it is recommended that you retest both devices that are being tested.
*/     

   void        TokenReset();    //!< Resets the token handling for the test system.
/*!< This function resets the token handling for the test system. This function can be used to get the token out of the \a hang state if an application having the token crashes or stops responding.
*/    
   
};

#endif			// end of _DUALHEADBASE_H_
