#ifndef _UTILITY_H
#define _UTILITY_H

typedef enum tagWifiMode
{
    WIFI_11B,            
    WIFI_11AG,
	WIFI_11N_HT20,
	WIFI_11N_HT40,
    WIFI_11N_MF_HT20,
	WIFI_11N_GF_HT20,
	WIFI_11N_MF_HT40,
    WIFI_11N_GF_HT40,
	WIFI_11AC_VHT20,
	WIFI_11AC_VHT40,
	WIFI_11AC_VHT80,
	WIFI_11AC_VHT160,
	WIFI_11AC_VHT80_80,
	WIFI_11AC_MF_HT20,
	WIFI_11AC_GF_HT20,
	WIFI_11AC_MF_HT40,
	WIFI_11AC_GF_HT40,
	WIFI_11AC_NON_HT
} WIFI_MODE;

typedef enum tagBandWidth
{
    BW_20MHZ,
	BW_40MHZ,
	BW_80MHZ,
	BW_160MHZ,
	BW_80_80MHZ,
	BW_NOT_SUPPORT
}vDUT_BANDWIDTH;

void defineSpectrumMaskLimit();

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
								  double* valueDbLoSectionB);

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
								 int streamIndex);

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
								  int streamIndex);

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
								 int streamIndex);

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
									   double* valueDbLoSectionB);

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
										 int streamIndex);

int testSpectrumMask(double *freqBuf, double *maskBuf, int maskBufCnt, WIFI_MODE wifiMode, double *maskWiFi, double *vio, int chFreqMHz = 2412);

double GetOBW(double *freqBuf, double *dataBuf, int dataBufCnt, double obwPercentage, int &startIndex, int &stopIndex);

int CreateMemInTester(char *referenceFile, unsigned int *puiTotalLen, char ** ppcSendData, char *pcCmd);

#endif  // __UTILITY_H
