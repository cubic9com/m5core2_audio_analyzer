#pragma once

#include <M5Unified.h>
#include "Constants.h"

// Class responsible for rendering the spectrum
class SpectrumRenderer {
private:
  M5Canvas& canvas;
  int peakValues[FFTConfig::NUM_BANDS];
  uint16_t peakColors[FFTConfig::NUM_BANDS];
  bool wasAllZero;
  
  // Band values used for the last drawn frame (for unchanged-frame skipping)
  uint8_t lastBandValues[FFTConfig::NUM_BANDS];
  bool hasLastBandValues;
  
  // Pre-calculated canvas constants
  int baseY;
  int maxHeight;
  int barStep;

public:
  // Constructor
  SpectrumRenderer(M5Canvas& canvas);

  // Draw the spectrum
  void draw(uint8_t* bandValues);

  // Initialize the canvas
  void initialize();
};
