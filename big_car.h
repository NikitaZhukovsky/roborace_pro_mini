#include <Servo.h>

#define SENSOR_AMOUNT 3

#define SERVO_PIN 5

#define INA 9
#define INB 10
#define PWM 11

#define NEUTRAL_POINT 89
#define MIN_POINT 23
#define MAX_POINT 150 // 130 start

#define NEUTRAL_SPEED 90
#define MIN_SPEED 94 // was 100
#define MAX_SPEED 103 // was 200
#define MIN_BSPEED 85

#define IR_PIN1 A6
#define IR_PIN2 A2
#define IR_PIN3 A1


class Car {
  private:
  public:
    Servo steer_servo;
    Servo motor;
  
    float last_error;

    const int sensor_amount = SENSOR_AMOUNT;
    void write_speed(int s);
    void write_steer(int s);
    int* read_sensors();
    void init();
};

const float pi = 3.141592653589;

void Car::init() {

  steer_servo.attach(SERVO_PIN);
  pinMode(INA, OUTPUT);   
  pinMode(INB, OUTPUT);  
  pinMode(PWM, OUTPUT);

  pinMode(IR_PIN1, INPUT);
  pinMode(IR_PIN2, INPUT);
  pinMode(IR_PIN3, INPUT);
  
  pinMode(12, OUTPUT);
  pinMode(2, INPUT);
 

  write_steer(0);
  write_speed(0);
}

void Car::write_speed(int s){
  s = constrain(s, -1000, 1000);
  s = map(s, -1000, 1000, -255, 255);
  if (s > 0){
    digitalWrite(INA, HIGH);
    digitalWrite(INB, LOW);
  } else {
    digitalWrite(INA, LOW);
    digitalWrite(INB, HIGH);
  }
  analogWrite(PWM, abs(s));
}

const int SAMPLE_NUM = 3;

int compare_int (const void * a, const void * b) {
  if ( *(int*)a < * (int*)b ) return -1;
  if ( *(int*)a == *(int*)b ) return 0;
  if ( *(int*)a > *(int*)b ) return 1;
  return 0;
}

float read_sensor(int Sensor, int model) {
  int raw[SAMPLE_NUM];
  for (int i = 0; i < SAMPLE_NUM; i++) {
    raw[i] = analogRead(Sensor);
  }
  qsort(raw, SAMPLE_NUM, sizeof(int), compare_int);

  float sensorValue = (raw[SAMPLE_NUM/2]);
  return (187754 * pow(sensorValue, -1.51)) * 10.0;
}

int* Car::read_sensors() {
  static int values[SENSOR_AMOUNT];

  values[0] = read_sensor(IR_PIN1, -1);
  values[1] = read_sensor(IR_PIN2, -1);
  values[2] = read_sensor(IR_PIN3, -1);

  return values;
}

void Car::write_steer(int s) {
  s = s * -1;
  s = constrain(s, -1000, 1000);
  if (s < 0) {
    s = map(s, -1000, 0, MIN_POINT, NEUTRAL_POINT);
  } else {
    s = map(s, 0, 1000, NEUTRAL_POINT, MAX_POINT);
  }
  steer_servo.write(s);
}
