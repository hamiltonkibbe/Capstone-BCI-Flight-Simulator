#ifndef __SYSTEM_PARAMETERS_H__
#define __SYSTEM_PARAMETERS_H__

#include <glade/glade.h>
#include <stdlib.h>
#include <vector>


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Macros
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define BLINK_THRESHOLD     40
#define DEFAULT_BLOCKSIZE   32
#define DEFAULT_FS          600
#define DEFAULT_CP          12346
#define DEFAULT_DP          12348
#define DETECTION_PERIODS   2
#define NUMBER_OF_SEQUENCES 30
#define PERIODS_TO_AVERAGE  120
#define SEQUENCE_LENGTH     31
#define NUMBER_OF_CHANNELS  1
#define BLINK_CHANNEL       (NUMBER_OF_CHANNELS + 1)
#define TRIGGER_CHANNEL     (NUMBER_OF_CHANNELS + 2)




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// SystemParameters Class
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace std;
class SystemParameters {

private:

	int samplingRate;

	int blockSize;

	int channelCount;

	int Fs;

	int bit_rate;

public:
	void setFs(int f) { Fs = f; }

	void setBitRate(int r) { bit_rate = r; }

	void setSamplingRate(int argRate) { samplingRate = argRate; }

	void setBlockSize(int argSize) { blockSize = argSize; }

	void setChannelCount(int argCount) { channelCount = argCount; }

	int getFs(void) { return Fs; }

	int getBitRate(void) { return bit_rate; }

	int getSamplingRate() const { return samplingRate; }

	int getBlockSize() const { return blockSize; }

	int getChannelCount() const { return channelCount; }
};


#endif
