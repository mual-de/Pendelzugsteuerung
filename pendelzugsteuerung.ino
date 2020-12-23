
#define MAX_SPEED 200
#define MIN_SPEED 80
#define ACCELERATION 30
#define RIGHT HIGH

typedef void (*StateFunction) ();

int right_sensor = 6;
int right_endposition_sensor = 7;
int left_sensor = 4;
int left_endposition_sensor = 3;

int start_stop = 2;

int gsm = 9;
int in1 = 10;
int in2 = 11;

uint8_t velocity = MAX_SPEED;


void init_state();
void drive_right_state();
void drive_left_state();
void stop_right_state();
void stop_left_state();
void start_right_state();
void start_left_state();
StateFunction last_state = drive_right_state;
void drive_right_direction(uint8_t velo);
void drive_left_direction(uint8_t velo);

void setup() {
  pinMode(gsm, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(right_sensor, INPUT_PULLUP);
  pinMode(right_endposition_sensor, INPUT_PULLUP);

  pinMode(left_sensor, INPUT_PULLUP);
  pinMode(left_endposition_sensor, INPUT_PULLUP);

  pinMode(start_stop, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("PENDELZUGSTEUERUNG V 1.0");
  Serial.println("------- mual-de -------");
  Serial.println("Stelle die Lok in die Mitte der Strecke!");

}

void loop() {
  init_state();

}

void init_state(){
  drive_right_direction(0);
  delay(200);
  int last_start_stop_val = digitalRead(start_stop);
  delay(100);
  int start_stop_val = digitalRead(start_stop);
  if (start_stop_val == LOW && start_stop_val == last_start_stop_val ) {
    Serial.println("Init State -> Last State");
    last_state();
  }
  delay(1500);
  }

void drive_right_direction(uint8_t velo) {
  Serial.print("Actual Velocity Right: ");
  Serial.println(velo);
  analogWrite(gsm, velo);
  digitalWrite(in1, RIGHT);  // motor into right direction
  digitalWrite(in2, !RIGHT);
}

void drive_left_direction(uint8_t velo) {
  analogWrite(gsm, velocity);
  digitalWrite(in1, !RIGHT);  // motor into right direction
  digitalWrite(in2, RIGHT);
}

void drive_right_state() {
  last_state = drive_right_state;
  int last_start_stop_val = digitalRead(start_stop);
  int last_right_sensor_val = digitalRead(right_sensor);
  delay(50);
  int start_stop_val = digitalRead(start_stop);
  int right_sensor_val = digitalRead(right_sensor);
  Serial.print("Sensor Right: ");
  Serial.println(right_sensor_val);
  if (start_stop_val == LOW && start_stop_val == last_start_stop_val ) {
    Serial.println("right_state -> init_state");
    init_state();
  } else if (right_sensor_val == LOW && right_sensor_val == last_right_sensor_val) {
    Serial.println("right_state -> stop_right_state");
    stop_right_state();
  }
  else {
    Serial.print("Fahre nach Rechts mit velo: ");
    Serial.println(velocity);
    drive_right_direction(velocity);
    drive_right_state();
  }
}

void stop_right_state() {
  last_state = stop_right_state;
  int last_start_stop_val = digitalRead(start_stop);
  int last_right_endposition_sensor_val = digitalRead(right_endposition_sensor);
  delay(50);
  int start_stop_val = digitalRead(start_stop);
  int right_endposition_sensor_val = digitalRead(right_endposition_sensor);
  if (start_stop_val == LOW && start_stop_val == last_start_stop_val ) {
    Serial.println("right_state -> init_state");
    init_state();
  } else if (right_endposition_sensor_val == LOW && right_endposition_sensor_val == last_right_endposition_sensor_val) {
    Serial.println("stop_right_state -> start_left_state");
    drive_right_direction(0);
    start_left_state();
  }
  else {
    if (velocity == MIN_SPEED) {
      if (velocity - MIN_SPEED < ACCELERATION) {
        velocity = MIN_SPEED;
      } else {
        velocity = velocity - ACCELERATION;
      }
    }
    drive_right_direction(velocity);
    stop_right_state();
  }
}

void start_left_state() {
  last_state = start_left_state;
  int last_start_stop_val = digitalRead(start_stop);
  int last_right_sensor_val = digitalRead(right_sensor);
  delay(50);
  int start_stop_val = digitalRead(start_stop);
  int right_sensor_val = digitalRead(right_sensor);
  if (start_stop_val == LOW && start_stop_val == last_start_stop_val ) {
    Serial.println("right_state -> init_state");
    init_state();
  } else if (right_sensor_val == LOW && right_sensor_val == last_right_sensor_val) {
    Serial.println("start_left_state -> drive_left_state");
    drive_left_state();
  }
  else {
    if (velocity == MAX_SPEED) {
      if (velocity - MAX_SPEED >= ACCELERATION) {
        velocity = MAX_SPEED;
      } else {
        velocity = velocity + ACCELERATION;
      }
    }
    drive_left_direction(velocity);
    start_left_state();
  }
}

void drive_left_state() {
  last_state = drive_left_state;
  int last_start_stop_val = digitalRead(start_stop);
  int last_left_sensor_val = digitalRead(left_sensor);
  delay(50);
  int start_stop_val = digitalRead(start_stop);
  int left_sensor_val = digitalRead(left_sensor);
  if (start_stop_val == LOW && start_stop_val == last_start_stop_val ) {
    Serial.println("left_state -> init_state");
    init_state();
  } else if (left_sensor_val == LOW && left_sensor_val == last_left_sensor_val) {
    Serial.println("left_state -> stop_left_state");
    stop_left_state();
  }
  else {
    drive_left_direction(velocity);
    drive_left_state();
  }

}


void stop_left_state() {
  last_state = stop_left_state;
  int last_start_stop_val = digitalRead(start_stop);
  int last_left_endposition_sensor_val = digitalRead(left_endposition_sensor);
  delay(50);
  int start_stop_val = digitalRead(start_stop);
  int left_endposition_sensor_val = digitalRead(left_endposition_sensor);
  if (start_stop_val == LOW && start_stop_val == last_start_stop_val ) {
    Serial.println("left_state -> init_state");
    init_state();
  } else if (left_endposition_sensor_val == LOW && left_endposition_sensor_val == last_left_endposition_sensor_val) {
    Serial.println("stop_left_state -> start_right_state");
    drive_left_direction(0);
    start_right_state();
  }
  else {
    if (velocity == MIN_SPEED) {
      if (velocity - MIN_SPEED < ACCELERATION) {
        velocity = MIN_SPEED;
      } else {
        velocity = velocity - ACCELERATION;
      }
    }
    drive_left_direction(velocity);
    stop_left_state();
  }
}

void start_right_state() {
  last_state = start_right_state;
  int last_start_stop_val = digitalRead(start_stop);
  int last_left_sensor_val = digitalRead(left_sensor);
  delay(50);
  int start_stop_val = digitalRead(start_stop);
  int left_sensor_val = digitalRead(left_sensor);
  if (start_stop_val == LOW && start_stop_val == last_start_stop_val ) {
    Serial.println("start_right_state -> init_state");
    init_state();
  } else if (left_sensor_val == LOW && left_sensor_val == last_left_sensor_val) {
    Serial.println("start_right_state -> drive_right_state");
    drive_right_state();
  }
  else {
    if (velocity == MAX_SPEED) {
      if (velocity - MAX_SPEED >= ACCELERATION) {
        velocity = MAX_SPEED;
      } else {
        velocity = velocity + ACCELERATION;
      }
    }
    drive_right_direction(velocity);
    start_right_state();
  }
}
