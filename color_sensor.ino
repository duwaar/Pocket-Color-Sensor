/*
 * TO DO
 * 
 * MINIMUM VIABLE PRODUCT:
 * Done! - Read freq from sensor
 * Done! - Create a frequency calibration routine
 * Done! - Use frequency calibration and measurements to calculate intensity
 * Done! - Read a vector of "RGB" values from sensor
 *     - Why do I sometimes get negative values?
 * - Fix flicker issue
 *     - Maybe find the average of several readings?
 *     - Maybe find the deviation of the sample set and provide info about flicker?
 * Done! - Make a list of named colors
 *     - Add a calibration function
 * Done! - Find closest match in named list
 * Done! - Display name of closest color
 * - Display color name on LCD character display
 * 
 * EXTRA CREDIT:
 * - Build enclosure
 *     - Light source
 *     - Display
 *     - Battery and charging port/circuit/etc.
 * - Test Arduino code
 *     - https://stackoverflow.com/questions/780819/how-can-i-unit-test-arduino-code
 *     - Build "mock_arduino.h"
 *     - Compile code and run unit tests on development machine
 *     - http://www.throwtheswitch.org/ceedling/
 * - Build a spectrophotometer
 *     - https://www.instructables.com/Shoe-Box-Spectrophotometer/
 *     - Use this device to measure light sources, more easiliy calculate color space transformation
 *     - More easily calibrate this device because of the higher spectral resolution
 * - Color-space transformation calibration
 *     - Known (CIE color space mapped) source with >= 3 colors (points in color space)
 *     - Calculate transformation from sensor input to CIE color space
 *     - Use transformation to convert from "RGB" vector to CIE color space vector
 */

#include "src/TCS3200.hpp"
#include "src/ColorVector.hpp"
#include "src/FilterColor_t.hpp"
#include "src/FreqScale_t.hpp"
#include "src/known_colors.hpp"


TCS3200 ColorSensor = TCS3200(2, 3, 4, 5, 6, 7);
ColorVector color_reading;
ColorVector closest_color;

void setup() {
    Serial.begin(9600);
}

void loop() {
    color_reading = ColorSensor.readColorVector();
    closest_color = *find_closest_color(color_reading);
    Serial.print("closest color: ");
    Serial.print(closest_color.name());
    Serial.println();
    delay(200);
}

ColorVector* find_closest_color(ColorVector color_vector) {
    int
        closest_index = 0;
    float
        closest_distance = color_vector.distance(KNOWN_COLORS[0]),
        current_distance = 0.0;
    int
        KNOWN_COLORS_length = sizeof(KNOWN_COLORS) / sizeof(KNOWN_COLORS[0]);
        
    for (int i = 0; i < KNOWN_COLORS_length; i++) {
        current_distance = color_vector.distance(KNOWN_COLORS[i]);
        if (current_distance < closest_distance) {
            closest_index    = i;
            closest_distance = current_distance;
        }
    }
    return &KNOWN_COLORS[closest_index];
}
