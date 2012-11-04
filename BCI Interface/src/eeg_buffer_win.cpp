#include "Wfilter/eeg_buffer_win.hpp"

void eeg_buffer_windowed::collect(unsigned int num_blocks)
{
	using namespace std;
	unsigned int block_size = sysParams->getBlockSize();

	//if after we get one, we will have atl least one extra
	//then full_window should be true
	//and we need to adjust the start of the window
	bool full_window = blocks_collected  >= nblocks_win;

	//make sure there's room to collect all those samples
	if(blocks_collected >= nblocks_max - num_blocks)
	{
		return;
	}

	//main loop
	unsigned int last_block = blocks_collected + num_blocks;
	for(; blocks_collected < last_block; ++blocks_collected)
	{
		channelData = source->getData();
		//copy data to buffer
		for(unsigned int i = 0; i < channels.size(); ++i)
		{
			
			sample* channel_data = reinterpret_cast<sample*>(channelData->getDataForChannel(channels[i]));
			process_block();
			copy(channel_data, channel_data + block_size, end_markers[i]);
			end_markers[i] += block_size;

			//adjust begining of window
			if(full_window)
			{
				windows[i] += block_size;//do this most of the time
			}
			else
			{
				//don't adjust the beginings of the windows
				//are we going to have a full window after the next block?
				if(blocks_collected >= nblocks_win)
				{
					full_window = true;
				}
			}
			process_data();
		}
	}
}

void eeg_buffer_windowed::update()
{
	unsigned int block_size = sysParams->getBlockSize();

	//if after we get one, we will have atl least one extra
	//then full_window should be true
	//and we need to adjust the start of the window
	bool full_window = blocks_collected  >= nblocks_win;

	//make sure there's room to collect all those samples
	if(blocks_collected >= nblocks_max - 1)
	{
		return;
	}

	channelData = source->getData();
	//copy data to buffer
	for(unsigned int i = 0; i < channels.size(); ++i)
	{
		
		sample* channel_data = reinterpret_cast<sample*>(channelData->getDataForChannel(channels[i]));
		process_block();

		copy(channel_data, channel_data + block_size, end_markers[i]);//copy to buffer
		end_markers[i] += block_size;//adjust markers for end of the buffer

		//adjust begining of window
		if(full_window)
		{
			windows[i] += block_size;//do this most of the time
		}
		process_data();
	}

	++blocks_collected;
}
