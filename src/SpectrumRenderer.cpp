#include "SpectrumRenderer.h"

// Constructor
SpectrumRenderer::SpectrumRenderer(M5Canvas& canvas) : canvas(canvas), wasAllZero(true) {
  // Initialize peak values and colors
  for (int i = 0; i < FFTConfig::NUM_BANDS; i++) {
    peakValues[i] = 0;
    peakColors[i] = 0;
  }
  
  // Initialize pale tone color array
  paleColors[0] = ColorConfig::PALE_RED;
  paleColors[1] = ColorConfig::PALE_PINK;
  paleColors[2] = ColorConfig::PALE_ORANGE;
  paleColors[3] = ColorConfig::PALE_YELLOW;
  paleColors[4] = ColorConfig::PALE_GREEN;
  paleColors[5] = ColorConfig::PALE_MINT;
  paleColors[6] = ColorConfig::PALE_CYAN;
  paleColors[7] = ColorConfig::PALE_SKY;
  paleColors[8] = ColorConfig::PALE_BLUE;
  paleColors[9] = ColorConfig::PALE_INDIGO;
  paleColors[10] = ColorConfig::PALE_PURPLE;
  paleColors[11] = ColorConfig::PALE_MAGENTA;
  paleColors[12] = ColorConfig::PALE_ROSE;
  paleColors[13] = ColorConfig::PALE_PEACH;
  paleColors[14] = ColorConfig::PALE_GOLD;
  paleColors[15] = ColorConfig::PALE_LIME;
}

// Draw the spectrum
void SpectrumRenderer::draw(uint8_t* bandValues) {
  const int baseY = canvas.height() - DisplayConfig::BASE_Y_OFFSET;
  const int maxHeight = canvas.height() - DisplayConfig::MAX_HEIGHT_OFFSET;
  
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
  uint16_t randomPaleColor = paleColors[random(16)];

  // Draw the spectrum
  for (int i = 0; i < FFTConfig::NUM_BANDS; i++) {
    int barHeight = map(bandValues[i], 0, 255, 0, maxHeight);
    int x = i * (DisplayConfig::BAR_WIDTH + DisplayConfig::BAR_SPACING);
    
    // Update peak values
    if (barHeight > peakValues[i]) {
      peakValues[i] = barHeight;
      // Select random color when peak value is updated
      peakColors[i] = randomPaleColor;
    } else {
      // Descent rate
      peakValues[i] = max(0, peakValues[i] - DisplayConfig::PEAK_DESCENT_RATE);
    }

    // Draw bar (single color, darker)
    canvas.fillRect(x, baseY - barHeight, DisplayConfig::BAR_WIDTH, barHeight, ColorConfig::BAR_COLOR);
    
    // Draw peak value (using randomly selected color)
    if (peakValues[i] > 0) {
      canvas.fillRect(x, baseY - peakValues[i] - DisplayConfig::PEAK_HEIGHT, 
                     DisplayConfig::BAR_WIDTH, DisplayConfig::PEAK_HEIGHT, peakColors[i]);
    }
  }
  
  // Transfer canvas contents to display
  canvas.pushSprite(0, 0);
}

// Initialize the canvas
void SpectrumRenderer::initialize() {
  canvas.fillScreen(ColorConfig::BACKGROUND);
  canvas.pushSprite(0, 0);
}
