#include <Servo.h>
#include <Arduino.h>

Servo myservo1;  // Entrance gate servo
Servo myservo2;  // Exit gate servo

// IR sensor pins
#define parking1_slot1_ir_s  4
#define parking1_slot2_ir_s  5
#define parking1_slot3_ir_s  6
#define parking2_slot1_ir_s  7
#define parking2_slot2_ir_s  8
#define parking2_slot3_ir_s  9

#define entrance_gate 12
#define exit_gate     13

int pos1 = 90;  // Entrance gate angle
int pos2 = 90;  // Exit gate angle
int S1=0, S2=0, S3=0, S4=0, S5=0, S6=0;
int slot = 6;

String sensor1, sensor2, sensor3, sensor4, sensor5, sensor6;
String cdata = "";

void setup() {
  Serial.begin(9600);

  pinMode(parking1_slot1_ir_s, INPUT);
  pinMode(parking1_slot2_ir_s, INPUT);
  pinMode(parking1_slot3_ir_s, INPUT);
  pinMode(parking2_slot1_ir_s, INPUT);
  pinMode(parking2_slot2_ir_s, INPUT);
  pinMode(parking2_slot3_ir_s, INPUT);

  pinMode(entrance_gate, INPUT);
  pinMode(exit_gate, INPUT);

  myservo1.attach(3);
  myservo2.attach(11);
  myservo1.write(0);
}

void Read_Sensor() {
  S1 = digitalRead(parking1_slot1_ir_s) == LOW ? 1 : 0;
  S2 = digitalRead(parking1_slot2_ir_s) == LOW ? 1 : 0;
  S3 = digitalRead(parking1_slot3_ir_s) == LOW ? 1 : 0;
  S4 = digitalRead(parking2_slot1_ir_s) == LOW ? 1 : 0;
  S5 = digitalRead(parking2_slot2_ir_s) == LOW ? 1 : 0;
  S6 = digitalRead(parking2_slot3_ir_s) == LOW ? 1 : 0;

  slot = 6 - (S1 + S2 + S3 + S4 + S5 + S6);

  Serial.println("========== PARKING STATUS ==========");
  if (slot == 0) {
    Serial.println("Sorry, No Parking Space Available.");
  } else {
    Serial.print("Available Slots: ");
    Serial.println(slot);
  }

  Serial.print("S1: "); Serial.println(S1 ? "Filled" : "Empty");
  Serial.print("S2: "); Serial.println(S2 ? "Filled" : "Empty");
  Serial.print("S3: "); Serial.println(S3 ? "Filled" : "Empty");
  Serial.print("S4: "); Serial.println(S4 ? "Filled" : "Empty");
  Serial.print("S5: "); Serial.println(S5 ? "Filled" : "Empty");
  Serial.print("S6: "); Serial.println(S6 ? "Filled" : "Empty");
  Serial.println("====================================");

  delay(1000);
}

void loop() {
  Read_Sensor();

  p1slot1(); p1slot2(); p1slot3();
  p2slot1(); p2slot2(); p2slot3();
  gates();

  // Sensor packet for optional logging
  cdata = sensor1 + "," + sensor2 + "," + sensor3 + "," + sensor4 + "," + sensor5 + "," + sensor6;
  Serial.print("Sensor Packet: ");
  Serial.println(cdata);

  delay(2000);
}

// Slot sensing and string encoding
void p1slot1() { sensor1 = digitalRead(parking1_slot1_ir_s) == LOW ? "255" : "0"; delay(10); }
void p1slot2() { sensor2 = digitalRead(parking1_slot2_ir_s) == LOW ? "255" : "0"; delay(10); }
void p1slot3() { sensor3 = digitalRead(parking1_slot3_ir_s) == LOW ? "255" : "0"; delay(10); }
void p2slot1() { sensor4 = digitalRead(parking2_slot1_ir_s) == LOW ? "255" : "0"; delay(10); }
void p2slot2() { sensor5 = digitalRead(parking2_slot2_ir_s) == LOW ? "255" : "0"; delay(10); }
void p2slot3() { sensor6 = digitalRead(parking2_slot3_ir_s) == LOW ? "255" : "0"; delay(10); }

void gates() {
  if (digitalRead(exit_gate) == LOW) {
    for (pos2 = 90; pos2 <= 180 ; pos2++) {
      myservo2.write(pos2);
      delay(15);
    }
    delay(1000);
    for (pos2 = 180; pos2 >= 90; pos2--) {
      myservo2.write(pos2);
      delay(15);
    }
  }

  if ((S1 + S2 + S3 + S4 + S5 + S6) < 6 && digitalRead(entrance_gate) == LOW) {
    for (pos1 = 0; pos1 <= 90; pos1++) {
      myservo1.write(pos1);
      delay(15);
    }
    delay(1000);
    for (pos1 = 90; pos1 >= 0; pos1--) {
      myservo1.write(pos1);
      delay(15);
    }
  }
}
