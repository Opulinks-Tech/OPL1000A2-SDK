/* Change History
 *
 * Version 1.0.0 (2010-07-27)
 *      - create project to supply dll for AIM Control using the existing AIM API( developed by Matt)
 *		- simplifies the functions in the AIM API and provides functions that can be used by the vDUT(DUT Control) layer 
 *		- or the IQMeasure layer
 *
 */

#ifndef _AIMCONTROL_H_
#define _AIMCONTROL_H_
#endif

#ifdef AIMCONTROL_EXPORTS
    #define AIMCONTROL_API extern "C" __declspec(dllexport)
#else
    #ifdef __cplusplus
        #define AIMCONTROL_API extern "C" __declspec(dllimport) 
    #else
        #define AIMCONTROL_API extern
    #endif
#endif

// ENUM and other definitions
enum AIM_ERROR_CODES
{
	ERR_AIM_OK,
	ERR_AIM_CAPTURE_AUDIO_FAILED,
	ERR_AIM_GENERATE_AUDIO_FAILED,
	ERR_AIM_QUERY_AIM_FAILED,
	ERR_AIM_SAVE_WAV_AUDIO_FAILED,
	ERR_AIM_PLAY_AUDIO_FAILED,
	ERR_AIM_LOAD_AUDIO_FAILED,
	ERR_AIM_STOP_AUDIO_PLAY_FAILED
};

#define AIM_AUDIO_CAPTURE "aimCapture.wav"
#define AIM_N_BITS 16
#define AIM_N_STEREO_CHANNELS 2
#define AIM_N_MONO_CHANNEL 1

// Aim Capture Audio function
	
//! Capture Audio signal using the AIM and save it as a wave file in the current working directory
/*!
 * \param[in] sampleRateHz Sets the Sample Rate for capturing the Audio through AIM (Hz)
 * \param[in] captureDurationMilliSec Sets Capture duration (ms)
 * \param[in] wavFileSaveLocation Sets the path and file name for the wav audio (location path + file name)
 * \param[in] stereo Sets a flag to indicate if the audio capture is Mono( single channel) or stereo ( 2 channels)(Stereo = 1; Mono = 0). 
 *			  You can specify stereo = 1 when recording a Mono signal and 
 *			  you will have the signal recorded from both the Channels, Left and Right.
  
 * \return ERR_AIM_OK if successful;
 */

	AIMCONTROL_API int AIM_CaptureAndSaveAudio(double sampleRateHz = 48000, 
											   double captureDurationMilliSec = 500, 
											   char* wavFileSaveLocation = AIM_AUDIO_CAPTURE, 
											   int stereo = 1);

// Aim Generate and Play Multi Tone Audio function
	
//! Generate, save a wave file(optional) and play Audio through the AIM Device continuously - Call AIM_StopPlayAudio to stop playing 
/*!
 * \param[in] sampleRateHz Sets the Sample Rate for the generated waveform (Hz)
 * \param[in] peakVoltageLevelVolts Sets peak Voltage level for the waveform -Vpeak- (V)
 * \param[in] toneCount Sets the number of tones generated ( count)
 * \param[in] stereoEnable Sets a flag to indicate if the generated audio is stereo(two channels) or Mono ( 1 channel)(Stereo = 1; Mono = 0). 
 *            You can specify stereoEnable = 1 and Generate a mono signal ( L = R) by specifying the leftRightRelation = LEFT_EQUALS_RIGHT.
 *			  This way you can have same signal on 2 channels.
 * \param[in] leftRightRelation Specifies the Left Right Channel Relation as an array corresponding to the 
 *			  freqHz array that specifies the array of tones.
 *			  LEFT_ONLY = 1;
 *			  RIGHT_ONLY = 2;
 *			  LEFT_EQUALS_RIGHT = 3;
 *			  LEFT_EQUALS_MINUS_RIGHT = 4;
 * \param[in] freqHz Specifies the Tones to be generated as an array 
 *			  The number of elements must match the toneCount parameter specified and the leftRightRelation array
 * \param[in] durationMilliSeconds Specifies the duration of the wav audio being generated. 
 *            This function loops the audio to have a continuous playback. 
 * \param[in] audioWaveFileOutput Specifies the location and filename where the generated wave audio needs to be saved.
 *            This is optional. If NULL is passed, the wave file is not saved and the waveform is played from the memory.
 * \return ERR_AIM_OK if successful;
 */
	
	AIMCONTROL_API int AIM_GenerateAndPlayAudioMultiTone(double sampleRateHz = 48000,
													double peakVoltageLevelVolts = 1,
													int toneCount = 1, 
													int stereoEnable = 0,
													int* leftRightRelation = NULL,
													double* freqHz = NULL,
													double durationMilliSeconds = 500,
													char* audioWaveFileOutput = NULL);


AIMCONTROL_API int AIM_GenerateAndPlayAudioSingleTone(double sampleRateHz = 48000,
													double peakVoltageLevelVolts = 1, 
													int stereoEnable = 0,
													int leftRightRelation = 3,
													double freqHz = 1000,
													double durationMilliSeconds = 500,
													char* audioWaveFileOutput = NULL);


//!  Load an existing wave file and play Audio through the AIM Device continuously - Call AIM_StopPlayAudio to stop playing
/*!
 * \param[in] audioWavFileInput Specifies the location and filename of the waveform to be loaded.
 * \return ERR_AIM_OK if successful;
 */
	AIMCONTROL_API int AIM_LoadAndPlayAudio(char* audioWavFileInput);
	
//! Call this function everytime you need to stop Audio play
/*!
 * \return ERR_AIM_OK if successful;
 */
	AIMCONTROL_API int AIM_StopPlayAudio();
	
//! Call this function to clear the memory allocated by the above functions for AIM hndl
/*!
 * \return ERR_AIM_OK if successful;
 */
	AIMCONTROL_API int AIM_FreeMemory();
