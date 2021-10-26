/* From sample code on usinig the HC-SR04
*/
// Trig & Echo pins
const int trigPin = 12,
          echoPin = 11;

int duration = 0, distance = 0;

void setup() {
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

 
  Serial.begin(9600);
   while(!Serial);
   Serial.printf("Printer initialized\n");
   delay(500);
}

void loop() {
  // Clear trigPin/Set LOW
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);

  //trigger sensor by setting tripPin high for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echoPin, pulseIn() returns the dur length of pulse
  //  in microseconds.
  duration = pulseIn(echoPin, HIGH);
  // Calculate the dist:
  distance = duration *0.034 / 2;

  // Print the dist to Serial Monitor:
  Serial.printf("Distance = %icm \n", distance);

  delay(50);
}
