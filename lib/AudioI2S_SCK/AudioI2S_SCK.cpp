#include "AudioI2S_SCK.h"
#include <stdlib.h> // For malloc/free
#include <string.h> // For memset
#include <stdio.h>  // For test case I/O

AudioI2S_SCK::AudioI2S_SCK(uint32_t fftSize) :
  //BUFFER Sizes
  _fftSize(fftSize),
  _bufferSize(0), //Already usable buffersize
  //BUFFERs
  _sampleBuffer(NULL),
  _sampleBufferWin(NULL),
  _fftBuffer(NULL),
  _spectrumBuffer(NULL),
  _AspectrumBuffer(NULL),
  _fftBufferDB(NULL),
  _spectrumBufferDB(NULL),
  _AspectrumBufferDB(NULL),
  _sampleBufferFilt(NULL),
  _sampleBufferFiltF32(NULL),
  //RMS
  _rms_specB(0),
  _rms_AspecB(0),
  _rms_specBDB(0),
  _rms_AspecBDB(0),
  _rmsFilterA(0),
  _rmsFilterADB(0),
  //EXTRAS
  _bitsPerSample(-1),
  _channels(-1),
  _SpectrumAvailable(1),
  _bufferAvailable(1),
  _RMSAvailable(1),
  _available(1),
  _sampleRate(0)
{
}

AudioI2S_SCK::~AudioI2S_SCK(){
  if (_sampleBuffer) {
    free(_sampleBuffer);
  }

  if (_fftBuffer) {
    free(_fftBuffer);
  }

  if (_spectrumBuffer) {
    free(_spectrumBuffer);
  }

  if (_AspectrumBuffer) {
    free(_AspectrumBuffer);
  }
    if (_fftBufferDB) {
    free(_fftBufferDB);
  }

  if (_spectrumBufferDB) {
    free(_spectrumBufferDB);
  }

  if (_AspectrumBufferDB) {
    free(_AspectrumBufferDB);
  }
}

int AudioI2S_SCK::available(){
  _available = _SpectrumAvailable*_bufferAvailable;
  return _available;
}

int AudioI2S_SCK::ConfigureFFT(int bitsPerSample,int channels, int bufferSize, int sampleRate){
    _bitsPerSample = bitsPerSample;
    _channels = channels;
    _bufferSize = bufferSize;
    _sampleRate = sampleRate;

    //General fool-proof checks
    if (_bitsPerSample != 32) {
      return 0;
    }

    if (_channels != 1 && _channels != 2) {
      return 0;
    }

    //Initialize fft
    if (ARM_MATH_SUCCESS != arm_rfft_init_q31(&_S31, _fftSize, 0, 1)) {
      return 0;
    }

    //Allocate memory for buffers
      //Allocate time buffer
      _sampleBuffer = calloc(_bufferSize, sizeof(q31_t));
      _sampleBufferWin = calloc(_bufferSize, sizeof(q31_t));
      
      //Allocate frequency buffers
      _fftBuffer = calloc(_fftSize, sizeof(q31_t));
      _spectrumBuffer = calloc(_fftSize/2, sizeof(q31_t));
      _AspectrumBuffer = calloc(_fftSize/2,sizeof(q31_t));

      //Allocate frecuency dB buffers
      _spectrumBufferDB = calloc(_fftSize/2, sizeof(q31_t));
      _AspectrumBufferDB = calloc(_fftSize/2,sizeof(q31_t));

    //Free all buffers in case of bad allocation
    if (_sampleBuffer == NULL || _sampleBufferWin == NULL || _fftBuffer == NULL || _spectrumBuffer == NULL || _AspectrumBuffer == NULL || _spectrumBufferDB == NULL || _AspectrumBufferDB == NULL) {

      if (_sampleBuffer) {
        free(_sampleBuffer);
        _sampleBuffer = NULL;
      }

      if (_sampleBufferWin) {
        free(_sampleBufferWin);
        _sampleBufferWin = NULL;
      }

      if (_fftBuffer) {
        free(_fftBuffer);
        _fftBuffer = NULL;
      }

      if (_spectrumBuffer) {
        free(_spectrumBuffer);
        _spectrumBuffer = NULL;
      }

      if (_AspectrumBuffer) {
        free(_AspectrumBuffer);
        _AspectrumBuffer = NULL;
      }

      if (_spectrumBufferDB) {
        free(_spectrumBufferDB);
        _spectrumBufferDB = NULL;
      }

      if (_AspectrumBufferDB) {
        free(_AspectrumBufferDB);
        _AspectrumBufferDB = NULL;
      }
      return 0;
    }
    SerialPrint("Configure FFT Successful",6,true);  
    return 1;
}

int AudioI2S_SCK::ConfigureFilter(int bitsPerSample,int channels, int bufferSize, int sampleRate){
    _bitsPerSample = bitsPerSample;
    _channels = channels;
    _bufferSize = bufferSize;
    _sampleRate = sampleRate;

    //General fool-proof checks
    if (_bitsPerSample != 32) {
      return 0;
    }

    if (_channels != 1 && _channels != 2) {
      return 0;
    }

    //Allocate memory for buffers

      //Allocate time buffer
      _sampleBuffer = calloc(_bufferSize, sizeof(q31_t));
      _sampleBufferWin = calloc(_bufferSize, sizeof(q31_t));
      
      //Allocate results buffer
      _sampleBufferFilt = calloc(_bufferSize, sizeof(q31_t));
      _sampleBufferFiltF32 = calloc(_bufferSize, sizeof(q31_t));
      
    //Free all buffers in case of bad allocation
    if (_sampleBuffer == NULL || _sampleBufferWin == NULL){

      if (_sampleBuffer) {
        free(_sampleBuffer);
        _sampleBuffer = NULL;
      }

      if (_sampleBufferWin) {
        free(_sampleBufferWin);
        _sampleBufferWin = NULL;
      }

      return 0;
    }
    SerialPrint("Configure Filter Successful",6,true);  
    return 1;
}

double AudioI2S_SCK::AudioSpectrumRead(int spectrum[], int Aspectrum [],int spectrumDB[], int AspectrumDB[], int fftSize){
  
  if (!_SpectrumAvailable) {
    return 0;
  }

  // Get buffer (currently hardcoded)
  GetBuffer(false);

  // Downscale the sample buffer for proper functioning
  DownScaling(_sampleBuffer, _bufferSize, CONST_FACTOR);

  // Apply Hann Window
  Window();
  
  // FFT - EQUALIZATION and A-WEIGHTING
  FFT();
  EQUALIZING();
  A_WEIGHTING();

  // RMS CALCULATION  
  _rms_specB = RMSG(_spectrumBuffer, _fftSize/2, 2, CONST_FACTOR); 
  _rms_AspecB = RMSG(_AspectrumBuffer, _fftSize/2, 2, CONST_FACTOR); 
  
  _rms_specBDB = FULL_SCALE_DBSPL-(FULL_SCALE_DBFS-20*log10(sqrt(2)*_rms_specB)); 
  _rms_AspecBDB = FULL_SCALE_DBSPL-(FULL_SCALE_DBFS-20*log10(sqrt(2)*_rms_AspecB)); 
 
  // UPSCALING THE BUFFERS
  UpScaling(_sampleBuffer,_bufferSize,CONST_FACTOR);
  UpScaling(_sampleBufferWin,_bufferSize,CONST_FACTOR);
  UpScaling(_spectrumBuffer,fftSize/2,CONST_FACTOR);
  UpScaling(_AspectrumBuffer,fftSize/2,CONST_FACTOR);

  // CONVERT 2 DB THE BUFFERS
  Convert2DB(_spectrumBuffer, _spectrumBufferDB, _fftSize/2);
  Convert2DB(_AspectrumBuffer, _AspectrumBufferDB, _fftSize/2);

  // COPY SPECTRUMS TO MAIN
  memcpy(spectrum, _spectrumBuffer, sizeof(int) * _fftSize/2);
  memcpy(Aspectrum, _AspectrumBuffer, sizeof(int) * _fftSize/2);
  memcpy(spectrumDB, _spectrumBufferDB, sizeof(int) * _fftSize/2);
  memcpy(AspectrumDB, _AspectrumBufferDB, sizeof(int) * _fftSize/2);
  
  // Set available to 0 to wait for new process
  _SpectrumAvailable = 0;

  return _rms_AspecBDB; 
}

double AudioI2S_SCK::AudioTimeFilter(){ 
   
  filterType32 *filter = FilterCreate(); // Create an instance of the filter

  if (!_bufferAvailable) { 
    return 0; 
  } 
 
  // Get buffer (currently hardcoded) 
  GetBuffer(true); 
 
  // Downscale the sample buffer for proper functioning 
  int _time_before_Down = millis();
  DownScaling(_sampleBufferWin, _bufferSize, CONST_FACTOR); 
  int _time_after_Down = millis();
  int time_delta_Down = _time_after_Down-_time_before_Down;
  
  // Filter by convolution - applies a-weighting + equalization + window
  FilterReset(filter);
  int _time_before_FIR = millis();
  int samplesProcessed = FilterInChunks(filter, _sampleBufferWin, _sampleBufferFilt, _bufferSize);
  int _time_after_FIR = millis();
  int time_delta_FIR = _time_after_FIR-_time_before_FIR;

  
  q31_t* sBW = (q31_t*)_sampleBufferWin;
  q31_t* sBF = (q31_t*)_sampleBufferFilt;
  
  for (int i = 0; i < (_bufferSize); i ++) {
    SerialPrint(String(i) + "\t" + String(*sBW) + "\t"+String(*sBF),6,true);
    sBW++;
    sBF++;
  }
  
  
  // RMS CALCULATION 
  int _time_before_RMS = millis();
  _rmsFilterA = RMSG(_sampleBufferFilt, _bufferSize, 3, CONST_FACTOR); 
  _rmsFilterADB = FULL_SCALE_DBSPL-(FULL_SCALE_DBFS-20*log10(sqrt(2)*_rmsFilterA)); 
  int _time_after_RMS = millis();
  int time_delta_RMS = _time_after_RMS-_time_before_RMS;
  
  SerialPrint("samplesProcessed = \t"+ String(samplesProcessed),6,true);
  SerialPrint("time_delta_Down : \t" + String(time_delta_Down),6,true);
  SerialPrint("time_delta_FIR : \t" + String(time_delta_FIR),6,true);
  SerialPrint("time_delta_RMS : \t" + String(time_delta_RMS),6,true);

  /*
    // UPSCALING THE BUFFERS 
    // To be removed if wanted
    UpScaling(_sampleBuffer,_bufferSize,CONST_FACTOR); 
    UpScaling(_sampleBufferWin,_bufferSize,CONST_FACTOR); 
    UpScaling(_sampleBufferFilt,_bufferSize,CONST_FACTOR); 
  */
  
  FilterDestroy(filter);
  // Set available to 0 to wait for new process 
  _bufferAvailable = 0; 

  return _rmsFilterADB; 
} 

double AudioI2S_SCK::AudioRMSRead_dB(){
  return _rms_specBDB;
}

int AudioI2S_SCK::FilterInChunks(filterType32* pThis, void* pInput, void* pOutput, int length){
  int processedLength = 0;
  unsigned int chunkLength, outLength;

  q31_t* _pInpBuffer = (q31_t*) pInput;
  q31_t* _pOutBuffer = (q31_t*) pOutput;

  while( length > 0 )
  {
    // Choose random chunkLength from 0 - 15
    chunkLength = 30;
    // Limit chunk length to the number of remaining samples                                  
    if( chunkLength > length ) chunkLength = length;  
    // Filter the block and determine the number of returned samples   
    outLength = FilterConv(pThis, _pInpBuffer, _pOutBuffer, chunkLength);
    //SerialPrint("outLength\t" + String(outLength),6,true);
    // Update the output pointer
    _pOutBuffer += outLength;            
    // Update the total number of samples output           
    processedLength += outLength;
    // Update the input pointer                  
    _pInpBuffer += chunkLength;
    // Update the number of samples remaining                        
    length -= chunkLength;
    // Cycle the simple random number generator        

  }
  //arm_q31_to_float(_sampleBufferFilt, _sampleBufferFiltF32, _bufferSize);

  // Return the number of samples processed
  return processedLength;                         
}

int AudioI2S_SCK::FilterConv(filterType32 * pThis, q31_t* pInputChunk, q31_t* pOutputChunk, unsigned int count) {
  arm_fir_q31( &pThis->_Q31, 
              pInputChunk, 
              pOutputChunk, 
              count);
  /*
  q31_t* pICprint =  pInputChunk;
  q31_t* pOCprint =  pOutputChunk;

  for (int i=0;i<count;i++){
    SerialPrint(String(i) + "\t" + String(*pICprint) + "\t" + String(*pOCprint),6,true);
    pICprint++;
    pOCprint++;
  }*/
  //SerialPrint("CHUNK COMPLETED",6,true);
  return count;
}

void AudioI2S_SCK::GetBuffer(bool windowed){
  //Get the hardcoded buffer
  //buffer is already treated (usable samples, averaged)
  //If the filter contains the window, we save the buffer into the _sampleBufferWin
  if (!windowed) {
      q31_t* dstG = (q31_t*)_sampleBuffer;
      for (int i = 0; i < _bufferSize; i ++) {
        int value = buffer[i];
        *dstG = value;
        dstG++;
      }
  }
  else {
      q31_t* dstG = (q31_t*)_sampleBufferWin;
      for (int i = 0; i < _bufferSize; i ++) {
        int value = buffer[i];
        *dstG = value;
        dstG++;
      }
  }
}

filterType32 *AudioI2S_SCK::FilterCreate(void){
  filterType32 *result = (filterType32 *)malloc( sizeof(filterType32) ); // Allocate memory for the object
  FilterInit(result);                     // Initialize it
  return result;                                // Return the result
}

void AudioI2S_SCK::FilterInit(filterType32 * pThis){
  arm_float_to_q31(firCoeffs, 
                  pThis->_CoeffsQ31, 
                  FILTERSIZE);

  /*
  for (int i = 0; i < FILTERSIZE; i++){
    SerialPrint(String(i)+"\t"+String(FILTERTAB[i]),6,true);
  }
  */
  arm_fir_init_q31(&pThis->_Q31, 
                  FILTERSIZE, 
                  pThis->_CoeffsQ31, 
                  pThis->_FstateQ31, 
                  FILTERBLOCKSIZE);
  FilterReset( pThis );
}

void AudioI2S_SCK::FilterDestroy(filterType32 *pObject ){
  free( pObject );
  free( _sampleBufferFilt);
  free(_sampleBufferWin);
}

void AudioI2S_SCK::FilterReset(filterType32 * pThis ) {
  // Reset state to 0
  memset( &pThis->_FstateQ31, 0, sizeof( pThis->_FstateQ31) );
  // Reset output
  memset(_sampleBufferFilt,0,sizeof(_sampleBufferFilt));
}

void AudioI2S_SCK::Window(){
  const q31_t* srcW = (const q31_t*)_sampleBuffer;
  q31_t* dstW = (q31_t*)_sampleBufferWin;

  //Apply hann window in time-domain
  for (int i = 0; i < _bufferSize; i ++) {
    double window = HANN[i];
    *dstW = (*srcW) * window;
    dstW++;
    srcW++;
  }
}

void AudioI2S_SCK::FFT(){
  //_sampleBufferWin is already treated for FFT (usable samples, averaged, windowed)

  /*
    // Print Time domain signal
    int32_t* sBprint = (int32_t*)_sampleBuffer;
    for (int i = 0; i < _bufferSize; i ++) {
      SerialPrint(String(i) + "\t" + String(*sBprint),6,true);
      sBprint++;
    }
  */
  
  // Calculate FFTBuffer ((r-i,r-i...))
  arm_rfft_q31(&_S31, (q31_t*)_sampleBufferWin, (q31_t*)_fftBuffer);

  //Calculate spectrumBuffer and normalize
  
  const q31_t* _pfftBuffer = (const q31_t*)_fftBuffer;
  q31_t* _pspectrumBuffer = (q31_t*) _spectrumBuffer;

  for (int i = 0; i < _fftSize; i +=2) {
    *_pspectrumBuffer = (*_pfftBuffer) * (*_pfftBuffer);
    _pfftBuffer++;
    
    *_pspectrumBuffer += (*_pfftBuffer) * (*_pfftBuffer);
    *_pspectrumBuffer = sqrt(*_pspectrumBuffer);

    //Normalize SpectrumBuffer
    if (i) {
      *_pspectrumBuffer = 2 * (*_pspectrumBuffer);
    }

    _pfftBuffer++;
    _pspectrumBuffer++;
  }
}

void AudioI2S_SCK::EQUALIZING(){
  //Get spectrumBuffer
  q31_t* spBE = (q31_t*)_spectrumBuffer;

  for (int i = 0; i < _fftSize/2; i ++) {
    
    //Deconvolution of the spectrumBuffer by division of the microphone frequency response
    double equalfactor = EQUALTAB[i];
    *spBE /= equalfactor;
    spBE++;
  }
}

void AudioI2S_SCK::A_WEIGHTING(){
  //Apply a-weighting to spectrumBuffer
  const q31_t* spBA = (const q31_t*)_spectrumBuffer;
  q31_t* AspBA = (q31_t*)_AspectrumBuffer;

  for (int i = 0; i < _fftSize/2; i ++) {
    
    //Apply a-weighting
    double weighingfactor = WEIGHTINGTAB[i];
    *AspBA = weighingfactor * (*spBA);

    spBA++;
    AspBA++;
  }
}

void AudioI2S_SCK::UpScaling(void *vector, int vectorSize, int factor){
  // UPSCALE signal by factor
    q31_t* _vectUP = (q31_t*) vector;

    for (int i = 0; i<vectorSize;i++){
      *_vectUP *= factor;
      _vectUP++;
    }
}

void AudioI2S_SCK::DownScaling(void *vector, int vectorSize, int factor){
    // DOWNSCALE signal by factor
    q31_t* _vectDW = (q31_t*) vector;
    for (int i = 0; i<vectorSize;i++){
      *_vectDW /= factor;
      _vectDW++;
    }
}

double AudioI2S_SCK::RMSG(void *inputBuffer, int inputSize, int typeRMS, int FACTOR){ 
  //typeRMS = 1 if time domain -- typeRMS = 2 if spectrum domain
  double _rmsOut = 0;
  const q31_t* _pBuffer = (const q31_t*) inputBuffer; 
 
  for (int i = 0; i < inputSize; i ++) { 
    _rmsOut += (*_pBuffer) * (*_pBuffer); 
    _pBuffer++; 
  } 
  _rmsOut = sqrt(_rmsOut/inputSize); 
  
  switch (typeRMS) {
    case 1: //TIME DOMAIN SIGNAL
      _rmsOut = _rmsOut * 1/RMS_HANN* FACTOR; 
      break;
    case 2: //SPECTRUM IN FREQ DOMAIN
      _rmsOut = _rmsOut * 1/RMS_HANN * FACTOR * sqrt(inputSize) / sqrt(2); 
      break;
    case 3:
      _rmsOut = _rmsOut * FACTOR; 
  }
  
  return _rmsOut;
 
  //_RMSAvailable = 1;  
} 

void AudioI2S_SCK::Convert2DB(void *vectorSource, void *vectorDest, int vectorSize){

    q31_t* _vectDB = (q31_t*) vectorDest;
    const q31_t* _vect = (const q31_t*) vectorSource;

    for (int i = 0; i<vectorSize;i++){
      if (*_vect>0){ 
        *_vectDB = FULL_SCALE_DBSPL-(FULL_SCALE_DBFS-20*log10(sqrt(2) * (*_vect)));
      } else {
        *_vectDB = 0;
      }
      _vectDB++;
      _vect++;
    }
    //_SpectrumAvailable = 1;
}

void AudioI2S_SCK::SerialPrint(String ToPrint, int PrioFac, bool NewLine){
  // ONLY FOR PRINTING
  if (PrioFac-PRIORITY>0) {
      if (!NewLine) {
        Serial.print(ToPrint);
      } else Serial.println(ToPrint);
  }
}