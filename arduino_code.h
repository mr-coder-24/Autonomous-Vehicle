#include <TinyGPS++.h>
TinyGPSPlus gps;
#define for_back_speed 150
#define left_ryt_speed 210
#define servo A7
#define motor_r_p 2
#define motor_r_m 3
#define motor_l_p 4
#define motor_l_m 5
#define ir_sensor_left A0
#define ir_sensor_right A1
#define echo A9     //Echo pin
#define trigger A8  //Trigger pin
int Set = 20;
int distance_L, distance_F, distance_R;

void setup() {
  pinMode(motor_r_p, OUTPUT);
  pinMode(motor_r_m, OUTPUT);
  pinMode(motor_l_p, OUTPUT);
  pinMode(motor_l_m, OUTPUT);

  pinMode(ir_sensor_left, INPUT);
  pinMode(ir_sensor_right, INPUT);
  Serial.begin(9600);
  Serial.println("welcome");
  pinMode(echo, INPUT);  // declare ultrasonic sensor Echo pin as input
  pinMode(trigger, OUTPUT);
  pinMode(servo, OUTPUT);
  for (int angle = 70; angle <= 140; angle += 5) {
    servoPulse(servo, angle);
  }
  for (int angle = 140; angle >= 0; angle -= 5) {
    servoPulse(servo, angle);
  }
  for (int angle = 0; angle <= 70; angle += 5) {
    servoPulse(servo, angle);
  }
  distance_F = Ultrasonic_read();
  delay(1000);
   Serial2.begin(9600);//gps
   Serial3.begin(9600);// bluetooth


}

void loop() {
   if (Serial2.available() > 0)
   {
     if (gps.encode(Serial2.read())) {
      if (gps.location.isValid()) {
        // Print latitude and longitude
        Serial3.print("Latitude: ");
        Serial3.println(gps.location.lat(), 6); // 6 decimal places for accuracy
        Serial3.print("Longitude: ");
        Serial3.println(gps.location.lng(), 6);
      } else {
        Serial3.println("Invalid location data");
      }
    }
   }
  distance_F = Ultrasonic_read();
  Serial.print("D F=");
  Serial.println(distance_F);


  if ((digitalRead(ir_sensor_left) == 1) && (digitalRead(ir_sensor_right) == 1)) {
    if (distance_F > Set) {
      move_forward();
     } else {
      Check_side();
     }
  }

  else if ((digitalRead(ir_sensor_left) == 1) && (digitalRead(ir_sensor_right) == 0)) {
    move_right();
    Serial.println("left");
  }

  else if ((digitalRead(ir_sensor_left) == 0) && (digitalRead(ir_sensor_right) == 1)) {
    move_left();
    Serial.println("right");
  }

  // else if ((digitalRead(ir_sensor_left) == 0) && (digitalRead(ir_sensor_right) == 0)) 
  // {
  //   robot_stop();
  // }
}


//***************************************

void servoPulse(int pin, int angle) {
  int pwm = (angle * 11) + 1000;  // Convert angle to microseconds
  digitalWrite(pin, HIGH);
  delayMicroseconds(pwm); 
  digitalWrite(pin, 0);
  delay(50);  // Refresh cycle of servo
}
//Ultrasonic_read**************************
long Ultrasonic_read() {
  digitalWrite(trigger, 0);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  long time = pulseIn(echo, HIGH);
  return time / 29 / 2;
}

void compareDistance() {
  if (distance_L < distance_R) {
    move_right();
    delay(800);
    move_forward();
    delay(1000);
    move_left();
    delay(800);
    move_forward();
    delay(1000);
     move_left();
    delay(800);
    move_forward();
    delay(1000);
    move_right();
    delay(800);
    move_forward();
    delay(10);

  }
  else if(distance_L == distance_R) 
  {
    move_right();
    delay(800);
    move_forward();
    delay(1000);
    move_left();
    delay(800);
    move_forward();
    delay(1000);
     move_left();
    delay(8000);
    move_forward();
    delay(1000);
    move_right();
    delay(800);
    move_forward();
    delay(10);
  }
   else {
    move_left();
    delay(800);
    move_forward();
    delay(1000);
    move_right();
    delay(800);
    move_forward();
    delay(1000);
    move_right();
    delay(800);
    move_forward();
    delay(1000);
    move_left();
    delay(800);
    move_forward();
    delay(10);
  }
}
void Check_side() {
  robot_stop();
  delay(1000);
  for (int angle = 70; angle <= 140; angle += 5) {
    servoPulse(servo, angle);
  }
  delay(300);
  distance_R = Ultrasonic_read();
  Serial.print("D R=");
  Serial.println(distance_R);
  delay(100);
  for (int angle = 140; angle >= 0; angle -= 5) {
    servoPulse(servo, angle);
  }
  delay(1000);
  distance_L = Ultrasonic_read();
  Serial.print("D L=");
  Serial.println(distance_L);
  delay(100);
  for (int angle = 0; angle <= 70; angle += 5) {
    servoPulse(servo, angle);
  }
  delay(300);
  compareDistance();
}


void move_forward() {
  analogWrite(motor_r_p, 0);
  analogWrite(motor_r_m, for_back_speed);
  analogWrite(motor_l_p, for_back_speed);
  analogWrite(motor_l_m, 0);
}

void move_backward() {
  analogWrite(motor_r_p, for_back_speed);
  analogWrite(motor_r_m, 0);
  analogWrite(motor_l_p, 0);
  analogWrite(motor_l_m, for_back_speed);
}

void move_left() {
  analogWrite(motor_r_p, 0);
  analogWrite(motor_r_m, left_ryt_speed);
  analogWrite(motor_l_p, 0);
  analogWrite(motor_l_m, left_ryt_speed);
}

void move_right() {
  analogWrite(motor_r_p, left_ryt_speed);
  analogWrite(motor_r_m, 0);
  analogWrite(motor_l_p, left_ryt_speed);
  analogWrite(motor_l_m, 0);
}

void robot_stop() {
  analogWrite(motor_r_p, 0);
  analogWrite(motor_r_m, 0);
  analogWrite(motor_l_p, 0);
  analogWrite(motor_l_m, 0);
}