#include "SpectrumRenderer.h"
#include <string.h>

// Constructor
SpectrumRenderer::SpectrumRenderer(M5Canvas& canvas) : canvas(canvas), wasAllZero(true) {
  // Initialize peak values and colors using memset for faster initialization
  memset(peakValues, 0, sizeof(peakValues));
  memset(peakColors, 0, sizeof(peakColors));
}

// Draw the spectrum
void SpectrumRenderer::draw(uint8_t* bandValues) {
  
  // Check if all values in bandValues and peakValues arrays are zero
  bool isAllZero = true;
  for (int i = 0; i < FFTConfig::NUM_BANDS; i++) {
    if (bandValues[i] > 0 || peakValues[i] > 0) {
      isAllZero = false;
      break;
    }
  }
  
  // Skip drawing if all values are zero and were zero last time (power saving)
  if (isAllZero && wasAllZero) {
    return;
  }
  
  // Record current state
  wasAllZero = isAllZero;
  
  // Clear canvas
  canvas.fillScreen(ColorConfig::BACKGROUND);
 
  // Select one random color for this drawing cycle (for visual consistency)
  uint16_t randomPaleColor = ColorConfig::PALE_COLORS[random(16)];

  // Draw the spectrum
  int x = 0;
  for (int i = 0; i < FFTConfig::NUM_BANDS; i++) {
    int barHeight = (bandValues[i] * maxHeight) / 255;
    
    // Update peak values
    if (barHeight > peakValues[i]) {
      peakValues[i] = barHeight;
      // Select random color when peak value is updated
      peakColors[i] = randomPaleColor;
    } else {
      // Descent rate
      peakValues[i] -= DisplayConfig::PEAK_DESCENT_RATE;
      if (peakValues[i] < 0) peakValues[i] = 0;
    }

    // Draw bar (single color, darker)
    canvas.fillRect(x, baseY - barHeight, DisplayConfig::BAR_WIDTH, barHeight, ColorConfig::BAR_COLOR);
    
    // Draw peak value (using randomly selected color)
    if (peakValues[i] > 0) {
      canvas.fillRect(x, baseY - peakValues[i] - DisplayConfig::PEAK_HEIGHT, 
                     DisplayConfig::BAR_WIDTH, DisplayConfig::PEAK_HEIGHT, peakColors[i]);
    }
    
    x += barStep;
  }
  
  // Transfer canvas contents to display
  canvas.pushSprite(0, 0);
}

// Initialize the canvas
void SpectrumRenderer::initialize() {
  // Pre-calculate canvas constants
  baseY = canvas.height() - DisplayConfig::BASE_Y_OFFSET;
  maxHeight = canvas.height() - DisplayConfig::MAX_HEIGHT_OFFSET;
  barStep = DisplayConfig::BAR_WIDTH + DisplayConfig::BAR_SPACING;
  
  canvas.fillScreen(ColorConfig::BACKGROUND);
  canvas.pushSprite(0, 0);
}
