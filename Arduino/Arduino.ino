#define DEBUG 0

const byte USS_TRIGGER_P = 2;
const byte USS_ECHO_P = 3;
const byte LED_P = 7;

float read_ultra_sonic() 
{
   digitalWrite(USS_TRIGGER_P, LOW);
   delayMicroseconds(2);

   digitalWrite(USS_TRIGGER_P, HIGH);
   delayMicroseconds(10);

   digitalWrite(USS_TRIGGER_P, LOW);

   float duration = pulseIn(USS_ECHO_P, HIGH); 
   float distance = duration * 0.0343 / 2.0;  // in centimeters

   return distance;
}

void setup() 
{
    pinMode(USS_TRIGGER_P, OUTPUT);
    pinMode(USS_ECHO_P, INPUT);
    pinMode(LED_P, OUTPUT);

    if (DEBUG) 
    {
        Serial.begin(9600);
        while (!Serial);
    }

    delay(100);
}

void loop() 
{
    float distance = read_ultra_sonic();

    if (DEBUG)
    {
        Serial.print("Distance sensor read "); 
        Serial.print(distance);
        Serial.println(" cm.");
    }

    if (distance < 15.00) 
    {
        digitalWrite(LED_P, HIGH);
    } 
    else 
    {
        digitalWrite(LED_P, LOW);
    }

    delay(200); 
}
