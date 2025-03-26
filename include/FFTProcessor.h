#pragma once

#include <arduinoFFT.h>
#include "Constants.h"

// Class responsible for FFT processing
class FFTProcessor {
private:
  // FFT variables
  float vReal[FFTConfig::SAMPLES];
  float vImag[FFTConfig::SAMPLES];
  ArduinoFFT<float> fft;

  // Pre-calculated band indices
  int startIndices[FFTConfig::NUM_BANDS];
  int endIndices[FFTConfig::NUM_BANDS];
  
  // Pre-calculate band indices
  void calculateBandIndices();

  // Prepare data for FFT
  void prepareFFT(int16_t* samples, int numSamples);

  // Calculate frequency bands
  void calculateFrequencyBands(uint8_t* bands, int numBands, int numSamples);

public:
  // Constructor
  FFTProcessor();

  // Process spectrum analysis using FFT
  void process(int16_t* samples, uint8_t* bands);
};
