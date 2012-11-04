/**********************************************************************

    UsbAmpSource.cpp
    g.USB amplifier source

 Copyright (C) 2011 Hamilton Kibbe
 This code is made available under the MIT License.
 <http://www.opensource.org/licenses/mit-license.html>

**********************************************************************/
#include "UsbAmpSource.h"


using namespace std;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	UsbAmpSource::UsbAmpSource
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
UsbAmpSource::UsbAmpSource()
{

	samplingRate = 0;
	blockSize = 0;

}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	UsbAmpSource::~UsbAmpSource
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
UsbAmpSource::~UsbAmpSource()
{


}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	UsbAmpSource::getSamplingRate
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int UsbAmpSource::getSamplingRate()
{

	return samplingRate;

}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	UsbAmpSource::getBlockSize
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int UsbAmpSource::getBlockSize()
{


	return blockSize;

}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	UsbAmpSource::getNumberOfChannels
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int UsbAmpSource::getNumberOfChannels()
{

	return cd.getNumberOfChannels();

}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	UsbAmpSource::getData
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ChannelData* UsbAmpSource::getData()
{

	amp.getData(buffer, bufferLength);
	cd.updateData();

	return &cd;

}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	UsbAmpSource::connect
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool UsbAmpSource::connect() {

	// Local variables
	bool success;

	amp.setVerbosityLevel(0);

	if(!amp.isConnected())
	{
		success = amp.connect();
	}
	else
	{
		success = 1;
	}
	return success;

}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	UsbAmpSource::disconnect
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void UsbAmpSource::disconnect() {

	assert(amp.isConnected());
	amp.disconnect();

}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	UsbAmpSource::setSamplingRate
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void UsbAmpSource::setSamplingRate(unsigned int argRate) {

	samplingRate = argRate;
	amp.setSamplingRate(samplingRate);

}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	UsbAmpSource::setBlockSize
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void UsbAmpSource::setBlockSize(unsigned int argSize) {

	blockSize = argSize;
	cd.setSamplesPerChannel(argSize);
	amp.setBufferSize(blockSize);

}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	UsbAmpSource::start
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void UsbAmpSource::start() {

	assert(samplingRate!=0);
	assert(blockSize!=0);
	assert(amp.isConnected());

	bufferLength = cd.getNumberOfChannels()*blockSize;

	//Convert to bytes
	bufferLength *= sizeof(float);

	buffer = new float[bufferLength];

	amp.setMode(Acquire);
	amp.start();

	cd.setRawData(buffer);
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	UsbAmpSource::stop
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void UsbAmpSource::stop() {

	delete [] buffer;
	amp.stop();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	UsbAmpSource::setActiveChannels
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void UsbAmpSource::setActiveChannels(char* channels,unsigned int count) {

	assert(amp.isConnected());
	amp.setActiveChannels(channels,count);

	if(triggerLine)
	{
		cd.setNumberOfChannels(count+1);
	}
	else
	{
		cd.setNumberOfChannels(count);
	}

}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	UsbAmpSource::setTriggerLineEnabled
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void UsbAmpSource::setTriggerLineEnabled(bool isEnabled) {

	amp.setTriggerLineEnabled(isEnabled);
	this->triggerLine = isEnabled;

}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	UsbAmpSource::setBandpass
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void UsbAmpSource::setBandpass(float low, float high, float Fs) {

	const FilterSpec* bandpassFilters = AmpLink::getBandpassFilters();

	for(int i=0;i<BANDPASS_FILTER_COUNT;++i) {

		//hf = Highpass Frequency = lower bound

		if( (bandpassFilters[i].hf == low) && (bandpassFilters[i].lf == high) && (bandpassFilters[i].sf == Fs)) {

			for(int j=1;j<=16;++j)
				amp.setBandpass(i, j);

			return;

		}

	}

	throw runtime_error("Bandpass filter not found.");

}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	UsbAmpSource::setNotch
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void UsbAmpSource::setNotch(float low, float high, float Fs) {

	const FilterSpec* notchFilters = AmpLink::getNotchFilters();

	for(int i=0;i<NOTCH_FILTER_COUNT;++i) {

		//hf = Highpass Frequency = lower bound

		if( (notchFilters[i].hf == low) && (notchFilters[i].lf == high) && (notchFilters[i].sf == Fs)) {

			for(int j=1;j<=16;++j)
				amp.setNotch(i, j);

			return;
		}

	}

	throw runtime_error("Notch filter not found.");

}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	UsbAmpSource::getTriggerLineEnabled
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool UsbAmpSource::getTriggerLineEnabled(void)
{
	return this->triggerLine;
}




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	amp_is_connected
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool amp_is_connected()
{
	bool connected = FALSE;

	AmpLink* testAmp = new AmpLink();

	testAmp->setVerbosityLevel(0);
  	connected = testAmp->connect();

	delete testAmp;


	return connected;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	createUsbAmpSource
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SignalSource * createUsbAmpSource(const SystemParameters * sysParams)
{
    // Local variables
    char activeChannels[] = { 1, 2, 3, };
    UsbAmpSource* src = new UsbAmpSource();

    // Connect to and set up g.USB amp
    src->connect();
    src->setSamplingRate(sysParams->getSamplingRate());
    src->setBlockSize(sysParams->getBlockSize());

    // Set up filters
    src->setBandpass(0.5, 60.0 , sysParams->getSamplingRate());
    src->setNotch(58, 62, sysParams->getSamplingRate());

    // Misc setting
    src->setTriggerLineEnabled(TRUE);
    src->setActiveChannels(activeChannels, 3);


    // Start the amp
    src->start();
    return src;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	createUsbAmpSource
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SignalSource * createUsbAmpSource(const SystemParameters * sysParams, int channels)
{
    // Local variables

	int i;
	char activeChannels[NUMBER_OF_CHANNELS+2];
	for (i = 0; i < channels+1; ++i){ activeChannels[i] = i+1; }

    UsbAmpSource* src = new UsbAmpSource();

    // Connect to and set up g.USB amp
    src->connect();
    src->setSamplingRate(sysParams->getSamplingRate());
    src->setBlockSize(sysParams->getBlockSize());

    // Set up filters
    src->setBandpass(0.5, 60.0 , sysParams->getSamplingRate());
    src->setNotch(58, 62, sysParams->getSamplingRate());

    // Misc setting
    src->setTriggerLineEnabled(TRUE);
    src->setActiveChannels(activeChannels, channels+1);


    // Start the amp
    src->start();



    return src;

}

