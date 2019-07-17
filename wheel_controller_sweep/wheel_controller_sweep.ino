// Pins
//------------------------------------------------------------
#define START_TRIALS 13
#define ENC_A 2 // Needs to be an interrupt pin
#define ENC_B 3 // Needs to be an interrupt pin
#define ENC_I 4 // Not actually used

#define SCOPE_OUT 23
#define TRIAL_OUT 25
#define CS_OUT 27
#define US_OUT 29
#define WAS_STILL_OUT 5

// Behavioral parameters
//------------------------------------------------------------
#define SCOPE_PRE 10000  // Turn on scope prior to trial start, ms
#define SCOPE_POST 10000 // Turn off scope after trial start

#define CS_DURATION 4000 // ms
#define CS_GRACE 3500 // Grace period, ms. US will be disabled during 
                      //  the initial CS_GRACE of a CS presentation

#define ITI_MIN 22000 // ms, Needs to be greater than SCOPE_PRE + SCOPE_POST
#define ITI_MAX 30000 // ms

#define CS_POLL_RATE 250 // ms

// The velocity calculations assume:
//  - 5 cm wheel radius
//  - 500 encoder clicks per full rotation
#define STILL_THRESHOLD 10 // Corresponds to ~2.5 cm/s over 0.25 sec
#define RUNNING_THRESHOLD 40 // Corresponds to ~10 cm/s over 0.25 sec

// FSM definitions
//------------------------------------------------------------
#define S_IDLE 0
#define S_PRE 1
#define S_BEGIN_TRIAL 2
#define S_CS 3
#define S_END_TRIAL 5
#define S_POST 6
#define S_ITI 7

int state = S_IDLE;
int cs_elapsed_time;
bool was_still;
bool enable_cs;
bool enable_us;

void setup() {
  // put your setup code here, to run once:
  pinMode(START_TRIALS, INPUT_PULLUP);
  pinMode(ENC_A, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENC_A), count_A, RISING);
  pinMode(ENC_B, INPUT);
  pinMode(ENC_I, INPUT);

  pinMode(SCOPE_OUT, OUTPUT);
  pinMode(TRIAL_OUT, OUTPUT);
  pinMode(CS_OUT, OUTPUT);
  pinMode(US_OUT, OUTPUT);
  pinMode(WAS_STILL_OUT, OUTPUT);
  
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
    digitalWrite(SCOPE_OUT, 0);
    digitalWrite(TRIAL_OUT, 0);
    digitalWrite(CS_OUT, 0);
    digitalWrite(US_OUT, 0);
    digitalWrite(WAS_STILL_OUT, 0);
 
    state = S_IDLE;
  }
  else {
    // Implement FSM
    switch (state) {
      case S_IDLE:
        // After the START_TRIALS signals is given, wait 2 sec prior
        // to launching trials
        delay(2000);
        state = S_PRE;
        break;
        
      case S_PRE:
        digitalWrite(SCOPE_OUT, 1);
        delay(SCOPE_PRE);
        state = S_BEGIN_TRIAL;
        break;

      case S_BEGIN_TRIAL:
        enable_cs = true;
        enable_us = true;
        was_still = false;
      
        digitalWrite(TRIAL_OUT, 1);
        state = S_CS;
        break;

      case S_CS:
        cs_elapsed_time = 0;
        
        while (cs_elapsed_time < CS_DURATION) {
          digitalWrite(CS_OUT, enable_cs);
          digitalWrite(US_OUT, (cs_elapsed_time >= CS_GRACE) && enable_us);
          digitalWrite(WAS_STILL_OUT, was_still);
          
          steps_fwd = 0;
          steps_bwd = 0;
          delay(CS_POLL_RATE);
          if (abs(steps_fwd) + abs(steps_bwd) < STILL_THRESHOLD) {
            was_still = true;
          }
          if (was_still && (steps_fwd - steps_bwd > RUNNING_THRESHOLD)) {
            enable_cs = false;
            enable_us = false;
          }
          
          cs_elapsed_time += CS_POLL_RATE; 
        }
        state = S_END_TRIAL;
        break;

      case S_END_TRIAL:
        digitalWrite(US_OUT, 0);
        digitalWrite(CS_OUT, 0);
        digitalWrite(TRIAL_OUT, 0);
        digitalWrite(WAS_STILL_OUT, 0);
        state = S_POST;
        break;

      case S_POST:
        delay(SCOPE_POST);
        digitalWrite(SCOPE_OUT, 0);
        state = S_ITI;
        break;

      case S_ITI:
        // ITI defined as time between _behavioral_ trials
        delay(random(ITI_MIN, ITI_MAX) - (SCOPE_PRE+SCOPE_POST));
        state = S_PRE;
        break;
 
      default:
        state = S_IDLE;
    }
  }
}
