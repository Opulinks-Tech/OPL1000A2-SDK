#ifndef _IQAPIFM_H_
#define _IQAPIFM_H_

// ****************************************************************
// Date   : 3/24/2009
// ****************************************************************

#include "iqapiDefines.h"
#include "iqapiHndlBase.h"
#ifdef WIN32
#pragma warning( disable : 4251 )
#endif
#include <map>
#include <fstream>

using namespace std;

#define     RESPONSE_SIZE                    5000
#define     FM_API_VERSION_STRING_LENGTH     25


//class iqapiHndl;



/// //********************************************************************************
/// FM implementation VSG
/// //********************************************************************************

//! Specifies FM capture parameters
class IQ_API iqapiCaptureFm
{
public:
    iqapiCaptureFm();
    ~iqapiCaptureFm();

    double   *real;	//!< Indicates value for \c real
    double   *imag;	//!< Indicates value for \c imag
    double   realDataOnly;	//!< Indicates value for \c real data. Audio Format: 0=>left channel 1=>right channel, -1=>both channel. Fm data format: 0=>complex, 1=>real only
    double   length;	//!< Indicates the data length
    double   sampleFreqHz;	//!< Indicates sample frequency in Hz
    // HWConfiguration ??

    char lastErr[MAX_LEN_ERR_TXT];	//!< Indicates last error
    int Save(char *fileNameX, char *fileNameY);  //!< Save the file with extension .txt. fileNameY will be generated if the imag part exists or 2 channels for .wav file

};

//! Specifies FM RF analysis results generated using \c iqapiAnalysisFmRf objects
class IQ_API iqapiResultFmRf : public iqapiResult
{
public:
    iqapiResultFmRf() {};	//!< Constructor
    ~iqapiResultFmRf() {};	//!< Destructor

    double obw;                                             //!< Indicates occupied bandwidth (Hz)
    double totalPower;	                                    //!< Indicates total power (dB)
    double powerInBw;                                       //!< channel power corresponding to the channel bandwidth (dB)
    //RF Spectrum Waveform
    double spectrumWaveLength;	                            //!< Indicates RF spectrum wavelength
    double spectrumWaveX[MAX_OUTPUT_WAVE_LENGTH];           //!< Indicates value of the X-axis of the RF spectrum waveform (Hz)
    double spectrumWaveY[MAX_OUTPUT_WAVE_LENGTH];           //!< Indicates value of the Y-axis of the RF spectrum waveform (dBm)
    // FFT output
    double RfSpectrumWaveSaLength;	                        //!< Indicates RF spectrum wavelength
    double RfSpectrumWaveSaResBw;	                        //!< Indicates RF spectrum wavelength resolution bandwidth (Hz)
    double RfSpectrumWaveSaNoiseBw;	                        //!< Indicates RF spectrum wavelength noise bandwidth (Hz)
    double RfSpectrumWaveSaFreqStart;	                    //!< Indicates frequency start of the RF spectrum wavelength (Hz)
    double RfSpectrumWaveSaFreqStop;	                    //!< Indicates end of the frequency of the RF spectrum wavelength (Hz)

};

//! Specifies FM RF analysis results generated using \c iqapiResultFmDemodSpectrum
class IQ_API iqapiResultFmDemodSpectrum : public iqapiResult
{
public:
    iqapiResultFmDemodSpectrum() {};
    ~iqapiResultFmDemodSpectrum() {};

    // 1)Demodulation measurement results:
    double   demodPlusPeakDeviation;                        //!< Indicates plus-peak deviation of the demodulation spectrum (Hz)
    double   demodMinusPeakDeviation;                       //!< Indicates minus-peak deviation of the demodulation spectrum (Hz)
    double   demodAveragePeakDeviation;                     //!< Indicates average peak deviation of the demodulation spectrum (Hz)
    double   demodRmsPeakDeviation;	                        //!< Indicates RMS peak deviation of the demodulation spectrum (Hz)
    double   demodCfo;			                            //!< Indicates carrier frequency offset of the demodulation spectrum (Hz)
    //2)FFT output 
    double   audioSpectrumWaveSaLength;	                    //!< Indicates the size of the audio spectrum FFT window
    double   audioSpectrumWaveSaResBw;	                    //!< Indicates the audio spectrum resolution bandwidth (Hz)
    double   audioSpectrumWaveSaNoiseBw;                    //!< Indicates the audio spectrum wave noise bandwidth (Hz)
    double   audioSpectrumWaveSaFreqStart;                  //!< Indicates the starting point of the audio spectrum (Hz)
    double   audioSpectrumWaveSaFreqStop;                   //!< Indicates the ending point of the audio spectrum (Hz)
    // 3) Audio Spectrum Waveform
    double   audioSpectrumWaveX[MAX_OUTPUT_WAVE_LENGTH];    //!< Indicates the x-axis of the audio spectrum waveform (Hz)
    double   audioSpectrumWaveY[MAX_OUTPUT_WAVE_LENGTH];	//!< Indicates the y-axis of the audio spectrum waveform (dB)
    double   audioSpectrumWaveLength;                       //!< Indicates the audio spectrum wavelength
    // 4)Audio Waveform
    double   audioWaveX[MAX_OUTPUT_WAVE_LENGTH];            //!< Indicates x-axis of waveform (sec)
    double   audioWaveY[MAX_OUTPUT_WAVE_LENGTH];            //!< Indicates y-axis of waveform (Hz)
    double   audioWaveLength;                               //!< Indicates length of the audio waveform

};
//! Specifies analysis results generated using the \c iqapiAnalysisFmMono objects
class IQ_API iqapiResultFmMono : virtual public iqapiResult
{
public:
    iqapiResultFmMono();
    ~iqapiResultFmMono();

    // 1)Demodulation measurement results:
    double   demodPlusPeakDeviation;	                    //!< Indicates plus-peak deviation of the demodulation spectrum  (Hz)
    double   demodMinusPeakDeviation;                       //!< Indicates minus-peak deviation of the demodulation spectrum (Hz)
    double   demodAveragePeakDeviation;                     //!< Indicates average-peak deviation of the demodulation spectrum (Hz)
    double   demodRmsPeakDeviation;	                        //!< Indicates RMS peak deviation of the demodulation spectrum (Hz)
    double   demodCfo;	                                    //!< Indicates carrier offset frequency of the demodulation spectrum (Hz)
    // 2)fft output
    double   audioSpectrumWaveSaLength;                     //!< Indicates the FFT size of the audio spectrum window 
    double   audioSpectrumWaveSaResBw;                      //!< Indicates the resolution bandwidth of the audio spectrum window (Hz)
    double   audioSpectrumWaveSaNoiseBw;                    //!< Indicates the noise bandwidth of the audio spectrum windows (Hz)
    double   audioSpectrumWaveSaFreqStart;                  //!< Indicates the starting point of the audio spectrum wave frequency (Hz)
    double   audioSpectrumWaveSaFreqStop;                   //!< Indicates the end point of the audio spectrum wave frequency (Hz)
    // 3)Audio Spectrum Waveform
    double   audioSpectrumWaveX[MAX_OUTPUT_WAVE_LENGTH];    //!< Indicates x-axis of the audio spectrum waveform (Hz)
    double   audioSpectrumWaveY[MAX_OUTPUT_WAVE_LENGTH];    //!< Indicates y-axis of the audio spectrum waveform (dB)
    double   audioSpectrumWaveLength;	                    //!< Indicates length of the audio wave form
    // 4)Audio Waveform
    double   audioWaveX[MAX_OUTPUT_WAVE_LENGTH];            //!< Indicates x-axis of the audio waveform (sec)
    double   audioWaveY[MAX_OUTPUT_WAVE_LENGTH];            //!< Indicates y-axis of the audio waveform (Hz)
    double   audioWaveLength;                               //!< Indicates length of the audio waveform
    // 5)Mono Audio Analysis outputs
    double   audioAnaSinad;                                 //!< Indicates Signal to Noise and Distortion ratio (dB): Total power divided by the power of noise and distortions
    double   audioAnaSnr;                                   //!< Indicates Signal to Noise ratio (dB): Measured desired signal power divided by the power of noise and distortions
    /*!\note Used for single signal tone only
    */
    double   audioAnaThd;                                   //!< Indicates Total Harmonic Distortion (THD) (dB): Measured totoal harmonic distortion power divided by the fundamental signal power, available only when a single tone is transmitted. 
    double   audioAnaThdPlusN;                              //!< Indicates THD + N. Times 100 to get percentage. Total noise and distoriton power divided by the fundamental signal power, available only when a single tone is transmitted.   
    double   audioAnaTnhd;                                  //!< Indicates Total Non-Harmonic Distortion (dB), Total non-harmonic spurious distoriton power (excluding noise) divided by the fundamental signal power, available only when a single tone is transmitted.
    /*!\note Used for single signal tone only
    */
    double   audioSpectrumWaveToneSigFreq[50];              //!< Indicates the audio tone frequencies detected (or defined by users) in the audio signal (Hz)
    double   audioSpectrumWaveToneSigPower[50];             //!< Indicates the signal power of the audio tones in the audio signal (dB)
    double   audioSpectrumWaveToneNSigFreq;                 //!< Indicates the detected number of signal frequencies 

    // Result check
    double   valid;                                         //!< Indicates that the result is valid
    char     *errMsg;                                       //!< Indicates that there is an error


};
//! Specifies analysis results generated using the \c iqapiAnalysisFmStereo objects
class IQ_API iqapiResultFmStereo : virtual public iqapiResultFmMono
{
public:
    iqapiResultFmStereo() {};
    ~iqapiResultFmStereo() {};


    // 1) Left Audio Spectrum Waveform
    double   leftAudioSpectrumWaveX[MAX_OUTPUT_WAVE_LENGTH];//!< Indicates x-axis of the left audio channel spectrum results (Hz)
    double   leftAudioSpectrumWaveY[MAX_OUTPUT_WAVE_LENGTH];//!< Indicates y-axis of the left audio channel spectrum results (dB)
    double   leftAudioSpectrumWaveLength;                   //!< Indicates x-axis of the left audio channel spectrum results 
    // 2) Left Audio Waveform
    double   leftAudioWaveX[MAX_OUTPUT_WAVE_LENGTH];        //!< Indicates x-axis of left audio spectrum results (sec)
    double   leftAudioWaveY[MAX_OUTPUT_WAVE_LENGTH];        //!< Indicates y-axis of left audio spectrum results (Hz)
    double   leftAudioWaveLength;                           //!< Indicates left audio wavelength 
    // 3 left audio analysis
    double   leftAudioAnaSinad;                             //!< Indicates Signal to Noise and Distortion ratio (dB): Total power divided by the power of noise and distortions
    double   leftAudioAnaSnr;                               //!< Indicates Signal to Noise ratio (dB): Measured desired signal power divided by the power of noise and distortions
    double   leftAudioAnaThd;                               //!< Indicates Total Harmonic Distortion (THD) (dB): Measured totoal harmonic distortion power divided by the fundamental signal power, available only when a single tone is transmitted. 
    double   leftAudioAnaThdPlusN;                          //!< Indicates THD + N. Times 100 to get percentage. Total noise and distoriton power divided by the fundamental signal power, available only when a single tone is transmitted.   
    double   leftAudioAnaTnhd;                              //!< Indicates Total Non-Harmonic Distortion (dB), Total non-harmonic spurious distoriton power (excluding noise) divided by the fundamental signal power, available only when a single tone is transmitted.

    double   leftAudioSpectrumWaveToneSigFreq[50];          //!< Indicates the audio tone frequencies detected (or defined by users) in the left audio channel (Hz)
    double   leftAudioSpectrumWaveToneSigPower[50];         //!< Indicates the signal power of the audio tones in the left audio channel (dB)
    double   leftAudioSpectrumWaveToneNSigFreq;             //!< Indicates number of signal frequencies detected in the left audio spectrum wave tone
    // 4) right Audio Spectrum Waveform
    double   rightAudioSpectrumWaveX[MAX_OUTPUT_WAVE_LENGTH]; //!< Indicates x-axis of the right audio spectrum waveform (Hz)
    double   rightAudioSpectrumWaveY[MAX_OUTPUT_WAVE_LENGTH]; //!< Indicates y-axis for the right audio spectrum waveform (dB)
    double   rightAudioSpectrumWaveLength;                  //!< Indicates length of the right audio spectrum waveform
    // 5) right Audio Waveform
    double   rightAudioWaveX[MAX_OUTPUT_WAVE_LENGTH];       //!< Indicates x-axis of the right audio waveform (sec)
    double   rightAudioWaveY[MAX_OUTPUT_WAVE_LENGTH];       //!< Indicates y-axis of the right audio waveform (Hz)
    double   rightAudioWaveLength;                          //!< Indicates length of the audio waveform
    // 6) right audio analysis
    double   rightAudioAnaSinad;                            //!< Indicates Signal to Noise and Distortion ratio (dB): Total power divided by the power of noise and distortions
    double   rightAudioAnaSnr;                              //!< Indicates Signal to Noise ratio (dB): Measured desired signal power divided by the power of noise and distortions
    double   rightAudioAnaThd;                              //!< Indicates Total Harmonic Distortion (THD) (dB): Measured totoal harmonic distortion power divided by the fundamental signal power, available only when a single tone is transmitted. 
    double   rightAudioAnaThdPlusN;                         //!< Indicates THD + N. Times 100 to get percentage. Total noise and distoriton power divided by the fundamental signal power, available only when a single tone is transmitted.
    double   rightAudioAnaTnhd;                             //!< Indicates Total Non-Harmonic Distortion (dB), Total non-harmonic spurious distoriton power (excluding noise) divided by the fundamental signal power, available only when a single tone is transmitted.

    double   rightAudioSpectrumWaveToneSigFreq[50];         //!< Indicates the signal frequency of the right audio spectrum waveform (Hz)
    double   rightAudioSpectrumWaveToneSigPower[50];        //!< Indicates signal power of the right audio spectrum waveform (dB)
    double   rightAudioSpectrumWaveToneNSigFreq;            //!< Indicates the detected number of frequencies in the right audio spectrum waveform
    // 7) crosstalk 
    double   crosstalkCrosstalk[50];                        //!< Indicates crosstalk array for each unique frequency tone (dB)
    double   crosstalkNSigFreq;                             //!< Indicates number of unique signal frequencies used in crosstalk 
    double   crosstalkSigFreq[50];                          //!< Indicates of unique crosstalk signal frequencies (Hz)
    double   crosstalkChannelIndex[50];                     //!< Indicates index for crosstalk channel frequencies
    /*!< Valid values are as follows:
    \n 0=left
    \n 1=right
    \n -1=both
    */
    double   crosstalkMaxPower;                             //!< Indicates highest signal power for crosstalk (dB)

    double   pilotIsStereo;                                 //!< Indicates whether pilot is stereo or not
    /*!< Valid values are as follows:
    \n 1=stereo
    \n 0=mono
    */
    double   pilotFrequencyOffset;                          //!< measure frequency offset of pilot tone

    double   audioDeviation;
    double   pilotDeviation;
};
//! Specifies analysis results generated using the \c iqapiAnalysisFmAuto objects
class IQ_API iqapiResultFmAuto : virtual public iqapiResultFmStereo
{
public:
    iqapiResultFmAuto() {}; //!< Constructor
    ~iqapiResultFmAuto() {}; //!< Destructor

    int      hasRds;                                         //!< Indicate if the signal contains RDS or not. 0=> No, 1 => Yes, -1 => Not initialized

   // Also refer iqapiResultFmStereo for output parameters

};
//! Specifies analysis results generated using the \c iqapiAnalysisAudioMono objects
class IQ_API iqapiResultAudioMono : public iqapiResult
{
public:
    iqapiResultAudioMono() {}; //!< Constructor
    ~iqapiResultAudioMono() {}; //!< Destructor

    // 1)fft output
    double   audioSpectrumWaveSaLength;                     //!< Indicates the FFT size of the audio spectrum waveform (Hz)
    double   audioSpectrumWaveSaResBw;                      //!< Indicates the bandwidth of the audio spectrum waveform (Hz)
    double   audioSpectrumWaveSaNoiseBw;                    //!< Indicates the noise bandwidth of the audio spectrum waveform (Hz)
    double   audioSpectrumWaveSaFreqStart;                  //!< Indicates the start of the signal frequency  for the audio spectrum waveform (Hz)
    double   audioSpectrumWaveSaFreqStop;                   //!< Indicates the end of the signal frequency  for the audio spectrum waveform (Hz)
    // 2)Audio Spectrum Waveform
    double   audioSpectrumWaveX[MAX_OUTPUT_WAVE_LENGTH];    //!< Indicates x-axis of the right audio spectrum waveform (Hz)
    double   audioSpectrumWaveY[MAX_OUTPUT_WAVE_LENGTH];    //!< Indicates y-axis of the right audio spectrum waveform (dB)
    double   audioSpectrumWaveLength;                       //!< Length of the audio spectrum waveform
    // 3)Mono Audio Analysis outputs
    double   audioAnaSinad;                                 //!< Indicates Signal to Noise and Distortion ratio (dB): Total power divided by the power of noise and distortions
    double   audioAnaSnr;                                   //!< Indicates Signal to Noise ratio (dB): Measured desired signal power divided by the power of noise and distortions
    double   audioAnaThd;                                   //!< Indicates Total Harmonic Distortion (THD) (dB): Measured totoal harmonic distortion power divided by the fundamental signal power, available only when a single tone is transmitted. 
    double   audioAnaThdPlusN;                              //!< Indicates THD + N. Times 100 to get percentage. Total noise and distoriton power divided by the fundamental signal power, available only when a single tone is transmitted.
    double   audioAnaTnhd;                                  //!< Indicates Total Non-Harmonic Distortion (dB), Total non-harmonic spurious distoriton power (excluding noise) divided by the fundamental signal power, available only when a single tone is transmitted.

    double   audioSpectrumWaveToneSigFreq[50];              //!< Indicates signal frequency of the audio spectrum waveform (Hz)
    double   audioSpectrumWaveToneSigPower[50];             //!< Indicates the signal power of the audio spectrum waveform (dB)
    double   audioSpectrumWaveToneNSigFreq;                 //!< Indicates the detected number of frequencies inteh audio spectrum waveform
};

//! Specifies analysis results generated using the \c iqapiAnalysisAudioStereo objects
class IQ_API iqapiResultAudioStereo : public iqapiResult
{
public:
    iqapiResultAudioStereo() {}; //! Constructor
    ~iqapiResultAudioStereo() {}; //!< Destructor

    // 1)FFT output 
    double   audioSpectrumWaveSaLength;	                    //!< Indicates the size of the audio spectrum FFT window
    double   audioSpectrumWaveSaResBw;	                    //!< Indicates the audio spectrum resolution bandwidth (Hz)
    double   audioSpectrumWaveSaNoiseBw;                    //!< Indicates the audio spectrum wave noise bandwidth (Hz)
    double   audioSpectrumWaveSaFreqStart;                  //!< Indicates the starting point of the audio spectrum (Hz)
    double   audioSpectrumWaveSaFreqStop;                   //!< Indicates the ending point of the audio spectrum (Hz)

    // 2) Left Audio Spectrum Waveform
    double   leftAudioSpectrumWaveX[MAX_OUTPUT_WAVE_LENGTH];//!< Indicates x-axis of the left audio spectrum waveform (Hz)
    double   leftAudioSpectrumWaveY[MAX_OUTPUT_WAVE_LENGTH];//!< Indicates the y-axis of the left audio spectrum waveform (dB)
    double   leftAudioSpectrumWaveLength;                   //!< Indicates the length of the left audio spectrum results
    // 3) left audio analysis
    double   leftAudioAnaSinad;                             //!< Indicates Signal to Noise and Distortion ratio (dB): Total power divided by the power of noise and distortions
    double   leftAudioAnaSnr;                               //!< Indicates Signal to Noise ratio (dB): Measured desired signal power divided by the power of noise and distortions
    double   leftAudioAnaThd;                               //!< Indicates Total Harmonic Distortion of the audio signal in the left channel
    double   leftAudioAnaThdPlusN;
    double   leftAudioAnaTnhd;                              //!< Indicates Total Non-Harmonic Distortion of the audio signal in the left channel

    double   leftAudioSpectrumWaveToneSigFreq[50];          //!< Indicates signal frequencies of the audio spectrum waveform in the left channel (Hz)
    double   leftAudioSpectrumWaveToneSigPower[50];         //!< Indicates signal power of the audio spectrum waveform in the left channel (dB)
    double   leftAudioSpectrumWaveToneNSigFreq;             //!< Indicates the detected number of signal frequencies of the audio spectrum waveform of the left channel
    // 4) right Audio Spectrum Waveform
    double   rightAudioSpectrumWaveX[MAX_OUTPUT_WAVE_LENGTH]; //!< Indicates the x-axis of the audio spectrum waveform of the right channel (Hz)
    double   rightAudioSpectrumWaveY[MAX_OUTPUT_WAVE_LENGTH]; //!< Indicates the y-axis of the audio spectrum waveform of the right channel (dB)
    double   rightAudioSpectrumWaveLength;                  //!< Indicates length of the audio spectrum waveform of the right channel
    // 5) right audio analysis
    double   rightAudioAnaSinad;                            //!< Indicates Signal to Noise and Distortion ratio (dB): Total power divided by the power of noise and distortions
    double   rightAudioAnaSnr;                              //!< Indicates Signal to Noise ratio (dB): Measured desired signal power divided by the power of noise and distortions
    double   rightAudioAnaThd;                              //!< Indicates Total Harmonic Distortion (THD) (dB): Measured totoal harmonic distortion power divided by the fundamental signal power, available only when a single tone is transmitted. 
    double   rightAudioAnaThdPlusN;                         //!< Indicates THD + N. Times 100 to get percentage. Total noise and distoriton power divided by the fundamental signal power, available only when a single tone is transmitted.
    double   rightAudioAnaTnhd;                             //!< Indicates Total Non-Harmonic Distortion (dB), Total non-harmonic spurious distoriton power (excluding noise) divided by the fundamental signal power, available only when a single tone is transmitted.

    double   rightAudioSpectrumWaveToneSigFreq[50];         //!< Indicates the audio tone frequencies detected (or defined by users) in the audio signal in the right channel (Hz)
    double   rightAudioSpectrumWaveToneSigPower[50];        //!< Indicates the signal power of the audio tones in the audio signal in the right channel (dB)
    double   rightAudioSpectrumWaveToneNSigFreq;            //!< Indicates the detected number of signal frequencies of the audio spectrum waveform in the right channel

    // 6) cross talk
    double   crosstalkCrosstalk[50];                        //!< Indicates crosstalk array for each unique frequency tone (dB)
    double   crosstalkNSigFreq;                             //!< Indicates number of unique signal frequencies used in crosstalk 
    double   crosstalkSigFreq[50];                          //!< Indicates unique crosstalk signal frequencies (Hz)
    double   crosstalkChannelIndex[50];                     //!< Indicates index for crosstalk channel frequencies
    /*!< Valid values are as follows:
    \n 0=left
    \n 1=right
    \n -1=both
    */
    double   crosstalkMaxPower; //!< Indicates the highest signal power used for crosstalk (dB)
};

class iqapiFmMeasurement
{
public:
    iqapiFmMeasurement() 
    {
      real = NULL;
      length = 0;
    };
    ~iqapiFmMeasurement() 
    {
      if (real == NULL)
      {
         delete real;
         real = NULL;
      }
    };

    double      *real;
    unsigned int length;
};

//! Specifies analysis results generated using the \c iqapiAnalysisRds objects
class IQ_API iqapiResultRds : virtual public iqapiResult
{
public:
    iqapiResultRds(); //!< Constructor
    ~iqapiResultRds(); //!< Destructor

    // RDS result
    double      rdsNBits;                                   //!< Indicates number of raw bits received
    double      rdsNBlock;                                  //!< Indicates number of detected RDS/MMBS blocks
    double      rdsNBlockError;                             //!< Indicates number of decoded blocks with error after decoding
    //   char        *rdsRadioText;                         //!< Indicates the raw decoded characters
    //   char        rdsOutputPiCode[20][5];
    double      nCompleteGroup;                             //!< Indicates number of complete RDS groups
    char        **rdsOutputPiCode;                          //!< Each Row contains program identification (PI) code (HEX String) for each recevied group
    char        *rdsMmbs1stSystemID;                        //!< System ID (HEX String) of detected first MMBS frame
    char        *rdsMmbs1stMsgType;                         //!< Message type (Service or Test) of detected first MMBS frame
    char        *rdsMmbs1stGroupType;                       //!< Group type (Traffic or EWS) of detected first MMBS frame
    char        *rdsMmbs1stCharType;                        //!< Character type (ASCII or Translation Table) of detected first MMBS frame
    char        *rdsOutputMMBSText;                         //!< ASCII text for MMBS frames with Character Type set to ASCII

    char        *rdsRegion;                                 //!< RDS broadcasting region: "United States", "Canada", "Mexico", or "World"
    double      rdsRegionLen;                               //!< length of the string

    char        *rdsFirstValidGroupPiCode;                  //!< Indicates the first valid program identification code
    char        *rdsFirstValidGroupStationID;               //!< Indicates the first valid group station ID

    double      rdsWaveLength;                              //!< Indicates the length of RDS waveform results
    double      rdsWaveX[MAX_OUTPUT_WAVE_LENGTH];           //!< Indicates x-axis of the RDS waveform
    double      rdsWaveY[MAX_OUTPUT_WAVE_LENGTH];           //!< Indicates y-axis of the RDS waveform

    char        *rdsText;                                   //!< Indicates RDS radio text, available only for RDS group type 2

    double      rdsBerValid;                                //!< Indicates if the RDS Bit Error Rate is valid
    double      rdsBer;                                     //!< Indicates RDS Bit Error Rate
    char        *rdsBerErrorMsg;                            //!< Indicates RDS Bit Error Rate message

    unsigned short        **rdsMsg;                         //!< number of rows = rdsNBlock, number of columns = 16, RDS/MMBS information bits

    double      *rawBits;                                   //!< raw binary bits received, including parity bits, its length indicated by rdsNBits
    double      *blockType;                                 //!< Indicates block type for each detected RDS/MMBS block. Its length is indicated by rdsNBlock. Its values are defined as the following:
                                                            //!<    1 = block 1 (offset A); 2 = block 2 (offset B); 3 = block3 version A (offset C); 4 = block 3 version B (offset C'); 5 = block 4; 6 = MMBS (offset E); 0 = invalid block

    double   rdsDeviation;
    int      hasRds;                                        //!< Indicate if the signal contains RDS or not. 0=> No, 1 => Yes, -1 => Not initialized
};
//! Specifies analysis results generated using the \c iqapiAnalysisRdsMono objects
class IQ_API iqapiResultRdsMono : public iqapiResultRds, public iqapiResultFmMono
{
public:
    iqapiResultRdsMono() {};    //!< Constructor
    ~iqapiResultRdsMono() {};   //!< Destructor
};
//! Specifies analysis results generated using the \c iqapiAnalysisRdsStereo objects
class IQ_API iqapiResultRdsStereo : public iqapiResultRds, public iqapiResultFmStereo
{
public:
    iqapiResultRdsStereo() {};  //!< Constructor
    ~iqapiResultRdsStereo() {}; //!< Destructor
};

//! Specifies analysis results generated using the \c iqapiAnalysisRdsAuto objects
class IQ_API iqapiResultRdsAuto : public iqapiResultRds, public iqapiResultFmAuto
{
public:
    iqapiResultRdsAuto() {};    //!< Constructor
    ~iqapiResultRdsAuto() {};   //!< Destructor
};

//! Specifies analysis for FM audio base
class IQ_API iqapiAnalysisFmAudioBase : public iqapiAnalysis
{
public:
    iqapiAnalysisFmAudioBase();      //!< Constructor
    ~iqapiAnalysisFmAudioBase() {};  //!< Destructor

    int SetDefault();

    double   saResBw;                                       //!< Indicates resolution bandwidth  (Hz)
    double   saNFilters;                                    //!< Indicates filter that can be applied

    /*!< Up to three filters can be applied simultaneously
    */
    //char     saFilterType1[28];                           //!< Indicates type of filter
    IQV_FM_FILTER_TYPE saFilterType1;                       //!< See enum for detail options available
    IQV_FM_FILTER_TYPE saFilterType2;                       //!< See enum for detail options available
    IQV_FM_FILTER_TYPE saFilterType3;                       //!< See enum for detail options available

    double   saFilterParam1;                                //!< Indicates filter parameter. For deemphasis filter, this is the time constant in sec: 75e-6 or 50e-6 or 25e-6
                                                            //!< For other types, this is the cutoff frequency (Hz)


    /*!< For deemphasis, this value is the time constant (sec); for all other parameters, this value is the cut-off frequency */
    double   saFilterParam2;	                            //!< Indicates filter parameter
    double   saFilterParam3;	                            //!< Indicates filter parameter
    double   audMaxAmpDynamicRange;                         //!< Indicates the maximum dynamic range (dB) down from the peak of all signals, below which any signal is considered as noise. Use -1 to search over the full range
    /*!< Tones that are smaller than the maximum dynamic range will be ignored.*/
    double   audToneHeightTh;                               //!< Indicates tone height detection threshold (dB)

    /*!< Tone height is the vertical distance between tone peak and the end that is higher.*/
    double   audFreqErrTol;                                 //!< Indicates error tolerance used in harmonic computation and crosstalk

    int      fmVsgNumOfTone;                                //!< Indicates number of user entered audio tones. The default value is 0

    double   fmVsgFreq[MAX_NUM_OF_TONES];                   //!< Indicates number of user-entered audio signal frequencies (Hz)
    double   fmVsgChannel[MAX_NUM_OF_TONES];                //!< Indicates number of user-entered audio channels   
    /*!< Valid values are as follows:
    \n 1 = right
    \n 0 = left
    \n -1 = both
    */
    int      fmVsgPreEmphasisMode;	//!<  Indicates pre-emphasis mode used in TX
    /*!< Valid values are as follows:
    \n 0=no preemphasis is used in VSG
    \n 1=use preemphasis, time constant=75 us
    \n 2=use preemphasis, time constant=50 us
    \n 3=use preemphasis, time constant=25 us
    */

    double   audFreqL;                                      //!< lower bound (Hz) of analyzed range for audio analysis, e.g., 300 Hz
    double   audFreqH;                                      //!< upper bound (Hz) of analyzed range for audio analysis, e.g., 20000 Hz

    void  ConvertFilterTypeCharToEnum(const char *filterTypeChar, IQV_FM_FILTER_TYPE *filterType);
    void  ConvertFilterTypeEnumToChar(IQV_FM_FILTER_TYPE filterType, char *filterTypeChar);
};

//! Specifies FM RF analysis parameters
class IQ_API iqapiAnalysisFmRf : public iqapiAnalysis
{
public:
    iqapiAnalysisFmRf();	//!< Constructor
    ~iqapiAnalysisFmRf() {};	//!< Destructor
    int      SetDefault();

    int Analyze();	

    double      saResBw;	                                //!< Indicates resolution bandwidth (Hz)
    double      percentObw;	                                //!< Indicates occupied bandwidth percentage (Hz)
    double      rfPowerMeasurementBw;	                    //!< Indicates bandwidth of RF power measurements (Hz)
    IQV_FM_WINDOW_TYPE   windowType;                        //!< indicate window type
};

//! Specifies FM Demod Spectrum parameters
class IQ_API iqapiAnalysisFmDemodSpectrum : public iqapiAnalysis
{
public:
    
    iqapiAnalysisFmDemodSpectrum();         //! Constructor    
    ~iqapiAnalysisFmDemodSpectrum() {};     //! Destructor

    int      SetDefault(); 

    double demodFsOut; //!< Indicates the sampling rate at FM demodulator output   
    /*!< Valid values are as follows:
    \n 127875 -- This is the default value
    \n 213125
    */

    double analyzedRange[2]; //!< Indicates the range of signal that is analyzed
    /*!< Valid values are as follows;
    \n start index
    \n stop index
    \n set to [-1, -1] to analyze the entire signal
    */
    double saResBw; //!< Indicates demod spectrum resolution bandwidth (Hz)
};
//! Specifies parameters for FM mono analysis
class IQ_API iqapiAnalysisFmMono : public iqapiAnalysisFmAudioBase
{
public:
    iqapiAnalysisFmMono();      //!< Constructor
    ~iqapiAnalysisFmMono() {};  //!< Destructor
};
//! Specifies parameters for FM stereo analysis
class IQ_API iqapiAnalysisFmStereo : public iqapiAnalysisFmAudioBase
{
public:
    iqapiAnalysisFmStereo();        //!< Constructor
    ~iqapiAnalysisFmStereo() {};    //!< Destructor

};

//! Specifies parameters for FM Auto analysis. This analysis is performed if user does not know that the data is mono or stereo
class IQ_API iqapiAnalysisFmAuto : public iqapiAnalysisFmStereo
{
public:
    iqapiAnalysisFmAuto();      //!< Constructor
    ~iqapiAnalysisFmAuto() {};  //!< Destructor

    //! Refer iqapiAnalysisFmStereo for input parameters
};
//! Specifies parameters for FM audio mono analysis
class IQ_API iqapiAnalysisAudioMono : public iqapiAnalysisFmAudioBase
{
public:
    iqapiAnalysisAudioMono();       //!< Constructor
    ~iqapiAnalysisAudioMono() {};   //!< Destructor

};
//! Specifies parameters for FM audio stereo analysis
class IQ_API iqapiAnalysisAudioStereo : public iqapiAnalysisFmAudioBase
{
public:
    iqapiAnalysisAudioStereo();     //!< Constructor
    ~iqapiAnalysisAudioStereo() {};	//!< Destructor
};
//! Specifies parameters for FM RDS analysis
class IQ_API iqapiAnalysisRds : public iqapiAnalysisFmAudioBase
{
public:
    iqapiAnalysisRds();         //!< Constructor
    ~iqapiAnalysisRds() {};	    //!< Destructor

    char     *rdsRefText;

    int      SetDefault();
};
//!Specifies parameters for FM RDS mono analysis
class IQ_API iqapiAnalysisRdsMono : public iqapiAnalysisFmAudioBase
{
public:
    iqapiAnalysisRdsMono();	//!< Constructor
    ~iqapiAnalysisRdsMono() {}; //!< Destructor

    char     *rdsRefText;

    int      SetDefault();
};
//! Specifies parameters for FM RDS stereo analysis
class IQ_API iqapiAnalysisRdsStereo : public iqapiAnalysisFmAudioBase
{
public:
    iqapiAnalysisRdsStereo();	//!< Constructor
    ~iqapiAnalysisRdsStereo() {}; //!< Destructor

    char     *rdsRefText;

    int      SetDefault();
};
//! Specifies parameters for FM RDS auto analysis
class IQ_API iqapiAnalysisRdsAuto : public iqapiAnalysisFmAudioBase
{
public:
    iqapiAnalysisRdsAuto(); //!< Constructor
    ~iqapiAnalysisRdsAuto() {};  //!< Destructor

    char     *rdsRefText;

    int      SetDefault();
};


//! Specifies parameters for analyzing FM signals
class IQ_API iqapiHndlFm
{      
    iqapiHndlFm() {};

public:
    iqapiHndlFm(iqapiHndlBase *parentParam);
    ~iqapiHndlFm();
    char lastErr[MAX_LEN_ERR_TXT];	                        //!< Indicates last error

    // **************************************************************************
    // FM Vsg and Vsa
    // **************************************************************************
    /// Activate and deactivate
    int Activate(IQV_FM_TECH_ACTIVATION   fmTechAct);       //!< Activates the FM VSA or VSG application
    int Deactivate(IQV_FM_TECH_ACTIVATION fmTechAct);       //!< Deactivates the FM VSA or VSG application
    int IsActivate(IQV_FM_TECH_ACTIVATION fmTechAct, bool *retActivate); //!< Checks if the VSA or VSG application is activated 
    int IsConnected(bool *retConnected);                    //!< Checks if the tester is connected

    // **************************************************************************
    // FM Vsg 
    // **************************************************************************
    void   SetParamModulationState(IQV_FM_VSG_MODULATION_STATE modulationStateParam);	//!< Sets parameters for the modulation state
    void   GetParamModulationState(IQV_FM_VSG_MODULATION_STATE *modulationStateParam);	//!< Gets parameters for the modulation state

    void   SetParamRfOutput(IQV_FM_VSG_RF_OUTPUT rfOutputParam);	//!< Sets RF output parameters
    void   GetParamRfOutput(IQV_FM_VSG_RF_OUTPUT *rfOutputParam);   //!< Gets RF  output parameters
    int    SetRfOutput(IQV_FM_VSG_RF_OUTPUT rfOutputParam);	        //!< Sets RF output 

    void   SetParamFrequency(unsigned int frequencyParam);          //!< Sets parameters for the carrier frequency of the transmitter
    void   GetParamFrequency(unsigned int *frequencyParam);	        //!< Gets parameters for the carrier frequency of the transmitter

    void   SetParamFrequencyOffset(int frequencyOffsetParam);       //!< Sets parameters for the carrier frequency offset
    void   GetParamFrequencyOffset(int *frequencyOffsetParam);      //!< Gets parameters for the carrier frequency offset

    void   SetParamFrequencyDeviation(unsigned int frequencyDeviationParam);    //!< Sets parameters for the carrier frequency deviation
    void   GetParamFrequencyDeviation(unsigned int *frequencyDeviationParam);   //!< Gets parameters for the carrier frequency deviation
    void   GetParamFrequencyDeviationInToneDeviationMode(unsigned int *frequencyDeviationInToneDeviationModeParam);      //<! Get the total freq. deviation (only available In tone deviation mode). freq. deviation = max(audio deviation) + pilot deviation (if stereo is true) + RDS deviation ( if RDS Mode is true ) 
    void   GetParamEstimatePeakDeviationInToneDeviationMode(double *estimatePeakPlusDeviationParam, double *estimatePeakMinusDeviationParam, double *estimatePeakRmsDeviationParam);    //!< For deviation tone table only. Get the estimated peak plus deviation, peak minus deviation and peak RMS deviation
      
    void   SetParamPowerDbm(double powerDbmParam);                  //!< Sets parameters for the carrier power level
    void   GetParamPowerDbm(double *powerDbmParam);                 //!< Gets parameters for the carrier power level

    void   SetParamStereo(IQV_FM_VSG_STEREO_MODES stereoParam);	    //!< Sets parameters for the stereo
    void   GetParamStereo(IQV_FM_VSG_STEREO_MODES *stereoParam);    //!< Gets parameters for the stereo

    void   SetParamPilotDeviation(unsigned int pilotDeviationParam);	//!< Sets parameters for pilot deviation
    void   GetParamPilotDeviation(unsigned int *pilotDeviationParam);	//!< Gets parameters for pilot deviation

    void   SetParamPreemphasisMode(IQV_FM_VSG_PREEMPHASIS_MODES preemphasisModeParam);	//!< Sets parameters for preemphasis mode
    void   GetParamPreemphasisMode(IQV_FM_VSG_PREEMPHASIS_MODES *preemphasisModeParam);	//!< Gets parameters for preemphasis mode

    void   SetParamRdsMode(IQV_FM_VSG_RDS_MODES rdsModeParam);      //!< Sets parameters for RDS mode
    void   GetParamRdsMode(IQV_FM_VSG_RDS_MODES *rdsModeParam);     //!< Gets parameters for RDS mode

    void   SetParamRdsPhaseShift(IQV_FM_VSG_RDS_PHASE_SHIFT rdsPhaseShiftParam);	//!< Sets parameters for RDS phase shift
    void   GetParamRdsPhaseShift(IQV_FM_VSG_RDS_PHASE_SHIFT *rdsPhaseShiftParam);	//!< Gets parameters for RDS phase shift

    void   SetParamRdsDeviation(unsigned int rdsDeviationParam);	//!< Sets parameters for RDS deviation
    void   GetParamRdsDeviation(unsigned int *rdsDeviationParam);	//!< Gets parameters for RDS deviation

    //! Sets parameters for distortion
    void SetParamDistortion(IQV_FM_VSG_DISTORTION_TYPES distortionTypeParam, unsigned int distortionFreqParam, unsigned int distortionDepthParam);
    void GetParamDistortion(IQV_FM_VSG_DISTORTION_TYPES *distortionTypeParam, unsigned int *distortionFreqParam, unsigned int *distortionDepthParam); //!< Gets parameters for distortion

    //! Sets parameters for VSG interfering carrier
    void   SetParamInterferer(IQV_FM_VSG_INTERFERING_CARRIER_MODES interfererModeParam, int interfererDeltaFreqParam, double interfererRelativePowerParam, int interfererFreqDeviationParam);

    //! Gets parameters for VSG interfering carrier
    void   GetParamInterferer(IQV_FM_VSG_INTERFERING_CARRIER_MODES *interfererModeParam, int *interfererDeltaFreqParam, double *interfererRelativePowerParam, int *interfererFreqDeviationParam);

    void    SetParamInterfererModulation(IQV_FM_VSG_MODULATION_TYPES interferingModTypeParam, unsigned int interferingModFreqParam);	//!< Sets parameters for VSG interfering carrier modulation
    void   GetParamInterfererModulation(IQV_FM_VSG_MODULATION_TYPES *interferingModTypeParam, unsigned int *interferingModFreqParam);	//!< Gets parameters for VSG interfering carrier modulation

    void   SetParamAudioToneMode(IQV_FM_VSG_AUDIO_TONE_MODE  audioToneModeParam);      //!< switch between tone table with amplitude or deviation. Need to use the corresponding set of function accordingly
    void   GetParamAudioToneMode(IQV_FM_VSG_AUDIO_TONE_MODE  *audioToneModeParam);

    /*!< For audio tone setup
        - High resolution has frequnecy limit to be 14778 Hz
        - Low resolution frequency limit to be 100000 Hz
        - To be consider high resolution, the frequency should be defined with a number 4 digit after the decimal point (exception with number equals to 5. Like 1000.0005) . Ex. : 1000.0001 or 1000.0006 
    */   
    //!< Use the following functions to operate on audio tone if audioToneMode is set to IQV_FM_AUDIO_TONE_AMPLITUDE
    int            SetParamAudioTone(IQV_FM_VSG_AUDIO_TONE_INDEX index, double audioToneFreqParam, IQV_FM_VSG_AUDIO_CHANNELS audioToneChannelParam, double audioToneAmpParam);	//!< Sets parameters for audio tone
    int            GetParamAudioTone(IQV_FM_VSG_AUDIO_TONE_INDEX index, double *audioToneFreqParam, IQV_FM_VSG_AUDIO_CHANNELS *audioToneChannelParam, double *audioToneAmpParam);	//!< Gets parameters for audio tone
    int            RemoveParamAudioTone(IQV_FM_VSG_AUDIO_TONE_INDEX index);	//!< Removes audio tone parameters
    void           RemoveParamAllAudioTone();  //!< Remove all tones from the amplitude tone table
    unsigned int   GetNumberOfAudioTone();     //!< Get  number of tone from teh amplitude tone table

    //!< Use the following functions to operate on audio tone if audioToneMode is set to IQV_FM_AUDIO_TONE_DEVIATION
    int            SetParamAudioToneWDeviation(IQV_FM_VSG_AUDIO_TONE_INDEX index, double audioToneFreqParam, IQV_FM_VSG_AUDIO_CHANNELS audioToneChannelParam, double audioToneDeviationParam);	//!< Sets parameters for audio tone with deviation
    int            GetParamAudioToneWDeviation(IQV_FM_VSG_AUDIO_TONE_INDEX index, double *audioToneFreqParam, IQV_FM_VSG_AUDIO_CHANNELS *audioToneChannelParam, double *audioToneDeviationParam);	//!< Gets parameters for audio tone with deviation
    int            RemoveParamAudioToneWDeviation(IQV_FM_VSG_AUDIO_TONE_INDEX index);	//!< Removes audio tone parameters
    void           RemoveParamAllAudioToneWDeviation();      //!< Remove all tones from the deviation tone table
    unsigned int   GetNumberOfAudioToneWDeviation();         //!< Get  number of tone from teh deviation tone table

    int   SetTx();	//!< Sets parameters to tester to trasmit a signal
    int   GetTx();	//!< Gets signal transmission parameters from tester
    bool  GetConFm();	//!< Get FM connection status 

    int   LoadRdsFile(char *fileName);  //!< Load Litepoint RDS waveform file
    /*!< The parameter value is as follows:
    \n \c fileName &mdash; indicates waveform file name
    */

    int   LoadRdsData(char *data);  //!< Load RDS type-2A frame with Radio Text specified by char *data
    /*!< The parameter value is as follows:
    \n \c data &mdash; indicates RDS Radio Text character data
    */

    int   LoadRdsDataInfoBits(char *frameVersion, char *infoHexData);   //!< Load User-defined RDS Information Bits. It can have up to 4 groups. Each group is specified by one character for frame version and 16 Hex bytes for information bits
    /*!< The parameter values are as follows:
    \n \c frameVersion &mdash; indicates RDS frame version. Version can be 'A' or 'B' for each group. If you have more than one group, concatenate them together ( ex.: for 2 groups of both type A, pass them as "AA". Leftmost group is transmitted first)
    \n \c infoHexData &mdash; indicates Information in Hex. 16 hex bytes for each group. If you have more than one group, concatenate them together. Leftmost bit is transmitted first.
    */


    // **************************************************************************
    // FM Vsa
    // **************************************************************************
    // RF input state
    void   SetParamVsaRfInput(IQV_FM_VSA_RF_INPUT rfInputParam);                //!< Sets the parameter for VSA RF input
    void   GetParamVsaRfInput(IQV_FM_VSA_RF_INPUT *rfInputParam);               //!< Gets the parameter for VSA RF input
    // AGC mode
    void   SetParamVsaAgcMode(IQV_FM_VSA_AGC_MODES agcModeParam);               //!< Sets the parameter for VSA AGC mode
    void   GetParamVsaAgcMode(IQV_FM_VSA_AGC_MODES *agcModeParam);              //!< Gets the parameter for VSA AGC mode
    // VSA carrier frequency 
    void   SetParamVsaFrequency(unsigned int vsaFrequencyParam);                //!< Sets the parameter for VSA frequency
    void   GetParamVsaFrequency(unsigned int *vsaFrequencyParam);               //!< Gets the parameter for VSA frequency
    // VSA power Dbm 
    void   SetParamVsaPowerDbm(double vsaPowerDbmParam);                        //!< Sets the parameter for VSA power in dBm
    void   GetParamVsaPowerDbm(double *vsaPowerDbmParam);                       //!< Gets the parameter for VSA power in dBm
    // VSA sample rate 
    void   SetParamVsaSampleRate(IQV_FM_VSA_SAMPLE_RATES vsaSampleRateParam);   //!< Sets the parameter for VSA sample rate
    void   GetParamVsaSampleRate(IQV_FM_VSA_SAMPLE_RATES *vsaSampleRateParam);  //!< Gets the parameter for VSA sample rate

    int    SetRx(); //!< Sets the parameters to receive a signal
    int    GetRx(); //!< Gets the parameters to receive a signal

    int    Capture(unsigned int time);                                          //!< Specifies how much time is required for a signal capture; specified in milliseconds

    // **************************************************************************
    // DualHead implementation
    // **************************************************************************
    int GetToken(unsigned int token, unsigned int *tokenID);    
    int SetToken(unsigned int token, unsigned int tokenID);  

    // **************************************************************************
    // FM analysis version
    // **************************************************************************
    int   GetFmAnalysisVersion(char version[FM_API_VERSION_STRING_LENGTH]);

    // **************************************************************************
    // FM Analysis
    // **************************************************************************
    int   LoadSignalFile(char *fileNameReal, char *fileNameImag = NULL, double sampleFreqHz = 639375); //!< Loads the signal file data. For RF signal file, use fileNameReal and fileNameImag. For audio mono file, use fileNameReal and sampleFreqHz. For audio stereo file, use fileNameReal, fileNameImag and sampleFreqHz.
    int   LoadSignalFileWav(char *fileNameWav);                                 //!< Loads the signal waveform

    int   Analyze();	                                                        //!< Indicates analysis of waveform

    iqapiCaptureFm    *dataFm;                                                  //!< Indicates data capture
    iqapiAnalysis     *analysis;                                                //!< Indicates data analysis
    iqapiResult       *results;                                                 //!< Indicates results of the captured data
    short             bitsPerSampleForWavFile;                                  //!< Indicates number of bit per sample for .wav file

private:  
    iqapiHndlBase                 *parentHndl;
    bool                          conFm;	                                    //!< Connection to FM output
    char                          response[RESPONSE_SIZE];

    //!< execute single command to tester
    int ExecSingleCommandAndCheck(char *command, char **singleCommandResult);

    // **************************************************************************
    // FM Vsg
    // **************************************************************************
    IQV_FM_VSG_MODULATION_STATE   modulationState;	                            //!< Indicates modulation state
    IQV_FM_VSG_RF_OUTPUT          rfOutput;	                                    //!< Indicates RF output
    unsigned int                  frequency;	                                //!< Indicates frequency
    int                           frequencyOffset;	                            //!< Indicates frequency offset
    unsigned int                  frequencyDeviation;	                        //!< Indicates frequency deviation ( in resolution of 10 Hz )
    unsigned int                  frequencyDeviationInToneDeviationMode;	    //!< Indicates frequency deviation in tone deviation mode
    double					      estimatePeakPlusDeviation;
    double						  estimatePeakMinusDeviation;
    double						  estimatePeakRmsDeviation;
    double                        powerDbm;	                                    //!< Indicates RF power output in dBm
    IQV_FM_VSG_STEREO_MODES       stereo;	                                    //!< Indicates VSG stereo modes
    unsigned int                  pilotDeviation;	                            //!< Indicates pilot deviation
    IQV_FM_VSG_PREEMPHASIS_MODES  preemphasisMode;	                            //!< Indicates preemphasis mode
    IQV_FM_VSG_AUDIO_CHANNELS     audioChannel;	                                //!< Indicates VSG audio channels
    IQV_FM_VSG_RDS_MODES          rdsMode;	                                    //!< Indicates RDS mode
    IQV_FM_VSG_RDS_PHASE_SHIFT    rdsPhaseShift;	                            //!< Indicates RDS phase shift
    unsigned int                  rdsDeviation;	                                //!< Indicates RDS deviation
    IQV_FM_VSG_DISTORTION_TYPES   distortionType;	                            //!< Indicates distortion type
    unsigned int                  distortionFreq;	                            //!< Indicates distortion frequency
    unsigned int                  distortionDepth;	                            //!< Indicates distortion depth

    //! Audio tone table mode
    IQV_FM_VSG_AUDIO_TONE_MODE audioToneMode;
    //! Specifies audio tone with audio tone amplitude parameters  
    struct audioTone
    {
      double                                    audioToneFreq;	                //!< Indicates audio tone frequency (Hz) Range 10 - 15000
      IQV_FM_VSG_AUDIO_CHANNELS                 audioToneChannel;               //!< Indicates VSG audio channels
      double                                    audioToneAmp;	                //!< Indicates audio tone amplitude (dB) 
      IQV_FM_VSG_AUDIO_TONE_INDEX               audioToneIndex;                 //!< Indicates audio tone index
    };

    //! Specifies audio tone with audio tone deviation parameters (amplitude is derived from audio tone deviation)
    struct audioToneWDeviation
    {
      double                                    audioToneFreq;	                //!< Indicates audio tone frequency (Hz) Range 10 - 15000
      IQV_FM_VSG_AUDIO_CHANNELS                 audioToneChannel;               //!< Indicates VSG audio channels
      double                                    audioToneAmp;	                //!< Indicates audio tone amplitude calculated from audio tone deviation. (tone deviation)/(Total audio tone deviation)
      double                                    audioToneDeviation;             //!< Audio tone deviation specified by user. 
      IQV_FM_VSG_AUDIO_TONE_INDEX               audioToneIndex;                 //!< Indicates audio tone index
    };

    std::map<IQV_FM_VSG_AUDIO_TONE_INDEX, audioTone>                  mapAudioTone;
    std::map<IQV_FM_VSG_AUDIO_TONE_INDEX, audioToneWDeviation>        mapAudioToneWDeviation;

    /// VSG interfering carrier 
    IQV_FM_VSG_INTERFERING_CARRIER_MODES      interfererMode;                   //!< Indicates VSG interfering carrier modes
    int                                       interfererDeltaFreq;	            //!< Indicates change is frequency of the VSG interfering carrier
    double                                    interfererRelativePower;	        //!< Indicates relative power of the VSG interfering carrier
    int                                       interfererFreqDeviation;	        //!< Indicates frequency deviation of the VSG interfering carrier

    /// Vsg interfering carrier modulation
    IQV_FM_VSG_MODULATION_TYPES               interferingModType;	            //!< Indicates the type of modulation for the VSG interfering carrier
    unsigned int                              interferingModFreq;	            //!< Indicates the type of modulation frequency for the VSG interfering carrier

    int   ConClose();	                                                        //!< Indicates that the connection is closed
    int   GetErrorDescription(uint32 errCode, char *errMsg, unsigned int bufSize); //!< Gets the error description by error code 
    int   GetErrorCodeAndDescription();	                                        //!< Gets the error code and description
    int   PreCheckFmVsg();	                                                    //!< Pre-checks FM VSG

    /// Internal use for tone handling
    std::map<IQV_FM_VSG_AUDIO_TONE_INDEX, unsigned int>      mapToneIndexToHandle;

    // **************************************************************************
    // FM Vsa
    // **************************************************************************
    IQV_FM_VSA_RF_INPUT               rfInput;                                  //!< Indicates RF input
    IQV_FM_VSA_AGC_MODES              agcMode;                                  //!< Indicates AGC mode
    unsigned int                      vsaFrequency;                             //!< Indicates VSA frequency
    double                            vsaPowerDbm;                              //!< Indicates VSA power in dBm
    IQV_FM_VSA_SAMPLE_RATES           vsaSampleRate;                            //!< Indicates VSA sample rate

    // **************************************************************************
    // FM for all single command execution
    // **************************************************************************
    char                             *singleCommandResult;                      //!< Indicates result for a single command

    bool  LoadDoubleArray_Static(double *arr, int *len, char *filename, int maxSize);

    int   GetRidOffNonChunkDataFormat(const char *subChunk2Id, ifstream &ifsWav);//!< get rid off the non data chunk portion from .wav file
};

#endif // _IQAPIFM_H_
