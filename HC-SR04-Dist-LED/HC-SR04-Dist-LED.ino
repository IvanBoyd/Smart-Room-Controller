/* From sample code on usinig the HC-SR04
 *  ivan boyd
 *  program to hook up HC-SR04 distance sensor and obtain measurable
 *  values
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
 
  distance = getDist(trigPin, duration, distance);
  // Print the dist to Serial Monitor:
  Serial.printf("Distance = %icm \n", distance);
  delay(50);
}


int getDist(int _trigPin,int _duration,int _distance) {
 // Clear trigPin/Set LOW
  digitalWrite(_trigPin, LOW);
  delayMicroseconds(5);

  //trigger sensor by setting tripPin high for 10 microseconds
  digitalWrite(_trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trigPin, LOW);

  // Read the echoPin, pulseIn() returns the dur length of pulse
  //  in microseconds.
  _duration = pulseIn(echoPin, HIGH);
  // Calculate the dist:
  _distance = _duration *0.034 / 2;
  return _distance;
}
