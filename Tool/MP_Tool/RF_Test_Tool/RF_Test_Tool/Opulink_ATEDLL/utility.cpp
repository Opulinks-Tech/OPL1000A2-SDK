#include <stdio.h >
#include <stdlib.h >
#include <string.h>
#include <math.h>
#include <vector>
#include <map>
#include "IQmeasure.h"
#include "utility.h"
#include <fstream>

using namespace std;

#define MAX_BUFFER_SIZE			8192

#define NA_NUMBER				-99999.99

#define INT_MAX					2147483647    // maximum (signed) int value
#ifndef NA_INTEGER
	#define NA_INTEGER			-(INT_MAX)
#endif

#define MAX_POSITIVE_SECTION	5
#define MAX_NEGATIVE_SECTION	5

// Carrier number
#define CARRIER_0    0
#define CARRIER_1    1
#define CARRIER_2    2
#define CARRIER_6    6
#define CARRIER_16   16
#define CARRIER_17   17
#define CARRIER_26   26
#define CARRIER_28   28
#define CARRIER_31   31
#define CARRIER_32   32
#define CARRIER_33   33
#define CARRIER_42   42
#define CARRIER_43   43
#define CARRIER_58   58
#define CARRIER_70   70
#define CARRIER_84   84
#define CARRIER_85   85
#define CARRIER_95	 95
#define CARRIER_96	 96
#define CARRIER_97	 97
#define CARRIER_122  122

#define INDEX_OF_SUBCARRIER_0_HT20	32;
#define INDEX_OF_SUBCARRIER_0_HT40	64;
#define INDEX_OF_SUBCARRIER_0_HT80	128;

int     g_Tester_Number = 1;

//==============================================================================================

typedef struct tagDefaultMask
{
	double  freqOffset;
	double  deltaMaskPwr;
} DEFAULT_MASK_STRUCT;

vector<DEFAULT_MASK_STRUCT> l_wifiMask_11b;
vector<DEFAULT_MASK_STRUCT> l_wifiMask_11ag;

vector<DEFAULT_MASK_STRUCT> *l_wifiMask_11nHt20;
vector<DEFAULT_MASK_STRUCT> *l_wifiMask_11nHt40;
vector<DEFAULT_MASK_STRUCT> l_wifiMask_11nHt20_2G;
vector<DEFAULT_MASK_STRUCT> l_wifiMask_11nHt20_5G;
vector<DEFAULT_MASK_STRUCT> l_wifiMask_11nHt40_2G;
vector<DEFAULT_MASK_STRUCT> l_wifiMask_11nHt40_5G;

vector<DEFAULT_MASK_STRUCT> l_wifiMask_11acBss20;
vector<DEFAULT_MASK_STRUCT> l_wifiMask_11acBss40;
vector<DEFAULT_MASK_STRUCT> l_wifiMask_11acBss80;
vector<DEFAULT_MASK_STRUCT> l_wifiMask_11acBss160;
vector<DEFAULT_MASK_STRUCT> l_wifiMask_11acBss80_80;

//==============================================================================================

double CalculateIQtoP(double data_i, double data_q)
{
	return (data_i*data_i + data_q*data_q);
}

void CheckFlatnessViolation(double deltaPower, int carrier, double lowLimit, double highLimit, double *sectionPower, int *sectionCarrier, double *failMargin, int *sectionCarrier2)
{
	double _upperMargin = deltaPower - highLimit;
	double _lowerMargin = lowLimit - deltaPower;

	if((_upperMargin>=_lowerMargin) && (_upperMargin>(*failMargin)))
	{
		*failMargin = _upperMargin;
		*sectionPower = deltaPower;
		*sectionCarrier = carrier;
		*sectionCarrier2 = carrier;
	}
	else if((_upperMargin<_lowerMargin) && (_lowerMargin>(*failMargin)))
	{
		*failMargin = _lowerMargin;
		*sectionPower = deltaPower;
		*sectionCarrier = carrier;
		*sectionCarrier2 = carrier;
	}
}

int LoResultSpectralFlatness11AG( int* carrierNoUpA,
								  int* carrierNoUpB,
								  int* carrierNoLoA,
								  int* carrierNoLoB,
								  double* failMargin, 
								  int* leastMarginCarrier,
								  double* loLeakage, 
								  double* valueDbUpSectionA,
								  double* valueDbUpSectionB,
								  double* valueDbLoSectionA,
								  double* valueDbLoSectionB)
{
    int     result = ERR_OK;
   
    int     i, lengthIQ = 0;
    double  *dataQ, *dataI, *data;  
    double  avg_center_power = 0, avg_power = 0;
	int     numberOfEst = 0;
	int     carrierFirst, carrierSecond, carrierThird, carrierFourth;
	double  bufferReal[MAX_BUFFER_SIZE], bufferImag[MAX_BUFFER_SIZE];
	//bool    flatnessPass = true;


	/* for non-analysis cas, initialize margin and loLeakage to NaN*/
	*failMargin    = NA_NUMBER;
	*loLeakage = NA_NUMBER;
	
	*carrierNoUpA = NA_INTEGER;
	*carrierNoUpB = NA_INTEGER;
	*carrierNoLoA = NA_INTEGER;
	*carrierNoLoB = NA_INTEGER;

	/* for non-analysis cas, initialize valueDbUpSectionA/B and valudDbLoSectionA/B to NaN*/
	*valueDbUpSectionA = NA_NUMBER;
	*valueDbUpSectionB = NA_NUMBER;
	*valueDbLoSectionA = NA_NUMBER;
	*valueDbLoSectionB = NA_NUMBER;

	numberOfEst = ::LP_GetVectorMeasurement("hhEst", bufferReal, bufferImag, MAX_BUFFER_SIZE);
    if ( numberOfEst!=64 )	// IQ data length, should be 64
    {
        return -1;
    }
	else
	{
		lengthIQ = numberOfEst; 
	}
    
    dataI    = (double *) malloc (lengthIQ * sizeof (double));
	dataQ    = (double *) malloc (lengthIQ * sizeof (double));
    data     = (double *) malloc (lengthIQ * sizeof (double));

    if (dataQ && dataI && data)
    {
		// Get IQ data.			
		for (int carrier=0;carrier<numberOfEst;carrier++)
		{
			dataI[carrier] = bufferReal[carrier];
			dataQ[carrier] = bufferImag[carrier];

			// 1. Convert I and Q to power: P = I*I + Q*Q (the formula is not 100% correct, but log will take out the ratio)
			// 2. Average P 
			// 3. avg_power = 10 * log (avg_P)

			// Calculate the power at each point from I & Q data.
			data[carrier] = CalculateIQtoP( dataI[carrier], dataQ[carrier] );
		}

		// IEEE 802.11a/g Transmitter flatness
		// The average energy of the constellations in each of the spectral lines -16.. -1 and +1.. +16 will deviate no
		// more than +/- 2 dB from their average energy. The average energy of the constellations in each of the spectral
		// lines -26.. -17 and +17.. +26 will deviate no more than +2/-4 dB from the average energy of spectral lines
		// -16.. -1 and +1.. +16. The data for this test shall be derived from the channel estimation step.
		
		carrierFirst  = CARRIER_1;
		carrierSecond = CARRIER_16;
		carrierThird  = CARRIER_17;
		carrierFourth = CARRIER_26;
	
		// Calculate the "avg_center_power" from carriers of -16 to -1 and +1 to +16.
		// Calculate the "avg_power"        from carriers of -26 to -1 and +1 to +26.

        // Calculate the average energy for the spectral lines
		for (i=carrierFirst;i<=carrierSecond;i++)
		{
			avg_center_power = avg_center_power + data[i] + data[lengthIQ-i];
		}
		// Continue, calculating the (side) average energy. This one must add the average energy of avg_center_power.
		for (i=carrierThird;i<=carrierFourth;i++)
		{
			avg_power = avg_power + data[i] + data[lengthIQ-i];
		}
		avg_power = avg_power + avg_center_power;

		avg_center_power = avg_center_power/((carrierSecond-carrierFirst+1)*2);
		if (0!=avg_center_power)
		{
			avg_center_power = 10.0 * log10 (avg_center_power);
		}
		else
		{
			avg_center_power = 0;
		}
		avg_power = avg_power/((carrierFourth-carrierFirst+1)*2);
		if (0!=avg_power)
		{
			avg_power = 10.0 * log10 (avg_power);
		}
		else
		{
			avg_power = 0;
		}

	    for (int carrier=0;carrier<lengthIQ;carrier++)
	    {
			if (0!=data[carrier])
			{
				data[carrier] = 10.0 * log10 (data[carrier]);
			}
			else
			{
				data[carrier] = 0;
			}
	    }

		double deltaPower = 0;
		double highLimit = 2, lowLimit = -2;
		double sectionMargin[4] = {-99, -99, -99, -99};
		int sectionCarrierNo[4];

		// Check the flatness for carrier -16 to -1 and +1 to +16.
		for (i=carrierFirst;i<=carrierSecond;i++)
		{
			deltaPower = data[i]-avg_center_power;
			CheckFlatnessViolation(deltaPower, i, lowLimit, highLimit, valueDbUpSectionA, carrierNoUpA, &sectionMargin[0], &sectionCarrierNo[0]);
			deltaPower = data[lengthIQ-i]-avg_center_power;
			CheckFlatnessViolation(deltaPower, -i, lowLimit, highLimit, valueDbLoSectionA, carrierNoLoA, &sectionMargin[1], &sectionCarrierNo[1]);
		}

		// Continue, Check the flatness -26 to -17 and +17 to +26.
		lowLimit = -4;
		double sectionBFailMargin = 0;
		for (i=carrierThird;i<=carrierFourth;i++)
		{
			deltaPower = data[i]-avg_center_power;
			CheckFlatnessViolation(deltaPower, i, lowLimit, highLimit, valueDbUpSectionB, carrierNoUpB, &sectionMargin[2], &sectionCarrierNo[2]);
			deltaPower = data[lengthIQ-i]-avg_center_power;
			CheckFlatnessViolation(deltaPower, -i, lowLimit, highLimit, valueDbLoSectionB, carrierNoLoB, &sectionMargin[3], &sectionCarrierNo[3]);
		}

		// get the highest fail margin
		*failMargin = -99;
		for (i=0 ; i<4 ; i++)
		{
			if (sectionMargin[i]>*failMargin)
			{
				*failMargin = sectionMargin[i];
				*leastMarginCarrier = sectionCarrierNo[i];
			}
		}

		// Get the center frequency leakage, data[0] is the center frequency power
		// minus "10 * log10((double)((carrierFourth-carrierFirst+1)*2))" for calculate dBc measurement
		*loLeakage = data[0] - avg_power - 10 * log10((double)((carrierFourth-carrierFirst+1)*2));
	}
	else
	{
		// if (dataQ && dataI && data)
		result = -1;
	}

    if (dataQ)	   free (dataQ);
    if (dataI)	   free (dataI);
    if (data)	   free (data);

    return result;
}

int  LoResultSpectralFlatness11N(int basedOnVsa,
								 WIFI_MODE wifiMode,
								 int* carrierNoUpA,
								 int* carrierNoUpB,
								 int* carrierNoLoA,
								 int* carrierNoLoB,
								 double* failMargin,
								 int* leastMarginCarrier,
								 double* loLeakage,
								 double* valueDbUpSectionA,
								 double* valueDbUpSectionB,
								 double* valueDbLoSectionA,
								 double* valueDbLoSectionB,
								 int streamIndex)
// streamIndex = 0, 1, 2, 3 (zero based)
{
    int     result = ERR_OK;
   
    int     i, lengthIQ;     
    double  avg_center_power = 0, avg_power = 0;
	int     numberOfEst = 0, numberOfStream = 0, bandwidthMhz = 20;
	int     carrierFirst, carrierSecond, carrierThird, carrierFourth;
	double  *dataRaw, *data; 
	double  bufferReal[MAX_BUFFER_SIZE], bufferImag[MAX_BUFFER_SIZE];
	//bool    flatnessPass = true;
    int		vsaIndex = 0; 


	/* for non-analysis cas, initialize margin and loLeakage to NaN*/
	*failMargin = NA_NUMBER;
	*loLeakage = NA_NUMBER;

	*carrierNoUpA = NA_INTEGER;
	*carrierNoUpB = NA_INTEGER;
	*carrierNoLoA = NA_INTEGER;
	*carrierNoLoB = NA_INTEGER;

	/* for non-analysis cas, initialize valuedbUpSectionA/B to NaN*/
	*valueDbUpSectionA = NA_NUMBER;
	*valueDbUpSectionB = NA_NUMBER;
	*valueDbLoSectionA = NA_NUMBER;
	*valueDbLoSectionB = NA_NUMBER;

	vsaIndex = streamIndex;

    //NOTE: Nstreams x Ntones x NRx
	numberOfEst = ::LP_GetVectorMeasurement("channelEst", bufferReal, bufferImag, MAX_BUFFER_SIZE);
    if (numberOfEst <= 0)
    {
        return -1;
    }

	numberOfStream = (int)::LP_GetScalarMeasurement("rateInfo_spatialStreams", 0);

    if ((streamIndex >= numberOfStream) && !basedOnVsa)
    {
        return 0;	// special case, need return, but not error.
    }


	bandwidthMhz = (int)::LP_GetScalarMeasurement("rateInfo_bandwidthMhz", 0); 
	if ( (wifiMode==WIFI_11N_MF_HT20 || wifiMode==WIFI_11N_GF_HT20)&&(bandwidthMhz!=20) )
	{
		return -1;
	}
	else if ( (wifiMode==WIFI_11N_MF_HT40 || wifiMode==WIFI_11N_GF_HT40)&&(bandwidthMhz!=40) )
	{
		return -1;
	}
	else
	{
		// IQ data length, should be 64 or 128
		if (bandwidthMhz==20)
		{
			lengthIQ = 64;
		}
		else
		{
			lengthIQ = 128;
		}
	}
 
	dataRaw  = (double *) malloc (numberOfEst * sizeof (double));
    data     = (double *) malloc (lengthIQ * sizeof (double));

    if (dataRaw && data)
    {
		// 1. Convert I and Q to power: P = I*I + Q*Q (the formula is not 100% correct, but log will take out the ratio)
		// 2. Average P 
		// 3. avg_power = 10 * log (avg_P)

		// Calculate the power at each point from I & Q raw data.
		for (int carrier=0;carrier<numberOfEst;carrier++)
		{
			dataRaw[carrier] = CalculateIQtoP( bufferReal[carrier], bufferImag[carrier] );
		}

		// Initial the data[] array
		for (int carrier=0;carrier<lengthIQ;carrier++)
		{
			data[carrier] = 0;
		}	

        if(!basedOnVsa)
        {
		    // Get data by stream and each VSA.			
		    for (int carrier=0;carrier<lengthIQ;carrier++)
		    {
			    for (int i=0;i<g_Tester_Number;i++)
			    {
				    int numberOfIndex = (lengthIQ*numberOfStream*i)+(numberOfStream*carrier)+streamIndex;
				    if (numberOfIndex<numberOfEst)
				    {
					    data[carrier] = data[carrier] + dataRaw[numberOfIndex];
				    }
				    else
				    {
					    break;
				    }
			    }		
		    }
        }
        else // based on Vsa
        {
		    for (int carrier=0;carrier<lengthIQ;carrier++)
		    {
			    for (int i=0;i<numberOfStream;i++)
			    {
				    int numberOfIndex = (lengthIQ*numberOfStream*vsaIndex)+(numberOfStream*carrier)+i;
				    if (numberOfIndex<numberOfEst)
				    {
					    data[carrier] = data[carrier] + dataRaw[numberOfIndex];
				    }
				    else
				    {
					    break;
				    }
			    }		
		    }

        }




		// IEEE std 802.11n Transmitter flatness
		// HT20	
		// In a 20 MHz channel and in corresponding 20 MHz transmission in a 40 MHz channel, the average energy
		// of the constellations in each of the spectral lines ¡V16 to ¡V1 and +1 to +16 shall deviate no more than ¡Ó 2 dB
		// from their average energy. The average energy of the constellations in each of the spectral lines ¡V28 to ¡V17
		// and +17 to +28 shall deviate no more than +2/¡V4 dB from the average energy of spectral lines ¡V16 to ¡V1 and +1 to +16.

		// HT40
		// In a 40 MHz transmission (excluding HT duplicate format and non-HT duplicate format) the average energy
		// of the constellations in each of the spectral lines ¡V42 to ¡V2 and +2 to +42 shall deviate no more than ¡Ó 2 dB
		// from their average energy. The average energy of the constellations in each of the spectral lines ¡V43 to ¡V58
		// and +43 to +58 shall deviate no more than +2/¡V4 dB from the average energy of spectral lines ¡V42 to ¡V2 and +2 to +42.

		// In HT duplicate format and non-HT duplicate format the average energy of the constellations in each of the
		// spectral lines -42 to -33, -31 to -6, +6 to +31, and +33 to +42 shall deviate no more than ¡Ó 2 dB from their
		// average energy. The average energy of the constellations in each of the spectral lines -43 to -58 and +43 to
		// +58 shall deviate no more than +2/-4 dB from the average energy of spectral lines -42 to -33, -31 to -6, +6
		// to +31, and +33 to +42.

		if (lengthIQ==64)	// HT20 mode
		{
			carrierFirst  = CARRIER_1;
			carrierSecond = CARRIER_16;
			carrierThird  = CARRIER_17;
			carrierFourth = CARRIER_28;
		}
		else				// HT40 mode, lengthIQ=128
		{
			carrierFirst  = CARRIER_2;
			carrierSecond = CARRIER_42;
			carrierThird  = CARRIER_43;
			carrierFourth = CARRIER_58;
		}		


        /* Carrier number report order:
        * 20MHz: +1 ~ +32, -32 ~ -1
        * 40MHz: +1 ~ +64, -64 ~ -1
        */
        // Calculate the average energy for the spectral lines
		for ( i=carrierFirst;i<=carrierSecond;i++)
		{
			avg_center_power = avg_center_power + data[i] + data[lengthIQ-i];
		}
		// Continue, calculating the (side) average energy. This one must add the average energy of avg_center_power.
		for (i=carrierThird;i<=carrierFourth;i++)
		{
			avg_power = avg_power + data[i] + data[lengthIQ-i];
		}
		avg_power = avg_power + avg_center_power;

		avg_center_power = avg_center_power/((carrierSecond-carrierFirst+1)*2);
		if (0!=avg_center_power)
		{
			avg_center_power = 10.0 * log10 (avg_center_power);
		}
		else
		{
			avg_center_power= 0;
		}
		avg_power = avg_power/((carrierFourth-carrierFirst+1)*2);
		if (0!=avg_power)
		{
			avg_power = 10.0 * log10 (avg_power);
		}
		else
		{
			avg_power = 0;
		}

		for (int carrier=0;carrier<lengthIQ;carrier++)
		{
			if (0!=data[carrier])
			{
				data[carrier] = 10.0 * log10 (data[carrier]);
			}
			else
			{
				data[carrier] = 0;
			}
		}

		// Check the flatness
		double deltaPower = 0;
		double highLimit = 2, lowLimit = -2;
		double sectionMargin[4] = {-99, -99, -99, -99};
		int sectionCarrierNo[4];

		for (i=carrierFirst;i<=carrierSecond;i++)
		{
			//UPPER SECTION A
			deltaPower = data[i]-avg_center_power;
			CheckFlatnessViolation(deltaPower, i, lowLimit, highLimit, valueDbUpSectionA, carrierNoUpA, &sectionMargin[0], &sectionCarrierNo[0]);

			//LOWER SECTION A
			deltaPower = data[lengthIQ-i]-avg_center_power;
			CheckFlatnessViolation(deltaPower, -i, lowLimit, highLimit, valueDbLoSectionA, carrierNoLoA, &sectionMargin[1], &sectionCarrierNo[1]);
		}

		// Continue, Check the flatness. (side)
		lowLimit = -4;
		for (i=carrierThird;i<=carrierFourth;i++)
		{
			//UPPER SECTION B
			deltaPower = data[i]-avg_center_power;
			CheckFlatnessViolation(deltaPower, i, lowLimit, highLimit, valueDbUpSectionB, carrierNoUpB, &sectionMargin[2], &sectionCarrierNo[2]);
			
			//LOWER SECTION B
			deltaPower = data[lengthIQ-i]-avg_center_power;
			CheckFlatnessViolation(deltaPower, -i, lowLimit, highLimit, valueDbLoSectionB, carrierNoLoB, &sectionMargin[3], &sectionCarrierNo[3]);
		}

		// get the highest fail margin
		*failMargin = -99;
		for (i=0 ; i<4 ; i++)
		{
			if (sectionMargin[i]>*failMargin)
			{
				*failMargin = sectionMargin[i];
				*leastMarginCarrier = sectionCarrierNo[i];
			}
		}

		// Get the center frequency leakage 
		*loLeakage = data[0] - avg_power - 10 * log10((double)((carrierFourth-carrierFirst+1)*2));
	}
	else
	{
		// if (dataQ && dataI && data && maxData && minData)
		result = -1;
	}
    
    if (dataRaw)   free (dataRaw);
	if (data)	   free (data);

    return result;
}

int  LoResultSpectralFlatness11AC(int basedOnVsa,
								  int cbw,
								  int* carrierNoUpA,
								  int* carrierNoUpB,
								  int* carrierNoLoA,
								  int* carrierNoLoB,
								  double* failMargin,
								  int* leastMarginCarrier,
								  double* loLeakage,
								  double* valueDbUpSectionA,
								  double* valueDbUpSectionB,
								  double* valueDbLoSectionA,
								  double* valueDbLoSectionB,
								  int streamIndex)
								  // streamIndex = 0, 1, 2, 3 (zero based)
{
	int     result = ERR_OK;

	int     i, lengthIQ;     
	double  avg_center_power = 0, avg_power = 0;
	int     numberOfEst = 0, numberOfStream = 0, bandwidthMhz = 20;
	int     carrierFirst, carrierSecond, carrierThird, carrierFourth;

	double  *dataRaw, *data; 
	double  bufferReal[MAX_BUFFER_SIZE], bufferImag[MAX_BUFFER_SIZE];
	//bool    flatnessPass = true;
	int		vsaIndex = 0; 

	double sectionMargin[4] = {-99, -99, -99, -99};
	int sectionCarrierNo[4];

	// Added for 802.11ac
	int		carrierPilot;
	int		carrierFifth, carrierSixth, carrierSeventh,	carrierEigth, carrierNinth,carrierTenth;  
	int		carrierNumSection1, carrierNumSection2;
	//END



	/* for non-analysis cas, initialize margin and loLeakage to NaN*/
	*failMargin = NA_NUMBER;
	//	*loLeakage = NA_DOUBLE;

	*carrierNoUpA = NA_INTEGER;
	*carrierNoUpB = NA_INTEGER;
	*carrierNoLoA = NA_INTEGER;
	*carrierNoLoB = NA_INTEGER;

	/* for non-analysis cas, initialize valuedbUpSectionA/B to NaN*/
	*valueDbUpSectionA = NA_NUMBER;
	*valueDbUpSectionB = NA_NUMBER;
	*valueDbLoSectionA = NA_NUMBER;
	*valueDbLoSectionB = NA_NUMBER;

	vsaIndex = streamIndex;

	//NOTE: Nstreams x Ntones x NRx
	numberOfEst = ::LP_GetVectorMeasurement("channelEst", bufferReal, bufferImag, MAX_BUFFER_SIZE);
	if (numberOfEst <= 0)
	{
		return -1;
	}

	numberOfStream = (int)::LP_GetScalarMeasurement("rateInfo_spatialStreams", 0);

	if ((streamIndex >= numberOfStream) && !basedOnVsa)
	{
		return 0;	// special case, need return, but not error.
	}
	

	bandwidthMhz = (int)::LP_GetScalarMeasurement("rateInfo_bandwidthMhz", 0); 
	if ( ( cbw == BW_20MHZ) &&(bandwidthMhz!=20) )
	{
		return -1;
	}
	else if ( ( cbw == BW_40MHZ )&&(bandwidthMhz!=40) )
	{
		return -1;
	}
	else if ( ( cbw == BW_80MHZ )&&(bandwidthMhz!=80) )
	{
		return -1;
	}
	else if ( ( cbw == BW_160MHZ )&&(bandwidthMhz!=160) )
	{
		return -1;
	}
	else if (( cbw == BW_80_80MHZ ) &&(bandwidthMhz!=160) )     // BW_80_80MHZ not supported yet
	{
		return -1;
	}
	else     // bandwidthMHz is correct
	{
		//if ( 0 != strcmp(l_11ACtxVerifyFlatnessParam.PACKET_FORMAT,PACKET_FORMAT_NON_HT))    //802.11ac VHT, MF_HT, GF_HT
		//{
			// IQ data length, should be 64 or 128
			if ( bandwidthMhz == 20)
			{
				lengthIQ = 64;
				carrierFirst  = CARRIER_1;
				carrierSecond = CARRIER_16;
				carrierThird  = CARRIER_17;
				carrierFourth = CARRIER_28;
			}
			else if (bandwidthMhz == 40)
			{
				lengthIQ = 128;
				carrierFirst  = CARRIER_2;
				carrierSecond = CARRIER_42;
				carrierThird  = CARRIER_43;
				carrierFourth = CARRIER_58;
			}
			else if (bandwidthMhz == 80)
			{
				lengthIQ = 256;
				carrierFirst  = CARRIER_2;
				carrierSecond = CARRIER_84;
				carrierThird  = CARRIER_85;
				carrierFourth = CARRIER_122;
			}
			else if (bandwidthMhz == 160)
			{
				// Not supported yet
				return 0;
			}
			else   // 80_80
			{
				// Not supported yet
				return 0;
			}
		//}
		//else   //802.11ac non-HT duplicate
		//{
		//	if ( bandwidthMhz == 40)
		//	{
		//		lengthIQ = 128;
		//		carrierFirst  = CARRIER_6;
		//		carrierSecond = CARRIER_42;
		//		carrierPilot = CARRIER_32;
		//		carrierThird  = CARRIER_43;
		//		carrierFourth = CARRIER_58;
		//		//	carrierSecond = CARRIER_31;
		//		//	carrierThird  = CARRIER_33;
		//		//	carrierFourth = CARRIER_42;
		//		//	carrierFifth  = CARRIER_43;
		//		//	carrierSixth  = CARRIER_58;

		//	}
		//	else if (bandwidthMhz == 80)  // bandwidth = 80
		//	{
		//		lengthIQ = 256;
		//		carrierFirst	= CARRIER_6;
		//		carrierSecond	= CARRIER_31;
		//		carrierThird	= CARRIER_33;
		//		carrierFourth	= CARRIER_58;
		//		carrierFifth	= CARRIER_70;
		//		carrierSixth	= CARRIER_84;
		//		carrierNumSection1 = (carrierSecond - carrierFirst + 1) +
		//			(carrierFourth - carrierThird + 1) +
		//			(carrierSixth - carrierFifth + 1);

		//		carrierSeventh  = CARRIER_85;
		//		carrierEigth	= CARRIER_95;
		//		carrierNinth	= CARRIER_97;
		//		carrierTenth	= CARRIER_122;
		//		carrierNumSection2 = ( carrierEigth - carrierSeventh + 1) +
		//			( carrierTenth - carrierNinth +1);

		//	}
		//	else if ( bandwidthMhz == 160)
		//	{
		//		// Not supported yet
		//	}
		//	else		// bandwidth == 80_80
		//	{
		//		// Not supported yet
		//	}
		//}
	}

	dataRaw  = (double *) malloc (numberOfEst * sizeof (double));
	data     = (double *) malloc (lengthIQ * sizeof (double));

	if (dataRaw && data)
	{
		// 1. Convert I and Q to power: P = I*I + Q*Q (the formula is not 100% correct, but log will take out the ratio)
		// 2. Average P 
		// 3. avg_power = 10 * log (avg_P)

		// Calculate the power at each point from I & Q raw data.
		for (int carrier=0;carrier<numberOfEst;carrier++)
		{
			dataRaw[carrier] = CalculateIQtoP( bufferReal[carrier], bufferImag[carrier] );
		}

		// Initial the data[] array
		for (int carrier=0;carrier<lengthIQ;carrier++)
		{
			data[carrier] = 0;
		}	

		if(!basedOnVsa)
		{
			// Get data by stream and each VSA.			
			for (int carrier=0;carrier<lengthIQ;carrier++)
			{
				for (int i=0;i<g_Tester_Number;i++)
				{
					int numberOfIndex = (lengthIQ*numberOfStream*i)+(numberOfStream*carrier)+streamIndex;
					if (numberOfIndex<numberOfEst)
					{
						data[carrier] = data[carrier] + dataRaw[numberOfIndex];
					}
					else
					{
						break;
					}
				}		
			}
		}
		else // based on Vsa
		{
			for (int carrier=0;carrier<lengthIQ;carrier++)
			{
				for (int i=0;i<numberOfStream;i++)
				{
					int numberOfIndex = (lengthIQ*numberOfStream*vsaIndex)+(numberOfStream*carrier)+i;
					if (numberOfIndex<numberOfEst)
					{
						data[carrier] = data[carrier] + dataRaw[numberOfIndex];
					}
					else
					{
						break;
					}
				}		
			}

		}


		//if ( 0 != strcmp(l_11ACtxVerifyFlatnessParam.PACKET_FORMAT,PACKET_FORMAT_NON_HT) && bandwidthMhz != 160)    //802.11ac VHT, MF_HT, GF_HT, BW = 20,40,80, 80_80
		//{
			// Calculate the average energy for the spectral lines
			for ( i=carrierFirst;i<=carrierSecond;i++)
			{
				avg_center_power = avg_center_power + data[i] + data[lengthIQ-i];
			}
			// Continue, calculating the (side) average energy. This one must add the average energy of avg_center_power.
			for (i=carrierThird;i<=carrierFourth;i++)
			{
				avg_power = avg_power + data[i] + data[lengthIQ-i];
			}
			avg_power = avg_power + avg_center_power;

			avg_center_power = avg_center_power/((carrierSecond-carrierFirst+1)*2);
			if (0!=avg_center_power)
			{
				avg_center_power = 10.0 * log10 (avg_center_power);
			}
			else
			{
				avg_center_power= 0;
			}
			avg_power = avg_power/((carrierFourth-carrierFirst+1)*2);
			if (0!=avg_power)
			{
				avg_power = 10.0 * log10 (avg_power);
			}
			else
			{
				avg_power = 0;
			}

			for (int carrier=0;carrier<lengthIQ;carrier++)
			{
				if (0!=data[carrier])
				{
					data[carrier] = 10.0 * log10 (data[carrier]);
				}
				else
				{
					data[carrier] = 0;
				}
			}


			// Check the flatness
			double deltaPower = 0;
			double highLimit = 4, lowLimit = -4;
			//double sectionMargin[4] = {-99, -99, -99, -99};
			//int sectionCarrierNo[4];

			for (i=carrierFirst;i<=carrierSecond;i++)
			{
				//UPPER SECTION A
				deltaPower = data[i]-avg_center_power;
				CheckFlatnessViolation(deltaPower, i, lowLimit, highLimit, valueDbUpSectionA, carrierNoUpA, &sectionMargin[0], &sectionCarrierNo[0]);

				//LOWER SECTION A
				deltaPower = data[lengthIQ-i]-avg_center_power;
				CheckFlatnessViolation(deltaPower, -i, lowLimit, highLimit, valueDbLoSectionA, carrierNoLoA, &sectionMargin[1], &sectionCarrierNo[1]);
			}

			// Continue, Check the flatness. (side)
			highLimit = 4, lowLimit = -6;
			for (i=carrierThird;i<=carrierFourth;i++)
			{
				//UPPER SECTION B
				deltaPower = data[i]-avg_center_power;
				CheckFlatnessViolation(deltaPower, i, lowLimit, highLimit, valueDbUpSectionB, carrierNoUpB, &sectionMargin[2], &sectionCarrierNo[2]);

				//LOWER SECTION B
				deltaPower = data[lengthIQ-i]-avg_center_power;
				CheckFlatnessViolation(deltaPower, -i, lowLimit, highLimit, valueDbLoSectionB, carrierNoLoB, &sectionMargin[3], &sectionCarrierNo[3]);
			}

		//}
		//else if ( 0 != strcmp(l_11ACtxVerifyFlatnessParam.PACKET_FORMAT,PACKET_FORMAT_NON_HT) && bandwidthMhz == 160) 
		//{
		//	// BW = 160MHz, not supported yet
		//}
		//else     //802.11ac non-HT
		//{
		//	if ( bandwidthMhz == 40)
		//	{
		//		// Calculate the average energy for the spectral lines
		//		for ( i=carrierFirst;i<=carrierSecond;i++)
		//		{
		//			avg_center_power = avg_center_power + data[i] + data[lengthIQ-i];
		//		}
		//		// Carrier32 is pilot, not caculated in avg_center_power
		//		avg_center_power = avg_center_power -data[carrierPilot] - data[lengthIQ-carrierPilot];

		//		// Continue, calculating the (side) average energy. This one must add the average energy of avg_center_power.
		//		for (i=carrierThird;i<=carrierFourth;i++)
		//		{
		//			avg_power = avg_power + data[i] + data[lengthIQ-i];
		//		}
		//		avg_power = avg_power + avg_center_power;

		//		//avg_center_power = avg_center_power/((carrierSecond-carrierFirst+1)*2);
		//		// Carrier 32 is pilot, not caculated
		//		avg_center_power = avg_center_power/((carrierSecond-carrierFirst)*2);
		//		if (0!=avg_center_power)
		//		{
		//			avg_center_power = 10.0 * log10 (avg_center_power);
		//		}
		//		else
		//		{
		//			avg_center_power= 0;
		//		}
		//		avg_power = avg_power/((carrierFourth-carrierFirst+1)*2);
		//		if (0!=avg_power)
		//		{
		//			avg_power = 10.0 * log10 (avg_power);
		//		}
		//		else
		//		{
		//			avg_power = 0;
		//		}

		//		for (int carrier=0;carrier<lengthIQ;carrier++)
		//		{
		//			if (0!=data[carrier])
		//			{
		//				data[carrier] = 10.0 * log10 (data[carrier]);
		//			}
		//			else
		//			{
		//				data[carrier] = 0;
		//			}
		//		}

		//		// Check the flatness
		//		double deltaPower = 0;
		//		double highLimit = 4, lowLimit = -4;
		//		//double sectionMargin[4] = {-99, -99, -99, -99};
		//		//int sectionCarrierNo[4];

		//		for (i=carrierFirst;i<=carrierSecond;i++)
		//		{
		//			if ( i != carrierPilot)
		//			{
		//				//UPPER SECTION A
		//				deltaPower = data[i]-avg_center_power;
		//				CheckFlatnessViolation(deltaPower, i, lowLimit, highLimit, valueDbUpSectionA, carrierNoUpA, &sectionMargin[0], &sectionCarrierNo[0]);

		//				//LOWER SECTION A
		//				deltaPower = data[lengthIQ-i]-avg_center_power;
		//				CheckFlatnessViolation(deltaPower, -i, lowLimit, highLimit, valueDbLoSectionA, carrierNoLoA, &sectionMargin[1], &sectionCarrierNo[1]);
		//			}
		//			else
		//			{
		//				// do nothing
		//			}
		//		}

		//		// Continue, Check the flatness. (side)
		//		highLimit = 4, lowLimit = -6;
		//		for (i=carrierThird;i<=carrierFourth;i++)
		//		{
		//			//UPPER SECTION B
		//			deltaPower = data[i]-avg_center_power;
		//			CheckFlatnessViolation(deltaPower, i, lowLimit, highLimit, valueDbUpSectionB, carrierNoUpB, &sectionMargin[2], &sectionCarrierNo[2]);

		//			//LOWER SECTION B
		//			deltaPower = data[lengthIQ-i]-avg_center_power;
		//			CheckFlatnessViolation(deltaPower, -i, lowLimit, highLimit, valueDbLoSectionB, carrierNoLoB, &sectionMargin[3], &sectionCarrierNo[3]);
		//		}
		//	}
		//	else if ( bandwidthMhz == 80) 
		//	{
		//		// Calculate the average energy for the spectral lines
		//		for ( i=carrierFirst;i<=carrierSecond;i++)   // carrier 6 ~31
		//		{
		//			avg_center_power = avg_center_power + data[i] + data[lengthIQ-i];
		//		}
		//		for ( i=carrierThird;i<=carrierFourth;i++)   // carrier 33 ~58
		//		{
		//			avg_center_power = avg_center_power + data[i] + data[lengthIQ-i];
		//		}
		//		for ( i=carrierFifth;i<=carrierSixth;i++)   // carrier 70 ~84
		//		{
		//			avg_center_power = avg_center_power + data[i] + data[lengthIQ-i];
		//		}



		//		// Continue, calculating the (side) average energy. This one must add the average energy of avg_center_power.
		//		for (i=carrierSeventh;i<=carrierEigth;i++)
		//		{
		//			avg_power = avg_power + data[i] + data[lengthIQ-i];
		//		}
		//		for (i=carrierTenth;i<=carrierNinth;i++)
		//		{
		//			avg_power = avg_power + data[i] + data[lengthIQ-i];
		//		}

		//		avg_power = avg_power + avg_center_power;

		//		//avg_center_power = avg_center_power/((carrierSecond-carrierFirst+1)*2);
		//		// Carrier 32 is pilot, not caculated
		//		avg_center_power = avg_center_power/ ( carrierNumSection1 *2);
		//		if (0!=avg_center_power)
		//		{
		//			avg_center_power = 10.0 * log10 (avg_center_power);
		//		}
		//		else
		//		{
		//			avg_center_power= 0;
		//		}
		//		avg_power = avg_power/(carrierNumSection2 *2);
		//		if (0!=avg_power)
		//		{
		//			avg_power = 10.0 * log10 (avg_power);
		//		}
		//		else
		//		{
		//			avg_power = 0;
		//		}

		//		for (int carrier=0;carrier<lengthIQ;carrier++)
		//		{
		//			if (0!=data[carrier])
		//			{
		//				data[carrier] = 10.0 * log10 (data[carrier]);
		//			}
		//			else
		//			{
		//				data[carrier] = 0;
		//			}
		//		}

		//		// Check the flatness
		//		double deltaPower = 0;
		//		double highLimit = 4, lowLimit = -4;


		//		for (i=carrierFirst;i<=carrierSecond;i++)
		//		{
		//			//UPPER SECTION A
		//			deltaPower = data[i]-avg_center_power;
		//			CheckFlatnessViolation(deltaPower, i, lowLimit, highLimit, valueDbUpSectionA, carrierNoUpA, &sectionMargin[0], &sectionCarrierNo[0]);

		//			//LOWER SECTION A
		//			deltaPower = data[lengthIQ-i]-avg_center_power;
		//			CheckFlatnessViolation(deltaPower, -i, lowLimit, highLimit, valueDbLoSectionA, carrierNoLoA, &sectionMargin[1], &sectionCarrierNo[1]);
		//		}

		//		for (i=carrierThird;i<=carrierFourth;i++)
		//		{
		//			//UPPER SECTION A
		//			deltaPower = data[i]-avg_center_power;
		//			CheckFlatnessViolation(deltaPower, i, lowLimit, highLimit, valueDbUpSectionA, carrierNoUpA, &sectionMargin[0], &sectionCarrierNo[0]);

		//			//LOWER SECTION A
		//			deltaPower = data[lengthIQ-i]-avg_center_power;
		//			CheckFlatnessViolation(deltaPower, -i, lowLimit, highLimit, valueDbLoSectionA, carrierNoLoA, &sectionMargin[1], &sectionCarrierNo[1]);
		//		}

		//		for (i=carrierSixth;i<=carrierFifth;i++)
		//		{
		//			//UPPER SECTION A
		//			deltaPower = data[i]-avg_center_power;
		//			CheckFlatnessViolation(deltaPower, i, lowLimit, highLimit, valueDbUpSectionA, carrierNoUpA, &sectionMargin[0], &sectionCarrierNo[0]);

		//			//LOWER SECTION A
		//			deltaPower = data[lengthIQ-i]-avg_center_power;
		//			CheckFlatnessViolation(deltaPower, -i, lowLimit, highLimit, valueDbLoSectionA, carrierNoLoA, &sectionMargin[1], &sectionCarrierNo[1]);
		//		}

		//		// Continue, Check the flatness. (side)
		//		highLimit = 4, lowLimit = -6;
		//		for (i=carrierSeventh;i<=carrierEigth;i++)
		//		{
		//			//UPPER SECTION B
		//			deltaPower = data[i]-avg_center_power;
		//			CheckFlatnessViolation(deltaPower, i, lowLimit, highLimit, valueDbUpSectionB, carrierNoUpB, &sectionMargin[2], &sectionCarrierNo[2]);

		//			//LOWER SECTION B
		//			deltaPower = data[lengthIQ-i]-avg_center_power;
		//			CheckFlatnessViolation(deltaPower, -i, lowLimit, highLimit, valueDbLoSectionB, carrierNoLoB, &sectionMargin[3], &sectionCarrierNo[3]);
		//		}

		//		for (i=carrierTenth;i<=carrierNinth;i++)
		//		{
		//			//UPPER SECTION B
		//			deltaPower = data[i]-avg_center_power;
		//			CheckFlatnessViolation(deltaPower, i, lowLimit, highLimit, valueDbUpSectionB, carrierNoUpB, &sectionMargin[2], &sectionCarrierNo[2]);

		//			//LOWER SECTION B
		//			deltaPower = data[lengthIQ-i]-avg_center_power;
		//			CheckFlatnessViolation(deltaPower, -i, lowLimit, highLimit, valueDbLoSectionB, carrierNoLoB, &sectionMargin[3], &sectionCarrierNo[3]);
		//		}
		//	}
		//	else if ( bandwidthMhz == 160)
		//	{
		//		// not supported yet
		//	}
		//	else	// bandwidth = 80_80
		//	{
		//		// not supported yet
		//	}

		//}


		// get the highest fail margin
		*failMargin = -99;
		for (i=0 ; i<4 ; i++)
		{
			if (sectionMargin[i]>*failMargin)
			{
				*failMargin = sectionMargin[i];
				*leastMarginCarrier = sectionCarrierNo[i];
			}
		}

		// Get the center frequency leakage 
		*loLeakage = data[0] - avg_power - 10 * log10((double)((carrierFourth-carrierFirst+1)*2));
	}
	else
	{
		// if (dataQ && dataI && data && maxData && minData)
		result = -1;
	}

	if (dataRaw)   free (dataRaw);
	if (data)	   free (data);

	return result;
}

int LoResultSpectralFlatness11AG_byAPI( int* carrierNoUpA,
									   int* carrierNoUpB,
									   int* carrierNoLoA,
									   int* carrierNoLoB,
									   double* failMargin, 
									   int* leastMarginCarrier,
									   double* loLeakage, 
									   double* valueDbUpSectionA,
									   double* valueDbUpSectionB,
									   double* valueDbLoSectionA,
									   double* valueDbLoSectionB)
{
	int	iRet = ERR_OK;

	int	carrierFirst;
	int	carrierSecond;
	int	carrierThird;
	int	carrierFourth;
	int	indexOfSubcarrier_0;
	/* for non-analysis cas, initialize margin and loLeakage to NaN*/
	*failMargin    = NA_NUMBER;
	*loLeakage = NA_NUMBER;

	*carrierNoUpA = NA_INTEGER;
	*carrierNoUpB = NA_INTEGER;
	*carrierNoLoA = NA_INTEGER;
	*carrierNoLoB = NA_INTEGER;

	/* for non-analysis cas, initialize valueDbUpSectionA/B and valudDbLoSectionA/B to NaN*/
	*valueDbUpSectionA = NA_NUMBER;
	*valueDbUpSectionB = NA_NUMBER;
	*valueDbLoSectionA = NA_NUMBER;
	*valueDbLoSectionB = NA_NUMBER;

	// IEEE 802.11a/g Transmitter flatness
	// The average energy of the constellations in each of the spectral lines -16.. -1 and +1.. +16 will deviate no
	// more than +/- 2 dB from their average energy. The average energy of the constellations in each of the spectral
	// lines -26.. -17 and +17.. +26 will deviate no more than +2/-4 dB from the average energy of spectral lines
	// -16.. -1 and +1.. +16. The data for this test shall be derived from the channel estimation step.
	carrierFirst  = CARRIER_1;
	carrierSecond = CARRIER_16;
	carrierThird  = CARRIER_17;
	carrierFourth = CARRIER_26;
	indexOfSubcarrier_0 = INDEX_OF_SUBCARRIER_0_HT20;

	double bufferFlatnessReal[MAX_BUFFER_SIZE], bufferFlatnessImag[MAX_BUFFER_SIZE];
	double bufferFlatnessUpLimitReal[MAX_BUFFER_SIZE], bufferFlatnessUpLimitImag[MAX_BUFFER_SIZE];
	double bufferFlatnessLowLimitReal[MAX_BUFFER_SIZE], bufferFlatnessLowLimitImag[MAX_BUFFER_SIZE];
	int    bufferSizeFlatness = ::LP_GetVectorMeasurement("spectralFlatness", bufferFlatnessReal, bufferFlatnessImag, MAX_BUFFER_SIZE);
	int    bufferSizeFlatnessUpLimit = ::LP_GetVectorMeasurement("spectralFlatnessHighLimit", bufferFlatnessUpLimitReal, bufferFlatnessUpLimitImag, MAX_BUFFER_SIZE);
	int    bufferSizeFlatnessLowLimit = ::LP_GetVectorMeasurement("spectralFlatnessLowLimit", bufferFlatnessLowLimitReal, bufferFlatnessLowLimitImag, MAX_BUFFER_SIZE);

	if (bufferSizeFlatness>0 && bufferSizeFlatnessUpLimit>0 && bufferSizeFlatnessLowLimit>0)
	{
		double sectionMargin[4] = {-99, -99, -99, -99};
		int sectionCarrierNo[4];
		int subcarrier_index, i;

		// Check the flatness for carrier -16 to -1 and +1 to +16.
		for (i=carrierFirst; i<=carrierSecond; i++)
		{
			//UPPER SECTION A
			subcarrier_index = indexOfSubcarrier_0 + i;
			CheckFlatnessViolation(bufferFlatnessReal[subcarrier_index], i, bufferFlatnessLowLimitReal[subcarrier_index]
			, bufferFlatnessUpLimitReal[subcarrier_index], valueDbUpSectionA, carrierNoUpA, &sectionMargin[0], &sectionCarrierNo[0]);

			//LOWER SECTION A
			subcarrier_index = indexOfSubcarrier_0 - i;
			CheckFlatnessViolation(bufferFlatnessReal[subcarrier_index], -i, bufferFlatnessLowLimitReal[subcarrier_index]
			, bufferFlatnessUpLimitReal[subcarrier_index], valueDbLoSectionA, carrierNoLoA, &sectionMargin[1], &sectionCarrierNo[1]);
		}

		// Continue, Check the flatness -26 to -17 and +17 to +26.
		for (i=carrierThird; i<=carrierFourth; i++)
		{
			//UPPER SECTION B
			subcarrier_index = indexOfSubcarrier_0 + i;
			CheckFlatnessViolation(bufferFlatnessReal[subcarrier_index], i, bufferFlatnessLowLimitReal[subcarrier_index]
			, bufferFlatnessUpLimitReal[subcarrier_index], valueDbUpSectionB, carrierNoUpB, &sectionMargin[2], &sectionCarrierNo[2]);

			//LOWER SECTION B
			subcarrier_index = indexOfSubcarrier_0 - i;
			CheckFlatnessViolation(bufferFlatnessReal[subcarrier_index], -i, bufferFlatnessLowLimitReal[subcarrier_index]
			, bufferFlatnessUpLimitReal[subcarrier_index], valueDbLoSectionB, carrierNoLoB, &sectionMargin[3], &sectionCarrierNo[3]);
		}

		// get the highest fail margin
		*failMargin = -99;
		for (i=0 ; i<4 ; i++)
		{
			if (sectionMargin[i]>*failMargin)
			{
				*failMargin = sectionMargin[i];
				*leastMarginCarrier = sectionCarrierNo[i];
			}
		}

		// Get  LoLeakage
		*loLeakage = LP_GetScalarMeasurement("dcLeakageDbc");
	}
	else
	{
		iRet = -1;
	}

	return iRet;
}

int  LoResultSpectralFlatness11N_byAPI(int basedOnVsa,
								 int wifiMode,
								 int* carrierNoUpA,
								 int* carrierNoUpB,
								 int* carrierNoLoA,
								 int* carrierNoLoB,
								 double* failMargin,
								 int* leastMarginCarrier,
								 double* loLeakage,
								 double* valueDbUpSectionA,
								 double* valueDbUpSectionB,
								 double* valueDbLoSectionA,
								 double* valueDbLoSectionB,
								 int streamIndex)
{
	int	iRet = ERR_OK;

	int	carrierFirst;
	int	carrierSecond;
	int	carrierThird;
	int	carrierFourth;
	int	indexOfSubcarrier_0;

	int	numberOfStream = 0;
    int	vsaIndex = 0;
	int   bandwidthMhz;
	/* for non-analysis cas, initialize margin and loLeakage to NaN*/
	*failMargin    = NA_NUMBER;
	*loLeakage = NA_NUMBER;

	*carrierNoUpA = NA_INTEGER;
	*carrierNoUpB = NA_INTEGER;
	*carrierNoLoA = NA_INTEGER;
	*carrierNoLoB = NA_INTEGER;

	/* for non-analysis cas, initialize valueDbUpSectionA/B and valudDbLoSectionA/B to NaN*/
	*valueDbUpSectionA = NA_NUMBER;
	*valueDbUpSectionB = NA_NUMBER;
	*valueDbLoSectionA = NA_NUMBER;
	*valueDbLoSectionB = NA_NUMBER;

	vsaIndex = streamIndex;
	numberOfStream = (int)::LP_GetScalarMeasurement("rateInfo_spatialStreams", 0);
    if ((streamIndex >= numberOfStream) && !basedOnVsa)
    {
        return 0;	// special case, need return, but not error.
    }
	//for specify order analysis, capture valid streams, validVsaNum must be <= stream Nums
    //if(!strcmp(l_txVerifyFlatnessParam.TX_ANALYSIS_ORDER,"-1,-1,-1,-1"))
	//{
	//	//not specify order analysis
	//}
	//else
	//{
	//	if(vsaIndex >= numberOfStream)
	//	{
	//		return 0; // special case, need return, but not error.
	//	}
	//	else
	//	{
	//		//valid streams on valid VSA
	//	}
	//}

	bandwidthMhz = (int)::LP_GetScalarMeasurement("rateInfo_bandwidthMhz", 0); 
	if ( (wifiMode==WIFI_11N_MF_HT20 || wifiMode==WIFI_11N_GF_HT20)&&(bandwidthMhz!=20) )
	{
		return -1;
	}
	else if ( (wifiMode==WIFI_11N_MF_HT40 || wifiMode==WIFI_11N_GF_HT40)&&(bandwidthMhz!=40) )
	{
		return -1;
	}
	else
	{
		// IEEE std 802.11n Transmitter flatness
		// HT20	
		// In a 20 MHz channel and in corresponding 20 MHz transmission in a 40 MHz channel, the average energy
		// of the constellations in each of the spectral lines ¡V16 to ¡V1 and +1 to +16 shall deviate no more than ¡Ó 2 dB
		// from their average energy. The average energy of the constellations in each of the spectral lines ¡V28 to ¡V17
		// and +17 to +28 shall deviate no more than +2/¡V4 dB from the average energy of spectral lines ¡V16 to ¡V1 and +1 to +16.

		// HT40
		// In a 40 MHz transmission (excluding HT duplicate format and non-HT duplicate format) the average energy
		// of the constellations in each of the spectral lines ¡V42 to ¡V2 and +2 to +42 shall deviate no more than ¡Ó 2 dB
		// from their average energy. The average energy of the constellations in each of the spectral lines ¡V43 to ¡V58
		// and +43 to +58 shall deviate no more than +2/¡V4 dB from the average energy of spectral lines ¡V42 to ¡V2 and +2 to +42.

		// In HT duplicate format and non-HT duplicate format the average energy of the constellations in each of the
		// spectral lines -42 to -33, -31 to -6, +6 to +31, and +33 to +42 shall deviate no more than ¡Ó 2 dB from their
		// average energy. The average energy of the constellations in each of the spectral lines -43 to -58 and +43 to
		// +58 shall deviate no more than +2/-4 dB from the average energy of spectral lines -42 to -33, -31 to -6, +6
		// to +31, and +33 to +42.
		if (bandwidthMhz==20)
		{
			carrierFirst  = CARRIER_1;
			carrierSecond = CARRIER_16;
			carrierThird  = CARRIER_17;
			carrierFourth = CARRIER_28;
			indexOfSubcarrier_0 = INDEX_OF_SUBCARRIER_0_HT20;
		}
		else
		{
			carrierFirst  = CARRIER_2;
			carrierSecond = CARRIER_42;
			carrierThird  = CARRIER_43;
			carrierFourth = CARRIER_58;
			indexOfSubcarrier_0 = INDEX_OF_SUBCARRIER_0_HT40;
		}
	}

	double bufferFlatnessReal[MAX_BUFFER_SIZE], bufferFlatnessImag[MAX_BUFFER_SIZE];
	double bufferFlatnessUpLimitReal[MAX_BUFFER_SIZE], bufferFlatnessUpLimitImag[MAX_BUFFER_SIZE];
	double bufferFlatnessLowLimitReal[MAX_BUFFER_SIZE], bufferFlatnessLowLimitImag[MAX_BUFFER_SIZE];
	int    bufferSizeFlatness = ::LP_GetVectorMeasurement("spectralFlatness", bufferFlatnessReal, bufferFlatnessImag, MAX_BUFFER_SIZE);
	int    bufferSizeFlatnessUpLimit = ::LP_GetVectorMeasurement("spectralFlatnessHighLimit", bufferFlatnessUpLimitReal, bufferFlatnessUpLimitImag, MAX_BUFFER_SIZE);
	int    bufferSizeFlatnessLowLimit = ::LP_GetVectorMeasurement("spectralFlatnessLowLimit", bufferFlatnessLowLimitReal, bufferFlatnessLowLimitImag, MAX_BUFFER_SIZE);

	if (bufferSizeFlatness>0 && bufferSizeFlatnessUpLimit>0 && bufferSizeFlatnessLowLimit>0)
	{
		double sectionMargin[4] = {-99, -99, -99, -99};
		int sectionCarrierNo[4];
		int subcarrier_index, i;

		// Check the flatness for carrier section A
		for (i=carrierFirst; i<=carrierSecond; i++)
		{
			//UPPER SECTION A
			subcarrier_index = indexOfSubcarrier_0 + i;
			CheckFlatnessViolation(bufferFlatnessReal[subcarrier_index], i, bufferFlatnessLowLimitReal[subcarrier_index]
			, bufferFlatnessUpLimitReal[subcarrier_index], valueDbUpSectionA, carrierNoUpA, &sectionMargin[0], &sectionCarrierNo[0]);

			//LOWER SECTION A
			subcarrier_index = indexOfSubcarrier_0 - i;
			CheckFlatnessViolation(bufferFlatnessReal[subcarrier_index], -i, bufferFlatnessLowLimitReal[subcarrier_index]
			, bufferFlatnessUpLimitReal[subcarrier_index], valueDbLoSectionA, carrierNoLoA, &sectionMargin[1], &sectionCarrierNo[1]);
		}

		// Continue, Check the flatness section B
		for (i=carrierThird; i<=carrierFourth; i++)
		{
			//UPPER SECTION B
			subcarrier_index = indexOfSubcarrier_0 + i;
			CheckFlatnessViolation(bufferFlatnessReal[subcarrier_index], i, bufferFlatnessLowLimitReal[subcarrier_index]
			, bufferFlatnessUpLimitReal[subcarrier_index], valueDbUpSectionB, carrierNoUpB, &sectionMargin[2], &sectionCarrierNo[2]);

			//LOWER SECTION B
			subcarrier_index = indexOfSubcarrier_0 - i;
			CheckFlatnessViolation(bufferFlatnessReal[subcarrier_index], -i, bufferFlatnessLowLimitReal[subcarrier_index]
			, bufferFlatnessUpLimitReal[subcarrier_index], valueDbLoSectionB, carrierNoLoB, &sectionMargin[3], &sectionCarrierNo[3]);
		}

		// get the highest fail margin
		*failMargin = -99;
		for (i=0 ; i<4 ; i++)
		{
			if (sectionMargin[i]>*failMargin)
			{
				*failMargin = sectionMargin[i];
				*leastMarginCarrier = sectionCarrierNo[i];
			}
		}

		// Get  LoLeakage
		*loLeakage = LP_GetScalarMeasurement("dcLeakageDbc");
	}
	else
	{
		iRet = -1;
	}

	return iRet;
}

int  LoResultSpectralFlatness11AC_byAPI(int basedOnVsa,
													 int cbw,
													 int* carrierNoUpA,
													 int* carrierNoUpB,
													 int* carrierNoLoA,
													 int* carrierNoLoB,
													 double* failMargin,
													 int* leastMarginCarrier,
													 double* loLeakage,
													 double* valueDbUpSectionA,
													 double* valueDbUpSectionB,
													 double* valueDbLoSectionA,
													 double* valueDbLoSectionB,
													 int streamIndex)
{
	int	iRet = ERR_OK;

	int	carrierFirst;
	int	carrierSecond;
	int	carrierThird;
	int	carrierFourth;
	int	indexOfSubcarrier_0;

	int	numberOfStream = 0;
    int	vsaIndex = 0;
	int   bandwidthMhz;
	// Added for 802.11ac
	int		carrierPilot;
	int		carrierFifth, carrierSixth, carrierSeventh,	carrierEigth, carrierNinth,carrierTenth;  
	int		carrierNumSection1, carrierNumSection2;

	/* for non-analysis cas, initialize margin and loLeakage to NaN*/
	*failMargin    = NA_NUMBER;
	*loLeakage = NA_NUMBER;

	*carrierNoUpA = NA_INTEGER;
	*carrierNoUpB = NA_INTEGER;
	*carrierNoLoA = NA_INTEGER;
	*carrierNoLoB = NA_INTEGER;

	/* for non-analysis cas, initialize valueDbUpSectionA/B and valudDbLoSectionA/B to NaN*/
	*valueDbUpSectionA = NA_NUMBER;
	*valueDbUpSectionB = NA_NUMBER;
	*valueDbLoSectionA = NA_NUMBER;
	*valueDbLoSectionB = NA_NUMBER;

	vsaIndex = streamIndex;
	numberOfStream = (int)::LP_GetScalarMeasurement("rateInfo_spatialStreams", 0);
    if ((streamIndex >= numberOfStream) && !basedOnVsa)
    {
        return 0;	// special case, need return, but not error.
    }
	//for specify order analysis, capture valid streams, validVsaNum must be <= stream Nums
 //   if(!strcmp(l_txVerifyFlatnessParam.TX_ANALYSIS_ORDER,"-1,-1,-1,-1"))
	//{
	//	//not specify order analysis
	//}
	//else
	//{
	//	if(vsaIndex >= numberOfStream)
	//	{
	//		return 0; // special case, need return, but not error.
	//	}
	//	else
	//	{
	//		//valid streams on valid VSA
	//	}
	//}

	bandwidthMhz = (int)::LP_GetScalarMeasurement("rateInfo_bandwidthMhz", 0); 
	if ( ( cbw == BW_20MHZ) &&(bandwidthMhz!=20) )
	{
		return -1;
	}
	else if ( ( cbw == BW_40MHZ )&&(bandwidthMhz!=40) )
	{
		return -1;
	}
	else if ( ( cbw == BW_80MHZ )&&(bandwidthMhz!=80) )
	{
		return -1;
	}
	else if ( ( cbw == BW_160MHZ )&&(bandwidthMhz!=160) )
	{
		return -1;
	}
	else if (( cbw == BW_80_80MHZ ) &&(bandwidthMhz!=160) )     // BW_80_80MHZ not supported yet
	{
		return -1;
	}
	else     // bandwidthMHz is correct
	{
		//if ( 0 != strcmp(l_txVerifyFlatnessParam.PACKET_FORMAT,PACKET_FORMAT_NON_HT))    //802.11ac VHT, MF_HT, GF_HT
		//{
			if ( bandwidthMhz == 20)
			{
				carrierFirst  = CARRIER_1;
				carrierSecond = CARRIER_16;
				carrierThird  = CARRIER_17;
				carrierFourth = CARRIER_28;
				indexOfSubcarrier_0 = INDEX_OF_SUBCARRIER_0_HT20;
			}
			else if (bandwidthMhz == 40)
			{
				carrierFirst  = CARRIER_2;
				carrierSecond = CARRIER_42;
				carrierThird  = CARRIER_43;
				carrierFourth = CARRIER_58;
				indexOfSubcarrier_0 = INDEX_OF_SUBCARRIER_0_HT40;
			}
			else if (bandwidthMhz == 80)
			{
				carrierFirst  = CARRIER_2;
				carrierSecond = CARRIER_84;
				carrierThird  = CARRIER_85;
				carrierFourth = CARRIER_122;
				indexOfSubcarrier_0 = INDEX_OF_SUBCARRIER_0_HT80;
			}
			else if (bandwidthMhz == 160)
			{
				// Not supported yet
				return 0;
			}
			else   // 80_80
			{
				// Not supported yet
				return 0;
			}
		//}
		//else   //802.11ac non-HT duplicate
		//{
		//	if ( bandwidthMhz == 40)
		//	{
		//		carrierFirst  = CARRIER_6;
		//		carrierSecond = CARRIER_42;
		//		carrierPilot = CARRIER_32;
		//		carrierThird  = CARRIER_43;
		//		carrierFourth = CARRIER_58;
		//		indexOfSubcarrier_0 = INDEX_OF_SUBCARRIER_0_HT40;
		//		//	carrierSecond = CARRIER_31;
		//		//	carrierThird  = CARRIER_33;
		//		//	carrierFourth = CARRIER_42;
		//		//	carrierFifth  = CARRIER_43;
		//		//	carrierSixth  = CARRIER_58;

		//	}
		//	else if (bandwidthMhz == 80)  // bandwidth = 80
		//	{
		//		carrierFirst	= CARRIER_6;
		//		carrierSecond	= CARRIER_31;
		//		carrierThird	= CARRIER_33;
		//		carrierFourth	= CARRIER_58;
		//		carrierFifth	= CARRIER_70;
		//		carrierSixth	= CARRIER_84;
		//		carrierNumSection1 = (carrierSecond - carrierFirst + 1) +
		//			(carrierFourth - carrierThird + 1) +
		//			(carrierSixth - carrierFifth + 1);

		//		carrierSeventh  = CARRIER_85;
		//		carrierEigth	= CARRIER_95;
		//		carrierNinth	= CARRIER_97;
		//		carrierTenth	= CARRIER_122;
		//		carrierNumSection2 = ( carrierEigth - carrierSeventh + 1) +
		//			( carrierTenth - carrierNinth +1);

		//		indexOfSubcarrier_0 = INDEX_OF_SUBCARRIER_0_HT80;
		//	}
		//	else if ( bandwidthMhz == 160)
		//	{
		//		// Not supported yet
		//	}
		//	else		// bandwidth == 80_80
		//	{
		//		// Not supported yet
		//	}
		//}
	}

	double bufferFlatnessReal[MAX_BUFFER_SIZE], bufferFlatnessImag[MAX_BUFFER_SIZE];
	double bufferFlatnessUpLimitReal[MAX_BUFFER_SIZE], bufferFlatnessUpLimitImag[MAX_BUFFER_SIZE];
	double bufferFlatnessLowLimitReal[MAX_BUFFER_SIZE], bufferFlatnessLowLimitImag[MAX_BUFFER_SIZE];
	int    bufferSizeFlatness = ::LP_GetVectorMeasurement("spectralFlatness", bufferFlatnessReal, bufferFlatnessImag, MAX_BUFFER_SIZE);
	int    bufferSizeFlatnessUpLimit = ::LP_GetVectorMeasurement("spectralFlatnessHighLimit", bufferFlatnessUpLimitReal, bufferFlatnessUpLimitImag, MAX_BUFFER_SIZE);
	int    bufferSizeFlatnessLowLimit = ::LP_GetVectorMeasurement("spectralFlatnessLowLimit", bufferFlatnessLowLimitReal, bufferFlatnessLowLimitImag, MAX_BUFFER_SIZE);

	if (bufferSizeFlatness>0 && bufferSizeFlatnessUpLimit>0 && bufferSizeFlatnessLowLimit>0)
	{
		double sectionMargin[4] = {-99, -99, -99, -99};
		int sectionCarrierNo[4];
		int subcarrier_index, i;

		//if ( 0 != strcmp(l_txVerifyFlatnessParam.PACKET_FORMAT,PACKET_FORMAT_NON_HT) && bandwidthMhz != 160)    //802.11ac VHT, MF_HT, GF_HT, BW = 20,40,80, 80_80
		//{

			// Check the flatness for carrier section A
			for (i=carrierFirst; i<=carrierSecond; i++)
			{
				//UPPER SECTION A
				subcarrier_index = indexOfSubcarrier_0 + i;
				CheckFlatnessViolation(bufferFlatnessReal[subcarrier_index], i, bufferFlatnessLowLimitReal[subcarrier_index]
				, bufferFlatnessUpLimitReal[subcarrier_index], valueDbUpSectionA, carrierNoUpA, &sectionMargin[0], &sectionCarrierNo[0]);

				//LOWER SECTION A
				subcarrier_index = indexOfSubcarrier_0 - i;
				CheckFlatnessViolation(bufferFlatnessReal[subcarrier_index], -i, bufferFlatnessLowLimitReal[subcarrier_index]
				, bufferFlatnessUpLimitReal[subcarrier_index], valueDbLoSectionA, carrierNoLoA, &sectionMargin[1], &sectionCarrierNo[1]);
			}

			// Continue, Check the flatness section B
			for (i=carrierThird; i<=carrierFourth; i++)
			{
				//UPPER SECTION B
				subcarrier_index = indexOfSubcarrier_0 + i;
				CheckFlatnessViolation(bufferFlatnessReal[subcarrier_index], i, bufferFlatnessLowLimitReal[subcarrier_index]
				, bufferFlatnessUpLimitReal[subcarrier_index], valueDbUpSectionB, carrierNoUpB, &sectionMargin[2], &sectionCarrierNo[2]);

				//LOWER SECTION B
				subcarrier_index = indexOfSubcarrier_0 - i;
				CheckFlatnessViolation(bufferFlatnessReal[subcarrier_index], -i, bufferFlatnessLowLimitReal[subcarrier_index]
				, bufferFlatnessUpLimitReal[subcarrier_index], valueDbLoSectionB, carrierNoLoB, &sectionMargin[3], &sectionCarrierNo[3]);
			}
		//}
		//else if ( 0 != strcmp(l_txVerifyFlatnessParam.PACKET_FORMAT,PACKET_FORMAT_NON_HT) && bandwidthMhz == 160) 
		//{
		//	// BW = 160MHz, not supported yet
		//}
		//else     //802.11ac non-HT
		//{
		//	if ( bandwidthMhz == 40)
		//	{
		//		// Check the flatness for carrier section A
		//		for (i=carrierFirst; i<=carrierSecond; i++)
		//		{
		//			if ( i != carrierPilot)
		//			{
		//				//UPPER SECTION A
		//				subcarrier_index = indexOfSubcarrier_0 + i;
		//				CheckFlatnessViolation(bufferFlatnessReal[subcarrier_index], i, bufferFlatnessLowLimitReal[subcarrier_index]
		//				, bufferFlatnessUpLimitReal[subcarrier_index], valueDbUpSectionA, carrierNoUpA, &sectionMargin[0], &sectionCarrierNo[0]);

		//				//LOWER SECTION A
		//				subcarrier_index = indexOfSubcarrier_0 - i;
		//				CheckFlatnessViolation(bufferFlatnessReal[subcarrier_index], -i, bufferFlatnessLowLimitReal[subcarrier_index]
		//				, bufferFlatnessUpLimitReal[subcarrier_index], valueDbLoSectionA, carrierNoLoA, &sectionMargin[1], &sectionCarrierNo[1]);
		//			}
		//			else
		//			{
		//				// do nothing
		//			}
		//		}

		//		// Continue, Check the flatness section B
		//		for (i=carrierThird; i<=carrierFourth; i++)
		//		{
		//			//UPPER SECTION B
		//			subcarrier_index = indexOfSubcarrier_0 + i;
		//			CheckFlatnessViolation(bufferFlatnessReal[subcarrier_index], i, bufferFlatnessLowLimitReal[subcarrier_index]
		//			, bufferFlatnessUpLimitReal[subcarrier_index], valueDbUpSectionB, carrierNoUpB, &sectionMargin[2], &sectionCarrierNo[2]);

		//			//LOWER SECTION B
		//			subcarrier_index = indexOfSubcarrier_0 - i;
		//			CheckFlatnessViolation(bufferFlatnessReal[subcarrier_index], -i, bufferFlatnessLowLimitReal[subcarrier_index]
		//			, bufferFlatnessUpLimitReal[subcarrier_index], valueDbLoSectionB, carrierNoLoB, &sectionMargin[3], &sectionCarrierNo[3]);
		//		}
		//	}
		//	else if ( bandwidthMhz == 80) 
		//	{
		//		// Check the flatness for carrier section A
		//		for (i=carrierFirst; i<=carrierSecond; i++)
		//		{
		//			//UPPER SECTION A
		//			subcarrier_index = indexOfSubcarrier_0 + i;
		//			CheckFlatnessViolation(bufferFlatnessReal[subcarrier_index], i, bufferFlatnessLowLimitReal[subcarrier_index]
		//			, bufferFlatnessUpLimitReal[subcarrier_index], valueDbUpSectionA, carrierNoUpA, &sectionMargin[0], &sectionCarrierNo[0]);

		//			//LOWER SECTION A
		//			subcarrier_index = indexOfSubcarrier_0 - i;
		//			CheckFlatnessViolation(bufferFlatnessReal[subcarrier_index], -i, bufferFlatnessLowLimitReal[subcarrier_index]
		//			, bufferFlatnessUpLimitReal[subcarrier_index], valueDbLoSectionA, carrierNoLoA, &sectionMargin[1], &sectionCarrierNo[1]);
		//		}
		//		for (i=carrierThird; i<=carrierFourth; i++)
		//		{
		//			//UPPER SECTION A
		//			subcarrier_index = indexOfSubcarrier_0 + i;
		//			CheckFlatnessViolation(bufferFlatnessReal[subcarrier_index], i, bufferFlatnessLowLimitReal[subcarrier_index]
		//			, bufferFlatnessUpLimitReal[subcarrier_index], valueDbUpSectionA, carrierNoUpA, &sectionMargin[0], &sectionCarrierNo[0]);

		//			//LOWER SECTION A
		//			subcarrier_index = indexOfSubcarrier_0 - i;
		//			CheckFlatnessViolation(bufferFlatnessReal[subcarrier_index], -i, bufferFlatnessLowLimitReal[subcarrier_index]
		//			, bufferFlatnessUpLimitReal[subcarrier_index], valueDbLoSectionA, carrierNoLoA, &sectionMargin[1], &sectionCarrierNo[1]);
		//		}
		//		for (i=carrierSixth; i<=carrierFifth; i++)
		//		{
		//			//UPPER SECTION A
		//			subcarrier_index = indexOfSubcarrier_0 + i;
		//			CheckFlatnessViolation(bufferFlatnessReal[subcarrier_index], i, bufferFlatnessLowLimitReal[subcarrier_index]
		//			, bufferFlatnessUpLimitReal[subcarrier_index], valueDbUpSectionA, carrierNoUpA, &sectionMargin[0], &sectionCarrierNo[0]);

		//			//LOWER SECTION A
		//			subcarrier_index = indexOfSubcarrier_0 - i;
		//			CheckFlatnessViolation(bufferFlatnessReal[subcarrier_index], -i, bufferFlatnessLowLimitReal[subcarrier_index]
		//			, bufferFlatnessUpLimitReal[subcarrier_index], valueDbLoSectionA, carrierNoLoA, &sectionMargin[1], &sectionCarrierNo[1]);
		//		}

		//		// Continue, Check the flatness. (side)
		//		for (i=carrierSeventh; i<=carrierEigth; i++)
		//		{
		//			//UPPER SECTION B
		//			subcarrier_index = indexOfSubcarrier_0 + i;
		//			CheckFlatnessViolation(bufferFlatnessReal[subcarrier_index], i, bufferFlatnessLowLimitReal[subcarrier_index]
		//			, bufferFlatnessUpLimitReal[subcarrier_index], valueDbUpSectionB, carrierNoUpB, &sectionMargin[2], &sectionCarrierNo[2]);

		//			//LOWER SECTION B
		//			subcarrier_index = indexOfSubcarrier_0 - i;
		//			CheckFlatnessViolation(bufferFlatnessReal[subcarrier_index], -i, bufferFlatnessLowLimitReal[subcarrier_index]
		//			, bufferFlatnessUpLimitReal[subcarrier_index], valueDbLoSectionB, carrierNoLoB, &sectionMargin[3], &sectionCarrierNo[3]);
		//		}
		//		for (i=carrierNinth; i<=carrierTenth; i++)
		//		{
		//			//UPPER SECTION B
		//			subcarrier_index = indexOfSubcarrier_0 + i;
		//			CheckFlatnessViolation(bufferFlatnessReal[subcarrier_index], i, bufferFlatnessLowLimitReal[subcarrier_index]
		//			, bufferFlatnessUpLimitReal[subcarrier_index], valueDbUpSectionB, carrierNoUpB, &sectionMargin[2], &sectionCarrierNo[2]);

		//			//LOWER SECTION B
		//			subcarrier_index = indexOfSubcarrier_0 - i;
		//			CheckFlatnessViolation(bufferFlatnessReal[subcarrier_index], -i, bufferFlatnessLowLimitReal[subcarrier_index]
		//			, bufferFlatnessUpLimitReal[subcarrier_index], valueDbLoSectionB, carrierNoLoB, &sectionMargin[3], &sectionCarrierNo[3]);
		//		}
		//	}
		//	else if ( bandwidthMhz == 160)
		//	{
		//		// not supported yet
		//	}
		//	else	// bandwidth = 80_80
		//	{
		//		// not supported yet
		//	}

		//}
		// get the highest fail margin
		*failMargin = -99;
		for (i=0 ; i<4 ; i++)
		{
			if (sectionMargin[i]>*failMargin)
			{
				*failMargin = sectionMargin[i];
				*leastMarginCarrier = sectionCarrierNo[i];
			}
		}

		// Get  LoLeakage
		*loLeakage = LP_GetScalarMeasurement("dcLeakageDbc");
	}
	else
	{
		iRet = -1;
	}

	return iRet;
}

void defineSpectrumMaskLimit()
{
	DEFAULT_MASK_STRUCT dummyStruct;

	/*----------------------*
	 *  Construct the Mask  *
	 *----------------------*/
	// Default Mask of 802.11b
	dummyStruct.freqOffset   = 0;
	dummyStruct.deltaMaskPwr = 0;
	l_wifiMask_11b.push_back(dummyStruct);
	dummyStruct.freqOffset   = 11;
	dummyStruct.deltaMaskPwr = 0;
	l_wifiMask_11b.push_back(dummyStruct);
	dummyStruct.freqOffset   = 11;
	dummyStruct.deltaMaskPwr = -30;
	l_wifiMask_11b.push_back(dummyStruct);
	dummyStruct.freqOffset   = 22;
	dummyStruct.deltaMaskPwr = -30;
	l_wifiMask_11b.push_back(dummyStruct);
	dummyStruct.freqOffset   = 22;
	dummyStruct.deltaMaskPwr = -50;
	l_wifiMask_11b.push_back(dummyStruct);

	// Default Mask of 802.11ag
	dummyStruct.freqOffset   = 0;
	dummyStruct.deltaMaskPwr = 0;
	l_wifiMask_11ag.push_back(dummyStruct);
	dummyStruct.freqOffset   = 9;
	dummyStruct.deltaMaskPwr = 0;
	l_wifiMask_11ag.push_back(dummyStruct);
	dummyStruct.freqOffset   = 11;
	dummyStruct.deltaMaskPwr = -20;
	l_wifiMask_11ag.push_back(dummyStruct);
	dummyStruct.freqOffset   = 20;
	dummyStruct.deltaMaskPwr = -28;
	l_wifiMask_11ag.push_back(dummyStruct);
	dummyStruct.freqOffset   = 30;
	dummyStruct.deltaMaskPwr = -40;
	l_wifiMask_11ag.push_back(dummyStruct);

	// Default Mask of 802.11n HT20 on 2.4G
	dummyStruct.freqOffset   = 0;
	dummyStruct.deltaMaskPwr = 0;
	l_wifiMask_11nHt20_2G.push_back(dummyStruct);
	dummyStruct.freqOffset   = 9;
	dummyStruct.deltaMaskPwr = 0;
	l_wifiMask_11nHt20_2G.push_back(dummyStruct);
	dummyStruct.freqOffset   = 11;
	dummyStruct.deltaMaskPwr = -20;
	l_wifiMask_11nHt20_2G.push_back(dummyStruct);
	dummyStruct.freqOffset   = 20;
	dummyStruct.deltaMaskPwr = -28;
	l_wifiMask_11nHt20_2G.push_back(dummyStruct);
	dummyStruct.freqOffset   = 30;
	dummyStruct.deltaMaskPwr = -45;
	l_wifiMask_11nHt20_2G.push_back(dummyStruct);

	// Default Mask of 802.11n HT20 on 5G
	dummyStruct.freqOffset   = 0;
	dummyStruct.deltaMaskPwr = 0;
	l_wifiMask_11nHt20_5G.push_back(dummyStruct);
	dummyStruct.freqOffset   = 9;
	dummyStruct.deltaMaskPwr = 0;
	l_wifiMask_11nHt20_5G.push_back(dummyStruct);
	dummyStruct.freqOffset   = 11;
	dummyStruct.deltaMaskPwr = -20;
	l_wifiMask_11nHt20_5G.push_back(dummyStruct);
	dummyStruct.freqOffset   = 20;
	dummyStruct.deltaMaskPwr = -28;
	l_wifiMask_11nHt20_5G.push_back(dummyStruct);
	dummyStruct.freqOffset   = 30;
	dummyStruct.deltaMaskPwr = -40; //(IEEE new spec, 2012)
	l_wifiMask_11nHt20_5G.push_back(dummyStruct);

	// Default Mask of 802.11n HT40 on 2.4G
	dummyStruct.freqOffset   = 0;
	dummyStruct.deltaMaskPwr = 0;
	l_wifiMask_11nHt40_2G.push_back(dummyStruct);
	dummyStruct.freqOffset   = 19;
	dummyStruct.deltaMaskPwr = 0;
	l_wifiMask_11nHt40_2G.push_back(dummyStruct);
	dummyStruct.freqOffset   = 21;
	dummyStruct.deltaMaskPwr = -20;
	l_wifiMask_11nHt40_2G.push_back(dummyStruct);
	dummyStruct.freqOffset   = 40;
	dummyStruct.deltaMaskPwr = -28;
	l_wifiMask_11nHt40_2G.push_back(dummyStruct);
	dummyStruct.freqOffset   = 60;
	dummyStruct.deltaMaskPwr = -45;
	l_wifiMask_11nHt40_2G.push_back(dummyStruct);

	// Default Mask of 802.11n HT40 on 5G
	dummyStruct.freqOffset   = 0;
	dummyStruct.deltaMaskPwr = 0;
	l_wifiMask_11nHt40_5G.push_back(dummyStruct);
	dummyStruct.freqOffset   = 19;
	dummyStruct.deltaMaskPwr = 0;
	l_wifiMask_11nHt40_5G.push_back(dummyStruct);
	dummyStruct.freqOffset   = 21;
	dummyStruct.deltaMaskPwr = -20;
	l_wifiMask_11nHt40_5G.push_back(dummyStruct);
	dummyStruct.freqOffset   = 40;
	dummyStruct.deltaMaskPwr = -28;
	l_wifiMask_11nHt40_5G.push_back(dummyStruct);
	dummyStruct.freqOffset   = 60;
	dummyStruct.deltaMaskPwr = -40;//(IEEE new spec, 2012)
	l_wifiMask_11nHt40_5G.push_back(dummyStruct);

	//Default Mask of 802.11ac BSS20MHz
	dummyStruct.freqOffset   = 0;
	dummyStruct.deltaMaskPwr = 0;
	l_wifiMask_11acBss20.push_back(dummyStruct);
	dummyStruct.freqOffset   = 9;
	dummyStruct.deltaMaskPwr = 0;
	l_wifiMask_11acBss20.push_back(dummyStruct);
	dummyStruct.freqOffset   = 11;
	dummyStruct.deltaMaskPwr = -20;
	l_wifiMask_11acBss20.push_back(dummyStruct);
	dummyStruct.freqOffset   = 20;
	dummyStruct.deltaMaskPwr = -28;
	l_wifiMask_11acBss20.push_back(dummyStruct);
	dummyStruct.freqOffset   = 30;
	dummyStruct.deltaMaskPwr = -40;
	l_wifiMask_11acBss20.push_back(dummyStruct);

	//Default Mask of 802.11ac BSS40MHz
	dummyStruct.freqOffset   = 0;
	dummyStruct.deltaMaskPwr = 0;
	l_wifiMask_11acBss40.push_back(dummyStruct);
	dummyStruct.freqOffset   = 19;
	dummyStruct.deltaMaskPwr = 0;
	l_wifiMask_11acBss40.push_back(dummyStruct);
	dummyStruct.freqOffset   = 21;
	dummyStruct.deltaMaskPwr = -20;
	l_wifiMask_11acBss40.push_back(dummyStruct);
	dummyStruct.freqOffset   = 40;
	dummyStruct.deltaMaskPwr = -28;
	l_wifiMask_11acBss40.push_back(dummyStruct);
	dummyStruct.freqOffset   = 60;
	dummyStruct.deltaMaskPwr = -40;
	l_wifiMask_11acBss40.push_back(dummyStruct);

	//Default Mask of 802.11ac BSS80MHz
	dummyStruct.freqOffset   = 0;
	dummyStruct.deltaMaskPwr = 0;
	l_wifiMask_11acBss80.push_back(dummyStruct);
	dummyStruct.freqOffset   = 39;
	dummyStruct.deltaMaskPwr = 0;
	l_wifiMask_11acBss80.push_back(dummyStruct);
	dummyStruct.freqOffset   = 41;
	dummyStruct.deltaMaskPwr = -20;
	l_wifiMask_11acBss80.push_back(dummyStruct);
	dummyStruct.freqOffset   = 80;
	dummyStruct.deltaMaskPwr = -28;
	l_wifiMask_11acBss80.push_back(dummyStruct);
	dummyStruct.freqOffset   = 120;
	dummyStruct.deltaMaskPwr = -40;
	l_wifiMask_11acBss80.push_back(dummyStruct);


	//Default Mask of 802.11ac BSS160MHz
	dummyStruct.freqOffset   = 0;
	dummyStruct.deltaMaskPwr = 0;
	l_wifiMask_11acBss160.push_back(dummyStruct);
	dummyStruct.freqOffset   = 79;
	dummyStruct.deltaMaskPwr = 0;
	l_wifiMask_11acBss160.push_back(dummyStruct);
	dummyStruct.freqOffset   = 81;
	dummyStruct.deltaMaskPwr = -20;
	l_wifiMask_11acBss160.push_back(dummyStruct);
	dummyStruct.freqOffset   = 160;
	dummyStruct.deltaMaskPwr = -28;
	l_wifiMask_11acBss160.push_back(dummyStruct);
	dummyStruct.freqOffset   = 240;
	dummyStruct.deltaMaskPwr = -40;
	l_wifiMask_11acBss160.push_back(dummyStruct);

	//Default Mask of 802.11ac BSS80_80MHz. Not final, need to discuss about it later.
	dummyStruct.freqOffset   = 0;
	dummyStruct.deltaMaskPwr = -25;
	l_wifiMask_11acBss80_80.push_back(dummyStruct);
	dummyStruct.freqOffset   = 39;
	dummyStruct.deltaMaskPwr = 20;
	l_wifiMask_11acBss80_80.push_back(dummyStruct);
	dummyStruct.freqOffset   = 41;
	dummyStruct.deltaMaskPwr = 0;
	l_wifiMask_11acBss80_80.push_back(dummyStruct);
	dummyStruct.freqOffset   = 119;
	dummyStruct.deltaMaskPwr = 0;
	l_wifiMask_11acBss80_80.push_back(dummyStruct);
	dummyStruct.freqOffset   = 121;
	dummyStruct.deltaMaskPwr = -20;
	l_wifiMask_11acBss80_80.push_back(dummyStruct);
	dummyStruct.freqOffset   = 160;
	dummyStruct.deltaMaskPwr = -28;
	l_wifiMask_11acBss80_80.push_back(dummyStruct);
	dummyStruct.freqOffset   = 200;
	dummyStruct.deltaMaskPwr = -40;
	l_wifiMask_11acBss80_80.push_back(dummyStruct);


}

double iq_interpolate (double y_max, double y_min, double x_max, double x_min, double x_val)
//=============================================================================
//	Description:
//		A generic function for interpolating. Find y_val when x_val is present.
//
//                             (y_max - y_min)
//    Returns y_val = y_min + (--------------- * (x_val - x_min))
//                             (x_max - x_min)
//=============================================================================
{
        return (y_min + ((y_max - y_min) / (x_max - x_min) * (x_val - x_min)));
}

bool calu_spectral_mask_spec(	int points, double max_power, double freqs_mhz[],
                                int offset_num, double offsets_mhz[], double levels_dbr[],
                                double spec_trace_dbr[] )
{
// ****************************************************************************
//	This method will build a spec array for every point, and return the array.
// ****************************************************************************
	bool error=false;
	int i, j;
	double *spec_mhz, *spec_dbc;
	
	spec_mhz	= new double [ offset_num + 2 ];
	spec_dbc	= new double [ offset_num + 2 ];
	
	spec_mhz[0] = 0 ;
	spec_dbc[0] = levels_dbr[0] ;
	for ( i=1, j=0; i<=offset_num ; i++ ,j++ )
	{
		if ( offsets_mhz[j] == 0 )
			j-- ;
		spec_mhz[i] = offsets_mhz[j];
		spec_dbc[i] = levels_dbr [j];
	}
	spec_mhz[offset_num+1] = 99e3 ;
	spec_dbc[offset_num+1] = levels_dbr[j-1] ;
	
	int pt, rv, seg;
	double y_max=0, y_min=0, x_max=0, x_min=0, freq;

	for ( pt=0, rv=points-1 ; pt < (points/2) ; pt++, rv-- )
	{
		freq = freqs_mhz[rv];
		for ( seg=0 ; seg < (offset_num+1) ; seg++ )
		{
			if ( (freq >= spec_mhz[seg]) && (freq < spec_mhz[seg+1]) )
			{
				x_max = spec_mhz[seg+1];	// stop  freq
				x_min = spec_mhz[seg];		// start freq
				y_max = spec_dbc[seg+1];	// stop  dbr
				y_min = spec_dbc[seg];		// start dbr
			}
		}
		spec_trace_dbr[rv] = iq_interpolate ( y_max, y_min, x_max, x_min, freq ) + max_power ;
		spec_trace_dbr[pt] = spec_trace_dbr[rv];		// mirror image
	}
	
	free (spec_mhz);
	free (spec_dbc);
	
	return (error);
}

//=============================================================================
//	Description:
//		A generic function for interpolating. Find y_val when xVal is present.
//
//                             (yMax - yMin)
//    Returns y_val = yMin + (--------------- * (xVal - xMin))
//                             (xMax - xMin)
//=============================================================================
inline double IQInterpolate( double yMax, double yMin, double xMax, double xMin, double xVal )
{
	return (yMin + ((yMax - yMin) / (xMax - xMin) * (xVal - xMin)));
}

int testSpectrumMask(double *freqBuf, double *maskBuf, int maskBufCnt, WIFI_MODE wifiMode, double *maskWiFi, double *vio, int chFreqMHz)
{
	int passTest = ERR_OK;

	// Mask Test Result
	double MARGIN_DB_POSITIVE[MAX_POSITIVE_SECTION];		/*!< Margin to the mask, normally should be a negative number, if pass. */
	double MARGIN_DB_NEGATIVE[MAX_NEGATIVE_SECTION];		/*!< Margin to the mask, normally should be a negative number, if pass */
	double FREQ_AT_MARGIN_POSITIVE[MAX_POSITIVE_SECTION];	/*!< Point out the frequency offset at margin to the mask */
	double FREQ_AT_MARGIN_NEGATIVE[MAX_NEGATIVE_SECTION];	/*!< Point out the frequency offset at margin to the mask */
	double VIOLATION_PERCENT;								/*!< Percentage which fail the mask */

	//int    maskTopStart = 999999, maskTopStop = 0;
	double peakPower = NA_NUMBER;

	double firstRefFreq =  freqBuf[0]/1000000;				// to MHz
	double deltaFreq    = (freqBuf[1]-freqBuf[0])/1000000;	// to MHz

	vector<DEFAULT_MASK_STRUCT>::iterator maskBegin_Iter;
	vector<DEFAULT_MASK_STRUCT>::iterator maskEnd_Iter;
	vector<DEFAULT_MASK_STRUCT>::iterator dummy_Iter;

	// Modified for IEEE new spec of mask test on different band, 2012/10/08
	if(chFreqMHz < 2500)
	{
		l_wifiMask_11nHt20 = &l_wifiMask_11nHt20_2G;
		l_wifiMask_11nHt40 = &l_wifiMask_11nHt40_2G;
	}else
	{
		l_wifiMask_11nHt20 = &l_wifiMask_11nHt20_5G;
		l_wifiMask_11nHt40 = &l_wifiMask_11nHt40_5G;
	}

	/*--------------------------*
	 *  Looking for peak power  *
	 *--------------------------*/
	for (int i=0;i<maskBufCnt;i++)
	{
		if ( maskBuf[i]>peakPower )
		{   // store the current peak power and keep searching
			peakPower = maskBuf[i];
		}
		else
		{
			// keep searching...
		}
	}

	/*-----------------------*
	 *  Select default Mask  *
	 *-----------------------*/
	if (wifiMode == WIFI_11B)    //11b
	{
		maskBegin_Iter = l_wifiMask_11b.begin();
		maskEnd_Iter = l_wifiMask_11b.end();
	}
	else if (wifiMode == WIFI_11AG)
	{
		maskBegin_Iter = l_wifiMask_11ag.begin();
		maskEnd_Iter = l_wifiMask_11ag.end();
	}
	else if ((wifiMode == WIFI_11N_MF_HT20) || (wifiMode == WIFI_11N_GF_HT20) || (wifiMode == WIFI_11N_HT20) )
	{
		maskBegin_Iter = l_wifiMask_11nHt20->begin();
		maskEnd_Iter = l_wifiMask_11nHt20->end();
	}
	else if ((wifiMode == WIFI_11N_MF_HT40) || (wifiMode == WIFI_11N_GF_HT40) || (wifiMode == WIFI_11N_HT40) )
	{
		maskBegin_Iter = l_wifiMask_11nHt40->begin();
		maskEnd_Iter = l_wifiMask_11nHt40->end();
	}
	else // 11ac
	{
		int tempBW;

		if (wifiMode == WIFI_11AC_VHT20)
			tempBW = BW_20MHZ;
		else if (wifiMode == WIFI_11AC_VHT40)
			tempBW = BW_40MHZ;
		else if (wifiMode == WIFI_11AC_VHT80)
			tempBW = BW_80MHZ;
		else if (wifiMode == WIFI_11AC_VHT160)
			tempBW = BW_160MHZ;
		else if (wifiMode == WIFI_11AC_VHT80_80)
			tempBW = BW_80_80MHZ;

		switch (tempBW)
		{
			case BW_20MHZ:
				maskBegin_Iter = l_wifiMask_11acBss20.begin();
				maskEnd_Iter = l_wifiMask_11acBss20.end();
				break;
			case BW_40MHZ:
				maskBegin_Iter = l_wifiMask_11acBss40.begin();
				maskEnd_Iter = l_wifiMask_11acBss40.end();
				break;
			case BW_80MHZ:
				maskBegin_Iter = l_wifiMask_11acBss80.begin();
				maskEnd_Iter = l_wifiMask_11acBss80.end();
				break;
			case BW_160MHZ:
				maskBegin_Iter = l_wifiMask_11acBss160.begin();
				maskEnd_Iter = l_wifiMask_11acBss160.end();
				break;
			case BW_80_80MHZ:
				maskBegin_Iter = l_wifiMask_11acBss80_80.begin();
				maskEnd_Iter = l_wifiMask_11acBss80_80.end();
				break;
			default:
				maskBegin_Iter = l_wifiMask_11acBss20.begin();
				maskEnd_Iter = l_wifiMask_11acBss20.end();
				break;
		}

	}

	/*-------------------------------*
	 *  Construct the absolute Mask  *
	 *-------------------------------*/
	int    sectionIndex = 0;
	double xVal = 0, yDeltaPwrdB = 0;

	// Initial the return results
	VIOLATION_PERCENT  = 0;
	for (int x=0;x<MAX_POSITIVE_SECTION;x++)
	{
		MARGIN_DB_POSITIVE[x]		= NA_NUMBER;
		FREQ_AT_MARGIN_POSITIVE[x]	= NA_NUMBER;
		MARGIN_DB_NEGATIVE[x]		= NA_NUMBER;
		FREQ_AT_MARGIN_NEGATIVE[x] = NA_NUMBER;
	}

	for (int i=0;i<maskBufCnt;i++)
	{
		xVal = fabs(freqBuf[i]/1000000.0);
		dummy_Iter = maskBegin_Iter;
		sectionIndex = 0;

		while ( dummy_Iter!=maskEnd_Iter ) // not end of mask vector
		{
			if ( xVal >= dummy_Iter->freqOffset )
			{
				dummy_Iter++;
				sectionIndex++;

				if ( dummy_Iter!=maskEnd_Iter ) // not end of mask vector
				{
					if ( xVal < dummy_Iter->freqOffset )    // find out the range
					{
						double xMax = dummy_Iter->freqOffset;
						double yMax = dummy_Iter->deltaMaskPwr;
						dummy_Iter--;
						sectionIndex--;
						double xMin = dummy_Iter->freqOffset;
						double yMin = dummy_Iter->deltaMaskPwr;

						yDeltaPwrdB = IQInterpolate( yMax, yMin, xMax, xMin, xVal );

						//if (dummy_Iter->deltaMaskPwr==0) // This is the top of spectrum
						//{
						//    if (i<maskTopStart) maskTopStart = i;
						//    if (i>maskTopStop)  maskTopStop  = i;
						//}
						//else
						//{
						//    // do nothing, not top
						//}
						break;
					}
					else
					{
						// not in this Mask range, continue and keep searching
						continue;
					}
				}
				else    // already end of vector, thus direct assign mask_Iter->deltaMaskPwr to yDeltaPwrdB
				{
					dummy_Iter--;
					sectionIndex--;
					yDeltaPwrdB = dummy_Iter->deltaMaskPwr;
					break;
				}
			}
			else
			{
				// no this case, because fabs(currentFreq) should >= 0
			}
		}

		// Construct the absolute Mask and store into Array
		maskWiFi[i] = peakPower + yDeltaPwrdB;

		/*---------------------------------*
		 *  Report Margin by each section  *
		 *---------------------------------*/
		if ( sectionIndex==0 )		// This is the "TOP" of spectral, just skip this section.
		{
			// do nothing, moving to next frequency step
		}
		else
		{
			double deltaPower = maskBuf[i] - maskWiFi[i];	// check Mask

			if ( i>(maskBufCnt/2) )		// Upper section
			{
				if ( deltaPower>MARGIN_DB_POSITIVE[sectionIndex-1] )	// need to save this as margin
				{
					MARGIN_DB_POSITIVE[sectionIndex-1]      = deltaPower;		// save margin
					//					FREQ_AT_MARGIN_POSITIVE[sectionIndex-1] = (freqBuf[i]/1e6) + l_11ACtxVerifyMaskParam.CH_FREQ_MHZ;	// save frequency
					FREQ_AT_MARGIN_POSITIVE[sectionIndex-1] = (freqBuf[i]/1e6);	// save frequency

				}
				else
				{
					// do nothing
				}
			}
			else						// Lower section
			{
				if ( deltaPower>MARGIN_DB_NEGATIVE[sectionIndex-1] )	// need to save this as margin
				{
					MARGIN_DB_NEGATIVE[sectionIndex-1]      = deltaPower;		// save margin
					//					FREQ_AT_MARGIN_NEGATIVE[sectionIndex-1] = (freqBuf[i]/1e6) + l_11ACtxVerifyMaskParam.CH_FREQ_MHZ;	// save frequency
					FREQ_AT_MARGIN_NEGATIVE[sectionIndex-1] = (freqBuf[i]/1e6);	// save frequency
				}
				else
				{
					// do nothing
				}
			}

			if ( deltaPower>0 )  // Mask Failed
			{
				VIOLATION_PERCENT++;
			}
			else    // Mask Pass
			{
				// keep compare...
			}
		}
	}   // end - for (int i=0;i<maskBufCnt;i++)

	// Percentage, which fail the mask
	*vio = (VIOLATION_PERCENT/maskBufCnt) * 100.0;

	if (*vio!=0)
	{
		passTest = -1;
	}
	else
	{
		passTest = ERR_OK;
	}


	return passTest;
}

double GetOBW(double *freqBuf, double *dataBuf, int dataBufCnt, double obwPercentage, int &startIndex, int &stopIndex)
{
	double  totalPower = 0;
	//double  obwPowerLimit = 0;
	double  obwPowerExceed = 0.0;
	double  sumPower = 0.0;

	double deltaFreq    = (freqBuf[1]-freqBuf[0])/1000000;	// to MHz

	// Step 1, calculate the total power
	for (int i=0;i<dataBufCnt;i++)
	{
		totalPower = totalPower + pow( 10,(dataBuf[i]/10) );
	}

	// Step 2, calculate the exceed part( >99%) by OBW percantage
	obwPowerExceed = totalPower * (1.0 - obwPercentage);
	obwPowerExceed /= 2.0;

	//Step 3, Set flag from left sideband and right sideband
	startIndex = 0;
	stopIndex  = dataBufCnt - 1;

	//Find the point in 99%, start from left sideband, increase frequency
	sumPower = pow( 10,(dataBuf[startIndex]/10) );
	while((sumPower <= obwPowerExceed) && (startIndex < (dataBufCnt/2)))
	{
		startIndex++;
		sumPower += pow( 10,(dataBuf[startIndex]/10) );
	}

	//Find the point in 99%, start from right sideband, decrease frequency
	sumPower = pow( 10,(dataBuf[stopIndex]/10) );
	while((sumPower <= obwPowerExceed) && (stopIndex > (dataBufCnt/2)))
	{
		stopIndex --;
		sumPower += pow( 10,(dataBuf[stopIndex]/10) );
	}

	return deltaFreq * (stopIndex - startIndex);
}

int CreateMemInTester(char *referenceFile, unsigned int *puiTotalLen, char ** ppcSendData, char *pcCmd)
{
	int err = ERR_OK;
	//char waveformFile[256];

	ifstream ifsSignalFile;
	ifsSignalFile.open (referenceFile, ios::binary);
	//ifsSignalFile.open (waveformFile, ios::binary);
	if ( ifsSignalFile.is_open() == false )
	{
		//sprintf_s(lastErr, MAX_LEN_ERR_TXT, "Error: Fail to open the file '%s'\n", referenceFile);
		return -1;
	}
	// get length of file:
	ifsSignalFile.seekg (0, ios::end);
	int length = ifsSignalFile.tellg();
	ifsSignalFile.seekg (0, ios::beg);

	// allocate memory:
	char *buffer = new char [length+1]; // +1 means the size of '\n'

	const int ciSize = 256;
	char cDigit[ciSize];
	char cHeader[ciSize];
	memset(cHeader, 0x0, ciSize);

	string sWaveformName = referenceFile;
	int iPos = (int)sWaveformName.find_last_of("\\");
	if( iPos == -1 )
		iPos = (int)sWaveformName.find_last_of("//");

	string sFileName = sWaveformName.substr(iPos+1, sWaveformName.size()-(iPos+1));

	//Put the file to tester's user folder.
	sFileName = "/user/" + sFileName;

	sprintf(cHeader, "SYS;MMEM:DATA \"%s\",", sFileName.c_str());

	strcpy(pcCmd, cHeader);

	*puiTotalLen = length;

	*ppcSendData = new char [*puiTotalLen];
	memset(*ppcSendData, 0x0, *puiTotalLen);

	// read data as a block:
	ifsSignalFile.read (buffer,length);
	ifsSignalFile.close();


	memcpy(*ppcSendData, buffer, length);

	delete [] buffer;
	return err;

}


