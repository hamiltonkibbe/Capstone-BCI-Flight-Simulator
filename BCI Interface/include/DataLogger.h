/*=====================================================================

 DataLogger

 Records EEG Data to a file


 Copyright (C) 2010 Hamilton Kibbe
 This code is made available under the MIT License.
 <http://www.opensource.org/licenses/mit-license.html>

=====================================================================*/

#ifndef __AUTOTESTER_H__
#define __AUTOTESTER_H__


#include <cmath>
#include <fstream>
#include <glade/glade.h>
#include "Alert.h"
#include "SignalSource.h"
#include "SystemParameters.h"
#include "TestDef.h"
#include "UsbAmpSource.h"





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// DataLoger Class
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class DataLogger
{
public:

	SignalSource* source;

	SystemParameters* sysParams;

	testDef* test;

	int getIterationsInDuration(double duration);

//public:

	DataLogger();

	~DataLogger();

	void setSignalSource(SignalSource* src) { source = src; }

	void setSystemParameters(SystemParameters* params) {sysParams = params; }

	void setTest(testDef* thetest) {test = thetest;}

	void runTest();


};

#endif
