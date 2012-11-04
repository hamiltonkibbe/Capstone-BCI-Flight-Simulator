#ifndef TESTDEF_H_
#define TESTDEF_H_



#include <iostream>
#include <string>
#include "SystemParameters.h"


// ====================================================================
// Class Declaration
// ====================================================================
class testDef
{
private:

	double duration;

	string testName;

	string filename;

	SystemParameters* sysParams;

	void setBlockSize(int size) { sysParams->setBlockSize(size);}

	void setSamplingRate(int rate) { sysParams->setSamplingRate(rate); }


public:

	testDef(void);

	~testDef(void);

	void setTestName(string name);

	void setTestDuration(double argdur) { duration = argdur; }

	void setSystemParameters(SystemParameters* argParams) { sysParams = argParams; }

	double getDuration() { return duration; }

	string getTestName() { return testName; }

	string getFileName() { return filename; }


};

testDef* createTest(SystemParameters* params, double duration);

#endif
