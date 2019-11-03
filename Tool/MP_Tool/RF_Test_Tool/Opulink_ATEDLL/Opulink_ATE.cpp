//---------------------------------------------------------------------------

#include <vcl.h>
#include <windows.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//   Important note about DLL memory management when your DLL uses the
//   static version of the RunTime Library:
//
//   If your DLL exports any functions that pass String objects (or structs/
//   classes containing nested Strings) as parameter or function results,
//   you will need to add the library MEMMGR.LIB to both the DLL project and
//   any other projects that use the DLL.  You will also need to use MEMMGR.LIB
//   if any other projects which use the DLL will be performing new or delete
//   operations on any non-TObject-derived classes which are exported from the
//   DLL. Adding MEMMGR.LIB to your project will change the DLL and its calling
//   EXE's to use the BORLNDMM.DLL as their memory manager.  In these cases,
//   the file BORLNDMM.DLL should be deployed along with your DLL.
//
//   To avoid using BORLNDMM.DLL, pass string information using "char *" or
//   ShortString parameters.
//
//   If your DLL uses the dynamic version of the RTL, you do not need to
//   explicitly add MEMMGR.LIB as this will be done implicitly for you
//---------------------------------------------------------------------------

#include <vcl.h>
#include <iostream.h>
#include <conio.h>
#include <vcl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <map>
//#include "UART.h"

#include "IQmeasure.h"
#include "utility.h"
//#include "IQ2010Ext.h"

using namespace std;
#pragma hdrstop

#define MAX_BUFFER_SIZE			8192
#define IQ_TEST_TOKEN_TIMEOUT		10000	//ms
//#define GPS_MAX_SV_NO                   6
//#define LOAD_INTERNAL_WAVEFORM		0
#define FAIL   0
#define OK     1

//---------------------------------------------------------------------------
//#pragma package(smart_init)
//#pragma link "PERFGRAP"
//#pragma resource "*.dfm"

//---------------------------------------------------------------------------
// Function Declaration
extern "C" __declspec(dllexport) int  __stdcall ATE_Init(void);
extern "C" __declspec(dllexport) int  __stdcall ATE_IQ_Connect(void);
extern "C" __declspec(dllexport) int  __stdcall ATE_IQ_Init(void);
extern "C" __declspec(dllexport) int  __stdcall ATE_Test_WIFI_HP_Tx(int channel, double path_loss);
extern "C" __declspec(dllexport) int  __stdcall ATE_Test_WIFI_LP_Tx(int channel, double path_loss);
extern "C" __declspec(dllexport) int  __stdcall ATE_Test_WIFI_Rx(int channel, int power, char *pattern, int count, double path_loss);
extern "C" __declspec(dllexport) int  __stdcall ATE_Test_BLE_Tx(int channel, int logType, double path_loss);
extern "C" __declspec(dllexport) int  __stdcall ATE_Test_BLE_Rx(int channel, int power, char *pattern, int count, double path_loss);

//---------------------------------------------------------------------------
// Global Variables
int             errStatus;
int             errStatusDualHead;
int             typeIQTester;
int             chan, testCount;
double          samplingRate;
double          vio;


#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
        return 1;
}

//---------------------------------------------------------------------------
int __stdcall ATE_Init(void)
{
        typeIQTester = IQTYPE_2010;   //IQTYPE_XEL

        errStatus = LP_Init(typeIQTester);

        if (ERR_OK != errStatus)
        {
                printf("Init LitePoint library failed..\n");
        }
        else
                printf("Init LitePoint library ok..\n");


        if (!DirectoryExists(".\\mod"))
        {
                if (!CreateDir(".\\mod"))
                        throw Exception("Cannot create \\mod directory.");
        }

        if (!DirectoryExists(".\\report"))
        {
                if (!CreateDir(".\\report"))
                        throw Exception("Cannot create \\report directory.");
        }
        return OK;
}

//---------------------------------------------------------------------------
int __stdcall ATE_IQ_Connect(void)
{
        char    buffer[2048];
	int     tokenID;
	int     IQXEL160_DH_ROUT;
        int     rbIQxel160;
        char    *ptr;

        const char* ip;
        std::string s = "127.0.0.1";
        ip = s.c_str();

        tokenID = 1;
        IQXEL160_DH_ROUT = 1;
        rbIQxel160 = 0;
        typeIQTester = IQTYPE_2010;   //IQTYPE_XEL

        if (typeIQTester == IQTYPE_2010)
	{
                samplingRate = 80e6;
	}
	else
	{
                samplingRate = 160e6;
	}
        defineSpectrumMaskLimit();

        if (rbIQxel160)
                errStatusDualHead = LP_IQXEL160_DualHead_ConOpen(
						IQXEL160_DH_ROUT,	//IQxelConnectionType
						tokenID,		//tokenID
						"127.0.0.1"             //*ipAddress1
						);
        else
                errStatusDualHead = LP_DualHead_ConOpen(
						tokenID,		//tokenID
						"127.0.0.1",          	//*ipAddress1
						NULL,			//*ipAddress2
						NULL,			//*ipAddress3
						NULL			//*ipAddress4
						);
        if (ERR_OK != errStatusDualHead)
        {
                printf("The system could not connect to the tester. Check the IP address, etc.\n");
                return FAIL;
        }
        else
        {
                printf("Connect to IQ done..\n");
                        /*
                        LP_GetVersion(buffer, 2048);
		        ptr = strtok(buffer, "\n");
		        while (ptr != NULL)
                        {
                                printf("%s", ptr);
                                ptr = strtok(NULL, "\n");
                        }
                        */
        }

        return OK;
}

//---------------------------------------------------------------------------
int __stdcall ATE_IQ_Init(void)
{
	int num;
	bool bGetCtrl;

        printf("Try to get tester token...\n");
        errStatusDualHead = LP_DualHead_ObtainControl();
      	if (ERR_OK == errStatusDualHead)
                printf("Obtain IQ Control Done...\n");
      	else if (ERR_TOKEN_RETRIEVAL_TIMEDOUT == errStatusDualHead)
        {
                printf("Err: Token retrieval timeout...\n");
                return FAIL;
        }
      	else if (ERR_NOT_INITIALIZED == errStatusDualHead)
        {
                printf("Err: Not initialized.Retry...\n");

                errStatusDualHead = LP_DualHead_ObtainControl(
      			500,					//probeTimeMS
        		IQ_TEST_TOKEN_TIMEOUT	                //timeOutMS
	        	);
                if (ERR_OK != errStatusDualHead)
                {
                        printf("Err: Still Error...\n");
                        return FAIL;
                }
        }
      	else
        {
                printf("Err: Unknown error...\n");
                return FAIL;
        }

//	DateTime = Now();
	errStatus = LP_SetDefault();
        return OK;
}

//---------------------------------------------------------------------------
int SetFrequency(int channel)
{
        int freq;

        if (channel <= 13)
        {
                freq=2412+(channel-1)*5;
        }
        else
        if (channel == 14)
                freq = 2484;
        else
        {
                printf("Err: Frequncy set error, set to default 2412\n");
                freq = 2412;
                channel = 1;
        }
//        printf("Channel:%d, Freq:%ld\n",channel, freq);
        return freq;
}

//---------------------------------------------------------------------------
int SetBLEFrequency(int channel)
{
        int freq;

        freq=2402+(channel)*2;
        printf("Channel:%d, Freq:%ld\n",channel, freq);
        return freq;
/*
        if (channel >= 0 && channel <= 10)
        {
                freq=2404+(channel)*2;
        }
        else
        if (channel >= 11 && channel <= 36)
        {
                freq=2428+(channel-11)*2;
        }
        else
        if(channel == 37)
                freq = 2402;
        else
        if(channel == 38)
                freq = 2426;
        else
        if(channel == 39)
                freq = 2480;
        else
        {
                printf("Err: Frequncy set error, set to default 2412\n");
                freq = 2402;
                channel = 37;
        }
        printf("Channel:%d, Freq:%ld\n",channel, freq);
        return freq;
*/
}

//---------------------------------------------------------------------------
int __stdcall ATE_Test_WIFI_HP_Tx(int channel, double path_loss)
{
	char cTemp[512] = {0};
	char *message;
        FILE * FOpen;
        int freq;

        freq = SetFrequency(channel);
        printf("Freq:%d, Channel:%d\n", freq, channel);

        if(!(FOpen = fopen(".\\report\\Opulink_ATE_WIFI_tx_results.csv","r")))
        {
                // If file not exist, create file and add header
                FOpen =fopen(".\\report\\Opulink_ATE_WIFI_tx_results.csv","w");
                //fprintf(FOpen,"Channel,Data Rate (Mbps),") ;
                //fprintf(FOpen,"Data Rate (Mbps),") ;
                //fprintf(FOpen,"PeakPWR(dBm),RMSMaxAvgPWR(dBm),EVM(pk)(dB),EVM(all)(db),Freq Err(Hz),CLK Err(ppm),IQampErr(dB),IQphaseErr(Deg),RMSphaseErr(Deg)\n") ;
                //printf("Create file...\n");
        }
        else
        {
                // If file exist, append data
                fclose(FOpen);
                FOpen =fopen(".\\report\\Opulink_ATE_WIFI_tx_results.csv","w");
                //printf("Open file...\n");
        }

        printf("Start DUT Tx Testing: Channel:%d...\n", freq);

	if (ERR_OK != (errStatus = LP_SetVsa(
                                                freq*1e6,                       //double rfFreqHz
		                                12 + 3.0 - path_loss,       //double rfAmplDb = target pwr + PeakToAvg - cable loss
			    	                PORT_RIGHT,			//PORT_OFF, PORT_LEFT, PORT_RIGHT or PORT_BB
				                0,				//double extAttenDb
				                -25,                            //double triggerLevelDb
                                                5e-6                            //triggerPreTime
        )))
	{
		strcpy(message,LP_GetErrorString(errStatus));
                printf("Err: LP_SetVsa error...\n");
		return FAIL;
	}

	if (ERR_OK != (errStatus = LP_VsaDataCapture(
                                                30000e-6,                        //double samplingTimeSecs
	                                        IQV_TRIG_TYPE_FREE_RUN,         // 1 = IQV_TRIG_TYPE_FREE_RUN
                                                                                // 6 = IQV_TRIG_TYPE_IF2_NO_CAL
                                                samplingRate,	                // double sampleFreqHz
                                                IQV_NORMAL_CAPTURE_TYPE	        // int ht40Mode
        )))
	{
		strcpy(message,LP_GetErrorString(errStatus));
                printf("Err: LP_VsaDataCapture error...\n");
	}


	// Save the signal file
    	errStatus = (typeIQTester == IQTYPE_2010?
    				LP_SaveVsaSignalFile("mod\\11b_save.sig"):
    				LP_SaveVsaSignalFile("mod\\11b_save.iqvsa"));
    	if (ERR_OK != errStatus)
    	{
    		strcpy(message,LP_GetErrorString(errStatus));
                printf("Err: LP_SaveVsaSignalFile error...\n");
    		return FAIL;
    	}

        if (ERR_OK != (errStatus = LP_Analyze80211b(
                                                7,      //int eq_taps
							//IQV_EQ_OFF    = 1,
                                                        //IQV_EQ_5_TAPS = 5,
                                                        // IQV_EQ_7_TAPS = 7,
                                                        // IQV_EQ_9_TAPS = 9
                                                0,      //int DCremove11b_flag
                                                        //IQV_DC_REMOVAL_OFF = 0
                                                        //IQV_DC_REMOVAL_ON  = 1
                                                1	//int method_11b:
                                                        //IQV_11B_STANDARD_TX_ACC  = 1,
                                                        //IQV_11B_RMS_ERROR_VECTOR = 2
                                                )))
	{
		strcpy(message,LP_GetErrorString(errStatus));
                printf("Err: LP_Analyze80211b error...\n");
		return FAIL;
	}

//       if (ERR_OK != (errStatus = LP_AnalyzeTechnologyFFT(
//                                               3      //int technology
							//11ac    = 0,
                                                       //11n = 1,
                                                       //11ag = 2,
                                                       // 11b = 3
//                                               )))
//	{
//		strcpy(message,LP_GetErrorString(errStatus));
//               printf("Err: LP_AnalyzeTechnologyFFT error...\n");
//		return FAIL;
//	}






       
      	//Form1->lblElapsedTime->Text = FormatDateTime("hh:mm:ss:zzz", Now() - DateTime);
        //fprintf(FOpen, "%d,",channel);
	fprintf(FOpen, " %1.1f,"        ,LP_GetScalarMeasurement("bitRateInMHz",0));
	fprintf(FOpen, " %6.3f,"        ,LP_GetScalarMeasurement("pkPower",0)+path_loss);
	fprintf(FOpen, " %6.3f,"        ,LP_GetScalarMeasurement("rmsMaxAvgPower",0)+path_loss);
	fprintf(FOpen, " %6.3f,"        ,LP_GetScalarMeasurement("evmPk",0));
	fprintf(FOpen, " %6.3f,"        ,LP_GetScalarMeasurement("evmAll",0));
	fprintf(FOpen, " %12.3f,"       ,LP_GetScalarMeasurement("freqErr",0));
	fprintf(FOpen, " %12.3f,"       ,LP_GetScalarMeasurement("clockErr",0));
	fprintf(FOpen, " %6.3f,"        ,LP_GetScalarMeasurement("ampErrDb",0));
        fprintf(FOpen, " %6.3f,"        ,LP_GetScalarMeasurement("phaseErr",0));
        fprintf(FOpen, " %6.3f,"       ,LP_GetScalarMeasurement("rmsPhaseNoise",0));
//        fprintf(FOpen, " %6.3f\n"       ,LP_GetScalarMeasurement("spectrumCheck",0));

//        fclose(FOpen);
/*
	sprintf(cTemp, "Channel => %.0d\n", channel);
        printf("%s", cTemp);

	sprintf(cTemp, "DataRate => %.0f Mbps\n", LP_GetScalarMeasurement("bitRateInMHz",0));
        printf("%s", cTemp);

	sprintf(cTemp, "pkPower => %.3f dB\n", LP_GetScalarMeasurement("pkPower",0));
        printf("%s", cTemp);

	sprintf(cTemp, "rmsPower => %.3f dB\n", LP_GetScalarMeasurement("rmsMaxAvgPower",0));
        printf("%s", cTemp);

        sprintf(cTemp, "evmPk => %.3f dB\n", LP_GetScalarMeasurement("evmPk",0));
        printf("%s", cTemp);

	sprintf(cTemp, "evmAll => %.3f dB\n", LP_GetScalarMeasurement("evmAll",0));
        printf("%s", cTemp);

	sprintf(cTemp, "freqErr => %.3f Hz\n", LP_GetScalarMeasurement("freqErr",0));
        printf("%s", cTemp);

	sprintf(cTemp, "clockErr => %.3f ppm\n", LP_GetScalarMeasurement("clockErr",0));
        printf("%s", cTemp);

	sprintf(cTemp, "ampErrDb => %.3f dB\n", LP_GetScalarMeasurement("ampErrDb",0));
        printf("%s", cTemp);

      	sprintf(cTemp, "IQPhaseErr => %.3f Deg\n", LP_GetScalarMeasurement("phaseErr",0));
        printf("%s", cTemp);

	sprintf(cTemp, "rmsPhaseNoise => %.3f Deg\n", LP_GetScalarMeasurement("rmsPhaseNoise",0));
        printf("%s", cTemp);
*/
//        return OK;
//---------------------------------------------------------------------------
        errStatus = ::LP_SetAnalysisParameterInteger("AnalyzeFFT", "vsaNum", 1);
	errStatus = ::LP_AnalyzeFFT();

	// Store the result
	double *bufferRealX = NULL, *bufferImagX = NULL, *bufferRealY=NULL, *bufferImagY=NULL;
	bufferRealX = (double *)malloc((MAX_BUFFER_SIZE*8)*sizeof(double));
	bufferImagX = (double *)malloc((MAX_BUFFER_SIZE*8)*sizeof(double));
	bufferRealY = (double *)malloc((MAX_BUFFER_SIZE*8)*sizeof(double));
	bufferImagY = (double *)malloc((MAX_BUFFER_SIZE*8)*sizeof(double));

	int    bufferSizeX = ::LP_GetVectorMeasurement("x", bufferRealX, bufferImagX, MAX_BUFFER_SIZE*8);
	int    bufferSizeY = ::LP_GetVectorMeasurement("y", bufferRealY, bufferImagY, MAX_BUFFER_SIZE*8);

	if ( (0>=bufferSizeX)||(0>=bufferSizeY) )
	{
		errStatus = -1;

		if (bufferRealX) free(bufferRealX);
		if (bufferImagX) free(bufferImagX);
		if (bufferRealY) free(bufferRealY);
		if (bufferImagY) free(bufferImagY);
	}

	double *maskWiFi = (double *)malloc(bufferSizeY*sizeof(double));

	int maskResult = testSpectrumMask(bufferRealX, bufferRealY, bufferSizeY, WIFI_11B, maskWiFi, &vio);

       	sprintf(cTemp, "%s:\t%.3f %%, status:%d\n", "Spectrum violation", vio, maskResult);
        printf("%s", cTemp);

    //    strcat(message, cTemp);

 	double obwPercentage = 0.99;
	int startIndex = 0, stopIndex = 0;
	double obw;

	obw = GetOBW(bufferRealX, bufferRealY, bufferSizeY, obwPercentage, startIndex, stopIndex);

	sprintf(cTemp, "%s:\t%.3f MHz\n", "OBW", obw);
        printf("%s", cTemp);
     //   strcat(message, cTemp);

//	Form1->Series1->Clear();
//	Form1->Series2->Clear();

//	Form1->Chart1->Title->Text->Strings[0] = "PSD Plot";
//	for (i = 0; i < bufferSizeY; i++)
//	{
//		Form1->Series1->AddXY(bufferRealX[i]*1e-6, bufferRealY[i]);
//		Form1->Series2->AddXY(bufferRealX[i]*1e-6, maskWiFi[i]);
//	}
        fprintf(FOpen, " %d\n", maskResult);
	// free memory
 	if (bufferRealX) free(bufferRealX);
	if (bufferImagX) free(bufferImagX);
	if (bufferRealY) free(bufferRealY);
	if (bufferImagY) free(bufferImagY);
	if (maskWiFi) free(maskWiFi);

    fclose(FOpen);
    return OK;
}


int __stdcall ATE_Test_WIFI_Rx(int channel, int power, char *pattern, int count, double path_loss)
{
	char *message;
        FILE * FOpen;
        int freq;
printf("Start setting VSG for WiFi...\n");
        if (ERR_OK != (errStatus = LP_EnableVsgRF(1)))
	{
		strcpy(message,LP_GetErrorString(errStatus));
		return FAIL;
	}
	freq = SetFrequency(channel);

	if (ERR_OK != (errStatus = LP_SetVsg(	freq*1e6,    //double rfFreqHz
                                                power+path_loss,      //double rfPowerLeveldBm
						PORT_RIGHT  //int port
			   )))
        {
		strcpy(message,LP_GetErrorString(errStatus));
		return FAIL;
	}

	if (ERR_OK != (errStatus = LP_SetVsgModulation(pattern)))
	//if (ERR_OK != (errStatus = LP_SetVsgModulation("mod\\wave8.mod")))
	{
		strcpy(message,LP_GetErrorString(errStatus));
		return FAIL;
	}



	// First dummy frame to reset IQview
	//if (ERR_OK != (errStatus = LP_SetFrameCnt(1)))
	//{
	//	strcpy(message,LP_GetErrorString(errStatus));
	//	return FAIL;
	//}


	//Reset DUT Rx Statistic Data here

	// Start transmit frames
	if (ERR_OK != (errStatus = LP_SetFrameCnt(count)))
	{
		strcpy(message,LP_GetErrorString(errStatus));
		return FAIL;
	}

        while (ERR_OK != LP_TxDone())
	{
                printf("Transmit done...\n");
	        Sleep(1);
	}

//        if (ERR_OK != (errStatus = LP_EnableVsgRF(0)))
//	{
//		strcpy(message,LP_GetErrorString(errStatus));
//		return FAIL;
//	}

	//Get Rx Statistic Data here

 }


//---------------------------------------------------------------------------
int __stdcall ATE_Test_BLE_Tx(int channel, int logType, double path_loss)
{
	char cTemp[512] = {0};
	char *message;
        FILE * FOpen;
        int freq;

        freq = SetBLEFrequency(channel);

        if(!(FOpen = fopen(".\\report\\Opulink_ATE_BLE_tx_results.csv","r")))
        {
                // If file not exist, create file and add header
                FOpen =fopen(".\\report\\Opulink_ATE_BLE_tx_results.csv","w");
        }
        else
        {
                // If file exist, append data
                fclose(FOpen);
                FOpen =fopen(".\\report\\Opulink_ATE_BLE_tx_results.csv","w");
        }

        printf("Start DUT Tx Testing: Channel:%d...\n", freq);

	if (ERR_OK != (errStatus = LP_SetVsaBluetooth(
                                                freq*1e6,                       //double rfFreqHz
		                                8 + 3.0 - path_loss,                  //double rfAmplDb = target pwr + PeakToAvg - cable loss
			    	                PORT_RIGHT,			//PORT_OFF, PORT_LEFT, PORT_RIGHT or PORT_BB
				                -25,                            //double triggerLevelDb
                                                10e-6                           //double triggerPreTime
        )))
	{
		strcpy(message,LP_GetErrorString(errStatus));
                printf("Err: LP_SetVsaBluetooth error...\n");
		return FAIL;
	}

	if (ERR_OK != (errStatus = LP_VsaDataCapture(
                                                1000e-6,                        //double samplingTimeSecs
	                                        IQV_TRIG_TYPE_FREE_RUN,         // 1 = IQV_TRIG_TYPE_FREE_RUN
                                                                                // 6 = IQV_TRIG_TYPE_IF2_NO_CAL
                                                samplingRate,	                // double sampleFreqHz
                                                IQV_NORMAL_CAPTURE_TYPE	        // int ht40Mode
        )))
	{
		strcpy(message,LP_GetErrorString(errStatus));
                printf("Err: LP_VsaDataCapture error...\n");
	}


	// Save the signal file
    	errStatus = (typeIQTester == IQTYPE_2010?
    				LP_SaveVsaSignalFile("mod\\ble_save.sig"):
    				LP_SaveVsaSignalFile("mod\\ble_save.iqvsa"));
    	if (ERR_OK != errStatus)
    	{
    		strcpy(message,LP_GetErrorString(errStatus));
                printf("Err: LP_SaveVsaSignalFile error...\n");
    		return FAIL;
    	}


        //if (ERR_OK != (errStatus = IQ2010EXT_AnalyzeBluetooth(
        if (ERR_OK != (errStatus = LP_AnalyzeBluetooth(
                                                4,      //*         - 1: 1Mbit/s BDR
                                                        //*         - 2: 2Mbit/s EDR
                                                        //*         - 3: 3Mbit/s EDR
                                                        //*         - 0: Auto detect
                                                "All"
                                                )))
	{
		strcpy(message,LP_GetErrorString(errStatus));
                printf("Err: LP_AnalyzeBluetooth error...\n");
		return FAIL;
	}
      	//Form1->lblElapsedTime->Text = FormatDateTime("hh:mm:ss:zzz", Now() - DateTime);


        if(logType == 0)
        {
        }
        else if(logType ==1)
        {
                //fprintf(FOpen,"Freq,Peak Power,Freq Offset, DeltaF1Avg, PDU Length, CRC OK\n") ;
                //fprintf(FOpen, "%d,",freq);
                fprintf(FOpen, " %6.3f,"        ,LP_GetScalarMeasurement("P_pk_each_burst_dBm",0)+path_loss);
                fprintf(FOpen, " %6.3f,"        ,LP_GetScalarMeasurement("lePduLength",0));
                fprintf(FOpen, " %6.3f,"        ,LP_GetScalarMeasurement("leFreqOffset",0)/1000);
                fprintf(FOpen, " %6.3f,"        ,LP_GetScalarMeasurement("leDeltaF1Avg",0)/1000);
                fprintf(FOpen, " %6.3f,"        ,LP_GetScalarMeasurement("leIsCrcOk",0));
                //fprintf(FOpen, "\n");
                sprintf(cTemp, "P_pk_each_burst_dBm => %.3f \n", LP_GetScalarMeasurement("P_pk_each_burst_dBm",0)+path_loss);
                printf("%s", cTemp);
	        sprintf(cTemp, "leFreqOffset => %.3f \n", LP_GetScalarMeasurement("leFreqOffset",0));
                printf("%s", cTemp);
	        sprintf(cTemp, "leDeltaF1Avg => %.3f \n", LP_GetScalarMeasurement("leDeltaF1Avg",0));
                printf("%s", cTemp);
      	        sprintf(cTemp, "lePduLength => %.3f \n", LP_GetScalarMeasurement("lePduLength",0));
                printf("%s", cTemp);
              	sprintf(cTemp, "leIsCrcOk => %.3f \n", LP_GetScalarMeasurement("leIsCrcOk",0));
                printf("%s", cTemp);
        }
        else if(logType ==2)
        {
                //fprintf(FOpen,"DeltaF2Max, DeltaF2Avg, CRC OK\n") ;
                fprintf(FOpen, " %6.3f,"        ,LP_GetScalarMeasurement("leDeltaF2Max",0)/1000);
                fprintf(FOpen, " %6.3f,"        ,LP_GetScalarMeasurement("leDeltaF2Avg",0)/1000);
                fprintf(FOpen, " %6.3f,"        ,LP_GetScalarMeasurement("leFn",0)/1000);
                fprintf(FOpen, " %6.3f,"        ,LP_GetScalarMeasurement("leFnMax",0)/1000);
                fprintf(FOpen, " %6.3f,"        ,LP_GetScalarMeasurement("leDeltaF0FnMax",0)/1000);
                fprintf(FOpen, " %6.3f,"        ,LP_GetScalarMeasurement("leDeltaF1F0",0)/1000);
                fprintf(FOpen, " %6.3f,"        ,LP_GetScalarMeasurement("leDeltaFnFn_5Max",0)/1000);
                fprintf(FOpen, " %6.3f,"        ,LP_GetScalarMeasurement("leDeltaF2MaxTotalCount",0));
                fprintf(FOpen, " %6.3f,"        ,LP_GetScalarMeasurement("leIsCrcOk",0));
                fprintf(FOpen, "\n");

	        sprintf(cTemp, "leDeltaF2Max => %.3f \n", LP_GetScalarMeasurement("leDeltaF2Max",0));
                printf("%s", cTemp);
	        sprintf(cTemp, "leDeltaF2Avg => %.3f \n", LP_GetScalarMeasurement("leDeltaF2Avg",0));
                printf("%s", cTemp);
	        sprintf(cTemp, "leIsCrcOk => %.3f \n", LP_GetScalarMeasurement("leIsCrcOk",0));
                printf("%s", cTemp);
        }
        else
        {
	        sprintf(cTemp, "leValid => %.3f \n", LP_GetScalarMeasurement("leValid",0));
                printf("%s", cTemp);
        }
        fclose(FOpen);
/*
	sprintf(cTemp, "Channel => %.0d\n", channel);
        printf("%s", cTemp);

	sprintf(cTemp, "dataRateDetect => %6.3f \n", LP_GetScalarMeasurement("dataRateDetect",0));
        printf("%s", cTemp);
	sprintf(cTemp, "freq_deviation => %.3f \n", LP_GetScalarMeasurement("freq_deviation",0));
        printf("%s", cTemp);
	sprintf(cTemp, "freq_est => %.3f \n", LP_GetScalarMeasurement("freq_est",0));
        printf("%s", cTemp);
	sprintf(cTemp, "P_av_each_burst_dBm => %6.3f \n", LP_GetScalarMeasurement("P_av_each_burst_dBm",0));
        printf("%s", cTemp);
	sprintf(cTemp, "P_av_all_dBm => %.3f \n", LP_GetScalarMeasurement("P_av_all_dBm",0));
        printf("%s", cTemp);
	sprintf(cTemp, "P_peak_all_dBm => %.3f \n", LP_GetScalarMeasurement("P_peak_all_dBm",0));
        printf("%s", cTemp);

	sprintf(cTemp, "leDeltaF2Max => %.3f \n", LP_GetScalarMeasurement("leDeltaF2Max",0));
        printf("%s", cTemp);
      	sprintf(cTemp, "leDeltaF2MaxMinValue => %.3f \n", LP_GetScalarMeasurement("leDeltaF2MaxMinValue",0));
        printf("%s", cTemp);
	sprintf(cTemp, "leDeltaF2Avg => %.3f \n", LP_GetScalarMeasurement("leDeltaF2Avg",0));
        printf("%s", cTemp);
	sprintf(cTemp, "leBelow185F2Max => %.3f \n", LP_GetScalarMeasurement("leBelow185F2Max",0));
        printf("%s", cTemp);
      	sprintf(cTemp, "leValid => %.3f \n", LP_GetScalarMeasurement("leValid",0));
        printf("%s", cTemp);
       	sprintf(cTemp, "leMaxPowerDbm => %.3f \n", LP_GetScalarMeasurement("leMaxPowerDbm",0));
        printf("%s", cTemp);
*/
        return OK;
}


int __stdcall ATE_Test_BLE_Rx(int channel, int power, char *pattern, int count, double path_loss)
{
	char *message;
        FILE * FOpen;
        int freq;
printf("Start setting VSG for BLE...\n");
        if (ERR_OK != (errStatus = LP_EnableVsgRF(1)))
	{
		strcpy(message,LP_GetErrorString(errStatus));
		return FAIL;
	}
	freq = SetBLEFrequency(channel);

	if (ERR_OK != (errStatus = LP_SetVsg(	freq*1e6,    //double rfFreqHz
                                                power+path_loss,      //double rfPowerLeveldBm
						PORT_RIGHT  //int port
			   )))
        {
		strcpy(message,LP_GetErrorString(errStatus));
		return FAIL;
	}

	if (ERR_OK != (errStatus = LP_SetVsgModulation(pattern)))
	//if (ERR_OK != (errStatus = LP_SetVsgModulation("mod\\wave8.mod")))
	{
		strcpy(message,LP_GetErrorString(errStatus));
		return FAIL;
	}



	// First dummy frame to reset IQview
	//if (ERR_OK != (errStatus = LP_SetFrameCnt(1)))
	//{
	//	strcpy(message,LP_GetErrorString(errStatus));
	//	return FAIL;
	//}


	//Reset DUT Rx Statistic Data here

	// Start transmit frames
	if (ERR_OK != (errStatus = LP_SetFrameCnt(count)))
	{
		strcpy(message,LP_GetErrorString(errStatus));
		return FAIL;
	}

        while (ERR_OK != LP_TxDone())
	{
                printf("Transmit done...\n");
	        Sleep(1);
	}

	//Get Rx Statistic Data here

 }


 //---------------------------------------------------------------------------
int __stdcall ATE_Test_WIFI_LP_Tx(int channel)
{
	char cTemp[512] = {0};
	char *message;
        FILE * FOpen;
        int freq;

        freq = SetFrequency(channel);
        printf("Freq:%d, Channel:%d\n", freq, channel);

        if(!(FOpen = fopen(".\\report\\Opulink_ATE_WIFI_tx_results.csv","r")))
        {
                // If file not exist, create file and add header
                FOpen =fopen(".\\report\\Opulink_ATE_WIFI_tx_results.csv","w");
                //fprintf(FOpen,"Channel,Data Rate (Mbps),") ;
                //fprintf(FOpen,"Data Rate (Mbps),") ;
                //fprintf(FOpen,"PeakPWR(dBm),RMSMaxAvgPWR(dBm),EVM(pk)(dB),EVM(all)(db),Freq Err(Hz),CLK Err(ppm),IQampErr(dB),IQphaseErr(Deg),RMSphaseErr(Deg)\n") ;
                //printf("Create file...\n");
        }
        else
        {
                // If file exist, append data
                fclose(FOpen);
                FOpen =fopen(".\\report\\Opulink_ATE_WIFI_tx_results.csv","w");
                //printf("Open file...\n");
        }

        printf("Start DUT Tx Testing: Channel:%d...\n", freq);

	if (ERR_OK != (errStatus = LP_SetVsa(
                                                freq*1e6,                       //double rfFreqHz
		                                0 + 3.0 - 1,       //double rfAmplDb = target pwr + PeakToAvg - cable loss
			    	                PORT_RIGHT,			//PORT_OFF, PORT_LEFT, PORT_RIGHT or PORT_BB
				                0,				//double extAttenDb
				                -25,                            //double triggerLevelDb
                                                5e-6                            //triggerPreTime
        )))
	{
		strcpy(message,LP_GetErrorString(errStatus));
                printf("Err: LP_SetVsa error...\n");
		return FAIL;
	}

	if (ERR_OK != (errStatus = LP_VsaDataCapture(
                                                30000e-6,                        //double samplingTimeSecs
	                                        IQV_TRIG_TYPE_FREE_RUN,         // 1 = IQV_TRIG_TYPE_FREE_RUN
                                                                                // 6 = IQV_TRIG_TYPE_IF2_NO_CAL
                                                samplingRate,	                // double sampleFreqHz
                                                IQV_NORMAL_CAPTURE_TYPE	        // int ht40Mode
        )))
	{
		strcpy(message,LP_GetErrorString(errStatus));
                printf("Err: LP_VsaDataCapture error...\n");
	}


	// Save the signal file
    	errStatus = (typeIQTester == IQTYPE_2010?
    				LP_SaveVsaSignalFile("mod\\11b_save.sig"):
    				LP_SaveVsaSignalFile("mod\\11b_save.iqvsa"));
    	if (ERR_OK != errStatus)
    	{
    		strcpy(message,LP_GetErrorString(errStatus));
                printf("Err: LP_SaveVsaSignalFile error...\n");
    		return FAIL;
    	}

        if (ERR_OK != (errStatus = LP_Analyze80211b(
                                                7,      //int eq_taps
							//IQV_EQ_OFF    = 1,
                                                        //IQV_EQ_5_TAPS = 5,
                                                        // IQV_EQ_7_TAPS = 7,
                                                        // IQV_EQ_9_TAPS = 9
                                                0,      //int DCremove11b_flag
                                                        //IQV_DC_REMOVAL_OFF = 0
                                                        //IQV_DC_REMOVAL_ON  = 1
                                                1	//int method_11b:
                                                        //IQV_11B_STANDARD_TX_ACC  = 1,
                                                        //IQV_11B_RMS_ERROR_VECTOR = 2
                                                )))
	{
		strcpy(message,LP_GetErrorString(errStatus));
                printf("Err: LP_Analyze80211b error...\n");
		return FAIL;
	}

//       if (ERR_OK != (errStatus = LP_AnalyzeTechnologyFFT(
//                                               3      //int technology
							//11ac    = 0,
                                                       //11n = 1,
                                                       //11ag = 2,
                                                       // 11b = 3
//                                               )))
//	{
//		strcpy(message,LP_GetErrorString(errStatus));
//               printf("Err: LP_AnalyzeTechnologyFFT error...\n");
//		return FAIL;
//	}






       
      	//Form1->lblElapsedTime->Text = FormatDateTime("hh:mm:ss:zzz", Now() - DateTime);
        //fprintf(FOpen, "%d,",channel);
	fprintf(FOpen, " %1.1f,"        ,LP_GetScalarMeasurement("bitRateInMHz",0));
	fprintf(FOpen, " %6.3f,"        ,LP_GetScalarMeasurement("pkPower",0));
	fprintf(FOpen, " %6.3f,"        ,LP_GetScalarMeasurement("rmsMaxAvgPower",0));
	fprintf(FOpen, " %6.3f,"        ,LP_GetScalarMeasurement("evmPk",0));
	fprintf(FOpen, " %6.3f,"        ,LP_GetScalarMeasurement("evmAll",0));
	fprintf(FOpen, " %12.3f,"       ,LP_GetScalarMeasurement("freqErr",0));
	fprintf(FOpen, " %12.3f,"       ,LP_GetScalarMeasurement("clockErr",0));
	fprintf(FOpen, " %6.3f,"        ,LP_GetScalarMeasurement("ampErrDb",0));
        fprintf(FOpen, " %6.3f,"        ,LP_GetScalarMeasurement("phaseErr",0));
        fprintf(FOpen, " %6.3f,"       ,LP_GetScalarMeasurement("rmsPhaseNoise",0));
//        fprintf(FOpen, " %6.3f\n"       ,LP_GetScalarMeasurement("spectrumCheck",0));

//        fclose(FOpen);
/*
	sprintf(cTemp, "Channel => %.0d\n", channel);
        printf("%s", cTemp);

	sprintf(cTemp, "DataRate => %.0f Mbps\n", LP_GetScalarMeasurement("bitRateInMHz",0));
        printf("%s", cTemp);

	sprintf(cTemp, "pkPower => %.3f dB\n", LP_GetScalarMeasurement("pkPower",0));
        printf("%s", cTemp);

	sprintf(cTemp, "rmsPower => %.3f dB\n", LP_GetScalarMeasurement("rmsMaxAvgPower",0));
        printf("%s", cTemp);

        sprintf(cTemp, "evmPk => %.3f dB\n", LP_GetScalarMeasurement("evmPk",0));
        printf("%s", cTemp);

	sprintf(cTemp, "evmAll => %.3f dB\n", LP_GetScalarMeasurement("evmAll",0));
        printf("%s", cTemp);

	sprintf(cTemp, "freqErr => %.3f Hz\n", LP_GetScalarMeasurement("freqErr",0));
        printf("%s", cTemp);

	sprintf(cTemp, "clockErr => %.3f ppm\n", LP_GetScalarMeasurement("clockErr",0));
        printf("%s", cTemp);

	sprintf(cTemp, "ampErrDb => %.3f dB\n", LP_GetScalarMeasurement("ampErrDb",0));
        printf("%s", cTemp);

      	sprintf(cTemp, "IQPhaseErr => %.3f Deg\n", LP_GetScalarMeasurement("phaseErr",0));
        printf("%s", cTemp);

	sprintf(cTemp, "rmsPhaseNoise => %.3f Deg\n", LP_GetScalarMeasurement("rmsPhaseNoise",0));
        printf("%s", cTemp);
*/
//        return OK;
//---------------------------------------------------------------------------
        errStatus = ::LP_SetAnalysisParameterInteger("AnalyzeFFT", "vsaNum", 1);
	errStatus = ::LP_AnalyzeFFT();

	// Store the result
	double *bufferRealX = NULL, *bufferImagX = NULL, *bufferRealY=NULL, *bufferImagY=NULL;
	bufferRealX = (double *)malloc((MAX_BUFFER_SIZE*8)*sizeof(double));
	bufferImagX = (double *)malloc((MAX_BUFFER_SIZE*8)*sizeof(double));
	bufferRealY = (double *)malloc((MAX_BUFFER_SIZE*8)*sizeof(double));
	bufferImagY = (double *)malloc((MAX_BUFFER_SIZE*8)*sizeof(double));

	int    bufferSizeX = ::LP_GetVectorMeasurement("x", bufferRealX, bufferImagX, MAX_BUFFER_SIZE*8);
	int    bufferSizeY = ::LP_GetVectorMeasurement("y", bufferRealY, bufferImagY, MAX_BUFFER_SIZE*8);

	if ( (0>=bufferSizeX)||(0>=bufferSizeY) )
	{
		errStatus = -1;

		if (bufferRealX) free(bufferRealX);
		if (bufferImagX) free(bufferImagX);
		if (bufferRealY) free(bufferRealY);
		if (bufferImagY) free(bufferImagY);
	}

	double *maskWiFi = (double *)malloc(bufferSizeY*sizeof(double));

	int maskResult = testSpectrumMask(bufferRealX, bufferRealY, bufferSizeY, WIFI_11B, maskWiFi, &vio);

       	sprintf(cTemp, "%s:\t%.3f %%, status:%d\n", "Spectrum violation", vio, maskResult);
        printf("%s", cTemp);

    //    strcat(message, cTemp);

 	double obwPercentage = 0.99;
	int startIndex = 0, stopIndex = 0;
	double obw;

	obw = GetOBW(bufferRealX, bufferRealY, bufferSizeY, obwPercentage, startIndex, stopIndex);

	sprintf(cTemp, "%s:\t%.3f MHz\n", "OBW", obw);
        printf("%s", cTemp);
     //   strcat(message, cTemp);

//	Form1->Series1->Clear();
//	Form1->Series2->Clear();

//	Form1->Chart1->Title->Text->Strings[0] = "PSD Plot";
//	for (i = 0; i < bufferSizeY; i++)
//	{
//		Form1->Series1->AddXY(bufferRealX[i]*1e-6, bufferRealY[i]);
//		Form1->Series2->AddXY(bufferRealX[i]*1e-6, maskWiFi[i]);
//	}
        fprintf(FOpen, " %d\n", maskResult);
	// free memory
 	if (bufferRealX) free(bufferRealX);
	if (bufferImagX) free(bufferImagX);
	if (bufferRealY) free(bufferRealY);
	if (bufferImagY) free(bufferImagY);
	if (maskWiFi) free(maskWiFi);

    fclose(FOpen);
    return OK;
}

