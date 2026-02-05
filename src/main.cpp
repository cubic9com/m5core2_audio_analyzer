#include <M5Unified.h>
#include <string.h>
#include "Constants.h"
#include "FFTProcessor.h"
#include "SpectrumRenderer.h"

// Components
FFTProcessor fftProcessor;
M5Canvas canvas(&M5.Display);
SpectrumRenderer renderer(canvas);

// Data Buffers
int16_t audioBuffer[FFTConfig::SAMPLES];
uint8_t bandValues[FFTConfig::NUM_BANDS];

// Task Variables
TaskHandle_t audioTaskHandle = NULL;
TaskHandle_t displayTaskHandle = NULL;
SemaphoreHandle_t xSemaphore = NULL;
bool isRunning = true;

// Audio Processing
void audioProcessing() {
  // Initialize microphone using M5Unified library
  auto mic_cfg = M5.Mic.config();
  mic_cfg.sample_rate = FFTConfig::SAMPLING_FREQUENCY;
  mic_cfg.stereo = false;
  M5.Mic.config(mic_cfg);
  M5.Mic.begin();
  
  while (isRunning) {
    // Get data from microphone using M5Unified library
    int len = M5.Mic.record(audioBuffer, FFTConfig::SAMPLES);
    
    if (len > 0) {
      // Take semaphore for spectrum analysis
      if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
        // Perform spectrum analysis using FFT
        fftProcessor.process(audioBuffer, bandValues);
        
        xSemaphoreGive(xSemaphore);
      }
    }
    
    // Short delay
    vTaskDelay(SystemConfig::AUDIO_TASK_DELAY);
  }
  
  // Stop microphone when task ends
  M5.Mic.end();
  vTaskDelete(NULL);
}

// Display Update
void displayUpdate() {
  delay(SystemConfig::DISPLAY_INIT_DELAY);

  while (isRunning) {
    M5.update();
      
    // Spectrum display
    if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
      renderer.draw(bandValues);
      xSemaphoreGive(xSemaphore);
    }
    
    // Delay to control frame rate
    vTaskDelay(SystemConfig::DISPLAY_TASK_DELAY);
  }
  
  vTaskDelete(NULL);
}

// Audio Sampling Task (static method)
void audioTaskFunction(void* parameter) {
  audioProcessing();
}

// Display Task (static method)
void displayTaskFunction(void* parameter) {
  displayUpdate();
}

// Initialize
void initialize() {
  // Initialize M5Stack (enable microphone)
  // PSRAM is disabled by removing flags in platformio.ini
  auto cfg = M5.config();
  cfg.internal_mic = true;
  M5.begin(cfg);
  
  // Initialize random seed
  randomSeed(esp_random());
  
  // Initialize canvas
  canvas.createSprite(M5.Display.width(), M5.Display.height());
  renderer.initialize();
  
  // Create semaphore
  xSemaphore = xSemaphoreCreateMutex();
  
  // Initialize band values using memset for faster initialization
  memset(bandValues, 0, sizeof(bandValues));
}

// Start
void start() {
  // Start audio task (run on core 0)
  xTaskCreatePinnedToCore(
    audioTaskFunction,                   // Task function
    "AudioTask",                         // Task name
    SystemConfig::AUDIO_TASK_STACK_SIZE, // Stack size
    NULL,                                // Parameters
    SystemConfig::AUDIO_TASK_PRIORITY,   // Priority
    &audioTaskHandle,                    // Task handle
    SystemConfig::AUDIO_TASK_CORE        // Core to run on
  );
  
  // Start display task (run on core 1)
  xTaskCreatePinnedToCore(
    displayTaskFunction,                 // Task function
    "DisplayTask",                       // Task name
    SystemConfig::DISPLAY_TASK_STACK_SIZE, // Stack size
    NULL,                                // Parameters
    SystemConfig::DISPLAY_TASK_PRIORITY, // Priority
    &displayTaskHandle,                  // Task handle
    SystemConfig::DISPLAY_TASK_CORE      // Core to run on
  );
}

// Stop
void stop() {
  isRunning = false;
}

// Setup function
void setup() {
  // Initialize and start
  initialize();
  start();
}

// Main loop
void loop() {
  // Main loop does nothing (all processing is done in separate tasks)
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
