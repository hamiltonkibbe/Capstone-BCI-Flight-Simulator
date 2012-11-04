/** commands.h
 *
 * Holds the functions that set and get
 * information from flightgear.
 */
#ifndef Commands_class
#define Commands_class

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <math.h>
#include <signal.h>
#include <pthread.h>
#include <gdk/gdk.h>
#include "Socket.h"
#include "Sound.h"

#define MAX_BUF_SIZE 512
#define ADDR "127.0.0.1"
#define KNOTS_TO_FPM_CONV 101.268591

enum DETECTION {
	LEFT0 = 1,
	UP0,
	RIGHT0,
	DOWN0,
	LEFT1,
	UP1,
	RIGHT1,
	DOWN1,
	UP_RIGHT,
	UP_LEFT,
	DOWN_RIGHT,
	DOWN_LEFT,
	THROTTLE_UP,
	THROTTLE_DOWN,
	LEVEL,
	GEAR,
	PARK_BRAKE,
	FLAPS,
	FINE_CONTROL,
	RUDDER_LEFT,
	RUDDER_RIGHT
};

void* rightThread(void*);
void* leftThread(void*);
double mod(double, int);

class Commands {
	void* arg;
	static Socket *setSock;
	static Socket *getSock;
	static const int setPort = 12345;
	static const int getPort = 12347;
	static std::string recvBuf;

	void incVertSpeed0();
	void incVertSpeed1();
	void decVertSpeed0();
	void decVertSpeed1();
	void leftHeading0();
	void leftHeading1();
	void rightHeading0();
	void rightHeading1();
	void goUpRight();
	void goUpLeft();
	void goDownRight();
	void goDownLeft();

	void throttleUp();
	void throttleDown();
	void levelPlane();
	void switchGear();
	void switchBrake();
	void switchFlaps();
	void rudderLeft();
	void rudderRight();
	void switchFineControl();

public:
	static volatile bool stop;

	static bool fineControl;
	static double fake;
	static double destHeading; //heading in degrees at the start of a maneuver
	static double apHeading;

	static int K; //Gain
	static int right_n;  //number of times a right_heading command is sent, reset after each maneuver is completed.
	static int left_n;  //number of times a left_heading command is sent, reset after each maneuver is completed.
	static bool right;
	static bool left;

	pthread_t thread;

	Commands();
	Commands(Socket*, Socket*);
	~Commands();
	void detect(int);

	static void sendData(std::string);
	static std::string getData(std::string);
};

#endif
