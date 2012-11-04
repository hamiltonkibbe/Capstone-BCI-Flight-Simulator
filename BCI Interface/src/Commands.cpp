/** commands.cpp
 *
 * Holds the functions that set and get
 * information from flightgear.
 */

#include "Commands.h"

using namespace std;

int Commands::K = 1;
int Commands::left_n = 0;
int Commands::right_n = 0;
bool Commands::left = false;
bool Commands::right= false;
bool Commands::fineControl = true;
double Commands::fake = 0;
double Commands::destHeading = 0;
double Commands::apHeading = 0;
Socket* Commands::setSock;
Socket* Commands::getSock;
std::string Commands::recvBuf;

/* Constructor */
Commands::Commands()
{

    setSock = new Socket;
    getSock = new Socket;

	if (setSock->create() == false)
	{
		cout << "Error creating setSocket" << endl;
		exit(1);
	}
	if (getSock->create() == false)
	{
		cout << "Error creating getSocket" << endl;
		exit(1);
	}
	arg = NULL;

	if (setSock->connect(ADDR, setPort) == false)
	{
		cout << "connect failed" << endl;
	}
	if (getSock->bind(ADDR, getPort) == false)
	{
		cout << "bind failed" << endl;
	}
	thread = NULL;
}

Commands::Commands(Socket* _setSock, Socket* _getSock) {
	setSock = _setSock;
	getSock = _getSock;
	thread = NULL;

	arg = NULL;
}

Commands::~Commands()
{
    delete setSock;
    delete getSock;
}

void Commands::detect(int x) {
	cout << "detect(" << x << ")" << endl;

	switch (x) {
	case UP0:
		play_sound("../res/audio/UP0.ogg");
		incVertSpeed0();
		break;
	case UP1:
		play_sound("../res/audio/UP1.ogg");
		incVertSpeed1();
		break;
	case DOWN0:
		play_sound("../res/audio/DOWN0.ogg");
		decVertSpeed0();
		break;
	case DOWN1:
		play_sound("../res/audio/DOWN1.ogg");
		incVertSpeed1();
		break;
	case RIGHT0:
		play_sound("../res/audio/RIGHT0.ogg");
		rightHeading0();
		break;
	case RIGHT1:
		play_sound("../res/audio/RIGHT1.ogg");
		rightHeading1();
		break;
	case LEFT0:
		play_sound("../res/audio/LEFT0.ogg");
		leftHeading0();
		break;
	case LEFT1:
		play_sound("../res/audio/LEFT1.ogg");
		leftHeading1();
		break;
	case UP_RIGHT:
		play_sound("../res/audio/UP_RIGHT.ogg");
		goUpRight();
		break;
	case UP_LEFT:
		play_sound("../res/audio/UP_LEFT.ogg");
		goUpLeft();
		break;
	case DOWN_RIGHT:
		play_sound("../res/audio/DOWN_RIGHT.ogg");
		goDownRight();
		break;
	case DOWN_LEFT:
		play_sound("../res/audio/DOWN_LEFT.ogg");
		goDownLeft();
		break;
	case THROTTLE_UP:
		play_sound("../res/audio/THROTTLE_UP.ogg");
		throttleUp();
		break;
	case THROTTLE_DOWN:
		play_sound("../res/audio/THROTTLE_DOWN.ogg");
		throttleDown();
		break;
	case LEVEL:
		play_sound("../res/audio/LEVEL.ogg");
		levelPlane();
		break;
	case GEAR:
		play_sound("../res/audio/GEAR.ogg");
		switchGear();
		break;
	case PARK_BRAKE:
		switchBrake();
		break;
	case FLAPS:

		switchFlaps();
		break;
	case FINE_CONTROL:
		if(fineControl)
		{
			play_sound("../res/auido/COARSE_CONTROL.ogg");
		}
		else
		{
			play_sound("../res/audio/FINE_CONTROL.ogg");
		}
		switchFineControl();
		break;
	case RUDDER_LEFT:
		play_sound("../res/audio/RUDDER_LEFT.ogg");
		rudderLeft();
		break;
	case RUDDER_RIGHT:
		play_sound("../res/auido/RUDDER_RIGHT.ogg");
		rudderRight();
		break;
	default:
		cout << "detection " << x << " not found." << endl;
		break;
	}
}

void Commands::decVertSpeed0() {
	std::ostringstream s;
	if (fineControl) {
		double elevatorPosition = atof(getData("get double /controls/flight/elevator\n").c_str());

		sendData("set string /autopilot/locks/altitude off\n");
		sendData("set double /autopilot/settings/vertical-speed-fpm 0\n");
		if (elevatorPosition <= .95) {
			elevatorPosition += .05;
			s << "set double /controls/flight/elevator " << elevatorPosition << "\n";
			sendData(s.str());
		}
	} else {
		string vertSpeed = getData("get double /autopilot/settings/vertical-speed-fpm\n");
		string airSpeedKnots = getData("get double /instrumentation/airspeed-indicator/true-speed-kt\n");

		double airSpeedFpm = atof(airSpeedKnots.c_str()) * KNOTS_TO_FPM_CONV;
		double newVertSpeed = atof(vertSpeed.c_str()) - .052*airSpeedFpm;
		cout << "new vertspeed = " << newVertSpeed;

		s << "set double /autopilot/settings/vertical-speed-fpm " << newVertSpeed << "\n";
		sendData(s.str());
		sendData("set string /autopilot/locks/altitude vertical-speed-hold\n");
	}
}

void Commands::incVertSpeed0() {
	std::ostringstream s;

	if (fineControl) {
		double elevatorPosition = atof(getData("get double /controls/flight/elevator\n").c_str());

		sendData("set string /autopilot/locks/altitude off\n");
		sendData("set double /autopilot/settings/vertical-speed-fpm 0\n");
		if (elevatorPosition >= -.95) {
			elevatorPosition -= .05;
			s << "set double /controls/flight/elevator " << elevatorPosition << "\n";
			sendData(s.str());
		}
	} else {
		string vertSpeed = getData("get double /autopilot/settings/vertical-speed-fpm\n");
		string airSpeedKnots = getData("get double /instrumentation/airspeed-indicator/true-speed-kt\n");

		double airSpeedFpm = atof(airSpeedKnots.c_str()) * KNOTS_TO_FPM_CONV;
		double newVertSpeed = atof(vertSpeed.c_str()) + .052*airSpeedFpm;
		cout << "new vertspeed = " << newVertSpeed;

		s << "set double /autopilot/settings/vertical-speed-fpm " << newVertSpeed << "\n";
		sendData(s.str());
		sendData("set string /autopilot/locks/altitude vertical-speed-hold\n");
	}
}

void Commands::decVertSpeed1() {
	std::ostringstream s;

	if (fineControl) {
		double elevatorPosition = atof(getData("get double /controls/flight/elevator\n").c_str());

		sendData("set double /autopilot/settings/vertical-speed-fpm 0\n");
		sendData("set string /autopilot/locks/altitude off\n");
		if (elevatorPosition <= .9) {
			elevatorPosition += .1;
			s << "set double /controls/flight/elevator " << elevatorPosition << "\n";
			sendData(s.str());
		}
	} else {
		string vertSpeed = getData("get double /autopilot/settings/vertical-speed-fpm\n");
		string airSpeedKnots = getData("get double /instrumentation/airspeed-indicator/true-speed-kt\n");

		double airSpeedFpm = atof(airSpeedKnots.c_str()) * KNOTS_TO_FPM_CONV;
		double newVertSpeed = atof(vertSpeed.c_str()) - 0.176*airSpeedFpm;
		cout << "new vertspeed = " << newVertSpeed;

		s << "set double /autopilot/settings/vertical-speed-fpm " << newVertSpeed << "\n";
		sendData(s.str());
		sendData("set string /autopilot/locks/altitude vertical-speed-hold\n");
	}
}

void Commands::incVertSpeed1() {
	std::ostringstream s;

	if (fineControl) {
		double elevatorPosition = atof(getData("get double /controls/flight/elevator\n").c_str());

		sendData("set double /autopilot/settings/vertical-speed-fpm 0\n");
		sendData("set string /autopilot/locks/altitude off\n");
		if (elevatorPosition >= -.9) {
			elevatorPosition -= .1;
			s << "set double /controls/flight/elevator " << elevatorPosition << "\n";
			sendData(s.str());
		}
	} else {
		string vertSpeed = getData("get double /autopilot/settings/vertical-speed-fpm\n");
		string airSpeedKnots = getData("get double /instrumentation/airspeed-indicator/true-speed-kt\n");

		double airSpeedFpm = atof(airSpeedKnots.c_str()) * KNOTS_TO_FPM_CONV;
		double newVertSpeed = atof(vertSpeed.c_str()) + 0.176*airSpeedFpm;
		cout << "new vertspeed = " << newVertSpeed;

		s << "set double /autopilot/settings/vertical-speed-fpm " << newVertSpeed << "\n";
		sendData(s.str());
		sendData("set string /autopilot/locks/altitude vertical-speed-hold\n");
	}
}

void Commands::leftHeading0() {
	std::ostringstream s;
    if(thread) pthread_cancel(thread);

	double curHead = atof(getData("get double /orientation/heading-magnetic-deg\n").c_str());

	if(!right && !left) {
		s << "set double /autopilot/settings/heading-bug-deg " << curHead << "\n";
		sendData(s.str());
	}

	if (fineControl) {
		string aileron = getData("get double /controls/flight/aileron\n");
		double aileronPosition = atof(aileron.c_str());
		sendData("set string /autopilot/locks/heading off\n");
		if (aileronPosition >= -.95) {
			aileronPosition -= .05;
			s.str("");
			s << "set double /controls/flight/aileron " << aileronPosition << "\n";
			sendData(s.str());
		}
	} else {
		double error = fabs(atof(getData("get double /autopilot/settings/heading-bug-deg\n").c_str()) - curHead);
		if (error < 2)
			error = 0;
		else if (error > 180)
			error = 360 - error;  //to account for 0/360 border

		left = true;
		left_n++;

		if (right) {
			destHeading = destHeading + error - 3*left_n;
			if (left_n >= right_n) { right = false; right_n = 0;}
		} else {
			destHeading = (curHead - error - 3*left_n);
		}

		cout << endl;
		cout << "start heading = " << curHead << endl;
		cout << "error = " << error << endl;
		cout << "destheading = " << destHeading << endl;

		//pthread_create
		int rc = pthread_create(&thread, NULL, leftThread, arg);
		if (rc != 0) {
			cout << "pthread create failed. rc = " << rc << endl;
		}
	}
}

void Commands::leftHeading1() {
	std::ostringstream s;

	if(thread) pthread_cancel(thread);

	double curHead = atof(getData("get double /orientation/heading-magnetic-deg\n").c_str());

	if(!right && !left) {
		s << "set double /autopilot/settings/heading-bug-deg " << curHead << "\n";
		sendData(s.str());
	}

	if (fineControl) {
		string aileron = getData("get double /controls/flight/aileron\n");
		double aileronPosition = atof(aileron.c_str());
		sendData("set string /autopilot/locks/heading off\n");
		if (aileronPosition >= -.9) {
			aileronPosition -= .1;
			s.str("");
			s << "set double /controls/flight/aileron " << aileronPosition << "\n";
			sendData(s.str());
		}
	} else {
		double error = fabs(atof(getData("get double /autopilot/settings/heading-bug-deg\n").c_str()) - curHead);
		if (error < 2)
			error = 0;
		else if (error > 180)
			error = 360 - error;  //to account for 0/360 border

		left = true;
		left_n++;

		if (right) {
			destHeading = destHeading + error - 6*left_n;
			if (left_n >= right_n) { right = false; right_n = 0;}
		} else {
			destHeading = (curHead - error - 6*left_n);
		}

		cout << endl;
		cout << "start heading = " << curHead << endl;
		cout << "error = " << error << endl;
		cout << "destheading = " << destHeading << endl;

		//pthread_create
		int rc = pthread_create(&thread, NULL, leftThread, arg);
		if (rc != 0) {
			cout << "pthread create failed. rc = " << rc << endl;
		}
	}
}

void Commands::rightHeading0() {
	std::ostringstream s;

	if(thread) pthread_cancel(thread);

	double curHead = atof(getData("get double /orientation/heading-magnetic-deg\n").c_str());

	if(!right && !left) {
		s << "set double /autopilot/settings/heading-bug-deg " << curHead << "\n";
		sendData(s.str());
	}

	if (fineControl) {
		string aileron = getData("get double /controls/flight/aileron\n");
		double aileronPosition = atof(aileron.c_str());
		sendData("set string /autopilot/locks/heading off\n");
		if (aileronPosition <= .95) {
			aileronPosition += .05;
			s.str("");
			s << "set double /controls/flight/aileron " << aileronPosition << "\n";
			sendData(s.str());
		}
	} else {
		double error = fabs(atof(getData("get double /autopilot/settings/heading-bug-deg\n").c_str()) - curHead);
		if (error < 2)
			error = 0;
		else if (error > 180)
			error = 360 - error;  //to account for 0/360 border

		right = true;
		right_n++;

		if (left) {
			destHeading = destHeading - error + 3*right_n;
			if (right_n >= left_n) { left = false; left_n = 0;}
		} else {
			destHeading = (curHead + error + 3*right_n);// % 360;
		}

		cout << endl;
		cout << "start heading = " << curHead << endl;
		cout << "error = " << error << endl;
		cout << "destheading = " << destHeading << endl;

		//pthread_create
		int rc = pthread_create(&thread, NULL, rightThread, arg);
		if (rc != 0) {
			cout << "pthread create failed. rc = " << rc << endl;
		}
	}
}

void Commands::rightHeading1() {
	std::ostringstream s;

	if(thread) pthread_cancel(thread);

	double curHead = atof(getData("get double /orientation/heading-magnetic-deg\n").c_str());

	if(!right && !left) {
		s << "set double /autopilot/settings/heading-bug-deg " << curHead << "\n";
		sendData(s.str());
	}

	if (fineControl) {
		string aileron = getData("get double /controls/flight/aileron\n");
		double aileronPosition = atof(aileron.c_str());
		sendData("set string /autopilot/locks/heading off\n");
		if (aileronPosition <= .9) {
			aileronPosition += .1;
			s.str("");
			s << "set double /controls/flight/aileron " << aileronPosition << "\n";
			sendData(s.str());
		}
	} else {
		double error = fabs(atof(getData("get double /autopilot/settings/heading-bug-deg\n").c_str()) - curHead);
		if (error < 2)
			error = 0;
		else if (error > 180)
			error = 360 - error;  //to account for 0/360 border

		right = true;
		right_n++;

		if (left) {
			destHeading = destHeading - error + 6*right_n;
			if (right_n >= left_n) { left = false; left_n = 0;}
		} else {
			destHeading = (curHead + error + 6*right_n);// % 360;
		}

		cout << endl;
		cout << "start heading = " << curHead << endl;
		cout << "error = " << error << endl;
		cout << "destheading = " << destHeading << endl;

		//pthread_create
		int rc = pthread_create(&thread, NULL, leftThread, arg);
		if (rc != 0) {
			cout << "pthread create failed. rc = " << rc << endl;
		}
	}
}

void Commands::goUpRight() {
	if (fineControl) return;

	ostringstream s;

	if(thread) pthread_cancel(thread);

	double curHead = atof(getData("get double /orientation/heading-magnetic-deg\n").c_str());

	//go up
	string vertSpeed = getData("get double /autopilot/settings/vertical-speed-fpm\n");
	string airSpeedKnots = getData("get double /instrumentation/airspeed-indicator/true-speed-kt\n");

	double airSpeedFpm = atof(airSpeedKnots.c_str()) * KNOTS_TO_FPM_CONV;
	double newVertSpeed = atof(vertSpeed.c_str()) + .052*airSpeedFpm;
	cout << "new vertspeed = " << newVertSpeed;

	s << "set double /autopilot/settings/vertical-speed-fpm " << newVertSpeed << "\n";
	sendData(s.str());
	sendData("set string /autopilot/locks/altitude vertical-speed-hold\n");

	//go right
	double error = fabs(atof(getData("get double /autopilot/settings/heading-bug-deg\n").c_str()) - curHead);
	if (error < 2)
		error = 0;
	else if (error > 180)
		error = 360 - error;  //to account for 0/360 border

	right = true;
	right_n++;

	if (left) {
		destHeading = destHeading - error + 3*right_n;
		if (right_n >= left_n) { left = false; left_n = 0;}

	} else {
		destHeading = (curHead + error + 3*right_n);// % 360;
	}

	cout << endl;
	cout << "start heading = " << curHead << endl;
	cout << "error = " << error << endl;
	cout << "destheading = " << destHeading << endl;

	//pthread_create
	int rc = pthread_create(&thread, NULL, rightThread, arg);
	if (rc != 0) {
		cout << "pthread create failed. rc = " << rc << endl;
	}
}

void Commands::goDownRight() {
	if (fineControl) return;

	ostringstream s;

	if(thread) pthread_cancel(thread);

	double curHead = atof(getData("get double /orientation/heading-magnetic-deg\n").c_str());

	//go down
	string vertSpeed = getData("get double /autopilot/settings/vertical-speed-fpm\n");
	string airSpeedKnots = getData("get double /instrumentation/airspeed-indicator/true-speed-kt\n");

	double airSpeedFpm = atof(airSpeedKnots.c_str()) * KNOTS_TO_FPM_CONV;
	double newVertSpeed = atof(vertSpeed.c_str()) - .052*airSpeedFpm;
	cout << "new vertspeed = " << newVertSpeed;

	s << "set double /autopilot/settings/vertical-speed-fpm " << newVertSpeed << "\n";
	sendData(s.str());
	sendData("set string /autopilot/locks/altitude vertical-speed-hold\n");

	//go right
	double error = fabs(atof(getData("get double /autopilot/settings/heading-bug-deg\n").c_str()) - curHead);
	if (error < 2)
		error = 0;
	else if (error > 180)
		error = 360 - error;  //to account for 0/360 border

	right = true;
	right_n++;

	if (left) {
		destHeading = destHeading - error + 3*right_n;
		if (right_n >= left_n) { left = false; left_n = 0;}

	} else {
		destHeading = (curHead + error + 3*right_n);// % 360;
	}

	cout << endl;
	cout << "start heading = " << curHead << endl;
	cout << "error = " << error << endl;
	cout << "destheading = " << destHeading << endl;

	//pthread_create
	int rc = pthread_create(&thread, NULL, rightThread, arg);
	if (rc != 0) {
		cout << "pthread create failed. rc = " << rc << endl;
	}
}

void Commands::goUpLeft() {
	if (fineControl) return;

	ostringstream s;

	if(thread) pthread_cancel(thread);

	double curHead = atof(getData("get double /orientation/heading-magnetic-deg\n").c_str());

	//go up
	string vertSpeed = getData("get double /autopilot/settings/vertical-speed-fpm\n");
	string airSpeedKnots = getData("get double /instrumentation/airspeed-indicator/true-speed-kt\n");

	double airSpeedFpm = atof(airSpeedKnots.c_str()) * KNOTS_TO_FPM_CONV;
	double newVertSpeed = atof(vertSpeed.c_str()) + .052*airSpeedFpm;
	cout << "new vertspeed = " << newVertSpeed;

	s << "set double /autopilot/settings/vertical-speed-fpm " << newVertSpeed << "\n";
	sendData(s.str());
	sendData("set string /autopilot/locks/altitude vertical-speed-hold\n");

	//go left
	double error = fabs(atof(getData("get double /autopilot/settings/heading-bug-deg\n").c_str()) - curHead);
	if (error < 2)
		error = 0;
	else if (error > 180)
		error = 360 - error;  //to account for 0/360 border

	left = true;
	left_n++;

	if (right) {
		destHeading = destHeading + error - 3*left_n;
		if (left_n >= right_n) { right = false; right_n = 0;}
	} else {
		destHeading = (curHead - error - 3*left_n);
	}

	cout << endl;
	cout << "start heading = " << curHead << endl;
	cout << "error = " << error << endl;
	cout << "destheading = " << destHeading << endl;

	//pthread_create
	int rc = pthread_create(&thread, NULL, leftThread, arg);
	if (rc != 0) {
		cout << "pthread create failed. rc = " << rc << endl;
	}
}

void Commands::goDownLeft() {
	if (fineControl) return;

	ostringstream s;

	if(thread) pthread_cancel(thread);

	double curHead = atof(getData("get double /orientation/heading-magnetic-deg\n").c_str());

	//go down
	string vertSpeed = getData("get double /autopilot/settings/vertical-speed-fpm\n");
	string airSpeedKnots = getData("get double /instrumentation/airspeed-indicator/true-speed-kt\n");

	double airSpeedFpm = atof(airSpeedKnots.c_str()) * KNOTS_TO_FPM_CONV;
	double newVertSpeed = atof(vertSpeed.c_str()) - .052*airSpeedFpm;
	cout << "new vertspeed = " << newVertSpeed;

	s << "set double /autopilot/settings/vertical-speed-fpm " << newVertSpeed << "\n";
	sendData(s.str());
	sendData("set string /autopilot/locks/altitude vertical-speed-hold\n");

	//go left
	double error = fabs(atof(getData("get double /autopilot/settings/heading-bug-deg\n").c_str()) - curHead);
	if (error < 2)
		error = 0;
	else if (error > 180)
		error = 360 - error;  //to account for 0/360 border

	left = true;
	left_n++;

	if (right) {
		destHeading = destHeading + error - 3*left_n;
		if (left_n >= right_n) { right = false; right_n = 0;}
	} else {
		destHeading = (curHead - error - 3*left_n);
	}

	cout << endl;
	cout << "start heading = " << curHead << endl;
	cout << "error = " << error << endl;
	cout << "destheading = " << destHeading << endl;

	//pthread_create
	int rc = pthread_create(&thread, NULL, leftThread, arg);
	if (rc != 0) {
		cout << "pthread create failed. rc = " << rc << endl;
	}
}

void Commands::throttleUp() {
	string data = getData("get double /controls/engines/engine/throttle\n");
	double throttle = atof(data.c_str());
	if (throttle >= 1) {
		return;
	}
	throttle += .1;

	ostringstream s, s1, s2;
	s << "set double /controls/engines/engine/throttle " << throttle << "\n";
	s1 << "set double /controls/engines/engine[1]/throttle " << throttle << "\n";
	s2 << "set double /controls/engines/engine[2]/throttle " << throttle << "\n";

	sendData(s.str());
	sendData(s1.str());
	sendData(s2.str());
}

void Commands::throttleDown() {
	string data = getData("get double /controls/engines/engine/throttle\n");
	double throttle = atof(data.c_str());
	if (throttle <= 0) {
		return;
	}
	throttle -= .1;

	ostringstream s, s1, s2;
	s << "set double /controls/engines/engine/throttle " << throttle << "\n";
	s1 << "set double /controls/engines/engine[1]/throttle " << throttle << "\n";
	s2 << "set double /controls/engines/engine[2]/throttle " << throttle << "\n";

	sendData(s.str());
	sendData(s1.str());
	sendData(s2.str());
}

void Commands::levelPlane() {
	if(thread) pthread_cancel(thread);

	string currentHeading = getData("get double /orientation/heading-magnetic-deg\n");
	sendData("set double /autopilot/settings/heading-bug-deg " + currentHeading);  //new line already included from getData
	sendData("set string /autopilot/locks/heading dg-heading-hold\n");

	sendData("set double /autopilot/settings/vertical-speed-fpm 0\n");
	sendData("set string /autopilot/locks/altitude vertical-speed-hold\n");

	left = false;
	right = false;
	right_n = 0;
	left_n = 0;
}

/**TODO: sendData("set bool /controls/gear/gear-down 0")
 * 		in flightgear do:
 * 				if (type == bool) fgSetBool(property, atoi(value));
 */
void Commands::switchGear() {
	string data = getData("get bool /controls/gear/gear-down\n");
	bool gear = atoi(data.c_str());
	if (gear == 1) {
		sendData("set bool /controls/gear/gear-down false\n");
	} else if (gear == 0) {
		sendData("set bool /controls/gear/gear-down true\n");
	} else {
		cout << "Problem with landing gear..." << endl;
	}
}

void Commands::switchBrake() {
	double speed = atof(getData("get double /velocities/groundspeed-kt\n").c_str());

	if (speed < 5) {
		//play_sound("PARK_BRAKE.wav");
		double gear = atof(getData("get bool /controls/gear/brake-parking\n").c_str());
		if (gear == 1) {
			sendData("set bool /controls/gear/brake-parking false\n");
		} else if (gear == 0) {
			sendData("set bool /controls/gear/brake-parking true\n");
		} else {
			cout << "Problem with parking break..." << endl;
		}
	} else {
		//play_sound("FULL_BRAKES.wav");
		double speedBrake = atof(getData("get double /controls/flight/speedbrake\n").c_str());
		if (speedBrake == 0) {
			sendData("set double /controls/flight/speedbrake 1\n");
			sendData("set double /controls/flight/spoilers 1\n");
			sendData("set double /controls/gear/brake-left 1\n");
			sendData("set double /controls/gear/brake-right 1\n");
		} else {
			sendData("set double /controls/flight/speedbrake 0\n");
			sendData("set double /controls/flight/spoilers 0\n");
			sendData("set double /controls/gear/brake-left 0\n");
			sendData("set double /controls/gear/brake-right 0\n");
		}
	}
}

void Commands::switchFlaps() {
	double flapsCommand = atof(getData("get double /controls/flight/flapscommand\n").c_str());
	if (flapsCommand == 1) {
		//play_sound("FLAPS_OFF.wav");
		sendData("set double /controls/flight/flapscommand 0\n");
		sendData("set double /controls/flight/mainflaps 0\n");
		sendData("set double /controls/flight/slats 0\n");
		sendData("set double /controls/flight/auxflaps 0\n");
		sendData("set double /controls/flight/yasim-spoilers .14\n");
	} else {
		//play_sound("FLAPS_ON.wav");
		sendData("set double /controls/flight/flapscommand 1\n");
		sendData("set double /controls/flight/mainflaps 1\n");
		sendData("set double /controls/flight/slats 1\n");
		sendData("set double /controls/flight/auxflaps 1\n");
		sendData("set double /controls/flight/yasim-spoilers .067\n");
	}
}

void Commands::rudderLeft() {
	if (!fineControl) return;

	ostringstream s;

	//rudder stuff
	double rudderPosition = atof(getData("get double /controls/flight/rudder\n").c_str());
	if (rudderPosition >= -.95) {
		rudderPosition -= .05;
		s << "set double /controls/flight/rudder " << rudderPosition << "\n";
		sendData(s.str());
	}

	//steering stuff
	double steeringPosition = atof(getData("get double /controls/gear/steering\n").c_str());
	if (steeringPosition >= -68) {
		steeringPosition -= 2;
		s.str("");
		s << "set double /controls/gear/steering " << steeringPosition << "\n";
		sendData(s.str());
	}
}

void Commands::rudderRight() {
	if (!fineControl) return;

	ostringstream s;

	//rudder stuff
	double rudderPosition = atof(getData("get double /controls/flight/rudder\n").c_str());
	if (rudderPosition <= .95) {
		rudderPosition += .05;
		s << "set double /controls/flight/rudder " << rudderPosition << "\n";
		sendData(s.str());
	}

	//steering stuff
	double steeringPosition = atof(getData("get double /controls/gear/steering\n").c_str());
	if (steeringPosition <= 68) {
		steeringPosition += 2;
		s.str("");
		s << "set double /controls/gear/steering " << steeringPosition << "\n";
		sendData(s.str());
	}

}

void Commands::switchFineControl() {
	fineControl = !fineControl;

	if(thread) pthread_cancel(thread);

	if (fineControl) {
		sendData("set string /autopilot/locks/heading off\n");
		sendData("set string /autopilot/locks/altitude off\n");
		sendData("set double /logging/log/entry[1]/enabled 0\n");
	} else {
		sendData("set double /logging/log/entry[1]/enabled 1\n");

		right = false;
		right_n = 0;
		left = false;
		left_n = 0;
	}

	sendData("set double /controls/flight/elevator 0\n");
	sendData("set double /controls/flight/aileron 0\n");
	sendData("set double /controls/flight/rudder 0\n");
}

void Commands::sendData(string s) {

	setSock->send(s);

}

string Commands::getData(string s) {

	recvBuf.clear();


	setSock->send(s);
	getSock->recv(recvBuf);

	return recvBuf;
}

void* rightThread(void* _t) {
	bool shouldWait = false;
	ostringstream s;

	while (true) {
		double curHead = atof(Commands::getData("get double /orientation/heading-magnetic-deg\n").c_str());
		Commands::fake = (Commands::K * fabs(curHead - Commands::destHeading));

		/* to handle 0/360 border */
		if (Commands::fake > 360) Commands::fake -= 360;
		double fakeHead = Commands::destHeading + Commands::fake;
		bool bothMods = false;

		if (fakeHead > 360 && ((curHead + 180) > 360))
			bothMods = true;

		if ((mod(Commands::destHeading + Commands::fake, 360) > (curHead + 180)) ||
				(bothMods && (mod(Commands::destHeading + Commands::fake, 360) > mod(curHead + 180, 360)))) {
			Commands::apHeading = mod(curHead + 180, 360);

			s << "set double /autopilot/settings/heading-bug-deg " << Commands::apHeading << "\n";
			Commands::sendData(s.str());

			cout << "curHead = " << curHead << endl;
			cout << "fake = " << Commands::fake << endl;
			cout << "apHeading_right_" << Commands::right_n << " = " << Commands::apHeading << endl;

			shouldWait = true;
			break;
		} else {
			Commands::apHeading = mod(Commands::destHeading + Commands::fake, 360);
		}

		cout << "curHead = " << curHead << endl;
		cout << "fake = " << Commands::fake << endl;
		cout << "apHeading_right_" << Commands::right_n << " = " << Commands::apHeading << endl;

		s.str("");
		s << "set double /autopilot/settings/heading-bug-deg " << Commands::apHeading << "\n";
		Commands::sendData(s.str());

		Commands::sendData("set string /autopilot/locks/heading dg-heading-hold\n");

		if (fabs(atof(Commands::getData("get double /orientation/heading-magnetic-deg\n").c_str()) -
				mod(Commands::destHeading, 360)) < .75) {
			//finished maneuver
			cout << "maneuver finished" << endl;
			Commands::left_n = 0;
			Commands::right_n = 0;
			Commands::left = false;
			Commands::right = false;
			break;
		}

		cout << endl;
		usleep(1000*200);
	}

	if (shouldWait) {
		while(true) {
			if (fabs(atof(Commands::getData("get double /orientation/heading-magnetic-deg\n").c_str()) -
					mod(Commands::destHeading, 360)) < .75) {
				//finished maneuver
				cout << "maneuver finished" << endl;
				Commands::left_n = 0;
				Commands::right_n = 0;
				Commands::left = false;
				Commands::right = false;
				break;
			}
			sleep(1);
		}
	}

	cout << "finished loop" << endl;

	pthread_exit(NULL);

	return 0;
}

void* leftThread(void* _t) {
	bool shouldWait = false;
	ostringstream s;

	while (true) {
		double curHead = atof(Commands::getData("get double /orientation/heading-magnetic-deg\n").c_str());
		Commands::fake = Commands::K * fabs(curHead - Commands::destHeading);

		/* to handle 0/360 border */
		if (Commands::fake > 360) Commands::fake -= 360;
		double fakeHead = Commands::destHeading - Commands::fake;
		bool bothMods = false;

		if (fakeHead < 0 && ((curHead - 180) < 0))
			bothMods = true;

		if ((mod(Commands::destHeading - Commands::fake, 360) < (curHead - 178)) ||
				(bothMods && (mod(Commands::destHeading - Commands::fake, 360) < mod(curHead - 178, 360))) ){
			Commands::apHeading = mod(curHead - 178, 360);

			s << "set double /autopilot/settings/heading-bug-deg " << Commands::apHeading << "\n";
			Commands::sendData(s.str());

			cout << "curHead = " << curHead << endl;
			cout << "fake = " << Commands::fake << endl;
			cout << "apHeading_left_" << Commands::left_n << " = " << Commands::apHeading << endl;

			shouldWait = true;
			break;
		} else {
			Commands::apHeading = mod(Commands::destHeading - Commands::fake, 360);
		}

		cout << "curHead = " << curHead << endl;
		cout << "fake = " << Commands::fake << endl;
		cout << "apHeading_left_" << Commands::left_n << " = " << Commands::apHeading << endl;

		s.str("");
		s << "set double /autopilot/settings/heading-bug-deg " << Commands::apHeading << "\n";
		Commands::sendData(s.str());

		Commands::sendData("set string /autopilot/locks/heading dg-heading-hold\n");

		if (fabs(atof(Commands::getData("get double /orientation/heading-magnetic-deg\n").c_str()) -
				mod(Commands::destHeading, 360)) < .75) {
			//finished maneuver
			cout << "maneuver finished" << endl;
			Commands::left_n = 0;
			Commands::right_n = 0;
			Commands::left = false;
			Commands::right = false;
			break;
		}

		cout << endl;
		usleep(1000*200);
	}

	if (shouldWait){
		cout << "waiting.." << endl;
		while(true) {
			if (fabs(atof(Commands::getData("get double /orientation/heading-magnetic-deg\n").c_str()) -
					mod(Commands::destHeading, 360)) < .75) {
				//finished maneuver
				cout << "maneuver finished" << endl;
				Commands::left_n = 0;
				Commands::right_n = 0;
				Commands::left = false;
				Commands::right = false;
				break;
			}
			sleep(1);
		}
	}

	cout << "finished loop" << endl;

	pthread_exit(NULL);

	return 0;
}

double mod (double a, int b) {
	if (a < 0)
		return a + b;
	else
		return fmod(a,b);
}


