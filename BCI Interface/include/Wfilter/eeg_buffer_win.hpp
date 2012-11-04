#ifndef EEG_BUFFER_W_HPP
#define EEG_BUFFER_W_HPP
#include "eeg_buffer.hpp"
#include <cmath>
#include <utility>

class eeg_buffer_windowed : public eeg_buffer
{
protected:
	unsigned int nblocks_win, blocks_collected, nblocks_max;
	std::vector<double*> windows;
public:

	/*constuctor parameter description
	SignalSource* _source - the source for data eeg_buffer_windowed will use to get background data
	SystemParameters* _sysParams - params such as block size
	unsigned int nblocks_window - number of blocks in a window
	unsigned int _size - total maximum storage for each channel (in number of samples)
	const double* channel_array - a double array full of the 1-indexed channel numbers for eample {1, 3, 7}
	ch_size_type num_channels - size of channel numbers array
								num_channels should be 3 with channel_array = {1, 3, 7}
	*/
	eeg_buffer_windowed(SignalSource* _source, SystemParameters* _sysParams, unsigned int nblocks_window, unsigned int _size, const int* channel_array, ch_size_type num_channels)
		: eeg_buffer(_source,_sysParams, _size, channel_array, num_channels), nblocks_win(nblocks_window), blocks_collected(0)
	{
		windows = end_markers;
		nblocks_max = _size / _sysParams->getBlockSize();
	}

	virtual ~eeg_buffer_windowed() {}

	void reset()
	{
		end_markers = data;
		windows = data;
		blocks_collected = 0;
	}

	//0 based storage indexing for begining markers
	double* window(ch_size_type index) { return windows[index]; }
	std::vector<double*>::const_iterator window_begins() const { return windows.begin(); }

	//1 indexed version
	double* window_for_channel(int channel)
	{
		int index = std::find(channels.begin(), channels.end(), channel) - channels.begin();
		return windows[index];
	}

	//operator[] access from begining of window
	//different behavior from base class
	double* operator[](ch_size_type index) { return windows[index]; }

	//full() returns
	bool full() const { return blocks_collected >= nblocks_win; }

	void fill_window() //makes sure there is a full window of data
	{
		if(!full())
		{
			int remaining = nblocks_win - blocks_collected;
			collect(remaining);
		}
	}

	long long window_size() const //returns size of data in window
	{
		if(full())
			return  nblocks_win * sysParams->getBlockSize();
		else
			return blocks_collected * sysParams->getBlockSize();
	}

	void set_system_parameters(SystemParameters* argParams)
	{

		if(sysParams)
		{
			if(argParams->getBlockSize() != sysParams->getBlockSize())
			{
				nblocks_max = max_size /  argParams->getBlockSize();
				if(blocks_collected != 0) //uhhh just don't change sys params unless you know what you're doing, okay?
					blocks_collected =  ceil(static_cast<double>((sysParams->getBlockSize() * blocks_collected))/argParams->getBlockSize());
			}
		}
		sysParams = argParams;

	}

	virtual void collect(unsigned int num_blocks);
	virtual void update();//collect 1 block
};

#endif
