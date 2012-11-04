/*=====================================================================

 Detector

 Detects binary sequence in EEG data

 Copyright (C) 2010 Hamilton Kibbe
 This code is made available under the MIT License.
 <http://www.opensource.org/licenses/mit-license.html>

=====================================================================*/

#ifndef __DETECTOR_H__
#define __DETECTOR_H__



#include <assert.h>
#include <cmath>
#include <fstream>
#include <glade/glade.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

// Project-specific
#include "Alert.h"
#include "Commands.h"
#include "EEG_DEv_Filter.h"
#include "SignalSource.h"
#include "SystemParameters.h"
#include "UsbAmpSource.h"
#include "Sequence_Wrapper.h"
#include "DSP/circular_buffer.h"
#include "Wfilter/FilteredChannelData.h"
#include "Wfilter/eeg_whitening_filter.hpp"


typedef struct
{

	unsigned int sequence;

	long double max;

	double confidence;

} detector_vote;



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Detector Class
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class Detector
{
private:


	SignalSource * source;

	SystemParameters * sysParams;

	Sequence_Wrapper * sequences;

	eeg_filter * filter;

	Commands *fg_interface;

	circular_buffer<float> buffers[NUMBER_OF_CHANNELS];

	detector_vote detect_sequence(float* data, int channel);

	int find_trigger_index(float* data);

	void updateGUI(void);


public:

	Detector();

	~Detector();

	void setSequences(Sequence_Wrapper *  argSequences);

	void setSignalSource(SignalSource * src)	{ source = src; }

	void setSystemParameters(SystemParameters * params) { sysParams = params; }

    void set_filter(eeg_filter * _filter) { filter = _filter;}

	eeg_filter * get_filter() { return filter; }

	void run(void);

	vector< vector<float> > get_one_period(void);

    void detected(int sequence);

};


#endif



