#include "FFTProcessor.h"
#include <math.h>

// Constructor
FFTProcessor::FFTProcessor() : fft(vReal, vImag, FFTConfig::SAMPLES, FFTConfig::SAMPLING_FREQUENCY) {
  // Pre-calculate band indices
  calculateBandIndices();
}

// Pre-calculate band indices
void FFTProcessor::calculateBandIndices() {
  int numSamples = FFTConfig::SAMPLES;
  int maxFreqBin = numSamples / 2;
  float freqScale = (float)numSamples / FFTConfig::SAMPLING_FREQUENCY;
  
  // Parameters for logarithmic scale frequency band calculation
  float logMin = log10f(FFTConfig::MIN_FREQUENCY);
  float logMax = log10f(FFTConfig::MAX_FREQUENCY);
  float logStep = (logMax - logMin) / FFTConfig::NUM_BANDS;
  
  // Pre-calculate constant for frequency multiplication
  float freqMultiplier = powf(10.0f, logStep);
  
  // Calculate starting frequency using cumulative multiplication
  float currentFreq = powf(10.0f, logMin);
  
  for (int band = 0; band < FFTConfig::NUM_BANDS; band++) {
    // Calculate start and end frequencies for the band
    float startFreq = currentFreq;
    float endFreq = currentFreq * freqMultiplier;
    currentFreq = endFreq;
    
    // Convert frequencies to FFT bin indices
    int startIdx = round(startFreq * freqScale);
    int endIdx = round(endFreq * freqScale);
    
    // Range check (minimum is 4 to avoid noise)
    startIndices[band] = constrain(startIdx, 4, maxFreqBin - 1);
    endIndices[band] = constrain(endIdx, startIndices[band] + 1, maxFreqBin);
  }
}

// Prepare data for FFT
void FFTProcessor::prepareFFT(int16_t* samples, int numSamples) {
  // Pre-calculate normalization constant
  const float normScale = 1.0f / 32768.0f;
  
  // Prepare data for FFT
  for (int i = 0; i < numSamples; i++) {
    // Convert data from I2S using constant multiplication instead of division
    vReal[i] = (float)samples[i] * normScale;
    vImag[i] = 0.0f;
  }
  
  // Execute FFT
  fft.windowing(vReal, numSamples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  fft.compute(vReal, vImag, numSamples, FFT_FORWARD);
  fft.complexToMagnitude(vReal, vImag, numSamples);
}

// Calculate frequency bands
void FFTProcessor::calculateFrequencyBands(uint8_t* bands, int numBands, int numSamples) {
  // Calculate the range of available FFT bins
  // Valid frequency range is up to half the sampling frequency
  int maxFreqBin = numSamples / 2;
  float invMaxFreqBin = 1.0f / maxFreqBin;  // Pre-calculate inverse to avoid division in loop
  
  for (int band = 0; band < numBands; band++) {
    // Use pre-calculated indices
    int startIdx = startIndices[band];
    int endIdx = endIndices[band];
    
    // Calculate average value for each band
    float sum = 0.0f;
    for (int j = startIdx; j < endIdx; j++) {
      // Apply correction factor as higher frequencies attenuate more
      float scaleFactor = 1.0f + j * invMaxFreqBin * 2.0f;
      sum += vReal[j] * scaleFactor;
    }
    
    // Divide by number of bins to get average
    float avg = sum / (endIdx - startIdx);
    
    // Convert to 0-255 range (adjust sensitivity)
    float amplifiedValue = avg * 300.0f;
    
    // Apply noise floor (ignore signals below threshold)
    if (amplifiedValue < FFTConfig::NOISE_FLOOR) {
      amplifiedValue = 0.0f;
    }
    
    bands[band] = constrain(amplifiedValue, 0, 255);
  }
}

// Process spectrum analysis using FFT
void FFTProcessor::process(int16_t* samples, uint8_t* bands) {
  // Prepare FFT
  prepareFFT(samples, FFTConfig::SAMPLES);
  
  // Calculate frequency bands
  calculateFrequencyBands(bands, FFTConfig::NUM_BANDS, FFTConfig::SAMPLES);
}
