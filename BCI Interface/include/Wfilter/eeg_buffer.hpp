#ifndef EEG_BUFFER_HPP
#define EEG_BUFFER_HPP

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Project-specific
#include "ChannelData.h"
#include "SignalSource.h"
#include "SystemParameters.h"


class eeg_buffer
{
public:
	typedef const float sample;
	typedef std::vector<double*>::size_type ch_size_type;

protected:	//protected so derived class may modify
	void (*proc)(eeg_buffer*);
	unsigned int max_size;
	std::vector<double*> data;
	std::vector<double*> end_markers;
	std::vector<ch_size_type> channels;

    ChannelData* channelData;
	SignalSource * source;
	SystemParameters * sysParams;

public:
	/*constuctor parameter description
	SignalSource* _source - the source for data eeg_buffer_windowed will use to get background data
	SystemParameters* _sysParams - params such as block size
	unsigned int _size - total maximum storage for each channel (in number of samples)
	const double* channel_array - a double array full of the 1-indexed channel numbers for eample {1, 3, 7}
	ch_size_type num_channels - size of channel numbers array
								num_channels should be 3 with channel_array = {1, 3, 7}
	*/
	eeg_buffer(SignalSource* _source, SystemParameters* _sysParams, unsigned int _size, const int* channel_array, ch_size_type num_channels)
		:proc(0),max_size(_size), channels(channel_array, channel_array + num_channels), channelData(0), source(_source),
		sysParams(_sysParams)
	{
		data.reserve(num_channels);
		for(unsigned int i = 0; i < num_channels; ++i)
		{
			data.push_back(new double[max_size]);
		}
		end_markers = data;
	}

	virtual ~eeg_buffer() {}

	std::vector<double*>::const_iterator data_begins() const { return data.begin(); }

	std::vector<double*>::const_iterator data_ends() const { return end_markers.begin(); }

	double* data_for_channel(int channel)
	{
		long long index = std::find(channels.begin(), channels.end(), channel) - channels.begin();
		return data[index];
	}

	double* end_for_channel(int channel)
	{
		long long index = std::find(channels.begin(), channels.end(), channel) - channels.begin();
		return end_markers[index];
	}

	double* operator[](ch_size_type index) { return data[index]; }

	long long num_channels() { return channels.size();}

	long long size() const { return end_markers[0] - data[0]; }

	unsigned int block_size() const
	{
		return sysParams->getBlockSize();
	}

	void reset()
	{
		end_markers = data;
	}

	SignalSource * get_source() { return source;}
	void set_source(SignalSource * arg) { source = arg; }

	SystemParameters * get_sysParams() { return sysParams;}
	void set_system_parameters(SystemParameters* argParams) { sysParams = argParams; }

	void set_callback(void (*_proc)(eeg_buffer *))
	{
		proc = _proc;
	}

	virtual void collect(unsigned int num_blocks);
	virtual void update();//collect 1 block0

	virtual void process_block();
	virtual void process_data();
};

#endif
