/*=====================================================================

 Calibrator

 Calibrates detectors for each user


 Copyright (C) 2010 Hamilton Kibbe
 This code is made available under the MIT License.
 <http://www.opensource.org/licenses/mit-license.html>

=====================================================================*/

#ifndef CALIBRATOR_H_
#define CALIBRATOR_H_

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <glade/glade.h>
#include <iostream>
#include <numeric>
#include <string>
#include "Alert.h"
#include "Sequence.h"
#include "Sequence_Wrapper.h"
#include "SystemParameters.h"
#include "Template.h"
#include "UsbAmpSource.h"
#include "Validator.h"
#include "Wfilter/eeg_whitening_filter.hpp"
#include "Wfilter/FilteredChannelData.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Calibrator Class
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class Calibrator
{
public:

	Calibrator();

	~Calibrator();

	void set_sequences(Sequence_Wrapper* seq) { sequences = seq; }

	void set_system_parameters(SystemParameters* argParams) { sysParams = argParams; }

	void set_source(SignalSource * arg) { source = arg; }

	void set_username(const char* name) { username = name; }

	void set_bit_rate(int br) { bit_rate = br; }

	void calibrate(void);

	void load_calibration_files(void);

	void write_calibration_file(int seq, vector< vector<float> > *temp);

	void set_filter(eeg_filter * _filter) { filter = _filter;}
	eeg_filter * get_filter(){return filter;}

private:

	int bit_rate;

	void xval(const char* fname);

	const char* username;

	SignalSource *source;

	Sequence_Wrapper *sequences;

	SystemParameters *sysParams;

	int find_trigger_index(float* data);

	vector< vector<float> > get_one_period(void);

	eeg_filter * filter;


};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Support
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool is_corrupt(vector< vector<float> > data);

void update_cal_progress_window(int count);

void print_stats(Sequence_Wrapper* sequences);

#endif
