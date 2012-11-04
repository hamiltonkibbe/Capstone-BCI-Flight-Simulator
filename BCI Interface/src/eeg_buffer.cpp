#include "Wfilter/eeg_buffer.hpp"

void eeg_buffer::collect(unsigned int num_blocks)
{
	using namespace std;
	unsigned int block_size = sysParams->getBlockSize(), size_to_collect = num_blocks * block_size;

	//make sure there's room to collect all those samples
	if(size_to_collect > max_size - (end_markers[0] - data[0]))
	{
		return;
	}

	//main loop
	for(unsigned int blocks = 0; blocks < num_blocks; ++blocks)
	{
		channelData = source->getData();
		process_block();
		//copy data to buffer
		for(unsigned int i = 0; i < channels.size(); ++i)
		{

			sample* channel_data = reinterpret_cast<sample*>(channelData->getDataForChannel(channels[i]));
			copy(channel_data, channel_data + block_size, end_markers[i]);
			end_markers[i] += block_size;
		}
		process_data();
	}
}

void eeg_buffer::update()
{
	this->collect(1);
}

void eeg_buffer::process_block()
{
	//filtering applied just one block goes here
	//as do algorithms which want the latest block seperate
}

void eeg_buffer::process_data()
{
	//after all the new blocks are added to the buffer, do this
	if(proc)
	{
		proc(this);
	}
}
