#include <Servo.h>

// Introducing Servo Motors with Labels

Servo armA;
Servo armB;
Servo tailC;

Servo platformArmD;
Servo platformArmE;
Servo platformArmF;


// Assigning Each Servo to Pins on Leonardo

const int pinD = 2;
const int pinE = 3;
const int pinF = 4;

const int pinTail = 9;

const int pinA = 10;
const int pinB = 11;

const int buzzerPin = 12;

// Placeholder Angles for the Hands and Tail's State Positions

const int platformUp = 0;
const int platformDown = 90;

const int armDown = 45;
const int armTop = 130;
const int armStartPosition = 80;

const int tailClosed = 100;
const int tailOpen = 170;


// Placeholders Used to Prevent Frantic Class Switching

int currentClass = 0;
int previousClass = 0;


/* Placeholders for Arm Movement
Tracks the last angle commanded to both arms.
Sets value to their known physical position at startup. */

const int moveTop = 0;
const int syncFlap = 1;    // integers used to determine which arm protocol to follow based on class
const int altFlap = 2;

int armMotion = moveTop;   
int armPositionA = armStartPosition;     // Create individual positions for each hand to help with sync vs alternate flapping
int armPositionB = armStartPosition;

int armTargetA = armStartPosition;
int armTargetB = armStartPosition;

bool class2PlatformRaised = false;       // Placeholder used as an indicator for when platform is moved in class 2 (used for aiding subsequent events in the class)

// Used for tailoring arm movement speed
unsigned long lastArmStep = 0;
const unsigned long armStepInterval = 100;  // Lower value --> Faster arm movement


/* Placeholders for Tail Movement
Helps organize variables for programming tail position and movement speed */

int tailPosition = tailClosed;
int tailDirection = 1;

unsigned long lastTailMove = 0;
const unsigned long tailSpeed = 10;


bool tailClapStarted = false;  // Used to 

void resetTailClaps() {
  tailClapStarted = false;
}

/* Placeholders for Happy and Sad Song Notes */

// Happy Jingle Notes
const int happyNotes[] = { 2000, 0, 2300, 0, 2600, 0, 2300, 0, 2100, 0, 2400, 0, 2700, 0 };
const unsigned long happyDurations[] = { 130, 35, 130, 35, 130, 35, 130, 35, 130, 35, 130, 35, 180, 200 };

// Sad Jingle Notes
const int sadNotes[] = { 2900, 0, 2600, 0, 2300, 0, 2100, 0, 2500, 0, 2200, 0, 2000 };
const unsigned long sadDurations[] = { 160, 35, 160, 35, 180, 35, 250, 120, 130, 35, 150, 35, 500 };

// Parameters to used when calling startSong() command.
const int* activeNotes = NULL;
const unsigned long* activeDurations = NULL;
int songLength = 0;
int songNoteIndex = 0;
bool songPlaying = false;
unsigned long songDeadline = 0;


void setup() {
  Serial.begin(115200);  // Be sure to set this baud rate to the same as Processing

  platformArmD.attach(pinD);
  platformArmE.attach(pinE);
  platformArmF.attach(pinF);

  tailC.attach(pinTail);

  armA.attach(pinA);
  armB.attach(pinB);

  armA.write(armPositionA);
  armB.write(armPositionB);

  Serial.println("Arduino ready - waiting for Processing.");
}


void loop() {
  while (Serial.available() > 0) {      // Processing sends raw class bytes to Arduino: 1, 2, or 3
    int incomingClass = Serial.read();  // Arduino reads byte form proessing here

    if (incomingClass >= 1 && incomingClass <= 3) {   // Only adjusts this parameter if a correct class gets sent (failsafe)
      currentClass = incomingClass;
    }
  }


  if (currentClass != previousClass) {  // Set class behavior only when the class changes
    switch (currentClass) {             // Behavior Switchboard. Changes behaviors in a single loop.

        
      /* CLASS 1: Seal Flaps Arms, Keeps Platform Low, Claps Tail, Plays Happy Song */
      case 1:
        armMotion = syncFlap;  // Sets the arm flap protocol. Both arms first flap will move down.
        armTargetA = armDown;   
        armTargetB = armDown;

        resetTailClaps();                  // 

        platformArmD.write(platformDown);
        platformArmE.write(platformDown);  // Sends Platform Arms into Down Position
        platformArmF.write(platformDown);

        startSong(happyNotes, happyDurations, sizeof(happyNotes) / sizeof(happyNotes[0])); // Plays Happy Song immediately after balancing the platform


        /* ARDUINO SERIAL MONITOR CONFIRMAITONS */
        Serial.println("COLOR: RED");
        Serial.println("CLASS: 1 | BEHAVIOR: BALANCE CENTER");
        break;

      /* CLASS 2: Seal slowly raises arms and closes tail; Launches ball and opens tail when arms reach the top, plays sad song */
      case 2:
        armMotion = moveTop;  // Sets the arm raise protocol. Both arms will move up.
        armTargetA = armTop;
        armTargetB = armTop;

        class2PlatformRaised = false;   

        platformArmD.write(platformDown);  // Keep D and E at the bottom until A and B reach the top.
        platformArmE.write(platformDown);
        platformArmF.write(platformDown);

        // Closes the tail immediately when class 2 gets declared
        tailPosition = tailClosed;        
        tailDirection = -1;
        tailClapStarted = false;
        tailC.write(tailClosed);

        /* ARDUINO SERIAL MONITOR CONFIRMAITONS */
        Serial.println("COLOR: GREEN");
        Serial.println("CLASS: 2 | BEHAVIOR: THROW BALL");
        break;

      /* CLASS 3: Seal Alternates Waving Arms, Keeps Ball Platform High, Claps Tail, plays happy song */
      case 3:
        // Sets the alternate flap protocal. The arms will move in opposite directions.
        armMotion = altFlap;
        armTargetA = armTop;
        armTargetB = armDown;

        resetTailClaps();

        // Raises the platform when class is called.
        platformArmD.write(platformUp);
        platformArmE.write(platformUp);
        platformArmF.write(platformUp);

        startSong(happyNotes, happyDurations, sizeof(happyNotes) / sizeof(happyNotes[0]));  // Plays Happy Song immediately after raising the platform

        /* ARDUINO SERIAL MONITOR CONFIRMAITONS */
        Serial.println("COLOR: BLUE");
        Serial.println("CLASS: 3 | BEHAVIOR: RAISE BALL");
        break;
    }

    previousClass = currentClass;  // IMPORTANT: Must sit outside of switchboard loop. Changes last class value stored, preventing constant repeat commands sent.
  }

  moveArms();     // Arm move command sits outside switchboard but inside loop(). Function is designed to move the limbs one step each time its called.
  updateSong();   // Constantly checking if song needs to be played, and if so, play the next note in the songs index.


  // CONTINUATION OF CLASS 2'S ACTIONS
  if (currentClass == 2 && armPositionA == armTop && armPositionB == armTop && !class2PlatformRaised) {  // Next actions on class 2 don't happen until arms reach the top
    platformArmD.write(platformUp);
    platformArmE.write(platformUp);  // Launches the ball off platform by commanding 2 platform servo to shift up. Happens after arms reach top position.
    tailC.write(tailOpen);   // Opens tail at the same time the platform moves

    class2PlatformRaised = true;  // Important placeholder to ensure the sad Song doesn't constantly start over.

    startSong(sadNotes, sadDurations, sizeof(sadNotes) / sizeof(sadNotes[0]));  // Plays Sad Song immediately after throwing the ball. 
  }

  if (currentClass == 1 || currentClass == 3) {   // Constantly runs the clapping tail function when class 1 and 3 are called. Again, function is desgned to move one step each time its called.
    clapTail();
  }
}


// =====================================================
// SLOW MAIN ARM MOVEMENT
// =====================================================

void moveArms() {
  unsigned long currentTime = millis();

  if (currentTime - lastArmStep < armStepInterval) {
    return;
    }

  lastArmStep = currentTime;

  if (armPositionA < armTargetA) {
    armPositionA++;
  } else if (armPositionA > armTargetA) {
    armPositionA--;
  }

  if (armPositionB < armTargetB) {
    armPositionB++;
  } else if (armPositionB > armTargetB) {
    armPositionB--;
  }

  armA.write(armPositionA);
  armB.write(armPositionB);

  if (armPositionA == armTargetA && armPositionB == armTargetB) {
    if (armMotion == syncFlap) {
      if (armTargetA == armTop) {
        armTargetA = armDown;
        armTargetB = armDown;
      } else {
        armTargetA = armTop;
        armTargetB = armTop;
      }
    } else if (armMotion == altFlap) {
      int oldTargetA = armTargetA;
      armTargetA = armTargetB;
      armTargetB = oldTargetA;
    }
  }
}

/* TAIL CLAPPING FUNCTIONS */
// These function are called in the switchboard and in loop ()
// Calling in loop() allows the claping to continue indefinitely (as the function moves the tail one degree every time its called)

void clapTail() {
  unsigned long currentTime = millis();

  // Begin each set of claps from the open position
  if (!tailClapStarted) {
    tailPosition = tailOpen;    // Sets the starting position of the tail to open state any time we call the clap function
    tailDirection = -1;
    tailC.write(tailPosition);
    lastTailMove = currentTime;
    tailClapStarted = true;
  }

  // Continuously changes the position of the tail, creating the clap motion. Switches direction when the tail reaches its open and closed angle limits.
  if (currentTime - lastTailMove >= tailSpeed) {
    lastTailMove = currentTime;
    tailPosition += 3 * tailDirection;  // Change the coefficient to adjust clapping speed. Higher number --> Faster clap

    if (tailDirection == -1 && tailPosition <= tailClosed) {   // Ensures servos don't overextend on the clap functions
      tailPosition = tailClosed;
      tailDirection = 1;
    } else if (tailDirection == 1 && tailPosition >= tailOpen) {
      tailPosition = tailOpen;
      tailDirection = -1;
    }

    tailC.write(tailPosition);
  }
}

// Song Commands
void startSong(const int notes[], const unsigned long durations[], int count) {
  activeNotes = notes;
  activeDurations = durations;
  songLength = count;
  songNoteIndex = 0;
  songPlaying = (count > 0);

  if (songPlaying) {
    startCurrentNote();
  }
}

void startCurrentNote() {
  int frequency = activeNotes[songNoteIndex];                 // Pulls the next note's pitch in the jingle
  unsigned long duration = activeDurations[songNoteIndex];    // Pulls the next note's corresponding length in the jingle

  if (frequency == 0) {
    noTone(buzzerPin);    // Essentially creates the pauses in the jingle
  } else {
    tone(buzzerPin, frequency, duration);
  }

  songDeadline = millis() + duration;
}

void updateSong() {
  if (!songPlaying || millis() < songDeadline) {
    return;
  }
  songNoteIndex++;

  if (songNoteIndex >= songLength) {      // Ensures buzzer stops playing noise if it reaches the end of the jingle
    songPlaying = false;
    noTone(buzzerPin);
    return;
  }
  startCurrentNote();
}
