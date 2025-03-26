#pragma once

#include <M5Unified.h>

//=============================================================================
// Constant Namespaces
//=============================================================================

// FFT Processing Constants
namespace FFTConfig {
  // FFT Basic Settings
  constexpr int SAMPLES = 256;                // Number of samples
  constexpr int SAMPLING_FREQUENCY = 2000;    // Sampling frequency
  constexpr int NUM_BANDS = 16;               // Number of frequency bands
  
  // Frequency Range Settings
  constexpr float MIN_FREQUENCY = 100.0f;     // Minimum frequency (Hz)
  constexpr float MAX_FREQUENCY = 1000.0f;    // Maximum frequency (Hz)
  
  // Noise Floor Settings
  constexpr float NOISE_FLOOR = 40.0f;        // Noise floor
}

// Display Related Constants
namespace DisplayConfig {
  // Bar Display Settings
  constexpr int BAR_WIDTH = 16;               // Width of bars
  constexpr int BAR_SPACING = 4;              // Spacing between bars
  
  // Peak Display Settings
  constexpr int PEAK_HEIGHT = 2;              // Height of peak indicators
  constexpr int PEAK_DESCENT_RATE = 3;        // Descent rate of peak indicators
  
  // Screen Settings
  constexpr int BASE_Y_OFFSET = 0;            // Base Y-coordinate offset
  constexpr int MAX_HEIGHT_OFFSET = 10;       // Maximum height offset
}

// Color Related Constants
namespace ColorConfig {
  // Basic Colors
  constexpr uint16_t TFT_DARKGREY = 0x7BEF;
  constexpr uint16_t TFT_VERYDARKGREY = 0x18C3;  // Darker grey (about 1/4 brightness of TFT_DARKGREY)
  constexpr uint16_t BACKGROUND = TFT_BLACK;     // Background color
  constexpr uint16_t BAR_COLOR = TFT_VERYDARKGREY; // Bar color
  
  // Pale Tone Color Definitions
  constexpr uint16_t PALE_RED = 0xFDEF;      // Pale red
  constexpr uint16_t PALE_PINK = 0xFEDF;     // Pale pink
  constexpr uint16_t PALE_ORANGE = 0xFEE0;   // Pale orange
  constexpr uint16_t PALE_YELLOW = 0xFFF0;   // Pale yellow
  constexpr uint16_t PALE_GREEN = 0xDFF7;    // Pale green
  constexpr uint16_t PALE_MINT = 0xCFF9;     // Pale mint
  constexpr uint16_t PALE_CYAN = 0xAFFC;     // Pale cyan
  constexpr uint16_t PALE_SKY = 0xAFDF;      // Pale sky blue
  constexpr uint16_t PALE_BLUE = 0xAEFF;     // Pale blue
  constexpr uint16_t PALE_INDIGO = 0xB5DF;   // Pale indigo
  constexpr uint16_t PALE_PURPLE = 0xD5FF;   // Pale purple
  constexpr uint16_t PALE_MAGENTA = 0xFCDF;  // Pale magenta
  constexpr uint16_t PALE_ROSE = 0xFEDC;     // Pale rose
  constexpr uint16_t PALE_PEACH = 0xFEF0;    // Pale peach
  constexpr uint16_t PALE_GOLD = 0xFFF0;     // Pale gold
  constexpr uint16_t PALE_LIME = 0xDFF0;     // Pale lime
  
  // Pale Color Array
  const uint16_t PALE_COLORS[16] = {
    PALE_RED, PALE_PINK, PALE_ORANGE, PALE_YELLOW,
    PALE_GREEN, PALE_MINT, PALE_CYAN, PALE_SKY,
    PALE_BLUE, PALE_INDIGO, PALE_PURPLE, PALE_MAGENTA,
    PALE_ROSE, PALE_PEACH, PALE_GOLD, PALE_LIME
  };
}

// System-wide Constants
namespace SystemConfig {
  // Task Settings
  constexpr int AUDIO_TASK_STACK_SIZE = 16384;  // Audio task stack size
  constexpr int DISPLAY_TASK_STACK_SIZE = 8192; // Display task stack size
  constexpr int AUDIO_TASK_PRIORITY = 1;        // Audio task priority
  constexpr int DISPLAY_TASK_PRIORITY = 1;      // Display task priority
  constexpr int AUDIO_TASK_CORE = 0;            // Core to run audio task
  constexpr int DISPLAY_TASK_CORE = 1;          // Core to run display task
  
  // Delay Settings
  constexpr int AUDIO_TASK_DELAY = 1;           // Audio task delay
  constexpr int DISPLAY_TASK_DELAY = 10;        // Display task delay (approx. 100fps)
  constexpr int DISPLAY_INIT_DELAY = 700;       // Display initialization delay
}
