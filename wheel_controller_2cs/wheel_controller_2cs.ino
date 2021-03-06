// Pins
//------------------------------------------------------------
#define START_TRIALS 13
#define ENC_A 2 // Needs to be an interrupt pin
#define ENC_B 3 // Needs to be an interrupt pin
#define ENC_I 4 // Not actually used

#define TRIAL_OUT 25
#define CS_OUT 27
#define US_OUT 29
#define ITI_OUT 5
#define FORCED_CS_OUT 23

// Behavioral parameters
//------------------------------------------------------------
#define INITIAL_DELAY 2000 // Delay after turning on manual switch, ms

#define CS_DURATION 4000 // ms
#define US_DURATION 500 // ms, Applied at end of CS_DURATION
#define CS_GRACE CS_DURATION-US_DURATION // US will not be applied during this time

#define ITI_MIN 5000 // ms, Needs to be larger than STILL_DURATION
#define ITI_MAX 15000

#define CS_POLL_RATE 250 // ms

// The velocity calculations assume:
//  - 5 cm wheel radius
//  - 250 encoder clicks per full rotation
// - RUNNING_THRESHOLD 10 (Day 1) 15 (Day 2) 20 (Day 3+)
#define RUNNING_THRESHOLD 10 // Corresponds to >10 cm/s over 0.25 sec

// CS will be given once the animal has remained below "STILL_THRESHOLD"
// over "STILL_DURATION"
#define STILL_DURATION 1000 // ms
#define STILL_THRESHOLD 10 // Corresponds to X cm/s over STILL_DURATION

// If the mouse has not satisfied the "still" condition in "FORCED_TRIAL_THRESHOLD"
// milliseconds, then apply forced trial.
#define FORCED_TRIAL_THRESHOLD 25000 // ms
#define FORCED_CS_DURATION 2000

// FSM definitions
//------------------------------------------------------------
#define S_IDLE 0
#define S_WAIT_FOR_STILL 1
#define S_CS 3
#define S_FORCED_CS 4
#define S_END_TRIAL 5
#define S_ITI 7

int state = S_IDLE;
int elapsed_time;
bool enable_cs;
bool enable_us;

void setup() {
  // put your setup code here, to run once:
  pinMode(START_TRIALS, INPUT_PULLUP);
  pinMode(ENC_A, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENC_A), count_A, RISING);
  pinMode(ENC_B, INPUT);
  pinMode(ENC_I, INPUT);

  pinMode(TRIAL_OUT, OUTPUT);
  pinMode(CS_OUT, OUTPUT);
  pinMode(US_OUT, OUTPUT);
  pinMode(ITI_OUT, OUTPUT);
  pinMode(FORCED_CS_OUT, OUTPUT);
  
  state = S_IDLE;
}

// Encoder counters
volatile int steps_fwd = 0;
volatile int steps_bwd = 0;

void count_A() {
  if (digitalRead(ENC_B))
    steps_fwd++;
  else
    steps_bwd++;
}

void loop() {
  bool trials_enabled = !digitalRead(START_TRIALS);
  if (!trials_enabled) {
    // Controller should be disabled
    digitalWrite(TRIAL_OUT, 0);
    digitalWrite(CS_OUT, 0);
    digitalWrite(US_OUT, 0);
    digitalWrite(ITI_OUT, 0);
    digitalWrite(FORCED_CS_OUT, 0);
 
    state = S_IDLE;
  }
  else {
    // Implement FSM
    switch (state) {
      case S_IDLE:
        // After the START_TRIALS signals is given, wait before launching trials
        delay(INITIAL_DELAY);
        state = S_WAIT_FOR_STILL;
        break;
        
      case S_WAIT_FOR_STILL:
        // By "default" a forced trial will be given unless the animal satisfies
        // the still condition, as below
        state = S_FORCED_CS;
        
        // Check for "still condition" before the time is up
        elapsed_time = 0;
        while (elapsed_time < FORCED_TRIAL_THRESHOLD) {
          steps_fwd = 0;
          steps_bwd = 0;
          delay(STILL_DURATION);
          if (abs(steps_fwd) + abs(steps_bwd) < STILL_THRESHOLD) {
            state = S_CS;
            break; // Stop waiting
          }
          elapsed_time += STILL_DURATION;
        }

        digitalWrite(TRIAL_OUT, 1);
        break;

      case S_CS:
        enable_cs = true;
        enable_us = true;
        
        elapsed_time = 0;
        while (elapsed_time < CS_DURATION) {
          digitalWrite(CS_OUT, enable_cs);
          digitalWrite(US_OUT, (elapsed_time >= CS_GRACE) && enable_us);
          
          steps_fwd = 0;
          steps_bwd = 0;
          delay(CS_POLL_RATE);
          if (steps_fwd - steps_bwd > RUNNING_THRESHOLD) {
            enable_cs = false;
            enable_us = false;
          }
          
          elapsed_time += CS_POLL_RATE; 
        }
        state = S_END_TRIAL;
        break;

      case S_FORCED_CS:
        digitalWrite(FORCED_CS_OUT, 1);
        delay(FORCED_CS_DURATION - US_DURATION);
        digitalWrite(US_OUT, 1);
        delay(US_DURATION);
        
        state = S_END_TRIAL;
        break;

      case S_END_TRIAL:
        digitalWrite(US_OUT, 0);
        digitalWrite(CS_OUT, 0);
        digitalWrite(FORCED_CS_OUT, 0);
        digitalWrite(TRIAL_OUT, 0);
        state = S_ITI;
        break;

      case S_ITI:
        // ITI defined as time between _behavioral_ trials
        digitalWrite(ITI_OUT, 1);
        delay(random(ITI_MIN, ITI_MAX) - STILL_DURATION);
        digitalWrite(ITI_OUT, 0);
        state = S_WAIT_FOR_STILL;
        break;
 
      default:
        state = S_IDLE;
    }
  }
}
