#ifndef __USB_AMP_SOURCE_H__
#define __USB_AMP_SOURCE_H__


// ====================================================================
// Includes
// ====================================================================

// Libraries
#include <bioamp/AmpLink.h>
#include <gtk/gtk.h>
#include <glade/glade.h>
#include <assert.h>
#include <stdexcept>

// Project-specific
#include "SignalSource.h"
#include "InterleavedChannelData.h"
#include "SystemParameters.h"

// ====================================================================
// Class Declaration
// ====================================================================

class UsbAmpSource : public SignalSource {

private:

	AmpLink amp;

	InterleavedChannelData cd;

	int blockSize;

	bool isConnected;

	bool triggerLine;

	int samplingRate;

	unsigned bufferLength;

	float* buffer;

public:

	UsbAmpSource();

	~UsbAmpSource();

	int getSamplingRate();

	int getBlockSize();

	int getNumberOfChannels();

	ChannelData* getData();

	bool connect();

	void disconnect();

	void setSamplingRate(unsigned int argRate);

	void setBlockSize(unsigned int argSize);

	void start();

	void stop();

	void setActiveChannels(char* channels,unsigned int count);

	void setTriggerLineEnabled(bool isEnabled);

	void setBandpass(float low, float hight, float Fs);

	void setNotch(float low, float hight, float Fs);

	bool getTriggerLineEnabled(void);


};

// ====================================================================
// Support Function Prototypes
// ====================================================================



/*---------------------------------------------------------------------
/	 amp_is_connected()
/	 returns true if a g.USB amp is connected.
/--------------------------------------------------------------------*/
bool amp_is_connected();
SignalSource* createUsbAmpSource(const SystemParameters * sysParams);
SignalSource* createUsbAmpSource(const SystemParameters * sysParams, int channels);
int check_amp_timeout(void* data);

#endif
