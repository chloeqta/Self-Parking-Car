// pin setup
#define trigPin1 12
#define echoPin1 13
#define trigPin2 11
#define echoPin2 10
#define trigPin3 9
#define echoPin3 6

const int Enable1Pin=5;  // for Arduino Uno must be one of the PWM pins: 3, 5, 6, 9, 10, and 11
const int Enable2Pin=3;
const int In1pin=7;
const int In2pin=8;
const int In3pin=2;
const int In4pin=4;

bool wall = false;
long duration1, duration2, duration3;
long distance1 = 0, distance2, distance3; 
double old_distance1 = 0, old_distance2;
const int depth = 30; // size of a parking spot
double delta_speed = 0;
const long speed_avg = 75; // ideal driving speed

 void setup() {
    Serial.begin(9600);
    // Setup motor 1 (controls wheel furthest from sensors):
    pinMode(In1pin,OUTPUT);
    pinMode(In2pin,OUTPUT);
    pinMode(Enable1Pin,OUTPUT);
    // Setup motor 2 (controls wheel closest to sensors):
    pinMode(In3pin,OUTPUT);
    pinMode(In4pin,OUTPUT);
    pinMode(Enable2Pin,OUTPUT);
    // Setup sensor 1 (side of car):
    pinMode(echoPin1, INPUT);
    pinMode(trigPin1, OUTPUT);
    digitalWrite(trigPin1, LOW);
    // Setup sensor 2 (side of car):
    pinMode(echoPin2, INPUT);
    pinMode(trigPin2, OUTPUT);
    digitalWrite(trigPin2, LOW);
    // Setup sensor 3 (front of car):
    pinMode(echoPin3, INPUT);
    pinMode(trigPin3, OUTPUT);
    digitalWrite(trigPin3, LOW);
 }
 
 // Motor 1 speed control
 double setspeed1(double speed) {
    digitalWrite(In1pin,HIGH);
    digitalWrite(In2pin,LOW);
    analogWrite(Enable1Pin,speed);
 }
 
 // Motor 2 speed control
 double setspeed2(double speed){
    digitalWrite(In3pin,HIGH);
    digitalWrite(In4pin,LOW);
    analogWrite(Enable2Pin,speed);
 }
  
// Wall detection: observes distance between side of car and wall. If depth exceeds const value 30, the observed area is considered large enough to be a parking spot.
 bool walldetect (int distance) {
    if (distance >= depth) { return (wall = true); }
    else { return (wall = false); }
 }

// Loop used to continuously check whether a parking spot has been reached
// No spot detected: continue driving parallel along the wall
// Spot detected: turn into parking spot and stop motors before wall collision
 void loop() {
    /* 
    */
    digitalWrite(trigPin3, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin3, LOW);
    duration3 = pulseIn(echoPin3, HIGH);
    distance3 = (duration3/2) / 29.15;
    
    walldetect(distance1-old_distance1);
    
    // if wall is undetected, adjust individual wheel speeds to maintain parallel driving
    if (wall == false) {
        digitalWrite(trigPin1, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin1, LOW);
        duration1 = pulseIn(echoPin1, HIGH);
        distance1 = (duration1/2) / 29.15;
        
        digitalWrite(trigPin2, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin2, LOW);
        duration2 = pulseIn(echoPin2, HIGH);
        distance2 = (duration2/2) / 29.15;
        
        if (distance1 >= 40 || distance2 >= 40) { return; }
        
        long h = distance1-distance2;
        if (h <= 10) {
            delta_speed = delta_speed - 0.1*h; //1/10 is subject to change
            Serial.println("Wheels Running");
            double speed1 = speed_avg - delta_speed;
            double speed2 = speed_avg + delta_speed;
            setspeed1(speed1);
            setspeed2(speed2);
            delay(30);
        }
        long old_distance1 = distance1;     
    }
    if (wall == true) {
        setspeed1(0); // Both motor speeds are stopped
        setspeed2(0);
        delay(10);
        setspeed1(75);
        setspeed2(75);
        delay(75);
        setspeed1(0);
        setspeed2(0);
        delay(10);
        setspeed1(85); // One motor turns on to facilitate the turn into parking spot
        delay(900);
        while (distance3 > 7) { // 
            digitalWrite(trigPin3, HIGH);
            delayMicroseconds(10);
            digitalWrite(trigPin3, LOW);
            duration3 = pulseIn(echoPin3, HIGH);
            distance3 = (duration3/2) / 29.15;
            setspeed1(95);
            setspeed2(95);
            delay(50);
        }  
        setspeed1(0);
        setspeed2(0);
        delay(10000);
    }
 }
     

       
