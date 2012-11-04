#ifndef FILTERED_CHANNEL_DATA_HPP
#define FILTERED_CHANNEL_DATA_HPP
#include "ChannelData.h"
#include "eeg_filter.hpp"
#include "cconv.hpp"
#include <vector>
#include <algorithm>
#include <iterator>

class FilteredChannelData : public ChannelData
{
	protected:

	eeg_filter * filter;
    const void* data;

    unsigned int channelCount;
    unsigned int samplesPerChannel;

    bool isDemuxed;
	bool isPlotted;
	bool reallocChannels;

    std::vector<float*> channels;

    void reallocChannelBuffers();
    void demuxData();

public:

    FilteredChannelData(ChannelData * _data, eeg_filter * _filter);
    virtual ~FilteredChannelData();

    const void* getRawData() const;
    unsigned getNumberOfChannels() const;
    unsigned getSamplesPerChannel() const;

    const void* getDataForChannel(unsigned int channel);

    void setRawData(void* argData);
    void setNumberOfChannels(unsigned int nChannels);
    void setSamplesPerChannel(unsigned int nSamples);

	void updateData();

};



//Random Access Iterator for interleaved float data
class interleaved_iterator
{
private:

    float * p;
	int n_channels;

public:
	typedef std::random_access_iterator_tag iterator_category;
    typedef float        value_type;
    typedef int   difference_type;
    typedef float*          pointer;
    typedef float&      reference;


	interleaved_iterator(float * data, int number_of_channels)
		: p(data), n_channels(number_of_channels) {}

	interleaved_iterator(const interleaved_iterator& it)
		: p(it.p), n_channels(it.n_channels) {}

	interleaved_iterator & operator=(const interleaved_iterator& rhs)
	{
		p = rhs.p;
		n_channels = rhs.n_channels;
		return *this;
	}

	float& operator*() const
	{
		return *p;
	}

	float& operator[](int n)
	{
		return p[n * n_channels];
	}

	float * ptr()
	{
	    return p;
	}

	interleaved_iterator & operator++()
	{
		p += n_channels;
		return *this;
	}
	interleaved_iterator & operator--()
	{
		p -= n_channels;
		return *this;
	}

	interleaved_iterator operator++(int)
	{
		interleaved_iterator temp = *this;
		p += n_channels;
		return temp;
	}
	interleaved_iterator operator--(int)
	{
		interleaved_iterator temp = *this;
		p -= n_channels;
		return temp;
	}

	interleaved_iterator operator+(int n) const
	{
		return interleaved_iterator(p + n * n_channels, n_channels);
	}

	interleaved_iterator operator-(int n) const
	{
		return interleaved_iterator(p - n * n_channels, n_channels);
	}

	int operator-(const interleaved_iterator & rhs) const
	{
		return (p - rhs.p)/n_channels;
	}

	bool operator<(const interleaved_iterator & rhs)
	{
		return p < rhs.p;
	}

	bool operator<=(const interleaved_iterator & rhs)
	{
		return p <= rhs.p;
	}

	bool operator>(const interleaved_iterator & rhs)
	{
		return p > rhs.p;
	}

	bool operator>=(const interleaved_iterator & rhs)
	{
		return p >= rhs.p;
	}

	bool operator==(const interleaved_iterator & rhs) const
	{
		return p == rhs.p;
	}

	bool operator!=(const interleaved_iterator & rhs) const
	{
		return p != rhs.p;
	}
};

namespace std
{
template<> struct iterator_traits<interleaved_iterator>
{
    typedef interleaved_iterator::iterator_category iterator_category;
    typedef interleaved_iterator::value_type        value_type;
    typedef interleaved_iterator::difference_type   difference_type;
    typedef interleaved_iterator::pointer           pointer;
    typedef interleaved_iterator::reference         reference;
};
}


#endif
