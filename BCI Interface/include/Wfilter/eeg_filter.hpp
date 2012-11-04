/* eeg_filter : base class for collection of filters that correspond to eeg channels */

#ifndef EEG_FILTER_HPP
#define EEG_FILTER_HPP

class eeg_filter
{
public:
	virtual const double* filter_for_channel(int ch) const = 0;
	virtual const double* filter(int index) const= 0;
	virtual int n_filters() const = 0;
	virtual int filter_size() const = 0;
	virtual int channel_number(int index) const = 0;
};
#endif