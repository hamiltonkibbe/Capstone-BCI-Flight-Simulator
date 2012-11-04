#include "Wfilter/FilteredChannelData.h"
#include "SystemParameters.h"
#include <assert.h>

FilteredChannelData::FilteredChannelData(ChannelData * _data, eeg_filter * _filter)
	: filter(_filter), data(_data->getRawData()), channelCount(_data->getNumberOfChannels()),
	samplesPerChannel(_data->getSamplesPerChannel())
{

	this->isDemuxed = false;
	this->reallocChannels = true;
	this->isPlotted = false;


}

//-----------------------------------------------------------------------------


FilteredChannelData::~FilteredChannelData() {


}

//-----------------------------------------------------------------------------


const void* FilteredChannelData::getRawData() const {

	return this->data;

}

//-----------------------------------------------------------------------------

unsigned FilteredChannelData::getNumberOfChannels() const {

	return this->channelCount;

}

//-----------------------------------------------------------------------------

unsigned FilteredChannelData::getSamplesPerChannel() const {

	return this->samplesPerChannel;

}

//-----------------------------------------------------------------------------

void FilteredChannelData::setRawData(void* argData) {

	this->data = argData;
	this->isDemuxed = false;
	this->isPlotted = false;

}

//-----------------------------------------------------------------------------

void FilteredChannelData::updateData() {

	this->isDemuxed = false;
	this->isPlotted = false;
}

//-----------------------------------------------------------------------------

void FilteredChannelData::setNumberOfChannels(unsigned int nChannels) {

	this->channelCount = nChannels;
	this->reallocChannels = true;




}

//-----------------------------------------------------------------------------

void FilteredChannelData::setSamplesPerChannel(unsigned int nSamples) {

	this->samplesPerChannel = nSamples;
	this->reallocChannels = true;

}


//-----------------------------------------------------------------------------

void FilteredChannelData::reallocChannelBuffers() {

	assert(this->channelCount!=0);
	assert(this->samplesPerChannel!=0);

	if(channels.size()!=0) {

		//Free old buffers

		while(!(this->channels.empty())) {

			delete this->channels.back();
			this->channels.pop_back();
		}

	}

	for(unsigned int i=0;i<this->channelCount;++i) {

		float* channelData = new float[this->samplesPerChannel];
		channels.push_back(channelData);
	}

	this->isDemuxed = false;
	this->isPlotted = false;
}

//-----------------------------------------------------------------------------

void FilteredChannelData::demuxData() {

	float* float_data = (float*)(this->data);

	//Deinterleave & filter everything except trigger channel
	for(int i = 0; i != NUMBER_OF_CHANNELS; ++i)
	{
		interleaved_iterator data_it(float_data + i, this->channelCount);
		cconv(data_it, this->samplesPerChannel, filter->filter(i), filter->filter_size(), channels[i], this->samplesPerChannel);
	}

    for(unsigned int i = NUMBER_OF_CHANNELS; i != this->channelCount; ++i)
    {
        //Deinterleave trigger channels
        interleaved_iterator trigger_begin(float_data + i, this->channelCount), trigger_end = trigger_begin + this->samplesPerChannel;
        std::copy(trigger_begin, trigger_end, channels[i]);
    }


	this->isDemuxed = true;
}


//-----------------------------------------------------------------------------
const void* FilteredChannelData::getDataForChannel(unsigned int channel)
{

	//Transform from 1-based channel number to zero-based index
	channel--;

	assert(this->data!=NULL);
	assert(this->channelCount!=0);
	assert(this->samplesPerChannel!=0);

	if(channel>this->channelCount)
		return NULL;

	if(reallocChannels)
		this->reallocChannelBuffers();

	if(!(this->isDemuxed))
		this->demuxData();

	return this->channels[channel];
}
