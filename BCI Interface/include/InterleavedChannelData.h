#ifndef _INTERLEAVEDCHANNELDATA_H
#define	_INTERLEAVEDCHANNELDATA_H


// ====================================================================
// Includes
// ====================================================================

// Libraries
#include <vector>
#include <assert.h>

// Project-specific
#include "ChannelData.h"

// ====================================================================
// Class Declaration
// ====================================================================
using namespace std;
class InterleavedChannelData : public ChannelData {

protected:

    void* data;

    unsigned int channelCount;
    unsigned int samplesPerChannel;
	unsigned int totalSamples;

    bool isDemuxed;
	bool reallocChannels;

    vector<float*> channels;

    void reallocChannelBuffers();
    void demuxData();

public:

    InterleavedChannelData();

    virtual ~InterleavedChannelData();

    const void* getRawData() const;

    unsigned getNumberOfChannels() const;

    unsigned getSamplesPerChannel() const;

    const void* getDataForChannel(unsigned int channel);

    void setRawData(void* argData);

    void setNumberOfChannels(unsigned int nChannels);

    void setSamplesPerChannel(unsigned int nSamples);

	void updateData();


};


#endif
