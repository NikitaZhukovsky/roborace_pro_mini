#pragma GCC optimization("Ofast")
#include "big_car.h"
#include "tests.h"
#define LOOP_TIME 40;

Car car;

void go_back() {
  car.write_speed(0);
  delay(80);
  car.write_speed(-250);
  delay(400);
  car.write_speed(0);
  delay(80);
}

void go_back_2() {
  car.write_speed(0);
  delay(80);
  car.write_speed(-250);
  delay(1000);
  car.write_speed(0);
  delay(80);
}


void work() {
  int* s = car.read_sensors();

  int front_dist = (s[1]);
  int diff;

  bool front_clear = s[1] < 1100;

  if ( s[0] > 1200 && s[2] > 1200) {
    diff = 800;
  }
  else {
    diff = (s[2] - s[0])+100;
  }

  float coef;
  float spd;
  switch (front_clear) {
    case 0:
      coef = 0.6; spd = 190;
      break;
    case 1:
      coef = 1.7; spd = 100;
      break;
  }
  car.write_steer(diff * coef);
  car.write_speed(spd);
 

  bool close_front = s[1] < 301;
  
  static int stuck_time = 0;
  
  if (close_front) {
    stuck_time += 1;
  } else {
    stuck_time = 0;
  }

  if (stuck_time > 25) {
    car.write_steer(0);
    go_back();
    car.write_speed(250);
    stuck_time = 0;
  }


  // Тут код для обнаружения разворотов
  static float turns = 0;
  turns +=  diff/ -1000;
  turns = constrain(turns, -400, 4);

  if (turns < -200.0) {
    car.write_speed(0);
    delay(100);
    car.write_steer(1000);
    delay(20);
    go_back_2();
    car.write_steer(-1000);
    car.write_speed(250);
    turns = 0.0; 
  }
}

unsigned long start_time = 0;
unsigned long next_loop = 0;

void setup() {
  Serial.begin(9600);
  car.init();
  delay(3000);
}

void loop() {
  unsigned long cur_time = millis();
  if (cur_time > next_loop) {
    next_loop = next_loop + LOOP_TIME;
    next_loop = max(cur_time, next_loop);
    work();
  }
}
