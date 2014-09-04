#include <Servo.h>

// output pin assignments
const byte PIN_M_LR   = 4;  // motor left/right
const byte PIN_M_LR_S = 3;  // pwm left/right
const byte PIN_M_IO   = 7;  // motor in/out
const byte PIN_M_IO_S = 11; // pwm in/out
const byte PIN_M_UD   = 8;  // motor up/down
const byte PIN_M_UD_S = 5;  // pwm up/down
const byte PIN_S_G    = 16; // servo gripper
const byte PIN_S_N    = 17; // servo neck
const byte PIN_LL     = 9;  // limit left
const byte PIN_LR     = 10; // limit right
const byte PIN_LI     = 12; // limit in
const byte PIN_LO     = 18; // limit out
const byte PIN_LU     = 14; // limit up
const byte PIN_LD     = 15; // limit down

// limit flag positions
const byte FLAG_LL = 7;
const byte FLAG_LR = 6;
const byte FLAG_LI = 5;
const byte FLAG_LO = 4;
const byte FLAG_LU = 3;
const byte FLAG_LD = 2;

// declare servos
Servo SERVO_G; // servo gripper
Servo SERVO_N; // servo neck

// holds the data, sent over serial
int dataValue = 0;

// limit switches state flags
// PIN_LL PIN_LR PIN_LI PIN_LO PIN_LU PIN_LD TBD TBD
byte limitFlags = B00000000; 

void setup() {
    Serial.begin(9600);
    // init pins
    pinMode(PIN_M_LR, OUTPUT);
    pinMode(PIN_M_IO, OUTPUT);
    pinMode(PIN_M_UD, OUTPUT);
    pinMode(PIN_S_G, OUTPUT);
    pinMode(PIN_S_N, OUTPUT);
    pinMode(PIN_LL, INPUT);
    pinMode(PIN_LR, INPUT);
    pinMode(PIN_LI, INPUT);
    pinMode(PIN_LO, INPUT);
    pinMode(PIN_LU, INPUT);
    pinMode(PIN_LD, INPUT);
    // init servos
    SERVO_G.attach(PIN_S_G);
    SERVO_N.attach(PIN_S_N);
}
 
void loop() {
    // check if any commands recvd
    if (Serial.available()) {
        char incomingChar = Serial.read();
        if (incomingChar >= '0' && incomingChar <= '9') {
            dataValue = (dataValue * 10) + (incomingChar - '0'); // accumulate the data value
        } else { // non-digit, so consider as a command code
            processCommand(incomingChar, dataValue);
            // reply back to calling device
            Serial.println(limitFlags);
            dataValue = 0; // reset            
        }
    }
    
    // scan limit switches, and save state into limitFlags
    bitWrite(limitFlags, FLAG_LL, digitalRead(PIN_LL));
    bitWrite(limitFlags, FLAG_LR, digitalRead(PIN_LR));
    bitWrite(limitFlags, FLAG_LI, digitalRead(PIN_LI));
    bitWrite(limitFlags, FLAG_LO, digitalRead(PIN_LO));
    bitWrite(limitFlags, FLAG_LU, digitalRead(PIN_LU));
    bitWrite(limitFlags, FLAG_LD, digitalRead(PIN_LD));
    //bitWrite(limitFlags, 1, digitalRead(PIN_));
    //bitWrite(limitFlags, 0, digitalRead(PIN_));
    
    // stop any motors if it's limits have been reached
    if (bitRead(limitFlags,FLAG_LL) || bitRead(limitFlags,FLAG_LR) {
        analogWrite(PIN_M_LR_S, 0);
    }
    if (bitRead(limitFlags,FLAG_LI) || bitRead(limitFlags,FLAG_LO) {
        analogWrite(PIN_M_IO_S, 0);
    }
    if (bitRead(limitFlags,FLAG_LU) || bitRead(limitFlags,FLAG_LD) {
        analogWrite(PIN_M_UD_S, 0);
    }
    
}

void processCommand(char commandValue, int dataValue) {
    switch (commandValue) {
        case '<':
            if (!bitRead(limitFlags,FLAG_LL)) {
                analogWrite(PIN_M_LR_S, dataValue);
                digitalWrite(PIN_M_LR, 0);
            }
        break;
        case '>':
            if (!bitRead(limitFlags,FLAG_LR)) {
                analogWrite(PIN_M_LR_S, dataValue);
                digitalWrite(PIN_M_LR, 1);
            }
        break;
        case ';':
            if (!bitRead(limitFlags,FLAG_LI)) {
                analogWrite(PIN_M_IO_S, dataValue);
                digitalWrite(PIN_M_IO, 0);
            }
        break;
        case ':':
            if (!bitRead(limitFlags,FLAG_LO)) {
                analogWrite(PIN_M_IO_S, dataValue);
                digitalWrite(PIN_M_IO, 1);
            }
        break;
        case '^':
            if (!bitRead(limitFlags,FLAG_LU)) {
                analogWrite(PIN_M_UD_S, dataValue);
                digitalWrite(PIN_M_UD, 0);
            }        
        break;
        case 'v':
            if (!bitRead(limitFlags,FLAG_LU)) {
                analogWrite(PIN_M_UD_S, dataValue);
                digitalWrite(PIN_M_UD, 1);
            }
        break;
        case '#':
            SERVO_G.write(dataValue);
        break;
        case '~':
            SERVO_N.write(dataValue);
        break;

        case '?':
            // no op, just reply back with flags
        break;
        
        case 'L': // read and list state of limit switches
            Serial.print("L: ");
            Serial.print(digitalRead(PIN_LL));
            Serial.print(" ");
            Serial.print(digitalRead(PIN_LR));
            Serial.print(" ");
            Serial.print(digitalRead(PIN_LI));
            Serial.print(" ");
            Serial.println(digitalRead(PIN_LO));
        break;

        default:
            // unknown cmd, stop all motors
            analogWrite(PIN_M_LR_S, 0);
            analogWrite(PIN_M_IO_S, 0);
            analogWrite(PIN_M_UD_S, 0);
            Serial.println("na");// reply back to calling device
        break;
    }
}