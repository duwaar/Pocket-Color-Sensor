#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <limits.h>

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define ROLLING_BUFFER_LENGTH 20
#define COLOR_TIMEOUT 100000

// Pins
const int
    COLOR_OUT = 2,
    OE        = 3,
    S0        = 4,
    S1        = 5,
    S2        = 6,
    S3        = 7;

// Rolling Average
class RollingAverage {
public:
    RollingAverage();
    float update(unsigned int new_value);
    unsigned long sum;
private:
    unsigned int buffer_index;
    unsigned int buffer[ROLLING_BUFFER_LENGTH];
};

RollingAverage::RollingAverage() {
    for (int i = 0; i < ROLLING_BUFFER_LENGTH; i++) {
        buffer[i] = 0;
    }
    sum = 0;
    buffer_index = 0;
}

float RollingAverage::update(unsigned int new_value) {
    sum -= buffer[buffer_index];
    buffer[buffer_index] = new_value;
    buffer_index++;
    if (buffer_index >= ROLLING_BUFFER_LENGTH) {
        buffer_index = 0;
    }
    sum += new_value;
    return sum / ROLLING_BUFFER_LENGTH;
}

RollingAverage
    red_smoothed,
    green_smoothed,
    blue_smoothed;

// Colors
class Color {
public:
    int rgb[3];
    String name;
    Color::Color(int red, int green, int blue, String color_name);
};

Color::Color(int red, int green, int blue, String color_name) {
    rgb[0] = red;
    rgb[1] = green;
    rgb[2] = blue;
    name = color_name;
}

Color VEC_BLUE(6174, 1413, 681, "BLUE");
Color VEC_DARK_GREY(26723, 62717, 61443, "DARK_GREY");
Color VEC_GREEN(1745, 846, 2140, "GREEN");
Color VEC_LIGHT_GREY(1731, 1596, 1735, "LIGHT_GREY");
Color VEC_ORANGE(579, 1195, 1866, "ORANGE");
Color VEC_PURPLE(1268, 1801, 910, "PURPLE");
Color VEC_RED(697, 3802, 3626, "RED");
Color VEC_WHITE(277, 252, 268, "WHITE");
Color VEC_YELLOW(460, 535, 1137, "YELLOW");

Color color_list[] = {
    VEC_BLUE,
    VEC_DARK_GREY,
    VEC_GREEN,
    VEC_LIGHT_GREY,
    VEC_ORANGE,
    VEC_PURPLE,
    VEC_RED,
    VEC_WHITE,
    VEC_YELLOW
};

// Reading from the TCS3200
enum FilterColor_t {RED, GREEN, BLUE, CLEAR};

void setFilterColor(int color) {
    if (color == RED) {
        digitalWrite(S2, LOW);
        digitalWrite(S3, LOW);
    } else if (color == GREEN) {
        digitalWrite(S2, HIGH);
        digitalWrite(S3, HIGH);
    } else if (color == BLUE) {
        digitalWrite(S2, LOW);
        digitalWrite(S3, HIGH);
    } else if (color == CLEAR) {
        digitalWrite(S2, HIGH);
        digitalWrite(S3, LOW);
    }
}

unsigned long readColor(int color) {
    setFilterColor(color);
    unsigned long pulse_length = pulseIn(COLOR_OUT, HIGH, COLOR_TIMEOUT);
    if (pulse_length == 0) {
        
    }
    return light_intensity;
}

// Show the Adafruit splash screen.
void adafruitSplash() {
    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    display.display();
    delay(1500);
}

void setup() {
    Serial.begin(9600);
    Serial.println("Booting.");
    
    // SSD1306 setup
    Serial.println("Adafruit splash screen.");
    adafruitSplash();
    Serial.println("Set typeface.");
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    Serial.println("Start display.");
    Serial.println("Clear display.");
    display.clearDisplay();
    Serial.println("Set cursor location.");
    display.setCursor(0,0);
    Serial.println("Render text.");
    display.println("Setting up...");
    Serial.println("Flip display buffer.");
    display.display();

    // TCS3200 setup
    Serial.println("TCS3200 setup.");
    pinMode(COLOR_OUT, INPUT);
    pinMode(OE, OUTPUT);
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);

    // Set frequency scale to 20%
    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);

    // Enable output
    digitalWrite(OE, LOW);

    // Create smoothing buffers
    Serial.println("Create smoothing buffers.");
    display.println("Create smoothing buffers.");
    for (int i = 0; i < ROLLING_BUFFER_LENGTH; i++) {
        red_smoothed.update(readColor(RED));
        green_smoothed.update(readColor(GREEN));
        blue_smoothed.update(readColor(BLUE));
    }
    Serial.println("blue, red, green");

    display.println("Setup complete.");
    display.display();
    delay(1000);
}

void loop() {
    unsigned long
        raw_red   = readColor(RED),
        raw_green = readColor(GREEN),
        raw_blue  = readColor(BLUE);
    float
        red   = red_smoothed.update(raw_red),
        green = green_smoothed.update(raw_green),
        blue  = blue_smoothed.update(raw_blue);
    Serial.print(raw_blue);
    Serial.print(", ");
    Serial.print(raw_red);
    Serial.print(", ");
    Serial.print(raw_green);
    Serial.println();
    int closest_index = 0;
    float
        least_distance = 1000000.0,
        distance = 0.0;
    for (int i = 0; i < sizeof(color_list) / sizeof(Color); i++) {
        distance = sqrt(
              sq(red   - color_list[i].rgb[0])
            + sq(green - color_list[i].rgb[1])
            + sq(blue  - color_list[i].rgb[2])
            );
        if (distance < least_distance) {
            least_distance = distance;
            closest_index = i;
        }
    }

    display.clearDisplay();
    display.setCursor(0,0);
    display.print(color_list[closest_index].name);
    display.display();
}
