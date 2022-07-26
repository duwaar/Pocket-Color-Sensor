#ifndef TCS3200_
#define TCS3200_

#include "Arduino.h"

class TCS3200 {
  public:
    TCS3200();
    void begin();
    float frequencyRead();
    // int* colorRead();
    // String colorWordRead();

    enum color_t {RED, GREEN, BLUE, WHITE};
    void setColor(color_t color);

    enum freq_scale_t {POWER_DOWN, PERCENT_2, PERCENT_20, PERCENT_100};
    void setFreqScale(freq_scale_t freq_scale);
    
  private:
    void _ISR_pulseIncrement();
    const uint8_t
      _OUT = 2,
      _OE = 3,
      _S0 = 4,
      _S1 = 5,
      _S2 = 6,
      _S3 = 7;
};

#endif // TCS3200_
