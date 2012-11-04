/* threads.cxx 
 * contains our thread function pointers
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#if defined(__linux__) && defined(__i386__)
#  include <fpu_control.h>
#  include <signal.h>
#endif

#include <simgear/io/sg_socket_udp.hxx>
#include <simgear/misc/sg_sleep.hxx>

#include <iostream>
#include <string.h>
#include "globals.hxx"
#include "fg_props.hxx"
#include "threads.hxx"

using namespace simgear;

void start_capstone_threads() {
  
    /* Open 2 new threads for send/receive sockets */
    pthread_t thread1;//, thread2;
    pthread_t thread2;
    int rc;
    int *t1 = NULL;//, 
    int *t2 = NULL;
    rc = pthread_create(&thread1, NULL, receive_thread, (void*)t1);
    if (rc != 0){
	SG_LOG( SG_GENERAL, SG_ALERT, "pthread_create(receive_thread) error: rc = " << rc );
    }
    rc = pthread_create(&thread2, NULL, send_thread,(void*)t2);
    if (rc != 0){
	SG_LOG( SG_GENERAL, SG_ALERT, "pthread_create(send_thread) error: rc = " << rc );
	}
    return;
}


/**
   This function can receive 'set' or 'get' commands from a client on localhost:12345. 
   If it is a 'set' command, the type, property, value are parsed and the appropriate
   fgSet___() method is called. If it is a 'get' command, the type and property are 
   parsed, the appropriate fgGet___() method is called, and the value receiver from the 
   fgGet__() method is then sent out on localhost:12347 as a UDP string.
   commands accepted: 
   "set <type> <property> <value>"
   "get <type> <property>"
 */
void *receive_thread(void *t1) {
	string ip = "127.0.0.1";
    string port_rcv = "12345";
    string port_snd = "12347";

    /* Open recv FGSocket */
    SG_LOG( SG_GENERAL, SG_ALERT, "Opening Receiver Socket" );
    SGSocketUDP *socket_rcv = new SGSocketUDP (ip, port_rcv);
    SGProtocolDir dir = SG_IO_IN;
    if (!socket_rcv->open(dir)){
	SG_LOG( SG_GENERAL, SG_ALERT, "can't open socket " << ip << ":" << port_rcv );
	return NULL;
    } else {
	SG_LOG( SG_GENERAL, SG_ALERT, "Socket open " << ip << ":" << port_rcv);
    }

    /* Open send FGSocket */
    SG_LOG( SG_GENERAL, SG_ALERT, "Opening Sender Socket" );
    SGSocketUDP *socket_snd = new SGSocketUDP (ip, port_snd);
    dir = SG_IO_OUT;
    if (!socket_snd->open(dir)){
	SG_LOG( SG_GENERAL, SG_ALERT, "can't open socket " << ip << ":" << port_snd );
	return NULL;
    } else {
	SG_LOG( SG_GENERAL, SG_ALERT, "Socket open " << ip << ":" << port_snd);
    }
    
    char buf[256];
    char func[20] = {0}; //either 'get' or 'set'
    char type[20] = {0};
    char property[256] = {0};
    char value[256];
    int ret;
    
    char *temp, *next_token;
    memset(buf, 0, sizeof(buf));

    while ( (ret = socket_rcv->readline( buf, sizeof(buf) )) >= 0 ) {
	memset(func, 0, sizeof(func));
	memset(type, 0, sizeof(type));
	memset(property, 0, sizeof(property));
	memset(value, 0, sizeof(value));
      
	temp = strchr(buf, ' ');
      
	if (temp == NULL){
	    SG_LOG( SG_GENERAL, SG_ALERT, "make sure you are sending a \'\\n\' character " << buf);
	    continue;
	}
      
	memcpy(func, buf, temp - buf);
	SG_LOG( SG_GENERAL, SG_ALERT, "func = " <<  func 
		<< "; length = " << strlen(func));
	if ((strncmp(func, "get", 3) != 0) && (strncmp(func,"set", 3) != 0)) {
	    SG_LOG( SG_GENERAL, SG_ALERT, "first token must be equal to either \"get\" or \"set\"... Ignoring.");
	    continue;
	}

	temp += 1;
	next_token = strchr(temp, ' ');
	if (next_token == NULL) { 
	    SG_LOG( SG_GENERAL, SG_ALERT, "error finding second space");
	    continue;
	}

	if ((next_token - temp) > sizeof(type)) {
	    SG_LOG( SG_GENERAL, SG_ALERT, "type (second argument) is too long\n");
	    continue;
	}
	
	memcpy(type, temp, next_token - temp);
	SG_LOG( SG_GENERAL, SG_ALERT, "type = " <<  type 
		<< "; length = " << strlen(type));

	next_token += 1;
	
	/* If the function of this call is a get, then get the 
	   value specifed and continue.
	*/
	if (strncmp(func, "get", 3) == 0) {
	    temp = strchr(next_token, '\0');
	    if (temp == NULL) { 
		SG_LOG( SG_GENERAL, SG_ALERT, "error finding end");
		continue;
	    }
	    memcpy(property, next_token, temp - next_token);
	    property[strlen(property) - 1] = '\0';  //to take care of the '\n'
	    SG_LOG( SG_GENERAL, SG_ALERT, "property = " <<  property 
		    << "; length = " << strlen(property));

	    char _value[256] = {0};
	    if (strncmp(type, "bool", strlen(type)) == 0){
		bool val = fgGetBool(property, 0);
		sprintf(_value, "%d\n", val);
	    } else if (strncmp(type, "double", strlen(type)) == 0){
		double val = fgGetDouble(property, 0);
		sprintf(_value, "%.10f\n", val);
	    }

	    socket_snd->writestring(_value);
	    SG_LOG( SG_GENERAL, SG_ALERT, "called fgGetBool with argument: " << 
		    property << " and received: " << _value << "\n");
	
	    continue;
	}
	
	temp = strchr(next_token, ' ');
	if (temp == NULL) { 
	    SG_LOG( SG_GENERAL, SG_ALERT, "error finding end");
	    continue;
	}
	memcpy(property, next_token, temp - next_token);
	SG_LOG( SG_GENERAL, SG_ALERT, "property = " <<  property 
		<< "; length = " << strlen(property));

	temp += 1;

	memcpy(value, temp, strlen(temp));
	value[strlen(value) - 1] = '\0';  //to take care of the '\n'
	SG_LOG( SG_GENERAL, SG_ALERT, "value = " <<  value 
		<< "; length = " << strlen(value));
	
	if (strncmp(type, "bool", strlen(type)) == 0){
	    if (strncmp(value, "false", strlen(value)) == 0) {
		fgSetBool(property, 0);
	    } else 
		fgSetBool(property, 1);
	    SG_LOG( SG_GENERAL, SG_ALERT, "called fgSetBool with arguments: " << 
		    property << " " << value << "\n");
	} else if (strncmp(type, "double", strlen(type)) == 0){
	    fgSetDouble(property, atof(value));

	    SG_LOG( SG_GENERAL, SG_ALERT, "called fgSetDouble with arguments: " << 
		    property << " " << value << "\n");
	} else if (strncmp(type, "string", strlen(type)) == 0){
	    if (strncmp(value, "off",strlen(value)) == 0) {
		fgSetString(property, "");
                SG_LOG( SG_GENERAL, SG_ALERT, "called fgSetString with arguments: " <<
                    property << "  \n");
	
		
	    }
	    else{

	    fgSetString(property, value);
	    SG_LOG( SG_GENERAL, SG_ALERT, "called fgSetString with arguments: " << 
		    property << " " << value << "\n");
		}	
	}
    }
    SG_LOG( SG_GENERAL, SG_ALERT, "end of receive_thread. " << ret);
    return NULL;
    
}

/* 
   /controls/autoflight/autopilot[%d]/engage
   /controls/autoflight/autothrottle-arm
   /controls/autoflight/autothrottle-engage
   /controls/autoflight/heading-select
   /controls/autoflight/altitude-select
   /controls/autoflight/bank-angle-select
   /controls/autoflight/vertical-speed-select
   /controls/autoflight/speed-select
   /controls/autoflight/mach-select
   /controls/autoflight/vertical-mode
   /controls/autoflight/lateral-mode

   /autopilot/settings/heading-bug-deg     -> double
   /autopilot/settings/target-altitude-ft  -> double
   /autopilot/settings/target-aoa-deg      -> double
   /autopilot/settings/target-pitch-deg    -> double
   /autopilot/settings/target-roll-deg
   /autopilot/settings/target-speed-kt     -> double
   /autopilot/settings/vertical-speed-fpm  -> double

   /autopilot/locks/altitude   ->string
   /autopilot/locks/aoa       ->none  ?
   /autopilot/locks/heading    -> string
   /autopilot/locks/passive-mode   ->double
   /autopilot/locks/speed  -> string

   /controls/flight/aileron
   /controls/flight/aileron-trim
   /controls/flight/elevator
   /controls/flight/elevator-trim
   /controls/flight/rudder
   /controls/flight/rudder-trim
   /controls/flight/flaps
   /controls/flight/slats
   /controls/flight/spoilers
   /controls/flight/SAS-pitch
   /controls/flight/SAS-roll
   /controls/flight/SAS-yaw
   /controls/flight/auxFlaps
   /controls/flight/mainFlaps
*/

void *send_thread(void* t2) {
string ip = "127.0.0.1";
    string port_rcv = "12346";
    string port_snd = "12348";

    /* Open recv FGSocket */
    SG_LOG( SG_GENERAL, SG_ALERT, "Opening Receiver Socket" );
    SGSocketUDP *socket_rcv = new SGSocketUDP (ip, port_rcv);
    SGProtocolDir dir = SG_IO_IN;
    if (!socket_rcv->open(dir)){
	SG_LOG( SG_GENERAL, SG_ALERT, "can't open socket " << ip << ":" << port_rcv );
	return NULL;
    } else {
	SG_LOG( SG_GENERAL, SG_ALERT, "Socket open " << ip << ":" << port_rcv);
    }

    /* Open send FGSocket */
    SG_LOG( SG_GENERAL, SG_ALERT, "Opening Sender Socket" );
    SGSocketUDP *socket_snd = new SGSocketUDP (ip, port_snd);
    dir = SG_IO_OUT;
    if (!socket_snd->open(dir)){
	SG_LOG( SG_GENERAL, SG_ALERT, "can't open socket " << ip << ":" << port_snd );
	return NULL;
    } else {
	SG_LOG( SG_GENERAL, SG_ALERT, "Socket open " << ip << ":" << port_snd);
    }
    
    char buf[256];
    char func[20] = {0}; //either 'get' or 'set'
    char type[20] = {0};
    char property[256] = {0};
    char value[256];
    int ret;
    
    char *temp, *next_token;
    memset(buf, 0, sizeof(buf));

    while ( (ret = socket_rcv->readline( buf, sizeof(buf) )) >= 0 ) {
	memset(func, 0, sizeof(func));
	memset(type, 0, sizeof(type));
	memset(property, 0, sizeof(property));
	memset(value, 0, sizeof(value));
      
	temp = strchr(buf, ' ');
      
	if (temp == NULL){
	    SG_LOG( SG_GENERAL, SG_ALERT, "make sure you are sending a \'\\n\' character " << buf);
	    continue;
	}
      
	memcpy(func, buf, temp - buf);
	SG_LOG( SG_GENERAL, SG_ALERT, "func = " <<  func 
		<< "; length = " << strlen(func));
	if ((strncmp(func, "get", 3) != 0) && (strncmp(func,"set", 3) != 0)) {
	    SG_LOG( SG_GENERAL, SG_ALERT, "first token must be equal to either \"get\" or \"set\"... Ignoring.");
	    continue;
	}

	temp += 1;
	next_token = strchr(temp, ' ');
	if (next_token == NULL) { 
	    SG_LOG( SG_GENERAL, SG_ALERT, "error finding second space");
	    continue;
	}

	if ((next_token - temp) > sizeof(type)) {
	    SG_LOG( SG_GENERAL, SG_ALERT, "type (second argument) is too long\n");
	    continue;
	}
	
	memcpy(type, temp, next_token - temp);
	SG_LOG( SG_GENERAL, SG_ALERT, "type = " <<  type 
		<< "; length = " << strlen(type));

	next_token += 1;
	
	/* If the function of this call is a get, then get the 
	   value specifed and continue.
	*/
	if (strncmp(func, "get", 3) == 0) {
	    temp = strchr(next_token, '\0');
	    if (temp == NULL) { 
		SG_LOG( SG_GENERAL, SG_ALERT, "error finding end");
		continue;
	    }
	    memcpy(property, next_token, temp - next_token);
	    property[strlen(property) - 1] = '\0';  //to take care of the '\n'
	    SG_LOG( SG_GENERAL, SG_ALERT, "property = " <<  property 
		    << "; length = " << strlen(property));

	    char _value[256] = {0};
	    if (strncmp(type, "bool", strlen(type)) == 0){
		bool val = fgGetBool(property, 0);
		sprintf(_value, "%d\n", val);
	    } else if (strncmp(type, "double", strlen(type)) == 0){
		double val = fgGetDouble(property, 0);
		sprintf(_value, "%.10f\n", val);
	    }

	    socket_snd->writestring(_value);
	    SG_LOG( SG_GENERAL, SG_ALERT, "called fgGetBool with argument: " << 
		    property << " and received: " << _value << "\n");
	
	    continue;
	}
	
	temp = strchr(next_token, ' ');
	if (temp == NULL) { 
	    SG_LOG( SG_GENERAL, SG_ALERT, "error finding end");
	    continue;
	}
	memcpy(property, next_token, temp - next_token);
	SG_LOG( SG_GENERAL, SG_ALERT, "property = " <<  property 
		<< "; length = " << strlen(property));

	temp += 1;

	memcpy(value, temp, strlen(temp));
	value[strlen(value) - 1] = '\0';  //to take care of the '\n'
	SG_LOG( SG_GENERAL, SG_ALERT, "value = " <<  value 
		<< "; length = " << strlen(value));
	
	if (strncmp(type, "bool", strlen(type)) == 0){
	    if (strncmp(value, "false", strlen(value)) == 0) {
		fgSetBool(property, 0);
	    } else 
		fgSetBool(property, 1);
	    SG_LOG( SG_GENERAL, SG_ALERT, "called fgSetBool with arguments: " << 
		    property << " " << value << "\n");
	} else if (strncmp(type, "double", strlen(type)) == 0){
	    fgSetDouble(property, atof(value));

	    SG_LOG( SG_GENERAL, SG_ALERT, "called fgSetDouble with arguments: " << 
		    property << " " << value << "\n");
	} else if (strncmp(type, "string", strlen(type)) == 0){
	    if (strncmp(value, "off",strlen(value)) == 0) {
		fgSetString(property, "");
                SG_LOG( SG_GENERAL, SG_ALERT, "called fgSetString with arguments: " <<
                    property << "  \n");
	
		
	    }
	    else{

	    fgSetString(property, value);
	    SG_LOG( SG_GENERAL, SG_ALERT, "called fgSetString with arguments: " << 
		    property << " " << value << "\n");
		}	
	}
    }
    SG_LOG( SG_GENERAL, SG_ALERT, "end of receive_thread. " << ret);
    return NULL;
    }
