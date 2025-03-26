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
  
  // Parameters for logarithmic scale frequency band calculation
  float logMin = log10f(FFTConfig::MIN_FREQUENCY);
  float logMax = log10f(FFTConfig::MAX_FREQUENCY);
  float logStep = (logMax - logMin) / FFTConfig::NUM_BANDS;
  
  for (int band = 0; band < FFTConfig::NUM_BANDS; band++) {
    // Calculate start and end frequencies for the band
    float startFreq = powf(10.0f, logMin + band * logStep);
    float endFreq = powf(10.0f, logMin + (band + 1) * logStep);
    
    // Convert frequencies to FFT bin indices
    int startIdx = round(startFreq * numSamples / FFTConfig::SAMPLING_FREQUENCY);
    int endIdx = round(endFreq * numSamples / FFTConfig::SAMPLING_FREQUENCY);
    
    // Range check (minimum is 4 to avoid noise)
    startIndices[band] = constrain(startIdx, 4, maxFreqBin - 1);
    endIndices[band] = constrain(endIdx, startIndices[band] + 1, maxFreqBin);
  }
}

// Prepare data for FFT
void FFTProcessor::prepareFFT(int16_t* samples, int numSamples) {
  // Prepare data for FFT
  for (int i = 0; i < numSamples; i++) {
    // Convert data from I2S
    vReal[i] = (float)samples[i] / 32768.0f; // Normalize 16-bit audio to -1.0~1.0
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
  
  for (int band = 0; band < numBands; band++) {
    // Use pre-calculated indices
    int startIdx = startIndices[band];
    int endIdx = endIndices[band];
    
    // Calculate average value for each band
    float sum = 0.0f;
    for (int j = startIdx; j < endIdx; j++) {
      // Apply correction factor as higher frequencies attenuate more
      float scaleFactor = 1.0f + (float)j / maxFreqBin * 2.0f;
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
