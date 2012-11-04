#ifndef SIGNAL_SOURCE_H__
#define SIGNAL_SOURCE_H__


// ====================================================================
// Includes
// ====================================================================
#include "ChannelData.h"


// ====================================================================
// Class Declaration
// ====================================================================
class SignalSource {
	
public:
	
	SignalSource() {}
	~SignalSource() {}
	
	virtual int getSamplingRate()=0;
	virtual int getBlockSize()=0;
	virtual int getNumberOfChannels()=0;
	virtual ChannelData* getData()=0;

};

#endif
