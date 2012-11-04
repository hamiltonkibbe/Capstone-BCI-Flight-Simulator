#ifndef eeg_whitening_filter_HPP
#define eeg_whitening_filter_HPP

#include <algorithm>
#include <fstream>
#include <iterator>
#include "csv_vv.hpp"
#include "eeg_buffer_win.hpp"
#include "whitening_filter.hpp"
#include "eeg_filter.hpp"
#include "../Alert.h"
#include "UsbAmpSource.h"
#include "GUI.h"
#include <vector>

using namespace std;

template <typename T, typename RowIt, typename ColSize>
class vv_row_iterator
{
	RowIt row;
	ColSize col;
public:
	vv_row_iterator(RowIt _row, ColSize _col)
		:row(_row), col(_col)
	{
	}
	vv_row_iterator(const vv_row_iterator& arg)
		: row(arg.row), col(arg.col)
	{
	}

	vv_row_iterator & operator=(const vv_row_iterator& rhs)
	{
		row = rhs.row;
		col = rhs.col;
	}

	T& operator*() const
	{
		return (*row)[col];
	}

	vv_row_iterator & operator++()
	{
		++row;
		return *this;
	}
	vv_row_iterator & operator--()
	{
		--row;
		return *this;
	}

	vv_row_iterator operator++(int)
	{
		vv_row_iterator temp = *this;
		++row;
		return temp;
	}
	vv_row_iterator operator--(int)
	{
		vv_row_iterator temp = *this;
		--row;
		return temp;
	}
};

class eeg_whitening_filter : public eeg_filter
{
private:
	std::vector<whitening_filter*> wf;
	std::vector<int> channel_numbers;
public:
	eeg_whitening_filter(SignalSource* _source, SystemParameters* _sysParams, unsigned int background_size, unsigned int filter_size,
		const int* channel_array, unsigned int num_channels)
			: channel_numbers(channel_array, channel_array + num_channels)
	{
		eeg_buffer_windowed buf(_source, _sysParams, 1, background_size + _sysParams->getBlockSize(), channel_array, num_channels);
		buf.collect(background_size/_sysParams->getBlockSize());
		for(unsigned int i = 0; i < num_channels; ++i)
		{
			wf.push_back(new whitening_filter((buf.data_begins())[i], background_size, filter_size));
		}
	}

	//get whiting filter impulse responses from file
	eeg_whitening_filter(const char * file_name);

	~eeg_whitening_filter()
	{
		for(unsigned int i = 0; i < wf.size(); ++i)
		{
			delete (wf[i]);
		}
	}

	const double* filter_for_channel(int ch) const
	{
		long long index =
			std::find(channel_numbers.begin(), channel_numbers.end(), ch) - channel_numbers.begin();
		return wf[index]->begin();
	}

	const double* filter(int index) const { return wf[index]->begin(); }

	virtual int n_filters() const { return wf.size(); }

	int filter_size() const
	{
		if(wf.empty())
			return 0;
		else
			return  wf[0]->size();
	}

	int channel_number(int index) const
	{
		return channel_numbers[index];
	}

	void save(const char* file_name);
	void load(const char* file_name);

};


eeg_whitening_filter* new_whitening_filter(SignalSource* src, SystemParameters * sysP, int downsample_factor);

#endif
