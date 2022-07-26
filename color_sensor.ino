// PINS
const uint8_t
    OUT = 2,
    OE = 3,
    S0 = 4,
    S1 = 5,
    S2 = 6,
    S3 = 7;

// PULSE COUNTING
volatile uint16_t
    pulse_count;
uint32_t
    start_time,
    end_time;

void setup() {
    pinMode(OE, OUTPUT);
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);

    // Enable output
    digitalWrite(OE, LOW);

    // Set frequency scale
    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);

    // Set color to read
    digitalWrite(S2, HIGH);
    digitalWrite(S3, LOW);

    attachInterrupt(digitalPinToInterrupt(OUT), ISR_pulseIncrement, RISING);
    
    
    Serial.begin(9600);
}

void loop() {
    delay(200);
    Serial.print(frequencyRead());
    Serial.print(" kHz");
    Serial.println();
}

void ISR_pulseIncrement() {
    pulse_count += 1;
}

float frequencyRead() {
    // Returns frequency in kHz
    end_time = micros();
    float frequency = 1000.0 * pulse_count / (end_time - start_time);
    start_time = micros();  
    pulse_count = 0;
    return frequency;
}
