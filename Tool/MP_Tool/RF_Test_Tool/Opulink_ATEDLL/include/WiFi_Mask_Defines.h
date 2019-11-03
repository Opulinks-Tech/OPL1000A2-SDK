#pragma once
#include <vector>

using namespace std;

enum WIFI_ERR_CODE
{
    ERR_MASK_OK,
    ERR_INVALID_MASK_TYPE
};

enum WIFI_MASK_TYPE
{
 WIFI_11B,
 WIFI_11AG,
 WIFI_11N_HT20,
 WIFI_11N_HT40,
 WIFI_USER_DEFINE_MASK_HT20,
 WIFI_USER_DEFINE_MASK_HT40
};


// Internal definition
typedef struct tagDefaultMask
{ 
    double  freqOffset;
    double  deltaMaskPwr;
} DEFAULT_MASK_STRUCT;

typedef struct maskFreqVsMargin_t
{
    double freqMHz;
    double margin;
} maskFreqVsMargin;

int VerifyPowerMask (double *freqBufHz, double *maskBuf, int maskBufCnt, enum WIFI_MASK_TYPE wifiMaskType, double centerFrequencyHz, double obwPercentage, double *violationPercentage, vector<maskFreqVsMargin> *maskFreqVsMarginLo, vector<maskFreqVsMargin> *maskFreqVsMarginHi, double *obwMhz, bool *isTestPass, double *OBW_FREQ_START_MHZ, double *OBW_FREQ_STOP_MHZ);
int VerifyObw(double *maskBuf, int maskBufCnt, double obwPercentage, int &startIndex, int &stopIndex);
