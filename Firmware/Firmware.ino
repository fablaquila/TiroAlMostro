#include "serialCommunication.h"
#include "joystick.h"
#include "moles.h"
#include "microsmooth.h"
#include "servoCalibration.h"

// We need this here otherwise moles.h doesn't compile (the compiler can't find
// Servo.h)
#include <Servo.h>

// The baud rate to use for communication with computer
const long baudRate = 115200;

// Pin numbers: Joystick
const int joystickP1 = 11;
const int joystickP2 = 12;
const int joystickX = A0;
const int joystickY = A1;

// The object taking care of serial communication
SerialCommunication serialCommunication;

// The object taking care of reading joystick status
Joystick joystick;

// The number of moles in the game
const int numMoles = 9;

// The object managing the moles and the moles pins (we can use any digital
// output, the Servo library uses a timer interrupt to generate the PWM, not the
// hardware PWM)
// int molesPins[numMoles] = {   8,    5,    2,    9,    6,    3,    10,    7,   4};
int molesPins[numMoles] = {   4,    7,    10,   3,    6,    9,     2,    5,   8};
// int servoMin[numMoles] = { 544,  544,  544,  544,  544,  544,  544,  544,  544};
// int servoMax[numMoles] = {2400, 2400, 2400, 2400, 2400, 2400, 2400, 2400, 2400};
// int servoMin[numMoles] = { 600,  600,  600,  600,  600,  600,  600,  600,  600};
// int servoMax[numMoles] = {2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000};
//int servoMin[numMoles] = { 900,  900,  900,  900,  900,  900,  900,  900,  900};
//int servoMax[numMoles] = {1700, 1700, 1700, 1700, 1700, 1700, 1700, 1700, 1700};
int servoMin[numMoles] = {1000, 1000, 1050, 1000,  900,  900,  900,  900,  900};
int servoMax[numMoles] = {1900, 1700, 1900, 1900, 1600, 1650, 1650, 1600, 1750};

typedef Moles<numMoles> GameMoles;
GameMoles moles;

// The object to perform servo servo calibration
ServoCalibration<numMoles> servoCalibration;

// Whether to send joystick positions or not
bool sendJoystick = false;

// The smoothing filter
uint16_t* filterXHistory = NULL;
uint16_t* filterYHistory = NULL;

void setup() {
	// Initializing serial communication
	serialCommunication.begin(baudRate);

	// Initializing joystick
	joystick.begin(joystickP1, joystickP2, joystickX, joystickY);

	// Initializing moles
	moles.begin(molesPins, servoMin, servoMax);

	// Initializing filters
	filterXHistory = ms_init(SMA);
	filterYHistory = ms_init(SMA);
}

void loop() {
	// Reading from the serial line
	if (serialCommunication.commandReceived() && (serialCommunication.receivedCommandNumParts() != 0)) {
		if (serialCommunication.receivedCommandPart(0)[0] == 'S') {
			sendJoystick = true;
		} else if (serialCommunication.receivedCommandPart(0)[0] == 'A') {
			moles.attach();
		} else if (serialCommunication.receivedCommandPart(0)[0] == 'D') {
			moles.detach();
		} else if ((serialCommunication.receivedCommandPart(0)[0] == 'M') && (serialCommunication.receivedCommandNumParts() >= 2)) {
			// Move servo to bring up or down the moles
			moles.setStatus(serialCommunication.receivedCommandPartAsInt(1));
		} else if (serialCommunication.receivedCommandPart(0)[0] == 'C') {
			// Starting servo calibration. The funtion returns when the calibration procedure ends
			servoCalibration.enterLoop(moles, serialCommunication);
		}
	}

	if (sendJoystick) {
		// Reading joystick and sending activations
		joystick.readStatus();

		const int filteredX = sma_filter(joystick.xPosition(), filterXHistory);
		const int filteredY = sma_filter(joystick.yPosition(), filterYHistory);

		serialCommunication.newCommandToSend();
		serialCommunication.appendCommandPart("J");
		serialCommunication.appendCommandPart(filteredX);
		serialCommunication.appendCommandPart(filteredY);
		serialCommunication.appendCommandPart(joystick.button1Pressed());
		serialCommunication.appendCommandPart(joystick.button2Pressed());
		serialCommunication.sendCommand();

		// This is just to avoid flooding the PC: joystick movement depends on the time
		// passed since the last command and this measurement has millisecond resolution
		delay(5);
	}
}
